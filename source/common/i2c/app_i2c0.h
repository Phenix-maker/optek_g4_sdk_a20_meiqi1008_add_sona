/*
** Copyright (C) 2002 Optek. All rights reserved.
**
** The information and source code contained herein is the exclusive
** property of Mediamatics and may  not be disclosed, examined or
** reproduced in whole or in part without explicit written authorization
** from the company.
*/

/*
** Author:
** 
** Date:
**
** Description:
*/

#ifndef _APP_I2C0_H_
#define _APP_I2C0_H_


//#define IIC0_WORD_ADDRESS_SUPPORT

void app_i2c0_init (void);
void app_i2c0_open (void);
void app_i2c0_close (void);
void app_i2c0_fini (void);

I2C_RETURN I2C0_write (U8 i2c_addr, U8 *wbuf, U16 len);
I2C_RETURN I2C0_read (U8 i2c_addr, U8 *rbuf, U16 len);

#ifdef IIC0_WORD_ADDRESS_SUPPORT
I2C_RETURN I2C0_write_sub (U8 i2c_addr, U16 sub_addr, U8 *wbuf, U16 len);
I2C_RETURN I2C0_read_sub (U8 i2c_addr, U16 sub_addr, U8 *rbuf, U16 len);
#else
I2C_RETURN I2C0_write_sub (U8 i2c_addr, U8 sub_addr, U8 *wbuf, U16 len);
I2C_RETURN I2C0_read_sub (U8 i2c_addr, U8 sub_addr, U8 *rbuf, U16 len);
#endif


//audio AMP NTP8928/NTP8938 i2c pin
/*	check pins assignment before using i2c0 */
#if 0//def
#define I2C0_SCLK_PIN     GPIO0_10_INDEX
#define I2C0_SDATA_PIN    GPIO0_11_INDEX
#else
#define I2C0_SCLK_PIN     GPIO0_07_INDEX
#define I2C0_SDATA_PIN    GPIO0_08_INDEX
#endif


#endif //_APP_I2C0_H_
