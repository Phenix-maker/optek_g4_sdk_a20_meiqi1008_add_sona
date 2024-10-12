/**
 ****************************************************************************************
 *
 * @file optek_link_null.c
 *
 * @brief optek link empty interface.
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
#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "codec.h"

#ifdef OPL_MODE_GAME_HP_w_MIC

XA_ERRORCODE xa_lc3_enc_main_init(U32 bitrate,U16 samplerate,U8 channels,float frame_Time_ms);
int xa_lc3_enc_frame(U16 *pInBuf, U32 samples, U8 **pOutBuf, U32 *pOutDataLen);
XA_ERRORCODE xa_lc3_dec_handle_init(void **decode_handle,U32 bitrate,U16 samplerate,U8 channels,float frame_Time_ms);
XA_ERRORCODE xa_lc3_dec_frame (void *decode_handle,U8 *pIn,U8 inLen,U8 **pout_buf, U16 *out_byte_len);
U8 dec_get_optek_link_rx_data(U8 **pOut, U8 rx_sel, U8 len);

volatile APK_AND_TX_SYNC_STATUS_ENUM spk_out_sync_w_tx_status;

typedef struct {
    u8 rx_data[8][120];
    U32 rx1_frame_id;
    U32 rx2_frame_id;
    U8 rx1_len;
    U8 rx2_len;
    U8 req_tx_len;
    U8 tx_len;
    U8 tx_frame_id;
	U8 tx_data[120];
} OPL_DATA_t;

typedef struct {
    OPL_DATA_t opl_data;
    enSyncStatus synced_w_output;
    uint8_t synced_w_input;
    uint8_t working;
    uint8_t req_rx_fifo_reset;
    FIFO rx_fifo;
    void * decodeHandle;
    uint32_t eventGroup;
} opl_envs_t;

opl_envs_t opl_envs;

//first element in this table use for default role
static const U8 sw_role_table[] = 
{
#ifdef OPL_MASTER_ENABLE
	OPL_ROLE_MASTER,
#endif

#ifdef OPL_SLAVE_ENBALE
    OPL_ROLE_SLAVE,
#endif

#ifdef OPL_MASTER_ENABLE    
    nonOPL_ROLE,
#endif
};

const SW_ROLE_t  sw_role_info = {sw_role_table,sizeof(sw_role_table)/sizeof(sw_role_table[0])};

void *rx1_decode_hanle;

void optek_link_trans_init(void) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_trans_init(void)
{
    opl_envs.opl_data.tx_len = 0;
    opl_envs.opl_data.req_tx_len = 0;
    opl_envs.opl_data.tx_frame_id = 0;

    if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
    {
        opl_envs.req_rx_fifo_reset = TRUE;
    }
    //opl_envs.working = FALSE;
    //opl_envs.synced_w_input = REQUEST_SYNC;
}

void optek_link_codec_init(U16 sample_rate,U8 bt_src)
{
	optek_link_trans_init();

	#ifdef OPL_SLAVE_ENBALE
	if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
	{
		if (sample_rate == 48000)
		{
			if (xa_lc3_enc_main_init(100*8*200, sample_rate, 2, 5.0) != XA_NO_ERROR)
			{
				DBG_Printf("lc3 enc init error\n\r");
			}

            if (xa_lc3_dec_handle_init(&opl_envs.decodeHandle, 50*8*200, audio_in_stream.sample_rate, 1, 5.0) != XA_NO_ERROR)
            {
                DBG_Printf("rx1 lc3 dec init error\n\r");
            }

		}
		else
		{
            DBG_Assert(FALSE);
		}
	}
	#endif

	#ifdef OPL_MASTER_ENABLE
	if (global_vari.optek_link_role == OPL_ROLE_MASTER)
    {
		if (sample_rate == 48000)
		{
			if (xa_lc3_enc_main_init(50*8*200, sample_rate, 1, 5.0) != XA_NO_ERROR)
			{
				DBG_Printf("lc3 enc init error\n\r");
			}
		}
		else
		{
            DBG_Assert(FALSE);
		}

        if (xa_lc3_dec_handle_init(&opl_envs.decodeHandle, 100*8*200, audio_in_stream.sample_rate, 2, 5.0) != XA_NO_ERROR)
        {
            DBG_Printf("rx1 lc3 dec init error\n\r");
        }
    }
    #endif

}

void optek_link_put_tx_data(short *ptr,U16 len) __INTERNAL_RAM_TEXT;
void optek_link_put_tx_data(short *ptr,U16 len)
{
    btdm_debug_pin(31, 1);
	#ifdef OPL_SLAVE_ENBALE
	if (opl_status.opl_main_status != enOPL_CLOSED)
	{	
		if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
		{
            U8 *pEncData;
            U32 encDataLen;
            xa_lc3_enc_frame(ptr, len>>2, &pEncData, &encDataLen);
            memcpy(&opl_envs.opl_data.tx_data[opl_envs.opl_data.tx_len], pEncData, encDataLen);

            opl_envs.opl_data.req_tx_len = encDataLen;

            U8 inLen;
            U8 *p_rx1;
            U8 *pDecOut1;
            U16 outlen;

            inLen = get_lc3_decode_frame_bytes(opl_envs.decodeHandle);
            inLen = dec_get_optek_link_rx_data(&p_rx1, 1, inLen);

            xa_lc3_dec_frame(opl_envs.decodeHandle, p_rx1, inLen, &pDecOut1, &outlen);

            DBG_Assert(len == outlen*2);
            int i;

            short *pin = pDecOut1;
            short *pout = ptr;
            for (i=0; i<outlen/2; i++)
            {
                *pout++ = *pin;
                *pout++ = *pin++;
            }
            extern FIFO UsbDevUpFifo;
            int free_space_len = fifo_get_free_space(&UsbDevUpFifo);

            if (free_space_len < len)
            {
                //DBG_Printf("usb mic overflow\n\r");
            }

            int putlen = fifo_put_data(&UsbDevUpFifo, ptr, len);
            //memcpy(ptr, pDecOut1, len);
		}
	}	
	#endif
    btdm_debug_pin(31, 0);


	#ifdef OPL_MASTER_ENABLE
	if (opl_status.opl_main_status != enOPL_CLOSED)
	{	
		if (global_vari.optek_link_role == OPL_ROLE_MASTER)
		{
            U8 *pEncData;
            U32 encDataLen;
            xa_lc3_enc_frame(ptr, len>>2, &pEncData, &encDataLen);
            memcpy(&opl_envs.opl_data.tx_data[opl_envs.opl_data.tx_len], pEncData, encDataLen);

            if (opl_envs.opl_data.tx_len)
            {
                opl_envs.opl_data.req_tx_len = opl_envs.opl_data.tx_len + encDataLen;
                opl_envs.opl_data.tx_len = 0;
            }
            else
            {
                opl_envs.opl_data.tx_len = encDataLen;
            }


            U8 inLen;
            U8 *p_rx1;
            U8 *pDecOut1;
            U16 outlen;

            inLen = get_lc3_decode_frame_bytes(opl_envs.decodeHandle);
            inLen = dec_get_optek_link_rx_data(&p_rx1, 1, inLen);

            xa_lc3_dec_frame(opl_envs.decodeHandle, p_rx1, inLen, &pDecOut1, &outlen);

            DBG_Assert(len == outlen);

            memcpy(ptr, pDecOut1, len);
		}
	}	
	#endif
    btdm_debug_pin(31, 1);
    btdm_debug_pin(31, 0);
}

extern void optek_link_set_timer (uint16_t val);

void optek_link20_timer1_in_frame(u32 link_role, u32 link_mode) __attribute__ ((section (".internal_ram_1_text"))); 
void optek_link20_timer1_in_frame (u32 link_role, u32 link_mode)
{

}

void optek_link20_start_in_frame (u32 link_role, u32 link_mode, u8 *tx_buf, u16 tx_buf_len)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link20_start_in_frame (u32 link_role, u32 link_mode, u8 *tx_buf, u16 tx_buf_len)
{
	btdm_debug_pin(29, 1);
	int i;

#ifdef OPL_SLAVE_ENBALE
	if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
    {
        if (REQUEST_SYNC == opl_envs.synced_w_output)
        {
            opl_envs.synced_w_output = SYNC_COMPLETED;

            hw_audio_qadc_fifo_enable();
            //audio_aux_input_rx_restart_req(48*4+12);
            decode_start_w_init_msgSend_from_isr();
/*
            if (FALSE == opl_envs.working) 
            {
                opl_envs.working = TRUE;
            }
*/
        }

        memcpy(tx_buf, opl_envs.opl_data.tx_data, opl_envs.opl_data.req_tx_len);
        opl_envs.opl_data.req_tx_len = 0;
    }
#endif

#ifdef OPL_MASTER_ENABLE
	if (global_vari.optek_link_role == OPL_ROLE_MASTER)
    {
        if (SYNC_COMPLETED != opl_envs.synced_w_input)
        {
            if (REQUEST_SYNC == opl_envs.synced_w_input)
            {
                opl_envs.synced_w_input = SYNC_IN_PROGRESS_STEP1;
                hw_audio_qadc_fifo_disable();
                //audio_aux_input_rx_stop_req();
                //btdm_debug_pin(27, 1);
                //btdm_debug_pin(27, 0);
            }
            else
            {
                //btdm_debug_pin(27, 1);
                //btdm_debug_pin(27, 0);
                opl_envs.synced_w_input = SYNC_COMPLETED;
                hw_audio_qadc_fifo_enable();
                //audio_aux_input_rx_restart_req(48*3);
                decode_start_w_init_msgSend_from_isr();

                if (FALSE == opl_envs.working) 
                {
                    opl_envs.working = TRUE;
                    xEventGroupClearBits(opl_envs.eventGroup, 1);
                }
                
            }
        }
        
        if (opl_envs.opl_data.req_tx_len)
        {
            //btdm_debug_pin(29, 1);
            memcpy(tx_buf + 1, opl_envs.opl_data.tx_data, opl_envs.opl_data.req_tx_len);
            tx_buf[0] = opl_envs.opl_data.tx_frame_id++;
            opl_envs.opl_data.req_tx_len = 0;
            //btdm_debug_pin(29, 0);
        }
    }
#endif
	btdm_debug_pin(29, 0);
}

uint32_t data_err_counter = 0;

u32 optek_link20_rx_in_frame (u32 link_role, u32 link_mode, u32 frame_id, U8 slot_idx, u8 *rx_buf, u16 rx_buf_len, u8 *rx_status, u8 crc_err)  __attribute__ ((section (".internal_ram_1_text")));
u32 optek_link20_rx_in_frame (u32 link_role, u32 link_mode, u32 frame_id, U8 slot_idx, u8 *rx_buf, u16 rx_buf_len, u8 *rx_status, u8 crc_err)
{
	//DBG_iPrintf("%d,%d,%d,%d,%x,%x\n", frame_id, slot_idx, rx_buf_len, crc_err, rx_buf[rx_buf_len-1], rx_buf[rx_buf_len/2] );
    if (0 == crc_err)
    {
		if (global_vari.optek_link_role == OPL_ROLE_MASTER)
		{
			if (opl_envs.opl_data.rx1_frame_id != frame_id)
			{
				if (rx_buf_len >= 5)
				{
					opl_envs.opl_data.rx1_frame_id = frame_id;
					memcpy(opl_envs.opl_data.rx_data[0], rx_buf, rx_buf_len);
					opl_envs.opl_data.rx1_len = rx_buf_len;
				}
			}
		}
		else
		{
			if (opl_envs.opl_data.rx1_frame_id != rx_buf[5])
			{
                btdm_debug_pin(28, 1);
				if (rx_buf_len >= 6)
				{
					opl_envs.opl_data.rx1_frame_id = rx_buf[0];

                    if (opl_envs.req_rx_fifo_reset == TRUE)
                    {
                        fifo_init(&opl_envs.rx_fifo, opl_envs.opl_data.rx_data, 8*96, TRUE);
                        opl_envs.req_rx_fifo_reset = FALSE;
                    }

                    fifo_put_data(&opl_envs.rx_fifo, rx_buf+1, rx_buf_len-1);
                    if (FALSE == opl_envs.working) 
                    {
                        if (fifo_get_data_len(&opl_envs.rx_fifo) > 96*2)
                            opl_envs.working = TRUE;
                    }
					//memcpy(opl_envs.opl_data.rx_data[0], rx_buf+6, rx_buf_len-6);
					//opl_envs.opl_data.rx1_len = rx_buf_len-6;
				}
                btdm_debug_pin(28, 0);
			}
		}
    }

	return 1;			//internal crc ok
}

u32 optek_link20_rx_end (u32 link_role, u32 link_mode, u32 frame_id)  __attribute__ ((section (".internal_ram_1_text")));
u32 optek_link20_rx_end (u32 link_role, u32 link_mode, u32 frame_id)
{
    btdm_debug_pin(28, 1);
    if (opl_envs.opl_data.rx1_frame_id != frame_id)
    {
        opl_envs.opl_data.rx1_len = 0;
    }

    BaseType_t xHigherPriorityTaskWoken1, xResult;

	// xHigherPriorityTaskWoken must be initialised to pdFALSE.
	xHigherPriorityTaskWoken1 = pdFALSE;

    xResult = xEventGroupSetBitsFromISR(opl_envs.eventGroup, 1, &xHigherPriorityTaskWoken1);
    if( xResult == pdPASS)
    {
        if (xHigherPriorityTaskWoken1)//If the priority of the timer daemon task is higher than the priority of the currently running task (the task the interrupt interrupted)
        {
            portYIELD_FROM_ISR();
        }
    }
    else
    {
        DBG_Assert(FALSE);//pdFALSE will be returned if the timer service queue was full
    }
    btdm_debug_pin(28, 0);
}

/**
 ****************************************************************************************
 * @brief get optek link receive data
 *
 * @param[out]  pOut    	pointer of the receive data
 * @param[in]  rx_sel     	which rx to get
 * @param[in]  len    	    the desired len
 *
 * @return len of rx data(actual len)
 * 
 * @notes:must first use the rx1 data
 ****************************************************************************************
 */

U8 temp_buf[100];
U8 dec_get_optek_link_rx_data(U8 **pOut, U8 rx_sel, U8 len)
{
    U8 ret = 0;

    if (opl_envs.working)
    {
        if (rx_sel == 1)
        {

            if (global_vari.optek_link_role == OPL_ROLE_MASTER)
            {
                xEventGroupWaitBits(opl_envs.eventGroup, 1, pdTRUE, pdFALSE, pdMS_TO_TICKS(100));

                if (opl_envs.opl_data.rx1_len)
                {
                    *pOut = opl_envs.opl_data.rx_data[0];
                    DBG_Assert(opl_envs.opl_data.rx1_len == len);
                    ret = len;
                }
            }
            else
            {
                len = fifo_get_data(&opl_envs.rx_fifo, temp_buf, len);
                *pOut = temp_buf;
                ret = len;
            }
        }
    }

    return ret;
}

/*
	when dma end, should get dma_end_time(rwip_time_get)
*/
rwip_time_t rwip_time_get(void);

rwip_time_t dma_end_bt_time;

void optek_link20_get_master_start_time (rwip_time_t *dma_end_time, uint32_t *delay_time_hus)
{
    if (global_vari.optek_link_role == OPL_ROLE_MASTER)
    {
        *dma_end_time = dma_end_bt_time;
        *delay_time_hus = (5000-100)*2;
        opl_envs.synced_w_output = SYNC_COMPLETED;	
    }
}


u32 optek_link20_get_master_tx_rx_len (uint8_t mode, uint16_t *max_m_tx_len, uint16_t *max_s_tx_len)
{
    u32 succ = 1;
    switch (mode)
    {

        case OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S2:          
        case OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S3:         
        case OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S1:          
        case OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S2:           
        case OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S3:          
        case OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S1:              
        {
            *max_m_tx_len = 185;
            *max_s_tx_len = 0;
        }        
        break;

        case OPTEK_LINK_MODE_USB_PT5MS_S1:
        {
            *max_m_tx_len = 0;
            *max_s_tx_len = 120;     
        }
        break;  
        case OPTEK_LINK_MODE_USB_PT2P5MS_S1:
        case OPTEK_LINK_MODE_USB_PT2P5MS_S1_w_HFP:        
        {
            *max_m_tx_len = 0;
            *max_s_tx_len = 60;     
        }
        break;

        case OPTEK_LINK_MODE_USB_PT7P5MS_S1:
        case OPTEK_LINK_MODE_USB_PT7P5MS_w_HFP_S1:
        {
            *max_m_tx_len = 0;
            *max_s_tx_len = 150;     
        }
        break;
        case OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S2_2_CH:     
        case OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S1_2_CH:     
        case OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S2_2_CH:      
        case OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S1_2_CH:
        {            
            *max_m_tx_len = 100;
            *max_s_tx_len = 0;
        }     
        break;

        case OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S1_SUBW:      
        case OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S1_SUBW:
        {
            *max_m_tx_len = 100;
            *max_s_tx_len = 0;
        }     

        break;

        case OPTEK_LINK_MODE_USB_PT5MS_S1_w_Up_Voice:
        {
            //max 100 bytes, 80kbps/one channel
            *max_m_tx_len = 100+1;                            
            *max_s_tx_len = 100;
        }     
        break;   
        case OPTEK_LINK_MODE_USB_PT7P5MS_S2_w_Up_Voice:
        {
            //72kbps/one channel
            *max_m_tx_len = 135;                            
            *max_s_tx_len = 135;
        }     
        break;   
        case OPTEK_LINK_MODE_USB_PT7P5MS_S2_tmono_w_Up_Voice:
        {
            //96kbps/one channel
            *max_m_tx_len = 135;                                //TODO                            
            *max_s_tx_len = 135;
        }
        case OPTEK_LINK_MODE_MICx2_PT2P5MS:
        {
            *max_m_tx_len = 0;
            *max_s_tx_len = 30;     
        }
        break;   
        case OPTEK_LINK_MODE_USB_PT5MS_BC:
        case OPTEK_LINK_MODE_USB_PT5MS_BC_6T:
        {
            //max 100 bytes, 80kbps/one channel
            *max_m_tx_len = 120;                            
            *max_s_tx_len = 0;
        }     
        break;   
 
        case OPTEK_LINK_MODE_USB_PT2P5MS_BC:
       {
            //max 100 bytes, 80kbps/one channel
            *max_m_tx_len = 60;                            
            *max_s_tx_len = 0;
        }     
        break;   


        default:
        {
            *max_m_tx_len = 0;
            *max_s_tx_len = 0;
            succ = 0;
        }
        break;
    }
    return succ;
}


uint8_t optek_link20_slave_conn_cb(uint8_t mode, uint16_t *pmax_m_tx_len, uint16_t *pmax_s_tx_len,  uint8_t id) __attribute__ ((section (".internal_ram_1_text")));
uint8_t optek_link20_slave_conn_cb(uint8_t mode, uint16_t *pmax_m_tx_len, uint16_t *pmax_s_tx_len,  uint8_t id)
{
#ifdef OPL_SLAVE_ENBALE
	if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
    {
        opl_envs.synced_w_output = REQUEST_SYNC;
        opl_envs.req_rx_fifo_reset = TRUE;
        hw_audio_qadc_fifo_disable();
        //audio_aux_input_rx_stop_req();
    }

    uint16_t max_m_tx_len, max_s_tx_len;

    optek_link20_get_master_tx_rx_len (mode, &max_m_tx_len, &max_s_tx_len);

    //swap master and slave tx/rx len
    *pmax_m_tx_len = max_m_tx_len;
    *pmax_s_tx_len = max_s_tx_len;

	return true;
#else
    return false;
#endif
}

#if 0
uint8_t optek_link20_slave_conn_cb(uint8_t mode, uint8_t id) __attribute__ ((section (".internal_ram_1_text")));
uint8_t optek_link20_slave_conn_cb(uint8_t mode, uint8_t id)
{
#ifdef OPL_SLAVE_ENBALE
	if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
    {
        opl_envs.synced_w_output = REQUEST_SYNC;
        opl_envs.req_rx_fifo_reset = TRUE;
        hw_audio_qadc_fifo_disable();
        //audio_aux_input_rx_stop_req();
    }
#endif
	return true;
}
#endif

void optek_link_slave_link_loss_cb (u8 id)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link_slave_link_loss_cb (u8 id)
{
    //DBG_Assert(FALSE);
    decode_exit_req_set_from_isr();
    //audio_aux_input_rx_stop_req();
}

void optek_link_master_link_loss_cb (u8 slave_index, u8 id)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link_master_link_loss_cb (u8 slave_index, u8 id)
{
    //DBG_iPrintf("slave %d loss\n", slave_index);
/*
    opl_envs.conn_status[slave_index] = enOPL_DISCONNECTED;

    if (opl_envs.conn_status[0] == enOPL_DISCONNECTED && opl_envs.conn_status[1] == enOPL_DISCONNECTED)
    {
	    opl_status.opl_main_status = enOPL_DISCONNECTED;
    }
*/    
}

void optek_link_master_busy_ind (void)
{
    //master is connected with other device
    //DBG_Assert (0);
}

void optek_link_output_dma_end_callback(void) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_output_dma_end_callback(void)
{
    btdm_debug_pin(30, 1);
    if (global_vari.optek_link_role == OPL_ROLE_MASTER)
    {
        if (REQUEST_SYNC == opl_envs.synced_w_output)
        {
            dma_end_bt_time = rwip_time_get();
        }
    }
    btdm_debug_pin(30, 0);
}

void optek_link_mode_init (void)
{
    opl_envs.eventGroup = xEventGroupCreate();    
    opl_envs.synced_w_output = REQUEST_SYNC;
    opl_envs.working = FALSE;
    opl_envs.synced_w_input = REQUEST_SYNC;
}

#endif
