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

#include "object.h"
#include "periph.h"

extern unsigned int bss_start; 
extern unsigned int bss_end; 
extern unsigned char classes_start; 
extern unsigned char classes_end; 
extern unsigned char objects_start; 
extern unsigned char objects_end; 

extern void setup(void);

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

void c_startup (void)
{
	unsigned int *bss = &bss_start;
	unsigned int *bssend = &bss_end;
	unsigned char *class_start = &classes_start;
	unsigned char *class_end = &classes_end;
	Class *class;

	/* Clear BSS */
	while(bss < bssend)
		*bss++ = 0;

	while (class_start != class_end)
	{
		class = (Class *)class_start;
		init_objects_for_class(class);
		class_start = roundup(class_start + sizeof(Class), ALIGNMENT);
	}

	setup();
}
