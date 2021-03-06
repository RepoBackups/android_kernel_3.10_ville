/*
 * Copyright (C) 2008 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/linkage.h>
#include <asm/assembler.h>

		.text

		.global fiq_glue_end

		/* fiq stack: r0-r15,cpsr,spsr of interrupted mode */

ENTRY(fiq_glue)
<<<<<<< HEAD
		/* store pc, cpsr from previous mode */
		mrs	r12, spsr
		sub	r11, lr, #4
		subs	r10, #1
		bne	nested_fiq

		stmfd	sp!, {r11-r12, lr}
=======
		/* store pc, cpsr from previous mode, reserve space for spsr */
		mrs	r12, spsr
		sub	lr, lr, #4
		subs	r10, #1
		bne	nested_fiq

		str	r12, [sp, #-8]!
		str	lr, [sp, #-4]!
>>>>>>> common/android-3.10.y

		/* store r8-r14 from previous mode */
		sub	sp, sp, #(7 * 4)
		stmia	sp, {r8-r14}^
		nop

		/* store r0-r7 from previous mode */
		stmfd	sp!, {r0-r7}

		/* setup func(data,regs) arguments */
		mov	r0, r9
		mov	r1, sp
		mov	r3, r8

		mov	r7, sp

		/* Get sp and lr from non-user modes */
		and	r4, r12, #MODE_MASK
		cmp	r4, #USR_MODE
		beq	fiq_from_usr_mode

		mov	r7, sp
		orr	r4, r4, #(PSR_I_BIT | PSR_F_BIT)
		msr	cpsr_c, r4
		str	sp, [r7, #(4 * 13)]
		str	lr, [r7, #(4 * 14)]
		mrs	r5, spsr
		str	r5, [r7, #(4 * 17)]

		cmp	r4, #(SVC_MODE | PSR_I_BIT | PSR_F_BIT)
		/* use fiq stack if we reenter this mode */
		subne	sp, r7, #(4 * 3)

fiq_from_usr_mode:
		msr	cpsr_c, #(SVC_MODE | PSR_I_BIT | PSR_F_BIT)
		mov	r2, sp
		sub	sp, r7, #12
		stmfd	sp!, {r2, ip, lr}
		/* call func(data,regs) */
		blx	r3
		ldmfd	sp, {r2, ip, lr}
		mov	sp, r2

		/* restore/discard saved state */
		cmp	r4, #USR_MODE
		beq	fiq_from_usr_mode_exit

		msr	cpsr_c, r4
		ldr	sp, [r7, #(4 * 13)]
		ldr	lr, [r7, #(4 * 14)]
		msr	spsr_cxsf, r5

fiq_from_usr_mode_exit:
		msr	cpsr_c, #(FIQ_MODE | PSR_I_BIT | PSR_F_BIT)

		ldmfd	sp!, {r0-r7}
<<<<<<< HEAD
		add	sp, sp, #(7 * 4)
		ldmfd	sp!, {r11-r12, lr}
exit_fiq:
		msr	spsr_cxsf, r12
		add	r10, #1
		movs	pc, r11
=======
		ldr	lr, [sp, #(4 * 7)]
		ldr	r12, [sp, #(4 * 8)]
		add	sp, sp, #(10 * 4)
exit_fiq:
		msr	spsr_cxsf, r12
		add	r10, #1
		cmp	r11, #0
		moveqs	pc, lr
		bx	r11 /* jump to custom fiq return function */
>>>>>>> common/android-3.10.y

nested_fiq:
		orr	r12, r12, #(PSR_F_BIT)
		b	exit_fiq

fiq_glue_end:

<<<<<<< HEAD
ENTRY(fiq_glue_setup) /* func, data, sp */
		mrs		r3, cpsr
=======
ENTRY(fiq_glue_setup) /* func, data, sp, smc call number */
		stmfd		sp!, {r4}
		mrs		r4, cpsr
>>>>>>> common/android-3.10.y
		msr		cpsr_c, #(FIQ_MODE | PSR_I_BIT | PSR_F_BIT)
		movs		r8, r0
		mov		r9, r1
		mov		sp, r2
<<<<<<< HEAD
		moveq		r10, #0
		movne		r10, #1
		msr		cpsr_c, r3
=======
		mov		r11, r3
		moveq		r10, #0
		movne		r10, #1
		msr		cpsr_c, r4
		ldmfd		sp!, {r4}
>>>>>>> common/android-3.10.y
		bx		lr

