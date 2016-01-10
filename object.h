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

#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>

#define _ALIGN(x)	__attribute__((aligned((x))))

#define _CLASS_INIT(name)	__attribute__((section(name))) 		\
							__attribute((aligned(4)))

#define OBJECT(Class, object)	\
	Class object __attribute__ ((section("objects"))) = {	\
		.obj = {											\
			.name = #Class,									\
			.size = sizeof(Class),							\
		},													\
	} 

#define CLASS(class_name, fn)											\
	Class class_name##_class __attribute__ ((section("classes"))) =		\
	{	.name = #class_name,											\
		.obj_init = fn,											\
	}

typedef struct 
{
	char name[20];
	unsigned int size;
	bool init;
} Object _ALIGN(4);

typedef struct
{
	char name[20];
	void (*obj_init)(void *);
} Class _ALIGN(4);

#endif
