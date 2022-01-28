/* hijack.c
 * author: Cel Skeggs <cela@meraki.net>
 */
#include <linux/hijack.h>
#include <linux/kallsyms.h>
#include <linux/string.h>

char *hijack_get_symbol_name(unsigned long ip, char *buf)
{
    if (!ip)
        strcpy(buf, "_null");
    else if (!kallsyms_lookup(ip, NULL, NULL, NULL, buf))
        sprintf(buf, "_unknown_%lx", ip);
    return buf;
}
EXPORT_SYMBOL(hijack_get_symbol_name);
