/*
 * app_standby_source.c
 *
 *  Created on: 2024/2/21
 *      Author: Windows
 */

#include "os_config.h"

#include "window.h"
#ifdef STANDBY_SOURCE_ENABLE
#include "hw_timer.h"
#include "hw_pll.h"
#include "hw_cpu_clock.h"

#include "sdk_com.h"
#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"
#include "system_task.h"
#include "system_timer.h"
#include "otg.h"

#include "Audio_Device.h"

#include "arc_detect.h"

#include "app_media_process.h"

#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif

#include "app_standby_source.h"



/**
 ****************************************************************************************
**/

/***********************************************
*******   Standby release window SET    ********
***********************************************/

static const APP_NAV_KEY APP_NAV_STANDBY_RELEASE_KEY [] =
{

};

void app_window_standby_release_display(void)
{
    printf_info("enter release standby window\n\r");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_standby_status(FALSE);
    #endif

}

extern const WINDOW standby_release_window;

static void app_standby_release_window_timer(void)
{
	app_window_time_start ();

    U16 standby_release_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (standby_release_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

    #ifdef STANDBY_MODE_DELAY_RELEASE
    static U8 delay_temp;
    if (delay_temp++ > STANBY_RELEASE_DELAY_TIME_MS/(app_window_current_get()->window_time))
    {
        delay_temp = 0;
        global_vari.audio_source = global_vari.last_audio_source;

        enter_fix_source(global_vari.audio_source);
    }
    #endif
}

const WINDOW standby_release_window = {
    "standby release window",
	app_window_standby_release_display,         //draw region
	KEY_TABLE(APP_NAV_STANDBY_RELEASE_KEY),
	1000,                                       //window time
	app_standby_release_window_timer,           //window timer handler
    WINDOW_RESERVES
};

/**
 ****************************************************************************************
**/


U32 app_window_standby_key_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    return 300;
}

U32 app_window_standby_key_up(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_standby_key_hold(KEY_HANDLE_PARAM_STRU param)
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

U32 app_standby_relese_keyfunction(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
        app_source_convert_to_standby_release();
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

U32 app_standby_enter_keyfunction(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
        app_source_convert_to_standby_enter();
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
*******       Standby window SET        ********
***********************************************/

static const APP_NAV_KEY APP_NAV_STANDBY_KEY [] =
{
	{APP_KEY_POWER, NULL, app_standby_relese_keyfunction, 0, NULL},
};


void app_window_standby_display(void)
{
    printf_info("enter standby window\n\r");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_standby_status(TRUE);
    #endif

}

extern const WINDOW standby_window;

static void app_standby_window_timer(void)
{
	app_window_time_start ();

    U16 standby_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (standby_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_standby_status(TRUE);
    #endif
}

const WINDOW standby_window = {
    "standby window",
	app_window_standby_display,         //draw region
	KEY_TABLE(APP_NAV_STANDBY_KEY),
	1000,                               //window time
	app_standby_window_timer,           //window timer handler
    WINDOW_RESERVES
};

void app_standby_source_entry(void)
{
    DBG_Printf("\n\r%s\n\r", __func__);

	decode_exit_wait_forever_msgSend();

    global_vari.audio_source = SOURCE_STANDBY;

    app_nav_window_set( &standby_window, 0);

  	hw_pll_audio_clock_set(SF_BASE_48000, SPDIF_SF_1x);

    #ifdef CEC_SYNC_STATUS_POWER_STANBY
    //cec_report_power_on_to_power_standby();
    #endif

	// #ifdef ARC_PLUG_AT_STABNDBY_ENABLE
	// if (arc_dectect_status_get() == enARC_PLUG)
	// {
	// 	app_timer_arc_cec_plug_timer_set(25);
	// }
	// #endif //ARC_PLUG_AT_STABNDBY_ENABLE    

}

void app_source_convert_to_standby(void)
{
    if (global_vari.audio_source != SOURCE_STANDBY)
    {
        app_source_finish();

        app_standby_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_standby_source_exit(void)
{

}

const APP_SOURCE_STRU standby_source = {
    "standby source",
    SOURCE_STANDBY,
    app_source_convert_to_standby,
    app_standby_source_exit
};

void app_source_convert_to_standby_enter(void)
{

    global_vari.last_audio_source = global_vari.audio_source;

    enter_fix_source(SOURCE_STANDBY);

}

void app_source_convert_to_standby_release(void)
{
    #ifdef CEC_SYNC_STATUS_POWER_STANBY
    //cec_report_power_standby_to_power_on();
    #endif

    #ifdef STANDBY_MODE_DELAY_RELEASE
    app_nav_window_set( &standby_release_window, 0);
    #else
    global_vari.audio_source = global_vari.last_audio_source;

    enter_fix_source(global_vari.audio_source);
    #endif
}
#endif //STANDBY_SOURCE_ENABLE
U8 app_source_get_standby_mode_status(void)
{
    U8 ret = FALSE;
    if (global_vari.audio_source == SOURCE_STANDBY)
    {
        ret = TRUE;
    }
    else
    {
        ret = FALSE;
    }

    DBG_Printf("%s:audio_source = %d\n\r", __func__, global_vari.audio_source);

    return ret;

}


