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

#include <gpio.h>

const struct pin_info pin_lkp[] =
{ 
		{ INVAL, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 1, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 2, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 3, 2, GPFSEL0, GPSET0, GPCLR0, GPLEV0 }, 
		{ 4, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 5, 3, GPFSEL0, GPSET0, GPCLR0, GPLEV0 }, 
		{ 6, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 7, 4, GPFSEL0, GPSET0, GPCLR0, GPLEV0 }, 
		{ 8, 14, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 9, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 10, 15, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 11, 17, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 12, 18, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 13, 27, GPFSEL2, GPSET0, GPCLR0, GPLEV0 }, 
		{ 14, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 15, 22, GPFSEL2, GPSET0, GPCLR0, GPLEV0 }, 
		{ 16, 23, GPFSEL2, GPSET0, GPCLR0, GPLEV0 }, 
		{ 17, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 18, 24, GPFSEL2, GPSET0, GPCLR0, GPLEV0 }, 
		{ 19, 10, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 20, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 21, 9, GPFSEL0, GPSET0, GPCLR0, GPLEV0 }, 
		{ 22, 25, GPFSEL2, GPSET0, GPCLR0, GPLEV0 }, 
		{ 23, 11, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 24, 8, GPFSEL0, GPSET0, GPCLR0, GPLEV0 }, 
		{ 25, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 26, 7, GPFSEL0, GPSET0, GPCLR0, GPLEV0 }, 
		{ 27, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 28, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 29, 5, GPFSEL0, GPSET0, GPCLR0, GPLEV0 }, 
		{ 30, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 31, 6, GPFSEL0, GPSET0, GPCLR0, GPLEV0 }, 
		{ 32, 12, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 33, 13, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 34, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 35, 19, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 36, 16, GPFSEL1, GPSET0, GPCLR0, GPLEV0 }, 
		{ 37, 26, GPFSEL2, GPSET0, GPCLR0, GPLEV0 }, 
		{ 38, 20, GPFSEL2, GPSET0, GPCLR0, GPLEV0 }, 
		{ 39, INVAL,  INVAL, INVAL, INVAL, INVAL }, 
		{ 40, 21, GPFSEL2, GPSET0, GPCLR0, GPLEV0 }, 
		{ 41, 35, GPFSEL3, GPSET1, GPCLR1, GPLEV1 }, /* power - active low */
		{ 42, 47, GPFSEL4, GPSET1, GPCLR1, GPLEV1 } /* active - active high */ 
};

void pinMode(int nr, enum mode mode)
{
	int gpio;
	int bit;
	unsigned int reg;

	if (nr < 0 || nr >= MAX_PINS)
		return;

	gpio = pin_lkp[nr].gpio;	

	if (gpio == INVAL)
		return;

	bit = (gpio - ((gpio/10) * 10)) * 3;
	reg = gpio_read(pin_lkp[nr].func_sel);
	reg &= ~(7 << bit);
	reg |= (mode << bit);
	gpio_write(pin_lkp[nr].func_sel, reg);
}

void digitalWrite(int nr, enum level value)
{
	int gpio;
	int bit;
	unsigned int reg;
	unsigned offset;

	if (nr < 0 || nr >= MAX_PINS)
		return;

	gpio = bit = pin_lkp[nr].gpio;

	if (gpio == INVAL)
		return;

	if (gpio > 31)
		bit = gpio - 32;

	if (value == HIGH)
		offset = pin_lkp[nr].set;
	else
		offset = pin_lkp[nr].clear;

	reg = gpio_read(offset);
	reg |= (1 << bit);
	gpio_write(offset, reg);
}

int digitalRead(int nr)
{
	int gpio;
	int bit;
	unsigned int reg;

	if (nr < 0 || nr >= MAX_PINS)
		return 0;

	gpio = bit = pin_lkp[nr].gpio;

	if (gpio == INVAL)
		return 0;

	if (gpio > 31)
		bit = gpio - 32;

	reg = gpio_read(pin_lkp[nr].level);

	return ((reg & (1 << bit)) >> bit);
}
