/* interrupts.h -- Interrupt related definitions
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

#ifndef	__INTERRUPT_H__
#define	__INTERRUPT_H__



#ifndef _XTSTR
#define _XTSTR(x) # x
#define XTSTR(x) _XTSTR(x)
#endif


#define INT_LEVEL_MASK      0
#define INT_ENABLE_MASK     4


#if defined THREAD_RTOS
extern unsigned int intMasking[2];

static __inline__ unsigned int enable_ints(unsigned int mask)
{
    unsigned int ret;
    unsigned int new_intenable;

    __asm__ __volatile__( 
	"rsil    a15, 1                 \n\t"
	"l32i    %0,%3,"XTSTR(INT_ENABLE_MASK)" \n\t"
	"or      %1, %0, %2             \n\t"
	"s32i    %1,%3,"XTSTR(INT_ENABLE_MASK)" \n\t"
	"l32i    %2,%3,"XTSTR(INT_LEVEL_MASK)" \n\t"
	"and     %1,%1,%2               \n\t"
	"wsr     %1,"XTSTR(INTENABLE)"  \n\t"
	"wsr     a15, "XTSTR(PS)"       \n\t"
	"rsync                          \n\t"
	: "=&a" (ret), "=&a" (new_intenable)
	: "a" (mask), "a" (intMasking)
	: "a15"
	);
    return ret;
}

static __inline__ unsigned int disable_ints(unsigned int mask)
{
    unsigned int ret;
    unsigned int new_intenable;

    __asm__ __volatile__( 
	"rsil    a15, 1                 \n\t"
	"l32i    %0,%3,"XTSTR(INT_ENABLE_MASK)" \n\t"
	"and      %1, %0, %2             \n\t"
	"s32i    %1,%3,"XTSTR(INT_ENABLE_MASK)" \n\t"
	"l32i    %2,%3,"XTSTR(INT_LEVEL_MASK)" \n\t"
	"and     %1,%1,%2               \n\t"
	"wsr     %1,"XTSTR(INTENABLE)"  \n\t"
	"wsr     a15, "XTSTR(PS)"       \n\t"
	"rsync                          \n\t"
	: "=&a" (ret), "=&a" (new_intenable)
	: "a" (mask), "a" (intMasking)
	: "a15"
	);
    return ret;
}

static __inline__ unsigned int read_interrupt()
{
    unsigned int interrupt;

    __asm__ __volatile__ ( 
	"rsr     %0, "XTSTR(INTERRUPT)
	: "=a" (interrupt) 
	);
    return interrupt;
}
#endif


#define	LEVEL1_INT0_MASK    (1<<0)
#define	LEVEL1_INT1_MASK    (1<<1)
#define	LEVEL1_INT2_MASK    (1<<2)
#define	LEVEL1_INT3_MASK    (1<<3)
#define	LEVEL1_INT4_MASK    (1<<4)
#define	LEVEL1_INT5_MASK    (1<<5)
#define	LEVEL1_INT6_MASK    (1<<6)

#define	LEVEL2_INT8_MASK    (1<<8)
#define	LEVEL3_INT9_MASK    (1<<9)
#define	LEVEL3_INT10_MASK   (1<<10)

#define	LEVEL2_INT19_MASK	(1<<19)
#define	LEVEL2_INT20_MASK   (1<<20)
#define	LEVEL1_INT21_MASK   (1<<21)

#define	INT_DMA             LEVEL3_INT9_MASK
#define	INT_USB             LEVEL2_INT8_MASK
#define	INT_INTCON          LEVEL1_INT1_MASK
#define	INT_GPIO            LEVEL1_INT0_MASK

//LEVEL1_INI1
#define UART_INT_MASK           (1<<0)
#define SPI_INT_MASK            (1<<1)
#define I2S0_INT_MASK           (1<<2)
#define I2S1_INT_MASK           (1<<3)
#define I2C0_INT_MASK           (1<<4)
#define LCD_INT_MASK            (1<<5)
#define SD_INT_MASK             (1<<6)
#define RTC_INT_MASK            (1<<7)
#define UART1_INT_MASK          (1<<8)
#define UART2_INT_MASK          (1<<10)
#define IR_INT_MASK             (1<<11)
#define CEC_INT_MASK            (1<<12)
#define ADC_INT_MASK            (1<<13)

#define I2C1_INT_MASK           (1<<30)
#define SPI_TEST_INT_MASK       (1<<31)


#define SOFT0_INT_MASK          (1<<14)
#define SOFT1_INT_MASK          (1<<15)
#define SOFT2_INT_MASK          (1<<16)
#define SOFT3_INT_MASK          (1<<17)
#define SOFT4_INT_MASK          (1<<18)
#define SOFT5_INT_MASK          (1<<19)
#define SOFT6_INT_MASK          (1<<20)
#define SOFT7_INT_MASK          (1<<21)
#define SOFT8_INT_MASK          (1<<22)

#define REG_INT_ENABLE(x)       (REG_INTCON_INT_ENA |= x )

#define SOFT_INT_ENABLE(x)      (REG_INTCON_INT_ENA |= SOFT##x##_INT_MASK )
#define SOFT_INT_DISABLE(x)     (REG_INTCON_INT_ENA &= ~SOFT##x##_INT_MASK)
#define SOFT_INT_SET(x)         (REG_INTCON_INT_TEST |= SOFT##x##_INT_MASK)
#define SOFT_INT_CLR(x)         (REG_INTCON_INT_TEST &= ~SOFT##x##_INT_MASK)

#define SOFT0_INT_ENABLE        (REG_INTCON_INT_ENA |= SOFT0_INT_MASK )
#define SOFT0_INT_DISABLE       (REG_INTCON_INT_ENA &= ~SOFT0_INT_MASK)

#define SOFT0_INT_SET           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST |= SOFT0_INT_MASK; \
                                   TX_RESTORE; \
                                }

#define SOFT0_INT_CLR           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST &= ~SOFT0_INT_MASK;	\
                                   TX_RESTORE; \
                                }


#define SOFT1_INT_ENABLE        (REG_INTCON_INT_ENA |= SOFT1_INT_MASK )
#define SOFT1_INT_DISABLE       (REG_INTCON_INT_ENA &= ~SOFT1_INT_MASK)

#define SOFT1_INT_SET           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST |= SOFT1_INT_MASK; \
                                   TX_RESTORE; \
                                }

#define SOFT1_INT_CLR           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST &= ~SOFT1_INT_MASK;	\
                                   TX_RESTORE; \
                                }


#define SOFT2_INT_ENABLE        (REG_INTCON_INT_ENA |= SOFT2_INT_MASK )
#define SOFT2_INT_DISABLE       (REG_INTCON_INT_ENA &= ~SOFT2_INT_MASK)

#define SOFT2_INT_SET           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST |= SOFT2_INT_MASK; \
                                   TX_RESTORE; \
                                }

#define SOFT2_INT_CLR           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST &= ~SOFT2_INT_MASK;	\
                                   TX_RESTORE; \
                                }


#define SOFT3_INT_ENABLE		(REG_INTCON_INT_ENA |= SOFT3_INT_MASK)
#define SOFT3_INT_DISABLE		(REG_INTCON_INT_ENA &= ~SOFT3_INT_MASK)	


#define SOFT3_INT_SET           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST |= SOFT3_INT_MASK; \
                                   TX_RESTORE; \
                                }

#define SOFT3_INT_CLR           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST &= ~SOFT3_INT_MASK;	\
                                   TX_RESTORE; \
                                }



#define SOFT4_INT_ENABLE        (REG_INTCON_INT_ENA |= SOFT4_INT_MASK)
#define SOFT4_INT_DISABLE       (REG_INTCON_INT_ENA &= ~SOFT4_INT_MASK)	

#define SOFT4_INT_SET           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST |= SOFT4_INT_MASK; \
                                   TX_RESTORE; \
                                }

#define SOFT4_INT_CLR           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST &= ~SOFT4_INT_MASK;	\
                                   TX_RESTORE; \
                                }


#define SOFT5_INT_SET           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST |= SOFT5_INT_MASK; \
                                   TX_RESTORE; \
                                }

#define SOFT5_INT_CLR           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST &= ~SOFT5_INT_MASK;	\
                                   TX_RESTORE; \
                                }

#define SOFT5_INT_ENABLE		(REG_INTCON_INT_ENA |= SOFT5_INT_MASK)
#define SOFT5_INT_DISABLE		(REG_INTCON_INT_ENA &= ~SOFT5_INT_MASK)


#define SOFT6_INT_ENABLE        (REG_INTCON_INT_ENA |= SOFT6_INT_MASK )
#define SOFT6_INT_DISABLE       (REG_INTCON_INT_ENA &= ~SOFT6_INT_MASK)

#define SOFT6_INT_SET           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST |= SOFT6_INT_MASK; \
                                   TX_RESTORE; \
                                }

#define SOFT6_INT_CLR           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST &= ~SOFT6_INT_MASK;	\
                                   TX_RESTORE; \
                                }


#define SOFT7_INT_ENABLE        (REG_INTCON_INT_ENA |= SOFT7_INT_MASK )
#define SOFT7_INT_DISABLE       (REG_INTCON_INT_ENA &= ~SOFT7_INT_MASK)

#define SOFT7_INT_SET           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST |= SOFT7_INT_MASK; \
                                   TX_RESTORE; \
                                }

#define SOFT7_INT_CLR           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST &= ~SOFT7_INT_MASK;	\
                                   TX_RESTORE; \
                                }


#define SOFT8_INT_ENABLE        (REG_INTCON_INT_ENA |= SOFT8_INT_MASK )
#define SOFT8_INT_DISABLE       (REG_INTCON_INT_ENA &= ~SOFT8_INT_MASK)

#define SOFT8_INT_SET           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST |= SOFT8_INT_MASK; \
                                   TX_RESTORE; \
                                }

#define SOFT8_INT_CLR           { \
                                   TX_INTERRUPT_SAVE_AREA; \
                                   TX_DISABLE; \
                                   REG_INTCON_INT_TEST &= ~SOFT8_INT_MASK;	\
                                   TX_RESTORE; \
                                }


#define	SD_DMA_SOFT_INT_MASK        SOFT4_INT_MASK

#define	SD_DMA_SOFT_INT_ENABLE      SOFT_INT_ENABLE(4)
#define	SD_DMA_SOFT_INT_DISABLE     SOFT_INT_DISABLE(4)
#define	SD_DMA_SOFT_INT_SET         SOFT_INT_SET(4)
#define	SD_DMA_SOFT_INT_CLR         SOFT_INT_CLR(4)



void L1_int0_extlevel(void *param);
void L1_int1_extlevel(void *param);
void L1_int6_timer0(void *para);
void L1_int2_extlevel(void *param);
void L1_int3_extlevel(void *param);
void L2_int8_extlevel(void *param);
void L3_int9_extlevel(void *param);
void L3_int10_timer1(void *param);

void L2_int20_extlevel(void *param);
void L1_int21_extlevel(void *param);
void L2_int19_extlevel(void *param);

void xtensa_timer0_updata(void);
void xtensa_timer1_updata(void);

void sd_isr( void );

#ifdef SD_ENABLE
void sd_dma7TxCompleteCallback( void );
void sd_dma8RxCompleteCallback( void );
#endif

#endif //__INTERRUPT_H__
