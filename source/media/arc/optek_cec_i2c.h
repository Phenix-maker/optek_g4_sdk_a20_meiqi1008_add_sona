#ifndef __OPTEK_CEC_I2C_H__
#define __OPTEK_CEC_I2C_H__


#define CEC_IIC_WORD_ADDRESS_SUPPORT

typedef enum {
	I2C_SUCCESS,	/* @emem Success */
	I2C_ERROR		/* @emem Failure */
} I2C_RETURN;



#ifdef CEC_IIC_WORD_ADDRESS_SUPPORT
I2C_RETURN CEC_I2C_write_sub (U8 i2c_addr, U8 sub_addr, U8 *wbuf, U16 len);
#else
I2C_RETURN CEC_I2C_write_sub (U8 i2c_addr, U16 sub_addr, U8 *wbuf, U16 len);
#endif

#ifdef CEC_IIC_WORD_ADDRESS_SUPPORT
I2C_RETURN CEC_I2C_read_sub (U8 i2c_addr, U16 sub_addr, U8 *rbuf, U16 len);
#else
I2C_RETURN CEC_I2C_read_sub (U8 i2c_addr, U8 sub_addr, U8 *rbuf, U16 len);
#endif

void cec_i2c_init (void);


#endif //__OPTEK_CEC_I2C_H__
