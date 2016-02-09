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

#include <config.h>

.macro save_regs
	@ allocate stack frame on current stack
	stmia   sp, {r0 - r12}  @ Save user registers (now in svc mode) r0-r12
	mov r0, sp      @ not bootloader stack :(
	mov r1, lr
	mov r2, pc
	mrs r3, cpsr
	mrs r4, spsr
	add r5, sp, #52		@ placing sp at r13 location
	stmia   r5, {r0 - r4}   @ save sp_SVC, lr_SVC, pc
.endm

.globl _start
_start: 
/* Set stack for interrupts */
	cps #18
    mov sp,#0x8000000

/* I do not rely on U-boot or Rpi boot code for proper initial values */
	cps #19
	ldr sp,=bl_regs
	save_regs
	mrc p15, 0, r1, c1, c0, 0   @ SCTLR (System Control Register)
    bic r1, r1, #0x1000  @ clear High vecs - SCTLR.V = 0
    mcr p15, 0, r1, c1, c0, 0

/* U-boot modifies VBAR we have to set to 0x00 */
	mov r1, #0x0000
    mcr p15, 0, r1, c12, c0, 0  @ Set VBAR

	ldr r1,=current
	mov r2, #2
	str r2, [r1]
	ldr sp,=stacks
	mov r1, #STACK_SIZE
	subs r1, r1, #8 @ sp should be 8 byte aligned
	add sp, sp, r1
	mov r0, sp

	bl c_startup

hang: bl loop
	bl fill_rxfifo
	b hang

/* 
	mov r0, #0x8000
	ldr r1, gpsel
	str r0, [r1]

gpsel:		.word 0x3F200020
*/

irq_handler:
	push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl interrupt_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

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
_interrupt:					.word irq_handler
_fast_interrupt:			.word fiq_handler

__reset: b _start
