/**
 ****************************************************************************************
 *
 * @file optek_link_single_way.c
 *
 * @brief optek link single way mode implementation.
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

#include "xa_type_def.h"
#include "xa_error_standards.h"

#ifdef OPL_MODE_SINGLE_WAY
XA_ERRORCODE xa_lc3_enc_main_init(U32 bitrate,U16 samplerate,U8 channels,float frame_Time_ms);
int xa_lc3_enc_frame(U16 *pInBuf, U32 samples, U8 **pOutBuf, U32 *pOutDataLen);

//void this_function_check_elf_file(void) __attribute__((section (".warning_pls_use_elf32xtensa_normal_x")));
void this_function_check_elf_file(void)
{

}

#define MASTER_48K_TX_LEN 96
#define MASTER_44K_TX_LEN 108
#define MASTER_32K_TX_LEN 150

#define SLAVE_TX_LEN  6
#define TXRX_NUM 8 

U8 slave_48k_rx_len_table[TXRX_NUM];
U8 slave_44k_rx_len_table[TXRX_NUM];
U8 slave_32k_rx_len_table[TXRX_NUM];
U8 slave_tx_len_table[TXRX_NUM];

extern u8 master_bdaddr[6];

volatile APK_AND_TX_SYNC_STATUS_ENUM spk_out_sync_w_tx_status;

typedef struct {
	U8 tx_data[MASTER_32K_TX_LEN*2];
    U8 tx_len;
	U8 last_tx_len;	
	FIFO tx_fifo;
} BC_TX_DATA_t;

BC_TX_DATA_t bc_tx_data;

typedef struct {
	U8 rx_buf[2][MASTER_32K_TX_LEN];
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
	OPL_ROLE_SLAVE,
	OPL_ROLE_MASTER,
	nonOPL_ROLE
};

const SW_ROLE_t  sw_role_info = {sw_role_table,sizeof(sw_role_table)/sizeof(sw_role_table[0])};

void optek_link_trans_init(void) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_trans_init(void)
{
	bc_tx_data.tx_len = 0;
	bc_tx_data.last_tx_len = 0;
	rx1_data.rx_len = 0;
	rx1_data.rxing_len = 0;
	rx1_data.rx_buf_index = 0;
	rx1_data.loss_flag = 0;
}

void optek_link_codec_init(U16 sample_rate,U8 bt_src)
{
	optek_link_trans_init();
	#ifdef OPL_MASTER_ENABLE
	if (global_vari.optek_link_role == OPL_ROLE_MASTER)
	{
		fifo_init(&bc_tx_data.tx_fifo, bc_tx_data.tx_data, sizeof(bc_tx_data.tx_data), TRUE);

		if (sample_rate == 44100)
		{
			if (xa_lc3_enc_main_init(158760, sample_rate, 2, 5.0) != XA_NO_ERROR)
			{
				DBG_Printf("lc3 enc init error\n\r");
			}
		}
		else
		{
			if (xa_lc3_enc_main_init(96*8*200, sample_rate, 2, 5.0) != XA_NO_ERROR)
			{
				DBG_Printf("lc3 enc init error\n\r");
			}
		}

		if (sample_rate == 48000)
		{
			app_change_mode_req(BC_SF48K_PT5MS, 100, 6);
		}
		else if (sample_rate == 44100)
		{
			app_change_mode_req(BC_SF44K_PT5MS, 108, 6);
		}
		else if (sample_rate == 32000)
		{
			app_change_mode_req(BC_SF32K_PT5MS, 150, 6);
		}
		else
		{
			DBG_Printf("optek link not support this sample rate\r\n");
		}

		//spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING;
		spk_out_sync_w_tx_status = SPK_AND_TX_SYNCED;//study later
	}
	#endif
}

void optek_link_put_tx_data(short *ptr,U16 len) __INTERNAL_RAM_TEXT;
void optek_link_put_tx_data(short *ptr,U16 len)
{
	#ifdef OPL_MASTER_ENABLE
	if (opl_status.opl_main_status != enOPL_CLOSED)
	{	
		if (global_vari.optek_link_role == OPL_ROLE_MASTER)
		{
	#if 1
			U8 *pEncData;		
			U32 encDataLen;						
			xa_lc3_enc_frame(ptr,len>>2,&pEncData,&encDataLen);

			if (fifo_get_free_space(&bc_tx_data.tx_fifo) >= encDataLen)
			{
				fifo_put_data(&bc_tx_data.tx_fifo, pEncData, encDataLen);
			}
			else
			{
				DBG_Printf("opl tx overflow\r\n");
			}
	#else
			if(bc_tx_data.tx_len == 0)
			{
				U8 *pEncData;
				U32 encDataLen;
				xa_lc3_enc_frame(ptr,len>>2,&pEncData,&encDataLen);
				memcpy(&bc_tx_data.tx_data[0],pEncData,encDataLen);
				bc_tx_data.tx_len = encDataLen;

				//DBG_Assert(bc_tx_data.tx_len <= MASTER_TX_LEN);
			}
			else
			{
				DBG_Printf("tx loss packet\r\n");
			}
	#endif
		}
	}	
	#endif
}

i32 dritf_samples;
extern I32 adj_outsamples;

void optek_link_rx_cb(u8 *buf, u16 len, u8 bc_index, u16 pt_end_time, u8 role, u8 frame_index, u8 id) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_rx_cb(u8 *buf, u16 len, u8 bc_index, u16 pt_end_time, u8 role, u8 frame_index, u8 id)
{	
	rx1_data.loss_flag = 0;
	if (role == OPTEK_LINK_MASTER)
	{
		//no data need receive
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

#define TX_AND_DECODE_DIST 10//SAMPLES
//disable sync with output, study later
int local_tx_remaining_samples_get(void)
{
	return TX_AND_DECODE_DIST;
}

void  optek_link_slave_rx_end_cb(u8 *buf, u16 len, uint8_t id)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link_slave_rx_end_cb(u8*buf, u16 len, u8 id)
{
#if 1	
	if (rx1_data.rxing_len)
	{
		rx1_data.rx_len = rx1_data.rxing_len;
		rx1_data.rx_buf_index = 1 - rx1_data.rx_buf_index;
		rx1_data.rx_data_index = 0;
		rx1_data.rxing_len = 0;
	}

	if (global_vari.optek_link_role == OPL_ROLE_MASTER)
		return;


	//return;
	/**************for sync********************/

	if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING && decode_type_is_set())
	{
		if (optek_link_mode == BC_SF48K_PT5MS)
		{//48K
			audio_in_stream.sample_rate = SAMPLE_RATE_48K;
		}
		else if (optek_link_mode == BC_SF44K_PT5MS)
		{//44.1K
			audio_in_stream.sample_rate = SAMPLE_RATE_44P1K;
		}
		else
		{//32K
			audio_in_stream.sample_rate = SAMPLE_RATE_32K;
		}
		audio_in_stream.channel = 2;
				
		decode_init_msgSend_from_isr();
		spk_out_sync_w_tx_status = SPK_AND_TX_SYNCING_STEP2;
	}
	else if (spk_out_sync_w_tx_status == SPK_AND_TX_SYNCING_STEP2)
	{
		U32 remaining_samples = local_tx_remaining_samples_get();
		
		if (1)//(adj_outsamples == 0)
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

		decode_start_msgSend_form_isr();

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
		if (remaining_samples > 120)
		{//That means it's already less than 0
			dritf_samples = remaining_samples - TX_AND_DECODE_DIST - 240;
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
{//loss one packet event
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
{	
    return bc_tx_data.last_tx_len;
}

U16 conn_cnt;
u8 optek_link_tx_data_cb(u8 cpy, u8 *buf, u8 role, u8 ext_frame, u8 id) __attribute__ ((section (".internal_ram_1_text")));
u8 optek_link_tx_data_cb(u8 cpy, u8 *buf, u8 role, u8 ext_frame, u8 id)
{
    u8 len = 0;
	U8 packet_len;

    if (cpy == 1)
    {	
#if 1
		len = MASTER_48K_TX_LEN;

		if (fifo_get_data_len(&bc_tx_data.tx_fifo) >= len)
		{
			//CFasm_memcpy((void *)buf, (void *)bc_tx_data.tx_data,len);
			fifo_get_data(&bc_tx_data.tx_fifo, buf, len);
			bc_tx_data.last_tx_len = len;			
		}
		else if (bc_tx_data.last_tx_len)
		{		
			memset((void *)buf,0,bc_tx_data.last_tx_len);
			bc_tx_data.last_tx_len = 0;	
		}

#else

		len = bc_tx_data.tx_len;
		
		if (len)
		{
			bc_tx_data.tx_len = 0;
	
			CFasm_memcpy((void *)buf, (void *)bc_tx_data.tx_data,len);
			bc_tx_data.last_tx_len = len;	
		}
		else if (bc_tx_data.last_tx_len)
		{		
			memset((void *)buf,0,bc_tx_data.last_tx_len);
			bc_tx_data.last_tx_len = 0;	
		}
#endif		
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

void optek_link_tx_len_err_cb(u8 max_len, u8 tx_en, u8 mode, u8 role, u8 id) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_tx_len_err_cb(u8 max_len, u8 tx_en, u8 mode, u8 role, u8 id)
{	
    DBG_Assert (0);    
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
/*	if (conn_rej_count)
	{
		conn_rej_count--;
		return FALSE;
	}
*/
	optek_link_trans_init();

	conn_cnt = 0;
	if (mode == BC_SF48K_PT5MS)
	{
		*slave_max_tx_len = slave_tx_len_table;
		*slave_max_rx_len = slave_48k_rx_len_table;
		opl_status.opl_main_status = enOPL_CONNECTED;
	}
	else if (mode == BC_SF44K_PT5MS)
	{
		*slave_max_tx_len = slave_tx_len_table;
		*slave_max_rx_len = slave_44k_rx_len_table;
		opl_status.opl_main_status = enOPL_CONNECTED;
	}
	else if (mode == BC_SF32K_PT5MS)
	{
		*slave_max_tx_len = slave_tx_len_table;
		*slave_max_rx_len = slave_32k_rx_len_table;
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
	//app_dac_receive_pcm_enable(FALSE);
	//app_main_data.playing_stream_status = STREAM_WAITING_DATA;
	//AUDIO_FIFO_STREAM_DISCARD_ALL_DATA();
	//#else
	conn_cnt = 0;
	//ukiMsgSend(app_cmd_DecoderExit);
	opl_status.opl_main_status = enOPL_DISCONNECTED;

	//DBG_iPrintf("slave loss\r");
	//app_main_data.playing_stream_status = STREAM_WAITING_DATA;
	conn_rej_count = 1;
	//ukiMsgSend(app_cmd_DecoderExit);
	//SOFT2_INT_SET;
	//SOFT3_INT_SET;
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
//	DBG_iPrintf("master loss\r");
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
#if 0	
	#ifdef LC3_ENCODE_ENABLE
	app_lc3_encoding_code_text_copy_init();
	#endif

	#ifdef LC3_DECODE_ENABLE
	app_lc3_decoding_code_text_copy_init();
	#endif

	if (0)
	{

	}
	#ifdef LC3_DECODE_ENABLE
	else if (app_main_data.optek_link_role == OPL_ROLE_SLAVE)
	{
		app_lc3_decoding_code_text_copy();
	}
	#endif
	#ifdef LC3_ENCODE_ENABLE
	else// if (app_main_data.optek_link_role == OPL_ROLE_MASTER)
	{
		//app_lc3_part_encoding_code_text_copy();
	}
	#endif	
#endif
}

static void optek_link_mode_rx_len_init (void)
{
	memset(slave_48k_rx_len_table,MASTER_48K_TX_LEN,sizeof(slave_48k_rx_len_table));
	memset(slave_44k_rx_len_table,MASTER_44K_TX_LEN,sizeof(slave_44k_rx_len_table));
	memset(slave_32k_rx_len_table,MASTER_32K_TX_LEN,sizeof(slave_32k_rx_len_table));
	memset(slave_tx_len_table,SLAVE_TX_LEN,sizeof(slave_tx_len_table));
}

void optek_link_mode_init (void)
{
	optek_link_mode_iram_text_init();
	optek_link_mode_rx_len_init();
}

#endif
