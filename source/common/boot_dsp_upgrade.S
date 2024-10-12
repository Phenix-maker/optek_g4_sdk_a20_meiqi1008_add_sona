#include "oem.h"

#include "flash_boot.h"


#if 1//def UPGRADE_PROJECT_ENABLE

#if 1//def UPGRADE_PROJECT_FIRST_START

	.global boot_dsp_upgrade_project_start

#ifdef BOOT_FROM_FLASH
#define BOOT_DSP_UPGRADE    ROM_DSP_UPGRADE_PRJOJECT_START_ADDR
#else
#define BOOT_DSP_UPGRADE    ROM_BOOT_ADDR
#endif

	.text
	.align      4
	.section    .flash_boot_switch_text, "ax"

	.align  4

JUMP_ADDR:
	.word	BOOT_DSP_UPGRADE

	.align  4

boot_dsp_upgrade_project_start:

//	caLL4    Sdram_Init
//	caLL4    Flash_Boot_Copy
	l32r     a0, JUMP_ADDR
	jx       a0

#endif //UPGRADE_PROJECT_FIRST_START

#endif //UPGRADE_PROJECT_ENABLE
