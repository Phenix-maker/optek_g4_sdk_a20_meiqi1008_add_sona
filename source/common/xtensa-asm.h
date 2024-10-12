/* Copyright (c) 2006 Tensilica Inc.  ALL RIGHTS RESERVED.

   Redistribution and use in source and binary forms, with or without 
   modification, are permitted provided that the following conditions are met: 

   1. Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
   IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL TENSILICA
   INCORPORATED BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  */

#include <xtensa/coreasm.h>

# Define macros for leaf function entry and return, supporting either the
# standard register windowed ABI or the non-windowed call0 ABI.  These
# macros do not allocate any extra stack space, so they only work for
# leaf functions that do not need to spill anything to the stack.

	.macro leaf_entry reg, size
#if XCHAL_HAVE_WINDOWED && !__XTENSA_CALL0_ABI__
	entry \reg, \size
#else
	/* do nothing */
#endif
	.endm

	.macro leaf_return
#if XCHAL_HAVE_WINDOWED && !__XTENSA_CALL0_ABI__
	retw
#else
	ret
#endif
	.endm

	.macro	src_b	r, w0, w1
#ifdef __XTENSA_EB__
	src	\r, \w0, \w1
#else
	src	\r, \w1, \w0
#endif
	.endm

	.macro	ssa8	r
#ifdef __XTENSA_EB__
	ssa8b	\r
#else
	ssa8l	\r
#endif
	.endm

#if XCHAL_HAVE_BE
#define	MASK0 0xff000000
#define	MASK1 0x00ff0000
#define	MASK2 0x0000ff00
#define	MASK3 0x000000ff
#else
#define	MASK0 0x000000ff
#define	MASK1 0x0000ff00
#define	MASK2 0x00ff0000
#define	MASK3 0xff000000
#endif

