#ifndef _TASK_H
#define _TASK_H

#include "config.h"

struct task_desc
{
	unsigned int *stack;
	char name[20];
	unsigned int id;
	struct task_desc *next;
	void (*task)(void);
};

void task_init(void);

#endif
