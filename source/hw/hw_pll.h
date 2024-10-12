#ifndef __HW_PLL__
#define __HW_PLL__

#include "hw_cpu_clock.h"



/*
 ****************************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**
 * @enum SF_BASE
 * @brief Base sample frequency
 */
enum SF_BASE {  
	SF_BASE_48000,
	SF_BASE_44100,
	SF_BASE_32000,
	SF_BASE_DET	     //for spdif sf detect, at current it is 49kHz sample frequency
};


/**
 * @enum SF_BASE
 * @brief Base sample frequency
 */
enum SPDIF_SF {
	SPDIF_SF_1x = 2, //single speed
	SPDIF_SF_2x = 1, //double speed
	SPDIF_SF_4x = 0, //quad speed
	SPDIF_SF_hx = 3  //half, 16,22.05,24
};


/*
 ****************************************************************************************
 * CONSTANT DEFINITIONS
 ****************************************************************************************
 */
#define CRYSTAL_FREQ    (48*1000*1000)
#define CLK_BASE_FREQ   (12288*1000)

#define CPU_48MHz     (48*1000*1000)     	/* CPU 48MHz, AHB 24MHz, 1/2 BTDM xMHz */
#define CPU_96MHz     (96*1000*1000)     	/* CPU 96MHz, AHB 48MHz, 1/2 BTDM xMHz */
#define CPU_393MHz    (393216*1000)    		/* CPU 393.216MHz, AHB 393.26/2MHz */


#ifdef LOW_POWER_MODE
#define SYS_CLK_FREQ_DEFAULT    CPU_32MHz
#endif


#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE

#if (SYS_CLK_FREQ_DEFAULT == CPU_48MHz)
/* (1/48)*4294967295=8134407.7557us */
#define CLK_SYC_TOTAL_TIME_MS    (50000*2)

#elif (SYS_CLK_FREQ_DEFAULT == CPU_96MHz)
/* (1/96)*4294967295=44739242.65625us */
#define CLK_SYC_TOTAL_TIME_MS    (30000)

#elif (SYS_CLK_FREQ_DEFAULT == CPU_393MHz)
/* (1/393.216)*4294967295=310 922 666.66412354us */
#define CLK_SYC_TOTAL_TIME_MS    (30000)

#else
#error CLK_SYC_TOTAL_TIME_MS Must Be defined!
#endif

#endif /* CLOCK_SYNC_WITH_MASTER_ENABLE */


/*
 ****************************************************************************************
 * DECLARATIONS
 ****************************************************************************************
 */
extern U32 SystemClock;

void hw_pll_audio_clock_set (U8 sf_base, U8 spdif_dec_sf);

void hw_audio_pll_clk_adj_by_driftR(double driftR);

#ifdef PLL_DIRFT_TEST
void dirft_set_val_init(void);
#endif

#endif //__HW_PLL__
