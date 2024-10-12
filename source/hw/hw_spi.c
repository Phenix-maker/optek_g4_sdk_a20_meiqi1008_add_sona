#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hw_spi.h"

#include "hal_gpio.h"
#include "hal_dma.h"

#include "interrupt.h"



#ifdef SPI_ENABLE


//#define READ_SPI_FLASH
#define READ_SPI_FLASH_ONE_PIN //one pin for spi flash read and write


//#define SPI_SLAVE_TEST //spi slave

#ifndef SPI_SLAVE_TEST
#define MASTER_SOC_2 //spi master delay

#endif

#define SPI_CS_ACTIVE_LOW

#define SPI_CS0_USED
//#define SPI_CS1_USED

//#define SPI_READ_FUNCTION

//#define SPI_Q_WRITE
#ifdef SPI_Q_WRITE
//#define SPI_Q_32B_TX_DATA
//#define SPI_Q_16B_TX_DATA
#endif

#define SPI_FIFO_LENGTH_SIZE    36

#define SPI_STATUS_IDLE_POLLING


#ifdef RTOS
static void *mutex_spi;

#define SPI_MUTEX_LOCK       xSemaphoreTake( mutex_spi, portMAX_DELAY)
#define SPI_MUTEX_UNLOCK     xSemaphoreGive( mutex_spi )

#define OS_MUTEX_SPI_LOCK    SPI_MUTEX_LOCK
#define OS_MUTEX_SPI_UNLOCK  SPI_MUTEX_UNLOCK
#else
#define SPI_MUTEX_LOCK
#define SPI_MUTEX_UNLOCK

#define OS_MUTEX_SPI_LOCK    SPI_MUTEX_LOCK
#define OS_MUTEX_SPI_UNLOCK  SPI_MUTEX_UNLOCK
#endif


#ifdef SPI_TEST

#if 0
#define TX_BUF_LEN    32
#define RX_BUF_LEN    32
#else
#define TX_BUF_LEN    4
#define RX_BUF_LEN    4
#endif

U8 spi_tx_buf[2][TX_BUF_LEN];
U8 spi_rx_buf[2][RX_BUF_LEN];

U16 spi_tx_len;
U16 spi_rx_len;
U8 spi_tx_buf_index;
U8 spi_rx_buf_index;
#endif


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

	#ifdef SPI_CS0_USED
	//SPI_CS0
	hal_gpio_mux_func_select(SPI_CS0_PIN_INDEX, MUX_SEL_FUNCTION1);
	#endif

	#ifdef SPI_CS1_USED
	//SPI_CS1
	hal_gpio_mux_func_select(SPI_CS1_PIN_INDEX, MUX_SEL_FUNCTION4);
	#endif

	//SPI_CLK
	hal_gpio_mux_func_select(SPI_CLK_PIN_INDEX, MUX_SEL_FUNCTION1);
	
	//SPI_D0 
	hal_gpio_mux_func_select(SPI_D0_PIN_INDEX, MUX_SEL_FUNCTION1);

	//hw_spi_2_en_select(TRUE);

	#ifdef SPI_READ_FUNCTION
	//SPI_D1 as SPI_MISO
	hal_gpio_mux_func_select(SPI_D1_PIN_INDEX, MUX_SEL_FUNCTION4);
	#endif

	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &val;

	val = REG_MISC_CLKGATE;

	pGate->spi_gate = 1; //spi clk
	REG_MISC_CLKGATE = val;

	val = REG_SPI_FORMAT;

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
	//pSpiCntl->div = SPI_CLK_DIV_4; //6 MHz

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

//#define SPI_MODE_CHECK_OUT

void hw_spi_mode_set(U8 mode)
{
	SPI_FORMATs *p;
	u32 val;
	p = (SPI_FORMATs *) &val;

	val = REG_SPI_FORMAT;

	if (mode == SPI_MODE_0)
	{
		//idle: CS is high, CLK is low.
		//work: DATA is changed when CLK is low.
		p->cpha = 0;
		p->cpol = 0;

		#ifdef SPI_MODE_CHECK_OUT
		delayus(1);
		REG_SPI_FORMAT = val;
		delayus(1);
		val = REG_SPI_FORMAT;
		delayus(1);
		if ( !((p->cpha == 0) && (p->cpol == 0)))
		{
			DBG_Assert(FALSE);
		}
		#endif
	}
	else if (mode == SPI_MODE_1)
	{
		//idle: CS is high, Clk is high. 
		//work: DATA is changed when CLK is high.
		p->cpha = 0;
		p->cpol = 1;

		#ifdef SPI_MODE_CHECK_OUT
		delayus(1);
		REG_SPI_FORMAT = val;
		delayus(1);
		val = REG_SPI_FORMAT;
		delayus(1);
		if ( !((p->cpha == 0) && (p->cpol == 1)))
		{
			DBG_Assert(FALSE);
		}
		#endif
	}
	else if (mode == SPI_MODE_2)
	{
		//idle: CS is high, CLK is low.
		//work: DATA is changed when CLK is high.
		p->cpha = 1;
		p->cpol = 0;

		#ifdef SPI_MODE_CHECK_OUT
		delayus(1);
		REG_SPI_FORMAT = val;
		val = REG_SPI_FORMAT;
		delayus(1);
		if ( !((p->cpha == 1) && (p->cpol == 0)) )
		{
			DBG_Assert(FALSE);
		}
		#endif
	}
	else if (mode == SPI_MODE_3)
	{
		//idle: CS is high, CLK is high. 
		//work: DATA is changed when CLK is low.
		p->cpha = 1;
		p->cpol = 1;

		#ifdef SPI_MODE_CHECK_OUT
		delayus(1);
		REG_SPI_FORMAT = val;
		delayus(1);
		val = REG_SPI_FORMAT;
		delayus(1);
		if ( !((p->cpha == 1) && (p->cpol == 1)) )
		{
			DBG_Assert(FALSE);
		}
		#endif
	}
	else
	{
		DBG_Assert(FALSE);
	}

	#ifndef SPI_MODE_CHECK_OUT
	REG_SPI_FORMAT = val;
	#endif
}

void hw_spi_cs_active_set(U8 con)
{
	SPI_FORMATs *p;

	u32 val;
	p = (SPI_FORMATs *) &val;

	val = REG_SPI_FORMAT;

	#ifdef SPI_CS_ACTIVE_LOW
	// CS is active when CS is low.
	//p->format.csvalue = 1;
	//p->format.csinactivev = 1;
	// 0 -> ----|____|----
	p->autocsActive = 0;
	#else
	// CS is active when CS is high.
	//p->format.csvalue = 0;
	//p->format.csinactivev = 0;
	// 0 -> ____|----|____
	p->autocsActive = 1;
	#endif
	
	REG_SPI_FORMAT = val;
}

void hw_spi_cs_select_set(U8 cs)
{
	SPI_FORMATs *p;
	U32 val;
	
	p = (SPI_FORMATs *) &val;
	val = REG_SPI_FORMAT;
	p->cssel = cs;
	
	REG_SPI_FORMAT = val;	
}

void hw_spi_cs_level_set(U8 con)
{

}

void hw_spi_clk_freq_set(U8 div)
{
	SPI_CONTROLs *p;
	U32 val;
	
	p = (SPI_CONTROLs *) &val;
	val = REG_SPI_CONTROL;
	p->div = div;
	
	REG_SPI_CONTROL = val;
}

void hw_spi_bits_set(U8 bits)
{
	SPI_CONTROLs *p;
	U32 val;
	
	p = (SPI_CONTROLs *) &val;
	DBG_Assert((bits >= SPI_WORD_8BITS) && (bits <= SPI_WORD_16BITS));
	val = REG_SPI_CONTROL;
	p->bits = (bits - 1);
	
	REG_SPI_CONTROL = val;	
}

void Spi_Write_Bytes(U8 *buf, U32 byte_size)
{
	U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
	U32 i;
	U32 temp;

	#ifdef RTOS
	ret = OS_MUTEX_SPI_LOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex Lock err\n\r");
	}
	#endif

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	#if 1
	REG_SPI_MAX_TRANS_NUM = byte_size;
	REG_SPI_RXTX_NUM = byte_size;

	for (i=0; i<byte_size; i++)
	{
		REG_SPI_TX_DATA = *buf++;
		while(1)
		{
			val = REG_SPI_STATUS;
			asm("nop");
			if (p_status->txBufTFull_n)
			{
				break;
			}
		}
	}

	//delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X

	#else
	U32 int fifo_length_cnt = byte_size/SPI_FIFO_LENGTH_SIZE;

	for (j=0; j<fifo_length_cnt; j++)
	{
		REG_SPI_MAX_TRANS_NUM = SPI_FIFO_LENGTH_SIZE;
		REG_SPI_RXTX_NUM = SPI_FIFO_LENGTH_SIZE;

		for (i=0; i<SPI_FIFO_LENGTH_SIZE; i++)
		{
			REG_SPI_TX_DATA = *buf++;
		}

		while(1)
		{
			val = REG_SPI_STATUS;
			asm("nop");
			if ( p_status->txBufTFull_n)
			{
				break;
			}		
		}
	}

	U32 remainder = byte_size%SPI_FIFO_LENGTH_SIZE;
	if (remainder > 0)
	{
		REG_SPI_MAX_TRANS_NUM = remainder;
		REG_SPI_RXTX_NUM = remainder;

		for (i=0; i<remainder; i++)
		{
			REG_SPI_TX_DATA = *buf++;
		}
	}
	#endif

	#ifdef RTOS
	ret = OS_MUTEX_SPI_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex unLock err\n\r");
	}
	#endif
}

void Spi_Page_Write(U8 addr, U8 *buf, U32 byte_size)
{
	volatile U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
	U32 i;
	U32 temp;

	#ifdef RTOS
	ret = OS_MUTEX_SPI_LOCK;
	if (ret == FALSE)hw_spi
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex Lock err\n\r");
	}
	#endif

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	#if 1
	REG_SPI_MAX_TRANS_NUM = byte_size+1;
	REG_SPI_RXTX_NUM = byte_size+1;

	REG_SPI_TX_DATA = addr;

	for (i=0; i<byte_size; i++)
	{
		REG_SPI_TX_DATA = *buf++;
		while(1)
		{
			val = REG_SPI_STATUS;
			asm("nop");
			if (p_status->txBufTFull_n)
			{
				break;
			}
		}
	}

	#else
	U32 int fifo_length_cnt = byte_size/SPI_FIFO_LENGTH_SIZE;

	for (j=0; j<fifo_length_cnt; j++)
	{
		REG_SPI_MAX_TRANS_NUM = SPI_FIFO_LENGTH_SIZE;
		REG_SPI_RXTX_NUM = SPI_FIFO_LENGTH_SIZE;

		for (i=0; i<SPI_FIFO_LENGTH_SIZE; i++)
		{
			REG_SPI_TX_DATA = *buf++;
		}

		while(1)
		{
			val = REG_SPI_STATUS;
			asm("nop");
			if ( p_status->txBufTFull_n)
			{
				break;
			}		
		}
	}

	U32 remainder = byte_size%SPI_FIFO_LENGTH_SIZE;
	if (remainder > 0)
	{
		REG_SPI_MAX_TRANS_NUM = remainder;
		REG_SPI_RXTX_NUM = remainder;

		for (i=0; i<remainder; i++)
		{
			REG_SPI_TX_DATA = *buf++;
		}
	}
	#endif

	#ifdef RTOS
	ret = OS_MUTEX_SPI_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex unLock err\n\r");
	}
	#endif
}



//bit[15] -> bit[0]
void Spi_Write_Bits(U16 data)
{
	U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *) &val;

	#ifdef RTOS
	ret = OS_MUTEX_SPI_LOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex Lock err\n\r");
	}
	#endif

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	REG_SPI_MAX_TRANS_NUM = 1;
	REG_SPI_RXTX_NUM = 1;
	asm("nop");
	REG_SPI_TX_DATA = data;

	//delayus(100); //at least 75us

	#ifdef RTOS
	ret = OS_MUTEX_SPI_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex unLock err\n\r");
	}
	#endif
}

//bit[32] -> bit[0]
void Spi_Write_32Bits(U32 data)
{
	U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *) &val;

	#ifdef RTOS
	ret = OS_MUTEX_SPI_LOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex Lock err\n\r");
	}
	#endif

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	while(1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if((p_status->txBufTFull_n))
		{
			break;
		}		
	}

	REG_SPI_MAX_TRANS_NUM = 1;
	asm("nop");
	REG_SPI_TX_DATA = data;
	
	//delayus(100); //at least 75us

	#ifdef RTOS
	ret = OS_MUTEX_SPI_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex unLock err\n\r");
	}
	#endif
}

void Spi_Read_Bytes(U8 *buf, U32 bytes_count)
{
	U32 val;
//	SPI_CONTROLs *p_con = (SPI_CONTROLs *)&val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
	unsigned int i, j;
	unsigned int fifo_length_cnt;
	unsigned short int remainder;
	U16 temp;
	U8 tx_len;

	#ifdef RTOS
	ret = OS_MUTEX_SPI_LOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex Lock err\n\r");
	}
	#endif

	#if 0
	val = REG_SPI_CONTROL;
	p_con->bits = SPI_WORD_8BITS;
	REG_SPI_CONTROL = val;
	#endif

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	fifo_length_cnt = bytes_count/SPI_FIFO_LENGTH_SIZE;
	for (j=0; j<fifo_length_cnt; j++)
	{
		tx_len = 1;
		REG_SPI_RXTX_NUM = tx_len;
		REG_SPI_MAX_TRANS_NUM = SPI_FIFO_LENGTH_SIZE + tx_len;

		for (i=0; i<tx_len; i++)
		{
			while(1)
			{
				val = REG_SPI_STATUS;
				asm("nop");
				if ( p_status->txBufTFull_n)
				{
					break;
				}		
			}

			#if 1
			REG_SPI_TX_DATA = 0xFf;
			#else
			REG_SPI_TX_DATA = *spi_tx_buf;
			#endif
		}

		for (i=0; i<SPI_FIFO_LENGTH_SIZE; i++)
		{
			while(1)
			{
				val = REG_SPI_STATUS;
				asm("nop");
				if (p_status->rxBufEmpty_n)
				{
					break;
				}		
			}

			*buf++ = REG_SPI_RX_DATA;
		}
	}

	remainder = bytes_count%SPI_FIFO_LENGTH_SIZE;
	if (remainder > 0)
	{
		tx_len = 0;
		REG_SPI_RXTX_NUM = tx_len;
		REG_SPI_MAX_TRANS_NUM = remainder + tx_len;

		for (i=0; i<tx_len; i++)
		{
			while(1)
			{
				val = REG_SPI_STATUS;
				asm("nop");
				if ( p_status->txBufTFull_n)
				{
					break;
				}		
			}

			#if 1
			REG_SPI_TX_DATA = 0x55;
			#else
			REG_SPI_TX_DATA = *spi_tx_buf;
			#endif
		}

		for (i=0; i<remainder; i++)
		{
			while(1)
			{
				val = REG_SPI_STATUS;
				asm("nop");
				if (p_status->rxBufEmpty_n)
				{
					break;
				}		
			}

			*buf++ = REG_SPI_RX_DATA;
		}
	}

	//asm("nop");

	#ifdef RTOS
	ret = OS_MUTEX_SPI_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex unLock err\n\r");
	}
	#endif
}

void Spi_Page_Read(U8 addr, U8 *buf, U32 bytes_count)
{
	U32 val;
//	SPI_CONTROLs *p_con = (SPI_CONTROLs *)&val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
	unsigned int i, j;
	unsigned int fifo_length_cnt;
	unsigned short int remainder;
	U16 temp;
	U8 tx_len;
	U8 int_flag = FALSE;

	#ifdef RTOS
	ret = OS_MUTEX_SPI_LOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex Lock err\n\r");
	}
	#endif

	#if 0
	val = REG_SPI_CONTROL;
	p_con->bits = SPI_WORD_8BITS;
	REG_SPI_CONTROL = val;
	#endif

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	fifo_length_cnt = bytes_count/SPI_FIFO_LENGTH_SIZE;
	for (j=0; j<fifo_length_cnt; j++)
	{
		tx_len = 1;
		REG_SPI_RXTX_NUM = tx_len;
		REG_SPI_MAX_TRANS_NUM = SPI_FIFO_LENGTH_SIZE + tx_len;
		int_flag = TRUE;

		for (i=0; i<tx_len; i++)
		{
			while(1)
			{
				val = REG_SPI_STATUS;
				asm("nop");
				if ( p_status->txBufTFull_n)
				{
					break;
				}		
			}

			REG_SPI_TX_DATA = addr;
		}

		for (i=0; i<SPI_FIFO_LENGTH_SIZE; i++)
		{
			while(1)
			{
				val = REG_SPI_STATUS;
				asm("nop");
				if (p_status->rxBufEmpty_n)
				{
					break;
				}		
			}

			*buf++ = REG_SPI_RX_DATA;
		}
	}

	remainder = bytes_count%SPI_FIFO_LENGTH_SIZE;
	if (remainder > 0)
	{
		tx_len = 1;
		REG_SPI_RXTX_NUM = tx_len;
		REG_SPI_MAX_TRANS_NUM = remainder + tx_len;

		if (int_flag == FALSE)
		{		
			for (i=0; i<tx_len; i++)
			{
				while(1)
				{
					val = REG_SPI_STATUS;
					asm("nop");
					if ( p_status->txBufTFull_n)
					{
						break;
					}		
				}

				REG_SPI_TX_DATA = addr;
			}

	       for(i = 0;i< 10;i++)
		  	{}
		  delayus(1);
		}

		for (i=0; i<remainder; i++)
		{
			while(1)
			{
				val = REG_SPI_STATUS;
				asm("nop");
				if (p_status->rxBufEmpty_n)
				{
					break;
				}		
			}

			*buf++ = REG_SPI_RX_DATA;
		}
	}

	//asm("nop");

	#ifdef RTOS
	ret = OS_MUTEX_SPI_UNLOCK;
	if (ret == FALSE)
	{
		DBG_Assert(FALSE);
		DBG_Puts ("SPI Mutex unLock err\n\r");
	}
	#endif
}

void spi_transfer_size_set(U8 bits, U8 cs_select)
{
	hw_spi_bits_set(bits);
	hw_spi_cs_select_set(cs_select);
}


#ifdef SPI_INT_USE
void spi_int_handler_fromIsr(void) __INTERNAL_RAM_TEXT;
void spi_int_handler_fromIsr(void)
{
	U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
	unsigned int i, j;
	int spi_tx_num;
	int temp;

	#ifdef SPI_RX_INT_ENABLE
	if (p_status->rxBufEmpty_n)
	{
		//rx
		DBG_iPuts ("spi int rx\n\r");

		#ifdef SPI_STATUS_IDLE_POLLING
		while (1)
		{
			val = REG_SPI_STATUS;
			asm("nop");
			if ( (p_status->idle) && (p_status->txBufEmpty) ) 
			//if ( (p_status->idle) && (p_status->rxBufEmpty_n) )
			{
				break;
			}
		}
		#else
		delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
		#endif //SPI_STATUS_IDLE_POLLING

		//spi_rx_len = size;
		REG_SPI_RXTX_NUM = 1;
		spi_tx_num = 1;
		REG_SPI_MAX_TRANS_NUM = spi_tx_num + spi_rx_len;

		if (spi_rx_len > 0)
		{
			for (i=0; i<spi_tx_num; i++)
			{
				while(1)
				{
					val = REG_SPI_STATUS;
					asm("nop");
					if ( p_status->txBufTFull_n)
					{
						break;
					}		
				}

				#if 1
				REG_SPI_TX_DATA = 0x55;
				#else
				REG_SPI_TX_DATA = *spi_tx_buf;
				#endif
			}

			for (i=0; i<spi_rx_len; i++)
			{
				while(1)
				{
					val = REG_SPI_STATUS;
					asm("nop");
					if (p_status->rxBufEmpty_n)
					{
						break;
					}		
				}

				temp = REG_SPI_RX_DATA;
			}
		}
	}
	#endif //SPI_RX_INT_ENABLE


	#ifdef SPI_TX_INT_ENABLE
	if (p_status->txBufTFull_n)
	{
		//tx
		DBG_iPuts ("spi int tx\n\r");
	}
	#endif
}

#ifdef SPI_RX_INT_ENABLE
void Spi_Read_Bytes_Int_Start(U32 rx_size)
{
	U32 val;
//	SPI_CONTROLs *p_con = (SPI_CONTROLs *)&val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
	unsigned int i, j;
	int spi_tx_num;

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	spi_rx_len = rx_size;
	REG_SPI_RXTX_NUM = 1;
	spi_tx_num = 1;
	REG_SPI_MAX_TRANS_NUM = spi_rx_len + spi_tx_num;

	if (rx_size > 0)
	{
		for (i=0; i<spi_tx_num; i++)
		{
			while(1)
			{
				val = REG_SPI_STATUS;
				asm("nop");
				if ( p_status->txBufTFull_n)
				{
					break;
				}		
			}

			#if 1
			REG_SPI_TX_DATA = 0x55;
			#else
			REG_SPI_TX_DATA = *spi_tx_buf;
			#endif
		}
	}
}
#endif //SPI_RX_INT_ENABLE

#endif //SPI_INT_USE



#ifdef SPI_DMA_ENABLE

#ifdef SPI_DMA_TX_ENABLE
void spi_dma_write_bytes(U32 *p_data, U32 size) __INTERNAL_RAM_TEXT;
void spi_dma_write_bytes(U32 *p_data, U32 size)
{
    hal_dma_send_data(SPI_TX_DMA_ID, p_data, size);
}

void spi_dma_tx_start(U32 *p_data, U32 size)
{
	U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
//	int index;
	int byte_length;
	U32 *buf;

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	byte_length = size;
	REG_SPI_MAX_TRANS_NUM = byte_length;
	REG_SPI_RXTX_NUM = byte_length;
    hal_dma_send_data(SPI_TX_DMA_ID, p_data, byte_length);
}

void spi_dma_tx_start_with_address(U8 addr, U32 *p_data, U32 size)
{
	U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
//	int index;
	int byte_length;
	U32 *buf;
	TX_INTERRUPT_SAVE_AREA;

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	byte_length = size;
	REG_SPI_MAX_TRANS_NUM = byte_length+1;
	REG_SPI_RXTX_NUM = byte_length+1;

	//disable int
	TX_DISABLE;
	REG_SPI_TX_DATA = addr;
    hal_dma_send_data(SPI_TX_DMA_ID, p_data, byte_length);
	//enable int
	TX_RESTORE;
}

#ifdef SPI_DMA_TX_TEST
void spi_dma_tx_callback(void) __INTERNAL_RAM_TEXT;
void spi_dma_tx_callback(void)
{
	U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
	int index;
	int byte_length;
	U32 *buf;

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	byte_length = spi_tx_len;
	REG_SPI_MAX_TRANS_NUM = byte_length;
	REG_SPI_RXTX_NUM = byte_length;
	index = spi_tx_buf_index;
    hal_dma_send_data(SPI_TX_DMA_ID, spi_tx_buf[index], byte_length);
	spi_tx_buf_index = !spi_tx_buf_index;
}
#endif

#endif //SPI_DMA_TX_ENABLE


#ifdef SPI_DMA_RX_ENABLE
void spi_dma_read_bytes(U32 *p_data, U32 size) __INTERNAL_RAM_TEXT;
void spi_dma_read_bytes(U32 *p_data, U32 size)
{
	U32 val;
	SPI_STATUSs *p_status = (SPI_STATUSs *)&val;
	int i;
	int spi_tx_num;

	REG_SPI_RXTX_NUM = 1;
	spi_tx_num = 1;
	REG_SPI_MAX_TRANS_NUM = size + spi_tx_num;

	#ifdef SPI_STATUS_IDLE_POLLING
	while (1)
	{
		val = REG_SPI_STATUS;
		asm("nop");
		if ( (p_status->idle) && (p_status->txBufEmpty) ) 
		{
			break;
		}
	}
	#else
	delayus(11); //delay 2.5us at CPU_FREQ_BASE_32X
	#endif //SPI_STATUS_IDLE_POLLING

	if (size > 0)
	{
		for (i=0; i<spi_tx_num; i++)
		{
			while(1)
			{
				val = REG_SPI_STATUS;
				asm("nop");
				if ( p_status->txBufTFull_n)
				{
					break;
				}		
			}

			#if 1
			REG_SPI_TX_DATA = 0x55;
			#else
			REG_SPI_TX_DATA = *spi_tx_buf;
			#endif
		}

		hal_dma_receive_data(SPI_RX_DMA_ID, p_data, size);
	}
}

#ifdef SPI_DMA_RX_TEST
void spi_dma_rx_callback(void) __INTERNAL_RAM_TEXT;
void spi_dma_rx_callback(void)
{
	int index;
	int byte_length;
	U32 *buf;

 	byte_length = spi_rx_len;
	spi_rx_buf_index = !spi_rx_buf_index;
    index = spi_rx_buf_index;
    buf = spi_rx_buf[index];
	spi_dma_read_bytes(buf, byte_length);
}
#endif

#endif //SPI_DMA_RX_ENABLE


int32_t spi_dma_open(int32_t tx_ena, int32_t rx_ena, dma_done_callback *tx_cb, dma_done_callback * rx_cb)
{
#ifdef SPI_DMA_ENABLE
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *)&REG_MISC_CLKGATE;
	int i;

	pGate->spi_gate = 1; //open clk gate

	if (tx_ena)
	{
    	//hal_dma_transfer_init(SPI_TX_DMA_ID, (U32 *)(&REG_SPI_TX_DATA), NULL, SOURCE_DMA_SPI_TX, DMA_TRANS_1BYTE, DMA_BURST_SINGLE, 1, 0, 1, spi_dma_tx_callback);
    	hal_dma_transfer_init(SPI_TX_DMA_ID, (U32 *)(&REG_SPI_TX_DATA), NULL, SOURCE_DMA_SPI_TX, DMA_TRANS_1BYTE, DMA_BURST_SINGLE, 1, 0, 1, tx_cb);
		DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(SPI_TX_DMA_ID)));
	}

	if (rx_ena)
	{
		//hal_dma_transfer_init(SPI_RX_DMA_ID, NULL, (U32 *)(&REG_SPI_RX_DATA), SOURCE_DMA_SPI_RX, DMA_TRANS_1BYTE, DMA_BURST_SINGLE, 0, 1, 1, spi_dma_rx_callback);
		hal_dma_transfer_init(SPI_RX_DMA_ID, NULL, (U32 *)(&REG_SPI_RX_DATA), SOURCE_DMA_SPI_RX, DMA_TRANS_1BYTE, DMA_BURST_SINGLE, 0, 1, 1, rx_cb);
		DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(SPI_RX_DMA_ID)));
	}

    xt_ints_on(LEVEL3_INT9_MASK);

    return 0;
#endif
}

#endif //SPI_DMA_ENABLE



#ifdef SPI_TEST
void spi_test(void)
{
	int i, j;
	int tx_len;
	int rx_len;

	#if 1
	if (TX_BUF_LEN >= 4)
	{
		for (j=0; j<2; j++)
		{
			for (i=0; i<(TX_BUF_LEN/4); i++)
			{
				spi_tx_buf[j][i*4+0] = 0x00;
				spi_tx_buf[j][i*4+1] = 0x01;
				spi_tx_buf[j][i*4+2] = 0x02;
				spi_tx_buf[j][i*4+3] = 0x03;
			}
		}
	}
	#else
	memset(spi_tx_buf, 0xaa, sizeof(spi_tx_buf));
	#endif

	memset(spi_rx_buf, 0, sizeof(spi_rx_buf));
	//memset(spi_rx_buf, 0xFF, sizeof(spi_rx_buf));

	hw_spi_mode_set(SPI_MODE_0);
	hw_spi_cs_select_set(0);
	hw_spi_bits_set(8);

	asm("nop");


	#ifdef SPI_RX_INT_ENABLE
	Spi_Read_Bytes_Int_Start(RX_BUF_LEN);
	#else
	while (1)
	{
		asm("nop");

		#ifdef SPI_TX_TEST
		//tx_len = TX_BUF_LEN;
		tx_len = 4;
		Spi_Write_Bytes(spi_tx_buf[0], tx_len);
		delayms(10);
		#elif defined SPI_RX_TEST
		//rx_len = RX_BUF_LEN;
		rx_len = 4;
		Spi_Read_Bytes(spi_rx_buf[0], rx_len);
		delayms(10);
		#endif
	}
	#endif
}
#endif //SPI_TEST


#ifdef SPI_DMA_TX_TEST
void spi_dma_tx_test(void)
{
	int i, j;

	#if 1
	for (j=0; j<1; j++)
	{
		if (TX_BUF_LEN >= 4)
		{
			for (i=0; i<(TX_BUF_LEN/4); i++)
			{
				spi_tx_buf[j][i*4+0] = 0x00;
				spi_tx_buf[j][i*4+1] = 0x01;
				spi_tx_buf[j][i*4+2] = 0x02;
				spi_tx_buf[j][i*4+3] = 0x03;
			}
		}
	}

	for (j=1; j<2; j++)
	{
		if (TX_BUF_LEN >= 4)
		{
			for (i=0; i<(TX_BUF_LEN/4); i++)
			{
				spi_tx_buf[j][i*4+0] = 0x03;
				spi_tx_buf[j][i*4+1] = 0x02;
				spi_tx_buf[j][i*4+2] = 0x01;
				spi_tx_buf[j][i*4+3] = 0x00;
			}
		}
	}
	#else
	memset(spi_tx_buf, 0xaa, sizeof(spi_tx_buf));
	#endif

	//memset(spi_rx_buf, 0, sizeof(spi_rx_buf));
	memset(spi_rx_buf, 0xFF, sizeof(spi_rx_buf));

	spi_dma_open(1, 0, spi_dma_tx_callback, NULL);

	REG_SPI_MAX_TRANS_NUM = spi_tx_len;
	REG_SPI_RXTX_NUM = spi_tx_len;
	spi_dma_write_bytes(spi_tx_buf, spi_tx_len);
}
#endif //SPI_DMA_TX_TEST

#ifdef SPI_DMA_RX_TEST
void spi_dma_rx_test(void)
{
	int i, j;

	#if 1
	for (j=0; j<1; j++)
	{
		if (TX_BUF_LEN >= 4)
		{
			for (i=0; i<(TX_BUF_LEN/4); i++)
			{
				spi_tx_buf[j][i*4+0] = 0x00;
				spi_tx_buf[j][i*4+1] = 0x01;
				spi_tx_buf[j][i*4+2] = 0x02;
				spi_tx_buf[j][i*4+3] = 0x03;
			}
		}
	}

	for (j=1; j<2; j++)
	{
		if (TX_BUF_LEN >= 4)
		{
			for (i=0; i<(TX_BUF_LEN/4); i++)
			{
				spi_tx_buf[j][i*4+0] = 0x03;
				spi_tx_buf[j][i*4+1] = 0x02;
				spi_tx_buf[j][i*4+2] = 0x01;
				spi_tx_buf[j][i*4+3] = 0x00;
			}
		}
	}
	#else
	memset(spi_tx_buf, 0xaa, sizeof(spi_tx_buf));
	#endif

	//memset(spi_rx_buf, 0, sizeof(spi_rx_buf));
	memset(spi_rx_buf, 0xFF, sizeof(spi_rx_buf));

	spi_dma_open(0, 1, NULL, spi_dma_rx_callback);

	spi_tx_len = 4;
	spi_rx_len = 4;

	REG_SPI_MAX_TRANS_NUM = spi_rx_len;
	REG_SPI_RXTX_NUM = spi_rx_len;

	spi_dma_read_bytes(spi_rx_buf, spi_rx_len);
}
#endif //SPI_DMA_RX_TEST

#endif //SPI_ENABLE
