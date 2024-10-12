/*
 * app_bt.c
 *
 *  Created on: 2024/1/30
 *      Author: Windows
 */
#include "os_config.h" //FreeRTOS kernel includes.

#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"
#include "app_bt_tx.h"

#include "Audio_Device.h"

#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif
#include "bt_common.h"

#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif

/***********************************************
*******    BT HP Search window SET      ********
***********************************************/
static const APP_NAV_KEY APP_NAV_BT_HP_SEARCH_KEY [] = 
{
    {APP_KEY_PLAY, NULL, NULL, 0, NULL},
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

void app_window_bt_hp_search_display(void)
{
    printf_info("bt hp search window\r\n");
    U16 bt_hp_search_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_bt_status(bt_hp_search_win_count);
    #endif

}

static void app_bt_hp_search_window_timer(void)
{
    printf_info("%s\r\n", __func__);
    
	app_window_time_start ();

    U16 bt_hp_search_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (bt_hp_search_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }    
    #endif    
}

const WINDOW bt_hp_search_window = {
    "bt hp search window",
	app_window_bt_hp_search_display,        //draw region
	KEY_TABLE(APP_NAV_BT_HP_SEARCH_KEY),
	1000,                                   //window time
	app_bt_hp_search_window_timer,          //window timer handler	
    WINDOW_RESERVES
};


