#include "os_config.h" //FreeRTOS kernel includes.

#ifdef BTDM52_ENABLE
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

#include"codec_malloc.h"
#include"xa_sbc_dec_dec.h"
#ifdef STANDBY_SOURCE_ENABLE
#include "app_standby_source.h"
#endif

#ifdef OPTEK_LINK_ENABLE
#include "optek_link_commons.h"
#endif
#include "cpu_soft_reset.h"
#include "hw_spdif.h"
#include "hw_audio_adc_dac.h"
extern U8 BT_SBC_STREAM_START[];
extern U8 BT_SBC_STREAM_END[];


U32 app_window_bt_key_test_up(KEY_HANDLE_PARAM_STRU param)
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

U32 app_window_bt_skipf_key_up(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {

        if (param.hold_times > 0)
        {
            kBtHCI_ffd_release();
        }
        else 
        {
            #ifdef BT_ID3_DISPLAY_EANBLE
            app_main_data.disp_info = DISP_INFO_WAITING;
            track_id3_ready = FALSE;
            track_id3_rx_flag = DISP_INFO_TITLE;
            memset( id3_tag.title, 0, (MAX_ID3_TEXT + 1)*2);
            memset( id3_tag.artist, 0, (MAX_ID3_TEXT + 1)*2);
            memset( id3_tag.album, 0, (MAX_ID3_TEXT + 1)*2);
            #endif
            
            kBtHCI_next ();
        }

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

U32 app_window_bt_skipf_key_hold(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 hold,combo timers:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    kBtHCI_ffd_press();
    
    return 0;

}

U32 app_window_bt_skipb_key_up(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 up,combo tiemrs:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    if (param.combo_times == 0)
    {
        if (param.hold_times > 0)
        {
            kBtHCI_fbw_release();	
        }
        else 
        {
            #ifdef BT_ID3_DISPLAY_EANBLE
            app_main_data.disp_info = DISP_INFO_WAITING;
            track_id3_ready = FALSE;
            track_id3_rx_flag = DISP_INFO_TITLE;
            memset( id3_tag.title, 0, (MAX_ID3_TEXT + 1)*2);
            memset( id3_tag.artist, 0, (MAX_ID3_TEXT + 1)*2);
            memset( id3_tag.album, 0, (MAX_ID3_TEXT + 1)*2);
            #endif

		    kBtHCI_prev ();
	    }	

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

U32 app_window_bt_skipb_key_hold(KEY_HANDLE_PARAM_STRU param)
{
    printf_info("key 0 hold,combo timers:%d, hold times:%d\r\n", param.combo_times, param.hold_times);

    kBtHCI_fbw_press();
        
    return 0;

}

void app_pair_ctl_led(void)
{
	DBG_Printf("BT %s\r\n",__func__);

    #if 1
	if (bt_status.bt_main_status == enBT_CONNECTED)
	{
		// app_main_data.bt_flush_not_relink = TRUE;
		bt_hci_flush();
        delayms(200);
        bt_status.bt_main_status = enBT_PAIRING;
        app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE); 

	}

    #else
    app_source_next_enter();
    bt_status.bt_main_status = enBT_PAIRING;
    app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE); 
    #endif
}

/***********************************************
*******          BT window SET          ********
***********************************************/

static const APP_NAV_KEY APP_NAV_BT_KEY [] = 
{
    #ifdef OPTEK_LINK_ENABLE
    {APP_KEY_PLAY, app_nav_optek_link_pairing_keyhandler, NULL, 0, NULL},
    {APP_KEY_STOP, NULL, app_optek_link_role_sw_keyhandler, 0, NULL},
    #endif
    //{APP_KEY_PLAY, NULL, hci_get_conn_devices_name_by_bd_addr_keyhandleer, 0, NULL},
 

    // {APP_KEY_PAIRING, NULL, app_pair_ctl_led, 4000, app_nav_upgrade_project},

    {APP_KEY_SKIPF, NULL, app_window_bt_skipf_key_up, 800, app_window_bt_skipf_key_hold},
    {APP_KEY_SKIPB, NULL, app_window_bt_skipb_key_up, 800, app_window_bt_skipb_key_hold},
   
    {APP_KEY_PLUS, NULL, app_audio_device_volume_plus, 0, NULL},
    {APP_KEY_MINUS, NULL, app_audio_device_volume_minus, 0, NULL},

	{APP_KEY_SOURCE, NULL, app_source_next_enter_for_keyfunction, 4000, app_nav_upgrade_project},

    // {APP_KEY_PAIRING, NULL, app_nav_BT_manual_pair, 0, NULL},


    #ifdef STANDBY_SOURCE_ENABLE
    {APP_KEY_POWER, NULL, app_standby_enter_keyfunction, 0, NULL},
    #endif    

};

void app_window_bt_display(void)
{
    printf_info("BlueThooth window\r\n");

    #ifdef DISPLAY_MODULE_ENABLE
    // app_display_module.disp_source(global_vari.audio_source);
    #endif

}

static void app_bt_window_timer(void)
{
	app_window_time_start ();

    U16 bt_win_count = app_window_data.app_window[app_window_data.window_index].win_cn++;

    #ifdef WINDOW_PRINTF_ENABLE
    if (bt_win_count < WINDOW_PRINTF_COUNT)
    {
        printf_info("%s\r\n", __func__);
    }
    #endif
    
    // if(bt_win_count <= 25 && bt_status.bt_main_status == enBT_PAIRING)
    if( bt_status.bt_main_status == enBT_PAIRING)
    {
        #ifdef DISPLAY_MODULE_ENABLE
        app_display_module.disp_bt_status(bt_win_count);
        #endif    
        // DBG_Printf("bt_cnt: %d\r\n",bt_win_count);
    }
    else if(bt_status.bt_main_status != enBT_CONNECTED)
    {
        hal_gpio_output_level_set(GPIO0_10_INDEX,GPIO_HIGH);
    }
    else if(bt_status.bt_main_status == enBT_CONNECTED)
    {
        hal_gpio_output_level_set(GPIO0_10_INDEX,GPIO_LOW);
    }
}

const WINDOW bt_window = {
    "bt window",
	app_window_bt_display,              //draw region
	KEY_TABLE(APP_NAV_BT_KEY),
	1000,                               //window time
	app_bt_window_timer,                //window timer handler	
    WINDOW_RESERVES
};

void app_bt_source_entry(void)
{
    DBG_Printf("%s\r\n",__func__);
	decode_exit_req_set();

    global_vari.audio_source = SOURCE_BT;

    #ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
    app_task_timer_ms_set(TIMER_SAVE_GlobalVari, SAVE_GLOBAL_VARI_STRU_DELAY_TIME);
    #endif

    // dac_write(0x0f,0x00);//soft unmute dac1
	// dac_write(0x10,0x00);//soft unmute dac2

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
    app_nav_window_set( &bt_window, 0);

    #ifdef BT_TX_ROLE_ENABLE
    bt_set_hp_enable(FALSE);
    #endif

#ifdef OPL_MASTER_ENABLE	

#ifndef OPL_MODE_SWF
    DBG_Assert(FALSE);//optek link not support bt source except subwoofer mode
#endif

    if(global_vari.optek_link_role == OPL_ROLE_MASTER)
    {
        void app_change_mode_req(u8 mode, u8 max_master_tx_len, u8 max_master_rx_len);
        //app_change_mode_req(IDLE_W_BT,6,6);
        app_change_mode_req(TWS_SUBW_W_BT_T6_SF44K,245,6);
        //optek_link_role_stop();
        delayms(100);
    }
#endif
    // if ((bt_status.bt_main_status == enBT_OPENED)||(bt_status.bt_main_status == enBT_DISCONNECTED)||(bt_status.bt_main_status == enBT_RELINK) )
    {
        //relink
        /*
        * app_task_timer_ms_set(TIMER_BT_Relink, 1000);
        * */        
       	#ifdef BT_RELINK_ENABLE        
        if (bt_pairing_info_not_none())
        {
            app_task_timer_ms_set(TIMER_BT_Relink, 500);
        }
        else
        {
            bt_status.bt_main_status = enBT_PAIRING;
            app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE);
            // app_task_timer_ms_set(TIMER_BT_Pairing, 500);
        }
        #else
        bt_status.bt_main_status = enBT_PAIRING;
        app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE);        
        #endif
    }



	AUDIO_FIFO_STREAM_FLUSH();

	U8 *pStreamBuf;
	U32 size;

    pStreamBuf = BT_SBC_STREAM_START;
    size = BT_SBC_STREAM_END - BT_SBC_STREAM_START;

    AUDIO_FIFO_STREAM_OPEN(pStreamBuf, size);

    AUDIO_FIFO_STREAM_CREATE(0, STREAM_LENTH_UNKNOWN, 0, 0);
    audio_env.playing_stream_status = STREAM_WAITING_DATA;

    sbc_decocde_set();

}

void app_source_convert_to_bt(void)
{
    if (global_vari.audio_source != SOURCE_BT)
    {
        app_source_finish();

        app_bt_source_entry();
    }
    else
    {
        //do nothing
    }
}

void app_bt_source_exit(void)
{
    if ((bt_status.bt_main_status >= enBT_OPENED) && (bt_status.bt_main_status <= enBT_PAIRING))
	{

		u8 hci_relink_busy (void);
		void hci_relink_request_cancel (void);

		#ifdef BT_RELINK_ENABLE
        app_task_timer_ms_set(TIMER_BT_Relink, 0);
		#endif

		hci_relink_request_cancel(); //msut before hci_relink_busy()

		// app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_DISABLE);
		delayms (10);

		if (hci_relink_busy() == TRUE)
		{
			//relink is in process
			// app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_DISABLE);
		} 
		else
		{
			if (hci_A2DP_conn_present() == TRUE)
			{				
				bt_hci_flush();
				delayms(200);
			}
			else
			{
				//no mobile phone conn
				app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_DISABLE);
			}
		}
	}
}

const APP_SOURCE_STRU bt_source = {
    "bt source",
    SOURCE_BT,
    app_source_convert_to_bt,
    app_bt_source_exit
};
#endif
