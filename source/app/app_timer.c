#include "os_config.h"

#include "mem_reloc.h"

#include "hal_gpio.h"

#include "sdk_config.h"
#include "key_scan.h"
#include "system_timer.h"

#ifdef ROTARY_ENABLE
#include "rotary_used_gpio.h"
#endif

#include "arc_detect.h"
#include "arc_process.h"
#include "optek_cec.h"

#include "app_timer.h"
#include "sona_dsp.h"



void usbLUNtimerTick( void );

APP_TIMER_DATA app_timer_data;

extern U8 usbmsc_USBhostOpened;
extern int SB_Init(void);

void app_5ms_timer(U32 tick)
{
	#ifdef KEY_ENABLE
    KeyScan(tick);
	#endif

	#ifdef BATT_DETECT_ENABLE

	if(tick % 2 == 0) 	
	{
		hal_adc_vol_convert(tick);
	}	
	if(tick % 100  == 0)  //0.5s
    {
		// func_w_one_param_msgSend(hal_batt_vol_detect,tick);
		hal_batt_vol_detect(tick);

		batt1_led();
		batt2_led();
	}
	// if(tick % 140 == 0)  //700ms
 	// {

	// 	batt1_led();
	// 	batt2_led();
	// }
	if(tick % 40 == 0) // 200ms
	{
		app_sw_key_status_check(tick);

	} 

	#endif

	#ifdef ROTARY_ENABLE
	#ifdef ROTARY_DET_USED_GPIO
	rotary_used_gpio_process(tick);
	#endif  //ROTARY_DET_USED_GPIO
    #endif  //ROTARY_ENABLE

    #ifdef ARC_ENABLE
	#ifndef ARC_PLUG_AT_OTHER_SRC_ENABLE
	#ifdef ARC_PLUG_AT_STABNDBY_ENABLE
    if ( (global_vari.audio_source == SOURCE_ARC) || (app_source_get_standby_mode_status()) )
	#else
	if (global_vari.audio_source == SOURCE_ARC)
	#endif
	#else
	#ifndef ARC_PLUG_AT_STABNDBY_ENABLE
	if (!app_source_get_standby_mode_status())
	#endif
	#endif
	{
	    arc_dectect_polling();
	}
	
	#ifdef ARC_CEC_CMD_RX_POLLING_ENABLE
    if (arc_detect_status == enARC_PLUG)
    {
       cec_cmd_rx_handle(0);
    }
	#endif
	
    #endif /* ARC_ENABLE */


	#ifdef HDMI_ENABLE
	#ifdef HDMI_INT_CHECKTOUT_USED_TIMER
	if (app_timer_data.hdmi_int_finish_wait_timer)
	{
		app_timer_data.hdmi_int_finish_wait_timer--;	
		if (app_timer_data.hdmi_int_finish_wait_timer == 0)
		{
			hdmi_int_process_finish_set();
		}
	}
	#endif /* HDMI_INT_CHECKTOUT_USED_TIMER */

	if (hdmi_infor.init_enable == TRUE)
	{
		if ( (tick % 50) == 0)
		{
			func_wo_param_msgSend(SB_Init);
		}
	}
	#endif /* HDMI_ENABLE */


	#ifdef USB_HOST_ENABLE
	{
		static mscClasn_cnt = 0;

		if (mscClasn_cnt++ > 5)
		{
			if (usbmsc_USBhostOpened)
			{
				if (mscClass_isOpend())
				{
					usbLUNtimerTick();
				}
			}
			
			mscClasn_cnt = 0;
		}
	}
	#endif /* USB_HOST_ENABLE */


	#ifdef FOR_ESD_PROTECT
	if (app_timer_data.esdProtectTimer)
	{
		app_timer_data.esdProtectTimer--;
	}
	#endif /* FOR_ESD_PROTECT */


	#ifdef OPTEK_ARC_CEC_ENABLE
	if (app_timer_data.arc_plug_timer)
	{
		if (--app_timer_data.arc_plug_timer == 0)
		{
			func_w_one_param_msgSend(arc_detect_plug, 0);
		}
	}

	if (app_timer_data.arc_unplug_timer)
	{
		if (--app_timer_data.arc_unplug_timer == 0)
		{
			func_w_one_param_msgSend(arc_detect_unplug, 0);
		}
	}

	if (app_timer_data.cec_init_start_timer)
	{
		if (--app_timer_data.cec_init_start_timer == 0)
		{
			func_wo_param_msgSend(cee_set_initiate_start);
		}
	}

	if (app_timer_data.cec_rejectcom_time)
	{
		if (--app_timer_data.cec_rejectcom_time == 0)
		{
			DBG_Printf("cec_reject_time timeout\r\n");
		}
	}

	if (app_timer_data.cec_standby_wait_time)
	{
		if (--app_timer_data.cec_standby_wait_time == 0)
		{
			app_timer_cec_standby_time_set(1000);
		}
	}

	if (app_timer_data.cec_standby_time)
	{
		if (--app_timer_data.cec_standby_time == 0)
		{
			app_timer_cec_standby_time_set(1000);
		}
	}

	if (app_timer_data.cec_initiate_wait_time)
	{
		if (--app_timer_data.cec_initiate_wait_time == 0)
		{
			func_wo_param_msgSend(cee_set_initiat_wait_callback);
		}
	}

	if (app_timer_data.cec_handle_enable_time)
	{
		if (--app_timer_data.cec_handle_enable_time == 0)
		{
			DBG_Printf("cec_handle_disable_time over\r\n");
		}
	}
	#endif //OPTEK_ARC_CEC_ENABLE

	#ifdef SPI_DMA_RX_TEST
	if (app_timer_data.spi_dma_timer > 0)
	{
		if (--app_timer_data.spi_dma_timer == 0)
		{
			DBG_Puts("spi dma timer is up\n\r");
			void spi_dma_rx_test(void);
			func_wo_param_msgSend(spi_dma_rx_test);
		}
	}
	#endif

}

#ifdef ROTARY_ENABLE

#ifdef ROTARY_DET_USED_GPIO
void timer_rotary_gpio_timer_set (U16 dir)
{
	app_timer_data.rotary_gpio_dir = dir;
	
	if (app_timer_data.rotary_gpio_timer)
	{
		app_timer_data.rotary_gpio_timer = (U16) ( (100 + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS);
	}
	else
	{
		app_timer_data.rotary_gpio_timer = (U16) ( (50 + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS); 
	}
	
}
#endif //ROTARY_DET_USED_GPIO

#ifdef ROTARY_0_ENABLE
void timer_rotary0_timer_set (U16 ms)  __attribute__ ((section (".interal_ram_1_text")));

void timer_rotary0_timer_set (U16 dir)
{
	timer_data.rotary0_dir = dir;

	#ifdef CD_RIPPING
	if (app_main_data.CdRipping)
	{
		if (timer_data.rotary0_timer)
		{
			//timer_data.rotary0_timer = (U16) ( (100 + CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 100ms
			timer_data.rotary0_timer = (U16) ( (150 + CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 150ms
		}
		else
		{
			timer_data.rotary0_timer = (U16) ( (50 + CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 50ms
		}
	}
	else
	#endif
	{
		if (timer_data.rotary0_timer)
		{
			//timer_data.rotary0_timer = (U16) ( (50 + CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 25
			//timer_data.rotary0_timer = (U16) ( (75 CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 75s
			timer_data.rotary0_timer = (U16) ( (100 + CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 100ms
		}
		else
		{
			timer_data.rotary0_timer = (U16) ( (50 + CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 50ms
		}
	}
}

void timer_rotary0_timer_clr (void)
{
	timer_data.rotary0_timer = 0;
}
#endif //ROTARY_0_ENABLE

#ifdef ROTARY_1_ENABLE
void timer_rotary1_timer_set (U16 ms)  __attribute__ ((section (".interal_ram_1_text")));

void timer_rotary1_timer_set (U16 dir)
{
	timer_data.rotary1_dir = dir;

	if (timer_data.rotary1_timer)
	{
		//timer_data.rotary1_timer = (U16) ( (50 + CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 50ms
		timer_data.rotary1_timer = (U16) ( (100 + CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 100ms
	}
	else
	{
		timer_data.rotary1_timer = (U16) ( (50 + CD_TIMER_TIME - 1) / CD_TIMER_TIME); // 50ms
	}
}

void timer_rotary1_timer_clr (void)
{
	timer_data.rotary1_timer = 0;
}
#endif //ROTARY_1_ENABLE

#endif //ROTARY_ENABLE


#ifdef USB_HOST_ENABLE

/**************************usb*****************************/

void timer_usbTimeSet( U16 ms )
{
}
void timer_usbTimeStart( void )
{
}
void timer_usbTimeStop( void )
{
}

void timer_usbRHtimeSet( U16 ms )
{
}

void timer_usbRHtimerStart( void )
{
}

void timer_usbRHtimerStop( void )
{
}

void timer_usbLUNtimerSet( U16 ms, U8 maxlun )
{
	app_timer_data.usbLUNtimerRun	=	0x00;
	app_timer_data.usbLUNtime		=	ms/SYSTEM_TIMER_PERIOD_MS + 1;
	app_timer_data.usbMaxluns		=	maxlun;
	memset( app_timer_data.usbLUNtimer, 0, sizeof(app_timer_data.usbLUNtimer) );
}

void timer_usbLUNtimerStart( U16 tno )
{
	U16	tmp	=	0x01;

	DBG_assert( tno < USB_MSC_MAX_TIMER );
	
	tmp <<= tno;
	
	app_timer_data.usbLUNtimer[ tno ]	=	app_timer_data.usbLUNtime;
	app_timer_data.usbLUNtimerRun	|=	tmp;
}

void timer_usbLUNtimerStartAll( void )
{
	U16	i;

	for( i = 0; i < app_timer_data.usbMaxluns; i ++ ) {
		timer_usbLUNtimerStart( i );
	}
}

void timer_usbLUNtimerRestart( U16 tno )
{
	DBG_assert( tno < USB_MSC_MAX_TIMER );
	app_timer_data.usbLUNtimer[ tno ]	=	app_timer_data.usbLUNtime;
}

void timer_usbLUNtimerRestartAll( void )
{
	U16	i;

	for( i = 0; i < app_timer_data.usbMaxluns; i ++ ) {
		app_timer_data.usbLUNtimer[ i ]	=	app_timer_data.usbLUNtime;
	}
}

void timer_usbLUNtimerPause( U16 tno )
{
	DBG_assert( tno < USB_MSC_MAX_TIMER );	
	app_timer_data.usbLUNtimer[ tno ]	=	0;
}

void timer_usbLUNtimerPauseAll( void )
{
	U16	i;

	for( i = 0; i < app_timer_data.usbMaxluns; i ++ ) {
		app_timer_data.usbLUNtimer[ i ]	=	0;
	}
}

void timer_usbLUNtimerStop( U16 tno )
{
	U16	tmp	=	0x01;

	DBG_assert( tno < USB_MSC_MAX_TIMER );
	
	tmp <<= tno;
	
	app_timer_data.usbLUNtimerRun	&=	~tmp;	
	app_timer_data.usbLUNtimer[ tno ]	=	0x0;
}

void timer_usbLUNtimerStopAll( void )
{
	app_timer_data.usbLUNtimerRun	=	0x00;
	memset( app_timer_data.usbLUNtimer, 0, sizeof(app_timer_data.usbLUNtimer) );
}

void timer_usbLUNdetTimerSet( U16 ms )
{
	app_timer_data.usbLunDetTimer = ms/SYSTEM_TIMER_PERIOD_MS + 1;
}

void timer_usbLUNdetTimerStop( void )
{

}
#endif //USB_HOST_ENABLE



#ifdef USB_HOST_ENABLE
void mscClass_isendLUNpollTimeOutIRP( U32 luns );
void mscClass_sendLUNpollTimeOutIRP( U32 luns );
void usbLUNtimerTick( void )
{
	U16	i;
	U32	tmp = 0x01;
	U32	luns = 0;
	U8	timeout = FALSE;

	for( i = 0; i < app_timer_data.usbMaxluns; i ++ ) {
		if( (app_timer_data.usbLUNtimerRun & tmp) && (app_timer_data.usbLUNtimer[ i ] > 0) ) {
			if( -- app_timer_data.usbLUNtimer[ i ] == 0 ) {
				timeout	=	TRUE;
				luns	|=	tmp;
				app_timer_data.usbLUNtimer[ i ]	=	app_timer_data.usbLUNtime;
			}
		}
		
		tmp <<= 1;
	}

	if( timeout )	mscClass_sendLUNpollTimeOutIRP( luns );
}
#endif

#ifdef FOR_ESD_PROTECT
void app_timer_esdProtectTimer_time_set (U16 time)
{
	app_timer_data.esdProtectTimer = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

U16 app_timer_esdProtectTimer_time_get (void)
{
	return (app_timer_data.esdProtectTimer);
}
#endif


#ifdef HDMI_ENABLE

#ifdef HDMI_INT_CHECKTOUT_USED_TIMER

void app_timer_hdmi_int_finish_wait_time_set(U32 time)
{
	DBG_Printf("hdmi_int_wait_time_set1\n\r", __func__);

	app_timer_data.hdmi_int_finish_wait_timer = (U16) ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_hdmi_int_finish_wait_time_set_from_isr(U32 time)
{
	DBG_iPrintf("hdmi_int_wait_time_set2\n\r", __func__);
	
	app_timer_data.hdmi_int_finish_wait_timer = (U16) ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

U32 app_timer_hdmi_int_wait_time_get(void)
{
	return app_timer_data.hdmi_int_finish_wait_timer;
}

void app_timer_hdmi_int_wait_time_clr(void)
{
	app_timer_data.hdmi_int_finish_wait_timer = 0;
}

#endif //HDMI_INT_CHECKTOUT_USED_TIMER

#endif //HDMI_ENABLE



#ifdef ARC_ENABLE

#ifdef ARC_PLUG_TIMER_ENABLE
void app_timer_arc_cec_plug_timer_set (U16 time)
{
	//DBG_Printf("%s\n\r", __func__);
	DBG_Printf("%s=%dms\n\r", __func__, time);

	app_timer_data.arc_plug_timer = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_arc_cec_plug_timer_clr (void)
{
	DBG_Printf("%s\n\r", __func__);

	app_timer_data.arc_plug_timer = 0;
}

U16 app_timer_arc_cec_plug_timer_get (void)
{
	return (app_timer_data.arc_plug_timer);
}
#endif //ARC_PLUG_TIMER_ENABLE

#ifdef ARC_UNPLUG_TIMER_ENABLE
void app_timer_arc_cec_unplug_timer_set (U16 time)
{
	//DBG_Printf("%s\n\r", __func__);
	DBG_Printf("%s=%dms\n\r", __func__, time);

	app_timer_data.arc_unplug_timer = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_arc_cec_unplug_timer_clr (void)
{
	DBG_Printf("%s\n\r", __func__);

	app_timer_data.arc_unplug_timer = 0;
}

U16 app_timer_arc_cec_unplug_timer_get (void)
{
	return (app_timer_data.arc_unplug_timer);
}

void app_timer_arc_cec_init_start_timer_set (U16 time)
{
	//DBG_Printf("%s\n\r", __func__);
	DBG_Printf("%s=%dms\n\r", __func__, time);

	app_timer_data.cec_init_start_timer = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_arc_cec_init_start_timer_clr (void)
{
	DBG_Printf("%s\n\r", __func__);

	app_timer_data.cec_init_start_timer = 0;
}

U16 app_timer_arc_cec_init_start_timer_get (void)
{
	return (app_timer_data.cec_init_start_timer);
}
#endif //ARC_UNPLUG_TIMER_ENABLE


#ifdef OPTEK_ARC_CEC_ENABLE
void app_timer_cec_reject_com_time_set (U16 time)
{
	app_timer_data.cec_rejectcom_time = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_cec_reject_com_time_clr (void)
{
	app_timer_data.cec_rejectcom_time = 0;
}

U16 app_timer_cec_reject_com_time_get (void)
{
	return (app_timer_data.cec_rejectcom_time);
}

void app_timer_cec_standby_wait_time_set (U16 time)
{
	//DBG_Printf("%s\n\r", __func__);
	DBG_Printf("%s=%dms\n\r", __func__, time);

	app_timer_data.cec_standby_wait_time = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_cec_standby_wait_time_clr (void)
{
	DBG_Printf("%s\n\r", __func__);

	app_timer_data.cec_standby_wait_time = 0;
}

U16 app_timer_cec_standby_wait_time_get (void)
{
	return (app_timer_data.cec_standby_wait_time);
}

void app_timer_cec_standby_time_set (U16 time)
{
	//DBG_Printf("%s\n\r", __func__);
	DBG_Printf("%s=%dms\n\r", __func__, time);

	app_timer_data.cec_standby_time = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_cec_standby_time_clr (void)
{
	DBG_Printf("%s\n\r", __func__);
	app_timer_data.cec_standby_time = 0;
}

U16 app_timer_cec_standby_time_get (void)
{
	return (app_timer_data.cec_standby_time);
}

void app_timer_cee_initiate_wait_time_set (U16 time)
{
	//DBG_Printf("%s\n\r", __func__);
	DBG_Printf("%s=%dms\n\r", __func__, time);

	app_timer_data.cec_initiate_wait_time = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_cee_initiate_wait_time_clr (void)
{
	DBG_Printf("%s\n\r", __func__);

	app_timer_data.cec_initiate_wait_time = 0;
}

U16 app_timer_cee_initiate_wait_time_get (void)
{
	return (app_timer_data.cec_initiate_wait_time);
}

void app_timer_cec_handle_enable_timr_set (U16 time)
{
	//DBG_Printf("%s\n\r", __func__);
	DBG_Printf("%s=%dms\n\r", __func__, time);

	app_timer_data.cec_handle_enable_time = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_cec_handle_enable_timr_clr (void)
{
	DBG_Printf("%s\n\r", __func__);

	app_timer_data.cec_handle_enable_time = 0;
}

U16 app_timer_cec_handle_enable_timr_get (void)
{
	return (app_timer_data.cec_handle_enable_time);
}
#endif //OPTEK_ARC_CEC_ENABLE

#endif //ARC_ENABLE


#ifdef SPI_DMA_RX_TEST
void app_timer_spi_dma_timer_set (U16 time)
{
	//DBG_Printf("%s\n\r", __func__);
	DBG_Printf("%s=%dms\n\r", __func__, time);

	app_timer_data.spi_dma_timer = ( (time + SYSTEM_TIMER_PERIOD_MS - 1) / SYSTEM_TIMER_PERIOD_MS );
}

void app_timer_spi_dma_timer_clr (void)
{
	DBG_Printf("%s\n\r", __func__);

	app_timer_data.spi_dma_timer = 0;
}

U16 app_timer_spi_dma_timer_get (void)
{
	return (app_timer_data.spi_dma_timer);
}
#endif //SPI_DMA_RX_TEST
