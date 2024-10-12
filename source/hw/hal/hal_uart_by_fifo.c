/**
 ****************************************************************************************
 *
 * @file hal_uart0.c
 *
 * @brief uart hal asynchronous read and write data implement.
 *
 ****************************************************************************************
 */

#include "os_config.h"

#include "regmap.h"

#include "hw_uart.h"
#include "hw_cpu_clock.h"

#include "hal_gpio.h"
#include "hal_uart.h"
#include "hal_dma.h"

#include "printf.h"
#include "interrupt.h"
#include "flash_boot.h"



/*
 *****************************************************************************************
 * STRUCT DEFINITIONS
 *****************************************************************************************
 */

/* UART TX RX callback */
struct uart_txrxcallback
{
    //call back function pointer
    extif_callback callback;
    //parameter returned to callback when operation is over.
    uint32_t param;
};

/* hal UART structure */
typedef struct
{
    //tx callback
    struct uart_txrxcallback tx;
    //rx callback
    struct uart_txrxcallback rx;
    U32 dma_channel;
    //error detect
    uint8_t errordetect;
} hal_uart_stru;

/*
 *****************************************************************************************
 * PRIVATE VARIABLE DEFINITIONS
 *****************************************************************************************
 */

/* hal uart structure */
static hal_uart_stru hal_uart_fifo = {0};

void uart_fifo_dma_tx_handler(void);
void uart_fifo_dma_rx_handler(void);

#ifndef HW_UART_FIFO_DMA_USE
uint8_t *puart_fifo_tx_buffer;
uint32_t uart_fifo_tx_buffer_size = 0;
uint8_t *puart_fifo_rx_buffer;
uint32_t uart_fifo_rx_buffer_size = 0;
#endif


/*
 *****************************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Finish uart hardware setting function
 *
 * @param[in]  tx_ena    	Enable/disable uart tx function
 * @param[in]  rx_ena     	Enable/disable uart rx function
 * @param[in]  baudrate    	Transfer rate of data
 *
 * @return 0
 ****************************************************************************************
 */
int32_t hal_uart_fifo_open(int32_t tx_ena, int32_t rx_ena, uint32_t baudrate)
{
    return 0;
}


/**
 ****************************************************************************************
 * @brief Change transfer data rate function
 *
 * @param[in]  baudrate    	Transfer rate of data
 *
 * @return 0, with no special significance
 ****************************************************************************************
 */
int32_t hal_uart_fifo_baudrate_change(uint32_t baudrate)
{
    return 0;
}


/**
 ****************************************************************************************
 * @brief Change uart parity function
 *
 * @param[in]  baudrate    	Transfer rate of data
 *
 * @return 0, with no special significance
 ****************************************************************************************
 */
int32_t hal_uart_fifo_setParity(uint32_t parity)
{
    return 0;
}


/**
 *************************************************************************************
 * @brief Enable Interface receive flow.
 *************************************************************************************
 */
void hal_uart_fifo_rx_flow_on(void)
{
	//incomplete function
	DBG_Assert(FALSE);
}


/**
 *************************************************************************************
 * @brief Disable Interface receive flow.
 *************************************************************************************
 */
void hal_uart_fifo_rx_flow_off(void)
{
	//incomplete function
	DBG_Assert(FALSE);
}


/**
 *************************************************************************************
 * @brief Starts a data reception.
 *
 * @param[out] bufptr      Pointer to the RX buffer
 * @param[in]  size        Size of the expected reception
 * @param[in]  callback    Pointer to the function called back when transfer finished
 * @param[in]  param       parameter returned to callback when reception is finished
 *
 *************************************************************************************
 */
void hal_uart_fifo_read(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param)
{

}


/**
 *************************************************************************************
 * @brief Starts a data transmission.
 *
 * @param[in]  bufptr      Pointer to the TX buffer
 * @param[in]  size        Size of the transmission
 * @param[in]  callback    Pointer to the function called back when transfer finished
 * @param[in]  param       parameter returned to callback when transmission is finished
 *
 *************************************************************************************
 */
void hal_uart_fifo_write(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param)
{

}


/**
 *************************************************************************************
 * @brief
 *************************************************************************************
 */
uint8_t hal_uart_fifo_is_sending(void)
{
	return (hal_uart_fifo.tx.callback != NULL);
} 


/**
 *************************************************************************************
 * @brief 
 *************************************************************************************
 */
void Uart_fifo_IsrHandler(void)
{

}


// Creation of uart fifo external interface api
const extif_api_stru uart_fifo_api =
{
    hal_uart_fifo_read,
    hal_uart_fifo_write,
    hal_uart_fifo_rx_flow_on,
    hal_uart_fifo_rx_flow_off,
};
