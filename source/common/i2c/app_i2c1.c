/*
** Copyright (C) 2001 Optek. All rights reserved.
**
** The information and source code contained herein is the exclusive
** property of Optek and may not be disclosed, examined or
** reproduced in whole or in part without explicit written authorization
** from the company.
*/

/*
** Author:
**
** Date: 1/15/2001
**
** Description: I2C Interface.
** 
*/

#include "os_config.h"


#ifdef I2C1_ENABLE
#include "regmap.h"

#include "hw_timer.h"
#include "hw_gpio.h"
#include "hw_uart.h"
#include "hw_pll.h"

#include "hal_gpio.h"

#include "app_i2c.h"
#include "app_i2c1.h"
#include "app_timer.h"

/*
*I2C_CLK_50KHZ
*I2C_CLK_100KHZ
*I2C_CLK_150KHZ
*I2C_CLK_200KHZ
*I2C_CLK_400KHZ
*/
#define I2C_CLK_100KHZ


#undef RTOS
#define RTOS


#ifdef RTOS
static void *mutex_i2c1;
#endif

static U8 word_sub_address;

#ifdef RTOS
#define I2C_MUTEX_LOCK       xSemaphoreTake( mutex_i2c1, portMAX_DELAY)
#define I2C0_MUTEX_UNLOCK    xSemaphoreGive( mutex_i2c1 )

#define OS_MUTEX_I2C_LOCK    I2C_MUTEX_LOCK
#define OS_MUTEX_I2C_UNLOCK  I2C0_MUTEX_UNLOCK
#else
#define I2C_MUTEX_LOCK
#define I2C0_MUTEX_UNLOCK

#define OS_MUTEX_I2C_LOCK    I2C_MUTEX_LOCK
#define OS_MUTEX_I2C_UNLOCK  I2C0_MUTEX_UNLOCK
#endif


#define I2C_Retries         0

#define I2C_CLK_DELAY(x)   \
{\
	int i2c_loop_cnt;\
	for(i2c_loop_cnt=0; i2c_loop_cnt<x; i2c_loop_cnt++)\
	{\
	    asm("nop");\
	}\
}


#if (SYS_CLK_FREQ_DEFAULT == CPU_FREQ_48MHz)

#if (defined I2C_CLK_400KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(1)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(1)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(1)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(1)
#define I2C_W_DATA_STEADY_DELAY           I2C_CLK_DELAY(1)
#define I2C_W_CLK_HIGH_STEADY_DELAY       I2C_CLK_DELAY(10)
#define I2C_R_CLK_LOW_STEADY_DELAY        I2C_CLK_DELAY(15)
#define I2C_R_CLK_HIGH_STEADY_DELAY       I2C_CLK_DELAY(8)

#elif (defined I2C_CLK_200KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(1)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(1)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(1)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(1)
#define I2C_W_DATA_STEADY_DELAY           delayus(1)
#define I2C_W_CLK_HIGH_STEADY_DELAY       delayus(1)
#define I2C_W_CLK_LOW_STEADY_DELAY        delayus(1)
#define I2C_R_CLK_LOW_STEADY_DELAY        delayus(1)
#define I2C_R_CLK_HIGH_STEADY_DELAY       delayus(1)

#elif (defined I2C_CLK_150KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(2)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(2)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(2)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(2)
#define I2C_W_DATA_STEADY_DELAY           delayus(2)
#define I2C_W_CLK_HIGH_STEADY_DELAY       delayus(2)
#define I2C_W_CLK_LOW_STEADY_DELAY        delayus(2)
#define I2C_R_CLK_LOW_STEADY_DELAY        delayus(2)
#define I2C_R_CLK_HIGH_STEADY_DELAY       delayus(2)

#elif (defined I2C_CLK_100KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(2)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(2)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(2)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(2)
#define I2C_W_DATA_STEADY_DELAY           delayus(4)
#define I2C_W_CLK_HIGH_STEADY_DELAY       delayus(4)
#define I2C_W_CLK_LOW_STEADY_DELAY        delayus(4)
#define I2C_R_CLK_LOW_STEADY_DELAY        delayus(4)
#define I2C_R_CLK_HIGH_STEADY_DELAY       delayus(4)

#elif (defined I2C_CLK_50KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(4)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(4)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(4)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(4)
#define I2C_W_DATA_STEADY_DELAY           delayus(8)
#define I2C_W_CLK_HIGH_STEADY_DELAY       delayus(8)
#define I2C_W_CLK_LOW_STEADY_DELAY        delayus(8)
#define I2C_R_CLK_LOW_STEADY_DELAY        delayus(8)
#define I2C_R_CLK_HIGH_STEADY_DELAY       delayus(8)
#endif

#define I2C_DUMMY_CLK_HIGH_LEVEL_DELAY    delayus(1)
#define I2C_DUMMY_CLK_LOW_LEVEL_DELAY     delayus(1)

#else
//(SYS_CLK_FREQ_DEFAULT == CPU_96MHz)

#if (defined I2C_CLK_500KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(1)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(1)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(1)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(1)
#define I2C_W_DATA_STEADY_DELAY           I2C_CLK_DELAY(50)
#define I2C_W_CLK_HIGH_STEADY_DELAY       I2C_CLK_DELAY(100)
#define I2C_W_CLK_LOW_STEADY_DELAY        I2C_CLK_DELAY(100)
#define I2C_R_CLK_LOW_STEADY_DELAY        I2C_CLK_DELAY(60)
#define I2C_R_CLK_HIGH_STEADY_DELAY       I2C_CLK_DELAY(75)

#elif (defined I2C_CLK_400KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(1)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(1)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(1)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(1)
#define I2C_W_DATA_STEADY_DELAY           I2C_CLK_DELAY(75)
#define I2C_W_CLK_HIGH_STEADY_DELAY       I2C_CLK_DELAY(100)
#define I2C_W_CLK_LOW_STEADY_DELAY        I2C_CLK_DELAY(100)
#define I2C_R_CLK_LOW_STEADY_DELAY        I2C_CLK_DELAY(90)
#define I2C_R_CLK_HIGH_STEADY_DELAY       I2C_CLK_DELAY(80)

#elif (defined I2C_CLK_300KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(1)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(1)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(1)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(1)
#define I2C_W_DATA_STEADY_DELAY           delayus(1)
#define I2C_W_CLK_HIGH_STEADY_DELAY       delayus(1)
#define I2C_W_CLK_LOW_STEADY_DELAY        delayus(1)
#define I2C_R_CLK_LOW_STEADY_DELAY        delayus(1)
#define I2C_R_CLK_HIGH_STEADY_DELAY       delayus(1)

#elif (defined I2C_CLK_200KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(2)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(2)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(2)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(2)
#define I2C_W_DATA_STEADY_DELAY           delayus(2)
#define I2C_W_CLK_HIGH_STEADY_DELAY       delayus(2)
#define I2C_W_CLK_LOW_STEADY_DELAY        delayus(2)
#define I2C_R_CLK_LOW_STEADY_DELAY        delayus(2)
#define I2C_R_CLK_HIGH_STEADY_DELAY       delayus(2)

#elif (defined I2C_CLK_100KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(2)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(2)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(2)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(2)
#define I2C_W_DATA_STEADY_DELAY           delayus(4)
#define I2C_W_CLK_HIGH_STEADY_DELAY       delayus(4)
#define I2C_W_CLK_LOW_STEADY_DELAY        delayus(4)
#define I2C_R_CLK_LOW_STEADY_DELAY        delayus(4)
#define I2C_R_CLK_HIGH_STEADY_DELAY       delayus(4)

#elif (defined I2C_CLK_50KHZ)
#define I2C_START_CLK_HIGH_STEADY_DELAY   delayus(4)
#define I2C_START_DATA_LOW_STEADY_DELAY   delayus(4)
#define I2C_STOP_CLK_HIGH_STEADY_DELAY    delayus(4)
#define I2C_STOP_DATA_HIGH_STEADY_DELAY   delayus(4)
#define I2C_W_DATA_STEADY_DELAY           delayus(8)
#define I2C_W_CLK_HIGH_STEADY_DELAY       delayus(8)
#define I2C_W_CLK_LOW_STEADY_DELAY        delayus(8)
#define I2C_R_CLK_LOW_STEADY_DELAY        delayus(8)
#define I2C_R_CLK_HIGH_STEADY_DELAY       delayus(8)
#endif


#define I2C_DUMMY_CLK_HIGH_LEVEL_DELAY    delayus(1)
#define I2C_DUMMY_CLK_LOW_LEVEL_DELAY     delayus(1)

#endif



#define CLK_USED_GPIO_GROUP_0
//#define CLK_USED_GPIO_GROUP_1
//#define CLK_USED_GPIO_GROUP_2


#define DATA_USED_GPIO_GROUP_0
//#define DATA_USED_GPIO_GROUP_1
//#define DATA_USED_GPIO_GROUP_2


/*	check pins assignment before using i2c1 */
#define I2C_SCLK_PIN     I2C1_SCLK_PIN
#define I2C_SDATA_PIN    I2C1_SDATA_PIN

#define I2C_CLK_STRETCHED_BY_SLAVE
#define I2C_CLK_STRETCH_TIME_OUT_CNT   0xFFFF


#ifdef I2C_CLK_STRETCHED_BY_SLAVE 

#define I2C_CLK_STICK_WAIT    \
{\
	int clk_cnt = 0;\
	while ((I2C_CLK_PIN == 0))\
	{\
		if (I2C_CLK_PIN)\
		{\
			break;\
		}\
		clk_cnt++;\
		if (clk_cnt>I2C_CLK_STRETCH_TIME_OUT_CNT)\
		{\
			DBG_Printf("clk stick err");\
		}\
	}\
}

#define I2C_CLK_STICK_WAIT_WITH_RETURN    \
{\
	int clk_cnt = 0;\
	while ((I2C_CLK_PIN == 0))\
	{\
		if (I2C_CLK_PIN)\
		{\
			break;\
		}\
		clk_cnt++;\
		if (clk_cnt>I2C_CLK_STRETCH_TIME_OUT_CNT)\
		{\
			DBG_Printf("clk stick err");\
			return I2C_ERROR;\
		}\
	}\
}

#else
#define I2C_CLK_STICK_WAIT
#endif //I2C_CLK_STRETCHED_BY_SLAVE


#if defined CLK_USED_GPIO_GROUP_0
#define I2C_CLK_HIGH		REG_GPIO0_DOE &= ~(1<<(I2C_SCLK_PIN-GPIO0_00_INDEX))
#define I2C_CLK_LOW			REG_GPIO0_DOE |=  (1<<(I2C_SCLK_PIN-GPIO0_00_INDEX))

#define I2C_CLK_PIN			(REG_GPIO0_DIN & (1<<(I2C_SCLK_PIN-GPIO0_00_INDEX)))
#elif defined CLK_USED_GPIO_GROUP_1
#define I2C_CLK_HIGH		REG_GPIO1_DOE &= ~(1<<(I2C_SCLK_PIN-GPIO1_00_INDEX))
#define I2C_CLK_LOW			REG_GPIO1_DOE |=  (1<<(I2C_SCLK_PIN-GPIO1_00_INDEX))

#define I2C_CLK_PIN			(REG_GPIO1_DIN & (1<<(I2C_SCLK_PIN-GPIO1_00_INDEX)))
#elif defined CLK_USED_GPIO_GROUP_2
#define I2C_CLK_HIGH		REG_GPIO2_DOE &= ~(1<<(I2C_SCLK_PIN-GPIO2_00_INDEX))
#define I2C_CLK_LOW			REG_GPIO2_DOE |=  (1<<(I2C_SCLK_PIN-GPIO2_00_INDEX))

#define I2C_CLK_PIN			(REG_GPIO2_DIN & (1<<(I2C_SCLK_PIN-GPIO2_00_INDEX)))
#else
#error Must defined which groupe clock pin belonged?
#endif


#if defined DATA_USED_GPIO_GROUP_0
#define I2C_DATA_HIGH       REG_GPIO0_DOE &= ~(1<<(I2C_SDATA_PIN-GPIO0_00_INDEX))
#define I2C_DATA_LOW        REG_GPIO0_DOE |=  (1<<(I2C_SDATA_PIN-GPIO0_00_INDEX))

#define I2C_DATA_PIN		(REG_GPIO0_DIN & (1<<(I2C_SDATA_PIN-GPIO0_00_INDEX)))

#elif defined DATA_USED_GPIO_GROUP_1
#define I2C_DATA_HIGH       REG_GPIO1_DOE &= ~(1<<(I2C_SDATA_PIN-GPIO1_00_INDEX))
#define I2C_DATA_LOW        REG_GPIO1_DOE |=  (1<<(I2C_SDATA_PIN-GPIO1_00_INDEX))

#define I2C_DATA_PIN		(REG_GPIO1_DIN & (1<<(I2C_SDATA_PIN-GPIO1_00_INDEX)))
#elif defined DATA_USED_GPIO_GROUP_2
#define I2C_DATA_HIGH       REG_GPIO2_DOE &= ~(1<<(I2C_SDATA_PIN-GPIO2_00_INDEX))
#define I2C_DATA_LOW        REG_GPIO2_DOE |=  (1<<(I2C_SDATA_PIN-GPIO2_00_INDEX))

#define I2C_DATA_PIN		(REG_GPIO2_DIN & (1<<(I2C_SDATA_PIN-GPIO2_00_INDEX)))
#else
#error Must defined which groupe data pin belonged?
#endif



void app_i2c1_init (void)
{
	word_sub_address = FALSE;

	#ifdef RTOS
	mutex_i2c1 = xSemaphoreCreateMutex();
	#endif

	//i2c clk,it is set to open drain pin
	hal_gpio_mux_func_select(I2C_SCLK_PIN, MUX_SEL_GPIO_INPUT);
	hal_gpio_output_level_set(I2C_SCLK_PIN, GPIO_LOW);

	//i2c data,it is set to open drain pin
	hal_gpio_mux_func_select(I2C_SDATA_PIN, MUX_SEL_GPIO_INPUT);
	hal_gpio_output_level_set(I2C_SDATA_PIN, GPIO_LOW);
}

/*entry: assumed that clk is low*/
static void I2C_STOP (void)
{
	I2C_DATA_LOW;

	I2C_CLK_HIGH;
	I2C_STOP_CLK_HIGH_STEADY_DELAY;
	I2C_CLK_STICK_WAIT;

	I2C_DATA_HIGH;
	I2C_STOP_DATA_HIGH_STEADY_DELAY;
}

/*entry: assumed that clk and data is high*/
static I2C_RETURN I2C_START (void)
{
	U8 i;

	I2C_DATA_HIGH;

	I2C_CLK_HIGH;
	I2C_START_CLK_HIGH_STEADY_DELAY;
	I2C_CLK_STICK_WAIT;

	if (I2C_DATA_PIN)
	{
		I2C_DATA_LOW;
		I2C_START_DATA_LOW_STEADY_DELAY;
		I2C_CLK_LOW;

		return I2C_SUCCESS;
	}
	else
	{
		//DATA is locked, so unlock it
		I2C_CLK_LOW;
		I2C_DUMMY_CLK_LOW_LEVEL_DELAY;

		for (i=0; i<9; i++)
		{
			I2C_CLK_HIGH;
			I2C_DUMMY_CLK_HIGH_LEVEL_DELAY;
			I2C_CLK_STICK_WAIT;
			
			I2C_CLK_LOW;
			I2C_DUMMY_CLK_LOW_LEVEL_DELAY;
		}
		
		I2C_STOP ();

        DBG_Printf("I2C1 START err\n\r");

		return I2C_ERROR;
	}
}

/*entry: assumed that clk is low*/
static I2C_RETURN I2C_write_byte (U8 b)
{
	U8 i;
	U8 bit_mask = 0x80;

	for (i=0; i<8; i++)
	{
		I2C_CLK_LOW;

		if (b & bit_mask)
		{
			I2C_DATA_HIGH;
		}
		else
		{
			I2C_DATA_LOW;
		}
		I2C_W_DATA_STEADY_DELAY;
		
		I2C_CLK_HIGH;
		I2C_W_CLK_HIGH_STEADY_DELAY;
		I2C_CLK_STICK_WAIT;
		
		bit_mask >>= 1;
	}

	I2C_CLK_LOW;
	I2C_DATA_HIGH;
	I2C_W_DATA_STEADY_DELAY;

	I2C_CLK_HIGH;
	I2C_W_CLK_HIGH_STEADY_DELAY;
	I2C_CLK_STICK_WAIT;

	if (I2C_DATA_PIN)
	{
		I2C_CLK_LOW;
		I2C_STOP ();
		
		return I2C_ERROR;
	}
	else
	{
		I2C_CLK_LOW;
		return I2C_SUCCESS;
	}
}

/*entry: assumed that clk is low*/
static I2C_RETURN I2C_write_word (U16 word)
{
	U8 i;
	U8 bit_mask = 0x80;

	for (i=0; i<16; i++)
	{
		I2C_CLK_LOW;

		if (word & bit_mask)
		{
			I2C_DATA_HIGH;
		}
		else
		{
			I2C_DATA_LOW;
		}
		I2C_W_DATA_STEADY_DELAY;
		
		I2C_CLK_HIGH;
		I2C_W_CLK_HIGH_STEADY_DELAY;
		I2C_CLK_STICK_WAIT;
		
		bit_mask >>= 1;
	}

	I2C_CLK_LOW;
	I2C_DATA_HIGH;
	I2C_W_DATA_STEADY_DELAY;

	I2C_CLK_HIGH;
	I2C_W_CLK_HIGH_STEADY_DELAY;
	I2C_CLK_STICK_WAIT;

	if (I2C_DATA_PIN)
	{
		I2C_CLK_LOW;
		I2C_STOP ();
		
		return I2C_ERROR;
	}
	else
	{
		I2C_CLK_LOW;
		return I2C_SUCCESS;
	}
}

/*entry: assumed that clk is low*/
static U8 I2C_read_byte (BOOL con)
{
	U8 i;
	U8 b=0;
	U8 bit_mask = 0x80;

	I2C_DATA_HIGH;

	for (i=0; i<8; i++, bit_mask >>= 1)
	{
		I2C_CLK_LOW;
		I2C_R_CLK_LOW_STEADY_DELAY;
		I2C_CLK_HIGH;
		I2C_R_CLK_HIGH_STEADY_DELAY;
		I2C_CLK_STICK_WAIT;

		if (I2C_DATA_PIN)
		{
			b |= bit_mask;
		}
	}

	I2C_CLK_LOW;
	I2C_R_CLK_LOW_STEADY_DELAY;

	if (con)
	{
		I2C_DATA_LOW;
	}
	else
	{
		I2C_DATA_HIGH;
	}

	
	I2C_CLK_HIGH;
	I2C_R_CLK_HIGH_STEADY_DELAY;
	I2C_CLK_STICK_WAIT;
	
	I2C_CLK_LOW;

	if (con)
	{
		I2C_DATA_HIGH;
	}

	return b;
}

I2C_RETURN I2C1_write (U8 i2c_addr, U8 *wbuf, U16 len)
{
	U32 i,r,num_error = 0;
	int ret;

	#ifdef RTOS
	ret = OS_MUTEX_I2C_LOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("I2C1 Mutex Lock err\n\r");
	}
	#endif

	do
	{
		r = I2C_START();
		if (r == I2C_SUCCESS)
		{
			r = I2C_write_byte (i2c_addr);
		}

		if (r != I2C_SUCCESS)
		{
			goto IIC_STOP;
		}
		
		for (i=0; i<len; i++)
		{
			if (r == I2C_SUCCESS)
			{
				r = I2C_write_byte (*wbuf++);
			}
			else
			{
				break;
			}
		}

		IIC_STOP:
		I2C_STOP();

	} while ( (r != I2C_SUCCESS) && (num_error++ < I2C_Retries) );

	if (r != I2C_SUCCESS)
	{
		DBG_Printf ("I2C1 W no resp:0x%x\n\r", i2c_addr);
	}

	#ifdef RTOS
	ret = OS_MUTEX_I2C_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("I2C1 Mutex unLock err\n\r");
	}
	#endif

	return r;
}

I2C_RETURN I2C1_read (U8 i2c_addr, U8 *rbuf, U16 len)
{
	U32 i,r,num_error = 0;
	int ret;
	U8 read_addr;
	U8 *pBuf;

	#ifdef RTOS
	ret = OS_MUTEX_I2C_LOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("I2C1 Mutex Lock err\n\r");
	}
	#endif

	do
	{
		pBuf = (U8 *) rbuf;

		r = I2C_START();

		read_addr = i2c_addr | 0x01; //read addr

		if (r == I2C_SUCCESS)
		{
			r = I2C_write_byte (read_addr);
		}

		if (r != I2C_SUCCESS)
		{
			goto IIC_STOP;
		}
		
		for (i=0; i<len-1; i++)
		{
			*pBuf++ = I2C_read_byte (TRUE);
		}

		*pBuf = I2C_read_byte (FALSE);

		IIC_STOP:

		I2C_STOP();

	} while ( (r != I2C_SUCCESS) && (num_error++ < I2C_Retries) );

	if (r != I2C_SUCCESS)
	{
		DBG_Printf ("I2C1 R no resp:0x%x\n\r", i2c_addr);
	}

	#ifdef RTOS
	ret = OS_MUTEX_I2C_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("I2C1 Mutex unLock err\n\r");
	}
	#endif

	return r;
}

#ifdef IIC1_WORD_ADDRESS_SUPPORT
I2C_RETURN I2C1_write_sub (U8 i2c_addr, U16 sub_addr, U8 *wbuf, U16 len)
#else
I2C_RETURN I2C1_write_sub (U8 i2c_addr, U8 sub_addr, U8 *wbuf, U16 len)
#endif
{
	U32 i,r,num_error = 0;
	int ret;

	#ifdef RTOS
	ret = OS_MUTEX_I2C_LOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("I2C0 Mutex Lock err\n\r");
	}
	#endif

	do
	{
		r = I2C_START();
		if (r == I2C_SUCCESS)
		{
			r = I2C_write_byte (i2c_addr);
		}

		if (r == I2C_SUCCESS)
		{
			#ifdef IIC1_WORD_ADDRESS_SUPPORT
			if (word_sub_address == TRUE)
			{
				r = I2C_write_word (sub_addr);
			}
			else
			#endif
			{
				r = I2C_write_byte (sub_addr);
			}
		}

		if (r != I2C_SUCCESS)
		{
			goto IIC_STOP;
		}
		
		for (i=0; i<len; i++)
		{
			if (r == I2C_SUCCESS)
			{
				r = I2C_write_byte (*wbuf++);
			}
			else
			{
				break;
			}
		}

		IIC_STOP:
		I2C_STOP();

	} while ( (r != I2C_SUCCESS) && (num_error++ < I2C_Retries) );

	if (r != I2C_SUCCESS)
	{
		DBG_Printf ("I2C1 W sub no resp:0x%x\n\r", i2c_addr);
	}

	#ifdef RTOS
	ret = OS_MUTEX_I2C_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("I2C1 Mutex unLock err\n\r");
	}
	#endif

	return r;
}

#ifdef IIC1_WORD_ADDRESS_SUPPORT
I2C_RETURN I2C1_read_sub (U8 i2c_addr, U16 sub_addr, U8 *rbuf, U16 len)
#else
I2C_RETURN I2C1_read_sub (U8 i2c_addr, U8 sub_addr, U8 *rbuf, U16 len)
#endif
{
	U32 i,r,num_error = 0;
	int ret;
	U8 read_addr;
	U8 *pBuf;

	#ifdef RTOS
	ret = OS_MUTEX_I2C_LOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("I2C1 Mutex Lock err\n\r");
	}
	#endif

	do
	{
		pBuf = (U8 *) rbuf;

		r = I2C_START();
		if (r == I2C_SUCCESS)
		{
			r = I2C_write_byte (i2c_addr);
		}

		if (r == I2C_SUCCESS)
		{
			#ifdef IIC1_WORD_ADDRESS_SUPPORT
			if (word_sub_address == TRUE)
			{
				r = I2C_write_word (sub_addr);
			}
			else
			#endif
			{
				r = I2C_write_byte (sub_addr);
			}
		}

		read_addr = i2c_addr | 0x01; //read addr

		if (r == I2C_SUCCESS)
		{
			r = I2C_START();
		}

		if (r == I2C_SUCCESS)
		{
			r = I2C_write_byte (read_addr);
		}

		if (r != I2C_SUCCESS)
		{
			goto IIC_STOP;
		}
		
		for (i=0; i<len-1; i++)
		{
			*pBuf++ = I2C_read_byte (TRUE);
		}

		*pBuf = I2C_read_byte (FALSE);

		IIC_STOP:
		
		I2C_STOP();

	} while ( (r != I2C_SUCCESS) && (num_error++ < I2C_Retries) );

	if (r != I2C_SUCCESS)
	{
		DBG_Printf ("I2C1 R sub no resp:0x%x\n\r", i2c_addr);
	}

	#ifdef RTOS
	ret = OS_MUTEX_I2C_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("I2C1 Mutex unLock err\n\r");
	}
	#endif

	return r;
}


/************************************************************************************/
/****************			end										*****************/				
/************************************************************************************/

#endif //I2C1_ENABLE
