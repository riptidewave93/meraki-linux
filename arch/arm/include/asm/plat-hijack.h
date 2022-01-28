/* plat-hijack.h
 * author: Cel Skeggs <cela@meraki.net>
 */
#ifndef __ASM_HIJACK_H
#define __ASM_HIJACK_H

// thunkgen.c
void *thunk_gen(void *branch_target, int assoc_register, void *assoc_source, unsigned int assoc_len);
void *thunk_to_assoc(void *thunk);

#endif
