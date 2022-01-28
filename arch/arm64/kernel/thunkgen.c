/* thunkgen.c
 * author: Cel Skeggs <cela@meraki.net>
 */
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/moduleloader.h>

struct thunk {
    unsigned int insn_load_assoc;
    unsigned int insn_load_target;
    unsigned int insn_branch;
    void *target;
    struct list_head thunk_list;
    unsigned int assoc_len;
    struct {} assoc;
};

// generates loc: adr x<dst>, target
#define INSN_ADR(dst, target, loc) \
        (0x10000000 | (dst) | (((target - loc) << 3) & 0x00FFFFE0))
// generates loc: ldr x<dst>, target
#define INSN_LDR(dst, target, loc) \
        (0x58000000 | (dst) | (((target - loc) << 3) & 0x00FFFFE0))
// generates br x<src>
#define INSN_BR(src) (0xD61F0000 | ((src) << 5))
#define INSN_ADR_ASSOC(dst) (INSN_ADR(dst, offsetof(struct thunk, assoc), \
                                      offsetof(struct thunk, insn_load_assoc)))
#define INSN_LDR_TARGET(dst) \
                (INSN_LDR(dst, offsetof(struct thunk, target), \
                               offsetof(struct thunk, insn_load_target)))

#define SCRATCH_REGISTER 16 // ip

static void thunk_build(struct thunk *thunk, void *target, int assoc_reg,
                        void *assoc, unsigned int assoc_len)
{
    *thunk = (struct thunk) {
        .insn_load_assoc = INSN_ADR_ASSOC(assoc_reg),
        .insn_load_target = INSN_LDR_TARGET(SCRATCH_REGISTER),
        .insn_branch = INSN_BR(SCRATCH_REGISTER),
        .target = target,
        .assoc_len = assoc_len,
    };

    memcpy(&thunk->assoc, assoc, assoc_len);
}

static bool thunk_matches(struct thunk *thunk, void *target, int assoc_reg,
                          void *assoc, unsigned int assoc_len)
{
    return thunk->insn_load_assoc == INSN_ADR_ASSOC(assoc_reg)
        && thunk->target == target
        && thunk->assoc_len == assoc_len
        && memcmp(&thunk->assoc, assoc, assoc_len) == 0;
}

static LIST_HEAD(thunks);

// thunk will clobber x16
void *thunk_gen(void *target, int assoc_reg, void *assoc,
                unsigned int assoc_len)
{
    struct thunk *thunk;
    if (assoc_reg < 0 || assoc_reg > 30 || assoc_reg == SCRATCH_REGISTER)
        BUG();

    list_for_each_entry(thunk, &thunks, thunk_list)
        if (thunk_matches(thunk, target, assoc_reg, assoc, assoc_len))
            return thunk;

    thunk = (struct thunk *) module_alloc(sizeof(struct thunk) + assoc_len);
    if (!thunk)
        return NULL;

    thunk_build(thunk, target, assoc_reg, assoc, assoc_len);
    list_add(&thunk->thunk_list, &thunks);
    return thunk;
}

void *thunk_to_assoc(void *thunk)
{
    struct thunk *cursor;
    list_for_each_entry(cursor, &thunks, thunk_list)
        if (cursor == thunk)
            return &cursor->assoc;
    return NULL;
}
