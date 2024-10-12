/**
 ****************************************************************************************
 *
 * @file hal_uart1.c
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

#include "fifo.h"
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
    // call back function pointer
    extif_callback callback;
    //parameter returned to callback when operation is over.
    uint32_t param;
};

/* hal UART structure */
typedef struct
{
    // tx callback
    struct uart_txrxcallback tx;
    // rx callback
    struct uart_txrxcallback rx;
    U32 dma_channel;
    // error detect
    uint8_t errordetect;
} hal_uart_stru;

/*
 *****************************************************************************************
 * PRIVATE VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/* hal uart structure */
static hal_uart_stru hal_uart1 = {0};

#define	UART_PRINTF_MAX_BUF  128
#define UART_TX_FIFO_SIZE    1024


uint8_t *puart1_tx_buffer;
uint32_t uart1_tx_buffer_size = 0;
uint8_t *puart1_rx_buffer;
uint32_t uart1_rx_buffer_size = 0;


/*
 *****************************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Finish uart haredware setting function
 *
 * @param[in]  tx_ena    	Enable/disable uart tx function
 * @param[in]  rx_ena     	Enable/disable uart rx function
 * @param[in]  baudrate    	Transfer rate of data
 *
 * @return 0
 ****************************************************************************************
 */
void uart1_dma_tx_handler(void);
void uart1_dma_rx_handler(void);

int32_t hal_uart1_open(int32_t tx_ena, int32_t rx_ena, int32_t baudrate)
{
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &REG_MISC_CLKGATE;;

	pGate->u_uart1_gate = 1; //open clk gate

	if (tx_ena)
	{
		hal_gpio_mux_func_select(UART1_TX_GPIO_INDEX,MUX_SEL_FUNCTION1);
	}
	if (rx_ena)
	{
		hal_gpio_mux_func_select(UART1_RX_GPIO_INDEX,MUX_SEL_FUNCTION1);
	}
	
    hw_uart_setBaudRate(en_UART1, baudrate, PERI_CLK_HZ);
    hw_uart_setParity(en_UART1, UART_PARITY_NON);
	hw_uart_open(en_UART1, tx_ena, rx_ena);//rx auto start if rx_ena = 1,no need start again

    UART1_INT_ENABLE;
    xt_ints_on(LEVEL1_INT1_MASK);

    return 0;
}


/**
 ****************************************************************************************
 * @brief Change transfer data rate function
 *
 * @param[in]  baudrate    	Transfer rate of data
 *
 * @return 0
 ****************************************************************************************
 */
int32_t hal_uart1_baudrate_change(int32_t baudrate)
{
	hw_uart_setBaudRate(en_UART1, baudrate, PERI_CLK_HZ);

	return 0;
}


/**
 ****************************************************************************************
 * @brief Change uart parity function
 *
 * @param[in]  baudrate    	Transfer rate of data
 *
 * @return 0
 ****************************************************************************************
 */
int32_t hal_uart1_setParity(uint32_t parity)
{
	hw_uart_setParity(en_UART1, parity);

	return 0;
}


/**
 *************************************************************************************
 * @brief Enable Interface receive flow.
 *************************************************************************************
 */
void hal_uart1_rx_flow_on(void)
{
	//incomplete function
	DBG_Assert(FALSE);
}


/**
 *************************************************************************************
 * @brief Disable Interface receive flow.
 *************************************************************************************
 */
void hal_uart1_rx_flow_off(void)
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
 *************************************************************************************
 */
void hal_uart1_read(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param)
{
    // Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

	puart1_rx_buffer = bufptr;
	uart1_rx_buffer_size = size;

    hal_uart1.rx.callback = callback;
    hal_uart1.rx.param    = param;

    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART1_CONTRL;

	if (!(pregs->intEnable.rx_buf_not_empty))
	{
		pregs->intEnable.rx_buf_not_empty = 1;
	}
}

/**
 *************************************************************************************
 * @brief Starts a data transmission.
 *
 * @param[in]  bufptr      Pointer to the TX buffer
 * @param[in]  size        Size of the transmission
 * @param[in]  callback    Pointer to the function called back when transfer finished
 * @param[in]  param       parameter returned to callback when transmission is finished
 *************************************************************************************
 */
void hal_uart1_write(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param)
{
    // Sanity check
    DBG_Assert(bufptr != NULL);
    DBG_Assert(size != 0);

    hal_uart1.tx.callback = callback;
    hal_uart1.tx.param    = param;

	puart1_tx_buffer = bufptr;
    uart1_tx_buffer_size = size;
    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART1_CONTRL;
	if (!(pregs->intEnable.tx_buf_half_empty))
	{
		pregs->intEnable.tx_buf_half_empty = 1;
	}
}

/**
 *************************************************************************************
 * @brief
 *************************************************************************************
 */
uint8_t hal_uart1_is_sending(void)
{
	return (hal_uart1.tx.callback != NULL);
} 


/**
 *************************************************************************************
 * @brief 
 *************************************************************************************
 */
void Uart1_IsrHandler(void)
{
    extif_callback callback;
	uint32_t param;
	uint32_t i;

    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART1_CONTRL;
    if( pregs->intEnable.tx_buf_half_empty )
    {
        for (i=0; i<uart1_tx_buffer_size; i++)
        {
            if (pregs->status.txBufTFull_n)
            {	
                pregs->txBuf = *puart1_tx_buffer++;
                uart1_tx_buffer_size--;
            }
            else
            {
                break;
            }
        }

        if (uart1_tx_buffer_size == 0)
        {
            pregs->intEnable.tx_buf_half_empty = 0;
            
            if(hal_uart1.tx.callback != NULL)
            {
                callback = hal_uart1.tx.callback;
                param = hal_uart1.tx.param;

                // Clear callback pointer
                hal_uart1.tx.callback = NULL;
                hal_uart1.tx.param    = NULL;

                // Call handler
                callback(param, EXTIF_STATUS_OK);
            }
        }
    }

    if( pregs->intEnable.rx_buf_not_empty )
    {
        #if ((defined SONA_DSP_PC_TOOLS_BY_UART_FIFO) && (defined SONA_DSP_PC_TOOLS_BY_UART_1))
        if(pregs->status.rxBufEmpty_n)
        {   
            U8 temp;
            temp = pregs->rxBuf;
            sona_fifo_data_receive(temp);
        }            
        #else
        for(i=0;i<uart1_rx_buffer_size;i++)
        {
            if(pregs->status.rxBufEmpty_n)
            {
                *puart1_rx_buffer++ = pregs->rxBuf;
                uart1_rx_buffer_size--;
	        }
        }
        if(uart1_rx_buffer_size == 0)
        {
            if(hal_uart1.rx.callback != NULL)
            {
                callback = hal_uart1.rx.callback;
                param = hal_uart1.rx.param;

                // Clear callback pointer
                hal_uart1.rx.callback = NULL;
                hal_uart1.rx.param    = NULL;	

                // Call handler
                callback(param, EXTIF_STATUS_OK);	
            }
        }
        #endif
    }
}

void uart1_tx_handler(void)
{
	extif_callback callback;
	uint32_t param;
	uint32_t i;

    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART1_CONTRL;
    if( pregs->intEnable.tx_buf_half_empty )
    {
        for (i=0; i<uart1_tx_buffer_size; i++)
        {
            if (pregs->status.txBufTFull_n)
            {	
                pregs->txBuf = *puart1_tx_buffer++;
                uart1_tx_buffer_size--;
            }
            else
            {
                break;
            }
        }

        if (uart1_tx_buffer_size == 0)
        {
            pregs->intEnable.tx_buf_half_empty = 0;
            
            if(hal_uart1.tx.callback != NULL)
            {
                callback = hal_uart1.tx.callback;
                param = hal_uart1.tx.param;

                // Clear callback pointer
                hal_uart1.tx.callback = NULL;
                hal_uart1.tx.param    = NULL;

                // Call handler
                callback(param, EXTIF_STATUS_OK);
            }
        }
    }
}

void uart1_rx_handler(void)
{
	extif_callback callback;
	uint32_t param;
	uint32_t i;

    UART_REGISTERS* pregs = (UART_REGISTERS*)&REG_UART1_CONTRL;
    if( pregs->intEnable.rx_buf_not_empty )
    {
        for(i=0;i<uart1_rx_buffer_size;i++)
        {
            if(pregs->status.rxBufEmpty_n)
            {
            *puart1_rx_buffer++ = pregs->rxBuf;
            uart1_rx_buffer_size--;
            }
        }
    }
    if(uart1_rx_buffer_size == 0)
    {
        if(hal_uart1.rx.callback != NULL)
        {
            callback = hal_uart1.rx.callback;
            param = hal_uart1.rx.param;

            // Clear callback pointer
            hal_uart1.rx.callback = NULL;
            hal_uart1.rx.param    = NULL;	

            // Call handler
            callback(param, EXTIF_STATUS_OK);	
        }
    }
}

// Creation of uart0 external interface api
const extif_api_stru uart1_api =
{
    hal_uart1_read,
    hal_uart1_write,
    hal_uart1_rx_flow_on,
    hal_uart1_rx_flow_off,
};
