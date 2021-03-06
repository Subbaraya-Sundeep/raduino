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

#include <timer.h>
#include <gpio.h>
#include <intc.h>
#include <uart.h>
#include <config.h>

extern Serial Serial1;
static bool flag;

OBJECT(Timer, Timer1);

void timer_isr(unsigned int irq, void *timer_data)
{
	timer_write(ARM_TIMER_CLRIRQ, 1);

	Timer1.msecs += TICKER_RATE;

	if (Timer1.msecs == 1000) {
		Timer1.secs++;
		Timer1.msecs = 0;
		digitalWrite(ACTIVE, flag);
		flag = !flag;
	}

	if(Timer1.secs == 60) {
		Timer1.mins++;
		Timer1.secs = 0;
	}

	if (Timer1.mins == 60) {
		Timer1.hours++;
		Timer1.mins = 0;
	}

	if (Timer1.hours == 24)
		Timer1.hours = 0;

}

void get_time(unsigned int *hrs, unsigned int *mins, unsigned int *secs,
				unsigned int *msecs)
{
	*hrs = Timer1.hours;
	*mins = Timer1.mins;
	*secs = Timer1.secs;
	*msecs = Timer1.msecs;
}

/* 
 * A delay of ms or above than that is guaranteed.
 * TODO: Do not use rely on interrupts
 * FIXME:This one will not work if ms is not multiple of TICKER_RATE
 */
void delay_ms(unsigned int ms)
{
	unsigned int tmp = Timer1.msecs;

	if (ms < TICKER_RATE)
		return; /*FIXME:*/
	while (ms > 0) {
		if (Timer1.msecs != tmp) {
			ms = ms - TICKER_RATE;
			tmp = Timer1.msecs;
		}
	}
}

void tick_init(unsigned int ms)
{
	unsigned int reg;

	/* Timer input clock 1MHz */
	timer_write(ARM_TIMER_LOAD, (ms * 1000) - 1);	
	reg = timer_read(ARM_TIMER_CTRL);
	reg |= ARM_TIMER_INTEN | ARM_TIMER_ON | ARM_TIMER_32BIT;
	timer_write(ARM_TIMER_CTRL, reg);	

	irq_add(0, timer_isr, NULL);
	intc_write(INTC_IER_BASIC, INTC_TIMER); /* Enable timer interrupt */
}

static void Timer_init(void *object)
{
		Timer *timer = object;

		timer->msecs = 0;
		timer->secs = 0;
		timer->mins = 0;
		timer->hours = 0;
}

CLASS(Timer, Timer_init);
