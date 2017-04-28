/*
 * MMIO register definitions used on various IBM/AMCC * 4xx processors
 * 
 * (C) Copyright 2008, Applied Micro Circuits Corporation
 * Victor Gallardo (vgallardo@amcc.com)
 *
 * based on dcr-regs.h
 *
 *    Copyright 2007 Benjamin Herrenschmidt, IBM Corp
 *                   <benh@kernel.crashing.org>
 */

#ifndef __MMIO_REGS_H__
#define __MMIO_REGS_H__

/* IIC Register offset */
#define IIC_MDBUF	0x00
#define IIC_SDBUF	0x02
#define IIC_LMADR	0x04
#define IIC_HMADR	0x05
#define IIC_CNTL	0x06
#define IIC_MDCNTL	0x07
#define IIC_STS		0x08
#define IIC_EXTSTS	0x09
#define IIC_LSADR	0x0A
#define IIC_HSADR	0x0B
#define IIC_CLKDIV	0x0C
#define IIC_INTRMSK	0x0D
#define IIC_XFRCNT	0x0E
#define IIC_XTCNTLSS	0x0F
#define IIC_DIRECTCNTL	0x10

/* STS Register Bit definition */
#define IIC_STS_PT	0x01
#define IIC_STS_IRQA	0x02
#define IIC_STS_ERR	0x04
#define IIC_STS_SCMP	0x08
#define IIC_STS_MDBF	0x10
#define IIC_STS_MDBS	0x20
#define IIC_STS_SLPR	0x40
#define IIC_STS_SSS	0x80

/* MDCNTL Register Bit definition */
#define IIC_MDCNTL_HSCL	0x01
#define IIC_MDCNTL_EUBS	0x02
#define IIC_MDCNTL_EINT	0x04
#define IIC_MDCNTL_ESM	0x08
#define IIC_MDCNTL_FSM	0x10
#define IIC_MDCNTL_EGC	0x20
#define IIC_MDCNTL_FMDB	0x40
#define IIC_MDCNTL_FSDB	0x80

/* CNTL Register Bit definition */
#define IIC_CNTL_PT	0x01
#define IIC_CNTL_READ	0x02
#define IIC_CNTL_CHT	0x04
#define IIC_CNTL_RPST	0x08
#define IIC_CNTL_AMD	0x40
#define IIC_CNTL_HMT	0x80

#endif /* __MMIO_REGS_H__ */
