/*
 * hw_spi_2.c
 *
 *  Created on: 2024/4/2
 *      Author: Windows
 */

#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"

#include "hal_gpio.h"

//#define SPI_2_ENABLE

#ifdef SPI_2_ENABLE
#include "hw_spi_2.h"

#define SPI_2_FUNC_ENABLE
#define DBG_Printf

#define SPI_2_CS_USED

void hw_spi_2_en_select(U8 sel)
{
	U32 val;
	val = REG_MISC_CPUCLK;
	MISC_CPUCLK_CONTROLs *pCpuClk = (MISC_CPUCLK_CONTROLs *) &val;

	if (sel == FALSE)
	{
		pCpuClk->spi_2_en = 0;
	}
	else if (sel == TRUE)
	{
		pCpuClk->spi_2_en = 1;
	}
	else
	{
		DBG_Assert(FALSE);
	}

	REG_MISC_CPUCLK = val;
}

void hw_spi_init(void)
{
	SPI_CONTROLs *pSpiCntl;
	SPI_FORMATs *pSpiFormat;

	U32 val;
	pSpiCntl = (SPI_CONTROLs *) &val;
	pSpiFormat = (SPI_FORMATs *) &val;

	#ifdef RTOS
	mutex_spi = xSemaphoreCreateMutex();
	#endif

	#ifdef SPI_2_CS_USED
	//SPI_2_CS
	hal_gpio_mux_func_select(SPI_2_CS_PIN_INDEX, MUX_SEL_FUNCTION3);
	#endif

	//SPI_2_CLK
	hal_gpio_mux_func_select(SPI_2_CLK_PIN_INDEX, MUX_SEL_FUNCTION3);

	//SPI_2_Data
	hal_gpio_mux_func_select(SPI_2_Data_PIN_INDEX, MUX_SEL_FUNCTION3);


	hw_spi_2_en_select(TRUE);

	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &val;

	val = REG_MISC_CLKGATE;

	pGate->spi_gate = 1; //spi clk
	REG_MISC_CLKGATE = val;

	val = REG_SPI_2_FORMAT;

	/*
	 * Mode 0: cpha=0, cpol=0 (SCLK is LOW in the inactive state)
	 * idle: CS is high, CLK is low.
	 * work: DATA is changed when CLK is low.
	 * Mode 3: cpha=1, cpol=1 (SCLK is high in the inactive state)
	 */
	pSpiFormat->cpha = 0;
	pSpiFormat->cpol = 0;


	// 0 -> ----|____|----
	pSpiFormat->autocsActive = 0;
	pSpiFormat->cssel = SPI_CS0;
	//pSpiFormat->cssel = SPI_CS1;

	#ifdef READ_SPI_FLASH_ONE_PIN
	pSpiFormat->din_out = 0;
	#else
	pSpiFormat->din_out = 1;
	#endif
	REG_SPI_FORMAT = val;


	val = REG_SPI_CONTROL;
	pSpiCntl->reset = 0;
	pSpiCntl->dmaSel = 1;
	//pSpiCntl->txFifoEn = 1;
	//pSpiCntl->rxFifoEn = 1;

	//master mode
	#ifdef SPI_SLAVE_TEST
	pSpiCntl->slave = SPI_SLAVE;
	#else
	pSpiCntl->slave = SPI_MASTER;
	#endif //SPI_SLAVE_TEST


	#ifdef SPI_Q_WRITE

	#ifdef SPI_Q_16B_TX_DATA
	pSpiCntl->bits = SPI_WORD_16BITS;
	#elif (defined SPI_Q_32B_TX_DATA)
	pSpiCntl->bits = SPI_WORD_32BITS;
	#else
	pSpiCntl->bits = SPI_WORD_8BITS;
	#endif //SPI_Q_16B_TX_DATA

	#else

	pSpiCntl->bits = SPI_WORD_8BITS;

	#endif //SPI_Q_WRITE


	//SCLK frequency: Fcrystal/2/DIV
	//if using LCDST073, it isn't ok for sending data continuously
	/*
	 * SPI_CLK_DIV_32 -> 50 KHz
	 * SPI_CLK_DIV_16 -> 1.5 MHz
	 * SPI_CLK_DIV_8  -> 3 MHz
	 * SPI_CLK_DIV_4  -> 6 MHz
	 * SPI_CLK_DIV_2  -> 12 MHz
	 */
	pSpiCntl->div = SPI_CLK_DIV_8; //3 MHz

	pSpiCntl->txFifoEn = 1;
	pSpiCntl->rxFifoEn = 1;

	#ifdef SPI_Q_WRITE
	//qspi support
	pSpiCntl->qspi	= 1;
	#else
	pSpiCntl->qspi	= 0;
	#endif

	//enable spi
	pSpiCntl->enable = 1;
	REG_SPI_CONTROL = val;

	#ifdef SPI_INT_USE
	SPI_INT_CONTROL *pSpiIntCntl = (SPI_INT_CONTROL *)&val;
	val = REG_SPI_INT_MASK;

	#ifdef SPI_TX_INT_ENABLE
	//pSpiIntCntl->txBufTFull;
	//pSpiIntCntl->txBufEmpty = 1;
	pSpiIntCntl->txBufEmpty_n = 1;
	#endif

	#ifdef SPI_RX_INT_ENABLE
	//pSpiIntCntl->rxBufTFull;
	pSpiIntCntl->rxBufEmpty = 0;
	pSpiIntCntl->rxBufEmpty_n = 1;
	#endif

	REG_SPI_INT_MASK = val;
	REG_INT_ENABLE(SPI_INT_MASK);
	XT_INTS_ON(LEVEL1_INT1_MASK);
	#endif //SPI_INT_USE


	#ifdef SPI_TEST
	spi_tx_len = 4;
	spi_rx_len = 4;
	spi_tx_buf_index = 0;
	spi_rx_buf_index = 0;

	#ifdef SPI_DMA_TX_TEST
	spi_dma_tx_test();
	#elif defined SPI_DMA_RX_TEST
	//spi_dma_rx_test();
	app_timer_spi_dma_timer_set(2000);
	#else
	spi_test();
	#endif

	#endif //SPI_TEST
}













#endif
