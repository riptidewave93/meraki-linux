#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>

        .set noreorder
__start:
        li  t0, CONF_CM_UNCACHED /* set up the kseg0 to be un-cached */
        mtc0 t0, CP0_CONFIG
        lui t0, %hi(relocate)
        ori t0, %lo(relocate)
        jr t0
        nop /* branch delay slot */
relocate:
        bal jumper
        nop /* branch delay slot */
jumper:
        sub sp, ra, 8
        li  t0, 0
        lw t1, _bsize
_zerobss:
        sw zero, _bstart(t0)
        addiu t0, 4
        bne t0, t1, _zerobss
        nop
        b loader_entry
        nop

.globl  __start
