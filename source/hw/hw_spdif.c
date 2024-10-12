#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hw_spdif.h"
#include "hal_gpio.h"


#ifdef SPDIF_ENABLE

void hw_spdif_init(int channel)
{
	U32 val;
	MISC_CPUCLK5_CONTROLs *pFracCLK_5 = (MISC_CPUCLK5_CONTROLs *) &val;
	SPDIF_CONFIG_AHB *pSpdifConfig = (SPDIF_CONFIG_AHB *) &val;
	GPIO_PINs *p_gpip_pin = (GPIO_PINs *) &val;

	#ifdef AUDIO_OUTPUT_FROM_SPDIF
    val = REG_GPIO4_FUNC;
	p_gpip_pin->bit_20 = !(!(1<<(GPIO0_20_INDEX-GPIO0_00_INDEX))); // SPDIF OUT
	REG_GPIO4_FUNC = val;
	#endif

	val = REG_MISC_CPUCLK_5;
	pFracCLK_5->pllFrac_spdif_sel = pllFrac_sel_FOUT4;
	REG_MISC_CPUCLK_5 = val;

	val = REG_MISC_CPUCLK_5;
	pFracCLK_5->pllFrac_spdif_dec_n = 2;
	pFracCLK_5->pllFrac_spdif_dec_en = 1;
	REG_MISC_CPUCLK_5 = val;

	val = REG_MISC_CPUCLK_5;
	val = SPDIF_CONFIG;

	pSpdifConfig->spdif_dec_enable = 1;
	pSpdifConfig->spdif_data_sel = 0;
	pSpdifConfig->spdif_dec_inv = 0;
	pSpdifConfig->spdif_dec_sr_detect_start = 0;
	pSpdifConfig->spdif_dec_aux_mask = 0; //only audio data
	pSpdifConfig->sync_err_auto_recover = 0;
	#ifdef I2S_TDM_DATA_WIDTH_24_BIT
	pSpdifConfig->spdif_dec_data_24b = 1; //24-bit
	#else
	pSpdifConfig->spdif_dec_data_24b = 0; //16-bit
	#endif

	
	//spdif dec fifo threshold, should be > dma bust size + margin(2)
	#ifdef I2S_TDM_DATA_WIDTH_24_BIT
	pSpdifConfig->spdif_fifo_rd_th	= 4;
	#else
	pSpdifConfig->spdif_fifo_rd_th	= 10;
	#endif
	pSpdifConfig->spdif_dec_clk_x4	= 1;
	#ifdef AUDIO_OUTPUT_FROM_SPDIF
	pSpdifConfig->spdif_enc_enable = 1; //enable spdif out
	#else
	pSpdifConfig->spdif_enc_enable = 0; //disable spdif out
	#endif
	pSpdifConfig->spdif_enc_inv = 0;
	SPDIF_CONFIG = val;
}

void hw_spdif_din_select(U16 sel)
{
	volatile U32 tmp;
	SPDIF_CONFIG_AHB *spdif_config;

	spdif_config = (SPDIF_CONFIG_AHB *) &tmp;
	tmp = SPDIF_CONFIG;

	if (sel == SPDIF_IN_CH_0)
	{
		hal_gpio_mux_func_select(SPDIF_IN_CH_0_PIN, MUX_SEL_GPIO_INPUT);

		spdif_config->spdif_data_sel = 0;
	}
	else if (sel == SPDIF_IN_CH_1)
	{
		hal_gpio_mux_func_select(SPDIF_IN_CH_1_PIN, MUX_SEL_GPIO_INPUT);
		spdif_config->spdif_data_sel = 1;
	}
	else
	{
		DBG_Assert(FALSE);
	}
	
	SPDIF_CONFIG = tmp;
}

void hw_spdif_open(int channel)
{
	if (channel == SPDIF_IN_CH_0)
	{
		hw_spdif_din_select(SPDIF_IN_CH_0);
	}
	else if (channel == SPDIF_IN_CH_1)
	{
		hw_spdif_din_select(SPDIF_IN_CH_1);
	}
	else
	{
		DBG_Assert(FALSE);
	}
}

void hw_spdif_close(int channel)
{
}

void hw_spdif_fini(void)
{
	volatile U32 tmp;
	SPDIF_CONFIG_AHB *spdif_config;
	
	tmp = SPDIF_CONFIG;
	spdif_config = (SPDIF_CONFIG_AHB *) &tmp;
	#ifndef AUDIO_OUTPUT_FROM_SPDIF
	spdif_config->spdif_enc_enable = 0;
	#endif
	spdif_config->spdif_dec_enable = 0;
	spdif_config->spdif_dec_sr_detect_start = 0;

	SPDIF_CONFIG = tmp;	
}


void hw_spdif_detect_level(u8 high)
{

}

void hw_spdif_dec_enable(void)
{
	volatile U32 tmp;
	SPDIF_CONFIG_AHB *spdif_config;
	tmp = SPDIF_CONFIG;
	spdif_config = (SPDIF_CONFIG_AHB *) &tmp;
	spdif_config->spdif_dec_enable = 1;
	SPDIF_CONFIG = tmp;
}

void hw_spdif_dec_disable(void)
{
	volatile U32 tmp;
	SPDIF_CONFIG_AHB *spdif_config;
	tmp = SPDIF_CONFIG;
	spdif_config = (SPDIF_CONFIG_AHB *) &tmp;
	spdif_config->spdif_dec_enable = 0;
	spdif_config->spdif_dec_sr_detect_start = 0;
	SPDIF_CONFIG = tmp;
}

void hw_spdif_sr_detect_enable(void)
{
	volatile U32 tmp;
	SPDIF_CONFIG_AHB *spdif_config;

	tmp = SPDIF_CONFIG;
	spdif_config = (SPDIF_CONFIG_AHB *) &tmp;
	spdif_config->spdif_dec_enable = 1;
	spdif_config->spdif_dec_sr_detect_start = 1;

	SPDIF_CONFIG = tmp;
}

void hw_spdif_sr_detect_disable(void)
{
	volatile U32 tmp;
	SPDIF_CONFIG_AHB *spdif_config;

	tmp = SPDIF_CONFIG;
	spdif_config = (SPDIF_CONFIG_AHB *) &tmp;
	spdif_config->spdif_dec_sr_detect_start = 0;

	SPDIF_CONFIG = tmp;
}

void hw_spdif_output_set(U8 con)
{
	volatile U32 tmp;
	SPDIF_CONFIG_AHB *spdif_config = (SPDIF_CONFIG_AHB *) &tmp;

	tmp = SPDIF_CONFIG;

	spdif_config = (SPDIF_CONFIG_AHB *) &tmp;
	if (spdif_config->spdif_enc_enable != con)
	{
		spdif_config->spdif_enc_enable = con;
	}

	SPDIF_CONFIG = tmp;
}

#endif //SPDIF_ENABLE
