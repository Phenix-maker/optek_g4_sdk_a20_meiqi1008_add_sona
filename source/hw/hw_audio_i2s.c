#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hw_audio_pwm_pdm.h"
#include "hw_audio_i2s.h"
#include "hal_dma.h"


#define IIS_OUTPUT_CHANNEL_NUM        AUDIO_OUT_STREAM_CHANNEL


void hw_audio_mi2s_rx_start(void)
{
	U32 val;
	MI2S_RX_CONFIG_AHB *pMi2sRx = (MI2S_RX_CONFIG_AHB *) &val;

	DBG_Printf("mi2s rx start\n\r");

	val = MI2S_RX_CONFIG;

	pMi2sRx->mi2s_enable = 1;
	MI2S_RX_CONFIG = val;
}

void hw_audio_mi2s_rx_start_from_isr(void)
{
	U32 val;
	MI2S_RX_CONFIG_AHB *pMi2sRx = (MI2S_RX_CONFIG_AHB *) &val;

	DBG_iPrintf("mi2s rx start from isr\n\r");

	val = MI2S_RX_CONFIG;

	pMi2sRx->mi2s_enable = 1;
	MI2S_RX_CONFIG = val;
}

void hw_audio_mi2s_rx_stop(void)
{
	U32 val;
	MI2S_RX_CONFIG_AHB *pMi2sRx = (MI2S_RX_CONFIG_AHB *) &val;

	DBG_Printf("mi2s rx stop\n\r");

	val = MI2S_RX_CONFIG;

	pMi2sRx->mi2s_enable = 0;
	MI2S_RX_CONFIG = val;
}

void hw_audio_mi2s_rx_stop_from_isr(void)
{
	U32 val;
	MI2S_RX_CONFIG_AHB *pMi2sRx = (MI2S_RX_CONFIG_AHB *) &val;

	DBG_iPrintf("i2s_rx_stop_isr\n\r");

	val = MI2S_RX_CONFIG;

	pMi2sRx->mi2s_enable = 0;
	MI2S_RX_CONFIG = val;
}

void hw_audio_mi2s_rx_init(void)
{
	U32 val;
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &val;
	GPIO_PINs *p_gpip_pin = (GPIO_PINs *) &val;
	MI2S_RX_CONFIG_AHB *pMi2sRx = (MI2S_RX_CONFIG_AHB *) &val;
	I2S_ST_CONFIG_AHB *pI2sSt = (I2S_ST_CONFIG_AHB *) &val;
	I2S_TDM_CONFIG_CDEF_AHB	*pTDM = (I2S_TDM_CONFIG_CDEF_AHB *) &val;
	CLASSD_CONFIG_AHB *classd_config_ahb = (CLASSD_CONFIG_AHB *) &val;
	CLASSD_CONFIG_AHB_2 *classd_config_ahb_2 = (CLASSD_CONFIG_AHB_2 *) &val;

	// clk control 
	val = REG_MISC_CLKGATE;
	pGate->AHB_Audio_Controller_gate = 1;		
	REG_MISC_CLKGATE = val;	
	REG_GPIO0_FUNC = 0;

	//i/o
	/* i2s0 ping assignment */
    val = REG_GPIO0_FUNC;
	#if ((defined AUDIO_INPUT_FROM_IIS0)||(defined AUX_INPUT_FROM_IIS0))
	#if (AUDIO_IN_STREAM_CHANNEL >= 2)
	p_gpip_pin->bit_3 = !(!(1<<(GPIO0_03_INDEX-GPIO0_00_INDEX))); //i2s0 dout0
	#endif
	#if (AUDIO_IN_STREAM_CHANNEL >= 4)
	p_gpip_pin->bit_4 = !(!(1<<(GPIO0_04_INDEX-GPIO0_00_INDEX))); //i2s0 dout1
	#endif
	#if (AUDIO_IN_STREAM_CHANNEL >= 6)
	p_gpip_pin->bit_5 = !(!(1<<(GPIO0_05_INDEX-GPIO0_00_INDEX))); //i2s0 dout2
	#endif
	#if (AUDIO_IN_STREAM_CHANNEL >= 8)
	p_gpip_pin->bit_6 = !(!(1<<(GPIO0_06_INDEX-GPIO0_00_INDEX))); //i2s0 dout3
	#endif
	p_gpip_pin->bit_7 = !(!(1<<(GPIO0_07_INDEX-GPIO0_00_INDEX))); //i2s0 bck
	p_gpip_pin->bit_8 = !(!(1<<(GPIO0_08_INDEX-GPIO0_00_INDEX))); //i2s0 lrck
	#endif
	#ifdef IIS_MCLK_OUTPUT_GPIO0
	p_gpip_pin->bit_0 = !(!(1<<(GPIO0_00_INDEX-GPIO0_00_INDEX))); //mclk out
	#else
	p_gpip_pin->bit_0 = (!(1<<(GPIO0_00_INDEX-GPIO0_00_INDEX))); //mclk out
	#endif

	#ifdef AUDIO_OUTPUT_FROM_PWM
	#if (AUDIO_OUT_STREAM_CHANNEL >= 2)
	p_gpip_pin->bit_24 = !(!(1<<(GPIO0_24_INDEX-GPIO0_00_INDEX))); //APWM1+
	p_gpip_pin->bit_25 = !(!(1<<(GPIO0_25_INDEX-GPIO0_00_INDEX))); //APWM1-
	p_gpip_pin->bit_26 = !(!(1<<(GPIO0_26_INDEX-GPIO0_00_INDEX))); //APWM2+
	p_gpip_pin->bit_27 = !(!(1<<(GPIO0_27_INDEX-GPIO0_00_INDEX))); //APWM2-
	#endif
	#if (AUDIO_OUT_STREAM_CHANNEL >= 4)
	p_gpip_pin->bit_28 = !(!(1<<(GPIO0_28_INDEX-GPIO0_00_INDEX))); //APWM3+
	p_gpip_pin->bit_29 = !(!(1<<(GPIO0_29_INDEX-GPIO0_00_INDEX))); //APWM3-
	p_gpip_pin->bit_30 = !(!(1<<(GPIO0_30_INDEX-GPIO0_00_INDEX))); //APWM4+
	p_gpip_pin->bit_31 = !(!(1<<(GPIO0_31_INDEX-GPIO0_00_INDEX))); //APWM4-
	#endif
	#endif
	REG_GPIO0_FUNC = val;

	/* cancel ping assignment 
	*val = REG_GPIO0_FUNC;
	*p_gpip_pin->bit_12 = 0;	//uart2 rx
	*p_gpip_pin->bit_28 = 0;	//apwm3+
	*p_gpip_pin->bit_29 = 0;	//apwm3-
	*REG_GPIO0_FUNC = val;
	*/
	
	val = REG_GPIO3_FUNC;	
	#if ((defined AUDIO_OUTPUT_FROM_IIS1)||((defined AUDIO_INPUT_FROM_IIS1)||(defined AUX_INPUT_FROM_IIS1)))
	#if ((defined AUDIO_OUTPUT_FROM_IIS1)||(((defined AUDIO_INPUT_FROM_IIS1)||(defined AUX_INPUT_FROM_IIS1)) && (AUDIO_IN_STREAM_CHANNEL == 4)))
	p_gpip_pin->bit_20 = !(!(1<<(GPIO0_20_INDEX-GPIO0_00_INDEX))); //i2s1 dout
	#endif
	p_gpip_pin->bit_21 = !(!(1<<(GPIO0_21_INDEX-GPIO0_00_INDEX))); //i2s1 bck
	p_gpip_pin->bit_22 = !(!(1<<(GPIO0_22_INDEX-GPIO0_00_INDEX))); //i2s1 lclk
	#if ((defined AUDIO_INPUT_FROM_IIS1)||(defined AUX_INPUT_FROM_IIS1))
	p_gpip_pin->bit_23 = !(!(1<<(GPIO0_23_INDEX-GPIO0_00_INDEX))); //i2s1 din
	#endif
	#endif

	#ifdef AUDIO_OUTPUT_FROM_IIS2
	/* i2s2 ping assignment */
	#if (AUDIO_OUT_STREAM_CHANNEL == 16)
	p_gpip_pin->bit_19 = !(!(1<<(GPIO0_19_INDEX-GPIO0_00_INDEX))); //i2s2 d7
	#endif
	#if (AUDIO_OUT_STREAM_CHANNEL >= 14)
	p_gpip_pin->bit_18 = !(!(1<<(GPIO0_18_INDEX-GPIO0_00_INDEX))); //i2s2 d6
	#endif
	#if (AUDIO_OUT_STREAM_CHANNEL >= 12)
	p_gpip_pin->bit_24 = !(!(1<<(GPIO0_24_INDEX-GPIO0_00_INDEX))); //i2s2 d5
	#endif
	#if (AUDIO_OUT_STREAM_CHANNEL >= 10)
	p_gpip_pin->bit_25 = !(!(1<<(GPIO0_25_INDEX-GPIO0_00_INDEX))); //i2s2 d4
	#endif
	#if (AUDIO_OUT_STREAM_CHANNEL >= 8)
	p_gpip_pin->bit_26 = !(!(1<<(GPIO0_26_INDEX-GPIO0_00_INDEX))); //i2s2 d3
	#endif
	#if (AUDIO_OUT_STREAM_CHANNEL >= 6)
	p_gpip_pin->bit_27 = !(!(1<<(GPIO0_27_INDEX-GPIO0_00_INDEX))); //i2s2 d2
	#endif
	#if (AUDIO_OUT_STREAM_CHANNEL >= 4)
	// p_gpip_pin->bit_28 = !(!(1<<(GPIO0_28_INDEX-GPIO0_00_INDEX))); //i2s2 d1
	#endif
	#if (AUDIO_OUT_STREAM_CHANNEL >= 2)
	p_gpip_pin->bit_29 = !(!(1<<(GPIO0_29_INDEX-GPIO0_00_INDEX))); //i2s2 d0
	#endif
	p_gpip_pin->bit_30 = !(!(1<<(GPIO0_30_INDEX-GPIO0_00_INDEX))); //i2s2 lrck
	p_gpip_pin->bit_31 = !(!(1<<(GPIO0_31_INDEX-GPIO0_00_INDEX))); //i2s2 bck
	#endif

	#ifdef IIS_MCLK_OUTPUT_GPIO9
	p_gpip_pin->bit_9 = !(!(1<<(GPIO0_09_INDEX-GPIO0_00_INDEX))); //mclk out
	#else
	p_gpip_pin->bit_9 = (!(1<<(GPIO0_09_INDEX-GPIO0_00_INDEX))); //mclk out
	#endif

	#ifdef IIS_MCLK_INTPUT_GPIO17
	/*
	 * OTK6266D test board v1.0 use GPIO17 as arc_detect pin
	 */
	p_gpip_pin->bit_17 = !(!(1<<(GPIO0_17_INDEX-GPIO0_00_INDEX))); //enable mclk in
	#endif
	REG_GPIO3_FUNC = val;


	#ifdef AUDIO_INPUT_FROM_IIS_TDM
	val = I2S_TDM_CONFIG_CDEF;
	pTDM->mi2s_in_tdm_32b = 1;
	I2S_TDM_CONFIG_CDEF = val;
	#endif

	#if (((defined AUDIO_INPUT_FROM_IIS1)||(defined AUX_INPUT_FROM_IIS1)) && (AUDIO_IN_STREAM_CHANNEL == 4))
	val = I2S_TDM_CONFIG_CDEF;
	pTDM->i2s_stereo_data_in_2 = 1;
	I2S_TDM_CONFIG_CDEF = val;
	#endif

	#if ((defined AUDIO_INPUT_FROM_IIS1) || (defined AUDIO_OUTPUT_FROM_IIS1))
	val = I2S_STEREO_CONFIG;
	
	#ifdef AUDIO_OUTPUT_FROM_IIS1
	pI2sSt->i2s_tx_enable = 1;
    #if (IIS_OUTPUT_CHANNEL_BIT_NUM == 16)
	pI2sSt->i2s_tx_24_bit = 0;
    #else
    pI2sSt->i2s_tx_24_bit = 1;
    #endif
	#endif

	#ifdef AUDIO_INPUT_FROM_IIS1
	pI2sSt->i2s_rx_enable = 1;
    #if (IIS_INPUT_CHANNEL_BIT_NUM == 16)
	pI2sSt->i2s_rx_24_bit = 0;
    #else
    pI2sSt->i2s_rx_24_bit = 1;
    #endif
	#endif

	/*
	*i2s_fmt
	*0 -> i2s mode
	*1 -> dsp A mode
	*/
	pI2sSt->i2s_fmt = 0;					//i2s mode

	#ifdef IIS1_BCLK_LRCK_FROM_ST_IIS
	pI2sSt->i2s_bck_lrck_sel = 0;
	#elif defined IIS1_BCLK_LRCK_FROM_IIS2
	pI2sSt->i2s_bck_lrck_sel = 1;
	#elif defined IIS1_BCLK_LRCK_FROM_IIS0
	pI2sSt->i2s_bck_lrck_sel = 2;
	#else
	DBG_Printf("err i2s_bck_lrck_sel set\n\r");
	#endif


	pI2sSt->i2s_lrck_swap = 0;	

	/*
	*pI2sSt->i2s_bck_div = 3;	
	*pI2sSt->i2s_lrck_div = 23; 
	*/
	pI2sSt->i2s_bck_div = 2;	
	pI2sSt->i2s_lrck_div = 31;	

	/*
	*i2s_rx_mode
	*0 -> i2s slave
	*1 -> i2s master
	*/	
	#ifdef IIS1_MASTER_ENABLE
	pI2sSt->i2s_rx_mode = 1;					//i2s master
	#else
	pI2sSt->i2s_rx_mode = 0;					//i2s slave
	#endif
	pI2sSt->i2s_out_fifo_th = 4;	
	pI2sSt->i2s_in_fifo_th = 6;	

	I2S_STEREO_CONFIG = val;
	#endif

	//config PLL
	val = REG_MISC_FRACPLL;

	// #if (((defined AUDIO_INPUT_FROM_IIS1)||(defined AUX_INPUT_FROM_IIS1)) && (AUDIO_IN_STREAM_CHANNEL == 4))
	// val = I2S_TDM_CONFIG_CDEF;
	// pTDM->i2s_stereo_data_in_2 = 1;
	// I2S_TDM_CONFIG_CDEF = val;	
	// #endif

	//config pwm
	//32'b 0_1_00_0_001110_0_1_11_0_0_1_01_0111_00_00_10_1_1; //8x //24bit, 2 chans, 8x, quan 7+1, 48k

	val = DA_PP_CLASSD_CONFIG_AHB_2;
	classd_config_ahb_2->pwm_drv_manul = 0;
	classd_config_ahb_2->pwm_err_en = 0;         //no check pwm_err input		
	classd_config_ahb_2->pwm_drv_max = 7;
	classd_config_ahb_2->pwm_drv_in = 3;
	classd_config_ahb_2->pwm_min = 5;
	classd_config_ahb_2->sdm_noise_en = 0;
	classd_config_ahb_2->fifo_rst = 0;	
	classd_config_ahb_2->fifo_rd_th = 8;         //for 8 chs 24b

	#ifdef AUDIO_INPUT_FROM_IIS_TDM
	classd_config_ahb_2->osrn_en = 0;
	#else
	classd_config_ahb_2->osrn_en = 1;            //pwm/pdm enable
	#endif

	classd_config_ahb_2->pwm_clk_en = 0;
	classd_config_ahb_2->syncK = 2;              //K4
	classd_config_ahb_2->dc_block_en = 0;
	classd_config_ahb_2->ch_swap = 0;
	classd_config_ahb_2->ch_mono = 0;            //mono
	classd_config_ahb_2->ch2_disable = 0;
	classd_config_ahb_2->pwm_clk_en = 1;         //enable pwm clk	
	
	DA_PP_CLASSD_CONFIG_AHB_2 = val;

			
	/*dc block start */
	DA_PP_DC_BLOCK_R = 0xc004ea4b;    //Q30 -0.9997 based on 8x48k, -1.2db/30Hz, please check with optek_dc_block_design.m
	/* dc block end */
	


	/* MI2S Rx in start */
	

	/* DA_CD_IN_FIFO_ADDR */
	val = MI2S_RX_CONFIG; 

	pMi2sRx->mi2s_enable = 0;

	#ifdef IIS0_MASTER_ENABLE
	pMi2sRx->mi2s_mode = 1; 		//master
	#else
	pMi2sRx->mi2s_mode = 0; 		//slave
	#endif

	#if (IIS_INPUT_CHANNEL_BIT_NUM == 24)
	pMi2sRx->mi2s_24_bit = 1;
	#else
	pMi2sRx->mi2s_24_bit = 0;     	//16-bit
	#endif

	#ifdef AUDIO_INPUT_FROM_IIS_TDM
	pMi2sRx->mi2s_fmt = 1;
	#else
	pMi2sRx->mi2s_fmt = 0;
	#endif

	pMi2sRx->mi2s_swap = 0;
	pMi2sRx->cdrom_en = 0;
	pMi2sRx->cdrom_discramble = 0;
	pMi2sRx->cdrom_mode = 1;
	pMi2sRx->mi2s_bck_div = 2;
	pMi2sRx->mi2s_lrck_div = 31;


	U32 chan = AUDIO_IN_STREAM_CHANNEL;
	switch (chan)
	{
		case 8:
		pMi2sRx->mi2s_chan_mask = 0x0F;
		break;

		case 6:
		pMi2sRx->mi2s_chan_mask = 0x07;
		break;

		case 4:
		pMi2sRx->mi2s_chan_mask = 0x03;
		break;

		case 2:
		pMi2sRx->mi2s_chan_mask = 0x01;
		break;

		default:
		pMi2sRx->mi2s_chan_mask = 0x01;
		break;
	}

	pMi2sRx->mi2s_cd_sel = 1;      //mi2s in

	#ifdef AUDIO_INPUT_FROM_IIS_TDM
	pMi2sRx->mi2s_fifo_th = 20;
	#else
	pMi2sRx->mi2s_fifo_th = 10;     //please check with burst_size
	#endif
	
	#ifdef IIS0_BCLK_LRCK_FROM_IIS0
	pMi2sRx->mi2s_bck_lrck_sel = 0;
	#elif defined IIS0_BCLK_LRCK_FROM_IIS2
	pMi2sRx->mi2s_bck_lrck_sel = 1;
	#elif defined IIS0_BCLK_LRCK_FROM_ST_IIS
	pMi2sRx->mi2s_bck_lrck_sel = 2;
	#else
	DBG_Printf("err mi2s_bck_lrck_sel set\n\r");
	#endif

	MI2S_RX_CONFIG = val;


	val = DA_PP_CLASSD_CONFIG;
	#if (IIS_OUTPUT_CHANNEL_BIT_NUM == 24)
	classd_config_ahb->classd_data_24_bit = 1;
	#else
	classd_config_ahb->classd_data_24_bit = 0; //16-bit
	#endif

	classd_config_ahb->classd_6db = 0;         //default: 0-> 0dB;1-> 6dB
	
	#ifdef AUDIO_INPUT_FROM_IIS_TDM
	classd_config_ahb->amclk_sel = 1;
	#else
	classd_config_ahb->amclk_sel = 0;
	#endif

//sigma delta frequency
#if (defined PDM_SDM64x)
	classd_config_ahb->osr_div = 2;	            //96/4 = 24
	classd_config_ahb->sdm_ext	= 0;            //sdm64
	classd_config_ahb->sigma_delta_quan = 1;
	classd_config_ahb->mode = 1;                //pdm
	DA_PP_PDM_SDM_LIM = 0x446;

#elif (defined PDM_SDM128x)
	classd_config_ahb->osr_div = 1;             //48/2 = 24
	classd_config_ahb->sdm_ext	= 1;            //sdm128
	classd_config_ahb->sigma_delta_quan = 1;
	classd_config_ahb->mode = 1;				//pdm
	DA_PP_PDM_SDM_LIM = 0x446;

#elif (defined PWM_4096x_SDM16x)
	classd_config_ahb->osr_div = 4;            //192/8 = 24
	classd_config_ahb->sdm_ext	= 1;           //osr8->osr16->sdm->pwm
	classd_config_ahb->sigma_delta_quan = 7;   // +/- 128

#ifdef SDM_O5
	classd_config_ahb->pwm_sdm_o5 = 1;
#else	
	classd_config_ahb->pwm_sdm_o5 = 0;
#endif	//SDM_O5/SDM_O5
	classd_config_ahb->mode = 0;						//pwm

#elif (defined PWM_4096x_SDM8x)
	classd_config_ahb->osr_div = 4;						//192/8 = 24
	classd_config_ahb->sdm_ext	= 0;					//osr8->sdm->pwm	
	classd_config_ahb->sigma_delta_quan = 8;			// +/- 256

#ifdef SDM_O5
	classd_config_ahb->pwm_sdm_o5 = 1;
#else	
	classd_config_ahb->pwm_sdm_o5 = 0;
#endif	//SDM_O5
	classd_config_ahb->mode = 0;              //pwm

#elif (defined PWM_2048x_SDM16x)
	classd_config_ahb->osr_div = 4;	
	classd_config_ahb->sdm_ext	= 1;           //osr8->osr16->sdm->pwm
	classd_config_ahb->sigma_delta_quan = 7;

#ifdef SDM_O5
	classd_config_ahb->pwm_sdm_o5 = 1;
#else	
	classd_config_ahb->pwm_sdm_o5 = 0;
#endif	//SDM_O5

	classd_config_ahb->mode = 0;             //pwm

#elif (defined PWM_2048x_SDM8x)

	#ifdef AUDIO_INPUT_FROM_IIS_TDM
	classd_config_ahb->osr_div = 1;
	#else
	classd_config_ahb->osr_div = 4;
	#endif
	
	classd_config_ahb->sdm_ext	= 0;         //osr8->sdm->pwm
	classd_config_ahb->sigma_delta_quan = 7; // +/- 128  

#ifdef SDM_O5
	classd_config_ahb->pwm_sdm_o5 = 1;
#else	
	classd_config_ahb->pwm_sdm_o5 = 0;
#endif	//SDM_O5

	classd_config_ahb->mode = 0; //pwm

#else
	//error
	DBG_Assert (0);
#endif


	classd_config_ahb->stream_type = 0;	 			//48k
	classd_config_ahb->sd_reset_en = 1;
	classd_config_ahb->pwm_reset_en = 1;
	classd_config_ahb->pwm_mi2s_chs = (AUDIO_OUT_STREAM_CHANNEL/2 -1);
	classd_config_ahb->classd_clk_err_en = 0;

	//mi2s
	classd_config_ahb->mi2s_da_pp_n = 1;           	//pcm data from fifo, please check osrn_en
	classd_config_ahb->mi2s_lrck_swap = 0;
	
	#ifdef AUDIO_INPUT_FROM_IIS_TDM
	classd_config_ahb->mi2s_sf_sel = 1; 			//96k
	#else
	classd_config_ahb->mi2s_sf_sel = 0;           	//48k
	#endif

	classd_config_ahb->mi2s_i2s_en = 1;
	DA_PP_CLASSD_CONFIG = val;

	DA_PP_MUTE_EN = 0;

	DA_PP_CLASSD_EN = 0x01; //classd enable
}

void hw_audio_mi2s_master_slave_switch(U8 mode)
{
	U32 val;
	MI2S_RX_CONFIG_AHB *pMi2sRx = (MI2S_RX_CONFIG_AHB *) &val;

	val = MI2S_RX_CONFIG;
	if (mode == IIS_RX_MASTER)
	{
		 //master
		pMi2sRx->mi2s_mode = 1;
		DBG_Puts ("mi2s rx master\n\r");
	}
	else
	{
		 //slave
		pMi2sRx->mi2s_mode = 0;
		DBG_Puts ("mi2s rx slave\n\r");
	}

	MI2S_RX_CONFIG = val;
}

void hw_audio_mi2s_osr_div_set(U8 audio_div)
{
	U32 val;
	CLASSD_CONFIG_AHB *classd_config_ahb = (CLASSD_CONFIG_AHB *) &val;

	DBG_Printf ("mi2s_osr_div_set=%d\n\r", audio_div);

	val = DA_PP_CLASSD_CONFIG;
	//2^osr_div
	classd_config_ahb->osr_div = audio_div;
	DA_PP_CLASSD_CONFIG = val;
}

void hw_audio_mi2s_in_data_width_set(U8 data_width)
{
	U32 val;
	MI2S_RX_CONFIG_AHB *pMi2sRx = (MI2S_RX_CONFIG_AHB *) &val;

	val = MI2S_RX_CONFIG;
	if (data_width == IIS_RX_DATA_24_BTT_WIDTH)
	{
		//24-bit
		pMi2sRx->mi2s_24_bit = 1;
		DBG_Printf ("mi2s_in_data_width:24-bit\n\r");
	}
	else
	{
		//16-bit
		pMi2sRx->mi2s_24_bit = 0;
		DBG_Printf ("mi2s_in_data_width:16-bit\n\r");
	}
	MI2S_RX_CONFIG = val;
}

void hw_audio_mi2s_in_channel_set(u8 channel_nummber)
{
	int val;
	MI2S_RX_CONFIG_AHB *pMi2sRx = (MI2S_RX_CONFIG_AHB *) &val;

	DBG_Printf ("mi2s_in_channel=%d\n\r", channel_nummber);

	val = MI2S_RX_CONFIG;
	switch(channel_nummber)
	{
		case 8:
			//8 channels
			pMi2sRx->mi2s_chan_mask = 0x0F;
			break;

		case 6:
			//6 channels
			pMi2sRx->mi2s_chan_mask = 0x07;
			break;

		case 4:
			//two data, 4 channels
			pMi2sRx->mi2s_chan_mask = 0x03;
			break;

		case 2:
			//2 channels
			pMi2sRx->mi2s_chan_mask = 0x01;
			break;

		default:
 			//2 channels
 			pMi2sRx->mi2s_chan_mask = 0x01;
			break;
	}	
	MI2S_RX_CONFIG = val;
}


void hw_audio_mi2s_out_channel_set(U8 channel_nummber)
{
	U32 val;
	CLASSD_CONFIG_AHB *classd_config_ahb = (CLASSD_CONFIG_AHB *) &val;

	DBG_Printf ("mi2s_out_channel=%d\n\r", channel_nummber);
	
	val = DA_PP_CLASSD_CONFIG;
	switch(channel_nummber)
	{
		case 16:
			//8 channels
			classd_config_ahb->pwm_mi2s_chs = 8 - 1 ;
			break;
					
		case 14:
			//8 channels
			classd_config_ahb->pwm_mi2s_chs = 7 - 1 ;
			break;
					
		case 12:
			//8 channels
			classd_config_ahb->pwm_mi2s_chs = 6 - 1 ;
			break;
					
		case 10:
			//8 channels
			classd_config_ahb->pwm_mi2s_chs = 5 - 1 ;
			break;

		case 8:
			//8 channels
			classd_config_ahb->pwm_mi2s_chs = 4 - 1 ;
			break;

		case 6:
			//6 channels
			classd_config_ahb->pwm_mi2s_chs = 3 - 1;
			break;

		case 4:
			//two data, 4 channels
			classd_config_ahb->pwm_mi2s_chs = 2 - 1;
			break;

		case 2:
			//2 channels
			classd_config_ahb->pwm_mi2s_chs = 1 - 1;
			break;

		default:
 			//2 channels
			classd_config_ahb->pwm_mi2s_chs = 1 - 1;
			break;
	}

	DA_PP_CLASSD_CONFIG = val;
}

void hw_audio_mi2s_mclk_switch(U8 external_mclk)
{
	u32 val;
	MISC_CPUCLK5_CONTROLs *pFracCLK_5 = (MISC_CPUCLK5_CONTROLs *) &val;

	val = REG_MISC_CPUCLK_5;

	if (external_mclk)
	{
		pFracCLK_5->pllFrac_apwm_sel = pllFrac_sel_MCLK_in_2x;
		DBG_Puts ("mi2s_mclk:external\n\r");
	}
	else
	{
		pFracCLK_5->pllFrac_apwm_sel = pllFrac_sel_FOUT2;
		DBG_Puts ("mi2s_mclk:internal\n\r");
	}

	REG_MISC_CPUCLK_5 = val;
}

/*
* pllFrac_sel_Crystal_48MHz
* pllFrac_sel_Crystal_96MHz
* */
void hw_audio_pllFrac_apwm_sel(U8 pllFrac_apwm_sel)
{
	DBG_Printf("%s\n\r",__func__);
	u32 val;
	MISC_CPUCLK5_CONTROLs *pFracCLK_5 = (MISC_CPUCLK5_CONTROLs *) &val;
	CLASSD_CONFIG_AHB *classd_config_ahb = (CLASSD_CONFIG_AHB *) &val;

	switch (pllFrac_apwm_sel)
	{
	case pllFrac_sel_Crystal_48MHz:

		val = REG_MISC_CPUCLK_5;
		pFracCLK_5->pllFrac_apwm_sel = pllFrac_sel_Crystal_48MHz;
		REG_MISC_CPUCLK_5 = val;

		val = DA_PP_CLASSD_CONFIG;
		//2^osr_div
		classd_config_ahb->osr_div = 1;
		DA_PP_CLASSD_CONFIG = val;

		break;

	case pllFrac_sel_Crystal_96MHz:

		val = REG_MISC_CPUCLK_5;
		pFracCLK_5->pllFrac_apwm_sel = pllFrac_sel_Crystal_96MHz;
		REG_MISC_CPUCLK_5 = val;

		val = DA_PP_CLASSD_CONFIG;
		//2^osr_div
		classd_config_ahb->osr_div = 2;
		DA_PP_CLASSD_CONFIG = val;

		hw_audio_d2ayn_sel();
		break;
	
	default:
		break;
	}
}

void hw_audio_d2ayn_sel(void)
{
	DBG_Printf("%s\n\r",__func__);
	U32 val;
	CLASSD_D2ASYN_CONFIGs *pD2asyn = (CLASSD_D2ASYN_CONFIGs *)&val;

	val = DA_PP_D2AYN_CONFIG;

	pD2asyn->d2asyn_step = 588;				//44.1*625/588 = 47.875
	//pD2asyn->d2asyn_step = 625;			//d2asyn bypass

	pD2asyn->d2asyn_rom_L625 = 1;

	pD2asyn->d2asyn_en=1;

	DA_PP_D2AYN_CONFIG = val;
}

/*
* i2s_bck_lrck_sel
* 2'b00->i2s st, 2'b01->from mi2s out, 2'b10->from mi2s in
* */
void i2s_st_bck_lrck_sel(U8 value)
{
	DBG_Printf("%s:%d\n\r",__func__,value);
	U32 val;
	I2S_ST_CONFIG_AHB *pI2sSt = (I2S_ST_CONFIG_AHB *) &val;
	GPIO_PINs *p_gpip_pin = (GPIO_PINs *) &val;	

	val = I2S_STEREO_CONFIG;
	pI2sSt->i2s_bck_lrck_sel = value;

	//1'b0->slave, 1'b1->master
	pI2sSt->i2s_rx_mode = 1;

	//1'b1->enable, 1'b0->disable
	pI2sSt->i2s_tx_enable = 1;

	// //1'b0->i2s, 1'b1->dsp A
	// pI2sSt->i2s_fmt = 0;

	// pI2sSt->i2s_lrck_swap = 0;

	// pI2sSt->i2s_bck_div = 2;
	// pI2sSt->i2s_lrck_div = 31;

	// pI2sSt->i2s_out_fifo_th = 4;
	// pI2sSt->i2s_in_fifo_th = 6;

	I2S_STEREO_CONFIG = val;

	val = REG_GPIO3_FUNC;
	p_gpip_pin->bit_21 = !(!(1<<(GPIO0_21_INDEX-GPIO0_00_INDEX))); //i2s1 bck
	p_gpip_pin->bit_22 = !(!(1<<(GPIO0_22_INDEX-GPIO0_00_INDEX))); //i2s1 lclk
	REG_GPIO3_FUNC = val;
}

void mi2s_bck_lrck_sel_switch(U8 value)
{
	U32 val;
	MI2S_RX_CONFIG_AHB *pMi2sRx = (MI2S_RX_CONFIG_AHB *) &val;

	val = MI2S_RX_CONFIG;
	DBG_Printf("%s:val=0x%x,value=%d\n\r", __func__, val, value);
	DBG_Printf("[befor]mi2s_bck_lrck_sel:%d\n\r", pMi2sRx->mi2s_bck_lrck_sel);
	pMi2sRx->mi2s_bck_lrck_sel = value;

	MI2S_RX_CONFIG = val;
}

void i2s_stereo_data_in_2_switch(U8 value, U8 input_channel, U8 output_channel)
{
	U32 val;
	I2S_TDM_CONFIG_CDEF_AHB	*pTDM = (I2S_TDM_CONFIG_CDEF_AHB *) &val;
	GPIO_PINs *p_gpip_pin = (GPIO_PINs *) &val;

	val = REG_GPIO3_FUNC;
	if (value)
	{
		p_gpip_pin->bit_20 = !(!(1<<(GPIO0_20_INDEX-GPIO0_00_INDEX))); //i2s1 dout
	}
	else
	{
		p_gpip_pin->bit_20 = (!(1<<(GPIO0_20_INDEX-GPIO0_00_INDEX))); //i2s1 dout
	}
	REG_GPIO3_FUNC = val;

	val = I2S_TDM_CONFIG_CDEF;
	DBG_Printf("%s:val=0x%x,value=%d\n\r", __func__, val, value);
	DBG_Printf("[befor]i2s_stereo_data_in_2:%d\n\r", pTDM->i2s_stereo_data_in_2);
	pTDM->i2s_stereo_data_in_2 = value;
	I2S_TDM_CONFIG_CDEF = val;

	hw_audio_mi2s_in_channel_set(input_channel);

	hw_audio_mi2s_out_channel_set(output_channel);

}
