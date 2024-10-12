#include "os_config.h" //FreeRTOS kernel includes.

#ifdef BT_HFP_ENABLE
#include "hw_timer.h"

#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"
#include "otg.h"
#include "bt_common.h"
#include "ht_hci_main.h"
#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif
#include "audio_com.h"
#include "audio_device.h"

#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif

#ifdef OPTEK_LINK_ENABLE
#include "optek_link_commons.h"
#endif


extern U8 BT_SBC_STREAM_START[];
extern U8 BT_SBC_STREAM_END[];

#ifdef BT_TX_ROLE_ENABLE
void hfp_audio_switch(void);

U32 app_audio_switch_to_a2dp(KEY_HANDLE_PARAM_STRU param)
{
    hfp_audio_disconn();
    return 0;
}
#endif


/***********************************************
*******       BT HFP window SET         ********
***********************************************/

static const APP_NAV_KEY APP_NAV_HFP_KEY [] = 
{
    #ifdef BT_TX_ROLE_ENABLE
    {APP_KEY_PLAY, app_audio_switch_to_a2dp, NULL, 0, NULL},    
    #endif
    //{APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    //{APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

//	{APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    
};

void app_window_hfp_display(void)
{
    printf_info("hfp window\r\n");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_source(global_vari.audio_source);
    #endif

}

static void app_hfp_window_timer(void)
{
	app_window_time_start ();

    U16 hfp_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (hfp_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }    
    #endif
}

const WINDOW hfp_window = {
    "hfp window",
	app_window_hfp_display,             //draw region
	KEY_TABLE(APP_NAV_HFP_KEY),
	1000,                               //window time
	app_hfp_window_timer,               //window timer handler	
    WINDOW_RESERVES
};

void app_hfp_source_entry(void)
{

	decode_exit_req_set();

 
    global_vari.audio_source = SOURCE_HFP;

    app_nav_window_set( &hfp_window, 0);

	AUDIO_FIFO_STREAM_FLUSH();

	U8 *pStreamBuf;
	U32 size;

    pStreamBuf = BT_SBC_STREAM_START;
    size = BT_SBC_STREAM_END - BT_SBC_STREAM_START;

    AUDIO_FIFO_STREAM_OPEN(pStreamBuf, size);

    AUDIO_FIFO_STREAM_CREATE(0, STREAM_LENTH_UNKNOWN, 0, 0);
    audio_env.playing_stream_status = STREAM_WAITING_DATA;

    //need change hfp's mic input function
//    audio_usb_speaker_input_rx_init();
//    audio_usb_speaker_input_rx_enable();

    msbc_decocde_set();

}

void app_source_convert_to_hfp(void)
{
    if (global_vari.audio_source != SOURCE_HFP)
    {
       if (global_vari.audio_source == SOURCE_BT)
       {
            decode_exit_wait_forever_msgSend();
       }
       else
       {
            app_source_finish();
       }

        app_hfp_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_hfp_source_exit(void)
{
    //audio_usb_speaker_input_receive_stop_req();
}

const APP_SOURCE_STRU hfp_source = {
    "hfp source",
    SOURCE_HFP,
    app_source_convert_to_hfp,
    app_hfp_source_exit
};
#endif
