/* reset.S -- Reset Vector
 * 
 * This code is taken from example code in the Xtensa Microprocessor
 * Programmer's Guide.
 *
 * WARNING:  It is highly unlikely that this code works as is on your
 *	particular Xtensa processor configuration.  It is hardcoded
 *	for the specific processor configuration used for the
 *	examples in the Xtensa Microprocessor Programmer's Guide.
 *	(The example code did not use the Xtensa HAL to address this
 *	portability issue, for clarity's sake.)  Getting it to work
 *	on another processor configuration requires some modifications.
 */

/*
 * Copyright (c) 2003-2005 by Tensilica Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of Tensilica Inc.
 * They may not be modified, copied, reproduced, distributed, or disclosed to
 * third parties in any manner, medium, or form, in whole or in part, without
 * the prior written consent of Tensilica Inc.
 */

#include <xtensa/coreasm.h>
#include <xtensa/simcall.h>

#include "os_type.h"
#include "oem.h"


#ifdef DEBUG_GPIO_ENABLE
#undef DEBUG_GPIO_ENABLE
#endif

#define CACHE_ON

#define PREFECH_ON

#define PREFECT_MODE    0x00000011

	.global _ResetVector


#define JUMP_ADDR1    0x412355aa
#define JUMP_ADDR2    0x40000000


#define ICACHE_SIZE 16384
#define DCACHE_SIZE 32768


//#define JUMP_TEST


#if (defined __XTENSA_CALL0_ABI__)
# define CALL	call0
# define CALLX	callx0
# define ARG1	a2	/* 1st outgoing call argument */
# define ARG2	a3	/* 2nd outgoing call argument */
# define ARG3	a4	/* 3rd outgoing call argument */
# define ARG4	a5	/* 4th outgoing call argument */
# define ARG5	a6	/* 5th outgoing call argument */
#elif XCHAL_HAVE_XEA3
# define CALL	call8
# define CALLX	callx8
# define ARG1	a10	/* 1st outgoing call argument */
# define ARG2	a11	/* 2nd outgoing call argument */
# define ARG3	a12	/* 3rd outgoing call argument */
# define ARG4	a13	/* 4th outgoing call argument */
# define ARG5	a14	/* 5th outgoing call argument */
#else
# define CALL	call4
# define CALLX	callx4
# define ARG1	a6	/* 1st outgoing call argument */
# define ARG2	a7	/* 2nd outgoing call argument */
# define ARG3	a8	/* 3rd outgoing call argument */
# define ARG4	a9	/* 4th outgoing call argument */
# define ARG5	a10	/* 5th outgoing call argument */
#endif


	.section    .ResetVector.text, "ax"
	.align  4

_ResetVector:
	j    Reset //jump around literal pool

	.literal_position //tells assembler place literals here
	.align  4

#ifdef JUMP_TEST
jumpAddr1:
	.word  JUMP_ADDR2
	.align  4
#endif

Reset:


/*
notes:Vatiable mode faster than fixed mode. Latenncy affects read/write speed.
*/

//LANTENCY
#define LANTENCY_3    0x0E //CLK <= 85MHZ
#define LANTENCY_4    0x0F //CLK <= 104MHZ
#define LANTENCY_5    0x00 //CLK <= 133MHZ
#define LANTENCY_6    0x01 //CLK <= 166MHZ
#define LANTENCY_7    0x02 //CLK <= 200MHZ
#define LANTENCY_8    0x03 //CLK <= ?MHZ
#define LANTENCY_10   0x05 //CLK <= 250MHZ

#define FIX_EANBLE    0 //0:Variable, 1:Fixed

/*
*LANTENCY_3
*LANTENCY_6
*/
#define LANTENCY_CFG  LANTENCY_6


#define PSRAM_CFG                     ( (0x008F<<16) | (LANTENCY_CFG<<4) | (FIX_EANBLE<<3) | 0x07)
#define PSRAM_CONFIG_REG_BASE_ADDR    0x19000000
#define PSRAM_CONFIG0_32b_ADDR        (PSRAM_CONFIG_REG_BASE_ADDR + 0x1000) //CA24, Addr 12 (16b)

	movi  a2, PSRAM_CFG
	movi  a3, PSRAM_CONFIG0_32b_ADDR
	s32i  a2, a3, 0	


	#ifdef JUMP_TEST
	l32r a0, jumpAddr1
	jx	 a0
	nop.N
	#endif


	/*
	*disable clk_err
	*/
	movi  a2, 0x1A000024    //regCtl_3 addr
	movi  a3, 0x000006c5						
	s32i  a3, a2,0

	movi  a2, 0x1A0000FC    //AON_WRITE_TRANS addr
	movi  a3, 0x8E89BE09    //trans id + addr
	s32i  a3, a2, 0


	#if 1
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

	movi a3, 0x60000000 //flash,psram
	set_access_mode 1      //cache on, test
	//set_access_mode 4      //cache on, test
	//set_access_mode 5      //cache on, test

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

	movi    sp, __stack
	movi    a2, PS_WOE_MASK | PS_UM_MASK //in corebits.h, hifi4

	wsr     a2, PS
	rsync

	//call4 hw_wdt_disable

	#ifdef DEBUG_GPIO_ENABLE
	call4   debug_pin_init
	#endif


	call4   SpiFlash_Enable
	call4   SpiFlash_4Bit_Enable
	caLL4   Flash_Boot_Copy
	call4   _init_reent_bss

	//initialize bss section
	movi    a8, _bss_start
	movi    a10, _bss_end //should aligned with 4 byte
	sub     a11, a10, a8
	srli    a11, a11, 2

	movi    a9, 0
	#if XCHAL_HAVE_LOOPS
	loopnez a11, zerodone
	#else
	beqz a11, zerodone
	branch_zero:
	#endif

	s32i    a9, a8, 0
	addi    a8, a8, 4

	#if !XCHAL_HAVE_LOOPS
	addi    a11, a11, -1
	bnez    a11, branch_zero
	#endif

	zerodone:


callmain:
	movi    a0, 0
	movi    a6, 0   // clear argc
	movi    a7, 0   // clear argv
	movi    a8, 0   // clear envp
	movi    a4, main
	callx4  a4

reset_exit:
	//movi    a2, SYS_exit
	//simcall
	break 1,1
