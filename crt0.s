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

.macro save_regs
	@ allocate stack frame on current stack
	sub sp, sp, #72

	stmia   sp, {r0 - r12}  @ Save user registers (now in svc mode) r0-r12
	add r0, sp, #72      @ grab pointer to old stack
	mov r1, lr
	mov r2, pc
	add r5, sp, #52		@ placing sp at r13 location
	stmia   r5, {r0 - r2}   @ save sp_SVC, lr_SVC, pc
	mov r0, sp      @ save current stack into r0 (param register)
.endm

.globl _start
_start:
    mov sp,#0x8000
	save_regs
    bl c_startup
hang: bl loop
	bl fill_rxfifo
	b hang

.section .vectors, "ax"
.org 0x00
	ldr pc, _reset
.org 0x04
	ldr pc, _undefined_instruction
.org 0x08
	ldr pc, _software_interrupt
.org 0x0C
	ldr pc, _prefetch_abort
.org 0x10
	ldr pc, _data_abort
.org 0x14
	ldr pc, _unused_handler
.org 0x18
	ldr pc, _interrupt
.org 0x1C
	ldr pc, _fast_interrupt

_reset:						.word __reset
_undefined_instruction:		.word undefined_handler
_software_interrupt:		.word swi_handler
_prefetch_abort:			.word prefetch_abort_handler
_data_abort:				.word data_abort_handler
_unused_handler:			.word __reset
_interrupt:					.word interrupt_handler
_fast_interrupt:			.word fiq_handler

__reset: b _start
