#ifndef __HW_SPI_H__
#define __HW_SPI_H__



//#define SPI_INT_USE
//#define SPI_TX_INT_ENABLE
//#define SPI_RX_INT_ENABLE

#define SPI_DMA_ENABLE
#define SPI_DMA_TX_ENABLE
#define SPI_DMA_RX_ENABLE


/*********************************/
#define SPI_CS0_PIN_INDEX       GPIO0_14_INDEX
#define SPI_CLK_PIN_INDEX       GPIO0_15_INDEX
#define SPI_D0_PIN_INDEX        GPIO0_16_INDEX
#define SPI_D1_PIN_INDEX        GPIO0_28_INDEX

#define SPI_CS1_PIN_INDEX       GPIO0_29_INDEX


// for special spi interface
#define SPI_SPECIAL_INTERFACE

#ifdef SPI_ENABLE
#define SPI_W_BUSY_PIN        	GPIO0_22_INDEX
#define SPI_R_IRQ_PIN         	GPIO0_24_INDEX
#else
#define SPI_W_BUSY_PIN        	GPIO_UNDEFINE_INDEX
#define SPI_R_IRQ_PIN         	GPIO_UNDEFINE_INDEX
#endif

#define SPI_WRITE_BYSY        	app_gpio_input_level_read(SPI_W_BUSY_PIN)
#define SPI_READ_IRQ          	app_gpio_input_level_read(SPI_R_IRQ_PIN)


#define SPI_CS_AUTO				0
#define SPI_CS_MANUAL			1

#define SPI_MASTER				0
#define SPI_SLAVE				1

#define SPI_MODE_0				0
#define SPI_MODE_1				1
#define SPI_MODE_2				2
#define SPI_MODE_3				3

#define SPI_CS0					0
#define SPI_CS1					1
#define SPI_CS2					2
#define SPI_CS3					3

#define SPI_WORD_8BITS			7
#define SPI_WORD_9BITS			8
#define SPI_WORD_10BITS			9
#define SPI_WORD_11BITS			10
#define SPI_WORD_12BITS			11
#define SPI_WORD_13BITS			12
#define SPI_WORD_14BITS			13
#define SPI_WORD_15BITS			14
#define SPI_WORD_16BITS			15
#define SPI_WORD_32BITS			31

#define SPI_CS_DELAY_HALF_CLK	0
#define SPI_CS_DELAY_1_CLK		1
#define SPI_CS_DELAY_2_CLK		2
#define SPI_CS_DELAY_3_CLK		3
#define SPI_CS_DELAY_4_CLK		4
#define SPI_CS_DELAY_5_CLK		5
#define SPI_CS_DELAY_6_CLK		6
#define SPI_CS_DELAY_7_CLK		7
#define set_spi_cs_delay(a)		bSPI_DELAY = a

//BUS CLK
#define SPI_CLK_DIV_2			0
#define SPI_CLK_DIV_4			1
#define SPI_CLK_DIV_8			2
#define SPI_CLK_DIV_16			3
#define SPI_CLK_DIV_32			4
#define SPI_CLK_DIV_64			5
#define SPI_CLK_DIV_128			6
#define SPI_CLK_DIV_256			7
#define SPI_CLK_DIV_512			8
#define SPI_CLK_DIV_1024		9
#define SPI_CLK_DIV_2048		10
#define SPI_CLK_DIV_4096		11

#define SPI_CLK_DEFAULT         SPI_CLK_DIV_256


void hw_spi_init(void);

void hw_spi_mode_set(U8 mode);
void hw_spi_cs_active_set(U8 con);
void hw_spi_clk_freq_set(U8 div);
void hw_spi_bits_set(U8 bits);

void spi_transfer_size_set(U8 bits, U8 cs_select);

void Spi_Write_Bits(U16 data);
void Spi_Write_Bytes(U8 *buf, U32 byte_size);

void Spi_Write_32Bits(U32 data);

void Spi_Write(U8 *buf, U32 bytes_count);
void Spi_Read(U8 *buf, U32 bytes_count);


void Spi_Page_Write(U8 addr, U8 *buf, U32 byte_size);
void Spi_Page_Read(U8 addr, U8 *buf, U32 byte_size);


void spi_int_handler_fromIsr(void);

void spi_dma_tx_start_with_address(U8 addr, U32 *p_data, U32 size);
void spi_dma_rx_start_with_address(U8 addr, U32 *p_data, U32 size);


#ifdef SPI_TEST
void spi_test(void);
void spi_dma_tx_test(void);
void spi_dma_rx_test(void);
void spi_dma_tx_callback(void);
void spi_dma_rx_callback(void);
#endif

#endif //__HW_SPI_H__
