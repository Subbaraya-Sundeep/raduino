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

#include <uart.h>
#include <task.h>

unsigned int Reg_0 __data__  __attribute__((aligned(8)));

unsigned int init_stack[512]  __data__ __attribute__((aligned(8)));

unsigned int stacks[N_TASKS][STACK_SIZE] __data__ __attribute__((aligned(8)));
extern Serial Serial1;
extern void loop(void);

unsigned char *rounddwn(unsigned int *addr, unsigned int align)
{
	unsigned int rem = (unsigned int)addr % align;

	return ((unsigned char *)addr - rem);
}

void show(unsigned int r0, unsigned spsr)
{
	
	Serial1.printf("r0:0x%x and spsr:0x%x\r\n", r0, spsr);
//	for(;;)
//		Serial1.printf("\r");
		
}

void task1(void)
{
	for (;;) {
		cli();
		Serial1.printf("	cpu running in %s mode for Task 1\r", cpu_mode());
		loop();
		fill_rxfifo();
		sti();
	}
}

void task2(void)
{
	for (;;) {
		cli();
		Serial1.printf("	cpu running in %s mode for Task 2\r", cpu_mode());
		sti();
	}
}

void task3(void)
{
	for (;;) {
		cli();
		Serial1.printf("	cpu running in %s mode for Task 3\r", cpu_mode());
		sti();
	}
}

void task4(void)
{
	for (;;) {
		cli();
		Serial1.printf("	cpu running in %s mode for Task 4\r", cpu_mode());
		sti();
	}
}

struct task_desc tdescs[N_TASKS] =
{
	{&stacks[0][STACK_SIZE-8], "loop",  0, &tdescs[1], task1},
	{&stacks[1][STACK_SIZE-8], "Task1", 1, &tdescs[2], task2},
	{&stacks[2][STACK_SIZE-8], "Task2", 2, &tdescs[3], task3},
	{&stacks[3][STACK_SIZE-8], "Task3", 3, &tdescs[0], task4},
};

struct task_desc *current = &tdescs[0];

typedef void (*tfunc)(void);

void stack_init(struct task_desc *td)
{
	/* save regs and return top of stack */
	*--td->stack = current_cpsr();
	*--td->stack = (unsigned int)td->stack;
	*--td->stack = (unsigned int)td->task;
	*--td->stack = (unsigned int)td->task;
	*--td->stack = reg(r12);
	*--td->stack = reg(r11);
	*--td->stack = reg(r10);
	*--td->stack = reg(r9);
	*--td->stack = reg(r8);
	*--td->stack = reg(r7);
	*--td->stack = reg(r6);
	*--td->stack = reg(r5);
	*--td->stack = reg(r4);
	*--td->stack = reg(r3);
	*--td->stack = reg(r2);
	*--td->stack = reg(r1);
	*--td->stack = reg(r0);
}

void task_init(void)
{
	int i, j;
	struct task_desc *td;
	struct cpu_regs *regs;

	td = tdescs;
	j = 0;

	while (j < N_TASKS) {
		td->stack = (unsigned int *)rounddwn(td->stack, 32); /* ARM sp should be 8 byte aligned */
		Serial1.printf("name:%s id:%d stackptr:%x next:%x\r\n", td->name, td->id, td->stack, td->next);
		stack_init(td);
#if 1
		regs = (struct cpu_regs *)td->stack;

		for (i = 0; i < 13; i++)
			Serial1.printf("r%d:0x%x\r\n", i, regs->regs[i]);

		Serial1.printf("lr:0x%x\r\n", regs->sp);
		Serial1.printf("SP:0x%x\r\n", regs->lr);
		Serial1.printf("spsr:0x%x\r\n", regs->pc);
#endif
		td = td->next;
		j++;
	}

	Serial1.printf("current:%s\r\n", current->name);
}

void save_tos(unsigned int *top)
{
	struct cpu_regs *regs = (struct cpu_regs *)top;
	int i;

//	Serial1.printf("save_tos:%x\r\n", top);
	current->stack = top;
#if 0
	for (i = 0; i < 13; i++)
		Serial1.printf("r%d:addr:0x%X:0x%x\r\n", i, &regs->regs[i], regs->regs[i]);

	Serial1.printf("lr::addr:0x%x:0x%x\r\n", &regs->sp, regs->sp);
	Serial1.printf("SP::addr:0x%x:0x%x\r\n", &regs->lr, regs->lr);
	Serial1.printf("spsr::addr:0x%x:0x%x\r\n", &regs->pc, regs->pc);
#endif
}

unsigned int *get_tos(void)
{
	struct cpu_regs *regs;
	int i;

//	Serial1.printf("get_tos:%x\r\n", current->stack);
	regs = (struct cpu_regs *)current->stack;
#if 0
	for (i = 0; i < 13; i++)
		Serial1.printf("r%d:addr:0x%X:0x%x\r\n", i, &regs->regs[i], regs->regs[i]);

	Serial1.printf("lr::addr:0x%x:0x%x\r\n", &regs->sp, regs->sp);
	Serial1.printf("SP::addr:0x%x:0x%x\r\n", &regs->lr, regs->lr);
	Serial1.printf("spsr::addr:0x%x:0x%x\r\n", &regs->pc, regs->pc);
#endif
	
	return current->stack;
}

void schedule(void)
{
	struct task_desc *next;

	next = current->next;
	current = next;
}
