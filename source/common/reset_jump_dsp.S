#include <xtensa/coreasm.h>
#include <xtensa/simcall.h>

#include "reset.h"
#include "oem.h"


#ifdef DSP_CORE_ENABLE

//#define CACHE_ON

#define PREFECH_ON

#define PREFECT_MODE    0x00000011

#define ICACHE_SIZE 16384
#define DCACHE_SIZE 32768


#define	RESET_DSP_CORE_START_ADDR           ROM_BOOT_START_ADDR

#define	ROM_DSP_CORE_APP_BOOT_START_ADDR    ROM_BOOT_START_ADDR

	/*
	*.word	0x00000106
	*.word	0x6A000420
	*.word	0xA0FFFF01
	*/


#ifdef ROM_START_ADD_SAME
	.global _Reset_DSP_Core
#else
	.global _Reset_DSP_Core
#endif

	.text

#ifdef ROM_START_ADD_SAME
	.section    .Reset_DSP_Boot.text, "ax"
#else
	.section    .Reset_DSP_Boot.text, "ax"
#endif
	.align  4

_Reset_DSP_Core:
	j    _Reset_DSP //jump around literal pool

	.literal_position //tells assembler place literals here
	.align  4

_Reset_DSP:

#ifdef UPGRADE_PROJECT_ENABLE
	/*
	*disable clk_err
	*/
	movi  a2, 0x1A000024    //regCtl_3 addr
	movi  a3, 0x000006c5						
	s32i  a3, a2,0

	movi  a2, 0x1A0000FC    //AON_WRITE_TRANS addr
	movi  a3, 0x8E89BE09    //trans id + addr
	s32i  a3, a2, 0


	#if 0
	/*
	*iram0 and dram0 remap, please note to modify the link file to meet this remap
	*/
	movi  a2, 0x18800034  //base misc ram remap addr
//	movi  a3, 0x0000003F  //256k iram, 416k dram, [5:4] = 2'h3->risc v iram 16k, [5:4] = 2'h1->risc v iram 8k, [5:4] = 2'h0->risc v iram 0k
	movi  a3, 0x00000037  //192k iram, 480k dram, [5:4] = 2'h3->risc v iram 16k, [5:4] = 2'h1->risc v iram 8k, [5:4] = 2'h0->risc v iram 0k
//	movi  a3, 0xg0000033  //128k iram, 544k dram, [5:4] = 2'h3->risc v iram 16k, [5:4] = 2'h1->risc v iram 8k, [5:4] = 2'h0->risc v iram 0k
//	movi  a3, 0x00e00031  // 64k iram, 608k dram, [5:4] = 2'h3->risc v iram 16k, [5:4] = 2'h1->risc v iram 8k, [5:4] = 2'h0->risc v iram 0k
//	movi  a3, 0x00e00030  //  0k iram, 672k dram, [5:4] = 2'h3->risc v iram 16k, [5:4] = 2'h1->risc v iram 8k, [5:4] = 2'h0->risc v iram 0k
	s32i  a3, a2, 0
	#endif


	//set wap to 64 bytes (cmd h77), please not cpu should support wrap
	movi  a2, 0x11770000
	movi  a3, 0x60000000
	s32i  a3, a2,0

	//set spi flash mode
	movi  a2, 0x11052000      //4 bit read
	//@movi  a2, 0x11053000   //4 bit fast read (without send command)

	//movi  a2, 0x11057900    //4 bit fast dtr read (without send command), 9 dummy cloks for GD
	//movi  a2, 0x11057700    //4 bit fast dtr read (without send command), 7 dummy cloks for Puya

	l8ui  a2, a2,0

	/*
	//set wap to 64 bytes (cmd h77), please not cpu should support wrap
	movi  a2, 0x11770000
	movi  a3, 0x60000000
	s32i  a3, a2,0
	*/

	/*
//	movi  a2, 0x11052000   //4 bit read
	movi  a2, 0x11053000   //4 bit fast read (without send command)
//	movi  a2, 0x11057900   //4 bit fast dtr read (without send command), 9 dummy cloks for GD
//	movi  a2, 0x11057700   //4 bit fast dtr read (without send command), 7 dummy cloks for Puya

	l8ui  a2, a2,0
	*/



#ifdef PREFECH_ON
	movi  a2, PREFECT_MODE			
	wsr   a2, PREFCTL // interrupt vector
#endif



	//wdt_pulse should be toggled in 250ms after reset
	/*
	*set REG_WATDOG_UNLOCK = UNLOCK_DWORD;
	*set REG_WATDOG_RESET = WDT_RESET_DWORD0;
	*/
	movi  a0, 0x6F70746B
	movi  a2, 0x18D00004
	s32i  a0, a2, 0

	movi  a0, 0x55AA9BDF
	movi  a2, 0x18D00008
	s32i  a0, a2, 0



	//version D
	movi    a2, VECTOR_BASE_ADDR    /* 0x40000000 */
	wsr     a2, VECBASE             /* interrupt vector */


	movi    a0, 0
	wsr     a0, INTENABLE /* disable interrupt */

	wsr     a0, IBREAKA_0
	wsr     a0, IBREAKA_1
	wsr     a0, IBREAKENABLE
	wsr     a0, DBREAKA_0
	wsr     a0, DBREAKA_1    
	wsr     a0, DBREAKC_0
	wsr     a0, DBREAKC_1
    
	wsr     a0, ICOUNT
	wsr     a0, ICOUNTLEVEL
	isync

	rsil    a1, XCHAL_DEBUGLEVEL - 1 /* XCHAL_DEBUGLEVEL = 6 in core-isa.h */

	wsr     a0, CCOUNT
	wsr     a0, WINDOWBASE
	rsync

	movi    a1, 1
	wsr     a1, WINDOWSTART /* 1->WINDOWSTART 0->WINDOWBASE */

	ssai    0

   //Level-1 interrupt
	wsr     a0, EXCSAVE_1
	wsr     a0, EPC_1
	wsr     a0, EXCCAUSE

    //Medium priority interrupts
	wsr     a0, EPC_2
	wsr     a0, EPS_2
	wsr     a0, EXCSAVE_2

	wsr     a0, EPC_3
	wsr     a0, EPS_3
	wsr     a0, EXCSAVE_3

    //High priority interrupts
	wsr     a0, EPC_4
	wsr     a0, EPS_4
	wsr     a0, EXCSAVE_4

	wsr     a0, EPC_5       
	wsr     a0, EPS_5
	wsr     a0, EXCSAVE_5
	
	//Debug EPC, EPS and EXCSAVE
	wsr     a0, EPC_6       
	wsr     a0, EPS_6
	wsr     a0, EXCSAVE_6
	
	#ifdef NMI_EPC_ORGINAL
	//NMI EPC, EPS and EXCSAVE
	wsr     a0, EPC_7       
	wsr     a0, EPS_7
	wsr     a0, EXCSAVE_7
	#endif
	
	//3 timers
	wsr     a0, CCOMPARE_0
	wsr     a0, CCOMPARE_1
//	wsr     a0, CCOMPARE_2

	movi    a2, XCHAL_INTTYPE_MASK_EXTERN_EDGE | XCHAL_INTTYPE_MASK_SOFTWARE /* in core-isa.h */
	wsr     a2, INTCLEAR

#if XCHAL_HAVE_CP
	wsr     a0, BR /* XCHAL_HAVE CP 1 in core-isa.h */
	wsr     a0, CPENABLE
#endif

	//the following code is in programmers_guide.pdf
	movi    a2, XCHAL_DEBUGLEVEL - 1
	wsr     a2, PS
	rsync


#ifdef CACHE_ON

	// set all region as bypass
	movi    a5, 0xE0000000
	movi    a4, 2
	movi    a3, 0
	j       3f
2:  sub     a3, a3, a5     /* just like plus 0x20000000 */
3:  wdtlb   a4, a3
	witlb   a4, a3     /* a4 = 2 -> tlb, bypass */
	bne     a3, a5, 2b
	isync

	/*  HiFi 4 (optek_hifi4_eval_D)	XCHAL_ICACHE_SIZE = 8192, XCHAL_DCACHE_SIZE = 16384, CACHE LINE 64 Bytes */

	movi    a2, ICACHE_SIZE
	movi    a3, 0
L0:
	iiu     a3, 0
	iiu     a3, 64
	addi    a3, a3, 128
	bne     a2, a3, L0


	movi    a2, ICACHE_SIZE
	movi    a3, 0
L1:
	iii     a3, 0
	iii     a3, 64
	addi    a3, a3, 128
	bne     a2, a3, L1

	isync

	movi 	a2, DCACHE_SIZE
	movi 	a3, 0
L2:
	//diu 	a3, 0
	//diu 	a3, 64
	addi 	a3, a3, 128
	bne 	a2, a3, L2
	
	movi 	a2, DCACHE_SIZE
	movi 	a3, 0
L3:
	dii 	a3, 0
	dii 	a3, 64
	addi 	a3, a3, 128
	bne 	a2, a3, L3
	
	dsync	

.macro set_access_mode am
	rdtlb1 	a4, a3
	ritlb1 	a5, a3
	srli    a4, a4, 4
	slli    a4, a4, 4
	srli    a5, a5, 4
	slli    a5, a5, 4
	addi    a4, a4, \am
	addi    a5, a5, \am
	wdtlb   a4, a3
	witlb   a5, a3
.endm

	//1: cached 
	//2: bypass 
	//15: invalid 
	//4: write-back/write allocate 
	//5: write-back/no write allocate
	movi    a3, 0 /* start at region 0 */
	set_access_mode 2

	movi    a3, 0x40000000 //iram
	set_access_mode 2			

	movi    a3, 0x60000000 //flash,psram
//	set_access_mode 1 //cache on, test
	set_access_mode 4 //cache on, test
//	set_access_mode 5 //cache on, test

	//set_access_mode 2 //cache off

	movi    a3, 0xE0000000
	set_access_mode 15

#endif	//CACHE_ON


	/*
	struct rom_table {
				void *unpack_start,			
				void *unpack_end,			
				void *archive_start			
	};
	*/


#if 0
	movi    a2, _vector_text_msg_start
	beqz    a2, unpackdone

unpack:
	l32i    a3, a2, 0       // start vaddr
	l32i    a4, a2, 4       // end vaddr
	l32i    a5, a2, 8       // store vaddr
	bltu    a3, a4, unpack1
	bnez    a3, unpacknext
	bnez    a5, unpacknext
	j       unpackdone

unpack1:
	/* *(start vaddr)++ = *(store vaddr)++ */
	/* *a3++ = *a5++ */
	l32i    a6, a5, 0 //a6 = *(a5+0); 
	addi    a5, a5, 4 //at +=4;
	s32i    a6, a3, 0 //*(a3+0) = a6;
	addi    a3, a3, 4 //a3 +=4;
	bltu    a3, a4, unpack1

unpacknext:
	addi    a2, a2, 12
	j       unpack

unpackdone:
#endif


	movi    sp, __stack
	movi    a2, PS_WOE_MASK | PS_UM_MASK //in corebits.h, hifi4

	wsr     a2, PS
	rsync


	#if 1
	call4   Flash_Boot_Switch
	#endif


#if 1
	movi    a2, _vector_text_msg_start
	beqz    a2, unpackdone

unpack:
	l32i    a3, a2, 0       // start vaddr
	l32i    a4, a2, 4       // end vaddr
	l32i    a5, a2, 8       // store vaddr
	bltu    a3, a4, unpack1
	bnez    a3, unpacknext
	bnez    a5, unpacknext
	j       unpackdone

unpack1:
	/* *(start vaddr)++ = *(store vaddr)++ */
	/* *a3++ = *a5++ */
	l32i    a6, a5, 0 //a6 = *(a5+0); 
	addi    a5, a5, 4 //at +=4;
	s32i    a6, a3, 0 //*(a3+0) = a6;
	addi    a3, a3, 4 //a3 +=4;
	bltu    a3, a4, unpack1

unpacknext:
	addi    a2, a2, 12
	j       unpack

unpackdone:
#endif

#endif //UPGRADE_PROJECT_ENABLE


	#if 1
	j	ROM_DSP_CORE_APP_BOOT_JUMP //jump around literal pool

	.literal_position //tells assembler place literals here
	.align  4

	ROM_DSP_CORE_APP_BOOT_START:
	.word	ROM_DSP_CORE_APP_BOOT_START_ADDR

	.align  4

	#ifdef ROM_START_ADD_SAME
	ROM_DSP_CORE_APP_BOOT_JUMP:
	#else
	ROM_DSP_CORE_APP_BOOT_JUMP:
	#endif

	l32r    a0, ROM_DSP_CORE_APP_BOOT_START
	jx      a0
	#endif

#else //DSP_CORE_ENABLE

	.word	0xFFFFFFFF
	.word	0xFFFFFFFF

#endif //DSP_CORE_ENABLE
