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

.macro save_bl_regs
	stmia   sp, {r0 - r12}  @ Save user registers (now in svc mode) r0-r12
	mov r0, sp      @ not bootloader stack :(
	mov r1, lr
	mov r2, pc
	mrs r3, cpsr
	mrs r4, spsr
	add r5, sp, #52		@ placing sp at r13 location
	stmia   r5, {r0 - r4}   @ save sp_SVC, lr_SVC, pc
.endm

/*
sp_svc ----> SVC mode SP points here before interrupt
 			spsr (cpsr of SVC mode)
			SP (of SVC mode)
			r14 (lr of SVC mode)
			lr (pc of SVC mode)
			r12
			r11
			r10
			r9
			r8
			r7
			r6
			r5
			r4
			r3
			r2
			r1
sp_irq --->	r0
*/
.macro save_context
	sub sp, sp, #68
	stmia   sp, {r0 - r12,lr}  @ Save user registers (now in svc mode) r0-r12

	cpsid i, #19 /* to svc */
	mov r0, lr
	cpsid i, #18 /* to interrupt */
	/* check alignment hole */
	add r1, sp, #68
	mrs r2, spsr
	add r5, sp, #56
	stmia   r5, {r0 - r2}
.endm

.macro restore_context
	add sp, sp, #56
	ldmia  sp, {r0 - r2}  @ Save user registers (now in svc mode) r0-r12
	msr spsr, r2

	/* check alignment hole ? */

	cpsid i, #19 /* to SVC */
	mov sp, r1
	mov r14, r0

	cpsid i, #18 /* to interrupt mode */

	sub sp, sp, #56
	ldmia  sp, {r0 - r12,lr}
	movs pc, lr /* go bck to task running in SVC mode */
.endm

.macro load_init_task
	add sp, sp, #56
	ldmia  sp, {r0 - r2}  @ Save user registers (now in svc mode)
	msr spsr, r2
	/* check alignment hole ? */
	sub sp, sp, #56
	ldmia  sp, {r0 - r12,lr}
.endm

.globl _start
_start: 

/* I do not rely on U-boot or Rpi boot code for proper initial values */
	cps #19
	ldr sp,=bl_regs
	save_bl_regs

	mrc p15, 0, r1, c1, c0, 0   @ SCTLR (System Control Register)
    bic r1, r1, #0x1000  @ clear High vecs - SCTLR.V = 0
    mcr p15, 0, r1, c1, c0, 0

/* U-boot modifies VBAR we have to set to 0x00 */
	mov r1, #0x0000
    mcr p15, 0, r1, c12, c0, 0  @ Set VBAR

	ldr sp,=init_stack
	mov r1, #512
	and r1, r1, #0xFFFFFFF8 @ sp should be 8 byte aligned
	add sp, sp, r1
	mov r0, sp

	bl c_startup

	bl get_tos

	mov sp, r0 /* current/task1 stack pointer */

	load_init_task

	cpsid i

	mov sp,#0x8000

	movs pc, lr /* go bck to task running in SVC mode */

hang: b hang

irq_handler:
	ldr sp,=Reg_0 /* sp_IRQ */
	str r0, [sp] /* save r0 in Reg_0 */

	cps #19 /* to SVC */
	mov r0, sp 

	cps #18 /* to Interrupt */
	mov sp, r0 /* Interrupt stack points to top of SVC stack */

	/* Add stack adjustment if required ? */
	ldr r0,=Reg_0
	ldr r0, [r0] /* load r0 from Reg_0 */

	save_context

	/* Save top of stack in task desc */
	mov r0, sp
	bl save_tos

    bl interrupt_handler

	/* Get top of stack from task desc */
	bl get_tos
	mov sp, r0

	restore_context

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
