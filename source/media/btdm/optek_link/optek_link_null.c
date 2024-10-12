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

#ifdef OPL_MODE_NULL
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
#include "xa_error_standards.h"


#ifdef OPL_MODE_NULL

static const U8 sw_role_table[] = 
{
	OPL_ROLE_MASTER,
    OPL_ROLE_SLAVE,
};

const SW_ROLE_t  sw_role_info = {sw_role_table,sizeof(sw_role_table)/sizeof(sw_role_table[0])};

extern void optek_link_set_timer (uint16_t val);

void optek_link_codec_init(U16 sample_rate,U8 bt_src)
{

}

void optek_link_put_tx_data(short *ptr,U16 len)
{

}

U8 dec_get_optek_link_rx_data(U8 **pOut, U8 rx_sel, U8 len)
{
    U8 ret = 0;

    return ret;
}

void optek_link20_timer1_in_frame(u32 link_role, u32 link_mode) __attribute__ ((section (".internal_ram_1_text"))); 
void optek_link20_timer1_in_frame (u32 link_role, u32 link_mode)
{

}

void optek_link20_start_in_frame (u32 link_role, u32 link_mode, u8 *tx_buf, u16 tx_buf_len)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link20_start_in_frame (u32 link_role, u32 link_mode, u8 *tx_buf, u16 tx_buf_len)
{

}

uint32_t data_err_counter = 0;

u32 optek_link20_rx_in_frame (u32 link_role, u32 link_mode, u32 frame_id, U8 slot_idx, u8 *rx_buf, u16 rx_buf_len, u8 *rx_status, u8 crc_err)  __attribute__ ((section (".internal_ram_1_text")));
u32 optek_link20_rx_in_frame (u32 link_role, u32 link_mode, u32 frame_id, U8 slot_idx, u8 *rx_buf, u16 rx_buf_len, u8 *rx_status, u8 crc_err)
{
	return 1;			//internal crc ok
}

u32 optek_link20_rx_end (u32 link_role, u32 link_mode, u32 frame_id)  __attribute__ ((section (".internal_ram_1_text")));
u32 optek_link20_rx_end (u32 link_role, u32 link_mode, u32 frame_id)
{
  
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

/*
	when dma end, should get dma_end_time(rwip_time_get)
*/
rwip_time_t rwip_time_get(void);
void optek_link20_get_master_start_time (rwip_time_t *dma_end_time, uint32_t *delay_time_hus)
{
	*dma_end_time = rwip_time_get();
	*delay_time_hus = (7500-100)*2;
    //opl_envs.synced = TRUE;
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

void optek_link_slave_link_loss_cb (u8 id)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link_slave_link_loss_cb (u8 id)
{

}

void optek_link_master_busy_ind (void)
{

}

void optek_link_output_dma_end_callback(void) __attribute__ ((section (".internal_ram_1_text")));
void optek_link_output_dma_end_callback(void)
{

}

void optek_link_master_link_loss_cb (u8 frame_index, u8 id)  __attribute__ ((section (".internal_ram_1_text")));
void optek_link_master_link_loss_cb (u8 frame_index, u8 id)
{
//	DBG_iPrintf("master loss\r");
}

void optek_link_mode_init (void)
{
}

#endif

#endif
