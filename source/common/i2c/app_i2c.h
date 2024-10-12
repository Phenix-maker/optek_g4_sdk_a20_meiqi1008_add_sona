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

#ifndef _APP_I2C_H_
#define _APP_I2C_H_


#include "c_def.h"
#include "oem.h"


/*
    @enum I2C_RETURN | Return status for driver calls
*/

typedef enum {
	I2C_BUS_CLK_STRETCH_ERROR = -5,
	I2C_BUS_STICK_ERROR = -4,
	I2C_RX_FIFO_OVER_ERROR = -3,
	I2C_TX_FIFO_OVER_ERROR = -2,
	I2C_NO_ACK_ERROR = -1,

	I2C_SUCCESS = 0,  /* @emem Success */
	I2C_ERROR = 1,    /* @emem Failure */

} I2C_RETURN;


#endif //_APP_I2C_H_
