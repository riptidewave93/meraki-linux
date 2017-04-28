/*
 * Meraki switch-arm support
 *
 * Copyright 2015 Cisco Systems
 * Dean Thomas <dean.thomas@meraki.com>
 *
 * Licensed under the GNU/GPL. See COPYING for details.
 */
#include <linux/of_platform.h>
#include <asm/hardware/cache-l2x0.h>

#include <asm/mach/arch.h>
#include <asm/siginfo.h>
#include <asm/signal.h>


static bool first_fault = true;

static int switch_arm_abort_handler(unsigned long addr, unsigned int fsr,
				 struct pt_regs *regs)
{
	if (fsr == 0x1c06 && first_fault) {
		first_fault = false;

		/*
		 * These faults with code 0x1c06 happens for no good reason,
		 * possibly left over from the CFE boot loader.
		 */
		pr_warn("External imprecise Data abort at addr=%#lx, fsr=%#x ignored.\n",
		addr, fsr);

		/* Returning non-zero causes fault display and panic */
		return 0;
	}

	/* Others should cause a fault */
	return 1;
}

static void __init switch_arm_init_early(void)
{
	/* Install our hook */
	hook_fault_code(16 + 6, switch_arm_abort_handler, SIGBUS, BUS_OBJERR,
			"imprecise external abort");
}

static const char __initconst *switch_arm_dt_compat[] = {
	"brcm",
	NULL,
};

DT_MACHINE_START(SWITCH_ARM, "SWITCH_ARM")
	.l2c_aux_val	= 0,
	.l2c_aux_mask	= ~0,
	.init_early	= switch_arm_init_early,
	.dt_compat	= switch_arm_dt_compat,
MACHINE_END
