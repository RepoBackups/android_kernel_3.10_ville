#ifndef __ARM_MMU_H
#define __ARM_MMU_H

#ifdef CONFIG_MMU

typedef struct {
#ifdef CONFIG_CPU_HAS_ASID
<<<<<<< HEAD
	u64 id;
#endif
	unsigned int kvm_seq;
=======
	atomic64_t	id;
#else
	int		switch_pending;
#endif
	unsigned int	vmalloc_seq;
>>>>>>> common/android-3.10.y
	unsigned long	sigpage;
} mm_context_t;

#ifdef CONFIG_CPU_HAS_ASID
#define ASID_BITS	8
#define ASID_MASK	((~0ULL) << ASID_BITS)
<<<<<<< HEAD
#define ASID(mm)	((mm)->context.id & ~ASID_MASK)
=======
#define ASID(mm)	((mm)->context.id.counter & ~ASID_MASK)
>>>>>>> common/android-3.10.y
#else
#define ASID(mm)	(0)
#endif

#else

/*
 * From nommu.h:
 *  Copyright (C) 2002, David McCullough <davidm@snapgear.com>
 *  modified for 2.6 by Hyok S. Choi <hyok.choi@samsung.com>
 */
typedef struct {
	unsigned long	end_brk;
} mm_context_t;

#endif

#endif
