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

#include "uart.h"
#include "gpio.h"
#include "string.h"

OBJECT(Serial, Serial1);
OBJECT(String, s1);
OBJECT(String, s2);

int i;

void setup()
{
	pinMode(ACTIVE, OUTPUT);
	pinMode(POWER, OUTPUT);

	digitalWrite(ACTIVE, HIGH);
	digitalWrite(POWER, LOW);

	delay();

	Serial1.begin(115200);
	digitalWrite(ACTIVE, LOW);
	digitalWrite(POWER, HIGH);
}

void loop()
{
	Serial1.printf("Hello from Serial1\r\n");

	Serial1.printf("initial string:%s\r\n", s1.buffer);
	s1.append(&s1, "string added to s1");
	s2.append(&s2, "string added to s2");
	s1.add(&s1, '#');
	s2.add(&s2, '&');
	s1.append(&s1, "the end of s1");
	s2.append(&s2, "the end of s2");

	Serial1.printf("int from bss %d\r\n", i);
	Serial1.printf("%s\r\n", s1.buffer);
	Serial1.printf("%s\r\n", s2.buffer);
	delay();
	s1.clear(&s1);
	s2.clear(&s2);

	Serial1.printf("after clearing:%s\r\n", s1.buffer);
	Serial1.printf("after clearing:%s\r\n", s2.buffer);
	s1.append(&s1, "Added again to s1");
	s2.append(&s2, "Added again to s2");
	Serial1.printf("%s\r\n", s1.buffer);
	Serial1.printf("%s\r\n", s2.buffer);
}
