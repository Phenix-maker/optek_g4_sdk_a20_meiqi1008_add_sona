#include "os_config.h"

#ifdef ARC_ENABLE
#include "hw_timer.h"
#include "hw_pll.h"
#include "hw_spdif.h"

#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"
#include "audio_arc_input.h"
#include "Audio_Device.h"
#include "app_timer.h"

#ifdef SPDIF_ENABLE
#include "spdif_process.h"
#include "spdif_data_parse.h"
#endif

#ifdef ARC_ENABLE
#include "arc_process.h"
#include "arc_detect.h"
#endif

#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif


#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif


extern const WINDOW arc_window;
/**
 ****************************************************************************************
**/
U32 app_window_arc_source_key_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    if (param.combo_times == 0)
    {
        return 300;
    }
    else if (param.combo_times == 1)
    {
        return 300;
    }
    else
    {
        //to do key handle
        return 300;
    }
}

U32 app_window_arc_source_key_up(KEY_HANDLE_PARAM_STRU param)
{   
    printf_info("key 0 up,combo tiemrs:%d,hold times:%d\r\n", param.combo_times, param.hold_times);
    
    if (param.combo_times == 0)
    {
        app_source_next_enter();
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

U32 app_window_arc_source_key_hold(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 hold,combo timers:%d,hold times:%d\r\n", param.combo_times, param.hold_times);
    
    if (param.hold_times < 10)
    {
        return 300;
    }
    else
    {
        return 0;
    }
}


/**
 ****************************************************************************************
 * @brief Function excutes at once when key pressd
 *
 * @param[in]  param    	param.combo_times
 *
 * @return 300              if the time interval between two hits is less than 300ms
 *                          it can be considered as a key combo
 ****************************************************************************************
 */
 
U32 app_window_arc_key_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    if (param.combo_times == 0)
    {
        return 300;
    }
    else if (param.combo_times == 1)
    {
        return 300;
    }
    else
    {
        //to do key handle
        return 300;
    }
}


/**
 ****************************************************************************************
 * @brief Function excutes at once when key released
 *
 * @param[in]  param    	param.combo_times
 *
 * @return 0                excute different function in different conditions
 ****************************************************************************************
 */
U32 app_window_arc_key_up(KEY_HANDLE_PARAM_STRU param)
{   
    printf_info("key 0 up,combo tiemrs:%d,hold times:%d\r\n", param.combo_times, param.hold_times);
    
    if (param.combo_times == 0)
    {
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


/**
 ****************************************************************************************
 * @brief Function excutes at once when key is holding
 *
 * @param[in]  param    	param.hold_times
 *
 * @return 300              if the time interval is less than 300ms
 *                          it can be considered as a key hold
 ****************************************************************************************
 */
U32 app_window_arc_key_hold(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 hold,combo timers:%d,hold times:%d\r\n", param.combo_times, param.hold_times);
    
    if (param.hold_times < 10)
    {
        return 300;
    }
    else
    {
        return 0;
    }
}

/***********************************************
*******         ARC window SET          ********
***********************************************/

static const APP_NAV_KEY APP_NAV_ARC_KEY [] = 
{
    // {APP_KEY_PLAY, app_window_arc_key_down, app_window_arc_key_up, 800, app_window_arc_key_hold},

    {APP_KEY_PLAY, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_SKIPF, NULL, app_audio_device_volume_minus, 0, NULL},

	{APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

};

void app_window_arc_display(void)
{
    printf_info("enter arc window\r\n");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_source(global_vari.audio_source);
    #endif

}

static void app_arc_window_timer(void)
{
	app_window_time_start ();

    U16 arc_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;
    static U8 channel_display;
    static U8 data_type_bck;
    U8 local_data_type = spdif_in_stream_data_type_get();

	if ((data_type_bck != local_data_type) || (channel_display != audio_out_stream.input_channel))
    {
        data_type_bck = local_data_type;
        switch (local_data_type)
        {
        case  SPDIF_STREAM_AC3:
            #ifdef DISPLAY_MODULE_ENABLE
            app_display_module.disp_decode_type(dispDeCode_DD_AC3);
            channel_display = audio_out_stream.input_channel;
            #endif
            break;

        case  SPDIF_STREAM_EC3:
            #ifdef DISPLAY_MODULE_ENABLE
            app_display_module.disp_decode_type(dispDeCode_DDp_EC3);
            #endif
            break;

         case  SPDIF_STREAM_DTS:
            #ifdef DISPLAY_MODULE_ENABLE
            app_display_module.disp_decode_type(dispDeCode_DTS);
            #endif
            break;

        case  SPDIF_STREAM_PCM:
            #ifdef DISPLAY_MODULE_ENABLE
            app_display_module.disp_decode_type(dispDeCode_PCM);
            #endif
            break;

        default:
            #ifdef DISPLAY_MODULE_ENABLE
            app_display_module.disp_decode_type(dispDeCode_UNKNOW);
            #endif
            break;
        }
    }
    #ifdef WINDOW_PRINTF_ENABLE
    if (arc_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

}

const WINDOW arc_window = {
    "arc window",
	app_window_arc_display,          //draw region
	KEY_TABLE(APP_NAV_ARC_KEY),
	1000,                            //window time
	app_arc_window_timer,            //window timer handler	
    WINDOW_RESERVES
};


void app_arc_source_entry(void)
{
    DBG_Printf("\n\r%s\n\r", __func__);

	decode_exit_req_set();

    global_vari.audio_source = SOURCE_ARC;

    #ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
    app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
    #endif

    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
    arcRx_Dma_Opened = FALSE;
	#endif

    app_nav_window_set( &arc_window, 0);

    #ifdef AUDIO_IN_STREAM_USED

	#ifdef ARC_SPDIF_IN_CHANNEL_SEL_0
	hw_spdif_init(SPDIF_IN_CH_0);
	hw_spdif_open(SPDIF_IN_CH_0);
	#elif defined ARC_SPDIF_IN_CHANNEL_SEL_1
	hw_spdif_init(SPDIF_IN_CH_1);
	hw_spdif_open(SPDIF_IN_CH_1);
	#else
	error Must define the input channel of arc spdif!
	#endif

	hw_pll_audio_clock_set(SF_BASE_DET, SPDIF_SF_4x);

	audio_input_rx_stream_config();
	spdif_in_stream.detect_status = STREAM_SPDIF_SR_DETECT;
    spdif_in_stream.data_type = SPDIF_STREAM_UNKNOWN;

    #else
	
    arc_decocde_set();    
    arc_decocde_start_w_init();

	#endif//AUDIO_IN_STREAM_USED

	#ifdef OPTEK_ARC_CEC_ENABLE
	if (arc_dectect_status_get() == enARC_PLUG)
	{
		app_timer_arc_cec_plug_timer_set(25);
	}
	#endif //OPTEK_ARC_CEC_ENABLE
}

void app_source_convert_to_arc(void)
{
    DBG_Printf("%s\n\r", __func__);

    if (global_vari.audio_source != SOURCE_ARC)
    {
        app_source_finish();

        app_arc_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_arc_source_exit(void)
{
    DBG_Printf("%s\n\r", __func__);
    audio_arc_input_rx_disable();

    #ifdef OPTEK_ARC_CEC_ENABLE
    #ifdef ARC_PLUG_TIMER_ENABLE
    app_timer_arc_cec_plug_timer_clr();
    #endif
    #ifdef ARC_UNPLUG_TIMER_ENABLE
    app_timer_arc_cec_unplug_timer_clr();
    #endif
    app_timer_arc_cec_init_start_timer_clr();
    cee_set_initiate_stop();
    #endif
}

const APP_SOURCE_STRU arc_source = {
    "arc source",
    SOURCE_ARC,
    app_source_convert_to_arc,
    app_arc_source_exit
};

#endif //HDMI_ENABLE
