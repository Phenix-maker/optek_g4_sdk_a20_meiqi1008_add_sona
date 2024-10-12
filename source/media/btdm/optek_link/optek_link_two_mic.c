/**
 ****************************************************************************************
 *
 * @file optek_link_two_mic.c
 *
 * @brief optek link two mic audio path.
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

#ifdef OPL_MODE_TWO_MIC

//#define OPTEK_LINK_DEBUG

#define MASTER_TX_LEN 6
#define SLAVE_TX_LEN  6

XA_ERRORCODE xa_lc3_enc_main_init(U32 bitrate,U16 samplerate,U8 channels,float frame_Time_ms);
int xa_lc3_enc_frame(U16 *pInBuf, U32 samples, U8 **pOutBuf, U32 *pOutDataLen);
XA_ERRORCODE xa_lc3_dec_handle_init(void **decode_handle,U32 bitrate,U16 samplerate,U8 channels,float frame_Time_ms);
XA_ERRORCODE xa_lc3_dec_frame (void *decode_handle,U8 *pIn,U8 inLen,U8 **pout_buf, U16 *out_byte_len);
U8 dec_get_optek_link_rx_data(U8 **pOut, U8 rx_sel, U8 len);

extern u8 master_bdaddr[6];

volatile APK_AND_TX_SYNC_STATUS_ENUM spk_out_sync_w_tx_status;

#define MAX_SLAVES 2

typedef struct {
    u8 rx_data[2][32];
    U32 rx1_frame_id;
    U32 rx2_frame_id;
    U8 rx1_len;
    U8 rx2_len;
    U8 tx_len;
	U8 *tx_data;
} OPL_DATA_t;

typedef struct {
    OPL_DATA_t opl_data;
    void * decodeHandle[MAX_SLAVES];

    uint8_t synced;
    uint8_t working;
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
};

const SW_ROLE_t  sw_role_info = {sw_role_table, sizeof(sw_role_table)/sizeof(sw_role_table[0])};

void *rx1_decode_hanle;

void optek_link_trans_init(void) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_trans_init(void)
{
    opl_envs.opl_data.tx_len = 0;
    opl_envs.working = FALSE;
}

void optek_link_codec_init(U16 sample_rate,U8 bt_src)
{
	int i;
	optek_link_trans_init();

    DBG_Assert(sample_rate == 48000);    
    DBG_Assert(bt_src == 0);

    if (0)
    {

    }
	#ifdef OPL_SLAVE_ENBALE
	else if (global_vari.optek_link_role == OPL_ROLE_SLAVE || global_vari.optek_link_role == OPL_ROLE_SLAVE2)
	{
        if (xa_lc3_enc_main_init(30*8*400, sample_rate, 1, 2.5) != XA_NO_ERROR)
        {
            DBG_Printf("lc3 enc init error\n\r");
        }
	}
	#endif
	#ifdef OPL_MASTER_ENABLE
	else if (global_vari.optek_link_role == OPL_ROLE_MASTER)
    {
        for (i=0; i<MAX_SLAVES; i++)
        {
            if (xa_lc3_dec_handle_init(&opl_envs.decodeHandle[i], 30*8*400, sample_rate, 1, 2.5) != XA_NO_ERROR)
            {
                DBG_Printf("slave %d lc3 dec init error\n\r", i+1);
            }
        }
    }
    #endif    
    else
    {
        DBG_Assert(FALSE);
    }
}

void optek_link_put_tx_data(short *ptr,U16 len) __INTERNAL_RAM_TEXT;
void optek_link_put_tx_data(short *ptr,U16 len)
{
    int i;
	if (opl_status.opl_main_status != enOPL_CLOSED)
	{	
        if (0)
        {

        }
    #ifdef OPL_SLAVE_ENBALE
		else if (global_vari.optek_link_role == OPL_ROLE_SLAVE || global_vari.optek_link_role == OPL_ROLE_SLAVE2)
		{
			//if(opl_envs.opl_data.tx_len == 0)
			{
				U8 *pEncData;
				U32 encDataLen;
				xa_lc3_enc_frame(ptr, len>>2, &pEncData, &encDataLen);
				//memcpy(&opl_data.tx_data[0],pEncData,encDataLen);
                opl_envs.opl_data.tx_data = pEncData;
				opl_envs.opl_data.tx_len = encDataLen;

				//DBG_Assert(opl_data.tx_len <= MASTER_TX_LEN);
			}
			//else
			{
				//DBG_Printf("tx loss packet\r\n");
			}
		}
    #endif    
	#ifdef OPL_MASTER_ENABLE
		else if (global_vari.optek_link_role == OPL_ROLE_MASTER)
		{
            U8 inLen;
            U8 *p_rx;
            U8 *pDecOut[MAX_SLAVES];
            U16 outlen[MAX_SLAVES];

            for (i=0; i<MAX_SLAVES; i++)
            {
                inLen = get_lc3_decode_frame_bytes(opl_envs.decodeHandle[i]);
                inLen = dec_get_optek_link_rx_data(&p_rx, (U8)i, inLen);

                xa_lc3_dec_frame(opl_envs.decodeHandle[i], p_rx, inLen, &pDecOut[i], &outlen[i]);
                DBG_Assert(outlen[0] == outlen[i]);
            }

            #if 1//max slaves equal to 2
            DBG_Assert(len == outlen[0]*2);

            short *pin1 = pDecOut[0];
            short *pin2 = pDecOut[1];
            short *pout = ptr;
            for (i=0; i<outlen[0]/2; i++)
            {
                *pout++ = *pin1++;
                *pout++ = *pin2++;
            }
            #if 0//put data to usb upstream
            extern FIFO UsbDevUpFifo;
            int free_space_len = fifo_get_free_space(&UsbDevUpFifo);

            if (free_space_len < len)
            {
                //DBG_Printf("usb mic overflow\n\r");
            }

            int putlen = fifo_put_data(&UsbDevUpFifo, ptr, len);
            #endif
            #endif
		}
	#endif
        else
        {
            DBG_Assert(FALSE);
        }
	}	
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
	btdm_debug_pin(29, 0);

	int i;

#ifdef OPL_SLAVE_ENBALE
	if (global_vari.optek_link_role == OPL_ROLE_SLAVE || global_vari.optek_link_role == OPL_ROLE_SLAVE2)
    {
        if (FALSE == opl_envs.synced)
        {
            opl_envs.synced = TRUE;

            hw_audio_qadc_fifo_enable();
            //audio_aux_input_rx_restart_req(48*2);
            decode_start_w_init_msgSend_from_isr();
            //DBG_iPrintf("synced!\n");
        }

        memcpy(tx_buf, opl_envs.opl_data.tx_data, opl_envs.opl_data.tx_len);
        //DBG_iPrintf("%d,%x,%x\n", opl_envs.opl_data.tx_len, tx_buf[15], tx_buf[29] );
        opl_envs.opl_data.tx_len = 0;
/*
        for (i=0; i < 30; i++)
        {
            tx_buf[i] = i;
        }
*/
    }
#endif

    if (FALSE == opl_envs.working) 
    {
        opl_envs.working = TRUE;
    }  
}

uint32_t data_err_counter = 0;

u32 optek_link20_rx_in_frame (u32 link_role, u32 link_mode, u32 frame_id, U8 slot_idx, u8 *rx_buf, u16 rx_buf_len, u8 *rx_status, u8 crc_err)  __attribute__ ((section (".internal_ram_1_text")));
u32 optek_link20_rx_in_frame (u32 link_role, u32 link_mode, u32 frame_id, U8 slot_idx, u8 *rx_buf, u16 rx_buf_len, u8 *rx_status, u8 crc_err)
{

#if 1
    if (0 == crc_err)
    {
        if (slot_idx % 2 == 0 && opl_envs.opl_data.rx1_frame_id != frame_id)
        {
            btdm_debug_pin(29, 1);
            btdm_debug_pin(29, 0);            
            opl_envs.opl_data.rx1_frame_id = frame_id;
            memcpy(opl_envs.opl_data.rx_data[0], rx_buf, rx_buf_len);
            opl_envs.opl_data.rx1_len = rx_buf_len;
        }
        else if (slot_idx % 2 == 1 && opl_envs.opl_data.rx2_frame_id != frame_id)
        {
            opl_envs.opl_data.rx2_frame_id = frame_id;
            memcpy(opl_envs.opl_data.rx_data[1], rx_buf, rx_buf_len);
            opl_envs.opl_data.rx2_len = rx_buf_len;
        }
    }
#else
	int i;
	if (!crc_err)
	{
		for (i=0; i < 30; i++)
		{
			if (rx_buf[i] != i)
			{
				data_err_counter++;
				DBG_iPrintf("i:%d,d:%d\n", i, rx_buf[i]);
			}
			//DBG_Assert(FALSE);
		}	
	}
#endif
	return 1;			//internal crc ok
}

u32 optek_link20_rx_end (u32 link_role, u32 link_mode, u32 frame_id)  __attribute__ ((section (".internal_ram_1_text")));
u32 optek_link20_rx_end (u32 link_role, u32 link_mode, u32 frame_id)
{
    btdm_debug_pin(28, 1);
    btdm_debug_pin(28, 0);
     
    if (opl_envs.opl_data.rx1_frame_id != frame_id)
    {
        opl_envs.opl_data.rx1_len = 0;
        //DBG_iPrintf("rx1 err\n");
    }

    if (opl_envs.opl_data.rx2_frame_id != frame_id)
    {
        opl_envs.opl_data.rx2_len = 0;
        //DBG_iPrintf("rx2 err\n");
    }

    BaseType_t xHigherPriorityTaskWoken1, xHigherPriorityTaskWoken2, xResult;

	// xHigherPriorityTaskWoken must be initialised to pdFALSE.
	xHigherPriorityTaskWoken1 = pdFALSE;
    xHigherPriorityTaskWoken2 = pdFALSE;

    xEventGroupSetBitsFromISR(opl_envs.eventGroup, 2, &xHigherPriorityTaskWoken2);

	// Set bit 0 and bit 4 in xEventGroup.
    xResult = xEventGroupSetBitsFromISR(opl_envs.eventGroup, 1, &xHigherPriorityTaskWoken1);
    if( xResult == pdPASS)
    {
        if (xHigherPriorityTaskWoken1 || xHigherPriorityTaskWoken2)//If the priority of the timer daemon task is higher than the priority of the currently running task (the task the interrupt interrupted)
        {
            portYIELD_FROM_ISR();
        }
    }
    else
    {
        DBG_Assert(FALSE);//pdFALSE will be returned if the timer service queue was full
    }    
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
U8 dec_get_optek_link_rx_data(U8 **pOut, U8 rx_sel, U8 len)
{
    U8 ret = 0;

    if (opl_envs.working)
    {
        if (rx_sel == 0)
        {
            xEventGroupClearBits(opl_envs.eventGroup, 1);
            xEventGroupClearBits(opl_envs.eventGroup, 2);
            xEventGroupWaitBits(opl_envs.eventGroup, 1, pdTRUE, pdFALSE, pdMS_TO_TICKS(100));
    btdm_debug_pin(27, 1);
    btdm_debug_pin(27, 0);
            if (opl_envs.opl_data.rx1_len)
            {
                *pOut = opl_envs.opl_data.rx_data[0];
                DBG_Assert(opl_envs.opl_data.rx1_len == len);
                ret = len;
            }
        }

        if (rx_sel == 1)
        {
            xEventGroupWaitBits(opl_envs.eventGroup, 2, pdTRUE, pdFALSE, pdMS_TO_TICKS(100));

            if (opl_envs.opl_data.rx2_len)
            {
                *pOut = opl_envs.opl_data.rx_data[1];
                DBG_Assert(opl_envs.opl_data.rx2_len == len);
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
	//just for demo
	*dma_end_time = rwip_time_get();
	*delay_time_hus = (2500-100)*2;
    opl_envs.synced = TRUE;	
}
u32 optek_link20_get_master_tx_rx_len (uint8_t mode, uint16_t *max_m_tx_len, uint16_t *max_s_tx_len)
{
    u32 succ = 1;

    *max_m_tx_len = 0;
    *max_s_tx_len = 30; 

    return succ;
}

uint8_t optek_link20_slave_conn_cb(uint8_t mode, uint16_t *pmax_m_tx_len, uint16_t *pmax_s_tx_len,  uint8_t id) __attribute__ ((section (".internal_ram_1_text")));
uint8_t optek_link20_slave_conn_cb(uint8_t mode, uint16_t *pmax_m_tx_len, uint16_t *pmax_s_tx_len,  uint8_t id)
{
#ifdef OPL_SLAVE_ENBALE
	if (global_vari.optek_link_role == OPL_ROLE_SLAVE || global_vari.optek_link_role == OPL_ROLE_SLAVE2)
    {
        opl_envs.synced = FALSE;
        hw_audio_qadc_fifo_disable();
    }

    uint16_t max_m_tx_len, max_s_tx_len;

    optek_link20_get_master_tx_rx_len (mode, &max_m_tx_len, &max_s_tx_len);

    //swap master and slave tx/rx len
    *pmax_m_tx_len = max_m_tx_len;
    *pmax_s_tx_len = max_s_tx_len;
    //opl_envs.conn_status[0] = enOPL_CONNECTED;
    //opl_status.opl_main_status = enOPL_CONNECTED;

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
	if (global_vari.optek_link_role == OPL_ROLE_SLAVE || global_vari.optek_link_role == OPL_ROLE_SLAVE2)
    {
        //DBG_iPrintf("syncing\n");
        opl_envs.synced = FALSE;
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
    decode_exit_req_set_from_isr();
    //DBG_Assert(FALSE);
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
    if (global_vari.optek_link_role == OPL_ROLE_MASTER)
    {
        if (FALSE == opl_envs.synced)
        {
            dma_end_bt_time = rwip_time_get();
        }
    }
}

void optek_link_mode_init (void)
{
    opl_envs.eventGroup = xEventGroupCreate();    
    opl_envs.synced = FALSE;
    opl_envs.working = FALSE;
}

#endif
