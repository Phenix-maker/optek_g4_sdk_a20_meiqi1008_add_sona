#include "os_config.h"


#if ( (defined USB_HOST_ENABLE) && (defined USB_HOST_MSC_ENABLE) )
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

#include "app_media_process.h"

#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif

#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif

/**
 ****************************************************************************************
**/
U32 app_window_usb_msc_source_key_down(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_usb_msc_source_key_up(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_usb_msc_source_key_hold(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_usb_msc_key_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    return 300;
}

U32 app_window_usb_msc_key_up(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_usb_msc_key_test_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 12 down,combo timers:%d\r\n", param.combo_times);

    return 0;
}

U32 app_window_usb_msc_key_test_up(KEY_HANDLE_PARAM_STRU param)
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


U32 app_window_usb_msc_key_hold(KEY_HANDLE_PARAM_STRU param)
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
*******        USB msc window SET       ********
***********************************************/

static const APP_NAV_KEY APP_NAV_USB_MSC_KEY [] = 
{
    {APP_KEY_PLAY, app_window_usb_msc_key_down, app_window_usb_msc_key_up, 800, app_window_usb_msc_key_hold},
   
    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

	{APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 0, NULL},

    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

};


void app_window_usb_msc_display(void)
{
    printf_info("enter usb msc window\n\r");

    #ifdef DISPLAY_MODULE_ENABLE
    app_display_module.disp_source(global_vari.audio_source);
    app_display_module.disp_usb_status(dispUSB_INIT);
    #endif

}

extern const WINDOW usb_msc_window;

static void app_usb_msc_window_timer(void)
{
	app_window_time_start ();

    U16 usb_msc_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (usb_msc_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif
    
    static U8 delay_temp;
    if (delay_temp++ > 1000/(app_window_current_get()->window_time))
    {
        delay_temp = 0;
        //app_nav_window_set(&readingcd_window, 0);
        #ifdef DISPLAY_MODULE_ENABLE
        app_display_module.disp_usb_status(dispUSB_OPEN);
        #endif
    }
}

const WINDOW usb_msc_window = {
    "usb msc window",
	app_window_usb_msc_display,         //draw region
	KEY_TABLE(APP_NAV_USB_MSC_KEY),
	1000,                               //window time
	app_usb_msc_window_timer,           //window timer handler	
    WINDOW_RESERVES
};

void app_usb_msc_source_entry(void)
{
    DBG_Printf("\n\r%s\n\r", __func__);

	decode_exit_wait_forever_msgSend();

    global_vari.audio_source = SOURCE_USB_MSC;

    #ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
    app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
    #endif
	
    app_nav_window_set( &usb_msc_window, 0);

	//app_nav_window_set(&readingcd_window, 0);

    usb_msc_process_id = USB_MSC_DUMMY_PROCESS;
 
  	hw_pll_audio_clock_set(SF_BASE_48000, SPDIF_SF_1x);

    #ifdef USB_HOST_ENABLE
    usbmsc_USBhostClose();   
    #endif

    #ifdef USB_HOST_ENABLE
    usbmsc_USBhostOpen();
    #endif

    #if ( (defined USB_HOST_ENABLE) || (defined USB_DEV_ENABLE) )
    USB_INT_ENABLE;
    #endif
}

void app_source_convert_to_usb_msc(void)
{
    if (global_vari.audio_source != SOURCE_USB_MSC)
    {
        app_source_finish();

        app_usb_msc_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_usb_msc_source_exit(void)
{
    // usbmsc_USBhostClose();

    // USB_INT_DISABLE;
    usb_msc_process_id = USB_MSC_DUMMY_PROCESS;
    
    app_cmd_DecoderExit();
}

const APP_SOURCE_STRU usb_msc_source = {
    "usb msc source",
    SOURCE_USB_MSC,
    app_source_convert_to_usb_msc,
    app_usb_msc_source_exit
};

#endif //( (defined USB_HOST_ENABLE) && (defined USB_HOST_MSC_ENABLE) )
