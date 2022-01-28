#ifndef __ASM_HIJACK_H
#define __ASM_HIJACK_H

// autobindiff.c
bool autobindiff(void *blob, void *templates, unsigned long lenw, unsigned int *bitfield_out, unsigned int bitfield_num);

// thunkgen.c
void *thunk_gen(void *branch_target, int assoc_register, void *assoc_source, unsigned int assoc_len);
void *thunk_to_assoc(void *thunk);

#endif
