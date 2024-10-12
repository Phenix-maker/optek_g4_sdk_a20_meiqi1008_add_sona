#include "os_config.h"
#include "regmap.h"

#include "window.h"
#include "app_window.h"
#include "Audio_Device.h"

#ifdef BT_VOLUME_SET
#include "bt_common.h"
#endif

#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif

#define APP_AUDIO_DEV_ENABLE
#define DBG_Printf  myprintf


#ifdef AUDIO_PROCESSOR
#ifdef AUDIO_MA12070P
#include "Audio_MA12070P.h"
#if (AMP_DEV_COUNT_MAX == 1)
const AUDIO_DEVICE AudioAmpDev[AMP_DEV_COUNT_MAX] = {AUDIO_DEV_MA12070P};
#else
const AUDIO_DEVICE AudioAmpDev[AMP_DEV_COUNT_MAX] = {{AUDIO_DEV_MA12070P},{AUDIO_DEV_MA12070P_R}};
#endif
#elif defined AUDIO_NTP8928
const AUDIO_DEVICE AudioAmpDev[AMP_DEV_COUNT_MAX] = {AUDIO_DEV_NTP8928};
#elif defined AUDIO_ACM8625
#include "amp_acm8625.h"
const AUDIO_DEVICE AudioAmpDev[AMP_DEV_COUNT_MAX] = {AUDIO_DEV_ACM8625};
#elif defined AUDIO_AMP_TAS5828M
#include "Audio_TAS5828.h"
const AUDIO_DEVICE AudioAmpDev[AMP_DEV_COUNT_MAX] = {AUDIO_AMP_DEV_TAS5828M};
#elif defined AUDIO_AMP_TAS6582
#include "Audio_TAS6582.h"
const AUDIO_DEVICE AudioAmpDev[AMP_DEV_COUNT_MAX] = {AUDIO_AMP_DEV_TAS6582};
#else
const AUDIO_DEVICE AudioAmpDev[AMP_DEV_COUNT_MAX] = {AUDIO_DEV_NONE};
#endif
#ifdef AUDIO_OPTEK_DAPP
#include "Audio_optek_dapp.h"
const AUDIO_DEVICE AUDIOdevice = {AUDIO_DEV_OPTEK_DAPP};
#else
const AUDIO_DEVICE AUDIOdevice = {AUDIO_DEV_NONE};
#endif

void Audio_Dev_None_Init(void)
{
	DBG_Printf("%s\n\r",__func__);
}

void Audio_Dev_None_Open(void)
{
	DBG_Printf("%s\n\r",__func__);
}

void Audio_Dev_None_Set_Channel(U8 channel)
{
	DBG_Printf("%s\n\r",__func__);
}

void Audio_Dev_None_Set_Volume(U8 volume)
{
	DBG_Printf("%s\n\r",__func__);
}

void Audio_Dev_None_Set_Speaker(U8 speaker, U8 attenuation)
{
	DBG_Printf("%s\n\r",__func__);
}

void Audio_Dev_None_Set_Bass_Treble(U8 bass, U8 treble)
{
	DBG_Printf("%s\n\r",__func__);
}

void Audio_Dev_None_Set_eq_mode(U8 mode)
{
	DBG_Printf("%s\n\r",__func__);
}

void Audio_Dev_None_Set_Mute(U8 con)
{
	DBG_Printf("%s\n\r",__func__);
}

void Audio_Dev_None_Set_AM_Mode(U8 con, U32 kFreq)
{
	DBG_Printf("%s\n\r",__func__);
}

#endif


AUDIO_VARI_STRU audio_vari;

void OutputStatusSetCH_sel_function(U8 output_ch, OUTPUT_CH_STATUS_SET *OutputStatusSetCH)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef APP_AUDIO_DEV_ENABLE
	switch (output_ch)
	{
	#if (AUDIO_OUT_STREAM_CHANNEL == 2)	
	case 2:
		/*L/R*/
		OutputStatusSetCH[0].OutputCh_en = TRUE;
		OutputStatusSetCH[0].OutputCh_index = OUTPUT_CH_0_INDEX;

		OutputStatusSetCH[1].OutputCh_en = TRUE;
		OutputStatusSetCH[1].OutputCh_index = OUTPUT_CH_1_INDEX;		
		break;
	#endif

	#if (AUDIO_OUT_STREAM_CHANNEL == 4)
	case 4:
		/*L/R/center/sub*/
		OutputStatusSetCH[0].OutputCh_en = TRUE;
		OutputStatusSetCH[0].OutputCh_index = OUTPUT_CH_0_INDEX;

		OutputStatusSetCH[1].OutputCh_en = TRUE;
		OutputStatusSetCH[1].OutputCh_index = OUTPUT_CH_1_INDEX;

		OutputStatusSetCH[2].OutputCh_en = TRUE;
		OutputStatusSetCH[2].OutputCh_index = OUTPUT_CH_2_INDEX;

		OutputStatusSetCH[3].OutputCh_en = TRUE;
		OutputStatusSetCH[3].OutputCh_index = OUTPUT_CH_3_INDEX;		
		break;
	#endif

	#if (AUDIO_OUT_STREAM_CHANNEL == 6)		
	case 6:
		/*L/R*/
		OutputStatusSetCH[0].OutputCh_en = TRUE;
		OutputStatusSetCH[0].OutputCh_index = OUTPUT_CH_0_INDEX;

		OutputStatusSetCH[1].OutputCh_en = TRUE;
		OutputStatusSetCH[1].OutputCh_index = OUTPUT_CH_1_INDEX;

		OutputStatusSetCH[2].OutputCh_en = TRUE;
		OutputStatusSetCH[2].OutputCh_index = OUTPUT_CH_2_INDEX;

		OutputStatusSetCH[3].OutputCh_en = TRUE;
		OutputStatusSetCH[3].OutputCh_index = OUTPUT_CH_3_INDEX;		

		OutputStatusSetCH[4].OutputCh_en = TRUE;
		OutputStatusSetCH[4].OutputCh_index = OUTPUT_CH_4_INDEX;

		OutputStatusSetCH[5].OutputCh_en = TRUE;
		OutputStatusSetCH[5].OutputCh_index = OUTPUT_CH_5_INDEX;		
		break;
	#endif

	#if (AUDIO_OUT_STREAM_CHANNEL == 8)		
	case 8:
		/*L/R*/
		OutputStatusSetCH[0].OutputCh_en = TRUE;
		OutputStatusSetCH[0].OutputCh_index = OUTPUT_CH_0_INDEX;

		OutputStatusSetCH[1].OutputCh_en = TRUE;
		OutputStatusSetCH[1].OutputCh_index = OUTPUT_CH_1_INDEX;

		OutputStatusSetCH[2].OutputCh_en = TRUE;
		OutputStatusSetCH[2].OutputCh_index = OUTPUT_CH_2_INDEX;

		OutputStatusSetCH[3].OutputCh_en = TRUE;
		OutputStatusSetCH[3].OutputCh_index = OUTPUT_CH_3_INDEX;		

		OutputStatusSetCH[4].OutputCh_en = TRUE;
		OutputStatusSetCH[4].OutputCh_index = OUTPUT_CH_4_INDEX;

		OutputStatusSetCH[5].OutputCh_en = TRUE;
		OutputStatusSetCH[5].OutputCh_index = OUTPUT_CH_5_INDEX;		

		OutputStatusSetCH[6].OutputCh_en = TRUE;
		OutputStatusSetCH[6].OutputCh_index = OUTPUT_CH_6_INDEX;

		OutputStatusSetCH[7].OutputCh_en = TRUE;
		OutputStatusSetCH[7].OutputCh_index = OUTPUT_CH_7_INDEX;		
		break;
	#endif

	#if (AUDIO_OUT_STREAM_CHANNEL == 10)		
	case 10:
		/*L/R*/
		OutputStatusSetCH[0].OutputCh_en = TRUE;
		OutputStatusSetCH[0].OutputCh_index = OUTPUT_CH_0_INDEX;

		OutputStatusSetCH[1].OutputCh_en = TRUE;
		OutputStatusSetCH[1].OutputCh_index = OUTPUT_CH_1_INDEX;

		OutputStatusSetCH[2].OutputCh_en = TRUE;
		OutputStatusSetCH[2].OutputCh_index = OUTPUT_CH_2_INDEX;

		OutputStatusSetCH[3].OutputCh_en = TRUE;
		OutputStatusSetCH[3].OutputCh_index = OUTPUT_CH_3_INDEX;		

		OutputStatusSetCH[4].OutputCh_en = TRUE;
		OutputStatusSetCH[4].OutputCh_index = OUTPUT_CH_4_INDEX;

		OutputStatusSetCH[5].OutputCh_en = TRUE;
		OutputStatusSetCH[5].OutputCh_index = OUTPUT_CH_5_INDEX;		

		OutputStatusSetCH[6].OutputCh_en = TRUE;
		OutputStatusSetCH[6].OutputCh_index = OUTPUT_CH_6_INDEX;

		OutputStatusSetCH[7].OutputCh_en = TRUE;
		OutputStatusSetCH[7].OutputCh_index = OUTPUT_CH_7_INDEX;		

		OutputStatusSetCH[8].OutputCh_en = TRUE;
		OutputStatusSetCH[8].OutputCh_index = OUTPUT_CH_8_INDEX;

		OutputStatusSetCH[9].OutputCh_en = TRUE;
		OutputStatusSetCH[9].OutputCh_index = OUTPUT_CH_9_INDEX;		
		break;
	#endif

	#if (AUDIO_OUT_STREAM_CHANNEL == 12)	
	case 12:
		/*L/R*/
		OutputStatusSetCH[0].OutputCh_en = TRUE;
		OutputStatusSetCH[0].OutputCh_index = OUTPUT_CH_0_INDEX;

		OutputStatusSetCH[1].OutputCh_en = TRUE;
		OutputStatusSetCH[1].OutputCh_index = OUTPUT_CH_1_INDEX;

		OutputStatusSetCH[2].OutputCh_en = TRUE;
		OutputStatusSetCH[2].OutputCh_index = OUTPUT_CH_2_INDEX;

		OutputStatusSetCH[3].OutputCh_en = TRUE;
		OutputStatusSetCH[3].OutputCh_index = OUTPUT_CH_3_INDEX;		

		OutputStatusSetCH[4].OutputCh_en = TRUE;
		OutputStatusSetCH[4].OutputCh_index = OUTPUT_CH_4_INDEX;

		OutputStatusSetCH[5].OutputCh_en = TRUE;
		OutputStatusSetCH[5].OutputCh_index = OUTPUT_CH_5_INDEX;		

		OutputStatusSetCH[6].OutputCh_en = TRUE;
		OutputStatusSetCH[6].OutputCh_index = OUTPUT_CH_6_INDEX;

		OutputStatusSetCH[7].OutputCh_en = TRUE;
		OutputStatusSetCH[7].OutputCh_index = OUTPUT_CH_7_INDEX;		

		OutputStatusSetCH[8].OutputCh_en = TRUE;
		OutputStatusSetCH[8].OutputCh_index = OUTPUT_CH_8_INDEX;

		OutputStatusSetCH[9].OutputCh_en = TRUE;
		OutputStatusSetCH[9].OutputCh_index = OUTPUT_CH_9_INDEX;		

		OutputStatusSetCH[10].OutputCh_en = TRUE;
		OutputStatusSetCH[10].OutputCh_index = OUTPUT_CH_10_INDEX;

		OutputStatusSetCH[11].OutputCh_en = TRUE;
		OutputStatusSetCH[11].OutputCh_index = OUTPUT_CH_11_INDEX;		
		break;
	#endif
	
	#if (AUDIO_OUT_STREAM_CHANNEL == 14)
	case 14:
		/*L/R*/
		OutputStatusSetCH[0].OutputCh_en = TRUE;
		OutputStatusSetCH[0].OutputCh_index = OUTPUT_CH_0_INDEX;

		OutputStatusSetCH[1].OutputCh_en = TRUE;
		OutputStatusSetCH[1].OutputCh_index = OUTPUT_CH_1_INDEX;		

		OutputStatusSetCH[2].OutputCh_en = TRUE;
		OutputStatusSetCH[2].OutputCh_index = OUTPUT_CH_2_INDEX;

		OutputStatusSetCH[3].OutputCh_en = TRUE;
		OutputStatusSetCH[3].OutputCh_index = OUTPUT_CH_3_INDEX;		

		OutputStatusSetCH[4].OutputCh_en = TRUE;
		OutputStatusSetCH[4].OutputCh_index = OUTPUT_CH_4_INDEX;

		OutputStatusSetCH[5].OutputCh_en = TRUE;
		OutputStatusSetCH[5].OutputCh_index = OUTPUT_CH_5_INDEX;		

		OutputStatusSetCH[6].OutputCh_en = TRUE;
		OutputStatusSetCH[6].OutputCh_index = OUTPUT_CH_6_INDEX;

		OutputStatusSetCH[7].OutputCh_en = TRUE;
		OutputStatusSetCH[7].OutputCh_index = OUTPUT_CH_7_INDEX;		

		OutputStatusSetCH[8].OutputCh_en = TRUE;
		OutputStatusSetCH[8].OutputCh_index = OUTPUT_CH_8_INDEX;

		OutputStatusSetCH[9].OutputCh_en = TRUE;
		OutputStatusSetCH[9].OutputCh_index = OUTPUT_CH_9_INDEX;		

		OutputStatusSetCH[10].OutputCh_en = TRUE;
		OutputStatusSetCH[10].OutputCh_index = OUTPUT_CH_10_INDEX;

		OutputStatusSetCH[11].OutputCh_en = TRUE;
		OutputStatusSetCH[11].OutputCh_index = OUTPUT_CH_11_INDEX;		

		OutputStatusSetCH[12].OutputCh_en = TRUE;
		OutputStatusSetCH[12].OutputCh_index = OUTPUT_CH_12_INDEX;

		OutputStatusSetCH[13].OutputCh_en = TRUE;
		OutputStatusSetCH[13].OutputCh_index = OUTPUT_CH_13_INDEX;		
		break;
	#endif

	#if (AUDIO_OUT_STREAM_CHANNEL == 16)
	case 16:
		/*L/R*/
		OutputStatusSetCH[0].OutputCh_en = TRUE;
		OutputStatusSetCH[0].OutputCh_index = OUTPUT_CH_0_INDEX;

		OutputStatusSetCH[1].OutputCh_en = TRUE;
		OutputStatusSetCH[1].OutputCh_index = OUTPUT_CH_1_INDEX;		

		OutputStatusSetCH[2].OutputCh_en = TRUE;
		OutputStatusSetCH[2].OutputCh_index = OUTPUT_CH_2_INDEX;

		OutputStatusSetCH[3].OutputCh_en = TRUE;
		OutputStatusSetCH[3].OutputCh_index = OUTPUT_CH_3_INDEX;		

		OutputStatusSetCH[4].OutputCh_en = TRUE;
		OutputStatusSetCH[4].OutputCh_index = OUTPUT_CH_4_INDEX;

		OutputStatusSetCH[5].OutputCh_en = TRUE;
		OutputStatusSetCH[5].OutputCh_index = OUTPUT_CH_5_INDEX;		

		OutputStatusSetCH[6].OutputCh_en = TRUE;
		OutputStatusSetCH[6].OutputCh_index = OUTPUT_CH_6_INDEX;

		OutputStatusSetCH[7].OutputCh_en = TRUE;
		OutputStatusSetCH[7].OutputCh_index = OUTPUT_CH_7_INDEX;		

		OutputStatusSetCH[8].OutputCh_en = TRUE;
		OutputStatusSetCH[8].OutputCh_index = OUTPUT_CH_8_INDEX;

		OutputStatusSetCH[9].OutputCh_en = TRUE;
		OutputStatusSetCH[9].OutputCh_index = OUTPUT_CH_9_INDEX;		

		OutputStatusSetCH[10].OutputCh_en = TRUE;
		OutputStatusSetCH[10].OutputCh_index = OUTPUT_CH_10_INDEX;

		OutputStatusSetCH[11].OutputCh_en = TRUE;
		OutputStatusSetCH[11].OutputCh_index = OUTPUT_CH_11_INDEX;		

		OutputStatusSetCH[12].OutputCh_en = TRUE;
		OutputStatusSetCH[12].OutputCh_index = OUTPUT_CH_12_INDEX;

		OutputStatusSetCH[13].OutputCh_en = TRUE;
		OutputStatusSetCH[13].OutputCh_index = OUTPUT_CH_13_INDEX;		

		OutputStatusSetCH[14].OutputCh_en = TRUE;
		OutputStatusSetCH[14].OutputCh_index = OUTPUT_CH_14_INDEX;

		OutputStatusSetCH[15].OutputCh_en = TRUE;
		OutputStatusSetCH[15].OutputCh_index = OUTPUT_CH_15_INDEX;		
		break;
	#endif
	default:
		printf_error("channel status set");
		break;
	}
	#endif	
}
void app_audio_device_init(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef APP_AUDIO_DEV_ENABLE
	int i;
	audio_vari.volume = VOLUME_DEFAULT;
	audio_vari.bass = BASS_DEFAULT;
	audio_vari.treble = TREBLE_DEFAULT;
	audio_vari.mute = FALSE;
	audio_vari.eq_mode = EQ_DEFAULT;

	//OutputStatusSetCH_sel_function(AUDIO_OUT_STREAM_CHANNEL, audio_vari.OutputStatusSetCH);
	#endif
}

void app_audio_device_open(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef APP_AUDIO_DEV_ENABLE

	#endif
}


void VolMuteSwitch(void)
{
	#ifdef APP_AUDIO_DEV_ENABLE
	if (audio_vari.volume == 0)
	{
		#ifdef AUDIO_CODEC_USED_MUTE
		AUDIO_DEV_CODEC_MUTE_ON;
		#endif
		#ifdef AUDIO_AMP_USED_MUTE
		AUDIO_DEV_AMP_MUTE_ON;
		#endif
	}
	else
	{
		#ifdef AUDIO_CODEC_USED_MUTE
		AUDIO_DEV_CODEC_MUTE_OFF;
		#endif
		#ifdef AUDIO_AMP_USED_MUTE
		AUDIO_DEV_AMP_MUTE_OFF;
		#endif
	}
	#endif
}

void app_audio_device_volume_set(U8 con)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef APP_AUDIO_DEV_ENABLE
	U8 vol_changed = FALSE;
	int i;

	if (con)
	{
		if (audio_vari.volume < VOLUME_MAX)
		{
			audio_vari.volume++;
			vol_changed = TRUE;
		}
		else
		{
			audio_vari.volume = VOLUME_MAX;
		}
	}
	else
	{
		if (audio_vari.volume > VOLUME_MIN)
		{
			audio_vari.volume--;
			vol_changed = TRUE;
		}
		else
		{
			audio_vari.volume = VOLUME_MIN;
		}
	}


	#ifdef DISPLAY_MODULE_ENABLE
	if (app_window_current_get() != (WINDOW *)&volume_window)
	{
		app_nav_window_set( &volume_window, 1);
	}
	else
	{
		app_display_module.disp_volume_status(audio_vari.volume);
		app_window_timer_count_init();
	}
	#endif

	if (vol_changed)
	{
		#ifdef AUDIO_CODEC_USED_VOL
		if (global_vari.audio_source == SOURCE_ARC) 
		{
			if( audio_vari.mute == FALSE)
			{
				AUDIOdevice.Set_Volume(audio_vari.volume);
			}
			
		}
		else
		{
			AUDIOdevice.Set_Volume(audio_vari.volume);
		}
		#endif
		#ifdef AUDIO_AMP_USED_VOL
		for (i=0;i<AMP_DEV_COUNT_MAX;i++)
		{
			AudioAmpDev[i].Set_Volume(audio_vari.volume);
		}
		#endif

		#ifdef AUDIO_CODEC_USED_BASS_TREBLE
		AUDIOdevice.Set_Bass_Treble(audio_vari.bass, audio_vari.treble);
		#endif
		#ifdef AUDIO_AMP_USED_BASS_TREBLE
		for (i=0;i<AMP_DEV_COUNT_MAX;i++)
		{
			AudioAmpDev[i].Set_Bass_Treble(audio_vari.bass, audio_vari.treble);
		}		
		#endif

		#ifdef BT_VOLUME_SET
		if (global_vari.audio_source == SOURCE_BT)
		{		
			app_nav_bt_avrcp_vol();
		}
		#endif

		#ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
		app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
		#endif

	}
	// if (global_vari.audio_source == SOURCE_ARC && audio_vari.volume == 1 && audio_vari.mute == TRUE)//
	// {
	// 	// VolMuteSwitch();
	// }

	#if ((defined CEC_SYNC_STATUS_MUTE)||(defined CEC_SYNC_STATUS_VOLUME))
	if (global_vari.audio_source == SOURCE_ARC)
	{
		cec_report_audio_volume_mute_status();
	}
	#endif

	// VolMuteSwitch();

	// #if ((defined CEC_SYNC_STATUS_MUTE)||(defined CEC_SYNC_STATUS_VOLUME))//
	// if (global_vari.audio_source == SOURCE_ARC && audio_vari.volume == 0)
	// {
	// 	cec_report_audio_volume_mute_status();
	// }
	// #endif
	DBG_Printf("volume = %d\r\n",audio_vari.volume);
	
	#endif

}

U32 app_audio_device_volume_plus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		app_audio_device_volume_set(TRUE);
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

U32 app_audio_device_volume_minus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		app_audio_device_volume_set(FALSE);
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

void app_audio_device_bass_set(U8 con)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef APP_AUDIO_DEV_ENABLE
	U8 bass_changed = FALSE;
	int i;

	if (con)
	{
		if (audio_vari.bass < BASS_MAX)
		{
			audio_vari.bass++;
			bass_changed = TRUE;
		}
		else
		{
			audio_vari.bass = BASS_MAX;
		}
	}
	else
	{
		if (audio_vari.bass > BASS_MIN)
		{
			audio_vari.bass--;
			bass_changed = TRUE;
		}
		else
		{
			audio_vari.bass = BASS_MIN;
		}
	}

	if (bass_changed)
	{
		#ifdef AUDIO_CODEC_USED_BASS_TREBLE
		AUDIOdevice.Set_Bass_Treble(audio_vari.bass, audio_vari.treble);
		#endif
		#ifdef AUDIO_AMP_USED_BASS_TREBLE
		for (i=0;i<AMP_DEV_COUNT_MAX;i++)
		{
			AudioAmpDev[i].Set_Bass_Treble(audio_vari.bass, audio_vari.treble);
		}		
		#endif

		#ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
		app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
		#endif
	}
	#endif
}

U32 app_audio_device_bass_plus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		app_audio_device_bass_set(TRUE);
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

U32 app_audio_device_bass_minus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		app_audio_device_bass_set(FALSE);
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

void app_audio_device_treble_set(U8 con)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef APP_AUDIO_DEV_ENABLE
	U8 treble_changed = FALSE;
	int i;

	if (con)
	{
		if (audio_vari.treble < BASS_MAX)
		{
			audio_vari.treble++;
			treble_changed = TRUE;
		}
		else
		{
			audio_vari.treble = BASS_MAX;
		}
	}
	else
	{
		if (audio_vari.treble > BASS_MIN)
		{
			audio_vari.treble--;
			treble_changed = TRUE;
		}
		else
		{
			audio_vari.treble = BASS_MIN;
		}
	}

	if (treble_changed)
	{
		#ifdef AUDIO_CODEC_USED_BASS_TREBLE
		AUDIOdevice.Set_Bass_Treble(audio_vari.bass, audio_vari.treble);
		#endif
		#ifdef AUDIO_AMP_USED_BASS_TREBLE
		for (i=0;i<AMP_DEV_COUNT_MAX;i++)
		{
			AudioAmpDev[i].Set_Bass_Treble(audio_vari.bass, audio_vari.treble);
		}		
		#endif

		#ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
		app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
		#endif		
	}
	#endif
}

U32 app_audio_device_treble_plus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		app_audio_device_treble_set(TRUE);
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

U32 app_audio_device_treble_minus(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		app_audio_device_treble_set(FALSE);
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

const AUDIO_EQ_STRU eq_flat_mode = {
    "EQ FLAT mode",
    EQ_FLAT,
};

const AUDIO_EQ_STRU eq_classic_mode = {
    "EQ CLASSIC mode",
    EQ_CLASSIC,
};

const AUDIO_EQ_STRU eq_rock_mode = {
    "EQ ROCK mode",
    EQ_ROCK,
};

const AUDIO_EQ_STRU eq_jazz_mode = {
    "EQ JAZZ mode",
    EQ_JAZZ,
};

const AUDIO_EQ_STRU eq_pop_mode = {
    "EQ POP mode",
    EQ_POP,
};

const AUDIO_EQ_STRU *EQ_mode_for_sw[] =
{
	&eq_flat_mode,
	&eq_classic_mode,
	&eq_rock_mode,
	&eq_jazz_mode,
	&eq_pop_mode
};

void app_eq_mode_next_enter(void)
{
	int i;
	int loop;

	loop = sizeof(EQ_mode_for_sw)/sizeof(EQ_mode_for_sw[0]);
	DBG_Printf("%d;%d;%d\n\r", loop, sizeof(EQ_mode_for_sw), sizeof(EQ_mode_for_sw[0]));

	if (loop > 0)
	{
		for (i=0; i<loop; i++)
		{
			if (audio_vari.eq_mode == EQ_mode_for_sw[i]->id)
			{
				DBG_Printf("%d;%s;%d\n\r", audio_vari.eq_mode, EQ_mode_for_sw[i]->string, EQ_mode_for_sw[i]->id);
				if (i<(loop-1))
				{
					DBG_Printf("%d;%s;%d\n\r", audio_vari.eq_mode, EQ_mode_for_sw[i+1]->string, EQ_mode_for_sw[i+1]->id);
					audio_vari.eq_mode = EQ_mode_for_sw[i+1]->id;
				}
				else
				{
					DBG_Printf("%d;%s;%d\n\r", audio_vari.eq_mode, EQ_mode_for_sw[0]->string, EQ_mode_for_sw[0]->id);
					audio_vari.eq_mode = EQ_mode_for_sw[0]->id;
				}
				break;
			}
		}		
	}
	else
	{
		audio_vari.eq_mode = EQ_DEFAULT;
	}

	#ifdef AUDIO_CODEC_USED_EQ_MODE
	AUDIOdevice.Set_Eq_Mode(audio_vari.eq_mode);
	#endif
	#ifdef AUDIO_AMP_USED_BASS_TREBLE
	for (i=0;i<AMP_DEV_COUNT_MAX;i++)
	{
		AudioAmpDev[i].Set_Eq_Mode(audio_vari.eq_mode);
	}		
	#endif
}

U32 app_audio_device_eq_mode_loop(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
		app_eq_mode_next_enter();
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

/***********************************************
*******        volume window set        ********
***********************************************/

static const APP_NAV_KEY APP_NAV_VOLUME_KEY [] = 
{
	#if 0
    {APP_KEY_PLAY, NULL, NULL, 0, NULL},

    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

    {APP_KEY_SOURCE, NULL, NULL, 0, NULL},

    {APP_KEY_EQ, NULL, app_audio_device_eq_mode_loop, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, NULL, 0, NULL},
    #endif  
	#else
	// {APP_KEY_PLAY, app_window_arc_key_down, app_window_arc_key_up, 800, app_window_arc_key_hold},

    {APP_KEY_PLAY, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_SKIPF, NULL, app_audio_device_volume_minus, 0, NULL},

	{APP_KEY_SOURCE, NULL, NULL, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif 

	#endif

};

void app_window_volume_display(void)
{
    printf_info("enter volume window\n\r");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_volume_status(audio_vari.volume);
    #endif
}

static void app_volume_window_timer(void)
{
	app_window_time_start ();

    U16 volume_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (volume_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

	app_window_timer_count_plus();

	if (app_window_timer_count_get() > 5000/app_window_time_get())
	{
		app_window_cancel();
	}
}

const WINDOW volume_window =
{
	"volume window",
	app_window_volume_display,         		//draw region
	KEY_TABLE(APP_NAV_VOLUME_KEY),
	1000,                                   //window time(ms)
	app_volume_window_timer,                //window time handler
	WINDOW_RESERVES
};
