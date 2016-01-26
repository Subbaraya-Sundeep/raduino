/*
 * Copyright (c) 2016 Subbaraya Sundeep <sundeep.babi@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef PERIPH_H
#define PERIPH_H

#include "object.h"

#define write32(addr, val)	(*(volatile unsigned int *) (addr) = (val))
#define read32(addr)	(*(volatile unsigned int *) (addr))
#define INVAL		0xDEAD

#define delay()		({				\
	volatile unsigned int i; 		\
	for(i=0; i < 0x100000; i++); })

enum mode {
	INPUT = 0,
	OUTPUT,
};

enum level {
	LOW = 0,
	HIGH,
};

struct cpu_regs {
	unsigned regs[17];
};

#define IO_BASE				0x3F000000

/* GPIO */
#define GPIO_BASE			(IO_BASE + 0x200000)
#define GPFSEL0				0x0000
#define GPFSEL1				0x0004
#define GPFSEL2				0x0008
#define GPFSEL3				0x000C
#define GPFSEL4				0x0010

#define GPSET0				0x001C
#define GPSET1				0x0020
#define GPCLR0				0x0028
#define GPCLR1				0x002C

#define GPLEV0				0x0034
#define GPLEV1				0x0038

#define GPPUD       		0x0094
#define GPPUDCLK0   		0x0098

#define gpio_write(reg, val)	write32(GPIO_BASE + (reg), (val))
#define gpio_read(reg)			read32(GPIO_BASE + (reg))

/* AUX - UART1 */
#define AUX_BASE			(IO_BASE + 0x210000)
#define AUX_ENABLES     	0x5004
#define AUX_MU_IO_REG   	0x5040
#define AUX_MU_IER_REG  	0x5044
#define AUX_MU_IIR_REG  	0x5048
#define AUX_MU_LCR_REG  	0x504C
#define AUX_MU_MCR_REG  	0x5050
#define AUX_MU_LSR_REG  	0x5054
#define AUX_MU_MSR_REG  	0x5058
#define AUX_MU_SCRATCH  	0x505C
#define AUX_MU_CNTL_REG 	0x5060
#define AUX_MU_STAT_REG 	0x5064
#define AUX_MU_BAUD_REG 	0x5068

#define RX_LEVEL(reg)	(((reg) & (0xF << 16)) >> 16)
#define TX_LEVEL(reg)	(((reg) & (0xF << 24)) >> 24)

#define aux_write(reg, val)		write32(AUX_BASE + (reg), (val))
#define aux_read(reg)			read32(AUX_BASE + (reg))

#endif
