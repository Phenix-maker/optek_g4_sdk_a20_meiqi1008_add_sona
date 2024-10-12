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



#define HW_UART0_DMA_USE


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
static hal_uart_stru hal_uart0 = {0};

void uart0_dma_tx_handler(void);
void uart0_dma_rx_handler(void);

#ifndef HW_UART0_DMA_USE
uint8_t *puart0_tx_buffer;
uint32_t uart0_tx_buffer_size = 0;
uint8_t *puart0_rx_buffer;
uint32_t uart0_rx_buffer_size = 0;
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
int32_t hal_uart0_open(int32_t tx_ena, int32_t rx_ena, uint32_t baudrate)
{
#ifdef HW_UART0_DMA_USE
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &REG_MISC_CLKGATE;

	pGate->u_uart0_gate = 1; //open clk gate

	if (tx_ena)
	{
		hal_gpio_mux_func_select(UART0_TX_GPIO_INDEX,MUX_SEL_FUNCTION1);
    	hal_dma_transfer_init(UART0_TX_DMA_ID, (U32 *)(&REG_UART0_TX_DATA), NULL, SOURCE_DMA_UART_0_TX, DMA_TRANS_1BYTE, DMA_BURST_SINGLE, 1, 0, 1, uart0_dma_tx_handler);
		DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(UART0_TX_DMA_ID)));
	}
	if (rx_ena)
	{
		hal_gpio_mux_func_select(UART0_RX_GPIO_INDEX,MUX_SEL_FUNCTION1);
		hal_dma_transfer_init(UART0_RX_DMA_ID, NULL, (U32 *)(&REG_UART0_RX_DATA), SOURCE_DMA_UART_0_RX, DMA_TRANS_1BYTE, DMA_BURST_SINGLE, 0, 1, 1, uart0_dma_rx_handler);
		DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(UART0_RX_DMA_ID)));
	}

	
    hw_uart_setBaudRate(en_UART0, baudrate, PERI_CLK_HZ);
    hw_uart_setParity(en_UART0, UART_PARITY_NON);
	hw_uart_open(en_UART0, tx_ena, rx_ena);//rx auto start if rx_ena = 1,no need start again

    xt_ints_on(LEVEL3_INT9_MASK);

    return 0;
#else
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &REG_MISC_CLKGATE;

	pGate->u_uart0_gate = 1; //open clk gate

	if (tx_ena)
	{
		hal_gpio_mux_func_select(UART0_TX_GPIO_INDEX,MUX_SEL_FUNCTION1);
	}
	if (rx_ena)
	{
		hal_gpio_mux_func_select(UART0_RX_GPIO_INDEX,MUX_SEL_FUNCTION1);
	}
	
    hw_uart_setBaudRate(en_UART0, baudrate, PERI_CLK_HZ);
    hw_uart_setParity(en_UART0, UART_PARITY_NON);
	hw_uart_open(en_UART0, tx_ena, rx_ena);//rx auto start if rx_ena = 1,no need start again

    UART0_INT_ENABLE;
    xt_ints_on(LEVEL1_INT1_MASK);

    return 0;
#endif
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
int32_t hal_uart0_baudrate_change(uint32_t baudrate)
{
	hw_uart_setBaudRate(en_UART0, baudrate, PERI_CLK_HZ);
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
int32_t hal_uart0_setParity(uint32_t parity)
{
	hw_uart_setParity(en_UART0, parity);
    return 0;
}


/**
 *************************************************************************************
 * @brief Enable Interface receive flow.
 *************************************************************************************
 */
void hal_uart0_rx_flow_on(void)
{
	//incomplete function
	DBG_Assert(FALSE);
}


/**
 *************************************************************************************
 * @brief Disable Interface receive flow.
 *************************************************************************************
 */
void hal_uart0_rx_flow_off(void)
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
void hal_uart0_read(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param)
{
#ifdef HW_UART0_DMA_USE
    //Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

    hal_uart0.rx.callback = callback;
    hal_uart0.rx.param    = param;

    hal_dma_receive_data(UART0_RX_DMA_ID, bufptr, size);
#else
    // Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

	puart0_rx_buffer = bufptr;
	uart0_rx_buffer_size = size;

    hal_uart0.rx.callback = callback;
    hal_uart0.rx.param    = param;

    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART0_CONTRL;

	if (!(pregs->intEnable.rx_buf_not_empty))
	{
		pregs->intEnable.rx_buf_not_empty = 1;
	}
#endif
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
void hal_uart0_write(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param)
{
#ifdef HW_UART0_DMA_USE
    //Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

    hal_uart0.tx.callback = callback;
    hal_uart0.tx.param    = param;

    hal_dma_send_data(UART0_TX_DMA_ID, bufptr, size);
#else
    // Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

    hal_uart0.tx.callback = callback;
    hal_uart0.tx.param    = param;

	puart0_tx_buffer = bufptr;
    uart0_tx_buffer_size = size;
    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART0_CONTRL;
	if (!(pregs->intEnable.tx_buf_half_empty))
	{
		pregs->intEnable.tx_buf_half_empty = 1;
	}
#endif
}


/**
 *************************************************************************************
 * @brief
 *************************************************************************************
 */
uint8_t hal_uart0_is_sending(void)
{
	return (hal_uart0.tx.callback != NULL);
} 


/**
 *************************************************************************************
 * @brief 
 *************************************************************************************
 */
void Uart0_IsrHandler(void)
{
#ifdef HW_UART0_DMA_USE
	extif_callback callback;
	uint32_t param;

	if(hal_uart0.tx.callback != NULL)
	{
		callback = hal_uart0.tx.callback;
		param = hal_uart0.tx.param;

		// Clear callback pointer
		hal_uart0.tx.callback = NULL;
		hal_uart0.tx.param    = NULL;	

		// Call handler
		callback(param, EXTIF_STATUS_OK);	
	}

	if(hal_uart0.rx.callback != NULL)
	{
		callback = hal_uart0.rx.callback;
		param = hal_uart0.rx.param;

		// Clear callback pointer
		hal_uart0.rx.callback = NULL;
		hal_uart0.rx.param    = NULL;	

		// Call handler
		callback(param, EXTIF_STATUS_OK);	
	}
#else
    extif_callback callback;
	uint32_t param;
	uint32_t i;

    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART0_CONTRL;
    if( pregs->intEnable.tx_buf_half_empty )
    {
        for (i=0; i<uart0_tx_buffer_size; i++)
        {
            if (pregs->status.txBufTFull_n)
            {	
                pregs->txBuf = *puart0_tx_buffer++;
                uart0_tx_buffer_size--;
            }
            else
            {
                break;
            }
        }

        if (uart0_tx_buffer_size == 0)
        {
            pregs->intEnable.tx_buf_half_empty = 0;
            
            if(hal_uart0.tx.callback != NULL)
            {
                callback = hal_uart0.tx.callback;
                param = hal_uart0.tx.param;

                // Clear callback pointer
                hal_uart0.tx.callback = NULL;
                hal_uart0.tx.param    = NULL;

                // Call handler
                callback(param, EXTIF_STATUS_OK);
            }
        }
    }

    if( pregs->intEnable.rx_buf_not_empty )
    {
        for(i=0;i<uart0_rx_buffer_size;i++)
        {
            if(pregs->status.rxBufEmpty_n)
            {
            *puart0_rx_buffer++ = pregs->rxBuf;
            uart0_rx_buffer_size--;
            }
        }
        if(uart0_rx_buffer_size == 0)
        {
            if(hal_uart0.rx.callback != NULL)
            {
                callback = hal_uart0.rx.callback;
                param = hal_uart0.rx.param;

                // Clear callback pointer
                hal_uart0.rx.callback = NULL;
                hal_uart0.rx.param    = NULL;	

                // Call handler
                callback(param, EXTIF_STATUS_OK);	
            }
        }
    }
#endif
}

#ifdef HW_UART0_DMA_USE
void uart0_dma_tx_handler(void)
{
    extif_callback callback;
	uint32_t param;

	if(hal_uart0.tx.callback != NULL)
	{
		callback = hal_uart0.tx.callback;
		param = hal_uart0.tx.param;

		// Clear callback pointer
		hal_uart0.tx.callback = NULL;
		hal_uart0.tx.param    = NULL;	

		// Call handler
		callback(param, EXTIF_STATUS_OK);	
	}
}

void uart0_dma_rx_handler(void)
{
    extif_callback callback;
	uint32_t param;

	if(hal_uart0.rx.callback != NULL)
	{
		callback = hal_uart0.rx.callback;
		param = hal_uart0.rx.param;

		// Clear callback pointer
		hal_uart0.rx.callback = NULL;
		hal_uart0.rx.param    = NULL;	

		// Call handler
		callback(param, EXTIF_STATUS_OK);	
	}
}
#endif

// Creation of uart0 external interface api
const extif_api_stru uart0_api =
{
    hal_uart0_read,
    hal_uart0_write,
    hal_uart0_rx_flow_on,
    hal_uart0_rx_flow_off,
};
