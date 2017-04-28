/*
* DesignWare HS OTG controller driver
* Copyright (C) 2006 Synopsys, Inc.
* Portions Copyright (C) 2010 Applied Micro Circuits Corporation.
*
* This program is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License version 2 for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see http://www.gnu.org/licenses
* or write to the Free Software Foundation, Inc., 51 Franklin Street,
* Suite 500, Boston, MA 02110-1335 USA.
*
* Based on Synopsys driver version 2.60a
* Modified by Mark Miesfeld <mmiesfeld@apm.com>
* Modified by Stefan Roese <sr@denx.de>, DENX Software Engineering
* Modified by Chuck Meade <chuck@theptrgroup.com>
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL SYNOPSYS, INC. BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES
* (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#if !defined(__DWC_OTG_PLAT_H__)
#define __DWC_OTG_PLAT_H__

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <asm/io.h>

/**
 * @file
 *
 * This file contains the Platform Specific constants, interfaces
 * (functions and macros) for Linux.
 *
 */
#if !defined(CONFIG_4xx)
#error "The contents of this file are AMCC 44x processor specific!!!"
#endif

#if defined(CONFIG_405EX) || defined(CONFIG_460EX) || defined(CONFIG_APM86xxx)
#define CONFIG_DWC_OTG_REG_LE
#endif

#if defined(CONFIG_405EZ)
#define CONFIG_DWC_OTG_FIFO_LE
#endif

#define SZ_256K  0x00040000
/**
 * Reads the content of a register.
 *
 * @param _reg address of register to read.
 * @return contents of the register.
 *

 * Usage:<br>
 * <code>uint32_t dev_ctl = dwc_read_reg32(&dev_regs->dctl);</code>
 */
static __inline__ uint32_t dwc_read_reg32( volatile uint32_t *_reg)
{
	u32 val;
#ifdef CONFIG_DWC_DEBUG
	static uint32_t *_reg_prev = 0;
	static uint32_t val_prev = 0;
#endif
#ifdef CONFIG_DWC_OTG_REG_LE
	val = in_le32(_reg);
#else
	val = in_be32(_reg);
#endif
#ifdef CONFIG_DWC_DEBUG
	if (_reg_prev != _reg || val_prev != val) {
		printk("DWC:: Read: %p, val: 0x%08x\n", _reg, val);
		_reg_prev = (uint32_t *)_reg;
		val_prev = val;
	}
#endif
	return val;
};

/**
 * Writes a register with a 32 bit value.
 *
 * @param _reg address of register to read.
 * @param _value to write to _reg.
 *
 * Usage:<br>
 * <code>dwc_write_reg32(&dev_regs->dctl, 0); </code>
 */
static __inline__ void dwc_write_reg32( volatile uint32_t *_reg, const uint32_t _value)
{
#ifdef CONFIG_DWC_DEBUG
	static uint32_t *_reg_prev = 0;
	static uint32_t _value_prev = 0;
	if (_reg_prev != _reg || _value_prev != _value) {
		printk("DWC:: Writ: %p, val: 0x%08x\n", _reg, _value);
		_reg_prev = (uint32_t *)_reg;
		_value_prev = _value;
	}
#endif
#ifdef CONFIG_DWC_OTG_REG_LE
	 out_le32(_reg, _value);
#else
	 out_be32(_reg, _value);
#endif
};

/**
 * This function modifies bit values in a register.  Using the
 * algorithm: (reg_contents & ~clear_mask) | set_mask.
 *
 * @param _reg address of register to read.
 * @param _clear_mask bit mask to be cleared.
 * @param _set_mask bit mask to be set.
 *
 * Usage:<br>
 * <code> // Clear the SOF Interrupt Mask bit and <br>
 * // set the OTG Interrupt mask bit, leaving all others as they were.
 *    dwc_modify_reg32(&dev_regs->gintmsk, DWC_SOF_INT, DWC_OTG_INT);</code>
 */
static __inline__
void dwc_modify_reg32( volatile uint32_t *_reg, const uint32_t _clear_mask, const uint32_t _set_mask)
{
#ifdef CONFIG_DWC_OTG_REG_LE
	out_le32( _reg, (in_le32(_reg) & ~_clear_mask) | _set_mask );
#else
	out_be32( _reg, (in_be32(_reg) & ~_clear_mask) | _set_mask );
#endif
};

static __inline__ void dwc_write_datafifo32( volatile uint32_t *_reg, const uint32_t _value)
{
//	printk("DWC:: Writ-FIFO: 0x%08x, val: 0x%08x\n", _reg, _value);
#ifdef CONFIG_DWC_OTG_FIFO_LE
	out_le32(_reg, _value);
#else
	out_be32(_reg, _value);
#endif
};

static __inline__ uint32_t dwc_read_datafifo32( volatile uint32_t *_reg)
{
	u32 val;
#ifdef CONFIG_DWC_OTG_FIFO_LE
	val = in_le32(_reg);
#else
	val = in_be32(_reg);
#endif
//	printk("DWC:: Read-FIFO: 0x%08x, val: 0x%08x\n", _reg, val);
	return val;
};


/**
 * Wrapper for the OS micro-second delay function.
 * @param[in] _usecs Microseconds of delay
 */
static __inline__ void UDELAY( const uint32_t _usecs )
{
	udelay( _usecs );
}

/**
 * Wrapper for the OS milli-second delay function.
 * @param[in] _msecs milliseconds of delay
 */
static __inline__ void MDELAY( const uint32_t _msecs )
{
	mdelay( _msecs );
}

/**
 * Wrapper for the Linux spin_lock.  On the ARM (Integrator)
 * spin_lock() is a nop.
 *
 * @param _lock Pointer to the spinlock.
 */
static __inline__ void SPIN_LOCK( spinlock_t *_lock )
{
	spin_lock(_lock);
}

/**
 * Wrapper for the Linux spin_unlock.  On the ARM (Integrator)
 * spin_lock() is a nop.
 *
 * @param _lock Pointer to the spinlock.
 */
static __inline__ void SPIN_UNLOCK( spinlock_t *_lock )
{
	spin_unlock(_lock);
}

/**
 * Wrapper (macro) for the Linux spin_lock_irqsave.  On the ARM
 * (Integrator) spin_lock() is a nop.
 *
 * @param _l Pointer to the spinlock.
 * @param _f unsigned long for irq flags storage.
 */
#define SPIN_LOCK_IRQSAVE( _l, _f )  { \
	spin_lock_irqsave(_l,_f); \
	}

/**
 * Wrapper (macro) for the Linux spin_unlock_irqrestore.  On the ARM
 * (Integrator) spin_lock() is a nop.
 *
 * @param _l Pointer to the spinlock.
 * @param _f unsigned long for irq flags storage.
 */
#define SPIN_UNLOCK_IRQRESTORE( _l,_f ) {\
	spin_unlock_irqrestore(_l,_f); 	\
	}


/*
 * Debugging support vanishes in non-debug builds.
 */


/**
 * The Debug Level bit-mask variable.
 */
extern uint32_t g_dbg_lvl;
/**
 * Set the Debug Level variable.
 */
static inline uint32_t SET_DEBUG_LEVEL( const uint32_t _new )
{
	uint32_t old = g_dbg_lvl;
	g_dbg_lvl = _new;
	return old;
}

/** When debug level has the DBG_CIL bit set, display CIL Debug messages. */
#define DBG_CIL		(0x2)
/** When debug level has the DBG_CILV bit set, display CIL Verbose debug
 * messages */
#define DBG_CILV	(0x20)
/**  When debug level has the DBG_PCD bit set, display PCD (Device) debug
 *  messages */
#define DBG_PCD		(0x4)
/** When debug level has the DBG_PCDV set, display PCD (Device) Verbose debug
 * messages */
#define DBG_PCDV	(0x40)
/** When debug level has the DBG_HCD bit set, display Host debug messages */
#define DBG_HCD		(0x8)
/** When debug level has the DBG_HCDV bit set, display Verbose Host debug
 * messages */
#define DBG_HCDV	(0x80)
/** When debug level has the DBG_HCD_URB bit set, display enqueued URBs in host
 *  mode. */
#define DBG_HCD_URB	(0x800)

#define DBG_SP		(0x400)
/** When debug level has any bit set, display debug messages */
#define DBG_ANY		(0xFF)

/** All debug messages off */
#define DBG_OFF		0

/** Prefix string for DWC_DEBUG print macros. */
#define USB_DWC "dwc_otg: "

/**
 * Print a debug message when the Global debug level variable contains
 * the bit defined in <code>lvl</code>.
 *
 * @param[in] lvl - Debug level, use one of the DBG_ constants above.
 * @param[in] x - like printf
 *
 *    Example:<p>
 * <code>
 *      DWC_DEBUGPL( DBG_ANY, "%s(%p)\n", __func__, _reg_base_addr);
 * </code>
 * <br>
 * results in:<br>
 * <code>
 * usb-DWC_otg: dwc_otg_cil_init(ca867000)
 * </code>
 */
#ifdef CONFIG_DWC_DEBUG
# define DWC_DEBUGPL(lvl, x...) do{ if ((lvl)&g_dbg_lvl)printk( KERN_ERR USB_DWC x ); }while(0)
# define DWC_DEBUGP(x...)	DWC_DEBUGPL(DBG_ANY, x )

# define CHK_DEBUG_LEVEL(level) ((level) & g_dbg_lvl)

#else

# define DWC_DEBUGPL(lvl, x...) do{}while(0)
# define DWC_DEBUGP(x...)

# define CHK_DEBUG_LEVEL(level) (0)

#endif /*CONFIG_DWC_DEBUG*/

/**
 * Print an Error message.
 */
#define DWC_ERROR(x...) printk( KERN_ERR USB_DWC x )
/**
 * Print a Warning message.
 */
#define DWC_WARN(x...) printk( KERN_INFO USB_DWC x )
/**
 * Print a notice (normal but significant message).
 */
#define DWC_NOTICE(x...) printk( KERN_INFO USB_DWC x )
/**
 *  Basic message printing.
 */
#define DWC_PRINT(x...) printk( KERN_INFO USB_DWC x )

#endif

