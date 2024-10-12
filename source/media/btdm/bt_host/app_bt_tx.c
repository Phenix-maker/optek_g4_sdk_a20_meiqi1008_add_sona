/**
 ****************************************************************************************
 *
 * @file app_bt_tx.c
 *
 * @brief This file contains the bt tx module implement.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup bt_tx
 * @ingroup bt
 * 
 * @brief classical bt tx implement of app layer
 * 
 * @{ 
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "bt_def.h"
#include "bluetooth.h"
#include "hci.h"
#include "bt_lib_mask.h"
#include "ht_hci_main.h"
#include "bt_common.h"
#include "app_bt_tx.h"
#include "audio_task.h"

#ifdef BTDM52_ENABLE
/*
 * configs
 ****************************************************************************************
 */

//#define FILTER_BY_COMPANY_MACID
//#define FILTER_BY_DEVICE_NAME

#ifdef FILTER_BY_COMPANY_MACID
const u8 OEM_Company_ID[3] = {0x3F,0x9F,0x94};		//Optek Company ID 94-9F-3F
#endif

#ifdef FILTER_BY_COMPANY_MACID
const u8 device_name_filter_str[] = {"device name test"};
#endif

/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */
//const u8 hp_macid_for_test[6] = {0x25,0x9d,0x21,0x3f,0x9f,0x94}; 

typedef struct {
    
}hp_link_t;

u8 hp_conn_mode;
u8 hp_conn_index;
u8 hp_conn_macid[6];

enum {
	HP_NO_SEARCHING = 0,
		HP_SEARCHING,
		HP_REQ_SEARCHING
};

u8 hp_searching = HP_NO_SEARCHING;


/*
 * FUNCTION DEFINITIONS PART 1(is called by lib_btdm_host.lib)
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief filter by macid
 *
 * @return   
 *	FALSE :macid isn't required
 *	TRUE  :macid is required
 ****************************************************************************************
 */
u16 multi_speaker_inquiry_addr_cb (u8 *bt_addr)
{
#ifdef FILTER_BY_COMPANY_MACID
	//check company addr as OEM_Company_ID
	if (bt_addr[3] == OEM_Company_ID[0] && bt_addr[4] == OEM_Company_ID[1] &&
		bt_addr[5] == OEM_Company_ID[2])
	{
		return TRUE;
	}
	else 
	{
		DBG_Printf("device's companry macid:(%x,%x,%x) not match\n\r",bt_addr[3],bt_addr[4],bt_addr[5]);
		return FALSE;
	}
#else
	//no need check bt mac addr
	return TRUE;
#endif	
}

/**
 ****************************************************************************************
 * @brief filter by device's name
 *
 * @return   
 *	FALSE :device's name isn't required
 *	TRUE  :device's name is required
 ****************************************************************************************
 */
u8 multi_speaker_remote_name_cb (U8 *name)
{
#ifdef FILTER_BY_DEVICE_NAME
	//check device's name as device_name
	u16 len;

	len = strlen (device_name_filter_str);
	
	if (memcmp(device_name_filter_str,name,len) == 0) {
		return TRUE;
	} 
	else {
		DBG_Printf("device's name:(%s) not match\n\r",name);
		return FALSE;
	}
#else
	//no need check device's name 
	return TRUE;	
#endif
}

/**
 ****************************************************************************************
 * @brief notify that the hp connection has been disconnected
 ****************************************************************************************
 */
void BtHCI_hp_disconn(u32 link_loss)
{
	DBG_printf("disconnected!\r\n");
	//app_bt_init();
	app_bt_set_scan_enable_mode(enBT_PAGE_SCAN_ENABLE);
	if (bt_status.bt_main_status == enBT_CONNECTED)
	{
		//DBG_printf("reset\r\n");
		//hw_wdt_enable(1);// for pach bug, study later

		//while (1);
	}
	bt_status.bt_main_status = enBT_DISCONNECTED;
}

/**
 ****************************************************************************************
 * @brief notify that hp is connected
 ****************************************************************************************
 */
void BtHCI_hp_conn(void)
{
	bt_status.bt_main_status = enBT_CONNECTED;
	app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_DISABLE);
	DBG_printf("connected!\r\n");
}

/**
 ****************************************************************************************
 * @brief notify that no hp macid to relink
 ****************************************************************************************
 */
void BtHCI_otk_no_hp_macid(void)
{
	DBG_Printf ("OTK no HP mac id\n\r");
}

/**
 ****************************************************************************************
 * @brief notify that the hp connection is failed
 ****************************************************************************************
 */
void BtHCI_hp_conn_fail(void)
{
	DBG_Printf ("conn HP fail\n\r");
}

/**
 ****************************************************************************************
 * @brief notify that the inquiry is completed or timed out
 ****************************************************************************************
 */
void app_nav_bt_hp_search_end (void)
{
	int i;
	char *name;
	U8 * macid;
	i8 rssi;
	i8 max_rssi = -128;
    U8 max_rssi_index = 0xff;
	U8 total_num = bt_get_search_total_num();

	for (i=0;i<total_num;i++)
	{
		name = bt_get_search_dev_name(i);
		rssi = bt_get_search_dev_rssi(i);

		#ifdef FILTER_BY_COMPANY_MACID
		//filter by OEM_Company_ID
		macid = bt_get_search_mac_id(i);

		if (bt_addr[3] != OEM_Company_ID[0] || bt_addr[4] != OEM_Company_ID[1] ||
			bt_addr[5] != OEM_Company_ID[2])
		{
			DBG_Printf("device's companry macid:(%x,%x,%x) not match\n\r",bt_addr[3],bt_addr[4],bt_addr[5]);
			continue;			
		} 

		#endif

		#ifdef FILTER_BY_DEVICE_NAME
		//filter by device's name
		
		if (memcmp(device_name_filter_str,name,strlen (device_name_filter_str)) != 0) 
		{
			DBG_Printf("device's name:(%s) not match\n\r",name);
			continue;			
		} 
		#endif

		if (rssi > max_rssi)
		{
			max_rssi = rssi;
			max_rssi_index = i;
		}

    	DBG_Printf ("number %d %s, rssi:%d\n\r",i+1, name,rssi);
	}

	if (max_rssi_index != 0xff)
	{
		DBG_Printf ("try to connect the max rssi device, name:%s, rssi:%d,mac id:%m\n\r", bt_get_search_dev_name(max_rssi_index),bt_get_search_dev_rssi(max_rssi_index),bt_get_search_mac_id(max_rssi_index));

		uiBtHCI_HP_link(2,0, bt_get_search_mac_id(max_rssi_index));//connect the max rssi device
	}
	else
	{
		DBG_Printf("No matching device was found\r\n");
	}

    //app_window_cancel();
}

/**
 ****************************************************************************************
 * @brief notify that avdtp stream is ready, btdm task
 ****************************************************************************************
 */
void app_hp_stream_ready_cb(void)
{
#ifndef BT_HFP_ENABLE
	avdtp_hp_start();
#endif	
}

/**
 ****************************************************************************************
 * @brief notify that the inquiry is completed or timed out 
 * and no math device found
 ****************************************************************************************
 */
void multi_speaker_no_proper_inquiry (void)
{
	//uiBtHCI_set_inquiry_w_num_and_to(100, 8);  // reinquiry
	DBG_Printf("No matching BT Headphone\n\r");

}

/*****avrcp target key handle api**********/

void avrcp_remote_play_handle(void)
{
	DBG_printf("play key press\r\n");
}

void avrcp_remote_stop_handle(void)
{
	DBG_printf("stop key press\r\n");
}

void avrcp_remote_pause_handle(void)
{
	DBG_printf("pause key press\r\n");
}

void avrcp_remote_next_handle(void)
{
	DBG_printf("next key press\r\n");
}

void avrcp_remote_previous_handle(void)
{
	DBG_printf("previous key press\r\n");
}

void avrcp_remote_vol_up_handle (void)
{
#ifdef BT_VOLUME_SYC
	DBG_Puts("bt avrcp rmt vol+\n\r");
	app_audio_device_volume_set(TRUE);
#endif
}

void avrcp_remote_vol_down_handle (void)
{
#ifdef BT_VOLUME_SYC
	DBG_Puts("bt avrcp rmt vol-\n\r");
	app_audio_device_volume_set(FALSE);
#endif
}
/*****avrcp target api end*********/



/*
 * FUNCTION DEFINITIONS PART 2(is define for app layer)
 ****************************************************************************************
 */

void app_nav_bt_tx_init (void)
{
	bt_hp_setup_init();

#ifdef BT_TX_ROLE_ENABLE
#ifdef HFP_AG
	sdp_set_mask(SDP_HFP_AG_MASK);
#endif

#ifdef A2DP_SOURCE
	sdp_set_mask(SDP_A2DP_SRC_MASK);
#endif

#ifdef AVRCP_TARGET
	sdp_set_mask(SDP_AVRCP_TG_MASK);
#endif

#ifdef A2DP_CODEC_SR_48000
	bt_set_hp_avdtp_codec_sr(48000);
#endif

	bt_set_hp_inquiry_mode(2);

	bt_set_hp_inquiry_check_name_req(TRUE);

	bt_set_hp_enable(TRUE);
#endif	
}


#ifdef BT_TX_ROLE_ENABLE

u8 mac_addr[] = {0xcd,0x0e,0x55,0x31,0x03,0x21};
u8 mac_addr1[] = {0xBB,0x64,0x44,0x33,0x22,0x11};
void app_nav_conn_hp (void)
{
	DBG_Printf("try to conn\n\r");

	//for test
	//uiBtHCI_HP_link(2,0, mac_addr);
	uiBtHCI_HP_link(0,0, NULL);
	return;
}	

void app_nav_conn_hp1 (void)
{
	DBG_Printf("try to conn1\n\r");
	//for test
	uiBtHCI_HP_link(2,0, mac_addr1);
	return;
}	


void app_nav_bt_hp_search (void)
{	
	if (otk_psm_conn_present() == TRUE)
	{
		DBG_Printf("bt is connected\n\r");
	}
    DBG_Printf("start inquiry\n\r");
	uiBtHCI_set_inquiry_w_num_and_to(100, 8);

#if 0//search display
	if (app_main_data.window != (WINDOW *) &volume_window)
	{
		app_nav_window_set(&hp_search_window, app_window_data.window_index+1);
	}
#endif
}

void app_hp_tx_play(void)
{
	if (bt_status.bt_a2dp_status == enBT_A2DP_OPEN)
	{
		audio_aux_input_rx_start();
		decode_pause_disable();	
		kBtHCI_avdtp_start();
	}
	else
	{
		DBG_Printf("Out of condition,a2dp state:%d\r\n",bt_status.bt_a2dp_status);		
	}
}
void app_hp_tx_pause(void)
{
	if (bt_status.bt_a2dp_status == enBT_A2DP_STREAMING)
	{
		audio_aux_input_rx_stop_req();
		decode_pause_enable();
		kBtHCI_avdtp_suspend();
	}
	else
	{
		DBG_Printf("Out of condition,a2dp state:%d\r\n",bt_status.bt_a2dp_status);		
	}
}
#else
void app_hp_tx_play(void)
{

}
void app_hp_tx_pause(void)
{

}

#endif
/// @} bt_tx
#endif