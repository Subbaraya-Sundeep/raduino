#include <gpio.h>

void __attribute__((interrupt("UNDEF"))) undefined_handler(void)
{
	digitalWrite(ACTIVE, HIGH);
}

void __attribute__((interrupt("IRQ"))) interrupt_handler(void)
{
	digitalWrite(ACTIVE, HIGH);
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