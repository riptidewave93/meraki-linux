/* hijack.h
 * author: Cel Skeggs <cela@meraki.net>
 */
#ifndef _LINUX_HIJACK_H
#define _LINUX_HIJACK_H

#include <linux/kconfig.h>
#include <linux/types.h>

#ifdef CONFIG_TRACE_HIJACK

// WARNING: tracing functions with >4 parameters is UNDEFINED BEHAVIOR!
// WARNING: hijack_by_address is NOT THREADSAFE!

typedef void *(*entry_hook_t)(unsigned long param1, unsigned long param2, unsigned long param3, unsigned long param4, unsigned long entrypoint, unsigned long parent_ip);

typedef void (*exit_hook_t)(unsigned long return_value, unsigned long entrypoint, unsigned long parent_ip, void *entry_data);

// returns an error code if the hijacking could not be performed, or the symbol could not be found.
int hijack_by_address(void *target, entry_hook_t entry_hook, exit_hook_t exit_hook, bool unregister);

// buf must be at least KSYM_NAME_LEN in size
char *hijack_get_symbol_name(unsigned long ip, char *buf);

#endif

#endif
