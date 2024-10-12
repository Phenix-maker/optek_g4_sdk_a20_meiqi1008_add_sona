#include "os_config.h"

#include "regmap.h"

#include "hw_timer.h"
#include "hw_audio_pwm_pdm.h"
#include "hw_cpu_clock.h"
#include "hw_pll.h"

#ifdef SPDIF_ENABLE
#include "spdif_process.h"
#endif


double base_val;
double pre_base_val;


void hw_pll_clock_int (void)
{
	U32 val;
	MISC_PLLFRAC_CONTROLs *pFracPll = (MISC_PLLFRAC_CONTROLs  *) &val;	
	MISC_CPUCLK5_CONTROLs *pFracCLK_5 = (MISC_CPUCLK5_CONTROLs *) &val;

	//config PLL
	val = REG_MISC_FRACPLL;
	pFracPll->pllFrac_PD = 0;						
	pFracPll->pllFrac_VCOPD = 0;						
	pFracPll->pllFrac_REFDIV = 2;
	pFracPll->pllFrac_POSTDIV1 = 1;
	pFracPll->pllFrac_POSTDIV2 = 1;	
	REG_MISC_FRACPLL = val;

	hw_pll_audio_clock_set (SF_BASE_48000, SPDIF_SF_1x);

	val = REG_MISC_CPUCLK_5;
	pFracCLK_5->pllFrac_apwm_sel = pllFrac_sel_FOUT2;  //select internal clock
	REG_MISC_CPUCLK_5 = val;
}

void hw_pll_clock_set (U32 freq)
{

}


//max cpu 400MHz, 260Mhz, 200Mhz, 768MHz/N
//for 32kHz, 64kHz AND 128kHz, 12.288 * 64 = 786.432 786.432/(2 OR 4) CPU CLK, 786.432/4 apwm CLK, 786.432/6 spdif CLK

/***********************************************************************************
	12.288 12.288*8*8/24 = 32.768
	FBDIV	32
	FRAC	0.678 * 0x1000000 (16777216) = 12,884,901.888 = 12,884,901.902 

	11.2896 11.2896*8*8/24 = 30.1056
	FBDIV   30
	FRAC    0.1056 * 0x1000000 (16777216) = 1,771,674.0096 = 1,771,674 

	8.192 8.192*8*8/24 = 21.845	(8.192 x 64 = 393.216MHz, PLL min VCO > 400MHz ?)
	FBDIV   16
	FRAC    0.845 * 0x1000000 (16777216) = 14,176,747.52 = 14,176,748 

	(49k*256) 12.544*8*8/24 = 33.45066666666667,
	FBDIV	33
	FRAC	0.45066666666667 * 0x1000000 (16777216) = 7,560,932.01 = 7,560,932

************************************************************************************/	

//#define CPU_CLOCK_SWITCH_TO_PLL_FRAC
#define CPU_CLOCK_WAIT_PLL_LOCK

#ifdef PLL_DIRFT_TEST
#define PLL_DIRFT_TEST_FUNC_ENABLE
#define PLL_DIRFT_MAX 100
#define PLL_DIRFT_MIN -100

#define PLL_DIRFT_DECIMAL_MAX 10
#define PLL_DIRFT_DECIMAL_MIN -10

int dirft_set_val;
int dirft_decimal_set_val;

void dirft_set_val_init(void)
{
    DBG_Printf("%s\n\r", __func__);
    #ifdef PLL_DIRFT_TEST_FUNC_ENABLE
    dirft_set_val = 0;
    dirft_decimal_set_val = 0;
    #endif
}
void dirft_set_val_set(U8 con)
{
	DBG_Printf("%s\n\r", __func__);
	#ifdef PLL_DIRFT_TEST_FUNC_ENABLE

	if (con)
	{
		if (dirft_set_val< PLL_DIRFT_MAX)
		{
			dirft_set_val++;
		}
		else
		{
			dirft_set_val = PLL_DIRFT_MAX;
		}
	}
	else
	{
		if (dirft_set_val > PLL_DIRFT_MIN)
		{
			dirft_set_val--;
		}
		else
		{
			dirft_set_val = PLL_DIRFT_MIN;
		}
	}
    DBG_Printf("dirft_set_val:%d\n\r", dirft_set_val);
	#endif

}
U32 app_key_dirft_plus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		dirft_set_val_set(TRUE);
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {     
        printf_info("pass the key twice\n\r");
    }
    else if(param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");
    }

    return 0;
}
U32 app_key_dirft_minus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		dirft_set_val_set(FALSE);
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {     
        printf_info("pass the key twice\n\r");
    }
    else if(param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");
    }

    return 0;
}

void dirft_decimal_set_val_set(U8 con)
{
	DBG_Printf("%s\n\r", __func__);
	#ifdef PLL_DIRFT_TEST_FUNC_ENABLE

	if (con)
	{
		if (dirft_decimal_set_val< PLL_DIRFT_DECIMAL_MAX)
		{
			dirft_decimal_set_val++;
		}
		else
		{
			dirft_decimal_set_val = PLL_DIRFT_DECIMAL_MAX;
		}
	}
	else
	{
		if (dirft_decimal_set_val > PLL_DIRFT_DECIMAL_MIN)
		{
			dirft_decimal_set_val--;
		}
		else
		{
			dirft_decimal_set_val = PLL_DIRFT_DECIMAL_MIN;
		}
	}
    DBG_Printf("dirft_decimal_set_val:%d\n\r", dirft_decimal_set_val);
	#endif

}

U32 app_key_dirft_decimal_plus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		dirft_decimal_set_val_set(TRUE);
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {     
        printf_info("pass the key twice\n\r");
    }
    else if(param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");
    }

    return 0;
}

U32 app_key_dirft_decimal_minus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		dirft_decimal_set_val_set(FALSE);
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {     
        printf_info("pass the key twice\n\r");
    }
    else if(param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");
    }

    return 0;
}
#endif

void hw_pll_audio_clock_set (U8 sf_base, U8 spdif_dec_sf)
{
	volatile U32 val;
	U32 cpu_clk_used_pll_frac;
	MISC_PLLFRAC_CONTROLs *pllfrac1_config;
	MISC_CPUCLK5_CONTROLs *pllfrac3_config;
	MISC_CPUCLK_CONTROLs *pCpuClk = (MISC_CPUCLK_CONTROLs *)&val;

	#ifdef CPU_CLOCK_WAIT_PLL_LOCK
	//switch cpu clock to 96Mhz then adjust pll clock
	val = REG_MISC_CPUCLK;

	cpu_clk_used_pll_frac = pCpuClk->cpuClkSeln;
	if (cpu_clk_used_pll_frac == 1)
	{
		pCpuClk->xtal_48Mx2_sel = CPU_CLK_FREQ_SEL_96MHZ;
		pCpuClk->cpuClkSeln = 2;
		REG_MISC_CPUCLK = val;	
	}
	#endif

	val = REG_MISC_FRACPLL;
	pllfrac1_config = (MISC_PLLFRAC_CONTROLs *) &val;
	
	if (sf_base == SF_BASE_48000)
	{
		pllfrac1_config->pllFrac_FBDIV = 32;
		#ifdef PLL_DIRFT_TEST
        REG_MISC_FRACPLL_FRAC = 12884902 + (100000*dirft_set_val) + (10000*dirft_decimal_set_val);
        #else
		REG_MISC_FRACPLL_FRAC = 12884902;
        #endif
		base_val = 32.768;
	}
	else if (sf_base == SF_BASE_44100)
	{
		pllfrac1_config->pllFrac_FBDIV = 30;
		REG_MISC_FRACPLL_FRAC = 1771674;
		base_val = 30.1056;
	}
	else if (sf_base == SF_BASE_32000)
	{
		pllfrac1_config->pllFrac_FBDIV = 21;
		REG_MISC_FRACPLL_FRAC = 14176748;
		base_val = 21.845;
	}
	else if (sf_base == SF_BASE_DET)//49k
	{
		pllfrac1_config->pllFrac_FBDIV = 33;
		REG_MISC_FRACPLL_FRAC = 7560932;
	}
	else
	{
		DBG_Assert (FALSE);
	}

	pre_base_val = base_val;
	
	REG_MISC_FRACPLL = val;	


	#ifdef CPU_CLOCK_WAIT_PLL_LOCK
	//cpu clock switch to pll
	if (cpu_clk_used_pll_frac == 1)
	{
		delayus(10);
		
		val = REG_MISC_CPUCLK;
		pCpuClk->cpuClkSeln = 1;
		REG_MISC_CPUCLK = val;	
	}
	#endif

	asm("nop");

	val = REG_MISC_CPUCLK_5;
    
	pllfrac3_config = (MISC_CPUCLK5_CONTROLs *) &val; 

	{
		switch (spdif_dec_sf)
		{
		case SPDIF_SF_1x:
			pllfrac3_config->pllFrac_spdif_dec_n = 2;
			break;

		case SPDIF_SF_2x:
			pllfrac3_config->pllFrac_spdif_dec_n = 1;
			break;

		case SPDIF_SF_4x:
			pllfrac3_config->pllFrac_spdif_dec_n = 0;	
			break;

		case SPDIF_SF_hx:
			pllfrac3_config->pllFrac_spdif_dec_n = 4;
			break;

		default:
			DBG_Assert (FALSE);
			break;
		}
	}
		
	REG_MISC_CPUCLK_5 = val;
}

#ifdef AUDIO_CLOCK_SYNC_ENABLE
void hw_audio_pll_clk_adj_by_driftR(double driftR)
{
	U32 tmp,tmp2;
	double ppm = driftR*1000000.0;

	MISC_PLLFRAC1_CONTROLs *pllfrac1_config;
	double base_val,adjed_val;


	I32 int_part = (int)ppm;
	U32 frac_part = abs((int)((ppm - (double)int_part)*1000.0)); 
	#ifdef AUDIO_CLOCK_SYNC_PRINTF
	DBG_Printf("%d.%03dppm\r\n",int_part,frac_part);
	#endif
	if (audio_in_stream.sample_rate == 48000 || audio_in_stream.sample_rate == 96000)
	{
		base_val = 32.768;
	}
	else if (audio_in_stream.sample_rate == 44100 || audio_in_stream.sample_rate == 88200)
	{
		base_val = 30.1056;
	}
	else if (audio_in_stream.sample_rate == 32000 || audio_in_stream.sample_rate == 64000)
	{
		base_val = 21.845;
	}	
	else 
	{
		//not support now,study later
		DBG_Assert(false);
		DBG_Printf("pll adj not support\r\n");
		return;
	}

	adjed_val = base_val + driftR*base_val;


	tmp = REG_MISC_FRACPLL;
	pllfrac1_config = (MISC_PLLFRAC1_CONTROLs *) &tmp;

	pllfrac1_config->pllFrac_FBDIV = (unsigned int)adjed_val;//32;
	REG_MISC_FRACPLL_FRAC = (adjed_val - (unsigned int)adjed_val)*0x1000000;

	//DBG_Printf("audio clk adj int:%d\r\n",pllfrac1_config->pllFrac_FBDIV);

	REG_MISC_FRACPLL = tmp;	

	asm("nop");
	asm("nop");
	
}
#endif

#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE

void hw_audio_pll_clk_adj_by_drift(double drift)
{
#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	U32 tmp,tmp2;
	U32 sample_rate = audio_in_stream.sample_rate;

	MISC_PLLFRAC1_CONTROLs *pllfrac1_config;
	MISC_PLLFRAC2_CONTROLs *pllfrac2_config;
	double adjed_val;

	DBG_Printf("Clk adjust: %d%%ppm,", (int)(drift*100*1000000));
	if (drift < 0)
	{
		DBG_Printf("slower\n\r");
	}
	else
	{
		DBG_Printf("faster\n\r");
	}

	//+/- 50ppm
	if (drift > 0.000050 || drift < -0.000050)
	{
		DBG_Printf("|clk diff|>50ppm\n\r");
		DBG_Printf("Clk adjusted isn't supported\n\r");
		return;
	}

	if (sample_rate == 48000)
	{
		//12.288 12.288*8*6/24 = 24.576
		base_val = 24.576; //576: 9663676
	}
	else if (sample_rate == 44100)
	{
		//11.2896 11.2896*8*6/24 = 22.5792
		base_val = 22.5792; //9717364
	}
	else if (sample_rate == 32000)
	{
		//8.192 8.192*8*6/24 = 16.384
		base_val = 16.384;
	}
	else 
	{
		//not support now,study later
		DBG_Assert(FALSE);
		DBG_Printf("pll adj not support\r\n");
		return;
	}

	//if drift more than 0, the device clock is faster than the host clock.
	pre_base_val = pre_base_val - drift*base_val;

	adjed_val = pre_base_val;

	tmp = REG_MISC_FRACPLL;	
	pllfrac1_config = (MISC_PLLFRAC1_CONTROLs *) &tmp;
	
	tmp2 = 0;
	pllfrac2_config = (MISC_PLLFRAC2_CONTROLs *) &tmp2;

	/*
	*pllfrac1_config->pllFrac_FBDIV = (unsigned int)adjed_val;
	*pllfrac2_config->pllfrac_FBFRAC = (adjed_val - (unsigned int)adjed_val)*0x1000000;
	*/

	DBG_Printf("audio clk adj int:%d,frac:%d\r\n", pllfrac1_config->pllFrac_FBDIV, pllfrac2_config->pllfrac_FBFRAC);

	REG_MISC_FRACPLL = tmp;

	asm("nop");
	asm("nop");

	REG_MISC_FRACPLL_FRAC = tmp2;
#endif //CLOCK_SYNC_WITH_MASTER_ENABLE
}

void hw_audio_pll_clk_adj_by_sample_rate(double drift)
{
#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	U32 sample_rate = audio_in_stream.sample_rate;

	U32 tmp,tmp2;
	U32 sample_rate = audio_in_stream.sample_rate;

	MISC_PLLFRAC1_CONTROLs *pllfrac1_config;
	MISC_PLLFRAC2_CONTROLs *pllfrac2_config;
	double base_val_1;
	double adjed_val;

	DBG_Printf("%d%%ppm\n\r", (int)(drift*100*1000000));

	if (sample_rate == 48000)
	{
		base_val_1 = 32.768;
	}
	else if (sample_rate == 44100)
	{
		base_val_1 = 30.1056;
	}
	else if (sample_rate == 32000)
	{
		base_val_1 = 21.845;
	}
	else 
	{
		//not support now,study later
		DBG_Assert(FALSE);
		DBG_Printf("pll adj not support\r\n");
		return;
	}


	//if drift more than 0, the device clock is faster than the host clock.
	adjed_val = base_val_1 - drift*base_val;

	tmp = REG_MISC_FRACPLL;	
	pllfrac1_config = (MISC_PLLFRAC1_CONTROLs *) &tmp;
	
	tmp2 = 0;
	pllfrac2_config = (MISC_PLLFRAC2_CONTROLs *) &tmp2;

	/*
	*pllfrac1_config->pllFrac_FBDIV = (unsigned int)adjed_val;
	*pllfrac2_config->pllfrac_FBFRAC = (adjed_val - (unsigned int)adjed_val)*0x1000000;
	*/

	DBG_Printf("audio clk adj int:%d,frac:%d\r\n", pllfrac1_config->pllFrac_FBDIV, pllfrac2_config->pllfrac_FBFRAC);

	REG_MISC_FRACPLL = tmp;

	asm("nop");
	asm("nop");

	REG_MISC_FRACPLL_FRAC = tmp2;

#endif //CLOCK_SYNC_WITH_MASTER_ENABLE
}

void hw_audio_pll_clk_adj_by_dist_offset(U32 local_distance, i32 offset)
{

}

#endif //CLOCK_SYNC_WITH_MASTER_ENABLE
