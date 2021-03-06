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

#define ALIGNMENT		4

#include <object.h>
#include <periph.h>
#include <logger.h>
#include <task.h>
#include <timer.h>
#include <config.h>

extern unsigned int bss_start; 
extern unsigned int bss_end; 
extern unsigned char classes_start; 
extern unsigned char classes_end; 
extern unsigned char objects_start; 
extern unsigned char objects_end; 
extern unsigned int vectors_start; 
extern unsigned int vectors_end; 

extern void setup(void);

struct cpu_regs bl_regs __data__;

unsigned char *roundup(unsigned char *addr, int alignment)
{
	int rem;

	if(alignment == 0)  
  		return addr;  

	rem = (unsigned) addr % alignment; 
	if (rem == 0)
		return addr; 

	return addr + alignment - rem; 
}

bool object_belongs_to_class(Class *class, Object *obj)
{
	char *first	 = class->name;
	char *second = obj->name;

	/* just string comparison */
	while (*first == *second) {
		if (*first == '\0' || *second == '\0')
			break;
      first++;
      second++;
	}
 
	if (*first == '\0' && *second == '\0')
		return true;
	else
		return false;	
}

void init_objects_for_class(Class *class)
{
	unsigned char *obj_start = &objects_start;
	unsigned char *obj_end = &objects_end;
	Object *obj;

	while (obj_start != obj_end)
	{
		obj = (Object *)obj_start;
		if (!obj->init && object_belongs_to_class(class, obj)) {
			class->obj_init(obj);
			obj->init = true;
		}
		obj_start = roundup(obj_start + obj->size, ALIGNMENT);
	}
}

void c_startup (unsigned int *stack)
{
	unsigned int *bss = &bss_start;
	unsigned int *bssend = &bss_end;
	unsigned int *vstart = &vectors_start;
	unsigned int *vend = &vectors_end;
	unsigned char *class_start = &classes_start;
	unsigned char *class_end = &classes_end;
	unsigned int *vector_table = (unsigned int *)0x00;
	Class *class;
	struct cpu_regs *regs = &bl_regs;

	/* 
	 * Clear BSS ASAP since it looks like zero initialized 
	 * global variables also go into bss
	 */
	while (bss < bssend)
		*bss++ = 0;

#ifdef EARLY_DEBUG
	int i = 0;
	logger_init();
	log_printf("register values from boot loader\r\n");
	for (i = 0; i < 13; i++)
		log_printf("r%d:0x%x\r\n", i, regs->regs[i]);

	log_printf("sp:0x%x\r\n", regs->sp);
	log_printf("lr:0x%x\r\n", regs->lr);
	log_printf("pc:0x%x\r\n", regs->pc);
	log_printf("cpsr:0x%x\r\n", regs->cpsr);
	log_printf("spsr:0x%x\r\n", regs->spsr);
	log_printf("cpu is in %s mode\r\n", cpu_mode());
	log_printf("Stack:%x\r\n", stack);
#endif

	while (vstart < vend)
		*vector_table++ = *vstart++;

	while (class_start != class_end)
	{
		class = (Class *)class_start;
		init_objects_for_class(class);
		class_start = roundup(class_start + sizeof(Class), ALIGNMENT);
	}

	sti();
	task_init();
	cli();
	tick_init(TICKER_RATE);
	setup();
}
