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

#include "uart.h"
#include "printf.h"

#define write(x) _Generic((x),				\
					unsigned char: write_char, 	\
					int: write_int,				\
					default: write_char) (x)

#define NULL (void *)0

void serialEvent() __attribute__((weak));

void serialEvent()
{
}

static void uart_put(void *arg, char c)
{
	while (!(aux_read(AUX_MU_LSR_REG) & 0x20));

	aux_write(AUX_MU_IO_REG, c);
}

static void begin(unsigned int baud)
{
	unsigned int ra;

	switch (baud) {
	case 9600:
		ra = BAUD_9600;
		break;
	case 4800:
		ra = BAUD_4800;
		break;
	case 2400:
		ra = BAUD_2400;
		break;
	case 115200:
	default:
		ra = BAUD_115200;
		break;
	};

	aux_write(AUX_ENABLES, 1);
	aux_write(AUX_MU_IER_REG, 0);
	aux_write(AUX_MU_CNTL_REG,0);
	aux_write(AUX_MU_LCR_REG, 3);
	aux_write(AUX_MU_MCR_REG, 0);
	aux_write(AUX_MU_IER_REG, 0);
	aux_write(AUX_MU_IIR_REG, 0xC6);
	aux_write(AUX_MU_BAUD_REG, ra);

	ra = gpio_read(GPFSEL1);
	ra &= ~(7<<12); //gpio14
	ra |= 2<<12;    //alt5
	ra &= ~(7<<15); //gpio15
	ra |= 2<<15;    //alt5
	gpio_write(GPFSEL1, ra);

	gpio_write(GPPUD, 0);
	delay();
	gpio_write(GPPUDCLK0, (1<<14)|(1<<15));
	delay();
	gpio_write(GPPUDCLK0, 0);

	aux_write(AUX_MU_CNTL_REG, 3);

	init_printf(NULL, uart_put);
}


#if 0
static void write_char(unsigned char data)
{
	uart_put(data);
}

static void write_int(int data)
{
	uart_put('I');
}

static void write_hex(unsigned int data)
{
	uart_put('I');
}
#endif



static unsigned int read(void)
{
	unsigned int data;

	while(1)
	{
		if (aux_read(AUX_MU_LSR_REG) & 0x01)
			break;
	}

	data = aux_read(AUX_MU_IO_REG);

	return data;
}

static int available(void)
{
	return (RX_LEVEL(aux_read(AUX_MU_STAT_REG)));
}

void fill_rxfifo(void)
{
	unsigned char level;

	level = RX_LEVEL(aux_read(AUX_MU_STAT_REG));

	if (level > 0)
		serialEvent();
}

static void Serial_init(void *object)
{
		Serial *ser = object;

		ser->begin = begin;
		ser->printf = tfp_printf;
		ser->read = read;
		ser->available = available;
};

CLASS(Serial, Serial_init);
