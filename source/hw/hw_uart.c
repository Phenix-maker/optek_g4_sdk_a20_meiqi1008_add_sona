#include "os_config.h"

#include "regmap.h"


#include "hw_uart.h"
#include "hw_cpu_clock.h"
#include "hw_timer.h"

#include "hal_dma.h"
#include "hal_uart.h"

#include "interrupt.h"

#include <string.h>



/*
 * PUBLIC FUNCTIONS DEFINITION
 *****************************************************************************************
 */

int hw_uart_open( int num, int tx_ena, int rx_ena)
{
	u32 val;
	UART_CONTROLs *pUartCntl = (UART_CONTROLs *) &val;

	DBG_assert( num < en_NUM_MAX );

	if (num == en_UART0)
	{
		val = REG_UART0_CONTRL;

		pUartCntl->tx_ena = tx_ena;
		pUartCntl->tx_fifo_ena = tx_ena;
		pUartCntl->rx_ena = rx_ena;	
		pUartCntl->rx_fifo_ena = rx_ena;	
		REG_UART0_CONTRL = val;
	}
	else if(num == en_UART1)
	{
		val = REG_UART1_CONTRL;

		pUartCntl->tx_ena = tx_ena;
		pUartCntl->tx_fifo_ena = tx_ena;
		pUartCntl->rx_ena = rx_ena;	
		pUartCntl->rx_fifo_ena = rx_ena;
		REG_UART1_CONTRL = val;
	}
	else if(num == en_UART2)
	{
		val = REG_UART2_CONTRL;	

		pUartCntl->tx_ena = tx_ena;
		pUartCntl->tx_fifo_ena = tx_ena;
		pUartCntl->rx_ena = rx_ena;	
		pUartCntl->rx_fifo_ena = rx_ena;
		REG_UART2_CONTRL = val;
	}

	return TRUE;
}

int hw_uart_close( int num )
{
	return TRUE;
}

int hw_uart_setBaudRate( int num, U32 baudRate, U32 sysclk )
{
	u32 baudrate_div;	
	DBG_assert( num < en_NUM_MAX );

	baudrate_div = sysclk/baudRate;;


	if (num == en_UART0)
	{
		REG_UART0_BAUDRATE = baudrate_div;
	}
	else if(num == en_UART1)
	{
		REG_UART1_BAUDRATE = baudrate_div;
	}
	else
	{
		REG_UART2_BAUDRATE = baudrate_div;
	}

	return TRUE;
}

int hw_uart_setParity( int num, U32 parity)
{
	u32 val;
	UART_CONTROLs *pUartCntl = (UART_CONTROLs *) &val;

	if (num == en_UART0)
	{
		val = REG_UART0_CONTRL;
		pUartCntl->parity = parity;
		REG_UART0_CONTRL = val;
	}
	else if(num == en_UART1)
	{
		val = REG_UART1_CONTRL;
		pUartCntl->parity = parity;
		REG_UART1_CONTRL = val;
	}
	else
	{
		val = REG_UART2_CONTRL;
		pUartCntl->parity = parity;
		REG_UART2_CONTRL = val;
	}

	return TRUE;
}

void hw_uart_data_send( int num, char *buf, int size)
{
	uint32_t i;
	UART_REGISTERS* volatile pregs;

	if (num == en_UART0)
	{
		pregs = (UART_REGISTERS*)&REG_UART0_CONTRL;
	}
	else if(num == en_UART1)
	{
		pregs = (UART_REGISTERS*)&REG_UART1_CONTRL;
	}
	else
	{
		pregs = (UART_REGISTERS*)&REG_UART2_CONTRL;
	}

	for (i=0; i<size; i++)
	{
		while (pregs->status.txBufFull);
		pregs->txBuf = *buf++;
	}
}
