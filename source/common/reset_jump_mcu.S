#include "reset.h"


#ifdef MCU_CORE_ENABLE

#define RESET_MCU_ADDR      ROM_MCU_START_ADDR


	/*
	*.word	0x00000106
	*.word	0x6A000420
	*.word	0xA0FFFF01
	*/


#ifdef ROM_START_ADD_SAME
	.global _Reset_DSP_Core
#else
	.global _Reset_MCU
#endif

	.text

#ifdef ROM_START_ADD_SAME
	.section    .Reset_DSP_Boot.text, "ax"
#else
	.section    .Reset_MCU_Boot.text, "ax"
#endif

	.align  4

_Reset_MCU:
	j    _Reset_MCU_Jump // jump around literal pool

	.literal_position      // tells assembler place literals here
	.align  4


JUMP_MCU_ADDR:
	.word	RESET_MCU_ADDR

	.align  4

#ifdef ROM_START_ADD_SAME
_Reset_DSP_Core_Jump:
#else
_Reset_MCU_Jump:
#endif
	l32r     a0, JUMP_MCU_ADDR
	jx       a0

#else

	.text

#ifdef ROM_START_ADD_SAME
	.section    .Reset_DSP_Boot.text, "ax"
#else
	.section    .Reset_MCU_Boot.text, "ax"
#endif

	.align  4

#if 0
	.word	0xFFFFFFFF
#else
	/*Risc V:halt*/
	.word	0x10500073
#endif
	.word	0xFFFFFFFF
	.word	0xFFFFFFFF
	.word	0xFFFFFFFF

#endif //MCU_CORE_ENABLE
