#include "os_config.h"

#ifdef CEC_ENABLE
#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hw_cec.h"
#include "hal_gpio.h"

#include "interrupt.h"
#include "fifo.h"

#include "app_task.h"
#include "optek_cec_cmd.h"


//#define ARC_CEC_TX_BUF_CHECK_OUT

//#define ARC_CEC_RX_INT_CHECK


U8 cec_rx_buf[64];
U8 cec_tx_buf[16];

FIFO cec_rx_fifo;

extern FIFO cec_rx_fifo; 

U8 cec_rx_int_index;

void hw_cec_init( void )
{
	U32 val;
	MISC_CLKGATE_CONTROLs *pGate;
	GPIO_PINs *p_gpip_pin;
	MISC_AON_CONTROLs *pRtcCntl;
	MISC_CFG_CONTROLs *pCfgCntl;
	HDMICEC_CONTROLs *pHdmiCecCntl;

	DBG_Printf("%s\n\r", __func__);

	hal_gpio_mux_func_select( GPIO0_13_INDEX, MUX_SEL_FUNCTION1);
	/*
	*GPIO0_06_INDEX as 32kHz Output pin
	hal_gpio_mux_func_select( GPIO0_06_INDEX, MUX_SEL_FUNCTION4);
	*/

	val = REG_MISC_CLKGATE;
    MISC_CLKGATE_CONTROLs *pGate1 = (MISC_CLKGATE_CONTROLs *) &val;
    pGate1->i2c_gate = 1;
    pGate1->hdmi_cec_gate = 1;
	REG_MISC_CLKGATE = val;

	val = REG_MISC_RTC;
	MISC_AON_CONTROLs *pRtcCntl1 = (MISC_AON_CONTROLs *) &val;
	pRtcCntl1->aon_en = 0;
	REG_MISC_RTC = val;

	val = REG_MISC_CFG;
	MISC_CFG_CONTROLs *pCfgCntl1 = (MISC_CFG_CONTROLs *) &val;
	pCfgCntl1->lp_clk_sel = 1;
	REG_MISC_CFG = val;

	val = REG_HDMICEC_CONTRL;
	HDMICEC_CONTROLs *pHdmiCecCntl1 = (HDMICEC_CONTROLs *) &val;
	pHdmiCecCntl1->laddr = 0x05;
	pHdmiCecCntl1->enable = 1;
	REG_HDMICEC_CONTRL = val;

	/*
	*pMiscBtdmCntl2->bt_clk2lp32k = 1
	*32K CLK from btdm clk 16mHz/488
	*pMiscBtdmCntl2->bt_clk2lp32k = 0
	*32K CLK from aon 32k
	*/
	val = REG_MISC_BTDM_2;
	MISC_BTDM_2_CONTROLs *pMiscBtdmCntl2 = (MISC_BTDM_2_CONTROLs *) &val;
	pMiscBtdmCntl2->bt_clk2lp32k = 1;
	REG_MISC_BTDM_2 = val;

	
	fifo_init(&cec_rx_fifo, cec_rx_buf, sizeof(cec_rx_buf), 1);
	memset(cec_rx_buf, 0, sizeof(cec_rx_buf));
	memset(cec_tx_buf, 0, sizeof(cec_tx_buf));
	cec_rx_int_index = 0;

	REG_HDMICEC_INT_ENA = (0x1<<4);
	REG_INT_ENABLE(CEC_INT_MASK);

	XT_INTS_ON(LEVEL1_INT1_MASK);
}

void hw_cec_interrupt_set(control)
{
	if (control)
	{
		REG_INT_ENABLE(CEC_INT_MASK);
	}
	else
	{
		REG_INT_ENABLE(~CEC_INT_MASK);
	}
}

/**
 *****************************************************************************************
 * @brief read status of specific gpio pin.
 *
 * @param[in] mask   bit in REG of specific gpio pin
 *
 * @return 0-low 1-high
 *
 *****************************************************************************************
 */

void hw_cec_tx(U8 *buf, U8 len)
{
	int i;
	U32 val;
	HDMICEC_STATUSs *pHdmiCecStatus = (HDMICEC_STATUSs *)&REG_HDMICEC_STATUS;

	DBG_Printf ("cec tx\n\r");

	while (1)
	{
		val = REG_HDMICEC_STATUS;
		if(pHdmiCecStatus->txEmpty)
		{
			break;
		}
	}

	while(1)
	{
		val = REG_HDMICEC_STATUS;
		if(pHdmiCecStatus->idle)
		{
			delayms(10);
			val = REG_HDMICEC_STATUS;
			asm("nop");
			if(pHdmiCecStatus->idle)
			{
				break;
			}
		}
	}
	
	for(i=0; i<len; i++)
	{
		while (1)
		{
			val = REG_HDMICEC_STATUS;
			asm("nop");
			if(pHdmiCecStatus->txEmpty)
			{
				break;
			}
		}

		if (i == (len - 1))
		{
			//the end of a frame
			REG_HDMICEC_TX_DATA = (buf[i] << 1) | 1;
		}
		else
		{
			REG_HDMICEC_TX_DATA = (buf[i] << 1) | 0;
		}

		if (pHdmiCecStatus->txAckErr)
		{
			DBG_Printf ("txAckEr\n\r");

			#ifdef ARC_CEC_TX_BUF_CHECK_OUT
			{
				if (i == 0)
				{
					DBG_Printf ("TxData:%x ", buf[i]);
				}
				else if (i == (len-1))
				{
					DBG_Printf ("%x\n\r", buf[i]);
				}
				else
				{
					DBG_Printf ("%x ", buf[i]);
				}
			}
			#endif //ARC_CEC_TX_BUF_CHECK_OUT
		}
	}
}


/**
 *****************************************************************************************
 * @brief read status of specific gpio pin.
 *
 * @param[in] mask   bit in REG of specific gpio pin
 *
 * @return 0-low 1-high
 *
 *****************************************************************************************
 */

U8 cec_rx_int_buf[16];

void hw_cec_rx_fromIsr(void) __INTERNAL_RAM_TEXT;

void hw_cec_rx_fromIsr(void)
{
	U32 val;
	HDMICEC_STATUSs *pHdmiCecStatus;
	pHdmiCecStatus = (HDMICEC_STATUSs *) &val;

	#ifdef ARC_CEC_RX_INT_CHECK
	DBG_iPuts ("cec rx\n\r");
	#endif

	while(1)
	{
		val = REG_HDMICEC_STATUS;
		if (!pHdmiCecStatus->rxEmpty_n)
		{
			break;
		}

		val = REG_HDMICEC_RX_DATA;

		if (cec_rx_int_index > 16)
		{
			cec_rx_int_index = 0;
		}
		cec_rx_int_buf[cec_rx_int_index++] = (val >> 1);
		
		if (val & 0x1)
		{
			fifo_put_data_one_byte(&cec_rx_fifo, &cec_rx_int_index);
			
			fifo_put_data(&cec_rx_fifo, cec_rx_int_buf, cec_rx_int_index);

			#ifndef CEC_CMD_RX_POLLING_ENABLE
			func_w_one_param_msgSend_formIsr(cec_cmd_rx_handle, cec_rx_int_index);
			#endif
			
			cec_rx_int_index = 0;
		}
	}
}

#endif //CEC_ENABLE
