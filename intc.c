#include <periph.h>
#include <intc.h>

OBJECT(Intc, Intc1);

void irq_add(unsigned irq, isr_t handler, void *pdata)
{
	Intc1.idescs[irq].irq = irq;
	Intc1.idescs[irq].handler = handler;
	Intc1.idescs[irq].pdata = pdata;
}

static void Intc_init(void *object)
{
		Intc *intc = object;
}

CLASS(Intc, Intc_init);
