#include <gpio.h>
#include <uart.h>
#include <intc.h>

extern Serial Serial1;
extern Intc Intc1;

void __attribute__((interrupt("UNDEF"))) undefined_handler(void)
{
	digitalWrite(ACTIVE, HIGH);
}

void/* __attribute__((interrupt("IRQ")))*/interrupt_handler(void)
{
	Intc1.idescs[0].handler(Intc1.idescs[0].irq, Intc1.idescs[0].pdata);
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
