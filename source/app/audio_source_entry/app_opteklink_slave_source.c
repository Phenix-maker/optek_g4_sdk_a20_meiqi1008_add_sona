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

#ifdef OPL_SLAVE_ENBALE

/**
 ****************************************************************************************
**/
U32 app_nav_optek_link_pairing_keyhandler(KEY_HANDLE_PARAM_STRU param);
U32 app_optek_link_role_sw_keyhandler(KEY_HANDLE_PARAM_STRU param);

/***********************************************
*******   Optek link slave window SET   ********
***********************************************/

static const APP_NAV_KEY APP_NAV_OPTEKLINK_SLAVE_KEY [] = 
{
    #ifdef OPTEK_LINK_ENABLE
    {APP_KEY_PLAY, app_nav_optek_link_pairing_keyhandler, NULL, 0, NULL},
    {APP_KEY_STOP, app_optek_link_role_sw_keyhandler, NULL, 0, NULL},
    #endif
    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif
};

void app_window_opteklink_slave_display(void)
{
    printf_info("opteklink_slave window\r\n");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_source(global_vari.audio_source);
    #endif

}

static void app_opteklink_slave_window_timer(void)
{
	app_window_time_start ();

    U16 opteklink_slave_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (opteklink_slave_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif

}

const WINDOW opteklink_slave_window = {
    "opteklink_slave window",
	app_window_opteklink_slave_display,         //draw region
	KEY_TABLE(APP_NAV_OPTEKLINK_SLAVE_KEY),
	1000,                                      //window time
	app_opteklink_slave_window_timer,           //window timer handler	
    WINDOW_RESERVES
};

void app_opteklink_slave_source_entry(void)
{
    global_vari.audio_source = SOURCE_OPTEKLINK_SLAVE;

    app_nav_window_set( &opteklink_slave_window, 0);

    #ifdef OPL_MODE_SWF
    xa_pcm_downsample_decocde_set();
    #elif defined OPL_MODE_SINGLE_WAY
    lc3_decocde_set();
    #endif
}

void app_source_convert_to_opteklink_slave(void)
{
    DBG_Printf("\n\r%s\n\r", __func__);

    if (global_vari.audio_source != SOURCE_OPTEKLINK_SLAVE)
    {        
        app_source_finish();

        app_opteklink_slave_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_opteklink_slave_source_exit(void)
{

}

const APP_SOURCE_STRU opteklink_slave_source = {
    "opteklink_slave source",
    SOURCE_OPTEKLINK_SLAVE,
    app_source_convert_to_opteklink_slave,
    app_opteklink_slave_source_exit
};

#endif