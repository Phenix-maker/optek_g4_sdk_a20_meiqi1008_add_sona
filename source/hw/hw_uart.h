#ifndef	__HW_UART_H__
#define	__HW_UART_H__


/*
 ****************************************************************************************
 * MARCO DEFINTIONS
 ****************************************************************************************
 */
//#define HW_UART_TEST



/*
 ****************************************************************************************
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */
#define UART_FIFO_SIZE           64 //unit byte
#define	UART_FIFO_HALF_SIZE      (UART_FIFO_SIZE/2)



/**
 ****************************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**
 * @enum codec_memtype
 * @brief Memory type enumeration
 */
typedef	enum {
	en_UART0 = 0,
	en_UART1,
	en_UART2,
	en_NUM_MAX
} enum_UART_NUM;

typedef	enum {
	en_MODE_FIFO,
	en_MODE_NON_FIFO
} enum_UART_MODE;



/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
int hw_uart_open( int num, int tx_ena, int rx_ena);
int hw_uart_setBaudRate( int num, U32 baudRate, U32 sysclk);
int hw_uart_setParity( int num, U32 parity);
void hw_uart_data_send( int num, char *buf, int size);
int uart_tx_open( int num);
int uart_tx_close( int num);
int uart_rx_open( int num);
int uart_rx_close( int num);

#endif //__HW_UART_H__
