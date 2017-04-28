/*
 * APM86xxx suspend support
 *
 * Copyright (c) 2010 Applied Micro Circuits Corporation.
 * Victor Gallardo (vgallardo@apm.com)
 *
 * This program is used to generate definitions needed by
 * assembly language modules.
 *
 * We use the technique used in the OSF Mach kernel code:
 * generate asm statements containing #defines,
 * compile this file to assembler, and then extract the
 * #defines from the assembly-language output.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <stddef.h>
#include <linux/kbuild.h>

#include "apm86xxx_suspend.h"

int main(void)
{
	DEFINE(SAVE_DATA_SIZE, sizeof(struct apm86xxx_pm_save));

	DEFINE(SAVE_GPR0, offsetof(struct apm86xxx_pm_save, gpr[0]));
	DEFINE(SAVE_GPR1, offsetof(struct apm86xxx_pm_save, gpr[1]));
	DEFINE(SAVE_GPR2, offsetof(struct apm86xxx_pm_save, gpr[2]));
	DEFINE(SAVE_GPR3, offsetof(struct apm86xxx_pm_save, gpr[3]));
	DEFINE(SAVE_GPR4, offsetof(struct apm86xxx_pm_save, gpr[4]));
	DEFINE(SAVE_GPR5, offsetof(struct apm86xxx_pm_save, gpr[5]));
	DEFINE(SAVE_GPR6, offsetof(struct apm86xxx_pm_save, gpr[6]));
	DEFINE(SAVE_GPR7, offsetof(struct apm86xxx_pm_save, gpr[7]));
	DEFINE(SAVE_GPR8, offsetof(struct apm86xxx_pm_save, gpr[8]));
	DEFINE(SAVE_GPR9, offsetof(struct apm86xxx_pm_save, gpr[9]));
	DEFINE(SAVE_GPR10, offsetof(struct apm86xxx_pm_save, gpr[10]));
	DEFINE(SAVE_GPR11, offsetof(struct apm86xxx_pm_save, gpr[11]));
	DEFINE(SAVE_GPR12, offsetof(struct apm86xxx_pm_save, gpr[12]));
	DEFINE(SAVE_GPR13, offsetof(struct apm86xxx_pm_save, gpr[13]));
	DEFINE(SAVE_GPR14, offsetof(struct apm86xxx_pm_save, gpr[14]));
	DEFINE(SAVE_GPR15, offsetof(struct apm86xxx_pm_save, gpr[15]));
	DEFINE(SAVE_GPR16, offsetof(struct apm86xxx_pm_save, gpr[16]));
	DEFINE(SAVE_GPR17, offsetof(struct apm86xxx_pm_save, gpr[17]));
	DEFINE(SAVE_GPR18, offsetof(struct apm86xxx_pm_save, gpr[18]));
	DEFINE(SAVE_GPR19, offsetof(struct apm86xxx_pm_save, gpr[19]));
	DEFINE(SAVE_GPR20, offsetof(struct apm86xxx_pm_save, gpr[20]));
	DEFINE(SAVE_GPR21, offsetof(struct apm86xxx_pm_save, gpr[21]));
	DEFINE(SAVE_GPR22, offsetof(struct apm86xxx_pm_save, gpr[22]));
	DEFINE(SAVE_GPR23, offsetof(struct apm86xxx_pm_save, gpr[23]));
	DEFINE(SAVE_GPR24, offsetof(struct apm86xxx_pm_save, gpr[24]));
	DEFINE(SAVE_GPR25, offsetof(struct apm86xxx_pm_save, gpr[25]));
	DEFINE(SAVE_GPR26, offsetof(struct apm86xxx_pm_save, gpr[26]));
	DEFINE(SAVE_GPR27, offsetof(struct apm86xxx_pm_save, gpr[27]));
	DEFINE(SAVE_GPR28, offsetof(struct apm86xxx_pm_save, gpr[28]));
	DEFINE(SAVE_GPR29, offsetof(struct apm86xxx_pm_save, gpr[29]));
	DEFINE(SAVE_GPR30, offsetof(struct apm86xxx_pm_save, gpr[30]));
	DEFINE(SAVE_GPR31, offsetof(struct apm86xxx_pm_save, gpr[31]));

	DEFINE(SAVE_SPRG0, offsetof(struct apm86xxx_pm_save, sprg[0]));
	DEFINE(SAVE_SPRG1, offsetof(struct apm86xxx_pm_save, sprg[1]));
	DEFINE(SAVE_SPRG2, offsetof(struct apm86xxx_pm_save, sprg[2]));
	DEFINE(SAVE_SPRG3, offsetof(struct apm86xxx_pm_save, sprg[3]));
	DEFINE(SAVE_SPRG4, offsetof(struct apm86xxx_pm_save, sprg[4]));
	DEFINE(SAVE_SPRG5, offsetof(struct apm86xxx_pm_save, sprg[5]));
	DEFINE(SAVE_SPRG6, offsetof(struct apm86xxx_pm_save, sprg[6]));
	DEFINE(SAVE_SPRG7, offsetof(struct apm86xxx_pm_save, sprg[7]));

	DEFINE(SAVE_DEC, offsetof(struct apm86xxx_pm_save, dec));
	DEFINE(SAVE_TBWL, offsetof(struct apm86xxx_pm_save, tbwl));
	DEFINE(SAVE_TBWU, offsetof(struct apm86xxx_pm_save, tbwu));
	DEFINE(SAVE_TCR, offsetof(struct apm86xxx_pm_save, tcr));

	DEFINE(SAVE_USPRG0, offsetof(struct apm86xxx_pm_save, usprg0));

	DEFINE(SAVE_CR, offsetof(struct apm86xxx_pm_save, cr));
	DEFINE(SAVE_LR, offsetof(struct apm86xxx_pm_save, lr));
	DEFINE(SAVE_MSR, offsetof(struct apm86xxx_pm_save, msr));
	DEFINE(SAVE_MMUCR, offsetof(struct apm86xxx_pm_save, mmucr));

	DEFINE(SAVE_PID, offsetof(struct apm86xxx_pm_save, pid));

	DEFINE(SAVE_CCR0, offsetof(struct apm86xxx_pm_save, ccr[0]));
	DEFINE(SAVE_CCR1, offsetof(struct apm86xxx_pm_save, ccr[1]));

	DEFINE(SAVE_IVPR, offsetof(struct apm86xxx_pm_save, ivpr));
	DEFINE(SAVE_IVOR0, offsetof(struct apm86xxx_pm_save, ivor[0]));
	DEFINE(SAVE_IVOR1, offsetof(struct apm86xxx_pm_save, ivor[1]));
	DEFINE(SAVE_IVOR2, offsetof(struct apm86xxx_pm_save, ivor[2]));
	DEFINE(SAVE_IVOR3, offsetof(struct apm86xxx_pm_save, ivor[3]));
	DEFINE(SAVE_IVOR4, offsetof(struct apm86xxx_pm_save, ivor[4]));
	DEFINE(SAVE_IVOR5, offsetof(struct apm86xxx_pm_save, ivor[5]));
	DEFINE(SAVE_IVOR6, offsetof(struct apm86xxx_pm_save, ivor[6]));
	DEFINE(SAVE_IVOR7, offsetof(struct apm86xxx_pm_save, ivor[7]));
	DEFINE(SAVE_IVOR8, offsetof(struct apm86xxx_pm_save, ivor[8]));
	DEFINE(SAVE_IVOR9, offsetof(struct apm86xxx_pm_save, ivor[9]));
	DEFINE(SAVE_IVOR10, offsetof(struct apm86xxx_pm_save, ivor[10]));
	DEFINE(SAVE_IVOR11, offsetof(struct apm86xxx_pm_save, ivor[11]));
	DEFINE(SAVE_IVOR12, offsetof(struct apm86xxx_pm_save, ivor[12]));
	DEFINE(SAVE_IVOR13, offsetof(struct apm86xxx_pm_save, ivor[13]));
	DEFINE(SAVE_IVOR14, offsetof(struct apm86xxx_pm_save, ivor[14]));
	DEFINE(SAVE_IVOR15, offsetof(struct apm86xxx_pm_save, ivor[15]));

	/* Make sure TLB are always last -- needed for correct offset */
	DEFINE(SAVE_TLB0_TID, offsetof(struct apm86xxx_pm_save, tlb[0].tid));
	DEFINE(SAVE_TLB0_WD0, offsetof(struct apm86xxx_pm_save, tlb[0].word0));
	DEFINE(SAVE_TLB0_WD1, offsetof(struct apm86xxx_pm_save, tlb[0].word1));
	DEFINE(SAVE_TLB0_WD2, offsetof(struct apm86xxx_pm_save, tlb[0].word2));
	DEFINE(SAVE_TLB1_TID, offsetof(struct apm86xxx_pm_save, tlb[1].tid));

	return 0;
}
