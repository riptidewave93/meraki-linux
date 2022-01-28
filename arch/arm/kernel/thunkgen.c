/* thunkgen.c
 * author: Cel Skeggs <cela@meraki.net>
 */
#include <linux/list.h>
#include <linux/moduleloader.h>

struct thunk {
    unsigned long insn_load;
    unsigned long insn_branch;
    void *target;
    struct list_head thunk_list;
    unsigned long assoc_len;
    struct {} assoc;
};

// generates loc: adr r<dst>, target
#define INSN_ADR(dst, target, loc) \
        (0xE28F0000 | ((dst) << 12) | ((target - loc - 8) & 0xFFF))
#define INSN_ADR_ASSOC(dst) (INSN_ADR(dst, offsetof(struct thunk, assoc), \
                                           offsetof(struct thunk, insn_load)))
// generates ldr pc, [pc, #-4]
#define INSN_LDR_PC_NEXT 0xE51FF004

static void thunk_build(struct thunk *thunk, void *target, int assoc_reg,
                        void *assoc, unsigned int assoc_len)
{
    *thunk = (struct thunk) {
        .insn_load = INSN_ADR_ASSOC(assoc_reg),
        .insn_branch = INSN_LDR_PC_NEXT,
        .target = target,
        .assoc_len = assoc_len,
    };

    memcpy(&thunk->assoc, assoc, assoc_len);
}

static bool thunk_matches(struct thunk *thunk, void *target, int assoc_reg,
                        void *assoc, unsigned int assoc_len)
{
    return thunk->insn_load == INSN_ADR_ASSOC(assoc_reg)
        && thunk->target == target
        && thunk->assoc_len == assoc_len
        && memcmp(&thunk->assoc, assoc, assoc_len) == 0;
}

static LIST_HEAD(thunks);

void *thunk_gen(void *target, int assoc_reg,
                void *assoc, unsigned int assoc_len)
{
    struct thunk *thunk;

    if (assoc_reg < 0 || assoc_reg > 14)
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
