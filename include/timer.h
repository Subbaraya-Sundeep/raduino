#ifndef _TIMER_H
#define _TIMER_H

#include <periph.h>

typedef struct
{
	Object obj;

	volatile unsigned int msecs;
	volatile unsigned int secs;
	volatile unsigned int mins;
	volatile unsigned int hours;
} Timer;


void tick_init(unsigned int ms);
void get_time(unsigned int *hrs, unsigned int *mins, unsigned int *secs,
				unsigned int *msecs);
void delay_ms(unsigned int ms);

#endif
