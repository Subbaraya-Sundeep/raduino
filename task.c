#include <uart.h>
#include <task.h>

unsigned int stacks[N_TASKS][STACK_SIZE] __data__  __attribute__((aligned(8)));
extern Serial Serial1;

void task1()
{
	Serial1.printf("Hi..task 1\r\n");
}
void task2()
{
	Serial1.printf("Hi..task 2\r\n");
}
void task3()
{
	Serial1.printf("Hi..task 3\r\n");
}
void task4()
{
	Serial1.printf("Hi..task 4\r\n");
}

struct task_desc tdescs[N_TASKS] =
{
	{&stacks[0][STACK_SIZE-1], "Task0", 0, &tdescs[1], task1},
	{&stacks[1][STACK_SIZE-1], "Task1", 1, &tdescs[2], task2},
	{&stacks[2][STACK_SIZE-1], "Task2", 2, &tdescs[3], task3},
	{&stacks[3][STACK_SIZE-1], "Task3", 3, &tdescs[0], task4},
};

unsigned int current __data__;

typedef void (*tfunc)(void);

void task_init(void)
{
	int i;
	struct task_desc *td;

	td = tdescs;
	i = 0;

	while (i < 4) {
		Serial1.printf("name:%s id:%d stackptr:%x next:%x\r\n", td->name, td->id, td->stack, td->next);
		td->task();
		td = td->next;
		i++;
	}

	Serial1.printf("current:%d\r\n", current);
}

void stack_init(struct task_desc *td)
{
	/* save regs and return top of stack */	
}
