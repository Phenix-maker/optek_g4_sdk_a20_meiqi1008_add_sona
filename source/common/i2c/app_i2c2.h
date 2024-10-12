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

#ifndef _APP_I2C2_H_
#define _APP_I2C2_H_


#define IIC2_WORD_ADDRESS_SUPPORT

void app_i2c2_init (void);
void app_i2c2_open (void);
void app_i2c2_close (void);
void app_i2c2_fini (void);

I2C_RETURN I2C2_write (U8 i2c_addr, U8 *wbuf, U16 len);
I2C_RETURN I2C2_read (U8 i2c_addr, U8 *rbuf, U16 len);

#ifdef IIC2_WORD_ADDRESS_SUPPORT
I2C_RETURN I2C2_write_sub (U8 i2c_addr, U16 sub_addr, U8 *wbuf, U16 len);
I2C_RETURN I2C2_read_sub (U8 i2c_addr, U16 sub_addr, U8 *rbuf, U16 len);
#else
I2C_RETURN I2C2_write_sub (U8 i2c_addr, U8 sub_addr, U8 *wbuf, U16 len);
I2C_RETURN I2C2_read_sub (U8 i2c_addr, U8 sub_addr, U8 *rbuf, U16 len);
#endif


#endif //_APP_I2C2_H_
