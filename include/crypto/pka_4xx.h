/*******************************************************************************
 *
 * Copyright (c) 2008 Loc Ho <lho@amcc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * Detail Description:
 * This file defines ioctl structures for the Linux CryptoAPI interface. It
 * provides user space applications accesss into the Linux CryptoAPI
 * functionalities.
 *
 * @file pka4xx.h
 *
 * This file provides helper to the AMCC SoC PKA hardware.
 *
 *******************************************************************************
 */
#ifndef __PKA4XX_H__
#define __PKA4XX_H__

#define PKA4XX_RAM_SIZE       (8*1024)
/* Reserve last 72 bytes for used by sequencer firmware and round down
   to 8-bytes align */
#define PKA4XX_RAM_FREE_SIZE	(((8*1024)-72)&~0x7)
#define PKA4XX_PROGRAM_SIZE   (2*1024)

/**
 * PKA Register and bit Definitions
 *
 */

/* Register PKT_FUNCTION bit definition */
#define PKA_FUNCTION_MUL                0x00000001
#define PKA_FUNCTION_ADDSUB             0x00000002
#define PKA_FUNCTION_RSVD               0x00000004
#define PKA_FUNCTION_MSONE              0x00000008
#define PKA_FUNCTION_ADD                0x00000010
#define PKA_FUNCTION_SUB                0x00000020
#define PKA_FUNCTION_RSHIFT             0x00000040
#define PKA_FUNCTION_LSHIFT             0x00000080
#define PKA_FUNCTION_DIV                0x00000100
#define PKA_FUNCTION_MOD                0x00000200
#define PKA_FUNCTION_COMPARE            0x00000400
#define PKA_FUNCTION_COPY               0x00000800
#define PKA_FUNCTION_SEQOP_MASK         (7 << 12)
#define PKA_FUNCTION_SEQOP_EXPMOD_CRT   (1 << 12)
#define PKA_FUNCTION_SEQOP_EXPMOD_ACT4  (2 << 12)
#define PKA_FUNCTION_SEQOP_ECC_ADD      (3 << 12)
#define PKA_FUNCTION_SEQOP_EXPMOD_ACT2  (4 << 12)
#define PKA_FUNCTION_SEQOP_ECC_MUL      (5 << 12)
#define PKA_FUNCTION_SEQOP_EXPMOD_VAR   (6 << 12)
#define PKA_FUNCTION_SEQOP_MODINV       (7 << 12)
#define PKA_FUNCTION_RUN                (1 << 15)
#define PKA_FUNCTION_STALL              (1 << 24)

/* Register PKA_COMPARE bit definition */
#define PKA_COMPARE_EQUAL               0x00000001
#define PKA_COMPARE_LESSTHAN            0x00000002
#define PKA_COMPARE_GREATHERTHAN        0x00000004

/* Register PKA_MSW bit definition */
#define PKA_MSW_ADDR_MASK               0x000007FF
#define PKA_MSW_RESULT_ZERO             (1 << 15)

/* Register PKA_DIVMSW bit definition */
#define PKA_DIVMSW_ADDR_MASK            0x000007FF
#define PKA_DIVMSW_RESULT_ZERO          (1 << 15)

/* Register PKA_SEQ_CTRL bit definition */
#define PKA_SEQ_CTRL_SW_CTRL_TRIGGER_MASK   0x000000FF
#define PKA_SEQ_CTRL_SEQ_STATUS_MASK        0x0000FF00
#define PKA_SEQ_CTRL_RESET                  0x80000000

/* PKA Registers definitions */
#define PKA_APTR_ADDR                  0x0000
#define PKA_BPTR_ADDR                  0x0004
#define PKA_CPTR_ADDR                  0x0008
#define PKA_DPTR_ADDR                  0x000C
#define PKA_ALENGTH_ADDR               0x0010
#define PKA_BLENGTH_ADDR               0x0014
#define PKA_SHIFT_ADDR                 0x0018
#define PKA_FUNCTION_ADDR              0x001C
#define PKA_COMPARE_ADDR               0x0020
#define PKA_MSW_ADDR                   0x0024
#define PKA_DIVMSW_ADDR                0x0028
#define PKA_SEQ_CTRL_ADDR              0x00C8
#define PKA_RAM_ADDR                   0x2000
#define PKA_PROGRAM_ADDR               0x2000  /* PKA Program area is shared
						  with PKA_RAM. To access,
                                                  put the Sequencer into reset.
						  See PKA_SEQ_CTRL. */

/* # of operation can be queued */
#define PKA4XX_PENDING_OP_MAX		256
#define MAX_FREE_DESC_AVAIL_TRY		10
 
typedef void (*pka4xx_cb)(void *ctx, int status);

/** Structure definitions used for asynchronous PKA operations */
struct pka4xx_async_pkcp_op {
	u32 vecA_cnt;
	u32 *vecA;
	u32 vecB_cnt;
	u32 *vecB;
	u32 *vec_addsub_C;
	u8 shift_val;
};

struct pka4xx_async_expmod_op {
	u32 *base;
	u32 *exp;
	u32 *modulus;
	u32 exp_cnt;
	u32 base_mod_cnt;
};

struct pka4xx_async_expmod_crt_op {
	u32 *expP;
	u32 *expQ;
	u32 exp_len;
	u32 *modP;
	u32 *modQ;
	u32 mod_inverse_len;
	u32 *inverseQ;
	u32 *input;
};

#define async_pkcp		async_op.pkcp_op
#define async_expmod		async_op.expmod_op
#define async_expmod_crt	async_op.expmod_crt_op

struct pka4xx_op
{
	u8	id;
	u32	opcode;		/* Encoded as PKA_FUNCTION register */
	pka4xx_cb	cb;	/* Callback function */
	void 	*ctx;		/* Context for callback function */
	u16     resultC_cnt;	/* # of DWORD for result C */
	u16     resultD_cnt;	/* # of DWORD for result D */
	u32 	*resultC_addr;	/* Address for result C */
	u32 	*resultD_addr;	/* Address for result C */
	u32     ramC_addr;	/* PKA RAM Address for result C in DWORD unit */
	u32     ramD_addr;	/* PKA RAM Address for result D in DWORD unit */
	u32     ram_addr_start;	/* PKA RAM Address start */
	u16     ram_addr_size;	/* PKA RAM Address size */
	struct list_head next;
	int status;
	union {
		struct pka4xx_async_expmod_crt_op expmod_crt_op;
		struct pka4xx_async_expmod_op expmod_op;
		struct pka4xx_async_pkcp_op pkcp_op;
	} async_op;
};

/** Context for async operations */
struct pka4xx_ctx
{
	s16	ram_blk2use;
	struct {
		u8  usable;
		s16 sizeDW;
		u32 addrDW;
	} ram_blk[2];
	u16	op_head;
	u16	op_tail;
	struct pka4xx_op op[PKA4XX_PENDING_OP_MAX];
	u32 csr_paddr;
	u32 pka_ram_paddr;
	volatile char	*csr;
	volatile char	*pka_ram;
	int irq;
	struct list_head completed_event_queue;
	
	spinlock_t 	lock;
	struct tasklet_struct	tasklet;
};

struct hal_config;

/**
 * @brief   IRQ Handler for PKA Interrupts
 * @return  Success/Failure
 */
irqreturn_t pka4xx_irq_handler(int irq, void *id);

/**
 * @brief   Tasklet for post-processing of result of PKA operstaions
 */
void pka4xx_tasklet_cb (unsigned long data);

/**
 * @brief   Process completed events
 */
void pka4xx_process_completed_event (struct pka4xx_op *op);

/**
 * @brief   Retrieve the PKA context (OS neutral)
 * @return  A pointer to PKA context
 * @note New layer of the device musts returns a pointer to this variables.
 *
 *
 */
struct pka4xx_ctx *pka_get_ctx(void);

/**
 * @brief   This function MUSTs be called to initialize the HW
 *          access function.
 * @param   amcc_hal_cfg        HW access configuration. If NULL,
 *                              default will be used.
 * @return  PPRO_RC_OK or PPRO_RC_FATAL
 * @note    To load all access functions from HAL with default initial
 *              map address, pass a NULL as its parameter.
 * @n       To load all access functions from HAL with default runtime
 *              map address, initialize an amcc_hal_config_t struct
 *              with valid HAL signature and set
 *              amcc_hal_config->flags |= HAL_FLAG_LOADINIT;
 *
 *******************************************************************************
 */
int pka4xx_config_set(struct hal_config *cfg);

/**
 * @brief   This function MUSTs be called to de-initialize the HW
 *          access function.
 * @return  PPRO_RC_OK or PPRO_RC_FATAL
 *
 *
 */
int pka4xx_config_clear(void);

/**
 * @brief   This functions reads from the PKA registers.
 * @param   reg_addr    Device register offset
 * @param   data_val    An DWORD pointer to store the returned value
 * @return  PPRO_RC_OK or PPRO_RC_FATAL
 *
 *
 */
int pka4xx_csr_hw_read32(u32 reg_addr, u32 *data_val);

/**
 * @brief   This functions writes to the PKA registers.
 * @param   reg_addr    Device register offset
 * @param   data_val    An DWORD value to write
 * @return  PPRO_RC_OK or PPRO_RC_FATAL
 *
 *
 */
int pka4xx_csr_hw_write32(u32 reg_addr, u32 data_val);

/**
 * @brief   This functions reads from the PKA RAM.
 * @param   reg_addr    ram addr
 * @param   data_val    An DWORD pointer to store the returned value
 * @return  PPRO_RC_OK or PPRO_RC_FATAL
 *
 *
 */
int pka4xx_ram_hw_read32(u32 reg_addr, u32 *data_val);

/**
 * @brief   This functions writes to the PKA RAM.
 * @param   reg_addr    ram addr
 * @param   data_val    An DWORD value to write
 * @return  PPRO_RC_OK or PPRO_RC_FATAL
 *
 *
 */
int pka4xx_ram_hw_write32(u32 reg_addr, u32 data_val);
/**
 * @brief Initiaize PKA hardware
 * @return  PPRO_RC_OK or PPRO_RC_FATAL
 *
 *
 */
int pka4xx_hw_init(void);

/**
 * @brief De-Initiaize PKA hardware
 * @return  PPRO_RC_OK or PPRO_RC_FATAL
 *
 *
 */
int pka4xx_hw_deinit(void);

/**< PKA input vector max count */
#define PKA4XX_VECTOR_MAXSIZE         256

/**
 * @brief Performs large vector multiplication (A * B = C)
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param multiplicand_len Length of multiplicand in DWORD unit (<=
 *			    PKA4XX_VECTOR_MAXSIZE)
 * @param multiplicand  Multiplicand vector pointer of the operation - input A
 * @param multiplier_len   Length of multiplier in DWORD unit (<=
 *			    PKA4XX_VECTOR_MAXSIZE)
 * @param multiplier    Multiplier vector pointer of the operation - input B
 * @param product       Result product vector - output C. Length is
 *			 multiplicand_len * multiplier_len in DWORD unit
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int pka4xx_mul(pka4xx_cb cb,
			void *ctx,
			u32  *op_id,
			u32  multiplicand_len,
			u32  *multiplicand,
			u32  multiplier_len,
			u32  *multiplier,
			u32  *product);

/**
 * @brief Performs vector division (A / B = D and A % B = C)
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param dividend_len  Length of dividend in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param dividend      Dividend vector pointer of the operation - input A
 * @param divisor_len   Length of divisor in DWORD unit (<= dividend_len <=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param divisor       Divisor vector pointer of the operation - input B -
 *			 where most significant 32 bits != 0
 * @param remainder     Result reminder vector pointer of the operation -
 *			 output C. Length is divisor_len + 1 in DWORD unit
 * @param quotient      Result integer vector pointer of the operation -
 *			 output D. Length is dividend_len - divisor_len + 1
 *			 in DWORD unit
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int pka4xx_div(pka4xx_cb cb,
			void *ctx,
			u32  *op_id,
			u32  dividend_len,
			u32  *dividend,
			u32  divisor_len,
			u32  *divisor,
			u32  *remainder,
			u32  *quotient);

/**
 * @brief Performs vector modulo (A % B = C) where B != 0
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param dividend_len  Length of dividend in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param dividend      Dividend vector pointer of the operation - input A
 * @param divisor_len   Length of divisor in DWORD unit (<= dividend_len <=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param divisor       Divisor vector pointer of the operation - input B
 * @param remainder     Result reminder vector pointer of the operation -
 *			 output C. Length is divisor_len + 1 in DWORD unit
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int pka4xx_mod(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 dividend_len,
			u32 *dividend,
			u32 divisor_len,
			u32 *divisor,
			u32 *remainder);

/**
 * @brief Performs vector addition (A + B = C)
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param addendA_len   Length of addendA in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param addendA       Addend vector pointer of the operation - input A
 * @param addendB_len   Length of addendB in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param addendB       Addend vector pointer of the operation - input B
 * @param sum           Result sum vector pointer of the operation - output C.
 *			 Length is MAX(addendA_len, addendB_len) + 1 in DWORD
 *			 unit
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int pka4xx_add(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 addendA_len,
			u32 *addendA,
			u32 addendB_len,
			u32 *addendB,
			u32 *sum);

/**
 * @brief Performs vector substraction (A - B = C) where A >= B
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param minuend_len   Length of minuend in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param minuend       Minuendnd vector pointer of the operation - input A
 * @param subtrahend_len Length of minuend in DWORD unit (<=
 *			  PKA4XX_VECTOR_MAXSIZE)
 * @param subtrahend    Subtrahend vector pointer of the operation - input B
 * @param difference    Result difference vector pointer of the operation -
 *			 output C. Length is MAX(minuend_len, subtrahend_len)
 *			 in DWORD unit.
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int pka4xx_sub(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 minuend_len,
			u32 *minuend,
			u32 subtrahend_len,
			u32 *subtrahend,
			u32 *difference);

/**
 * @brief Performs vector addition and then substraction (A + C - B = D) where
 *        A + C >= B.
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param input_cnt     Length of all inputs in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param addendA       Addend vector pointer of the operation - input A
 * @param addendC       Addend vector pointer of the operation - input C
 * @param subtrahend    Subtrahend vector pointer of the operation - input B
 * @param result        Result vector pointer of the operation - output D.
 *                       Length is input_cnt + 1 in DWORD unit.
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int pka4xx_addsub(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 input_cnt,
			u32 *addendA,
			u32 *addendC,
			u32 *subtrahend,
			u32 *result);

/**
 * @brief Performs vector right shift (A >> X = C)
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param input_cnt     Length of input in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param input         Input vector pointer of the operation - input A
 * @param shift         Shift value of the operation - input X. 0 <= shift <= 31
 * @param result        Result vector pointer of the operation - output C
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int  	pka4xx_rshift(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 input_cnt,
			u32 *input,
			u8  shift,
			u32 *result);

/**
 * @brief Performs vector left shift (A << X = C)
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param input_cnt     Length of input in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param input         Input vector pointer of the operation - input A
 * @param shift         Shift value of the operation - input X. 0 <= shift <= 31
 * @param result        Result vector pointer of the operation - output C
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int  	pka4xx_lshift(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 input_cnt,
			u32 *input,
			u8  shift,
			u32 *result);

/**
 * @brief Performs vector comparsion (A < B, A == B, or A > B)
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param input1_cnt    Length of input1 in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param input1        Input vector pointer of the operation - input A
 * @param input2_cnt    Length of input2 in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param input2        Input vector pointer of the operation - input B
 * @param result        Single result pointer to an unsigned char to store
 *			the result. 0 = A=B; -1 = A<B; 1 = A>B.
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int  	pka4xx_compare(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 input1_cnt,
			u32 *input1,
			u32 input2_cnt,
			u32 *input2,
			int *result);

/**
 * @brief Performs vector modular exponentiation operation - C^A % B = D where B
 *        is odd integer and > 2^32 and C < B
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complext operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param odd_pwr_cnt	# of pre-calculated 'odd power'. For value of:
 *			 2 = ExpMod-ACT2
 *			 8 = ExpMod-ACT4
 *			 1,3,4,5,6,7,9..16 = ExpMod-variable
 *			 17...MAX  = invalid parameter
 * @param base_mod_cnt   Length of base and modulus in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param base          Base vector pointer of the operation - input C
 * @param modulus       Modulus vector pointer of the operation - input B
 * @param exponent_cnt  Length of exponent in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param exponent      Exponent vector pointer of the operation - input A
 * @param result        Result vector pointer of the operation. Length is
 *			 base_mod_cnt in DWORD unit.
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 * @note Use of odd power > 8 is not advisable as the speed advantage for
 *       each extra odd power decreases rapidly (and even become negative for
 *       short exponent vector lengths due to the extra pre-processsing
 *       required). More odd power requires more PKA RAM which will decrease
 *       queue (setup) of pending operation for asynchronous operation.
 *
 *
 */
extern int  pka4xx_expmod(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u8  odd_pwr_cnt,
			u32 base_mod_cnt,
			u32 *base,
			u32 *modulus,
			u32 exponent_cnt,
			u32 *exponent,
			u32 *result);

/**
 * @brief Performs vector modular exponentiation with CRT operation (RSA-CRT)
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complex operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param odd_pwr_cnt	# of pre-calculated 'odd power' (1-16)
 * @param exp_len    	Length of exponent(s) in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param expP          Exponent P vector pointer of the operation - input expP
 * @param expQ          Exponent Q vector pointer of the operation - input expQ
 * @param mod_inverse_len Length of modulus(es), inverse in DWORD unit (<=
 *			   PKA4XX_VECTOR_MAXSIZE)
 * @param modP          Modulus P vector pointer of the operation - input modP
 * @param modQ          Modulus Q vector pointer of the operation - input modQ
 * @param inverseQ      Inverse Q vector pointer of the operation - input
 *			 inverseQ
 * @param input         Input vector pointer of the operation - input
 *			 'Input'. Input is 2*mod_inverse_len.
 * @param result        Result vector of the operation. Result is
 *			 2*mod_inverse_len.
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 * @note CRT Formula Rewritten:
 *
 *		X = (Input % modP)^expP % modP
 *              Y = (Input % modQ)^expQ % modP
 *              Z = (((X-Y) % modP) * inverseQ % modP) * modQ
 *              R = Y + Z
 *        	inverseQ = modQ^-1 % modP
 *		expP = d^-1 % (modP - 1)
 *              expQ = d^-1 % (modQ - 1)
 *
 *        where modP and modQ is odd and co-prime (GCD=1), modP > 2^32,
 *	  modQ > 2^32, 0 < expP < modP - 1, 0 < expQ < modQ - 1,
 *        inverseQ * modP = 1, Input < (modP * modQ), and d is decrypt
 *        exponent. d can be calculated by d = e^-1 % (p - 1)(q - 1) where
 *        e is the encrypt exponent chosen such that it is prime and
 *        GCD(p - 1, q - 1) != e.
 *
 *
 */
int pka4xx_expmod_crt(pka4xx_cb cb,
				void *ctx,
				u32 *op_id,
				u8  odd_pwr_cnt,
				u32 exp_len,
				u32 *expP,
				u32 *expQ,
				u32 mod_inverse_len,
				u32 *modP,
				u32 *modQ,
				u32 *inverseQ,
				u32 *input,
				u32 *result);

/**
 * @brief Performs vector modular inversion operation - 1/A % B = D where B
 *        is odd integer and not 1.
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complext operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param inverse_len   Length of inverse in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param inverse       Inverse vector pointer of the operation - input A
 * @param modulus_len   Length of modulus in DWORD unit (<=
 *			 PKA4XX_VECTOR_MAXSIZE)
 * @param modulus       Modulus vector pointer of the operation - input B
 * @param result        Result vector pointer of the operation - output B.
 *                       Length is modulus_len in DWORD unit.
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *         PPRO_RC_HWERROR      - no inverse [GCD(A,B)=1] or modulus (B) even
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *
 */
int  	pka4xx_modinv(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 inverse_len,
			u32 *inverse,
			u32 modulus_len,
			u32 *modulus,
			u32 *result);

/**
 * @brief Performs vector ECC add operation (Point addition/doubling on elliptic
 *        curve) - y^2 = x^3 + a * x + b % p
 *                 pntA + pntC = pntD
 *        where p is prime and > 2^63, a < p, b < p and pntA and pntC on curve
 *
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complext operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param input_len     Length of points, p, and a in DWORD unit
 *			 (<= 24)
 * @param pntAx         Point Ax vector pointer of the operation - input pntAx
 * @param pntAy         Point Ay vector pointer of the operation - input pntAy
 * @param p             Curve parameter p pointer vector of the operation -
 *			 input p
 * @param a             Constant a vector pointer of the operation - input a
 * @param pntCx         Point Cx vector pointer of the operation - input pntCx
 * @param pntCy         Point Cy vector pointer of the operation - input pntCy
 * @param pntDx         Result pointer Dx vector pointer of the operation.
 *			 Length is input_len + 2 + (input_len % 2) in
 *			 DWORD unit.
 * @param pntDy         Result pointer Dy vector pointer of the operation.
 *			 Length is input_len + 2 + (input_len % 2) in
 *			 DWORD unit.
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *         PPRO_RC_HWERROR      - result is "point-at-infinity" or p not odd,
 *                                p too short, or etc
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *******************************************************************************
 */
int  	pka4xx_ecc_add(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 input_len,
			u32 *pntAx,
			u32 *pntAy,
			u32 *p,
			u32 *a,
			u32 *pntCx,
			u32 *pntCy,
			u32 *pntDx,
			u32 *pntDy);

/**
 * @brief Performs vector ECC multiply operation (Point multiplicatio on
 *        elliptic curve) - y^2 = x^3 + a * x + b % p
 *                          k * pntC = pntD
 *        where p is prime and > 2^63, a < p, b < p and pntA and pntC on curve
 *
 * @param cb            Callback function of asynchronous operation. If this
 *                       parameter is NULL, then it will block until operation
 *                       completed. As this function is called from interrupt
 *                       context, it should defer complext operation and
 *                       return as soon as possible. When there is pending
 *			 operation, this parameter can not be NULL.
 * @param ctx		A void context pointer passed in cb function
 * @param op_id		An integer identifier pointer for asynchronous
 *			 operation. It can be used to cancel pending operation.
 *			 Can be NULL.
 * @param k_len         Length of k constant in DWORD unit (<= 24)
 * @param k             Constant k vector pointer of the operation - input k
 * @param p_len      	Length of p in DWORD unit (p_len + a_len + b_len <= 24)
 * @param p             Curve parameter p vector of the operation - input p
 * @param a_len      	Length of a in DWORD unit (p_len + a_len + b_len <= 24)
 * @param a             Constant a vector of the operation - input a
 * @param b_len      	Length of b in DWORD unit (p_len + a_len + b_len <= 24)
 * @param b             Constant b vector of the operation - input b
 * @param pntCx         Point Cx vector pointer of the operation - input pntCx.
 *			 Length is p_len + a_len + b_len in DWORD unit.
 * @param pntCy         Point Cy vector pointer of the operation - input pntCy.
 *			 Length is p_len + a_len + b_len in DWORD unit.
 * @param result        Result vector pointer of the operation. Length is
 *			 2 * (p_len + a_len + 2 + ((p_len+a_len) % 2)) in
 *			 DWORD unit. The first one for pntDx and second for
 *			 pntDy.
 * @return PPRO_RC_INVALID_PARM - any invalid parameters (< 0)
 *         PPRO_RC_EBUSY        - If cb ia NULL and there is pending operation
 *				  (< 0). Or, hardware buffer is full.
 *         PPRO_RC_HWERROR      - result is "point-at-infinity" or p not odd,
 *                                p too short, or etc
 *	   PPRO_RC_EINPROGRESS  - operation is pending (< 0)
 *         PPRO_RC_OK 		- operation completed successfully (= 0)
 *
 *******************************************************************************
 */
int  	pka4xx_ecc_mul(pka4xx_cb cb,
			void *ctx,
			u32 *op_id,
			u32 k_len,
			u32 *k,
			u32 p_a_pnt_len,
			u32 *p,
			u32 *a,
			u32 *b,
			u32 *pntCx,
			u32 *pntCy,
			u32 *pntDx,
			u32 *pntDy);

#endif
