/*
 * audio_clock_sync.c
 *
 *  Created on: 2024/3/1
 *      Author: Windows
 */

#include "os_config.h"

#ifdef AUDIO_CLOCK_SYNC_ENABLE
#include "hw_cpu_clock.h"
#include "hw_pll.h"
#include "hw_timer.h"
#include "otg.h"

#include "system_timer.h"
#include "app_media_com.h"

#include "audio_clock_sync.h"

#define AUDIO_CLOCK_SYNC_FUNC_ENABLE

#ifndef AUDIO_CLOCK_SYNC_PRINTF
#define DBG_Printf
#endif

AUDIO_CLOCK_SYNC_STRU audio_clock_sync_vari;

void usbdev_clock_sync_irq(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef AUDIO_CLOCK_SYNC_FUNC_ENABLE
	//printf_info("src_type:%d\n\r",sync_w_src_control.sync_clks.src_type);
	if (sync_w_src_control.sync_clks.src_type == ASOURCE_USB_HOST)
	{
		//printf_info("restart:%d\n\r",sync_w_src_control.sync_clks.restart);
		sync_w_src_control.sync_clks.localCurCount_L = read_ccount();
		if (sync_w_src_control.sync_clks.restart == TRUE)
		{
			sync_w_src_control.sync_clks.restart = FALSE;
			sync_w_src_control.sync_clks.localStartCount = sync_w_src_control.sync_clks.localCurCount_L;
			sync_w_src_control.sync_clks.count_bak = sync_w_src_control.sync_clks.localStartCount;
			sync_w_src_control.sync_clks.localCurCount_H = 0;
			sync_w_src_control.sync_clks.srcCount = 0;
		}
		else
		{

			if (sync_w_src_control.sync_clks.localCurCount_L < sync_w_src_control.sync_clks.count_bak)
			{
				sync_w_src_control.sync_clks.localCurCount_H++;//carry
			}

			sync_w_src_control.sync_clks.count_bak = sync_w_src_control.sync_clks.localCurCount_L;	
			sync_w_src_control.sync_clks.srcCount++;
		}

	}

	#endif
}

#ifdef LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE

/**
 ****************************************************************************************
 * @addtogroup SYNC_WITH_AUDIO_SOURCE
 * @ingroup AUDIO_SYNC
 * 
 * @brief sync with audio source implementation
 *List of Acronyms and Abbreviations
 *driftR:Drift ratio
 *sync_w_src:sync with the audio source
 *
 * 
 * @{ 
 ****************************************************************************************
 */

void hw_audio_pll_clk_adj_by_driftR(double driftR);
void optek_link_master_clk_adj_by_driftR(double driftR);
int optek_link_slave_cal_driftR(double *driftR);
int bt_slave_cal_driftR(double *driftR);

sync_w_src_control_t  sync_w_src_control;


/*
 * PRIVATE FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief start a new  drift ratio calculation.
 *
 ****************************************************************************************
 */
static void sync_w_src_start_count(void)
{
	//DBG_Printf("%s\n\r",__func__);
	#ifdef AUDIO_CLOCK_SYNC_FUNC_ENABLE

	if (global_vari.audio_source == SOURCE_BT)
	{
		sync_w_src_control.sync_clks.src_type = ASOURCE_BT_MASTER;
		sync_w_src_control.sync_clks.counting_unit = COUNTING_UNIT_UNKNOW;
		//ld_acl_get_slave_time_sync_init();
	}
	else
	{
		if (global_vari.audio_source == SOURCE_USB_SPEAKER)
		{
			sync_w_src_control.sync_clks.src_type = ASOURCE_USB_HOST;
			sync_w_src_control.sync_clks.counting_unit = COUNTING_UNIT_MS;
		}
		else if ( global_vari.audio_source == SOURCE_OPTICAL || 
				  global_vari.audio_source == SOURCE_COAXIAL ||
				  global_vari.audio_source == SOURCE_ARC )
		{
			sync_w_src_control.sync_clks.src_type = ASOURCE_SPDIF;
			sync_w_src_control.sync_clks.counting_unit = COUNTING_UNIT_SAMPLES;
		}
		else
		{
			sync_w_src_control.sync_clks.src_type = ASOURCE_IIS_MASTER;
			sync_w_src_control.sync_clks.counting_unit = COUNTING_UNIT_SAMPLES;
		}
		sync_w_src_control.sync_clks.restart = TRUE;
	}

	#endif
}


/**
 ****************************************************************************************
 * @brief to get the source clock and local clk .
 *
 * @param[out]  source_clk  source clock,unit is machine cycle 
 * @param[out] 	local_clk   local clock,unit is machine cycle 
 * 
 * @return status      1:success | 0:clk not present
 *
 ****************************************************************************************
 */
static U8 sync_w_src_clocks_get(U64 *src_clk,U64 *local_clk)
{
	DBG_Printf("%s\n\r",__func__);
	//printf_info("%s\n\r",__func__);
	#ifdef AUDIO_CLOCK_SYNC_FUNC_ENABLE	
	if (sync_w_src_control.sync_clks.restart == TRUE)
	{
		// clk not present
		*src_clk = 1;
		*local_clk = 1;
		printf_info("sync_w_src_control.sync_clks.restart:%d\n\r",sync_w_src_control.sync_clks.restart);
		return 0;
	}
	TX_INTERRUPT_SAVE_AREA;

	TX_DISABLE;
	*local_clk = ((U64)sync_w_src_control.sync_clks.localCurCount_H<<32) | (U64)sync_w_src_control.sync_clks.localCurCount_L;
	*local_clk -= sync_w_src_control.sync_clks.localStartCount;
	*src_clk = sync_w_src_control.sync_clks.srcCount;
	TX_RESTORE;	

	if (sync_w_src_control.sync_clks.src_type == ASOURCE_USB_HOST)
	{
		*src_clk *= (SYS_CLK_FREQ_DEFAULT/1000L);
	}
		
	else if (sync_w_src_control.sync_clks.src_type == ASOURCE_SPDIF ||
			sync_w_src_control.sync_clks.src_type == ASOURCE_IIS_MASTER)
	{
		*src_clk *= SYS_CLK_FREQ_DEFAULT;
		*src_clk /= audio_in_stream.sample_rate;
	}
	else
	{
		*src_clk == 0;	
	}

	if (*src_clk == 0 || *local_clk == 0)
	{
		printf_info("src_clk:%d,local_clk:%d\n\r",*src_clk,*local_clk);
		return 0;
	}

	return 1;
	#else
	return 0;
	#endif
}

/**
 ****************************************************************************************
 * @brief calculate drift ratio and adjust local clock and master clock of optek link.
 *
 * @param[in]  adjppm   <0 means stream buffer is low,>0 means stream buffer is high.
 This value is added directly to the drift ratio to restore the stream buffer to normal
 *
 ****************************************************************************************
 */
static void sync_w_src_process(i8 adjppm)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef AUDIO_CLOCK_SYNC_FUNC_ENABLE	
	U64 srcClk,localClk;
	I64 drift;
	double driftR = 1.0;//need adjust drift ratio,initial a invalid value use to flag

	if (sync_w_src_control.sync_clks.src_type == ASOURCE_OPTEK_LINK_MASTER)
	{
		// if (optek_link_slave_cal_driftR(&driftR))
		// {
		// 	driftR = -driftR;
		// }
	}
	else
	{
		if (sync_w_src_control.sync_clks.src_type == ASOURCE_BT_MASTER)
		{
			// if (bt_slave_cal_driftR(&driftR))
			// {
			// 	driftR = -driftR;
			// }
		}
		else
		{
			//printf_info("---srcClk:0x%x,localClk:0x%x\r\n",(U32)srcClk,(U32)localClk);
			if (sync_w_src_clocks_get(&srcClk,&localClk))
			{	
				//DBG_printf("srcClk:0x%x,localClk:0x%x\r\n",(U32)srcClk,(U32)localClk);
				//printf_info("srcClk:0x%x,localClk:0x%x\r\n",(U32)srcClk,(U32)localClk);		
				drift = srcClk - localClk;

				driftR = (double)drift/(double)localClk;
			}
		}
	}


	#define PPM0p3  0.0000003
	#define PPM80   0.000150
	#define PPM0p001   0.0000000011

	DBG_Printf("adjppm:%d\r\n",adjppm);

	if (adjppm > 150)
		adjppm = 150;
	else if (adjppm < -150)
		adjppm = -150;

	driftR += PPM0p3*(double)adjppm;//adj by stream buffer,unit is 0.3ppm
	
	if (driftR < PPM80 && driftR > -PPM80)
	{
		if (driftR <= PPM0p001 && driftR >= -PPM0p001)
		{
			driftR = 0.0;
		}

		hw_audio_pll_clk_adj_by_driftR(driftR);
		
		// if (app_main_data.optek_link_role == OPL_ROLE_MASTER)
		// 	optek_link_master_clk_adj_by_driftR(driftR);
	}
	else
	{
		/*
		*driftR is too big,don't worry if it's not continuous,
		otherwise please check hardware, device or host?
		*/
		DBG_printf("driftR is too much\r\n");				
	}
	#endif
}


/*
 * PUBLIC FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief initial variable,call only once  during power on.
 *
 ****************************************************************************************
 */
void sync_w_src_init(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef AUDIO_CLOCK_SYNC_FUNC_ENABLE
	memset(&sync_w_src_control,0,sizeof(sync_w_src_control));
	sync_w_src_control.sync_clks.restart = FALSE;
	sync_w_src_control.sync_w_src_enable = FALSE;
	#endif
}

/**
 ****************************************************************************************
 * @brief start sync with audio source process.
 *
 ****************************************************************************************
 */
void sync_w_src_start(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef AUDIO_CLOCK_SYNC_FUNC_ENABLE
	sync_w_src_control.sync_process_interval_25ms = MsToOsTimerTick(START_INTERVAL_MS);
	sync_w_src_control.sync_process_dtime = system_timer_env.timer_tick + sync_w_src_control.sync_process_interval_25ms;
	sync_w_src_control.sync_w_src_enable = TRUE;
	sync_w_src_start_count();
	#endif
}

/**
 ****************************************************************************************
 * @brief stop sync with audio source process.
 *
 ****************************************************************************************
 */
void sync_w_src_stop(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef AUDIO_CLOCK_SYNC_FUNC_ENABLE
	sync_w_src_control.sync_w_src_enable = FALSE;
	#endif
}

/**
 ****************************************************************************************
 * @brief loop sync with audio source process.
 *
 ****************************************************************************************
 */
void sync_w_src_loop(void)
{
	// DBG_Printf("%s\n\r",__func__);
	#ifdef AUDIO_CLOCK_SYNC_FUNC_ENABLE
	int src_speed;
	if (sync_w_src_control.sync_w_src_enable == TRUE)
	{
		if (system_timer_env.timer_tick >= sync_w_src_control.sync_process_dtime)
		{
			src_speed = cur_decode_src_speed_get();

			if (src_speed)
			{
				sync_w_src_control.sync_process_interval_25ms = MsToOsTimerTick(ADJUSTING_INTERVAL_MS);
			}
			else
			{
				if (sync_w_src_control.sync_process_interval_25ms < MsToOsTimerTick(MAX_INTERVAL_MS))//60s
				{
					sync_w_src_control.sync_process_interval_25ms <<= 1;
					if (sync_w_src_control.sync_process_interval_25ms > MsToOsTimerTick(MAX_INTERVAL_MS))
						sync_w_src_control.sync_process_interval_25ms = MsToOsTimerTick(MAX_INTERVAL_MS);
				}
			}

			sync_w_src_control.sync_process_dtime = system_timer_env.timer_tick + sync_w_src_control.sync_process_interval_25ms;

			func_w_one_param_msgSend(sync_w_src_process,src_speed);

			func_wo_param_msgSend(sync_w_src_start_count);				
		}
	}
	#endif
}
/// @} SYNC_WITH_AUDIO_SOURCE
#endif

#endif
