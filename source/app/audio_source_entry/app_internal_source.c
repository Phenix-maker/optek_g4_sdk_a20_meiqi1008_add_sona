#include "os_config.h" //FreeRTOS kernel includes.

#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"
#include "app_bt_tx.h"
#include "otg.h"

#include "Audio_Device.h"

#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif
#include "bt_common.h"

#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif

#ifdef OPTEK_LINK_ENABLE
#include "optek_link_commons.h"
#endif

/**
 ****************************************************************************************
**/
U32 app_window_internal_source_key_down(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_internal_source_key_up(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_internal_source_key_hold(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_internal_key_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    return 300;
}

U32 app_window_internal_key_up(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

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

U32 app_window_internal_key_test_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 12 down,combo timers:%d\r\n", param.combo_times);

    return 0;
}

U32 app_window_internal_key_test_up(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

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


U32 app_window_internal_key_hold(KEY_HANDLE_PARAM_STRU param)
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
*******      Internal window SET        ********
***********************************************/

static const APP_NAV_KEY APP_NAV_INTERNAL_KEY [] = 
{
    #ifdef OPTEK_LINK_ENABLE
    {APP_KEY_PLAY, app_nav_optek_link_pairing, NULL, 0, NULL},
    #endif
    #ifdef BT_TX_ROLE_ENABLE
    {APP_KEY_STOP, app_nav_bt_hp_search, NULL, 0, NULL},
    #endif
    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

	{APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif
};

void app_window_internal_display(void)
{
    printf_info("internal window\r\n");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_source(global_vari.audio_source);
    #endif

}

static void app_internal_window_timer(void)
{
	app_window_time_start ();

    U16 internal_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (internal_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

}

const WINDOW internal_window = {
    "internal window",
	app_window_internal_display,        //draw region
	KEY_TABLE(APP_NAV_INTERNAL_KEY),
	1000,                               //window time
	app_internal_window_timer,          //window timer handler	
    WINDOW_RESERVES
};

void app_internal_source_entry(void)
{
	decode_exit_req_set();

    global_vari.audio_source = SOURCE_INTERNAL;

    #ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
    app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
    #endif

    app_nav_window_set( &internal_window, 0);
    
    #ifdef BTDM52_ENABLE
    #ifndef OPTEK_LINK_ENABLE
    app_bt_set_scan_enable_mode(enBT_PAGE_SCAN_ENABLE);
    #endif
    #endif

    internal_decocde_set();
    internal_decocde_start_w_init();
}

void app_source_convert_to_internal(void)
{
    DBG_Printf("\n\r%s\n\r", __func__);

    if (global_vari.audio_source != SOURCE_INTERNAL)
    {
        //global_vari.audio_source = SOURCE_INTERNAL;
        
        app_source_finish();

        app_internal_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_internal_source_exit(void)
{

}

const APP_SOURCE_STRU internal_source = {
    "internal source",
    SOURCE_INTERNAL,
    app_source_convert_to_internal,
    app_internal_source_exit
};
