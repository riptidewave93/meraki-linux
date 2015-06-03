/*
 *  linux/kernel/profile.c
 *  Simple profiling. Manages a direct-mapped profile hit count buffer,
 *  with configurable resolution, support for restricting the cpus on
 *  which profiling is done, and switching between cpu time and
 *  schedule() calls via kernel command line parameters passed at boot.
 *
 *  Scheduler profiling support, Arjan van de Ven and Ingo Molnar,
 *	Red Hat, July 2004
 *  Consolidation of architecture support code for profiling,
 *	William Irwin, Oracle, July 2004
 *  Amortized hit count accounting via per-cpu open-addressed hashtables
 *	to resolve timer interrupt livelocks, William Irwin, Oracle, 2004
 */

#include <linux/module.h>
#include <linux/profile.h>
#include <linux/bootmem.h>
#include <linux/notifier.h>
#include <linux/mm.h>
#include <linux/cpumask.h>
#include <linux/cpu.h>
#include <linux/highmem.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/kallsyms.h>
#include <asm/sections.h>
#include <asm/irq_regs.h>
#include <asm/ptrace.h>
#include <asm/current.h>
#include <asm/thread_info.h>
#include <linux/sched.h>

struct profile_hit {
	u32 pc, hits;
};
#define PROFILE_GRPSHIFT	3
#define PROFILE_GRPSZ		(1 << PROFILE_GRPSHIFT)
#define NR_PROFILE_HIT		(PAGE_SIZE/sizeof(struct profile_hit))
#define NR_PROFILE_GRP		(NR_PROFILE_HIT/PROFILE_GRPSZ)

/* Oprofile timer tick hook */
static int (*timer_hook)(struct pt_regs *) __read_mostly;
static int user_hits = 0;
static atomic_t unknown_kernel_hits = ATOMIC_INIT(0);
static atomic_t known_kernel_hits = ATOMIC_INIT(0);
static int nopid_hits = 0;
int profiler_enabled = 0;
static volatile int profiling_is_on = 1;

extern struct list_head modules;
static atomic_t *prof_buffer;
static unsigned long prof_len;
unsigned prof_shift;

int prof_on __read_mostly;
EXPORT_SYMBOL_GPL(prof_on);

static cpumask_var_t prof_cpu_mask;
#ifdef CONFIG_SMP_NOTREALLY
static DEFINE_PER_CPU(struct profile_hit *[2], cpu_profile_hits);
static DEFINE_PER_CPU(int, cpu_profile_flip);
static DEFINE_MUTEX(profile_flip_mutex);
#endif /* CONFIG_SMP */

int profile_setup(char *str)
{
	static char schedstr[] = "schedule";
	static char sleepstr[] = "sleep";
	static char kvmstr[] = "kvm";
	int par;

	if (!strncmp(str, sleepstr, strlen(sleepstr))) {
#ifdef CONFIG_SCHEDSTATS
		prof_on = SLEEP_PROFILING;
		if (str[strlen(sleepstr)] == ',')
			str += strlen(sleepstr) + 1;
		if (get_option(&str, &par))
			prof_shift = par;
		printk(KERN_INFO
			"kernel sleep profiling enabled (shift: %ld)\n",
			prof_shift);
#else
		printk(KERN_WARNING
			"kernel sleep profiling requires CONFIG_SCHEDSTATS\n");
#endif /* CONFIG_SCHEDSTATS */
	} else if (!strncmp(str, schedstr, strlen(schedstr))) {
		prof_on = SCHED_PROFILING;
		if (str[strlen(schedstr)] == ',')
			str += strlen(schedstr) + 1;
		if (get_option(&str, &par))
			prof_shift = par;
		printk(KERN_INFO
			"kernel schedule profiling enabled (shift: %d)\n",
			prof_shift);
	} else if (!strncmp(str, kvmstr, strlen(kvmstr))) {
		prof_on = KVM_PROFILING;
		if (str[strlen(kvmstr)] == ',')
			str += strlen(kvmstr) + 1;
		if (get_option(&str, &par))
			prof_shift = par;
		printk(KERN_INFO
			"kernel KVM profiling enabled (shift: %d)\n",
			prof_shift);
	} else if (get_option(&str, &par)) {
		prof_shift = par;
		prof_on = CPU_PROFILING;
		printk(KERN_INFO "kernel profiling enabled (shift: %d)\n",
			prof_shift);
	}
	return 1;
}
__setup("profile=", profile_setup);


int __ref profile_init(void)
{
	int buffer_bytes;

	// XXX turn on profiling
	prof_shift = 4;
	prof_on = CPU_PROFILING;

	if (!prof_on)
		return 0;

	/* only text is profiled */
	prof_len = (_etext - _stext) >> prof_shift;
	buffer_bytes = prof_len*sizeof(atomic_t);

	if (!alloc_cpumask_var(&prof_cpu_mask, GFP_KERNEL))
		return -ENOMEM;

	cpumask_copy(prof_cpu_mask, cpu_possible_mask);

	prof_buffer = kzalloc(buffer_bytes, GFP_KERNEL|__GFP_NOWARN);
	if (prof_buffer)
		return 0;

	prof_buffer = alloc_pages_exact(buffer_bytes,
					GFP_KERNEL|__GFP_ZERO|__GFP_NOWARN);
	if (prof_buffer)
		return 0;

	prof_buffer = vmalloc(buffer_bytes);
	if (prof_buffer) {
		memset(prof_buffer, 0, buffer_bytes);
		return 0;
	}

	free_cpumask_var(prof_cpu_mask);
	return -ENOMEM;
}

/* Profile event notifications */

static BLOCKING_NOTIFIER_HEAD(task_exit_notifier);
static ATOMIC_NOTIFIER_HEAD(task_free_notifier);
static BLOCKING_NOTIFIER_HEAD(munmap_notifier);

void profile_task_exit(struct task_struct *task)
{
	blocking_notifier_call_chain(&task_exit_notifier, 0, task);
}

int profile_handoff_task(struct task_struct *task)
{
	int ret;
	ret = atomic_notifier_call_chain(&task_free_notifier, 0, task);
	return (ret == NOTIFY_OK) ? 1 : 0;
}

void profile_munmap(unsigned long addr)
{
	blocking_notifier_call_chain(&munmap_notifier, 0, (void *)addr);
}

int task_handoff_register(struct notifier_block *n)
{
	return atomic_notifier_chain_register(&task_free_notifier, n);
}
EXPORT_SYMBOL_GPL(task_handoff_register);

int task_handoff_unregister(struct notifier_block *n)
{
	return atomic_notifier_chain_unregister(&task_free_notifier, n);
}
EXPORT_SYMBOL_GPL(task_handoff_unregister);

int profile_event_register(enum profile_type type, struct notifier_block *n)
{
	int err = -EINVAL;

	switch (type) {
	case PROFILE_TASK_EXIT:
		err = blocking_notifier_chain_register(
				&task_exit_notifier, n);
		break;
	case PROFILE_MUNMAP:
		err = blocking_notifier_chain_register(
				&munmap_notifier, n);
		break;
	}

	return err;
}
EXPORT_SYMBOL_GPL(profile_event_register);

int profile_event_unregister(enum profile_type type, struct notifier_block *n)
{
	int err = -EINVAL;

	switch (type) {
	case PROFILE_TASK_EXIT:
		err = blocking_notifier_chain_unregister(
				&task_exit_notifier, n);
		break;
	case PROFILE_MUNMAP:
		err = blocking_notifier_chain_unregister(
				&munmap_notifier, n);
		break;
	}

	return err;
}
EXPORT_SYMBOL_GPL(profile_event_unregister);

int register_timer_hook(int (*hook)(struct pt_regs *))
{
	if (timer_hook)
		return -EBUSY;
	timer_hook = hook;
	return 0;
}
EXPORT_SYMBOL_GPL(register_timer_hook);

void unregister_timer_hook(int (*hook)(struct pt_regs *))
{
	WARN_ON(hook != timer_hook);
	timer_hook = NULL;
	/* make sure all CPUs see the NULL hook */
	synchronize_sched();  /* Allow ongoing interrupts to complete. */
}
EXPORT_SYMBOL_GPL(unregister_timer_hook);

static inline int within(unsigned long addr, void *start, unsigned long size)
{
	return ((void *)addr >= start && (void *)addr < start + size);
}

#ifdef CONFIG_SMP_NOTREALLY
#error "Meraki has not added SMP profiling support yet"
/*
 * Each cpu has a pair of open-addressed hashtables for pending
 * profile hits. read_profile() IPI's all cpus to request them
 * to flip buffers and flushes their contents to prof_buffer itself.
 * Flip requests are serialized by the profile_flip_mutex. The sole
 * use of having a second hashtable is for avoiding cacheline
 * contention that would otherwise happen during flushes of pending
 * profile hits required for the accuracy of reported profile hits
 * and so resurrect the interrupt livelock issue.
 *
 * The open-addressed hashtables are indexed by profile buffer slot
 * and hold the number of pending hits to that profile buffer slot on
 * a cpu in an entry. When the hashtable overflows, all pending hits
 * are accounted to their corresponding profile buffer slots with
 * atomic_add() and the hashtable emptied. As numerous pending hits
 * may be accounted to a profile buffer slot in a hashtable entry,
 * this amortizes a number of atomic profile buffer increments likely
 * to be far larger than the number of entries in the hashtable,
 * particularly given that the number of distinct profile buffer
 * positions to which hits are accounted during short intervals (e.g.
 * several seconds) is usually very small. Exclusion from buffer
 * flipping is provided by interrupt disablement (note that for
 * SCHED_PROFILING or SLEEP_PROFILING profile_hit() may be called from
 * process context).
 * The hash function is meant to be lightweight as opposed to strong,
 * and was vaguely inspired by ppc64 firmware-supported inverted
 * pagetable hash functions, but uses a full hashtable full of finite
 * collision chains, not just pairs of them.
 *
 * -- wli
 */
static void __profile_flip_buffers(void *unused)
{
	int cpu = smp_processor_id();

	per_cpu(cpu_profile_flip, cpu) = !per_cpu(cpu_profile_flip, cpu);
}

static void profile_flip_buffers(void)
{
	int i, j, cpu;

	mutex_lock(&profile_flip_mutex);
	j = per_cpu(cpu_profile_flip, get_cpu());
	put_cpu();
	on_each_cpu(__profile_flip_buffers, NULL, 1);
	for_each_online_cpu(cpu) {
		struct profile_hit *hits = per_cpu(cpu_profile_hits, cpu)[j];
		for (i = 0; i < NR_PROFILE_HIT; ++i) {
			if (!hits[i].hits) {
				if (hits[i].pc)
					hits[i].pc = 0;
				continue;
			}
			atomic_add(hits[i].hits, &prof_buffer[hits[i].pc]);
			hits[i].hits = hits[i].pc = 0;
		}
	}
	mutex_unlock(&profile_flip_mutex);
}

static void profile_discard_flip_buffers(void)
{
	int i, cpu;

	mutex_lock(&profile_flip_mutex);
	i = per_cpu(cpu_profile_flip, get_cpu());
	put_cpu();
	on_each_cpu(__profile_flip_buffers, NULL, 1);
	for_each_online_cpu(cpu) {
		struct profile_hit *hits = per_cpu(cpu_profile_hits, cpu)[i];
		memset(hits, 0, NR_PROFILE_HIT*sizeof(struct profile_hit));
	}
	mutex_unlock(&profile_flip_mutex);
}

void profile_hits(int type, void *__pc, unsigned int nr_hits)
{
	unsigned long primary, secondary, flags, pc = (unsigned long)__pc;
	int i, j, cpu;
	struct profile_hit *hits;

	if (prof_on != type || !prof_buffer)
		return;
	pc = min((pc - (unsigned long)_stext) >> prof_shift, prof_len - 1);
	i = primary = (pc & (NR_PROFILE_GRP - 1)) << PROFILE_GRPSHIFT;
	secondary = (~(pc << 1) & (NR_PROFILE_GRP - 1)) << PROFILE_GRPSHIFT;
	cpu = get_cpu();
	hits = per_cpu(cpu_profile_hits, cpu)[per_cpu(cpu_profile_flip, cpu)];
	if (!hits) {
		put_cpu();
		return;
	}
	/*
	 * We buffer the global profiler buffer into a per-CPU
	 * queue and thus reduce the number of global (and possibly
	 * NUMA-alien) accesses. The write-queue is self-coalescing:
	 */
	local_irq_save(flags);
	do {
		for (j = 0; j < PROFILE_GRPSZ; ++j) {
			if (hits[i + j].pc == pc) {
				hits[i + j].hits += nr_hits;
				goto out;
			} else if (!hits[i + j].hits) {
				hits[i + j].pc = pc;
				hits[i + j].hits = nr_hits;
				goto out;
			}
		}
		i = (i + secondary) & (NR_PROFILE_HIT - 1);
	} while (i != primary);

	/*
	 * Add the current hit(s) and flush the write-queue out
	 * to the global buffer:
	 */
	atomic_add(nr_hits, &prof_buffer[pc]);
	for (i = 0; i < NR_PROFILE_HIT; ++i) {
		atomic_add(hits[i].hits, &prof_buffer[hits[i].pc]);
		hits[i].pc = hits[i].hits = 0;
	}
out:
	local_irq_restore(flags);
	put_cpu();
}

static int __cpuinit profile_cpu_callback(struct notifier_block *info,
					unsigned long action, void *__cpu)
{
	int node, cpu = (unsigned long)__cpu;
	struct page *page;

	switch (action) {
	case CPU_UP_PREPARE:
	case CPU_UP_PREPARE_FROZEN:
		node = cpu_to_node(cpu);
		per_cpu(cpu_profile_flip, cpu) = 0;
		if (!per_cpu(cpu_profile_hits, cpu)[1]) {
			page = alloc_pages_exact_node(node,
					GFP_KERNEL | __GFP_ZERO,
					0);
			if (!page)
				return NOTIFY_BAD;
			per_cpu(cpu_profile_hits, cpu)[1] = page_address(page);
		}
		if (!per_cpu(cpu_profile_hits, cpu)[0]) {
			page = alloc_pages_exact_node(node,
					GFP_KERNEL | __GFP_ZERO,
					0);
			if (!page)
				goto out_free;
			per_cpu(cpu_profile_hits, cpu)[0] = page_address(page);
		}
		break;
out_free:
		page = virt_to_page(per_cpu(cpu_profile_hits, cpu)[1]);
		per_cpu(cpu_profile_hits, cpu)[1] = NULL;
		__free_page(page);
		return NOTIFY_BAD;
	case CPU_ONLINE:
	case CPU_ONLINE_FROZEN:
		if (prof_cpu_mask != NULL)
			cpumask_set_cpu(cpu, prof_cpu_mask);
		break;
	case CPU_UP_CANCELED:
	case CPU_UP_CANCELED_FROZEN:
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
		if (prof_cpu_mask != NULL)
			cpumask_clear_cpu(cpu, prof_cpu_mask);
		if (per_cpu(cpu_profile_hits, cpu)[0]) {
			page = virt_to_page(per_cpu(cpu_profile_hits, cpu)[0]);
			per_cpu(cpu_profile_hits, cpu)[0] = NULL;
			__free_page(page);
		}
		if (per_cpu(cpu_profile_hits, cpu)[1]) {
			page = virt_to_page(per_cpu(cpu_profile_hits, cpu)[1]);
			per_cpu(cpu_profile_hits, cpu)[1] = NULL;
			__free_page(page);
		}
		break;
	}
	return NOTIFY_OK;
}
#else /* !CONFIG_SMP */
#define profile_flip_buffers()		do { } while (0)
#define profile_discard_flip_buffers()	do { } while (0)
#define profile_cpu_callback		NULL

void profile_hits(int type, void *__pc, unsigned int nr_hits)
{
	unsigned long pc;
	unsigned long pc_kernel_offset;
	struct module *mod = NULL;

	pc = (unsigned long) __pc;
	if (prof_on != type || !prof_buffer)
		return;
	pc_kernel_offset = (pc - (unsigned long)_stext) >> prof_shift;
	if (pc_kernel_offset < prof_len) {
		atomic_add(nr_hits, &prof_buffer[pc_kernel_offset]);
		atomic_add(nr_hits, &known_kernel_hits);
		return;
	}

	list_for_each_entry(mod, &modules, list)
		if (mod->prof_buffer &&
		    within(pc, mod->module_core, mod->core_text_size)) {
			int mod_offset = (pc - (unsigned long)mod->module_core) >> prof_shift;
			atomic_add(nr_hits, &mod->ticks);
			atomic_add(nr_hits, &mod->prof_buffer[mod_offset]);
			return;
		}

	atomic_add(nr_hits, &unknown_kernel_hits);
}
#endif /* !CONFIG_SMP */
EXPORT_SYMBOL_GPL(profile_hits);

void profile_tick(int type)
{
	struct pt_regs *regs;
	if (!profiler_enabled)
		return;

	regs = get_irq_regs();

	if (type == CPU_PROFILING && timer_hook)
		timer_hook(regs);

	if (!profiling_is_on)
		return;

	if (!current || !task_pid_nr(current))
		nopid_hits++;

	if (user_mode(regs))
		user_hits++;
	else if (prof_cpu_mask != NULL && cpumask_test_cpu(smp_processor_id(), prof_cpu_mask))
		profile_hit(type, (void *)profile_pc(regs));
}

#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

static int prof_cpu_mask_proc_show(struct seq_file *m, void *v)
{
	seq_cpumask(m, prof_cpu_mask);
	seq_putc(m, '\n');
	return 0;
}

static int prof_cpu_mask_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, prof_cpu_mask_proc_show, NULL);
}

static ssize_t prof_cpu_mask_proc_write(struct file *file,
	const char __user *buffer, size_t count, loff_t *pos)
{
	cpumask_var_t new_value;
	int err;

	if (!alloc_cpumask_var(&new_value, GFP_KERNEL))
		return -ENOMEM;

	err = cpumask_parse_user(buffer, count, new_value);
	if (!err) {
		cpumask_copy(prof_cpu_mask, new_value);
		err = count;
	}
	free_cpumask_var(new_value);
	return err;
}

static const struct file_operations prof_cpu_mask_proc_fops = {
	.open		= prof_cpu_mask_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= prof_cpu_mask_proc_write,
};

void create_prof_cpu_mask(struct proc_dir_entry *root_irq_dir)
{
	/* create /proc/irq/prof_cpu_mask */
	proc_create("prof_cpu_mask", 0600, root_irq_dir, &prof_cpu_mask_proc_fops);
}

static void
do_seq_profile(struct seq_file *m, unsigned long base_pc, atomic_t *buf, int count, const char *profname)
{
	int i;
	int lastcount = 0;
	int misc_counts = 0;
	int nosym_counts = 0;
	unsigned long pc = 0;
	char namebuf[KSYM_NAME_LEN+1];
	const char *last_sym_name = 0;
	unsigned long last_sym_pc = 0;
	unsigned long last_sym_size = 0;

	for (i = 0; i < count; i++)
		if (atomic_read(&buf[i])) {
			char *modname;
			unsigned long offset;
			pc = base_pc + (i << prof_shift);
			if (within(pc, (void *)last_sym_pc, last_sym_size)) {
				lastcount += atomic_read(&buf[i]);
			} else {
				if (lastcount > 5)
					seq_printf(m, "%08lx % 6d %s\n", pc, lastcount, last_sym_name);
				else
					misc_counts += lastcount;

				lastcount = 0;
				last_sym_name = kallsyms_lookup(pc, &last_sym_size, &offset, &modname, namebuf);
				if (!last_sym_name || last_sym_name[0] == '\0') {
					nosym_counts += atomic_read(&buf[i]);
					last_sym_pc = last_sym_size = 0;
				} else {
					last_sym_pc = pc - offset;
					lastcount = atomic_read(&buf[i]);
				}
			}
		}

	if (lastcount)
		seq_printf(m, "%08lx % 6d %s\n", pc, lastcount, last_sym_name);
	if (misc_counts)
		seq_printf(m, "00000000 % 6d misc_small_samples_%s\n", misc_counts, profname);
	if (nosym_counts)
		seq_printf(m, "00000000 % 6d nosym_samples_%s\n", nosym_counts, profname);
}

static int
show_profile(struct seq_file *m, void *v)
{
	struct module *mod;
	(void) v;
	if (!prof_buffer)
		return 1;
	profile_flip_buffers();

	profiling_is_on = 0;
	do_seq_profile(m, (unsigned long)_stext, prof_buffer, prof_len, "vmlinux");

	mutex_lock(&module_mutex);
	list_for_each_entry(mod, &modules, list) {
		if (mod->state == MODULE_STATE_LIVE && mod->prof_buffer) {
			do_seq_profile(m, (unsigned long)mod->module_core,
				       mod->prof_buffer, mod->core_text_size >> prof_shift, mod->name);
		}
	}

	mutex_unlock(&module_mutex);

	profiling_is_on = 1;

	return 0;
}

static int
show_profile_m(struct seq_file *m, void *v)
{
	struct module *mod;
	unsigned long total = 0;
	(void) v;

	seq_printf(m, "%d idle?\n\n", nopid_hits);
	seq_printf(m, "below should add up to 100%%\n");
	seq_printf(m, "%d userlevel\n", user_hits);
	seq_printf(m, "%d unknown_kernel_hits\n", atomic_read(&unknown_kernel_hits));
	seq_printf(m, "%d vmlinux\n", atomic_read(&known_kernel_hits));
	total = user_hits + atomic_read(&unknown_kernel_hits) + atomic_read(&known_kernel_hits);
	mutex_lock(&module_mutex);
	list_for_each_entry(mod, &modules, list) {
		if (mod->state != MODULE_STATE_LIVE) {
			continue;
		}
		total += atomic_read(&mod->ticks);
		seq_printf(m, "%d %s\n", atomic_read(&mod->ticks), mod->name);
	}

	seq_printf(m, "\n%lu total\n", total);
	mutex_unlock(&module_mutex);

	return 0;
}

static int open_profile(struct inode *inode, struct file *file)
{
	return single_open(file, show_profile, NULL);
}

static int open_profile_m(struct inode *inode, struct file *file)
{
	struct proc_dir_entry *dp = PDE(inode);
	return single_open(file, show_profile_m, dp->data);
}

/*
 * Writing to /proc/profile resets the counters
 *
 * Writing a 'profiling multiplier' value into it also re-sets the profiling
 * interrupt frequency, on architectures that support this.
 */
static ssize_t write_profile(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	struct module *mod = NULL;
	profiling_is_on = 0;
#ifdef CONFIG_SMP_NOTREALLY
	extern int setup_profiling_timer(unsigned int multiplier);

	if (count == sizeof(int)) {
		unsigned int multiplier;

		if (copy_from_user(&multiplier, buf, sizeof(int)))
			return -EFAULT;

		if (setup_profiling_timer(multiplier))
			return -EINVAL;
	}
#endif
	profile_discard_flip_buffers();
	memset(prof_buffer, 0, prof_len * sizeof(atomic_t));

	mutex_lock(&module_mutex);
	list_for_each_entry(mod, &modules, list) {
		atomic_set(&mod->ticks, 0);
		if (mod->prof_buffer)
			memset(mod->prof_buffer, 0,
			       (mod->core_text_size >> prof_shift) * sizeof(atomic_t));
	}
	mutex_unlock(&module_mutex);
	user_hits = nopid_hits = 0;
	atomic_set(&unknown_kernel_hits, 0);
	atomic_set(&known_kernel_hits, 0);

	profiling_is_on = 1;
	return count;
}

static const struct file_operations proc_profile_operations = {
	.open		= open_profile,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= write_profile,
};

static const struct file_operations proc_profile_m_operations = {
	.open		= open_profile_m,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= write_profile,
};

#ifdef CONFIG_SMP_NOTREALLY
static void profile_nop(void *unused)
{
}

static int create_hash_tables(void)
{
	int cpu;

	for_each_online_cpu(cpu) {
		int node = cpu_to_node(cpu);
		struct page *page;

		page = alloc_pages_exact_node(node,
				GFP_KERNEL | __GFP_ZERO | GFP_THISNODE,
				0);
		if (!page)
			goto out_cleanup;
		per_cpu(cpu_profile_hits, cpu)[1]
				= (struct profile_hit *)page_address(page);
		page = alloc_pages_exact_node(node,
				GFP_KERNEL | __GFP_ZERO | GFP_THISNODE,
				0);
		if (!page)
			goto out_cleanup;
		per_cpu(cpu_profile_hits, cpu)[0]
				= (struct profile_hit *)page_address(page);
	}
	return 0;
out_cleanup:
	prof_on = 0;
	smp_mb();
	on_each_cpu(profile_nop, NULL, 1);
	for_each_online_cpu(cpu) {
		struct page *page;

		if (per_cpu(cpu_profile_hits, cpu)[0]) {
			page = virt_to_page(per_cpu(cpu_profile_hits, cpu)[0]);
			per_cpu(cpu_profile_hits, cpu)[0] = NULL;
			__free_page(page);
		}
		if (per_cpu(cpu_profile_hits, cpu)[1]) {
			page = virt_to_page(per_cpu(cpu_profile_hits, cpu)[1]);
			per_cpu(cpu_profile_hits, cpu)[1] = NULL;
			__free_page(page);
		}
	}
	return -1;
}
#else
#define create_hash_tables()			({ 0; })
#endif

int __ref create_proc_profile(void) /* false positive from hotcpu_notifier */
{
	struct proc_dir_entry *entry;

	if (!prof_on)
		return 0;
	if (create_hash_tables())
		return -ENOMEM;
	entry = proc_create("profile", S_IWUSR | S_IRUGO,
			    NULL, &proc_profile_operations);
	if (!entry)
		return 0;

	entry = proc_create("profile_m", S_IWUSR | S_IRUGO,
			    NULL, &proc_profile_m_operations);
	if (!entry)
		return 0;

	hotcpu_notifier(profile_cpu_callback, 0);
	return 0;
}
module_init(create_proc_profile);
#endif /* CONFIG_PROC_FS */
