#include "os_type.h"


#ifdef FREE_RTOS_LIB_BUILD
#ifdef FREE_RTOS
#undef FREE_RTOS
#endif
#endif


#ifdef FREE_RTOS
/* _exit function for the Xtensa semihosting simulator.  */

/*
 * Copyright (c) 1998-2008 by Tensilica Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of Tensilica Inc.
 * They may not be modified, copied, reproduced, distributed, or disclosed to
 * third parties in any manner, medium, or form, in whole or in part, without
 * the prior written consent of Tensilica Inc.
 */

#include <sys/reent.h>
#include <xtensa/simcall.h>
#include <xtensa/config/core-isa.h>
#include <xtensa/config/core.h>

#include "mem_reloc.h"

void _exit (int n) __LIBKERNAL_RAM_TEXT;
void _exit (int n)
{
  /* Sync dirty data to memory before terminating.  */
#if XCHAL_DCACHE_IS_COHERENT
  xthal_cache_coherence_optout ();
#elif XCHAL_DCACHE_IS_WRITEBACK
  xthal_dcache_all_writeback ();
#endif //XCHAL_DCACHE_IS_COHERENT

  /*  Issue EXTW instruction (or MEMW if pre-LX) to sync memory and queues:  */
#pragma flush

  {
  register int a2 __asm__ ("a2") = SYS_exit;
  register int a3 __asm__ ("a3") = n;

    __asm__ volatile ("simcall"
		      : : "a" (a2), "a" (a3));
  }
  /*NOTREACHED*/
}

#ifdef DEFUN_MACRO_ORGINALL
void _DEFUN (exit, (code), int code)
{
  __call_exitprocs (code, NULL);

  if (_GLOBAL_REENT->__cleanup)
    (*_GLOBAL_REENT->__cleanup) (_GLOBAL_REENT);
  _exit (code);
}
#endif

#else

void _exit (int n) __attribute__ ((section (".internal_ram_1_text")));
void _exit (int n)
{
	/* Sync dirty data to memory before terminating.  */
	#if XCHAL_DCACHE_IS_COHERENT
	xthal_cache_coherence_optout ();
	#elif XCHAL_DCACHE_IS_WRITEBACK
	xthal_dcache_all_writeback ();
	#endif

	/*  Issue EXTW instruction (or MEMW if pre-LX) to sync memory and queues:  */
#pragma flush

	#ifdef SYS_EXIT_ORGINALL
	{
		register int a2 __asm__ ("a2") = SYS_exit;
		register int a3 __asm__ ("a3") = n;

		__asm__ volatile ("simcall"
			: : "a" (a2), "a" (a3));
		}

		/*NOTREACHED*/
	#endif
}

#endif //FREE_RTOS
