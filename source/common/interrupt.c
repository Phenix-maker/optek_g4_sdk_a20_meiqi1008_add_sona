////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 Optek.
//
//! \file interrupt.c
//! \brief
//! \version 0.1
//! \date Mar, 2005
//!
//! \see ThreadX User Guide
////////////////////////////////////////////////////////////////////////////////

#include "os_config.h"

#include "sdk_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_cec.h"
#include "hw_timer.h"
#include "hw_spi.h"
#include "hal_dma.h"
#include "hal_uart.h"
#include "hal_sflash.h"

#include "interrupt.h"
#include "remote.h"
#include "otg.h"

#ifdef HDMI_ENABLE
#include "hdmi_process.h"
#endif


void L1_int0_extlevel(void *param) __INTERNAL_RAM_TEXT;
void L1_int1_extlevel(void *param) __INTERNAL_RAM_TEXT;
void L1_int3_extlevel(void *param) __INTERNAL_RAM_TEXT;
void L1_int6_timer0(void *param) __INTERNAL_RAM_TEXT;
void L1_int21_extlevel(void *param) __INTERNAL_RAM_TEXT;
void L2_int8_extlevel(void *param) __INTERNAL_RAM_TEXT;
void L3_int9_extlevel(void *param) __INTERNAL_RAM_TEXT;
void L3_int10_timer1(void *param) __INTERNAL_RAM_TEXT;
void L2_int19_extlevel(void *param) __INTERNAL_RAM_TEXT;

#ifdef SD_ENABLE
#ifdef SD_DMA_TRANSFER
U8 sdDma7 = 0;
#endif
#endif


//the level of xchal int0 is 1

void L1_int0_extlevel(void *param)
{
	volatile U32 temp;

	temp = REG_GPIO0_INTST;
	REG_GPIO0_INTST = temp;		//clear int


#ifdef HDMI_ENABLE
	if (temp & HDMI_INT_PIN_MASK)
	{
		#ifdef SAVE_MODULE_FROM_FLASH
		sflash_in_programming_check();
		#endif
		
		//clear int
		REG_GPIO0_INTST |= HDMI_INT_PIN_MASK;
		hdmi_hw_isr();
	}
#endif
}


void L1_int1_extlevel(void *param)
{
	U32 val;
	INT_CONTROLs *int_reg = (INT_CONTROLs *)&val;
	
	#ifdef SAVE_MODULE_FROM_FLASH
	sflash_in_programming_check();
	#endif


	val = REG_INTCON_INT_STA;
	val &= REG_INTCON_INT_ENA;

	#ifdef USB_INT_CHANGEs_TO_LEVEL1
	if (val & SOFT5_INT_MASK)
	{			
		SOFT5_INT_CLR;
		usbotg_isr();
	}
	#endif
	
	#ifdef SOFT_INT_6_USED
	if (val & SOFT6_INT_MASK)
	{
		SoftInt6Isr();
	}
	#endif

	#ifdef SOFT_INT_7_USED
	if (val & SOFT7_INT_MASK)
	{
		SoftInt7Isr();
	}
	#endif

	#ifdef BTDM52_ENABLE
	if (val & SOFT8_INT_MASK)
	{
		SOFT8_INT_CLR;
		btdm_controller_sch();
	}
	#endif

	if (int_reg->uart0)
	{
		Uart0_IsrHandler();
	}

	if (int_reg->uart1)
	{
		Uart1_IsrHandler();
	}

	if (int_reg->uart2)
	{
		Uart2_IsrHandler();
	}

	#ifdef ARC_ENABLE
	if (int_reg->cec)
	{
		hw_cec_rx_fromIsr();
	}
	#endif

	#ifdef SPI_ENABLE
	#if ( (defined SPI_TX_INT_ENABLE) || (defined SPI_RX_INT_ENABLE) )
	if (int_reg->spi)
	{
		spi_int_handler_fromIsr();
	}
	#endif
	#endif
}

#ifdef BTDM52_ENABLE
void L2_int19_extlevel(void *param)
{
	#ifdef SAVE_MODULE_FROM_FLASH
	sflash_in_programming_check();
	#endif

	rwip_isr();
}
#endif

#ifdef INT_ORIGINAL
void L1_int2_extlevel(void *param)
{
}
#endif

void L1_int3_extlevel(void *param)
{ 
#ifdef REMOTE_ENABLE
	U32 value;
	
	#ifdef SAVE_MODULE_FROM_FLASH
	sflash_in_programming_check();
	#endif

	value = REG_IR_RXFIFO;
	remote_ir_recive(value);
#endif
}

#ifdef INT_ORIGINAL
void L1_int4_extlevel(void)
{
}

void L1_int5_extlevel(void)
{
}
#endif

//timer 0:the level of xchal int6 is 1
void L1_int6_timer0(void *param)
{
#ifdef TIMER0_ENABLE

#ifndef RTOS_ENABLE
	static unsigned char state = TRUE;

	if (state)
	{
		state = FALSE;
	}

	xtensa_timer0_updata();
#endif

#endif
}

#if 0
void L1_int7_software(void)
{
}
#endif

void L2_int8_extlevel(void *param)
{
#if	( (defined USB_HOST_ENABLE) || (defined USB_DEV_ENABLE) )
	#ifdef SAVE_MODULE_FROM_FLASH	
	sflash_in_programming_check();
	#endif
	
	usbotg_isr();
#endif
}

#ifdef L3_INT9_EXTLEVEL_ORGINAL
void L3_int9_extlevel(void *param)
{
	hal_dma_isr(param);
}
#endif

void L1_int21_extlevel(void *param)
{
	u16 val_16b;
	TIMER_INT_CLRs *pTime_int_clr = &val_16b;

	#ifdef SAVE_MODULE_FROM_FLASH
	sflash_in_programming_check();
	#endif

	val_16b = ADDR_TIMER_INT_CLR;
	pTime_int_clr->timer_2_int_clr = 1;
	ADDR_TIMER_INT_CLR = val_16b;

#ifdef RTOS_ENABLE
	BaseType_t xPortSysTickHandler( void );
	xPortSysTickHandler();
#endif

	REG_GPIO1_IE = 0;//this is a patch
}

//time 1:the level of xchal int10 is 3
void L3_int10_timer1(void *param)
{
#ifdef TIMER1_ENABLE
	TX_INTERRUPT_SAVE_AREA;

	#ifdef SAVE_MODULE_FROM_FLASH
	sflash_in_programming_check();
	#endif

	//test timing
	static U8 state = TRUE;
	
	if (state)
	{
		state = FALSE;
	}
	else
	{
		state = TRUE;
	}

	xtensa_timer1_updata();
#endif //TIMER1_ENABLE
}

#if INT_ORIGINAL
void L3_int11_software(void)
{
}

void L4_int12_extlevel(void)
{
}

//time 2:the level of xchal int13 is 5
void L5_int13_timer2(void)
{
}

void L6_int14_NMI(void)
{
}
#endif
