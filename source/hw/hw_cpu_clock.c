#include "os_config.h"

#include "regmap.h"

#include "hw_timer.h"
#include "hw_cpu_clock.h"
#include "hw_gpio.h"
#include "hw_hardware_aon.h"


U32 SystemClock;


void hw_cpu_clock_init(void)
{
	SystemClock = SYSTEM_CLK_FREQ;

	U32 val;
	MISC_CPUCLK_CONTROLs *pCpuClk = (MISC_CPUCLK_CONTROLs *) &val;
	MISC_CPUCLK2_CONTROLs *pCpuClk2 = (MISC_CPUCLK2_CONTROLs *) &val;
	MISC_CPUCLK5_CONTROLs *pCpuClk5 = (MISC_CPUCLK5_CONTROLs *) &val;


	val = REG_MISC_CPUCLK;
	pCpuClk->cpuDynamicClkEn = 0;
	pCpuClk->periClkBypass = 0;
	pCpuClk->periClkdDiv = PERI_CLK_DIVISOR/2; //48Mhz/8
	#ifdef JTAG_DEBUG_MODE
	pCpuClk->jtag_en = 1;
	#else
	pCpuClk->jtag_en = 0;
	#endif
	REG_MISC_CPUCLK = val;

	val = REG_MISC_CPUCLK_2;
	pCpuClk2->cpuClkBypass = 1;
	pCpuClk2->dmaCpuClkBypass = 1;
	REG_MISC_CPUCLK_2 = val;

	MISC_AON_CONTROLs *mis_aon_controls = (MISC_AON_CONTROLs *) &val;;
	val = REG_MISC_AON_CONTROL;
	mis_aon_controls->cpu_mem_emaw = 0;
	mis_aon_controls->cpu_mem_emas = 0;
	mis_aon_controls->cpu_mem_ema = 0;
	REG_MISC_AON_CONTROL = val;
	
	val = REG_MISC_CPUCLK_5;
	pCpuClk5->clk_sflash_ds = 1;
	pCpuClk5->clk_psram_ds = 1;
	REG_MISC_CPUCLK_5 = val;
}

void hw_cpu_clk_set(U32 freq) __attribute__ ((section (".internal_ram_1_text")));
void hw_cpu_clk_set(U32 freq)
{	
	U32 val;
	
	MISC_CPUCLK_CONTROLs *pCpuClk = (MISC_CPUCLK_CONTROLs *) &val;
	MISC_CPUCLK5_CONTROLs *pFracCLK_5 = (MISC_CPUCLK5_CONTROLs *) &val;
	MISC_CPUCLK3_CONTROLs *pCpuClk3 = (MISC_CPUCLK3_CONTROLs *) &val;

	DBG_Printf("%s,freq=%d\n\r", __func__, freq);

	switch(freq)
	{
		case CPU_FREQ_48MHz:
			hw_dcdc_voltage_set(5+2);//0.8v+x

			val = REG_MISC_CPUCLK;
			pCpuClk->xtal_48Mx2_sel = CPU_CLK_FREQ_SEL_48MHZ;
			pCpuClk->ahbClkDiv = 0x1;
			pCpuClk->cpuClkSeln = CPU_CLK_SRC_SEL_XTAL;
			REG_MISC_CPUCLK = val;
			SystemClock = freq;
			break;
			
		case CPU_FREQ_48MHz_2X:
			hw_dcdc_voltage_set(5+2);//0.8v+x

			val = REG_MISC_CPUCLK;
			pCpuClk->xtal_48Mx2_sel = CPU_CLK_FREQ_SEL_96MHZ;
			pCpuClk->ahbClkDiv = 0x1;
			pCpuClk->cpuClkSeln = CPU_CLK_SRC_SEL_XTAL;
			REG_MISC_CPUCLK = val;
			SystemClock = freq;
			break;
			
		case CPU_FREQ_BASE_16X:
			hw_dcdc_voltage_set(5+2);//0.8v+x

			//switch cpu clock to 96Mhz, and then adjust pll clock
			val = REG_MISC_CPUCLK;
			pCpuClk->xtal_48Mx2_sel = CPU_CLK_FREQ_SEL_96MHZ;
			pCpuClk->ahbClkDiv = 0x1;
			pCpuClk->cpuClkSeln = CPU_CLK_SRC_SEL_XTAL;
			REG_MISC_CPUCLK = val;

			val = REG_MISC_CPUCLK_5;
			pFracCLK_5->pllFrac_cpu_sel = pllFrac_sel_FOUT2;
			REG_MISC_CPUCLK_5 = val;

			val = REG_MISC_CPUCLK_3;
			pCpuClk3->pSram_CLK_dly = 0x0;
			REG_MISC_CPUCLK_3 = val;

			val = REG_MISC_CPUCLK;
			pCpuClk->ahbClkDiv = 0x2;
			pCpuClk->pSramClkDiv = 4;
			REG_MISC_CPUCLK = val;

			val = REG_MISC_CPUCLK;
			pCpuClk->cpuClkSeln = CPU_CLK_SRC_SEL_PLL;
			REG_MISC_CPUCLK = val;
			SystemClock = freq;
			break;
			
		case CPU_FREQ_BASE_32X:
			hw_dcdc_voltage_set(5+3);//0.8v+x

			//switch cpu clock to 96Mhz, and then adjust pll clock
			val = REG_MISC_CPUCLK;
			pCpuClk->xtal_48Mx2_sel = CPU_CLK_FREQ_SEL_96MHZ;
			pCpuClk->ahbClkDiv = 0x1;
			pCpuClk->cpuClkSeln = CPU_CLK_SRC_SEL_XTAL;
			REG_MISC_CPUCLK = val;

			val = REG_MISC_CPUCLK_5;
			pFracCLK_5->pllFrac_cpu_sel = pllFrac_sel_FOUT1PH0;
			REG_MISC_CPUCLK_5 = val;

			val = REG_MISC_CPUCLK_3;
			pCpuClk3->pSram_CLK_dly = 0x0;
			REG_MISC_CPUCLK_3 = val;

			val = REG_MISC_CPUCLK;
			pCpuClk->ahbClkDiv = 0x3; //for running at pSram
			pCpuClk->pSramClkDiv = 6;
			REG_MISC_CPUCLK = val;

			val = REG_MISC_CPUCLK;
			pCpuClk->cpuClkSeln = CPU_CLK_SRC_SEL_PLL;
			REG_MISC_CPUCLK = val;
			SystemClock = freq;
			break;
			
		case CPU_FREQ_BASE_64X:
			/*
			*hw_dcdc_voltage_set(5+4);
			*/
			hw_dcdc_voltage_set(5+5);//0.8v+x

			//switch cpu clock to 96Mhz, and then adjust pll clock
			val = REG_MISC_CPUCLK;
			pCpuClk->xtal_48Mx2_sel = CPU_CLK_FREQ_SEL_96MHZ;
			pCpuClk->ahbClkDiv = 0x1;
			pCpuClk->cpuClkSeln = CPU_CLK_SRC_SEL_XTAL;
			REG_MISC_CPUCLK = val;

			val = REG_MISC_CPUCLK_5;
			pFracCLK_5->pllFrac_cpu_sel = pllFrac_sel_FOUTPOSTDIV;
			REG_MISC_CPUCLK_5 = val;

			val = REG_MISC_CPUCLK_3;
			pCpuClk3->pSram_CLK_dly = 0x0;
			REG_MISC_CPUCLK_3 = val;

			val = REG_MISC_CPUCLK;
			pCpuClk->ahbClkDiv = 0x5;
			pCpuClk->pSramClkDiv = 10;
			REG_MISC_CPUCLK = val;

			val = REG_MISC_CPUCLK;
			pCpuClk->cpuClkSeln = CPU_CLK_SRC_SEL_PLL;
			REG_MISC_CPUCLK = val;
			SystemClock = freq;
			break;
			
		default:
			DBG_Assert(FALSE);
			DBG_Puts("CPU clock set error");
			break;
	}
}

/**
 * @brief set soc io drive capability
 * 
 * @param ds drive capability value, rang is 0~63
 */
void hw_clk_pdm_ds_set(U8 ds)
{
	U32 val;
	MISC_CPUCLK5_CONTROLs *pFracCLK = (MISC_CPUCLK5_CONTROLs *) &val;

	val = REG_MISC_CPUCLK_5;
	pFracCLK->clk_psram_ds = ds;
	REG_MISC_CPUCLK_5 = val;
}
