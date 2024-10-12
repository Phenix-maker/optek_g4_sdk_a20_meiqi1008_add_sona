#include "os_config.h"

#ifdef HDMI_ENABLE
#include "hw_timer.h"

#include "sdk_com.h"
#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"
#include "system_task.h"
#include "system_timer.h"

#ifdef SPDIF_ENABLE
#include "spdif_data_parse.h"
#include "spdif_process.h"
#endif

#include "audio_hdmi_input.h"
#include "Audio_Device.h"
#include "hdmi_process.h"
#include "ITEMT_Sys.h"
#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif

#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif


extern const WINDOW hdmi_window;

/**
 ****************************************************************************************
**/
U32 app_window_hdmi_source_key_down(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_hdmi_source_key_up(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_hdmi_source_key_hold(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_hdmi_key_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    return 300;
}


U32 app_window_hdmi_key_up(KEY_HANDLE_PARAM_STRU param)
{   
    printf_info("key 0 up, combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);
    
    if (param.combo_times == 0)
    {
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {
        #ifdef HDMI_ENABLE
		hdmi_start_set(TRUE);
        #endif

        printf_info("pass the key twice\n\r");
    }
    else if(param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");    
    }
    
    return 0;
}

U32 app_window_hdmi_key_test_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 12 down,combo timers:%d\r\n", param.combo_times);

    return 0;
}

U32 app_window_hdmi_key_test_up(KEY_HANDLE_PARAM_STRU param)
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


U32 app_window_hdmi_key_hold(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 hold,combo timers:%d, hold times:%d\r\n", param.combo_times, param.hold_times);
    
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
*******         HDMI window SET         ********
***********************************************/

static const APP_NAV_KEY APP_NAV_HDMI_KEY [] = 
{
    {APP_KEY_PLAY, app_window_hdmi_key_down, app_window_hdmi_key_up, 800, app_window_hdmi_key_hold},
   
    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

    {APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    
};

void app_window_hdmi_display(void)
{
    printf_info("enter hdmi window\n\r");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_source(global_vari.audio_source);
    #endif

}

static void app_hdmi_window_timer(void)
{ 
    app_window_time_start ();

    U16 hdmi_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (hdmi_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

    static U8 data_type_bck;
    U8 local_data_type = spdif_in_stream_data_type_get();

	if (data_type_bck != local_data_type)
    {
        data_type_bck = local_data_type;
        switch (local_data_type)
        {
        case  SPDIF_STREAM_AC3:
            #ifdef DISPLAY_MODULE_ENABLE
            app_display_module.disp_decode_type(dispDeCode_DD_AC3);
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
}

const WINDOW hdmi_window = {
    "hdmi window",
    app_window_hdmi_display,          //draw region
    KEY_TABLE(APP_NAV_HDMI_KEY),
    1000,                            //window time
    app_hdmi_window_timer,           //window timer handler	
    WINDOW_RESERVES
};


void app_hdmi_source_entry(void)
{
    DBG_Printf("\n\r%s\n\r", __func__);

   	decode_exit_req_set();

    global_vari.audio_source = SOURCE_HDMI;

    #ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
    app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
    #endif

    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
    hdmiRx_Dma_Opened = FALSE;
	#endif
    
    app_nav_window_set( &hdmi_window, 0);

	audio_input_rx_stream_config();
    audio_hdmi_input_rx_init();

	spdif_in_stream.detect_status = STREAM_SPDIF_SR_DETECT;
    spdif_in_stream.data_type = SPDIF_STREAM_UNKNOWN;
}

void app_source_convert_to_hdmi(void)
{
    if (global_vari.audio_source != SOURCE_HDMI)
    {
        app_source_finish();

        app_hdmi_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_source_hdmi_exit(void)
{
    audio_hdmi_input_rx_disable();
}

const APP_SOURCE_STRU hdmi_source = {
    "hdmi source",
    SOURCE_HDMI,
    app_source_convert_to_hdmi,
    app_source_hdmi_exit
};

#endif //HDMI_ENABLE
