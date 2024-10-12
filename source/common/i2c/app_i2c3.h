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

#ifndef _APP_I2C3_H_
#define _APP_I2C3_H_


#define IIC3_WORD_ADDRESS_SUPPORT

void app_i2c3_init (void);
void app_i2c3_open (void);
void app_i2c3_close (void);
void app_i2c3_fini (void);

I2C_RETURN I2C3_write (U8 i2c_addr, U8 *wbuf, U16 len);
I2C_RETURN I2C3_read (U8 i2c_addr, U8 *rbuf, U16 len);

#ifdef IIC3_WORD_ADDRESS_SUPPORT
I2C_RETURN I2C3_write_sub (U8 i2c_addr, U16 sub_addr, U8 *wbuf, U16 len);
I2C_RETURN I2C3_read_sub (U8 i2c_addr, U16 sub_addr, U8 *rbuf, U16 len);
#else
I2C_RETURN I2C3_write_sub (U8 i2c_addr, U8 sub_addr, U8 *wbuf, U16 len);
I2C_RETURN I2C3_read_sub (U8 i2c_addr, U8 sub_addr, U8 *rbuf, U16 len);
#endif


#endif //_APP_I2C3_H_
