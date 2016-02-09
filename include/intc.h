#ifndef INTC_H
#define INTC_H

#include "periph.h"
#include "object.h"

typedef void (*isr_t)(unsigned int irq, void *pdata);

typedef struct
{
	unsigned int irq;	
	void *pdata;
	isr_t handler;
} irq_desc;

typedef struct
{
	Object obj;

	irq_desc idescs[NR_IRQS];
} Intc;

void irq_add(unsigned irq, isr_t handler, void *pdata);

#endif
