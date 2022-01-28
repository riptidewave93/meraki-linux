/* hijack.c
 * author: Cel Skeggs <cela@meraki.net>
 */
#include <linux/slab.h>
#include <linux/ftrace.h>
#include <linux/module.h>

#include <linux/hijack.h>
#include <asm/plat-hijack.h>

/*
standard entrypoint:
	stp x29, x30, [sp, #-<N>]!
	mov x29, sp

patched entrypoint:
	stp x29, x30, [sp, #-<N>]!
	bl <thunk>
*/

#define FP_TO_FRAME(fp) ((struct frame_top *) (fp))
#define STACK_ENTRY_SIZE (8)

#define THUNK_ASSOC_REGISTER (6)

struct hijack_assoc {
    unsigned long padding; // must be at #0 for hijack_hook asm
    unsigned long patch_offset;
    unsigned long frame_len;
    entry_hook_t entry_hook;
    exit_hook_t exit_hook;
};

struct frame_top {
    unsigned long fp;
    unsigned long lr;
};
// variable-sized padding separates these two ... when rebuilt
struct frame_bottom {
    void *entry_data;
    unsigned long entrypoint;
    unsigned long lr;
    exit_hook_t exit_hook;
};

void hijack_hook(void);
void hijack_return_hook(void);

asm("hijack_hook:\n"
    "  ldr x16, [x6]\n"
    "  add x29, sp, x16\n"
    // pushes {x0, x1, x2, x3, x16, x30, PAD, PAD, PAD, PAD}
    "  stp x0, x1, [sp, #-80]!\n"
    "  stp x2, x3, [sp, #16]\n"
    "  stp x16, x30, [sp, #32]\n"
    "  mov x5, x30\n"
    "  mov x4, x29\n"
    // hijack_callback(x0, x1, x2, x3, fp, entrypoint, thunkdata)
    //    (parent_lr = *(fp+8)
    "  bl hijack_callback\n"
    "  cmp x0, #0\n"
    "  ldp x17, x16, [sp, #32]\n"
    "  ldp x2, x3, [sp, #16]\n"
    "  ldp x0, x1, [sp], #48\n"
	//          sp --\
	//               v
	// built frame: {<padding>, orig.x29, &return_hook, <padding>, entry_data,
    //                                         entrypoint, orig.x30, exit_hook}
    "  beq no_exit_hook\n"
    "  add x29, sp, x17\n"
    "  b exit_hook\n"
    "no_exit_hook:\n"
    "  add sp, sp, #32\n"
    "exit_hook:\n"
    "  ldr x30, [x29, #8]\n"
    "  br x16\n"
    ".type hijack_hook, %function\n"
    ".size hijack_hook, .-hijack_hook\n"
);

asm("hijack_return_hook:\n"
	// x0 = return value
	// stack: {entry_data, entrypoint, orig.x30, exit_hook}
    "  ldp x3, x1, [sp], #16\n"
    "  ldp x2, x4, [sp]\n"
    "  str x0, [sp, #8]\n"
	// exit_hook(return_value, entrypoint, parent_ip, entry_data)
    "  blr x4\n"
    "  ldp x30, x0, [sp], #16\n"
    "  ret\n"
     ".type hijack_return_hook, %function\n"
     ".size hijack_return_hook, .-hijack_return_hook\n"
);

static void rewrite_stack_frame(unsigned long fp, struct hijack_assoc *assoc,
                                unsigned long entrypoint, void *entry_data)
{
    struct frame_top *old_frame = FP_TO_FRAME(fp);
    struct frame_top *new_frame = FP_TO_FRAME(fp - 4 * STACK_ENTRY_SIZE);
    struct frame_bottom *new_frame_bottom = (struct frame_bottom *)
                    (fp + assoc->frame_len - 4 * STACK_ENTRY_SIZE);

    *new_frame = (struct frame_top) {
        .fp = old_frame->fp,
        .lr = (unsigned long) &hijack_return_hook,
    };
    *new_frame_bottom = (struct frame_bottom) {
        .lr = old_frame->lr,
        .exit_hook = assoc->exit_hook,
        .entrypoint = entrypoint,
        .entry_data = entry_data,
    };
}

bool hijack_callback(unsigned long x0, unsigned long x1, unsigned long x2,
                     unsigned long x3, unsigned long fp, unsigned long lr,
                     struct hijack_assoc *assoc)
{
    void *entry_data = NULL;
    unsigned long parent_ip = FP_TO_FRAME(fp)->lr;
    unsigned long entrypoint = lr - assoc->patch_offset - 4;

    if (assoc->entry_hook)
        entry_data = assoc->entry_hook(x0, x1, x2, x3, entrypoint, parent_ip);

    if (likely(assoc->exit_hook == NULL))
        return false;

    rewrite_stack_frame(fp, assoc, entrypoint, entry_data);

    return true;
}

static unsigned int gen_replaced_add_insn(unsigned int padding)
{
    if (padding >= 4096)
        BUG();
    return 0x910003FD | (padding << 10);
}

static int patch_ep(void *patch_address, void *hook, unsigned int padding)
{
    unsigned int new_insn, old_insn, found_insn;
    int err;

    new_insn = aarch64_insn_gen_branch_imm((unsigned long) patch_address,
                            (unsigned long) hook, AARCH64_INSN_BRANCH_LINK);
    old_insn = gen_replaced_add_insn(padding);

    if ((err = aarch64_insn_read(patch_address, &found_insn)) != 0)
        return err;

    if (old_insn != found_insn) {
        pr_err("hijack patch failure: old insn was %x, not expected %x\n",
                found_insn, old_insn);
        return -EINVAL;
    }

    err = aarch64_insn_patch_text_sync(&patch_address, &new_insn, 1);
    return err ? -EFAULT : 0;
}

static int unpatch_ep(void *patch_address)
{
    struct hijack_assoc *assoc;
    unsigned int new_insn;
    unsigned int found_insn;
    int err;
    void *hook;

    if ((err = aarch64_insn_read(patch_address, &found_insn)) != 0)
        return err;

    if (!aarch64_insn_is_bl(found_insn))
        return -EINVAL;

    hook = aarch64_get_branch_offset(found_insn) + patch_address;

    assoc = (struct hijack_assoc *) thunk_to_assoc(hook);
    if (!assoc)
        return -EINVAL;

    new_insn = gen_replaced_add_insn(assoc->padding);

    err = aarch64_insn_patch_text_sync(&patch_address, &new_insn, 1);
    return err ? -EFAULT : 0;
}

#define PROLOGUE_1_LEN 2
void prologue_1(void);

asm("prologue_1:\n"
    // min
    "  stp x29, x30, [sp, #-8]!\n"
    "  mov x29, sp\n"
    // max
    "  stp x29, x30, [sp, #0]!\n"
    "  mov x29, sp\n"
);

#define PROLOGUE_2_LEN 3
void prologue_2(void);

asm("prologue_2:\n"
    // min
    "  sub sp, sp, #0\n"
    "  stp x29, x30, [sp, #-8]\n"
    "  add x29, sp, #0\n"
    // max
    "  sub sp, sp, #4095\n"
    "  stp x29, x30, [sp, #0]\n"
    "  add x29, sp, #4095\n"
);

#define PROLOGUE_3_LEN 3
void prologue_3(void);
asm("prologue_3:\n"
    // min
    "  sub sp, sp, #0\n"
    "  stp x29, x30, [sp, #-8]!\n"
    "  mov x29, sp\n"
    // max
    "  sub sp, sp, #4095\n"
    "  stp x29, x30, [sp, #0]!\n"
    "  mov x29, sp\n"
);

// returns the patch address
static void *decode_prologue(void *address, unsigned long *frame_length,
                             unsigned long *padding)
{
    unsigned int code[3];
    int bitfield_values[3];
    int i;
    for (i = 0; i < 3; i++)
        if (aarch64_insn_read(address + i * 4, &code[i]))
            return NULL;
    if (autobindiff(code, prologue_1, PROLOGUE_1_LEN, bitfield_values, 1)) {
        int frame_len = bitfield_values[0] * -8;
        if (frame_len <= 0)
            return NULL;
        if (frame_len & 15)
            return NULL;
        *frame_length = frame_len;
        *padding = 0;
        return address + 4;
    }
    if (autobindiff(code, prologue_2, PROLOGUE_2_LEN, bitfield_values, 3)) {
        int entire_length = bitfield_values[0];
        int below_length = bitfield_values[1] * 8;
        if (entire_length & 15)
            return NULL;
        if (below_length < 0)
            return NULL;
        if (below_length & 15)
            return NULL;
        if (below_length != bitfield_values[2])
            return NULL;
        *frame_length = entire_length - below_length;
        *padding = below_length;
        return address + 8;
    }
    if (autobindiff(code, prologue_3, PROLOGUE_3_LEN, bitfield_values, 2)) {
        int frame_len = bitfield_values[0];
        int extra_length = bitfield_values[1] * -8;
        if (frame_len & 15)
            return NULL;
        if (extra_length <= 0)
            return NULL;
        if (extra_length & 15)
            return NULL;
        *frame_length = frame_len + extra_length;
        *padding = 0;
        return address + 8;
    }
    return NULL;
}

// just gets the patch address, and doesn't do a full validation
// (used for deregistering, when a prologue has already been mangled)
static void *quick_decode_prologue(void *address)
{
    unsigned int insn;
    if (aarch64_insn_read(address, &insn))
        return NULL;
    switch (insn >> 24) {
    case 0xA9: // stp
        return address + 4;
    case 0xD1: // sub
        return address + 8;
    default:
        return NULL;
    }
}

int hijack_by_address(void *f, entry_hook_t entry, exit_hook_t exit,
                      bool unreg)
{
    void *patch_address, *thunk;
    struct hijack_assoc assoc = {
        .entry_hook = entry,
        .exit_hook = exit
    };

    if (!f)
        return -ENOENT;

    if (!unreg) {
        patch_address = decode_prologue(f, &assoc.frame_len, &assoc.padding);
        if (!patch_address) {
            pr_err("hijack on unrecognized prologue for function %p\n", f);
            return -EINVAL;
        }
        assoc.patch_offset = patch_address - f;

        thunk = thunk_gen(&hijack_hook, THUNK_ASSOC_REGISTER, &assoc,
                          sizeof(assoc));
        if (!thunk)
            return -ENOMEM;

        return patch_ep(patch_address, thunk, assoc.padding);
    } else {
        patch_address = quick_decode_prologue(f);
        if (!patch_address) {
            pr_err("unhijack on unrecognized prologue for function %p\n", f);
            return -EINVAL;
        }

        return unpatch_ep(patch_address);
    }
}
EXPORT_SYMBOL(hijack_by_address);
