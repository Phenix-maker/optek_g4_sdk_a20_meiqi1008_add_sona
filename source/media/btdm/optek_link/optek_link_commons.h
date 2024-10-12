#ifndef _OPTEK_LINK_COMMONS_H_
#define _OPTEK_LINK_COMMONS_H_

#include "optek_link_config.h"

#define OPTEK_LINK_RX_QUALITY_DEBUG

#ifdef OPTEK_LINK_RX_QUALITY_DEBUG
typedef enum {
    enumRETRANSMISSION_0 = 0,
    enumRETRANSMISSION_1,
    enumRETRANSMISSION_2,
    enumRETRANSMISSION_3,
    enumRETRANSMISSION_4,
    enumRETRANSMISSION_5,
    enumRETRANSMISSION_6,
    enumRETRANSMISSION_7,
    enumRETRANSMISSION_8,
    enumRETRANSMISSION_9,
    enumRETRANSMISSION_10,
    enumRETRANSMISSION_11,
    enumRETRANSMISSION_12,
    enumRETRANSMISSION_13,
    enumRETRANSMISSION_14,
    enumTRANSMISSION_FAILED
} EnumRetransmissionCount;
extern U32 optek_link_rx_counters[enumTRANSMISSION_FAILED+1];
void optek_link_rx_quality_print(void);
#endif

const char *optek_link_data_get(void);
const char *optek_link_time_get(void);
const char *optek_link_version_get(void);

typedef enum {
	SPK_AND_TX_NOTSYNC,
	SPK_AND_TX_SYNCING,
	SPK_AND_TX_SYNCING_STEP2,
    SPK_AND_TX_SYNCING_STEP3,	
	SPK_AND_TX_SYNCED,
}APK_AND_TX_SYNC_STATUS_ENUM;

typedef enum {
	NO_SYNC_REQUIRED,
	REQUEST_SYNC,
	SYNC_IN_PROGRESS_STEP1,
    SYNC_IN_PROGRESS_STEP2,	
	SYNC_COMPLETED,
}enSyncStatus;

//extern volatile APK_AND_TX_SYNC_STATUS_ENUM spk_out_sync_w_tx_status;

void optek_link_codec_init(U16 sample_rate,U8 bt_src);
void optek_link_put_tx_data(short *ptr,U16 len);
void app_optek_link_role_sw(void);
void app_nav_optek_link_pairing(void);
void optek_link_role_stop(void);
void app_change_mode_req(u8 mode, u8 max_master_tx_len, u8 max_master_rx_len);
extern U8 optek_link_mode;

//should set BLE_EXT_ADV_MASK
void optek_link_init(void);

#define OPTEK_LINK_MAX_SLOT

#define OPTEK_LINK_POWER            (0x80|0x20)
#define OPTEK_LINK_PARING_POWER     (0x80|0x10)

#define OPTEK_LINK_MASTER_NO_RX_LEN 0x06            //just for bt addr size
#define OPTEK_LINK_MASTER_RX_48K_5MS_ST_LEN         100           //160kbps
#define OPTEK_LINK_MASTER_RX_24K_10MS_MONO_LEN      50            //40kbps
#define OPTEK_LINK_MASTER_RX_24K_5MS_MONO_LEN       25            //40kbps	

struct optek_link_setting_tag
{
    uint8_t role;                               //optek link role
    uint8_t mode;                               //optek link mode, just for master          
    uint8_t *btaddr;                            //optek link master addr or null in scan role, master role (using the deivce bt addr)
    uint8_t *link_group;                        //optek link group, 24 bit random number 
    uint16_t window;                            //scan window, N * 0.625 ms, window > 4, window < interval  
    uint16_t interval;                          //scan interval, N * 0.625 ms, interval > 4
    uint8_t tx_power;                           //optek link tx power
    uint8_t paring_power;                       //optek link master paring power
    uint8_t max_tx_len;                         //optek link master tx len
    uint8_t max_rx_len;                         //optek link should has rx len to constarin rx timing, for master it should have one value, for slave 0->setting by the first rx len
    uint8_t *max_multi_rx_len;                  //optek link should has rx len  for every slot to constarin rx timing, for master it should have one value, for slave 0->setting by the first rx len    
    uint8_t logic_conn_addr;                    //multi link multi conn ( >2 ), 0xFF->dynamic
    uint8_t logic_addr;                         //multi link multi conn ( >2 ), logic addr. 0->only rx, 0xFF->dynamic, max 4 based on application     
    uint8_t slave_rx_all;                       //multi link multi conn ( > 2), rx all packet
    uint8_t rx_skip_disable;                    //test for slot timing    
};


/*
*  optek_link_par       point to  optek_link_env_tag                      
*  id                   optek link id, OPTEK_LINK_H1 or OPTEK_LINK_H2
*  return               1->succ, 0->fail
*/
u8 optek_link_set (struct optek_link_setting_tag *optek_link_setting, u8 id);

/*
*  swap_cn              < 0 slow, > 0 fast; (abs(swap_cn) - 1) swap count
*  id                   optek link id, OPTEK_LINK_H1 or OPTEK_LINK_H2
*/

u8 optek_link_get_slave_time_sync_info (u32 *base_cnt_rxsync_1st, u16 *fine_cnt_rxsync_1st,
                                            u32  *base_cnt_rxsync_last, u16 *fine_cnt_rxsync_last,
                                            i32 *swap_cn, i16 *dyn_hus_cn_1st, i16 *dyn_hus_cn_last, u8 id);



u8 ld_acl_get_slave_time_sync_info (u32 *base_cnt_rxsync_1st, u16 *fine_cnt_rxsync_1st,
                                            u32  *base_cnt_rxsync_last, u16 *fine_cnt_rxsync_last,
                                            int32_t *swap_cn);


/*
*  id                   optek link id, OPTEK_LINK_H1 or OPTEK_LINK_H2
*/

void optek_link_get_slave_time_sync_init (u8 id);



/*
*  sync_mode            0->no sync, step = abs(sync_mode) and should <= 4, 
*                           sync_mode > 0 -> +step(fast->slow) in one packet time, sync_mode < 0 -> -step(slow->fast) in one packet time
*  sync_int             Fext%Fdsp 
*  sync_frac            Fext/Fdsp frac part
*  act_id               optek link id, OPTEK_LINK_H1 or OPTEK_LINK_H2
*  retern               1->succ, others->fail
*/

uint8_t optek_link_set_master_tx_dynamic_sync (int8_t sync_mode, uint16_t sync_int,  uint32_t sync_frac, uint8_t id);


/**
 *      enable 1 or 0  
 *      percentage 0,1->full in the special slot, 2->1/2, 3->1/3.....
 *      time, 0->forever, others N x packet time (eg 2.5ms, 5ms..., based on mode) 
 *      id OPTEK_LINK_H1 or OPTEK_LINK_H2
 *      return 1->succ, 0->fail
 * */

u8 optek_link_set_master_paring (u8 enable, u8 percentage, u16 time, u8 id);

/**
 *      enable 1,0
 *      id OPTEK_LINK_H1 or OPTEK_LINK_H2
 *      return 1->succ, 0->fail 
 * */

u8 kBtHCI_optek_link_enable(u8 enable, u8 id);


u8 optek_link_change_mode (u8 mode, u8 max_master_tx_len, u8 max_master_rx_len, u8 id);
u8 optek_link_role_change_slave_to_scan (u8 act_id);

/**
 *      sleep_time->    n * 0.3125 ms
 *      id OPTEK_LINK_H1 or OPTEK_LINK_H2
 *      return 1->succ, 0->fail 
 * */
u8 optek_link_set_master_sleep_time (u16 sleep_time, u8 id);

uint8_t optek_link_role_change_slave_to_scan (uint8_t act_id);

typedef enum
{
    enOPL_CLOSED,//default(power on status)
    enOPL_CLOSING,
    enOPL_OPENING,
    enOPL_OPENED,
	enOPL_CONNECTED,
	enOPL_DISCONNECTED,
	enOPL_ERROR,
	enOPL_UNKNOWN = 0xFF
} enOPL_MAIN_STATUS;

typedef struct {
	enOPL_MAIN_STATUS opl_main_status;
} OPL_STATUS_s;
extern OPL_STATUS_s opl_status;

typedef struct {
    U8 *sw_role_table;
    U8 table_size;
} SW_ROLE_t;

extern const SW_ROLE_t  sw_role_info;

void optek_link_set_role(U8 role);
U8 optek_link_get_role(void);
U8 optek_link_get_role_w_check(void);
U8 optek_link_is_slave(void);
U8 optek_link_is_master(void);
U8 optek_link_is_disable(void);

void app_optek_link_role_sw(void);

void app_optek_link_init(void);
void app_optek_link_open(void);
void app_change_mode_check(void);
#endif
