#ifndef _HAL_UART_H_
#define _HAL_UART_H_



/*
 ****************************************************************************************
 * CONSTANT DEFINITIONS
 ****************************************************************************************
 */
 /**
 * @brief UART transfer parity
 */
#define	UART_PARITY_NON			0
#define	UART_PARITY_EVEN		1
#define	UART_PARITY_ODD			2


/**
 * @enum enum_BAUD_RATE
 * @brief UART hardware transfer baudrate
 */
typedef enum
{ 
	BAUD_RATE_9600     = 9600,
	BAUD_RATE_19200    = 19200,
	BAUD_RATE_38400    = 38400,
	BAUD_RATE_115200   = 115200,
	BAUD_RATE_115200X2 = 115200*2,
	BAUD_RATE_115200X4 = 115200*4,
	BAUD_RATE_115200X8 = 115200*8
} enum_BAUD_RATE;


/**
 * @brief UART hardware transfer baudrate setting
 */
#define UART0_BAUDRATE	BAUD_RATE_115200
#define UART1_BAUDRATE	BAUD_RATE_115200
#define UART2_BAUDRATE	BAUD_RATE_115200

#define UART_FIFO_BAUDRATE	BAUD_RATE_115200

/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
int32_t hal_uart0_open(int32_t tx_ena, int32_t rx_ena, uint32_t baudrate);
int32_t hal_uart0_baudrate_change(uint32_t baudrate);
int32_t hal_uart0_setParity(uint32_t parity);
void hal_uart0_read(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param);
void hal_uart0_write(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param);
uint8_t hal_uart0_is_sending(void);

int32_t hal_uart1_open(int tx_ena, int rx_ena, int baudrate);
int32_t hal_uart1_baudrate_change(int baudrate);
int32_t hal_uart1_setParity(uint32_t parity);
void hal_uart1_read(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param);
void hal_uart1_write(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param);
uint8_t hal_uart1_is_sending(void);

int32_t hal_uart2_open(int tx_ena, int rx_ena, int baudrate);
int32_t hal_uart2_baudrate_change(int baudrate);
int32_t hal_uart2_setParity(uint32_t parity);
void hal_uart2_read(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param);
void hal_uart2_write(uint8_t *bufptr, uint32_t size, extif_callback callback, uint32_t param);
uint8_t hal_uart2_is_sending(void);





extern const extif_api_stru uart0_api;
extern const extif_api_stru uart1_api;
extern const extif_api_stru uart2_api;
extern const extif_api_stru uart_fifo_api;

#endif //_APP_UART_H_
