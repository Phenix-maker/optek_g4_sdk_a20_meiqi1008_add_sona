#ifndef BT_HCI_MAIN_H
#define BT_HCI_MAIN_H


//bt hci interface for ui
void kBtHCI_next(void);
void kBtHCI_prev(void);


void ht_hci_init(void);
int ht_hci_open(void);

int ht_hci_close(void);
void ht_hci_fini(void);

//#define TEST_BQB_ENABLE
//#define TEST_DUT_ENABLE

void sdp_init(void);
void sdp_set_mask (u32 mask);
void sdp_clear_mask (u32 mask);
u32 sdp_get_mask (void);
void record_list_init(void);

void bt_flash_reset(void);
void bt_flash_reset_w_multi_speaker_macid(void);

enum {
	CHAN_SUBW	= 0,
	CHAN_MIC_1	= 0,
	CHAN_MIC_2  = 1,
	CHAN_MIC_3  = 2
};


//#define RDA_5876_RF_FRONT
//#define TI_2564_RF_FRONT
//#define CSR_8811_RF_FRONT
//#define RTK_8761A_RF_FRONT
#define OTK_5288_RF_FRONT

//#define BT_MULTIROOM_BROADCAST
#define BT_MULTIROOM_BROADCAST_HANDLE	0x0FF0				//defined by Optek?


//bt mutlti speaker
enum {
	BT_NO_MULTI_SPEAKER_SOURCE = 0,
		BT_NO_MULTI_SPEAKER_SINK,
};

enum {
	BT_NO_MULTI_SPEAKER = 0,
		BT_MULTI_SPREAKER_L,
		BT_MULTI_SPREAKER_R,
		BT_MULTI_SPREAKER_MASTER,
		BT_MULTI_SPREAKER_SUB_MASTER,
		BT_MULTI_SPREAKER_SLAVE,
		BT_MULTI_SPREAKER_MAX
};

#define BT_MULTI_SPEAKER_TWS
//#define BT_MULTI_SPEAKDER_MULTIROM_STAR
//#define BT_MULTI_SPEAKDER_MULTIROM_RING



void ht_hci_set_multi_speaker_mode (u8 mode);
u8 ht_hci_get_multi_speaker_mode (void);

#ifdef CSR_8811_RF_FRONT
//#define HCI_BCSP	
//#define HCI_BCSP_CRC

//BCSP Link establishment state
#define		BCSP_LE_SYNC				0x01
#define		BCSP_LE_SYNC_RESP			0x02
#define		BCSP_LE_CONF				0x04
#define		BCSP_LE_CONF_RESP			0x08
#define		BCSP_LE_CMD					0x10
#define		BCSP_LE_COMPLETE			0x20

u8 bt_get_bcsp_state (void);
void bt_get_bcsp_set_state_bit (u8 sbit);

u8 bt_flash_is_multi_speaker_btaddr_present(void);

#endif

#define BT_LE_ENABLE
//#define BT_LE_CENTRAL
#define BT_LE_PERIPHERAL

#define BT_LE_ADDR_RANDOM


/**
 ****************************************************************************************
 * @defgroup bt_tx (implement class bt profiles of a2dp source,avrcp target,hfp AG)
 * @ingroup bt
 * @brief definitions for management and use of bt tx module
 *
 * @{
 ****************************************************************************************
 */
void bt_hp_setup_init (void);
void bt_set_hp_enable (u8 enable);
u8 bt_get_hp_status (void);

void bt_set_hp_vol_sync_enable (u8 enable);
u8 bt_get_hp_vol_sync_status (void);

/**
 ****************************************************************************************
 * @brief set tx audio samplerate
 *
 * @param[in]  sample_rate   48000 or 44100 
 *
 ****************************************************************************************
 */
void bt_set_hp_avdtp_codec_sr (u16 sample_rate);
u16 bt_get_hp_avdtp_codec_sr (void);

/**
 ****************************************************************************************
 * @brief set inquiry mode
 *
 * @param[in]  mode   
	0 :Standard Inquiry Result event format (default)
	1 :Inquiry Result format with RSSI
	2 :Inquiry Result with RSSI format or Extended Inquiry Result format
 ****************************************************************************************
 */
void bt_set_hp_inquiry_mode (u8 mode);
u8 bt_get_hp_inquiry_mode (void);

/**
 ****************************************************************************************
 * @brief set the req of check name
 *
 * @return   
 *	0 :app don't need check name
 *	1 :app need check name
 ****************************************************************************************
 */
void bt_set_hp_inquiry_check_name_req (u8 flag);
u8 bt_get_hp_inquiry_check_name_req (void);

void bt_hci_mem_init_cb (void);
u8 bt_flash_rx_dev_btaddr_present(void);


/**
 ****************************************************************************************
 * @brief start inquiry  (inquiry number fixed to 16 and timeout fixed to 8*1.28s)
 ****************************************************************************************
 */
void uiBtHCI_set_inquiry(void);

/**
 ****************************************************************************************
 * @brief start inquiry
 *
 * @param[in]  num        number to inquiry
 * @param[in]  timeout    timeout in this inquiry
 *
 ****************************************************************************************
 */
void uiBtHCI_set_inquiry_w_num_and_to(U8 num,U8 timeout);

/**
 ****************************************************************************************
 * @brief relink to hp.
 *
 * @param[in]  mode       hp addr source. 0->froam bt flash deta, 1->fraom seach dev list, 2->from MAC ID ptr
 * @param[in]  index      hp addr index  (use if mode equal to 0 or 1)
 * @param[in]  macid      hp addr pointer  (use if mode equal to 2)
 *
 ****************************************************************************************
 */
void uiBtHCI_HP_link(u8 mode, u8 index, u8 *macid);

/**
 ****************************************************************************************
 * @brief return bt tx connect status
 *
 * @return 
 * FALSE disconnected 
 * TRUE connected
 *
 ****************************************************************************************
 */
U8 otk_psm_conn_present(void);

/**
 ****************************************************************************************
 * @brief return search total number.
 *
 * @return     total number
 ****************************************************************************************
 */
u8 bt_get_search_total_num (void);

/**
 ****************************************************************************************
 * @brief return device's name in the search list according to the list index
 *
 * @param[in]  index   index in search list
 * 
 * @return device's name
 ****************************************************************************************
 */
u8 * bt_get_search_dev_name (u8 index);

/**
 ****************************************************************************************
 * @brief return device's rssi in the search list according to the list index
 *
 * @param[in]  index   index in search list
 * 
 * @return device's name
 ****************************************************************************************
 */
i8 bt_get_search_dev_rssi (u8 index);

/**
 ****************************************************************************************
 * @brief return device's macid in the search list according to the list index
 *
 * @param[in]  index   index in search list
 * 
 * @return device's macid
 ****************************************************************************************
 */
u8 *bt_get_search_mac_id (U8 index);

/**
 ****************************************************************************************
 * @brief return the maximum rssi from search list
 *
 * @return     rssi
 ****************************************************************************************
 */
i8 bt_get_search_data_max_rssi(void);

/**
 ****************************************************************************************
 * @brief return the valid device index of the maximum rssi from search list
 *
 * @return                      index
 ****************************************************************************************
 */
u8 bt_get_search_data_max_rssi_index (void);

///@} bt_tx


/**
 ****************************************************************************************
 * @brief Get the device's name specified by board addr in the ACL connected devices table
 *
 * @param[in]  bd_addr board addr
 *
 * @return The device's name
 * 
 * @note Return the first one if bd_addr is NULL
 ****************************************************************************************
 */
char * bt_get_devices_name_by_bd_addr_in_acl_connections(U8 *bd_addr);

void kBtHCI_avdtp_start (void);
void kBtHCI_avdtp_suspend (void);
void kBtHCI_avdtp_reconf (u8 freq);



/*BLE*/
void kBtHCI_adv_enable(void);
void kBtHCI_adv_disable(void);
void kBtHCI_ble_scan_enable(void);
void kBtHCI_ble_scan_disable(void);
void bt_ble_hci_flush (void);

u8 ld_acl_get_slave_time_sync_info (u32 *base_cnt_rxsync_1st, u16 *fine_cnt_rxsync_1st,u32 *base_cnt_rxsync_last, u16 *fine_cnt_rxsync_last,i32 *swap_cn);


enum {
	HCI_RX_EVENT,//HCI RX
	HOST_MSG_EVENT,
	A2DP_TX_EVENT,//A2DP SOURCE TX
	_100MS_TICK_EVENT,
	OTHERS_EVENT
};
void btdm_host_init(void);
void btdm_host_schedule(U32 event);

#endif//BT_HCI_MAIN_H
