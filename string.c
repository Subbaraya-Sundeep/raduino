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

#include <string.h>

static void clear(void *obj)
{
	String *str = obj;
 
	str->index = 0;
	str->buffer[str->index] = '\0';
}

static void add(void *obj, char c)
{
	String *str = obj;
	unsigned int size = sizeof(str->buffer) - 1; 

	if (str->index < size)
		str->buffer[str->index++] = c;

	str->buffer[str->index] = '\0';
}

static void append(void *obj, char *tmp)
{
	String *str = obj;
	unsigned int size = sizeof(str->buffer) - 1; 

	while (str->index < size && *tmp != '\0')
	{
		str->buffer[str->index++] = *tmp;
		tmp++;
	}

	str->buffer[str->index] = '\0';
}

static void String_init(void *object)
{
		String *str = object;

		str->index = 0;
		str->append = append;
		str->clear = clear;
		str->add = add;
};

CLASS(String, String_init);
