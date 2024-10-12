#include "os_config.h"

#ifdef AUX_ENABLE
#include "hw_timer.h"
#include "hw_pll.h"
#include "hw_cpu_clock.h"

#include "sdk_com.h"

#include "audio_task.h"
#include "app_window.h"
#include "app_source.h"

#include "system_task.h"
#include "system_timer.h"
#include "app_i2c.h"
#include "hw_audio_i2s.h"
#include "audio_aux_input.h"
#include "Audio_Device.h"
#include "hw_spdif.h"
#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif

#ifdef BATT_DETECT_ENABLE
#include "hal_key_adc.h"
#endif
#ifdef BT_TX_ROLE_ENABLE
#include "app_bt_tx.h"
#include "bt_common.h"
#endif
#include "hal_key_adc.h"
#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif

#ifdef OPTEK_LINK_ENABLE
#include "optek_link_commons.h"
#endif
#include "cpu_soft_reset.h"
#include "bt_common.h"

extern const WINDOW aux_window;

#include "hw_audio_adc_dac.h"

/**
 ****************************************************************************************
**/
U32 app_window_aux_source_key_down(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_aux_source_key_up(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_aux_source_key_hold(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_aux_key_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 down,combo timers:%d\r\n", param.combo_times);

    return 300;
}

U32 app_window_aux_key_up(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_aux_key_test_down(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 12 down,combo timers:%d\r\n", param.combo_times);

    return 0;
}

U32 app_window_aux_key_test_up(KEY_HANDLE_PARAM_STRU param)
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


U32 app_window_aux_key_hold(KEY_HANDLE_PARAM_STRU param)
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


#ifdef BT_TX_ROLE_ENABLE
void app_nav_conn_hp (void);
void hfp_audio_switch(void);

U32 app_audio_switch_to_hfp(KEY_HANDLE_PARAM_STRU param)
{
    hfp_audio_conn();

    return 0;
}

U32 app_relink_last_hp(KEY_HANDLE_PARAM_STRU param)
{
    if (param.hold_times == 0)
        app_nav_conn_hp();
    return 0;
}

U32 app_key_nav_bt_hp_search(KEY_HANDLE_PARAM_STRU param)
{
    app_nav_bt_hp_search();
    return 0;
}
#endif

U32 app_nav_optek_link_pairing_keyhandler(KEY_HANDLE_PARAM_STRU param);
U32 app_optek_link_role_sw_keyhandler(KEY_HANDLE_PARAM_STRU param);


void app_pair_ctl_LED(void)
{
    DBG_Printf("aux %s\r\n",__func__);
    // bt_status.bt_main_status = enBT_PAIRING;
    // app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE);
    enter_fix_source(SOURCE_BT);
    // enter_fix_source(SOURCE_BT);
}


/***********************************************
*******         AUX window SET          ********
***********************************************/

static const APP_NAV_KEY APP_NAV_AUX_KEY [] = 
{
    #ifdef OPTEK_LINK_ENABLE
    {APP_KEY_PLAY, app_nav_optek_link_pairing_keyhandler, NULL, 0, NULL},
    {APP_KEY_STOP, app_optek_link_role_sw_keyhandler, NULL, 0, NULL},
	#else
    {APP_KEY_PLAY, app_window_aux_key_down, app_window_aux_key_up, 800, app_window_aux_key_hold},
    #endif
    #ifdef BT_TX_ROLE_ENABLE
    {APP_KEY_STOP, app_nav_bt_hp_search, NULL, 0, NULL},
    #endif


    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

//    {APP_KEY_PAIRING, NULL, app_piar_ctl_LED, 0, NULL},
    {APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 4000, app_nav_upgrade_project},
    // {APP_KEY_PAIRING, NULL, app_pair_ctl_LED, 4000, app_nav_upgrade_project},

    // {APP_KEY_FOLDER, app_hpm_sw_down, app_hpm_sw_up, 0, NULL}, //HPM_SW
    // {APP_KEY_ALARM1, app_F_SW_down, app_F_SW_up, 0, NULL}, //F_SW
    // {APP_KEY_ALARM2, NULL, app_audio_device_volume_plus, 0, NULL}, //PHASE_SW
    // {APP_KEY_DISC_CHANGE, NULL, app_audio_device_volume_plus, 0, NULL},//FRO_SW


    // {APP_KEY_EQ, NULL, app_audio_device_eq_mode_loop, 0, NULL},
    // {APP_KEY_TVOUT, NULL, app_led_control, 0, NULL},


    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

};

void app_window_aux_display(void)
{
    printf_info("enter aux window\n\r");

    #ifdef DISPLAY_MODULE_ENABLE
    // app_display_module.disp_source(global_vari.audio_source);
    #endif
}

static void app_aux_window_timer(void)
{
	app_window_time_start ();

    U16 aux_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    // if (aux_win_count < WINDOW_PRINTF_COUNT)
    // {
    //     printf_info("%s\r\n", __func__);
    // }
    #endif
    // if(aux_win_count <= 45 && bt_status.bt_main_status == enBT_PAIRING)
    // {
    //     #ifdef DISPLAY_MODULE_ENABLE
    //     // app_display_module.disp_source(global_vari.audio_source);
    //     app_display_module.disp_bt_status(aux_win_count);
    //     #endif

    // }
    hal_gpio_output_level_set(GPIO0_10_INDEX,GPIO_HIGH);        

}

const WINDOW aux_window = {
    "aux window",
	app_window_aux_display,        //draw region
	KEY_TABLE(APP_NAV_AUX_KEY),
	1000,                          //window time
	app_aux_window_timer,          //window timer handler	
	WINDOW_RESERVES
};

void app_aux_source_entry(void)
{
    DBG_Printf("\n\r%s\n\r", __func__);

    decode_exit_wait_forever_msgSend();

    global_vari.audio_source = SOURCE_AUX;

    #ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
    app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
    #endif

    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
	auxRx_Dma_Opened = FALSE;
	#endif
	
    // dac_write(0x0f,0x01);//soft unmute dac1
	// dac_write(0x10,0x01);//soft unmute dac2

    app_nav_window_set( &aux_window, 0);

    #ifdef AUDIO_OUTPUT_FROM_SPDIF
    #ifdef OPTICAL_SPDIF_IN_CHANNEL_SEL_0
	hw_spdif_init(SPDIF_IN_CH_0);
	hw_spdif_open(SPDIF_IN_CH_0);
	#elif defined OPTICAL_SPDIF_IN_CHANNEL_SEL_1
	hw_spdif_init(SPDIF_IN_CH_1);
	hw_spdif_open(SPDIF_IN_CH_1);
	#else
	error Must define the input channel of optical spdif!
	#endif
    #endif
  	hw_pll_audio_clock_set(SF_BASE_48000, SPDIF_SF_1x);

    #ifdef AUDIO_IN_STREAM_USED
	audio_input_rx_stream_config();
    #endif

	#ifdef BT_TX_ROLE_ENABLE
    app_bt_set_scan_enable_mode(enBT_PAGE_SCAN_ENABLE);
	#endif

    audio_aux_input_rx_init();

    aux_decocde_set();

    aux_decocde_start_w_init();
}

void app_source_convert_to_aux(void)
{
    if (global_vari.audio_source != SOURCE_AUX)
    {
        app_source_finish();

        app_aux_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_aux_source_exit(void)
{
    audio_aux_input_rx_stop_req();
}

const APP_SOURCE_STRU aux_source = {
    "aux source",
    SOURCE_AUX,
    app_source_convert_to_aux,
    app_aux_source_exit
};

#endif //AUX_ENABLE
