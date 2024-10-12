#include "os_config.h"

#include "hw_timer.h"
#include "hw_pll.h"

#include "fifo.h"

#include "sdk_com.h"
#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"
#include "system_task.h"
#include "system_timer.h"
#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif
#include "spdif_data_parse.h"
#include "otg.h"
#include "audio_usb_speaker_input.h"
#include "Audio_Device.h"

#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif


#ifdef USB_SPEAKER
extern const WINDOW usbslave_window;

/**
 ****************************************************************************************
**/
U32 app_window_usbslave_source_key_down(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_usbslave_source_key_up(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_usbslave_source_key_hold(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_usbslave_key_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    return 300;
}


U32 app_window_usbslave_key_up(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up, combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

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

U32 app_window_usbslave_key_test_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 12 down,combo timers:%d\r\n", param.combo_times);

    return 0;
}

U32 app_window_usbslave_key_test_up(KEY_HANDLE_PARAM_STRU param)
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


U32 app_window_usbslave_key_hold(KEY_HANDLE_PARAM_STRU param)
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

#ifdef HID_FUNC_ENABLE

#define HIDPktSize 64

//const U8 hidbuftest[] = {'A','B','C','D','E','F','G','H','0','1','2','3','4','5','6','7'};
const U8 hidbuftest[] = "ABCDEFGH01234567ABCDEFGH12345678ABCDEFGH23456789ABCDEFGH34567890\0";
U8 hidbuftestRpt[64];

U16 usb_hid_data_in(short *pdata);
int HIDdataIN_Buf( U8* hidBuf, int inlen);

U32 app_window_hid_data_test_keydown(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 12 down,combo timers:%d\r\n", param.combo_times);

    hidbuftestRpt[0] = 0x01;    //Report_ID
    memcpy(hidbuftestRpt+1, hidbuftest, 63);
    printf_info("HIDdataIN_Buf: %x %x %x %x %x %x %x %x ... \r\n", hidbuftest[0],hidbuftest[1],hidbuftest[2],hidbuftest[3],hidbuftest[4],hidbuftest[5],hidbuftest[6],hidbuftest[7] );

    HIDdataIN_Buf(hidbuftestRpt, HIDPktSize);   //due to HIDReportDescriptor[] 0x95,0x3F,          		//17  	GLOBAL_REPORT_COUNT(63)	+ 1

    return 0;
}


U32 app_window_hid_key_play_keydown(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    g_submitIntInKdown(APP_KEY_PLAY);

    return 300;
}

U32 app_window_hid_key_play_keyup(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d,hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
        g_submitIntInKup();
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {
        printf_info("pass the key twice\n\r");
    }
    else if (param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");
    }

    return 0;
}

U32 app_window_hid_key_volume_plus_keydown(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    g_submitIntInKdown(APP_KEY_PLUS);

    return 300;
}

U32 app_window_hid_key_volume_plus_keyup(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d,hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
        g_submitIntInKup();
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {
        printf_info("pass the key twice\n\r");
    }
    else if (param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");
    }

    return 0;
}

U32 app_window_hid_key_volume_minus_keydown(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    g_submitIntInKdown(APP_KEY_MINUS);

    return 300;
}

U32 app_window_hid_key_volume_minus_keyup(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d,hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
        g_submitIntInKup();
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {
        printf_info("pass the key twice\n\r");
    }
    else if (param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");
    }

    return 0;
}

U32 app_window_hid_key_skipf_keydown(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    g_submitIntInKdown(APP_KEY_SKIPF);

    return 300;
}

U32 app_window_hid_key_skipf_keyup(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d,hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
        g_submitIntInKup();
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {
        printf_info("pass the key twice\n\r");
    }
    else if (param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");
    }

    return 0;
}

U32 app_window_hid_key_skipb_keydown(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    g_submitIntInKdown(APP_KEY_SKIPB);

    return 300;
}

U32 app_window_hid_key_skipb_keyup(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d,hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
        g_submitIntInKup();
        printf_info("pass the key once\n\r");
    }
    else if (param.combo_times == 1)
    {
        printf_info("pass the key twice\n\r");
    }
    else if (param.combo_times == 2)
    {
        printf_info("pass the key thrice\n\r");
    }

    return 0;
}
#endif

/***********************************************
*******     USB Speaker window SET      ********
***********************************************/

static const APP_NAV_KEY APP_NAV_USB_SPEAKER_KEY [] =
{
	#ifdef HID_FUNC_TEST
    {APP_KEY_PLAY, app_window_hid_key_play_keydown, app_window_hid_key_play_keyup, 0, NULL},

    {APP_KEY_SKIPF, app_window_hid_key_skipf_keydown, app_window_hid_key_skipf_keyup, 0, NULL},
    {APP_KEY_SKIPB, app_window_hid_key_skipb_keydown, app_window_hid_key_skipb_keyup, 0, NULL},

    {APP_KEY_PLUS, app_window_hid_key_volume_plus_keydown, app_window_hid_key_volume_plus_keyup, 0, NULL},
    {APP_KEY_MINUS, app_window_hid_key_volume_minus_keydown, app_window_hid_key_volume_minus_keyup, 0, NULL},

    {APP_KEY_EQ, app_window_hid_data_test_keydown, NULL, 0, NULL},    //key down needn't for HID data in !

    {APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

	#else
    {APP_KEY_PLAY, app_window_usbslave_key_down, app_window_usbslave_key_up, 800, app_window_usbslave_key_hold},

    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

    {APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    
	#endif
};

void app_window_usb_speaker_display(void)
{
    printf_info("enter usb spraker window\n\r");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_source(global_vari.audio_source);
    #endif

}

static void app_usb_speaker_window_timer(void)
{
    app_window_time_start ();

    U16 usb_speaker_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (usb_speaker_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }    
    #endif
}

const WINDOW usb_speaker_window = {
    "usbslave window",
    app_window_usb_speaker_display,         //draw region
    KEY_TABLE(APP_NAV_USB_SPEAKER_KEY),
    1000,                                   //window time
    app_usb_speaker_window_timer,           //window timer handler
    WINDOW_RESERVES
};

void app_usb_speaker_source_entry(void)
{
    DBG_Printf("\n\r%s\n\r", __func__);

	decode_exit_req_set();

   	global_vari.audio_source = SOURCE_USB_SPEAKER;

    #ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
    app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
    #endif

    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
    usbslaveRx_Dma_Opened = FALSE;
    #endif
    
    app_nav_window_set( &usb_speaker_window, 0);

  	hw_pll_audio_clock_set(SF_BASE_48000, SPDIF_SF_1x);

    #ifdef AUDIO_IN_STREAM_USED
	audio_input_rx_stream_config();
    audio_usb_speaker_input_rx_init();
    #else
    usb_decocde_set();
    usb_decocde_start_w_init();
    #endif

    #ifdef USB_DEV_ENABLE
    usb_devStop();
    
    app_cmd_switch2usbSpeaker();
    #endif
	
    audio_vari.mute = FALSE;
    #ifdef AUDIO_CODEC_USED_MUTE
    AUDIOdevice.Set_Mute(audio_vari.volume);
    #endif
    #ifdef AUDIO_AMP_USED_MUTE
    for (int i=0;i<AMP_DEV_COUNT_MAX;i++)
    {
        AudioAmpDev[i].Set_Mute(audio_vari.mute);
    }
    #endif
}

void app_source_convert_to_usb_speaker(void)
{
    if (global_vari.audio_source != SOURCE_USB_SPEAKER)
    {
        app_source_finish();

        app_usb_speaker_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_source_usb_speaker_exit(void)
{
    usb_devStop();
}

const APP_SOURCE_STRU usb_speaker_source = {
    "usb speaker source",
	SOURCE_USB_SPEAKER,
    app_source_convert_to_usb_speaker,
    app_source_usb_speaker_exit
};

#endif// USB_SPEAKER
