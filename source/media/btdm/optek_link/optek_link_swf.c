/**
 ****************************************************************************************
 *
 * @file optek_link_swf.c
 *
 * @brief optek link swf mode implementation.
 *
 ****************************************************************************************
 */

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
#include "optek_link_commons.h"
#include "mem_reloc.h"
#include "codec_malloc.h"
#include "audio_task.h"
#include "audio_codec.h"

#ifdef OPL_MODE_SWF

void xa_pcm_downsample_dec_init(void);

//void this_function_check_elf_file(void) __attribute__((section (".warning_pls_use_elf32xtensa_normal_x")));
void this_function_check_elf_file(void)
{

}

#define FULLBAND_MASTER_TX_LEN 24
#define MASTER_TX_LEN 240
#define SLAVE_TX_LEN  6

#define TXRX_NUM 11 

U8 fullband_slave_rx_len_table[TXRX_NUM];
U8 fullband_slave_tx_len_table[TXRX_NUM];
U8 slave_rx_len_table[8];
U8 slave_tx_len_table[8];

extern u8 master_bdaddr[6];

volatile APK_AND_TX_SYNC_STATUS_ENUM spk_out_sync_w_tx_status;

typedef struct {
	U8 tx_data[MASTER_TX_LEN];
    U8 tx_data1[MASTER_TX_LEN];
    U8 tx_len;
    U8 tx_len1;
	U8 last_tx_len;	
} BC_TX_DATA_t;

BC_TX_DATA_t bc_tx_data;

typedef struct {
	U8 rx_buf[2][MASTER_TX_LEN];
	U8 rx_buf_index;
	U8 rx_data_index;
	U8 rxing_len;
    U8 rx_len;
    U8 loss_flag;	
} RX_DATA_t;

RX_DATA_t rx1_data;

//first element in this table use for default role
static const U8 sw_role_table[] = 
{
	OPL_ROLE_MASTER,
	OPL_ROLE_SLAVE,
};

const SW_ROLE_t  sw_role_info = {sw_role_table,sizeof(sw_role_table)/sizeof(sw_role_table[0])};

void *rx1_decode_hanle;

//FIFO decode_out_fifo;
//#define DECODE_FIFO_SIZE (240*4+512)//10ms buffer + 512bytes


#define DOWNSAMPLE 10 

U8 start_offset;
void *downsampleFilterHandle;

FIFO opl_swf_tx_fifo;
#define TX_FIFO_SIZE (256*4*10)

U16 *enc_inp_buffer;

void optek_link_trans_init(void) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_trans_init(void)
{
	bc_tx_data.tx_len = 0;
	bc_tx_data.last_tx_len = 0;
    bc_tx_data.tx_len1 = 0;
	rx1_data.rx_len = 0;
	rx1_data.rxing_len = 0;
	rx1_data.rx_buf_index = 0;
	rx1_data.loss_flag = 0;
}

void optek_link_codec_init(U16 sample_rate,U8 bt_src)
{
	if (global_vari.optek_link_role == nonOPL_ROLE)
	{
		return;
	}

	if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
	{
		return;
	}

	optek_link_trans_init();

	start_offset = 0;
    fifo_init(&opl_swf_tx_fifo,codec_malloc(TX_FIFO_SIZE),TX_FIFO_SIZE,TRUE);
	enc_inp_buffer = codec_malloc(1152*2);

	#define SUBWOOFER_MAX_BIQUAD	2
	static const int subwoofer_dsp_coef[SUBWOOFER_MAX_BIQUAD*6] = {
		0x40000000,0x001c3611,0x00386c22,0x001c3611,0x87ba88d1,0x38b64f74,
		0x40000000,0x001c3611,0x00386c22,0x001c3611,0x87ba88d1,0x38b64f74,
	};

	downsampleFilterHandle = optek_bqriir_open(codec_malloc(optek_bqriir_alloc(SUBWOOFER_MAX_BIQUAD)),SUBWOOFER_MAX_BIQUAD,subwoofer_dsp_coef);
	if (downsampleFilterHandle == NULL) {
		DBG_Assert(FALSE);
	}

	if (global_vari.optek_link_role == OPL_ROLE_MASTER)
	{
		if (decode_type_get() == DECODE_SBC)
		{
			if (audio_in_stream.sample_rate == 48000)
			{
				app_change_mode_req(TWS_SUBW_W_BT_T6_SF48K,245,6);
			}
			else if (audio_in_stream.sample_rate == 44100)
			{
				app_change_mode_req(TWS_SUBW_W_BT_T6_SF44K,245,6);
			}
			else
			{
				DBG_Printf("optek link not support this sample rate in bt source\r\n");
			}
		}
		else
		{
			if (audio_in_stream.sample_rate == 48000)
			{
				//app_change_mode_req(TWS_SUBW_SF48K_PT5MS,48,6);
				app_change_mode_req(SUBW_SF48K_PT2P5MS,24,6);
			}
			else if (audio_in_stream.sample_rate == 44100)
			{
				//app_change_mode_req(TWS_SUBW_SF44K_PT5MS,48,6);
				app_change_mode_req(SUBW_SF44K_PT2P5MS,24,6);
			}
			else if (audio_in_stream.sample_rate == 32000)
			{
				//app_change_mode_req(TWS_SUBW_SF32K_PT5MS,48,6);
				app_change_mode_req(SUBW_SF32K_PT2P5MS,24,6);
			}
			else
			{
				DBG_Printf("optek link not support this sample rate in full band\r\n");
			}
		}

		spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING; 
#if 0
		if ((decode_type_get() == DECODE_SBC)/* || app_main_data.media == MEDIA_USB_DEVICE*/)
		{
			spk_out_sync_w_tx_status = SPK_AND_TX_SYNCED;
		}
#endif
	}
}

//void optek_link_put_tx_data(short *ptr,U16 len) __INTERNAL_RAM_TEXT;
void optek_link_put_tx_data(short *ptr,U16 len)
{
	if ((opl_status.opl_main_status != enOPL_CLOSED) && (global_vari.optek_link_role == OPL_ROLE_MASTER))
	{
		int i;	
		U32 pOutDataLen;
		U16 samples = len >> 1;

		short *pIn, *pend;
		static short *pOut;

		optek_hifi2_stereo_16b_to_mono (enc_inp_buffer,ptr,samples);
		samples >>= 1;
		optek_bqdiir_mono_16b_process(downsampleFilterHandle,enc_inp_buffer,enc_inp_buffer,samples);

		//down sample 1/16, 16bit mono samples
		//samples >>= 4;

		pIn = enc_inp_buffer + start_offset;
		

		pOut = enc_inp_buffer;	

		pend = enc_inp_buffer + samples;

		pOutDataLen = 0;
		while (pIn < pend)
		{
			*pOut = *pIn;

			pOut++;
			pIn+=DOWNSAMPLE;
			pOutDataLen += 2;
			//pOut[i] = pIn[i<<4];
		}

		start_offset = (pIn - pend);

		//bc_tx_data.tx_len = (U8)((U32)pOut - (U32)bc_tx_data.tx_data);
		U16 tx_len;
		if ((optek_link_mode == TWS_SUBW_W_BT_T6_SF48K) || (optek_link_mode == TWS_SUBW_W_BT_T6_SF44K))
		{
			tx_len = 240;
		}
		else
		{
			tx_len = 24;
		}

		if (fifo_put_data(&opl_swf_tx_fifo,(U8 *)enc_inp_buffer,pOutDataLen) != pOutDataLen)
		{
			DBG_Printf("tx overflow\r\n");
		}
	/*
		if (fifo_get_fifo_data_len(&opl_swf_tx_fifo) >= tx_len)
		{
			if (bc_tx_data.tx_len == 0)
			{
				fifo_get_data(&opl_swf_tx_fifo,bc_tx_data.tx_data,tx_len);
				bc_tx_data.tx_len = tx_len;
			}
			else
			{
				DBG_Printf("optek link tx too slow:%d\r\n",bc_tx_data.tx_len);
			}
		}
	*/	
	}	
}

i32 dritf_samples;
extern I32 adj_outsamples;
U8 swf_mac_id[4] = {0};

void rx_slave_mac_id(void)
{
	DBG_Printf("receive mac id:%x,%x,%x\r\n",swf_mac_id[1],swf_mac_id[2],swf_mac_id[3]);
}

void optek_link_rx_cb(u8 *buf, u16 len, u8 bc_index, u16 pt_end_time, u8 role, u8 frame_index, u8 id) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_rx_cb(u8 *buf, u16 len, u8 bc_index, u16 pt_end_time, u8 role, u8 frame_index, u8 id)
{	
	rx1_data.loss_flag = 0;
	if (role == OPTEK_LINK_MASTER)
	{
		//no data need receive
		if (opl_status.opl_main_status != enOPL_CONNECTED)
		{
			opl_status.opl_main_status = enOPL_CONNECTED;
		}

		if (len == 6)
		{
#if 0
			if (buf[1]=='M')
			{
				//DBG_iPrintf("%d,%d\n",buf[5],buf[1]+buf[2]+buf[3]+buf[4]);
				if (buf[5] == (U8)(buf[1]+buf[2]+buf[3]+buf[4]))
				{
					if (swf_mac_id[0] != 0xaa || (buf[2] != swf_mac_id[1] || buf[3] != swf_mac_id[2] || buf[4] != swf_mac_id[3]))
					{
						swf_mac_id[0] = 0xaa;
						swf_mac_id[1] = buf[2];
						swf_mac_id[2] = buf[3];
						swf_mac_id[3] = buf[4];

						ukiMsgSend(rx_slave_mac_id);
					}
				}
			}
#endif			
		}
	}
	else
	{
		//receive master data
		rx1_data.rxing_len = len;
		memcpy(rx1_data.rx_buf[rx1_data.rx_buf_index],buf,len);

		#ifdef OPTEK_LINK_RX_QUALITY_DEBUG
		if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCED)
			optek_link_rx_counters[bc_index]++;
		#endif
	}
}

void  optek_link_slave_rx_end_cb(u8 *buf, u16 len, uint8_t id)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link_slave_rx_end_cb(u8*buf, u16 len, u8 id)
{
	if (global_vari.optek_link_role == OPL_ROLE_MASTER)
		return;

	if (rx1_data.rxing_len)
	{
		rx1_data.rx_len = rx1_data.rxing_len;
		rx1_data.rx_buf_index = 1 - rx1_data.rx_buf_index;
		rx1_data.rx_data_index = 0;
		rx1_data.rxing_len = 0;
	}

#if 1//simple test
	if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING && decode_type_is_set())
	{
		if (optek_link_mode == SUBW_SF48K_PT2P5MS || optek_link_mode == TWS_SUBW_W_BT_T6_SF48K)
		{//48K
			audio_in_stream.sample_rate = SAMPLE_RATE_48K;
		}
		else if (optek_link_mode == SUBW_SF44K_PT2P5MS || optek_link_mode == TWS_SUBW_W_BT_T6_SF44K)
		{//44.1K
			audio_in_stream.sample_rate = SAMPLE_RATE_44P1K;
		}
		else
		{//32K
			audio_in_stream.sample_rate = SAMPLE_RATE_32K;
		}
		//audio_in_stream.playing_stream_chans = 2;

		decode_start_w_init_msgSend_from_isr();

		spk_out_sync_w_tx_status = 	SPK_AND_TX_SYNCED;			
	}
#else

	/**************for sync********************/
	#define TX_AND_DECODE_DIST 10//SAMPLES

	if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING && decode_type != DECODE_NONE)
	{
		if (optek_link_mode == SUBW_SF48K_PT2P5MS || optek_link_mode == TWS_SUBW_W_BT_T6_SF48K)
		{//48K
			audio_in_stream.sample_rate = SAMPLE_RATE_48K;
		}
		else if (optek_link_mode == SUBW_SF44K_PT2P5MS || optek_link_mode == TWS_SUBW_W_BT_T6_SF44K)
		{//44.1K
			audio_in_stream.sample_rate = SAMPLE_RATE_44P1K;
		}
		else
		{//32K
			audio_in_stream.sample_rate = SAMPLE_RATE_32K;
		}
		audio_in_stream.playing_stream_chans = 2;
		//app_audio_clock_freq_setting(audio_in_stream.sample_rate);

		uiDecSend (DECODE_INIT, 0);
		spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING_STEP2;
	}
	else if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING_STEP2)
	{
		U32 remaining_samples = local_tx_remaining_samples_get();
		
		if (adj_outsamples == 0)
		{
			i32 out_dma_cnt;

			out_dma_cnt = TX_AND_DECODE_DIST - remaining_samples;

			if (out_dma_cnt == 0)
			{
				spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING_STEP3;
			}
			else
			{
				adj_outsamples = out_dma_cnt; 
			}
		}
	}
	else if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING_STEP3)
	{	
		spk_out_sync_w_tx_status = SPK_AND_TX_SYNCED;

		uiDecSend (DECODE_FRAME, FALSE);

		#ifdef LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE
		if (app_main_data.optek_link_role == OPL_ROLE_SLAVE)
		{	
			ukiMsgSend(sync_w_src_start);
		}
		#endif
		dritf_samples = 0;		
	}
	else if(spk_out_sync_w_tx_status == SPK_AND_TX_SYNCED)
	{
		U32 remaining_samples = local_tx_remaining_samples_get();
		
		#if 1
		if (remaining_samples > 60)
		{//That means it's already less than 0
			dritf_samples = remaining_samples - TX_AND_DECODE_DIST - 120;
		}
		else
		{
			dritf_samples = remaining_samples - TX_AND_DECODE_DIST;		
		}
		#else
		if (remaining_samples > 20)
		{//That means it's already less than 0
			remaining_samples = 0;
		}

		dritf_samples = remaining_samples - TX_AND_DECODE_DIST;
		#endif				
	}
#endif
}

void optek_link_rx_err_cb(u8 index, u8 role, u8 id) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_rx_err_cb(u8 index, u8 role, u8 id)
{
	if (role == OPTEK_LINK_MASTER)
	{
	}
	else
	{
				//master packet loss
		rx1_data.loss_flag = 1;
		
		#ifdef OPTEK_LINK_RX_QUALITY_DEBUG
		optek_link_rx_counters[enumTRANSMISSION_FAILED]++;
		#endif
	}
}

U8 dec_get_optek_link_rx_data(U8 **pOut,U8 rx_sel,U8 len)
{
	u8 ret;

	if (rx_sel == 1)
	{
		if (rx1_data.rx_len >= len)
		{
			//memcpy(pOut,rx1_data.rx_buf,len);
			*pOut = &rx1_data.rx_buf[1 - rx1_data.rx_buf_index][rx1_data.rx_data_index];

			rx1_data.rx_data_index += len;
			rx1_data.rx_len -= len;
			ret = len;//rx1_data.rx_len;
			//if (ret != 100)
			//DBG_Printf("ret:%d\r\n",ret);
		}
		else
		{
			ret = 0;
		}
	}
	return ret;
}


u8 optek_link_tx_data_len(u8 role, u8 id)  __attribute__ ((section (".internal_ram_1_text")));
u8 optek_link_tx_data_len(u8 role, u8 id)
{//get next tx len
	//if (role == OPTEK_LINK_MASTER)	
    	return bc_tx_data.last_tx_len;
	//else
	//	return 6;
}

U16 conn_cnt;
u8 optek_link_tx_data_cb(u8 cpy, u8 *buf, u8 role, u8 ext_frame, u8 id) __attribute__ ((section (".internal_ram_1_text")));
#if 1
u8 optek_link_tx_data_cb(u8 cpy, u8 *buf, u8 role, u8 ext_frame, u8 id)
{
    u8 len = 0;
	U8 packet_len;

    if (cpy == 1)
    {
		if (optek_link_mode == TWS_SUBW_W_BT_T6_SF44K)
		{
			packet_len = 240;
		}
		else if (optek_link_mode == TWS_SUBW_W_BT_T6_SF48K)
		{
			packet_len = 240;
		}
		else
		{
			packet_len = 24;
		}

		if (role == OPTEK_LINK_MASTER)
		{
			if (spk_out_sync_w_tx_status != SPK_AND_TX_SYNCED)
			{
				if (fifo_get_data_len(&opl_swf_tx_fifo) >= packet_len*2)
				{
					spk_out_sync_w_tx_status = SPK_AND_TX_SYNCED;
				}
			}

			if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCED)
			{
				if (fifo_get_data_len(&opl_swf_tx_fifo) >= bc_tx_data.last_tx_len)
				{
					//bc_tx_data.tx_len = 0;
			
					//CFasm_memcpy((void *)buf, (void *)bc_tx_data.tx_data,len);

					fifo_get_data(&opl_swf_tx_fifo, buf, bc_tx_data.last_tx_len);

					len = bc_tx_data.last_tx_len;//packet_len;	
					bc_tx_data.last_tx_len = packet_len;
				}
				else if (bc_tx_data.last_tx_len)
				{
					spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING;
					memset((void *)buf,0,bc_tx_data.last_tx_len);
					len = bc_tx_data.last_tx_len;
					bc_tx_data.last_tx_len = 0;
				}
			}
			else if (bc_tx_data.last_tx_len)
			{
				memset((void *)buf,0,bc_tx_data.last_tx_len);
				len = bc_tx_data.last_tx_len;
				bc_tx_data.last_tx_len = 0;
			}			
		}
		else
		{
			if (1)
			{
				buf[1] = 'M';
				memset((void *)&buf[2],0x55,3);
				buf[5] = (U8)(buf[1]+buf[2]+buf[3]+buf[4]);
				len = 6;
				bc_tx_data.last_tx_len = 6;
			}
			else
			{
				if (bc_tx_data.last_tx_len != 0)
				{
					buf[1] = 'M';
					memset((void *)&buf[2],0x55,3);
					buf[5] = (U8)(buf[1]+buf[2]+buf[3]+buf[4]);
					bc_tx_data.last_tx_len = 0;
					len = 0;
				}
				else
				{
					len = 0;
				}
			}
		}
    }

#if 0
	/**************for sync********************/
	if (role == OPTEK_LINK_MASTER)
	{
		#define TX_AND_DECODE_DIST 10//SAMPLES

		if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING)
		{
			if (decode_type == DECODE_PCM)
			{
				app_dac_mute_enable();
				app_dac_receive_pcm_enable(FALSE);
				adj_outsamples = -1;
			}
			spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING_STEP2;
		}
		else if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING_STEP2)
		{
			U32 remaining_samples = local_tx_remaining_samples_get();
			
			if (adj_outsamples == 0)
			{
				i32 out_dma_cnt;

				out_dma_cnt = TX_AND_DECODE_DIST - remaining_samples;

				if (out_dma_cnt == 0)
				{
					spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING_STEP3;
				}
				else
				{
					adj_outsamples = out_dma_cnt; 
				}
			}
		}
		else if ((spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING_STEP3)&&(ext_frame == 0))
		{	
			spk_out_sync_w_tx_status = SPK_AND_TX_SYNCED;
			if (decode_type == DECODE_PCM)
			{		
				app_dac_mute_disable();
				app_dac_receive_pcm_enable_w_samples(16*5+48);
			}

			dritf_samples = 0;		
		}
		else if(spk_out_sync_w_tx_status == SPK_AND_TX_SYNCED)
		{
			U32 remaining_samples = local_tx_remaining_samples_get();
			
			if (remaining_samples > 20)
			{//That means it's already less than 0
				remaining_samples = 0;
			}

			dritf_samples = remaining_samples - TX_AND_DECODE_DIST;				
		}
	}
#endif

    return len;
}
#else
u8 optek_link_tx_data_cb(u8 cpy, u8 *buf, u8 role, u8 ext_frame, u8 id)
{
    u8 len = 0;
	U8 packet_len;

    if (cpy == 1)
    {
		if (optek_link_mode == TWS_SUBW_W_BT_T6_SF44K)
		{
			packet_len = 200;
		}
		else if (optek_link_mode == TWS_SUBW_W_BT_T6_SF48K)
		{
			packet_len = 200;
		}
		else
		{
			packet_len = 24;
		}

		if (bt_status.bt_a2dp_status == enBT_A2DP_STREAMING)
			packet_len = 240;
		else
			packet_len = 0;

		if (role == OPTEK_LINK_MASTER)
		{
			
			if (spk_out_sync_w_tx_status != SPK_AND_TX_SYNCED)
			{
				if (fifo_get_data_len(&opl_swf_tx_fifo) >= packet_len*10)
				{
					spk_out_sync_w_tx_status = SPK_AND_TX_SYNCED;
				}
			}

			spk_out_sync_w_tx_status = SPK_AND_TX_SYNCED;

			if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCED)
			{
				if (1)//(fifo_get_data_len(&opl_swf_tx_fifo) >= bc_tx_data.last_tx_len)
				{
					//bc_tx_data.tx_len = 0;
			
					//CFasm_memcpy((void *)buf, (void *)bc_tx_data.tx_data,len);

					fifo_get_data(&opl_swf_tx_fifo, buf, bc_tx_data.last_tx_len);

					len = bc_tx_data.last_tx_len;//packet_len;	
					bc_tx_data.last_tx_len = packet_len;
				}
				else if (bc_tx_data.last_tx_len)
				{
					memset((void *)buf,0,bc_tx_data.last_tx_len);
					len = bc_tx_data.last_tx_len;
					bc_tx_data.last_tx_len = 0;
				}
			}
		}
		else
		{
#if 0
			if (1)
			{
				buf[1] = 'M';
				memset((void *)&buf[2],0x55,3);
				buf[5] = (U8)(buf[1]+buf[2]+buf[3]+buf[4]);
				len = 6;
				bc_tx_data.last_tx_len = 6;
			}
			else
			{
				if (bc_tx_data.last_tx_len != 0)
				{
					buf[1] = 'M';
					memset((void *)&buf[2],0x55,3);
					buf[5] = (U8)(buf[1]+buf[2]+buf[3]+buf[4]);
					bc_tx_data.last_tx_len = 0;
					len = 0;
				}
				else
				{
					len = 0;
				}
			}
#endif			
		}
    }

#if 0
	/**************for sync********************/
	if (role == OPTEK_LINK_MASTER)
	{
		#define TX_AND_DECODE_DIST 10//SAMPLES

		if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING)
		{
			if (decode_type == DECODE_PCM)
			{
				app_dac_mute_enable();
				app_dac_receive_pcm_enable(FALSE);
				adj_outsamples = -1;
			}
			spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING_STEP2;
		}
		else if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING_STEP2)
		{
			U32 remaining_samples = local_tx_remaining_samples_get();
			
			if (adj_outsamples == 0)
			{
				i32 out_dma_cnt;

				out_dma_cnt = TX_AND_DECODE_DIST - remaining_samples;

				if (out_dma_cnt == 0)
				{
					spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING_STEP3;
				}
				else
				{
					adj_outsamples = out_dma_cnt; 
				}
			}
		}
		else if ((spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING_STEP3)&&(ext_frame == 0))
		{	
			spk_out_sync_w_tx_status = SPK_AND_TX_SYNCED;
			if (decode_type == DECODE_PCM)
			{		
				app_dac_mute_disable();
				app_dac_receive_pcm_enable_w_samples(16*5+48);
			}

			dritf_samples = 0;		
		}
		else if(spk_out_sync_w_tx_status == SPK_AND_TX_SYNCED)
		{
			U32 remaining_samples = local_tx_remaining_samples_get();
			
			if (remaining_samples > 20)
			{//That means it's already less than 0
				remaining_samples = 0;
			}

			dritf_samples = remaining_samples - TX_AND_DECODE_DIST;				
		}
	}
#endif

    return len;
}
#endif

void optek_link_tx_len_err_cb(u8 max_len, u8 tx_en, u8 mode, u8 role, u8 id) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_tx_len_err_cb(u8 max_len, u8 tx_en, u8 mode, u8 role, u8 id)
{	

}

extern U8 conn_rej_count;
u8 optek_link_slave_conn_cb (u8 mode, u8 **slave_max_tx_len, u8 **slave_max_rx_len, u8 id) __attribute__ ((section (".internal_ram_1_text")));
u8 optek_link_slave_conn_cb (u8 mode, u8 **slave_max_tx_len, u8 **slave_max_rx_len, u8 id)
{
    //please checking support mode, if not accept the mode, return 0, others return 1

	U8 ret = TRUE;

	*slave_max_tx_len = NULL;
	*slave_max_rx_len = NULL;

	//DBG_iPrintf("opl mode:%d\r\n",mode);
	if (conn_rej_count)
	{
		conn_rej_count--;
		return FALSE;
	}

	if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
		optek_link_trans_init();

	#ifdef OPTEK_LINK_RX_QUALITY_DEBUG
	memset(optek_link_rx_counters, 0, sizeof(optek_link_rx_counters));
	#endif	

	if (mode == SUBW_SF48K_PT2P5MS || mode == SUBW_SF44K_PT2P5MS || mode == SUBW_SF32K_PT2P5MS)
	{
		*slave_max_tx_len = fullband_slave_tx_len_table;
		*slave_max_rx_len = fullband_slave_rx_len_table;
		opl_status.opl_main_status = enOPL_CONNECTED;
	}
	else if (mode == TWS_SUBW_W_BT_T6_SF48K || mode == TWS_SUBW_W_BT_T6_SF44K)
	{
		*slave_max_tx_len = slave_tx_len_table;
		*slave_max_rx_len = slave_rx_len_table;
		opl_status.opl_main_status = enOPL_CONNECTED;
	}		
	else
	{
		ret = FALSE;
	}
	
	optek_link_mode = mode;

	//app_main_data.playing_stream_status = STREAM_WAITING_DATA;

	if (ret == TRUE)
	{
		spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING;		
	}

	return ret;
}

void optek_link_slave_link_loss_cb (u8 id)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link_slave_link_loss_cb (u8 id)
{
	opl_status.opl_main_status = enOPL_DISCONNECTED;
	conn_rej_count = 1;
	//ukiMsgSend(app_cmd_DecoderExit);
	decode_exit_req_set_from_isr();
}

void optek_link_slave_link_mode_err_cb (u8 id)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link_slave_link_mode_err_cb (u8 id)
{
	optek_link_slave_link_loss_cb(id);
}

void optek_link_master_link_loss_cb (u8 frame_index, u8 id)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link_master_link_loss_cb (u8 frame_index, u8 id)
{
	opl_status.opl_main_status = enOPL_DISCONNECTED;
}

void link_info_change_cb(u8 mode, u8 succ) __attribute__ ((section (".internal_ram_1_text")));
void link_info_change_cb(u8 mode, u8 succ)
{
    DBG_Assert (succ == 1);    
}
void optek_link_mode_change_cb(u8 mode, u8 succ) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_mode_change_cb(u8 mode, u8 succ)
{
    DBG_Assert (succ == 1);    
}
void optek_link_master_busy_ind (void)
{
    //master is connected with other device
    //DBG_Assert (0);
}

static void optek_link_mode_iram_text_init (void)
{

}

static void optek_link_mode_rx_len_init (void)
{
	memset(slave_tx_len_table,SLAVE_TX_LEN,sizeof(slave_tx_len_table));
	memset(slave_rx_len_table,MASTER_TX_LEN,sizeof(slave_rx_len_table));

	memset(fullband_slave_tx_len_table,SLAVE_TX_LEN,sizeof(fullband_slave_tx_len_table));
	memset(fullband_slave_rx_len_table,FULLBAND_MASTER_TX_LEN,sizeof(fullband_slave_rx_len_table));	
}

void optek_link_mode_init (void)
{
	optek_link_mode_iram_text_init();
	optek_link_mode_rx_len_init();
}

#endif
