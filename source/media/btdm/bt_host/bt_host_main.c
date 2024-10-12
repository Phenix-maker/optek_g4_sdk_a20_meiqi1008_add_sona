#include "os_config.h"

#include "sdk_com.h"

#include "bt_def.h"
#include "bluetooth.h"
#include "hci.h"
#include "hci_core.h"
#include "app_bt_tx.h"
#include "ht_hci_main.h"
#include "bt_lib_mask.h"
#include "bt_common.h"

#include "btdm_task.h"
#include "interrupt.h"
#include "hw_timer.h"
#include "hal_uart.h"
#include "hw_timer.h"
#include "fifo.h"
#include "audio_com.h"
#include "audio_device.h"
#include "hal_sflash.h"
#ifdef SAVE_MODULE_FROM_EEPROM
#include "app_save.h"
#include "eeprom_device.h"
#endif

#ifdef OPTEK_LINK_ENABLE
#include "optek_link_config.h"
#endif

#ifdef BTDM52_ENABLE

BT_STATUS_s bt_status;

extern struct bt_search_data *pSearchData;

const char *bt_lmp_data_get(void);
const char *bt_lmp_time_get(void);
const char *bt_lmp_version_get(void);

const char *bt_host_data_get(void);
const char *bt_host_time_get(void);
const char *bt_host_version_get(void);

int hci_conn_present(void);
u16 ml_device_get_conns(void);


bool BtHCI_hfp_is_calling(void);

void app_nav_bt_codec_reinit (void);

extern U32 __bt_conn_text_rom_start[];
extern U32 __bt_conn_text_ram_start[];
extern U32 __bt_conn_text_ram_end[];

extern U32 __tws_elfsymbol_text_ram_end[];

extern U32 __tws_elfsymbol_text_rom_start[];
extern U32 __tws_elfsymbol_text_ram_start[];
extern U32 __tws_elfsymbol_text_ram_end[];

#define BT_NAME_STRING_MAX_LEN    64
const u8 bt_name[BT_NAME_STRING_MAX_LEN] = {"MeiQi demo test\0\0"};           //max len is 64
const u8 bt_init_name[BT_NAME_STRING_MAX_LEN] = {"MeiQi demo test Init\0\0"}; //max len is 64

const u8 bt_upgrade_file_name[BT_NAME_STRING_MAX_LEN] = {"BlueSea_BT_SDK_upgrade.bin\0\0"}; //max len is 64

int app_bt_conn_code_text_copy(void)
{
#if 0
	int len;
	#ifdef OPL_MODE_TWS
	//len = __tws_elfsymbol_text_ram_end - __tws_elfsymbol_text_ram_start;

	//DBG_Printf("tws elfsymbol text len:%d\r\n",len);
	#else
	
	if (app_main_data.iram_code_flag != SHARE_IRAM_BT_CONN_CODE)
	{
		len = __bt_conn_text_ram_end - __bt_conn_text_ram_start;
		CFasm_memcpy(__bt_conn_text_ram_start, __bt_conn_text_rom_start, len*4);
		app_main_data.iram_code_flag = SHARE_IRAM_BT_CONN_CODE;
	}
	#endif
#endif
}

u8 codec_trans_id;
void app_dec_optek_psm_reinit (void)
{
	TX_INTERRUPT_SAVE_AREA;
	
	TX_DISABLE;
	codec_trans_id++;
	TX_RESTORE;	

}

u8 multiroom_debug;

void hci_read_clock_after_stream (void);


void hci_read_bt_hci_clock_sink(void);
void hci_read_bt_hci_clock_src(void);

void hci_read_bt_hci_clock_cb(void)
{
	memset(pSearchData,0,sizeof(struct bt_search_data));
}

u8 bt_clock_src_is_valid(void);

u8 bt_clock_sink_is_valid (void);

/*
	return 0, change to no ntp sync mode
	return 1, continue ntp sync mode
*/
u8 sbc_sync_continue_cb (void)
{

}

/*
	return 1->clock sink is not valid, need continue check
	return 2->clock is not valid with sink clock
	return 0->clock is valid with sink clock
*/

u8 avdtp_check_ntp_cb (u32 nt)
{

}

#define MOBILEPHONE_NTP
#define MOBILEPHONE_A2DP_DELAY	230		//ms
#define MOBILEPHONE_A2DP_DELAY_wo_NTP	(76*512)		/*76*512(2.9) ms*/
#define MAX_MULTIROOM_SYNC_BUF_SIZE		(96*512)			//0x8000			/*96*2.9 ms*/
#define MAX_MULTIROOM_SYNC_BUF_INT_SIZE		(MAX_MULTIROOM_SYNC_BUF_SIZE - 8*512)


/*called by avdtp, return delay report val, unit is 1/10 ms*/
u16 avdtp_get_delay_report_cb(void)
{
	return (MOBILEPHONE_A2DP_DELAY*10);						//1/10ms				
}

u32 dec_get_no_ntp_delay(void)
{
	return MOBILEPHONE_A2DP_DELAY_wo_NTP;						//1/10ms				
}

u32 dec_get_max_multirom_sync_buf_size(void)
{
	return MAX_MULTIROOM_SYNC_BUF_SIZE;						//1/10ms				
}


U32 app_dec_a2dp_stream_write(U8 *ptr, U32 len, int nt, u8 frames) __attribute__ ((section (".internal_ram_1_text")));

void BtHCI_SBC_freq(u8 freq)
{
#if 0//frequancy detect by codec
	if(BtHCI_hfp_is_calling())
		return;

	/* from a2dp, bit 1->48000, bit 1->44100, bit 2->3200 and bit 3->1600 */
	a2dp_sample_freq = freq;
	if (freq == 0x02) {
		//audio_pll_set(SR_44100_SPDIF_1x);
		hw_audio_pll_clk_fre_set(SF_BASE_44100,SPDIF_SF_1x);
		DBG_Printf("SBC sample freq 44100\n\r");
	}
	else if (freq == 0x01) {
		//audio_pll_set(SR_48000_SPDIF_1x);
		hw_audio_pll_clk_fre_set(SF_BASE_48000,SPDIF_SF_1x);
		DBG_Printf("SBC sample freq 48000\n\r");
	}
	else {
		//audio_pll_set(SR_32000_SPDIF_1x);
		hw_audio_pll_clk_fre_set(SF_BASE_32000,SPDIF_SF_1x);
		DBG_Printf("SBC sample freq 32000\n\r");
	} 

	//otk_psm_conn_ready ();
#endif
}

/*
	if nt = -1, no time stamp
*/
extern int decode_type;
U32 app_dec_a2dp_stream_write(U8 *ptr, U32 len, int nt, u8 frames)
{
	//printf_info("len:%d, frames:%d\n", len, frames);
	U32 xlen;

#ifdef BT_HFP_ENABLE
	if(BtHCI_hfp_is_calling())
		return 0;
#endif	

//	if (app_main_data.playing_stream_status == STREAM_INIT)
//		return;

//	if(app_main_data.media != MEDIA_BT_HCI)
//		return 0;

	//if (decode_type != DECODE_SBC)
	//	return;
	
	if (AUDIO_FIFO_STREAM_IS_VAILD() == FALSE)
	{
		return 0;
	}
#if 0
	u32 fifo_wlen;
	BT_TIME_STAMP bt_time_stamp;


	if (bt_multiroom_stream_enable == 0)
		return;

	

	if (app_main_data.media != MEDIA_BT_HCI)
	{
		return 0;
	}

	if(hfp_ring||hfp_calling)
		return;

	if (bt_clock_sink_is_valid() == FALSE) {
		//DBG_Printf ("Sink clk is invalid\n\r");
		return 0;
	}

	//DBG_Printf ("SBC pack len %d\n\r", len);
	//return;

	if (wCodecBusy == FALSE) {
		//waiting data
		if (len < 160) {
			//only one frame 
			return 0;
		}
	}

	if (nt != -1) {

#ifdef MOBILEPHONE_NTP
	
				
#if 0			//checking abs delay
		otk_stream_cn++;
		//if (otk_stream_cn > 400  && otk_stream_cn < 408) {
		if (otk_stream_cn % 200 == 0) {
			u32 optek_ts;				/*625/8=78.125us/oneunit*/
			u32 optek_ts_ms;

			optek_ts = bt_clock_read_sink();
			optek_ts_ms = ((optek_ts/8) * 5)/8;

			otk_stream_cn = 1;
			//should be synced with Bluetooth Timer, but now only 0?
			DBG_Printf ("NTP Sink 0x%x(RTP TS) 0x%x(BT CLK)\n\r",nt,bt_clock_read_sink());
			//DBG_Printf ("NTP Sink 0x%x(DT) 0x%x(BT)\n\r",nt,hci_clk_sink_debug);
			DBG_Printf ("A2DP Stream abs delay %d\n\r", (optek_ts_ms-nt));
		}
#endif

		nt += MOBILEPHONE_A2DP_DELAY;
		//nt = ((nt * 16 *4)/5);			//optek bt time
		nt = (int) ((((unsigned int)nt * 32)/5) * 2);				//nt is about 27 bit
		nt &= 0x3FFFFFFF;						//30 bit local bt clock

#else		//MOBILEPHONE_NTP
		
		nt = 0;			//study later

#endif		//MOBILEPHONE_NTP
	}
	else {
		nt = 0;
	}


	if ( (sdp_get_mask() & BT_APP_NEED_MASTER) != 0x0) {
		//device is master, no ntp
		nt = 0;
	}

	bt_time_stamp.time = nt;
	bt_time_stamp.frame = frames;
		
	fifo_wlen = fifo_put_data(&multirom_clock_fifo,&bt_time_stamp,sizeof(BT_TIME_STAMP));
		DBG_Assert (fifo_wlen == sizeof(BT_TIME_STAMP));


	if (sink_stream_started == 0) {
		sink_stream_started = 1;
		sink_stream_decoding_start_time = nt;
	}
#endif

#if 1	
	xlen = (U32) AUDIO_FIFO_STREAM_PUT_DATA (ptr, len);

	if (xlen != len)
	{
		//asm ("break 1,1");
		DBG_Printf("A2DP Stream overflow\n\r");
		//codec err, dead???

		//codec err, dead???
		//ukMsgSend(app_nav_bt_codec_reinit);			
		//return;
	}
#endif

#if 1
	xlen = (U32) AUDIO_FIFO_STREAM_DATA_LEN ();

	if (audio_env.playing_stream_status == STREAM_DEC_INIT || audio_env.playing_stream_status == STREAM_WAITING_DATA) 
	{
		if (xlen > AUDIO_FIFO_STREAM_FIFO_LEN()*1/4)
		{
			//app_dac_mute_disable();
			sbc_decocde_start_w_init();
			audio_env.playing_stream_status = STREAM_MEDIA;
		}
	}
#endif

	return 0;
}

void app_nav_bt_connect(void)
{
	printf_info("bt connecting\n");
#if 0	
	if (bt_get_hp_status() == TRUE) {
		//keep current status
		return;
	}

	if (app_main_data.standby_status)
	{
		app_main_data.media = MEDIA_BT_HCI;
		app_nav_standby_release();
	}
	else
	{
		app_nav_rmt_convert_bt_hci();
	}
#endif	
}

void app_nav_bt_medial_fini(void)
{
	printf_info("bt connecttion finish\n");
}

//callback from bt stack
void avdtp_rtp_seq_not_continouse_cb (void)
{

}

u8 hci_write_voice_setting_cvsd_cb (void)
{
#if 1//def	CVSD_CODEC_BY_OPTEK	
	return 0x01;					//cvsd enable
#else
	return 0x0;					//cvsd disable
#endif
}

u8 bt_hci_get_multi_link_max_conn (void)
{
	return 1;
}

void hci_LDON_High (void)
{
//	RADIO_LDON;
}

void hci_LDON_Low(void)
{
	//RADIO_LDOFF;
}

U8 app_cmd_first_power_on (void)
{
	return TRUE;				/*first power on*/	
	//return FALSE;			/*not first power on*/	
}

//unit is 625us
u16 optek_psm_flash_time_cb (void)
{
	return (60);
}

#define OTK_PSM_HEADER_SIZE		13			//same as RTP header, should be same as otk_psm.c

void otk_psm_head_vol (void);
void otk_psm_head_eq (void);
void otk_psm_head_srf (u16 srf);

extern U16 pre_audio_freq;


typedef struct {
	unsigned char cmd				:	1;
	unsigned char volume			:	7;

	unsigned char node				:	6;				/*0-63*/
	unsigned char media				:	2;				/*aux 0, bt 1*/

	unsigned char eq				:	4;
	unsigned char audio_sf			:	4;				/*should be 44100 or 48000?*/
	unsigned char mute				:	1;
	unsigned char codec_trans_id	:	7;
} CSRC_HEADER __attribute__((packed));

extern u32 i_samp_freq;


//the following three functions are called by sbcdec for using pcm down sample enc later
u32 Optek_psm_stream_send_for_BT(void *data, u32 len, u32 srf);
void Optek_psm_set_send_package_len_by_frame_len (u16 len);
void Optek_psm_send_trigger_by_sbcdec (void);

void Optek_psm_stream_tx_by_sbcdec(u8 *buf, u16 len)
{
}

void Optek_psm_set_send_package_len_by_frame_len_by_sbcdec(u16 len)
{
}


void Optek_psm_send_trigger_by_sbcdec (void)
{
}


u8 clock_diff_err_num;

void Optek_psm_stream_pll_adj (i32 diff) 
{

}

#define MIN_NTP_MARGIN	(512*6*2)		//about 1 a2dp stream

//only for bt src
U32 Optek_psm_stream_ntp_cb (void)
{
	return 0;
}

#define BT_NODE_DELAY 0//50			//50*625/8 = 3.9ms
u16 rx_2_tx_debug = 0;

void Optek_psm_rx_2_tx_stream_header_cb (u8 *pbuf)
{

}


void Optek_psm_stream_header_cb (u8 *pbuf, u32 ntp_time)
{
	pbuf[0] = 0;
	pbuf[1] = 0;
	pbuf[2] = 0;
	pbuf[3] = 0;
	pbuf[4] = 0;
	pbuf[5] = 0;
	pbuf[6] = 0;
	pbuf[7] = 0;
}


void otk_psm_ui_send_err(void);

u8 bt_clock_sink_is_valid (void);

U32 app_dec_multi_speaker_stream_write(U8 *ptr, U32 len)
{

}

void app_dec_sbc_adjust_fini (void)
{

}


/* page: 0 page and discover off, 1 discover on and page off, 2 discover off and page on, 3 discover on and page on*/
/* fast: 0 fast connection disable, 1 fast connection disable*/
void kBtHCI_page(u8 page, u8 fast);
void app_bt_set_scan_enable_mode(U8 scan_mode)
{
	switch (scan_mode)
	{
		case enBT_BOTH_SCAN_DISABLE:
			DBG_Printf ("non-connectable and non-discaverable\n\r");
			kBtHCI_page(0,0);
			break;
		case enBT_INQUIRY_SCAN_ENABLE:
			DBG_Printf ("discaverable\n\r");
			kBtHCI_page(1,1);
			break;
		case enBT_PAGE_SCAN_ENABLE:
			DBG_Printf ("connectable\n\r");
			kBtHCI_page(2,0);
			break;
		case enBT_BOTH_SCAN_ENABLE:
			DBG_Printf ("connectable and discaverable\n\r");
			kBtHCI_page(3,1);
			break;
		default:
			DBG_Printf ("non-connectable and non-discaverable\n\r");
			kBtHCI_page(0,0);
			break; 
	}
	bt_status.bt_scan_mode = scan_mode;
}


//for forcing bt diconnect
U8 bt_force_disconnect = FALSE;

//for forcing bt reconnect
U8 bt_froce_pairing;

//for judging connect type
U8 bt_connect_type = 0;

U8 bt_auto_diconnect = FALSE;
U8 bt_auto_connect_from_source;
U8 bt_avrcp_tick = 0;

/************************************/
/************************************/
u8 a2dp_sample_freq = 2;
extern u8 g_rx_present;

u8 hp_conn_req;

//U8 bt_hci_main_status;
//U8 bt_hci_play_status;
/* play status */
/*
#define AVRCP_PLAY_STATUS_STOPPED   0x00
#define AVRCP_PLAY_STATUS_PLAYING   0x01
#define AVRCP_PLAY_STATUS_PAUSED    0x02
#define AVRCP_PLAY_STATUS_FWD_SEEK  0x03
#define AVRCP_PLAY_STATUS_REV_SEEK  0x04
#define AVRCP_PLAY_STATUS_ERROR     0xFF
#define AVRCP_PLAY_STATUS_UNKNOWN   0xFE			//added by Optek
*/


u8 avrcp_get_status (void);
int hci_Optek_PSM_conn_present(void);
int hci_A2DP_conn_present(void);

void uiBtHCI_Optek_module_link(u8 chan);
void uiBtHCI_Optek_module_tx_link(u8 chan);
u8 bt_flash_is_btaddr_present(void);
u8 bt_flash_is_multi_speaker_btaddr_present(void);
void kBtHCI_adv_enable(void);
void kBtHCI_adv_disable(void);
extern U8 NVDS_buffer[0x800];

//#define FIXED_BT_MAC_ID
#ifdef FIXED_BT_MAC_ID
//const u8 tx_bdaddr[6] = {0x2c,0x40,0x65,0x3F,0x9f,0x94};
const u8 tx_bdaddr[6] = {0x75,0xB8,0x80,0x3F,0x9f,0x94};
#endif
void app_bt_init(void)
{
	U32 cnt = 0;
	u8 optek_link_mode;
	u16 optek_link_max_m_tx_len;
	u16 optek_link_max_m_rx_len;

	DBG_Printf("hci ver:%s,build time:%s %s\r\n",bt_host_version_get(),bt_host_time_get(),bt_host_data_get());
	
	//debug
//	malloc_init();

	memset(&bt_status,0,sizeof(bt_status));

	hci_dev_init();
//	Optek_psm_init();

	//init sdp db
	sdp_init();
#ifndef BT_TX_ROLE_ENABLE
	sdp_set_mask(SDP_A2DP_SNK_MASK);
	
#ifdef BT_VOLUME_SYC
	sdp_set_mask(SDP_AVRCP_CT_MASK);
	sdp_set_mask(SDP_AVRCP_TG_MASK);
	sdp_set_mask(AVRCP_CT_VOL_SYNC_MASK);
#endif	

#ifdef BT_HFP_ENABLE
	sdp_set_mask(SDP_HFP_HF_MASK);
	sdp_set_mask(HFP_HF_BATT_LEV_MASK);//optional   , report local batt lev to phone 
#endif

#ifdef BT_AUTOPLAY_ENABLE
	sdp_set_mask(AVRCP_CT_AUTOPLAY_MASK);
#endif
#endif
	//sdp_set_mask(BT_NAME_EXT_MASK);
	//sdp_set_mask(BT_APP_NEED_MASTER_MASK);
	
	global_vari.optek_link_role = optek_link_get_role_w_check();

#ifdef OPTEK_LINK_ENABLE
	if (FALSE == optek_link_is_disable())
		sdp_set_mask(BLE_EXT_ADV_MASK);
#endif	

#ifdef SAVE_MODULE_FROM_EEPROM
#ifdef BT_W_EEPROM
	sdp_set_mask(BT_w_EEPROM_MASK);
	if ( (sdp_get_mask() & BT_w_EEPROM_MASK) != 0x0)
	{
		//init bt eeprom data
		void app_nav_get_bt_data(void);
		app_nav_get_bt_data();
	}
#endif
#endif

	app_nav_bt_tx_init();

	record_list_init();

	hp_conn_req = 0;			

	bt_status.bt_main_status = enBT_OPENING;

	U8 *p;

	#ifdef FIXED_BT_MAC_ID
	p = tx_bdaddr;
	#else
	p = bt_flash_get_btaddr();
	#endif
	
	CFasm_memcpy(&NVDS_buffer[7],p,6);

	DBG_printf("mac addr:%m\r\n",&NVDS_buffer[7]);

retry:
	DBG_Printf("bt hci init start\r\n");	
	ht_hci_init ();
	if (ht_hci_open () != 0) {
		if (cnt++ < 3)
		{
			DBG_Printf ("BT HCI open err, retry again\n\r");
			goto retry;
		}
		bt_status.bt_main_status = enBT_ERROR;
		DBG_Printf("bt hci init fail!\r\n");
		DBG_Assert (FALSE);
	}
	else {
		ht_hci_set_multi_speaker_mode(BT_NO_MULTI_SPEAKER);

	#ifdef OPTEK_LINK_ENABLE
		#ifdef BLE_ENABLE
		if (optek_link_is_disable())
			kBtHCI_adv_enable();
		#endif
	#else
		#ifdef BLE_ENABLE
		kBtHCI_adv_enable();
		#endif
	#endif

		decode_frame_samples_set(0);

		#ifdef OPTEK_LINK_ENABLE
		if (FALSE == optek_link_is_disable())
		{
			delayms(20);//wait for printf
			app_optek_link_init();
			#ifdef OPL_MODE_TWO_MIC
			decode_frame_samples_set(120);
			optek_link_mode = OPTEK_LINK_MODE_MICx2_PT2P5MS;
			#elif (defined OPL_MODE_GAME_HP)
			decode_frame_samples_set(240);
			optek_link_mode = OPTEK_LINK_MODE_USB_PT5MS_S1;
			#elif (defined OPL_MODE_GAME_HP_LOW_LATENCY)
			decode_frame_samples_set(120);//2.5*48
			optek_link_mode = OPTEK_LINK_MODE_USB_PT2P5MS_S1;			
			#elif (defined OPL_MODE_GAME_HP_w_MIC)
			decode_frame_samples_set(240);
			optek_link_mode = OPTEK_LINK_MODE_USB_PT5MS_S1_w_Up_Voice;
			#elif (defined OPL_MODE_GAME_2HP_w_MIC)
			decode_frame_samples_set(360);
			optek_link_mode = OPTEK_LINK_MODE_USB_PT7P5MS_S2_w_Up_Voice;			
			#else

			decode_frame_samples_set(240);
			//optek_link_mode = OPTEK_LINK_MODE_MICx2_PT2P5MS;
			//optek_link_mode = OPTEK_LINK_MODE_MICx2_PT5P442MS;					//TODO			
			//optek_link_mode = OPTEK_LINK_MODE_USB_PT5MS_S1;			
			//optek_link_mode = OPTEK_LINK_MODE_USB_PT2P5MS_S1;		
			//optek_link_mode = OPTEK_LINK_MODE_USB_PT2P5MS_S1_w_HFP;
			//optek_link_mode = OPTEK_LINK_MODE_USB_PT7P5MS_S1;			
			optek_link_mode = OPTEK_LINK_MODE_USB_PT7P5MS_w_HFP_S1;		
			//optek_link_mode = OPTEK_LINK_MODE_USB_PT5MS_S1_w_Up_Voice;			
			//optek_link_mode = OPTEK_LINK_MODE_USB_PT7P5MS_S2_w_Up_Voice;

			//optek_link_mode = OPTEK_LINK_MODE_USB_PT5MS_BC;
			//optek_link_mode = OPTEK_LINK_MODE_USB_PT5MS_BC_6T;
			//optek_link_mode = OPTEK_LINK_MODE_USB_PT2P5MS_BC;

			//optek_link_mode = OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S1;
			//optek_link_mode = OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S2;
			//optek_link_mode = OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S3;


			#endif

    		optek_link20_get_master_tx_rx_len (optek_link_mode, &optek_link_max_m_tx_len, &optek_link_max_m_rx_len);
			app_change_mode_req(optek_link_mode,(u8) optek_link_max_m_tx_len,(u8) optek_link_max_m_rx_len);

			//app_optek_link_open();

			//app_nav_optek_link_pairing(); 
		}
		#endif
		bt_status.bt_main_status = enBT_OPENED;
		bt_status.bt_avrcp_status = enBT_AVRCP_UNKNOWN;
		DBG_Printf("bt hci init success\r\n");

		app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_DISABLE);

	}
}

void app_nav_detach(void)
{

}

void a2dp_status_change_notify(u8 status)
{
	bt_status.bt_a2dp_status = status;
	DBG_Printf("a2dp status change notify, new status:%d\r\n",bt_status.bt_a2dp_status);
}

void avrcp_status_change_notify(u8 play_status)
{
	bt_status.bt_avrcp_status = play_status;
	DBG_Printf("play status change cb new status:%d\r\n",bt_status.bt_avrcp_status);
}

void avrcp_track_change_notify(void)
{
	asm ("NOP");
}

/*
int app_nav_bt_status_get(void)
{
	int temp;
	int ret;

	temp = avrcp_get_status ();

	if (bt_status.bt_avrcp_status != temp)
	{
		bt_status.bt_avrcp_status = temp;
		DBG_Printf("detect avrcp new status:%d\r\n",bt_status.bt_avrcp_status);
	}

	return bt_status.bt_avrcp_status;
}
*/

static const char *metadata_to_str(u32 id)
{
#if 0
	switch (id)
	{
	case AVRCP_MEDIA_ATTRIBUTE_TITLE:
		return "Title";
	case AVRCP_MEDIA_ATTRIBUTE_ARTIST:
		return "Artist";
	case AVRCP_MEDIA_ATTRIBUTE_ALBUM:
		return "Album";
	case AVRCP_MEDIA_ATTRIBUTE_GENRE:
		return "Genre";
	case AVRCP_MEDIA_ATTRIBUTE_TRACK:
		return "TrackNumber";
	case AVRCP_MEDIA_ATTRIBUTE_N_TRACKS:
		return "NumberOfTracks";
	case AVRCP_MEDIA_ATTRIBUTE_DURATION:
		return "Duration";
	}
	
	return NULL;
#endif
}

//please note, this function is called by BT stack
void avrcp_parse_attribute_list_cb(u8 *operands, u8 count)
{

}


//debug
void app_cmd_ble_key_pressed(u8 key)
{

}

void app_cmd_ble_key_released(u8 key)
{
}


//debug
void app_cmd_ble_debug_vol(u8 level)
{

	
}

void BtHCI_avrcp_conn(void)
{
	bt_avrcp_tick = 0;
	DBG_Printf ("avrcp connected\n\r");
}

void avrcp_remote_vol_up (void)
{
#ifdef BT_VOLUME_SYC
	DBG_Puts("bt avrcp rmt vol+\n\r");
	app_audio_device_volume_set(TRUE);
#endif
}

void avrcp_remote_vol_down (void)
{
#ifdef BT_VOLUME_SYC
	DBG_Puts("bt avrcp rmt vol-\n\r");
	app_audio_device_volume_set(FALSE);
#endif
}

/* AVRCP absolute vol shoud be 0-127*/
void avrcp_remote_absolute_vol (u8 absolute_vol)
{	
#ifdef BT_VOLUME_SYC
	u8 vol;

	DBG_Puts("vol syc from bt device\n\r");
	if (bt_avrcp_tick < 2) {
		DBG_Printf("bt remote_absolute_vol:%d\n\r", bt_avrcp_tick);
		bt_avrcp_tick++;
		return;
	}

	if (absolute_vol > 127) {
		absolute_vol = 127;
	}

	vol = (absolute_vol * VOLUME_MAX + 126) / 127;
	app_cmd_bt_absolute_vol(vol);
#endif
}

/* AVRCP hp vol change*/
void avrcp_hp_vol_change_cb (u8 absolute_vol)
{	
#ifdef BT_VOLUME_SYC
	u8 vol;
	
	DBG_Printf("hp vol change:%d\n\r", absolute_vol);
	
	if (absolute_vol > 127) {
		absolute_vol = 127;
	}
	
	vol = (absolute_vol * VOLUME_MAX + 126) / 127;
	audio_vari.volume = vol;
#endif
}

//
void app_cmd_bt_absolute_vol(U8 vol)
{
	DBG_Printf("%s\r\n",__func__);
#ifdef BT_VOLUME_SYC
	int temp;
	int i;
#if 1
	if (vol == audio_vari.volume)
	{
	}
	else if (vol > audio_vari.volume)
	{
		temp = vol -  audio_vari.volume;
		do
		{
			app_audio_device_volume_set(TRUE);
			temp --;
		} while (temp);
	}
	else
	{
		temp = audio_vari.volume -  vol;
		do
		{
			app_audio_device_volume_set(FALSE);
			temp --;
		} while (temp);
	}
#else
	audio_vari.volume = vol;

	#ifdef AUDIO_CODEC_USED_VOL
	AUDIOdevice.Set_Volume(audio_vari.volume);
	#endif
	#ifdef AUDIO_AMP_USED_VOL
	for (i=0;i<AMP_DEV_COUNT_MAX;i++)
	{
		AudioAmpDev[i].Set_Volume(audio_vari.volume);
	}
	#endif
#endif
#endif
}

void app_cmd_bt_vol_up(U8 up)
{
#if 0	
	//need more stuty
	if (up == TRUE) {
		if (audio_vari.volume < VOLUME_MAX) {
			audio_vari.volume++;
		}
	}
	else {
		if (audio_vari.volume > 0) {
			audio_vari.volume--;
		}
	}
	
	#ifdef AUDIO_CODEC_USED_VOL
	AUDIOdevice.Set_Volume(audio_vari.volume);
	#endif
	#ifdef AUDIO_AMP_USED_VOL
	for (i=0;i<AMP_DEV_COUNT_MAX;i++)
	{
		AudioAmpDev[i].Set_Volume(audio_vari.volume);
	}
	#endif
#else
	DBG_Printf("%s\r\n",__func__);
	app_audio_device_volume_set(up);

#endif	
}

void app_nav_bt_hci_play(void)
{
	DBG_Puts("app nav bt paly\n\r");
	kBtHCI_play();
}

void kBtHCI_next_up(void)
{
	DBG_Printf("kBtHCI_next_up\n\r");
	if (0)//(key_process_envs.key_hold_counts > 0)
	{
		//key_hold_counter = 0;
		kBtHCI_ffd_release();
	}
	else {
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

}

void kBtHCI_prev_up(void)
{
	DBG_Printf("kBtHCI_prev_up\n\r");
	if (0)//(key_process_envs.key_hold_counts > 0)
	{
		//key_hold_counter = 0;
		kBtHCI_fbw_release();	
	}
	else {
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

}

void kBtHCI_next_hold(void)
{
	kBtHCI_ffd_press();
	//key_hold_counter++;
}

void kBtHCI_prev_hold(void)
{
	kBtHCI_fbw_press();
	//key_hold_counter++;
}



void BtHCI_a2dp_conn(void)
{
	DBG_Printf("bt conn success\r\n");
	bt_status.bt_main_status = enBT_CONNECTED;
	// app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_DISABLE);
	app_bt_set_scan_enable_mode(enBT_PAGE_SCAN_ENABLE);
	if(global_vari.audio_source != SOURCE_BT)
	{
		enter_fix_source(SOURCE_BT);
	}

}

void BtHCI_disconn(u32 link_loss);
void BtHCI_disconn_one(u32 link_loss)
{
	DBG_Printf ("disconn one\n\r");

	BtHCI_disconn(link_loss);
}

void BtHCI_disconn(u32 link_loss)
{
//app_timer_relink_monitor_time_clr();
//	if (app_main_data.tws_role == TWS_R) {
//		return;	
//	}
	DBG_Printf("bt disconnet\r\n");

	if (link_loss == TRUE) {
		DBG_Printf ("A2DP link loss\n\r");
	}
	else
	{
		//if (bt_hci_device_conns)
		//	app_prompt_tone_1_play();
	}

#ifdef BT_RELINK_ENABLE
	if(link_loss && (bt_status.bt_main_status == enBT_CONNECTED))
	{
		DBG_KEYPrintf("link_loss ==%d\n",link_loss);
		app_task_timer_ms_set(TIMER_BT_Relink, 1000);
	}
#endif	
	if (global_vari.audio_source == SOURCE_BT)
	{
		app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE);
	}
	else
	{
		// app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_DISABLE);
		app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE);//just for normal bt
	}
	
	bt_avrcp_tick = 0;
	
	bt_status.bt_main_status = enBT_PAIRING;
	bt_status.bt_avrcp_status = enBT_AVRCP_UNKNOWN;	

	// enter_fix_source(SOURCE_AUX);
	
#if 0//def BT_ID3_DISPLAY_EANBLE
	track_id3_ready = FALSE;
	
	memset( id3_tag.title, 0, (MAX_ID3_TEXT + 1)*2);
	memset( id3_tag.artist, 0, (MAX_ID3_TEXT + 1)*2);
	memset( id3_tag.album, 0, (MAX_ID3_TEXT + 1)*2);
#endif
	
	
}

#if 0
void BtHCI_set_dec(u32 freq)
{
	if (freq == 44100) 
	{
		audio_pll_set(SR_44100_SPDIF_1x);
		DBG_Printf("SBC sample freq 44100\n\r");
	}
	else if (freq == 48000) 
	{
		audio_pll_set(SR_48000_SPDIF_1x);
		DBG_Printf("SBC sample freq 48000\n\r");
	}
	else if (freq == 32000) {
		audio_pll_set(SR_32000_SPDIF_1x);
		DBG_Printf("SBC sample freq 32000\n\r");
	}
	else {
		DBG_Assert (FALSE);
		DBG_Printf("SBC sample freq err\n\r");
	}
}
#endif

/*
define in a2dp_codec.h
#define A2DP_CODEC_SBC			0x00
#define A2DP_CODEC_MPEG12		0x01
#define A2DP_CODEC_MPEG24		0x02
#define A2DP_CODEC_ATRAC		0x03
#define A2DP_CODEC_VENDOR		0xFF
*/

void BtHCI_set_dec(u32 val)
{
#if 0	
	U8 dec;
	U8 freq;
	int src_freq;
	U8 *pStreamBuf;
	U32 size;
	
	if (BtHCI_hfp_is_calling())
	{
		return;
	}
	
	//if (bt_get_hp_status() == TRUE)
	//	return;

	//app_dac_mute_enable();

	dec = (val & 0xFF);
	freq = ((val & 0xFF00) >> 8);

	a2dp_sample_freq = freq;
	if (freq == 0x02)
	{
		//src_freq = SF_BASE_44100;
		src_freq = 44100;
		DBG_Printf("SBC sample freq 44100\n\r");
	}
	else if (freq == 0x01)
	{
		//src_freq = SF_BASE_48000;
		src_freq = 48000;
		DBG_Printf("SBC sample freq 48000\n\r");
	}
	else
	{
		//src_freq = SF_BASE_32000;
		src_freq = 32000;
		DBG_Printf("SBC sample freq 32000\n\r");
	} 

	app_audio_clock_freq_setting(src_freq);
	
	app_cmd_DecoderExit();	

	//at current time, only support SBC
	DBG_Assert (dec == 0x0);
	uDecSend (DECODE_SET, DECODE_SBC);			

	//app_main_data.media = MEDIA_UNKNOWN;			//stop write a2dp stream

#ifdef STREAM_BUFFER_SIZE_UNFIXED
	pStreamBuf = BT_STREAM_START;
	size = BT_STREAM_END - BT_STREAM_START;
#else
	pStreamBuf = stream_buffer;
	#ifdef STREAM_BUFFER_USED_POINTER
	size = STREAM_BUF_MAX_SIZE;
	#else
	size = sizeof(stream_buffer);
	#endif
#endif

	AUDIO_FIFO_STREAM_FLUSH();
	//fifo_file_stream_flush(&audio_env.stream_fifo)

	AUDIO_FIFO_STREAM_OPEN(pStreamBuf, size);
	//fifo_init(&audio_env.stream_fifo, pStreamBuf, size, TRUE);

	AUDIO_FIFO_STREAM_CREATE(0,STREAM_LENTH_UNKNOWN,0,0);
	//fifo_create_file_stream(&audio_env.stream_fifo, 0, STREAM_LENTH_UNKNOWN, 0, 0);

	app_main_data.playing_stream_status = STREAM_DEC_INIT;

	//app_main_data.playing_stream_status = STREAM_WAITING_DATA;
	//app_main_data.media = MEDIA_BT_HCI;
#endif	
}

void uiBtHCI_set_pause (U32 sess);

void bt_hci_set_pause (void *sess)
{
}

void bt_hci_pasue_cb(void *sess)
{
	
}

/*iOS 9.21 bug, no avrcp play/pause*/
void BtHCI_a2dp_stream_start_cb(void)
{
	/*clear dec err count*/
}

#define RELINK_CYC_TIMER	3
U8 relink_cyc_timer = RELINK_CYC_TIMER;

void BtHCI_a2dp_relink_fail(void)
{
	DBG_Printf ("Relink Fail\n\r");
	#ifdef BT_RELINK_ENABLE
	
	if (relink_cyc_timer)
	{
        relink_cyc_timer--;
		DBG_Printf ("relink_cyc_timer:%d\n\r",relink_cyc_timer);
		if(global_vari.audio_source == SOURCE_BT)
		{
			app_task_timer_ms_set(TIMER_BT_Relink, 3000);
		}
		else
		{
			app_task_timer_ms_set(TIMER_BT_Pairing, 500);
			relink_cyc_timer = 0;
		}	
	}
	else
	{
		bt_status.bt_main_status = enBT_PAIRING;
		app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE);		
	}
	#endif
}

void uiBtHCI_Optek_module_link(u8 chan);

void app_nav_BT_manual_pair(void)
{
#if 1
	DBG_Puts("BT manual pair enter\n\r");
	app_window_timer_count_init();
	
	bt_status.bt_main_status = enBT_DISCONNECTED;

	bt_hci_flush();
#endif

}

void bt_flash_data_init(void);
U8 bt_pairing_info_not_none(void)
{
	U8 *p; 
	bt_flash_data_init();
	p = bt_flash_get_relink_btaddr(0);
	if (((p[3]==0) && (p[4]==0) && (p[5]==0))||((p[3]==0xFF) && (p[4]==0xFF) && (p[5]==0xFF)))
	{
//		DBG_Puts("No pairing info\n\r");
		return FALSE; 
	}
	else
	{
		return TRUE;
	}
}

void app_nav_bt_relink(void)
{
#ifdef BT_RELINK_ENABLE
	DBG_Puts("BT reLink\n\r");

	if (global_vari.audio_source == SOURCE_BT)
	{
		if (bt_status.bt_main_status != enBT_CONNECTED)
		{
			DBG_Puts("BT reLink1\n\r");
			bt_status.bt_main_status = enBT_RELINK;
			uiBtHCI_relink();
		}
	}
#endif
}

u16 bt_hci_conn_cb (u8 *btaddr)
{
	DBG_Printf("rev bt conn req\r\n");
	app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_DISABLE);
	//app_bt_set_scan_enable_mode(enBT_PAGE_SCAN_ENABLE);		
	return TRUE;
}

/*
	return	0 -> TWS/Multispk Master/ Normal
	return	1 -> TWS Slave	
	return  2 -> Mutlispk Slave
*/
u16 bt_hci_get_multispk_role_cb (void)
{
	if (1){//(app_main_data.tws_role == TWS_L) {		
		return 0;	
	}		
	else {
		//return 1;			//multispk slave
		return 2;			//tws slave	
	}
}

u32 bt_hci_rx_conn_cb(void)
{
	if (1)
		return TRUE;
	else
		return FALSE;
}

u32 bt_hci_new_rx_conn_cb(void)
{
	//if (accept new subw conn)
	if (1)
		return TRUE;
	else
		return FALSE;
}

void otk_psm_disconn (void)
{	
}

void BtHCI_OTK_PSM_disconn(void)
{

}

void BtHci_OTK_PSM_tx_fatal_err(void)
{
}
void BtHCI_OTK_PSM_stream_tx_disconn(void)
{

		
}

/*
if link_loss is 0, uplink power off
if uplink want power off downlink, please call the following function first 
void uiBtHCI_Optek_module_tx_close(void) then bt_hci_flush_multi_speaker() or bt_hci_flush_all()
*/

void BtHCI_OTK_PSM_stream_rx_disconn(u32 link_loss)
{
}

void otk_psm_conn_ready (void)
{

}

void otk_psm_conn_stream_tx_ready (void)
{

}

void otk_psm_conn_stream_rx_ready (void)
{
}


void bt_tx_test_1(void)
{
}

void bt_tx_test_2(void)
{
}

u8 Bt_HCI_get_ui_vol(void)
{
}

u8 Bt_HCI_get_ui_eq(void)
{
}

u8 bt_hci_set_subw_eq(void)
{
}


void bt_hci_32k_sim (void)
{
}


/*
	Optek psm intefare
*/
void otk_psm_head_vol (void)
{
}

void otk_psm_head_eq (void)
{
}

void otk_psm_head_srf (u16 srf)
{
}

void otk_psm_ui_vol (u8 vol)
{
 }

void otk_psm_ui_eq (u8 eq_bass)
{
}


void otk_psm_ui_slave_srf(u8 srf)
{

}
void otk_psm_ui_mute(u8 mute)
{
}
void otk_psm_ui_poff (void)
{
}

void otk_psm_conn_ok (void)
{
	
}

/*0 master, 1 slave*/
void otk_psm_role_switch (u8 rol)
{
	//DBG_Assert (FALSE);
}


void BtHCI_otk_psm_conn_fail(void)
{

}

void BtHCI_otk_psm_tx_conn_fail(void)
{
}


void BtHCI_otk_psm_no_macid(void)
{
}

void BtHCI_conn_complete_cb(u8 status)
{

}

void BtHCI_OTK_PSM_conn(void)
{

}

#if 0
U32 misSysCLK1p25ms (void)
{

}

U32 misSysCLK2BtClk (void)
{

}
#else

U32 misSysCLK1p25ms (void)
{
	return ((32000*5)/4);						//96000->1ms
}

U32 misSysCLK2BtClk (void)
{
	return (((32/2) * 625)/4);						//(96/312.5)
}
#endif

void avdtp_stream_start_cb (void)
{
	Optek_psm_set_send_package_len(109*6,6);
}

static const u8 pin_code[16] = {'0','0','0','0','x','x','x','x','x','x','x','x','x','x','x','x'};

const U8 *optek_hci_pin_code_cb(void)
{
	return (const U8 *)pin_code;
}

void hci_bt_clock_sink(void)
{
}

void hci_bt_clock_src(void)
{
}

U8 bt_clock_sink_set_invalid(void)
{
	return FALSE;
}

U8 bt_clock_src_is_valid(void)
{
	return FALSE;
}


/*
 ****************************************************************************************
 * @brief calculate drift ratio between bt master and slave(local).
 *
 * @param[out]  driftR:  drift ratio,unit is ppm
 * 
 * @return status      1:success | 0:fail
 *
 ****************************************************************************************
 */
int bt_slave_cal_driftR(double *driftR)
{
#if 0	
    u32 base_cnt_rxsync_1st;
    u16 fine_cnt_rxsync_1st;
    u32  base_cnt_rxsync_last; 
    u16 fine_cnt_rxsync_last;
    i32 swap_cn;	

    u8 succ;
    i32 drift;
    u32 dis_hs;
    u32 abs_swap_hs;

    succ = ld_acl_get_slave_time_sync_info (&base_cnt_rxsync_1st, &fine_cnt_rxsync_1st,
                                            &base_cnt_rxsync_last, &fine_cnt_rxsync_last,
                                            &swap_cn);

	//DBG_Printf("base last:%d,base 1st:%d,fine last:%d,fine 1st:%d,swap_cn:%d\n\r",base_cnt_rxsync_last,base_cnt_rxsync_1st,fine_cnt_rxsync_last,fine_cnt_rxsync_1st,swap_cn);

    //10ppm, 1s drift 20 (0.5us step), 10s drift 200 (0.5us step). therefore drift < 312 fro 10s test

	if (succ)
    {
        if (base_cnt_rxsync_last >= base_cnt_rxsync_1st)
            dis_hs = base_cnt_rxsync_last - base_cnt_rxsync_1st;
        else
            dis_hs = base_cnt_rxsync_last + 0x10000000- base_cnt_rxsync_1st;

		dis_hs = dis_hs*625 + fine_cnt_rxsync_last - fine_cnt_rxsync_1st;

		if (dis_hs < 1000000*2/625)
			return 0;


		drift = swap_cn*625 + fine_cnt_rxsync_last - fine_cnt_rxsync_1st;

        *driftR = (double) drift/(double)(dis_hs);

		return 1;
    }
#endif	
	return 0;
}

void get_class_of_device_service(U8 class[3])
{
#if 1//headphone
	class[0] = 0x18;//Minor Device Class:Headphones
	class[1] = 0x04;//Major Device Class:Audio/Video (headset, speaker, stereo, video display etc	
	class[2] = 0x24;//Major Service Class :Rendering (printing, speaker etc)  and Audio (speaker, microphone, headset service etc)	
#else //speaker
	class[0] = 0x14;//Minor Device Class:loudspeaker
	class[1] = 0x04;//Major Device Class:Audio/Video (headset, speaker, stereo, video display etc	
	class[2] = 0x24;//Major Service Class :Rendering (printing, speaker etc)  and Audio (speaker, microphone, headset service etc)	
#endif
}

//u8?ld_acl_get_slave_time_sync_info?

U8 _app_flash_program_user_data(U32 addr, U32 *buf, U32 byte_len)
{
	hal_sflash_write(addr, buf, byte_len);
}


void app_nav_save_bt_data(void)
{
#ifdef SAVE_MODULE_FROM_EEPROM	
	U8 addr;
	void *data;
	u16 len;

	DBG_Printf ("Update BT data\n\r");

	addr = APP_SAVE_BT_MULTI_LINK_PAGE_ADDR;

	data = bt_hci_get_flash_data (&len);
	eeprom_device_Write(addr, data, len);
#else
	DBG_Printf ("err: no define eeprom,cann't not save\n\r");	
#endif
}

void app_nav_get_bt_data(void)
{
#ifdef SAVE_MODULE_FROM_EEPROM	
	U8 addr;
	void *data;
	u16 len;
	
	addr = APP_SAVE_BT_MULTI_LINK_PAGE_ADDR;
	
	data = bt_hci_get_flash_data (&len);
	eeprom_device_Read(addr, data, len);
#else
	DBG_Printf ("err: no define eeprom,cann't not get\n\r");	
#endif	
}

//BLE ui
void bt_hci_ble_conn (void)
{
	DBG_printf("ble connected\r\n");	
}

void kBtHCI_adv_enable(void);

void bt_hci_ble_disconn (void)
{
	DBG_printf("ble disconnected\r\n");
	kBtHCI_adv_enable();
}

extern const u8 ble_name[];

int bt_hci_ble_adv_name_match (u8 *pbuf)
{
}

void app_ble_update_rxdata(U8 *p)
{
}
void app_nav_ble_set_volume(void)
{
}

void app_nav_ble_set_eq(void)
{

}

void app_ble_send_vol(void)
{	


}
void app_ble_send_bass(void)
{

}

void app_ble_send_souce(void)
{

}

void app_ble_send_vm_vol(void)
{	


}


#endif
