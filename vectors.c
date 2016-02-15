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
#include <uart.h>
#include <intc.h>

extern Serial Serial1;
extern Intc Intc1;
extern unsigned int Reg_0;
void schedule(void);

void __attribute__((interrupt("UNDEF"))) undefined_handler(void)
{
	digitalWrite(ACTIVE, HIGH);
}

void interrupt_handler(void)
{
	Intc1.idescs[0].handler(Intc1.idescs[0].irq, Intc1.idescs[0].pdata);
	schedule();
}

void __attribute__((interrupt("FIQ"))) fiq_handler(void)
{
	digitalWrite(ACTIVE, HIGH);
}

void __attribute__((interrupt("SWI"))) swi_handler(void)
{
	digitalWrite(ACTIVE, HIGH);
}

void __attribute__((interrupt("ABORT"))) prefetch_abort_handler(void)
{
	digitalWrite(ACTIVE, HIGH);
}

void __attribute__((interrupt("ABORT"))) data_abort_handler(void)
{
	digitalWrite(ACTIVE, HIGH);
}
