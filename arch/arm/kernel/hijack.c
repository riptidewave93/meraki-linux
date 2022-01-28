/* hijack.c
 * author: Cel Skeggs <cela@meraki.net>
 */
#include <linux/slab.h>
#include <linux/kallsyms.h>
#include <linux/ftrace.h>
#include <linux/module.h>
#include <linux/stop_machine.h>

#include <linux/hijack.h>
#include <asm/plat-hijack.h>

#include <asm/unwind.h>

#define NO_RETURN_HOOK ((exit_hook_t) 0)
#define THUNK_ASSOC_REGISTER 12 // ip
#define NORMAL_RETURN ((void *) 0)

#define ARM_PC_OFF (-8)
#define FRAME_PC_OFF (-4)
#define FRAME_SETUP_INIT (0xE1A0C00D)
#define FP_TO_FRAME(fp) (container_of(fp, struct hijack_stack_frame, saved_pc))

struct hijack_assoc {
    entry_hook_t entry_hook;
    exit_hook_t exit_hook;
};

// this structure is very precisely tied to both the exact trace_hijack_hook
// assembly code and the ARM ABI.
struct hijack_stack_frame {
    unsigned long reentry_pc;
    void *entry_data;
    exit_hook_t exit_cb;

    unsigned long saved_fp;
    unsigned long saved_sp;
    void *saved_lr;
    unsigned long saved_pc;
} __packed;

struct stopped_patch_args {
    unsigned long patch_at;
    unsigned long hook;
    bool unreg;
};

void trace_hijack_hook(void);
void trace_hijack_pull_return_hook(void);

asm ("trace_hijack_hook:\n"
     UNWIND(".fnstart\n")
     UNWIND(".save {lr}\n") // for the tracepoint's saving of lr
     "  push {r0-r4, lr}\n"
     UNWIND(".save {r0-r4, pc}\n")
     "  ldr r0, [sp, #24]\n"
     "  mov r1, fp\n"
     "  mov r2, ip\n"
     "  bl trace_hijack_callback\n"
     "  subs ip, r0, #0\n"   // #0 means NORMAL_RETURN
     "  popeq {r0-r4, ip, lr}\n"
     "  moveq pc, ip\n"
     "  pop {r0-r3}\n"
     UNWIND(".fnend\n")
     "  mov sp, ip\n"
     UNWIND(".fnstart\n")
     UNWIND(".save {fp, sp, lr}\n")
     UNWIND(".pad #12\n")
     "  ldr ip, [ip]\n"
     "  blx ip\n"
     "trace_hijack_pull_return_hook:\n"
     "  ldr r2, [sp, #20]\n"
     UNWIND(".fnend\n")
     "  pop {r1, r3, lr}\n"
     UNWIND(".fnstart\n")
     UNWIND(".save {fp, sp, lr}\n")
     "  mov fp, r0\n"
     "  blx lr\n"
     "  mov r0, fp\n"
     "  ldm sp, {fp, sp, pc}\n"
     UNWIND(".fnend\n")
     ".type trace_hijack_hook, %function\n"
     ".size trace_hijack_hook, .-trace_hijack_hook\n"
);

// grab the saved on-entry pc from the stack frame, because it corresponds
// directly to the original entrypoint of the function.
static inline unsigned long get_entrypoint(unsigned long *fp)
{
    unsigned long ep = FP_TO_FRAME(fp)->saved_pc + ARM_PC_OFF + FRAME_PC_OFF;

    if (unlikely(*(unsigned long *)ep != FRAME_SETUP_INIT)) {
        pr_err("incorrectly guessed prologue loc for function near %lx\n", ep);
        WARN_ON(1);
        return 0;
    }

    return ep;
}

// build a new stack frame in the place of the existing frame present at the
// frame pointer.
// (the frame pointer points to the pc value pushed during the prologue, so we
// can use that to compute the location of the stack frame overall)
static inline void *rewrite_stack_frame(unsigned long *fp, exit_hook_t exit_cb,
                  unsigned long reentry_pc, void *entry_data)
{
    struct hijack_stack_frame *frame = FP_TO_FRAME(fp);

    *frame = (struct hijack_stack_frame) {
        .saved_pc = (unsigned long) trace_hijack_hook,
        .saved_lr = frame->saved_lr,
        .saved_sp = frame->saved_sp,
        .saved_fp = frame->saved_fp,

        .reentry_pc = reentry_pc,
        .exit_cb = exit_cb,
        .entry_data = entry_data,
    };

    return frame;
}

void *trace_hijack_callback(unsigned long parent_ip, unsigned long *fp,
                    struct hijack_assoc *assoc, unsigned long r3,
                    unsigned long r0, unsigned long r1, unsigned long r2)
{
    unsigned long entrypoint;
    void *entry_data = NULL;

    if (unlikely(parent_ip == (unsigned long) trace_hijack_pull_return_hook))
        return NORMAL_RETURN;

    entrypoint = get_entrypoint(fp);

    if (assoc->entry_hook)
        entry_data = assoc->entry_hook(r0, r1, r2, r3, entrypoint, parent_ip);

    if (likely(assoc->exit_hook == NO_RETURN_HOOK || !entrypoint))
        return NORMAL_RETURN;

    return rewrite_stack_frame(fp, assoc->exit_hook, entrypoint, entry_data);
}

static int patch_tp(unsigned long patch_at, unsigned long hook, bool unreg)
{
    struct dyn_ftrace rec = { .ip = patch_at };

    if (unreg)
        return ftrace_make_nop(NULL, &rec, hook);
    else
        return ftrace_make_call(&rec, hook);
}

static int stopped_patch(void *args)
{
    struct stopped_patch_args *patch = (struct stopped_patch_args *) args;
    return patch_tp(patch->patch_at, patch->hook, patch->unreg);
}

static int stop_and_patch(unsigned long patch_at, unsigned long hook,
                    bool unreg)
{
    struct stopped_patch_args args = {
        .patch_at = patch_at,
        .hook = hook,
        .unreg = unreg
    };
    return stop_machine(&stopped_patch, &args, NULL);
}

int hijack_by_address(void *f, entry_hook_t entry, exit_hook_t exit,
                    bool unreg)
{
    struct hijack_assoc assoc = {
        .entry_hook = entry,
        .exit_hook = exit,
    };
    unsigned long size_ip, base_ip, trace_ip, offset;
    void *thunk;

    if (!kallsyms_lookup_size_offset((unsigned long) f, &size_ip, &offset))
        return -EINVAL;
    base_ip = (unsigned long) f - offset;

    trace_ip = ftrace_location_range(base_ip, base_ip + size_ip - 1);
    if (!trace_ip)
        return -EINVAL;

    thunk = thunk_gen(&trace_hijack_hook, THUNK_ASSOC_REGISTER, &assoc,
                    sizeof(struct hijack_assoc));
    if (!thunk)
        return -ENOMEM;

    return stop_and_patch(trace_ip, (unsigned long) thunk, unreg);
}
EXPORT_SYMBOL(hijack_by_address);
