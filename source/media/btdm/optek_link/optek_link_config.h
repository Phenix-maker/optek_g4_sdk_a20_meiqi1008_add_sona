#ifndef OPTEK_LINK_CONFIG_H_
#define OPTEK_LINK_CONFIG_H_

#undef ASSERT_ERR
#define ASSERT_ERR


#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long long

#define int8_t signed char
#define int16_t signed short
#define int32_t signed int
#define int64_t signed long long

#define LOGIC_ADDR_0            0			//recive all ，no reject 
#define LOGIC_ADDR_DYNAMIC      0xFF		//recive ->LOGIC_ADDR_DYNAMIC

/// Modified by Optek
#define MAX_DYNAMIC_INTV_SIZE     147


#define BC_ACT_ID           1                   //multiconn
#define SCONN_ACT_ID        2                   //shaed connection, ACK/NACK

#define LINK_STATUS_BIT_MASK     0xE0

#define LINK_STATUS_LADDR_1_BIT_MASK     0x80
#define LINK_STATUS_LADDR_2_BIT_MASK     0x40
#define LINK_STATUS_LADDR_3_BIT_MASK     0x20
#define LINK_STATUS_LADDR_4_BIT_MASK     0x10


#define LINK_STATUS_LOGIC_1_BIT_MASK     0x80
#define LINK_STATUS_LOGIC_2_BIT_MASK     0x40
#define LINK_STATUS_LOGIC_3_BIT_MASK     0x20

#define LINK_STATUS_EXT_MASK     0x80


#define SLAVE_ACK_MASK          0x80
#define MASTER_DEV_1_MASK       0x80
#define MASTER_DEV_2_MASK       0x80
#define MASTER_DEV_3_MASK       0x80

#define AE_LEN_W_ADI        4
#define AE_LEN_W_ADI_DYN        6
#define AE_LEN_WO_ADI       1

#define AE_LEN_W_PARING     (2+6+1)                 //should be plused by 1???, min tx len 2+6+2+1

#define LINK_INDEX_POS      0
#define LINK_MODE_POS       1       
#define DYNAMIC_0p5ms_CN_H  2     
#define DYNAMIC_0p5ms_CN_L  3     


#define SLAVE_RX_TX_ADJUST  34

//#define RF_TEST
#ifndef RF_TEST
#define RX_SKIP_ENABLE
//#define RX_SKIP_SEC2_DBG
//#define TX_SKIP_DISABLE
#endif //RF_TEST

#define MAX_SLAVE_W_TX_NUM  4
#define MAX_LINK_LOSS_ERR   16       
#define NO_PT_END_TIME      0xFFFF

#define SLOT_TX         0
#define SLOT_RX         1
#define SLOT_IDLE       2



#ifdef RF_TEST

#define OPTEK_LINK_MAX_CH 8

const uint8_t optek_link_rf_test_ch[OPTEK_LINK_MAX_CH] =
{
0,12,24,16,22,28,32,36
};

#else

#define OPTEK_LINK_MAX_CH 37

#endif


/*
*       Optek Wireless Link 2.0 => Optek Link 2.0
*
*       Wireless Application music/voice/IoT/control system
*       Low Latency, input point to output point min delay < 10ms
*       Broadcast, no limitation for rx devices
*       Multi Connection with Low latency, 255 devices
*       Multi Connection with Long Rang (LE LR RF/baseband) and without low latency, 65535 devices 
*/
enum {
    //optek link 2.0
    OPTEK_LINK_MODE_MICx2_PT2P5MS                   = 1,     //sample frequemcy 48kHz, 2 mic tx(tx 4 slots each mic), 1 rx       
    //OPTEK_LINK_MODE_MICx2_PT1P25MS                  = 2,
    OPTEK_LINK_MODE_MICx1_PT1P25MS                  = 3,
    OPTEK_LINK_MODE_MICx1_PT0P625MS                 = 4,
    OPTEK_LINK_MODE_PT5MS_BC                        = 5,      //bc stereo music, delay < 17ms
    OPTEK_LINK_MODE_PT2P5MS_BC                      = 6,      //bc stereo music, delay < 10ms
    OPTEK_LINK_MODE_PT1P25MS_BC                     = 7,      //bc mono music (mic), delay < 5ms
    OPTEK_LINK_MODE_PT0P625MS_BC                    = 8,      //bc mono music (mic), delay < 3ms

    OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_BC          = 9,     //rx bt music and bc stereo music, 6 tx slots, better dev is master, delay about 20ms
    OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S1_SUBW     = 10,     //rx bt music and tx to 1 slave(0.1), max 6 tx slots, better dev is master, delay about 20ms
    OPTEK_LINK_MODE_PT2P721MS_S1_SUBW               = 11,     //pcm in and tx to 1 slave(0.1), max 6 tx slots, better dev is master, delay about 20ms
    OPTEK_LINK_MODE_PT2P5MS_S1_SUBW                 = 12,     //pcm in and tx to 1 slave(0.1), max 6 tx slots, better dev is master, delay about 20ms

    //OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S1          = 13,     //rx bt music and tx to 1 slave, 6 tx slots, better dev is master, delay about 20ms
    //OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S2          = 14,     //rx bt music and tx to 2 slave, 6 tx slots, better dev is master, delay about 20ms
    //OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S3          = 15,     //rx bt music and tx to 3 slave (2.1), 6 tx slots, better dev is master, delay about 20ms
    //OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S1_CH3      = 16,     //rx bt music and tx to 1 slave (2.1), 6 tx slots, better dev is master, delay about 20ms
    //OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S2_CH3      = 17,     //rx bt music and tx to 2 slave (2.1), 6 tx slots, better dev is master, delay about 20ms
    OPTEK_LINK_MODE_SHARED_CONN                     = 16,       //for tws application
    OPTEK_LINK_MODE_PT5P442MS_S1                    = 18,     //pcm in and tx to 1 slave, 6 tx slots, better dev is master, delay about 15ms
    OPTEK_LINK_MODE_PT5P442MS_S2                    = 19,     //pcm in  and tx to 2 slave, 6 tx slots, better dev is master, delay about 15ms
    OPTEK_LINK_MODE_PT5P442MS_S3                    = 20,     //pcm in and tx to 3 slave (2.1), 6 tx slots, better dev is master, delay about 15ms
    OPTEK_LINK_MODE_PT5P442MS_S1_CH3                = 21,     //pcm in and tx to 1 slave (2.1), 6 tx slots, better dev is master, delay about 15ms
    OPTEK_LINK_MODE_PT5P442MS_S2_CH3                = 22,     //pcm in and tx to 2 slave (2.1), 6 tx slots, better dev is master, delay about 15ms

    OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S1_MONO     = 23,     //rx bt music and tx to 1 slave (mono), 6 tx slots, better dev is master, delay about 20ms
    OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S1_HFP         = 24,     //rx bt music and tx to 1 slave (msbc), 6 tx slots, better dev is master, delay about 15ms

    OPTEK_LINK_MODE_USB_PT5MS_S1                    = 25,      //usb audio, tx bc and 6 slots, rx 1 slave
    OPTEK_LINK_MODE_USB_PT5MS_S2                    = 26,      //usb audio, tx bc, rx 2 slave
    OPTEK_LINK_MODE_USB_PT2P5MS_S1                  = 27,      //usb audio, tx bc, rx 1 slave

    OPTEK_LINK_MODE_MICx2_PT5P442MS                 = 28,      //sample frequemcy 44.1kHz, 2 mic tx(tx 6 slots each mic), 1 rx 
    OPTEK_LINK_MODE_MICx3_PT5P442MS                 = 17,      //sample frequemcy 44.1kHz, 2 mic tx(tx 4 slots each mic), 1 rx     
    OPTEK_LINK_MODE_MICx4_PT8P16MS                  = 29,      //sample frequemcy 44.1kHz, 4 mic tx(tx 4 slots each mic). 1 rx
    OPTEK_LINK_MODE_MICx4_PT8P16MS_POLLING          = 30,      //sample frequemcy 44.1kHz, 4 mic tx(tx 4 slots each mic). 1 rx and polling total 63 mics

    OPTEK_LINK_MODE_USB_PT5MS_BC                    = 31,      //usb audio, tx bc and 7 slots
    OPTEK_LINK_MODE_USB_PT7P5MS_S1                  = 32,      //usb audio, tx bc and 6 slots, rx 1 slave    
    OPTEK_LINK_MODE_USB_PT7P5MS_w_HFP_S1            = 33,       //usb audio w HFP, tx bc and 4 slots, rx 1 slave 
    OPTEK_LINK_MODE_USB_PT5MS_S1_w_Up_Voice         = 34,
    OPTEK_LINK_MODE_USB_PT2P5MS_BC                  = 35,
    OPTEK_LINK_MODE_USB_PT7P5MS_S2_w_Up_Voice       = 36,

    OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S2           = 37,       //L,R,SUBW
    OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S2_2_CH      = 38,       //L,R
    OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S3           = 39,       //L,R,SUBW
    OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S1           = 40,       //L,R,SUBW
    OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S1_2_CH      = 41,       //L,R
    OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_S1_SUBW      = 42,       //SUBW
    OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S2             = 43,       //L,R,SUBW
    OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S2_2_CH        = 44,       //L,R
    OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S3             = 45,       //L,R,SUBW
    OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S1             = 46,       //L,R,SUBW
    OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S1_2_CH        = 47,       //L,R
    OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_S1_SUBW        = 48,       //SUBW
    OPTEK_LINK_MODE_USB_PT5MS_BC_6T                 = 49,       //sane with OPTEK_LINK_MODE_USB_PT5MS_BC except full_band = 0 and tx 6 slots for phome call detect
    OPTEK_LINK_MODE_USB_PT2P5MS_S1_w_HFP            = 50,       //detect phone call
    OPTEK_LINK_MODE_USB_PT7P5MS_S2_tmono_w_Up_Voice = 51,       //usb audio tx mono
    OPTEK_LINK_MODE_USB_PT5P442MS_S1_w_Up_Voice     = 52,
    OPTEK_LINK_MODE_USB_PT5P442MS_S1                = 53,
    OPTEK_LINK_MODE_USB_PT2P721MS_S1_w_HFP          = 54,
    OPTEK_LINK_MODE_USB_PT2P721MS_S1                = 55,
    OPTEK_LINK_MODE_USB_PT2P721MS_BC                = 56,
    OPTEK_LINK_DETECT                               = 57,            
    OPTEK_LINK_MODE_MICx2_PT5MS                     = 59,      //sample frequemcy 44.1kHz, 2 mic tx(tx 6 slots each mic), 1 rx 
    OPTEK_LINK_MODE_PT8P163MS_BT_MUSIC_MONO_SUBW_S1 = 60,
    OPTEK_LINK_MODE_PT7P5MS_BT_MUSIC_MONO_SUBW_S1   = 61,
    OPTEK_LINK_MODE_PT5P442MS_BT_MUSIC_MONO_SUBW_S1 = 62,
    OPTEK_LINK_MODE_PT5MS_BT_MUSIC_MONO_SUBW_S1     = 63,    
    OPTEK_LINK_MODE_USB_PT8P163MS_S1                = 64,      //usb audio, tx bc and 6 slots, rx 1 slave 
    OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S3          = 65,
    OPTEK_LINK_MODE_PT10MS_BT_MUSIC_S3              = 66,     
    OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S2          = 67,
    OPTEK_LINK_MODE_PT10MS_BT_MUSIC_S2              = 68,
    OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S1          = 69,
    OPTEK_LINK_MODE_PT10MS_BT_MUSIC_S1              = 70,

    OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S2_2_CH     = 71,     
    OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_S1_2_CH     = 72,     
    OPTEK_LINK_MODE_PT10MS_BT_MUSIC_S2_2_CH         = 73,   
    OPTEK_LINK_MODE_PT10MS_BT_MUSIC_S1_2_CH         = 74,           

    OPTEK_LINK_MODE_PT10P884MS_BT_MUSIC_MONO_SUBW_S1 = 75,
    OPTEK_LINK_MODE_PT10MS_BT_MUSIC_MONO_SUBW_S1     = 76,      

    OPTEK_LINK_MODE_USB_PT10MS_S1                   = 77,
    OPTEK_LINK_MODE_USB_PT10P884MS_S1               = 78,
    
    OPTEK_LINK_MODE_PT25MS_BT_MUSIC_MONO_SUBW_S1    = 13,
    OPTEK_LINK_MODE_PT25P884MS_BT_MUSIC_MONO_SUBW_S1    = 14,   
    OPTEK_LINK_MODE_PT20MS_BT_MUSIC_MONO_SUBW_S1    = 15,
    OPTEK_LINK_MODE_PT21P768MS_BT_MUSIC_MONO_SUBW_S1    = 79,   
    OPTEK_LINK_MODE_PT15MS_BT_MUSIC_MONO_SUBW_S1    = 80,
    OPTEK_LINK_MODE_PT16P326MS_BT_MUSIC_MONO_SUBW_S1    = 81,   

    OPTEK_LINK_MODE_MAX                             = 82,

};


enum {
    OPTEK_LINK_MASTER           =   0,
    OPTEK_LINK_SLAVE           =   1,
    OPTEK_LINK_SCAN             =   2,
    OPTEK_LINK_SLAVE_SYNCED    =   3,
};

#define OPTEK_LINK_H1						0x01			/*for multi conn*/
#define OPTEK_LINK_H2						0x02			/*for shared conn ack/nack*/

#define MAX_BC_NUM  32

//#define DYN_0p5us_CN

#define MAX_FRAMES      4

/// Time information
/*@TRACE*/
typedef struct
{
    /// Integer part of the time (in half-slot)
    uint32_t hs;
    /// Fractional part of the time (in half-us) (range: 0-624)
    uint16_t hus;
    /// Bluetooth timestamp value (in us) 32 bits counter
    uint32_t bts;
} rwip_time_t;

struct optek_link_tag
{
    uint8_t link_role;
    uint8_t link_mode;
    uint8_t link_mode_req;
    uint8_t link_mode_req_en;
    uint8_t link_pri;
    uint8_t link_pri_req;

    uint8_t slave_master_n;             /*0->master, 1->slave*/    
    uint8_t slave_syned;
    uint8_t scan;                       /*in scan phase*/

    uint8_t full_band;
    uint8_t sch_index;

    uint8_t ch_ind;

    uint8_t bc_num;                     /*total tx/rx slots, shoud < MAX_BC_NUM*/    
    uint8_t bc_index;                   /*current tx/rx slot in bc_num*/
    
    uint8_t slave_bc_num;
    uint8_t slave_bc_index;
    uint8_t slave_bc_ext_frame;          /*slave bc frame more than 1 packet time*/         

    uint8_t tx_data_len;
    uint8_t tx_data_req;
	
    uint16_t bc_index_offset[MAX_BC_NUM];
    uint16_t tx_rx_len[MAX_BC_NUM];
    uint16_t max_slave_rx_len;

    uint16_t max_master_rx_len;

    uint8_t rx_tx_n[MAX_BC_NUM];

    uint8_t channel[OPTEK_LINK_MAX_CH];
    uint8_t chan_size;

    uint8_t aa [4];                     /*access code based on bt addr*/

    uint8_t public_addr[6];
    //uint8_t public_addr_present;

    uint8_t logic_conn_addr;                    //multi link multi conn ( >2 ), 0xFF->dynamic     
    uint8_t logic_addr;                         //multi link multi conn ( >2 ), logic addr. 0->only rx, 0xFF->dynamic, max 4     
    uint8_t slave_rx_all;                       //multi link multi conn ( > 2), rx all packet
    uint8_t rx_skip_disable;                    //test for slot timing 

    uint8_t dynamic_intv_table[MAX_DYNAMIC_INTV_SIZE];
    uint8_t dynamic_intv_index;    
    uint8_t dynamic_intv_size;

    uint8_t *tx_data;
    uint8_t *tx_ae_header_data;


    uint8_t paring_en;
    uint8_t paring_ind;
    uint8_t paring_percentage;
    uint8_t paring_percentage_tick;    
    uint16_t paring_time;
    uint16_t paring_ticks;

    uint8_t polling;

    uint8_t paring_group[3];            //crc

    uint8_t tx_power;
    uint8_t paring_power;

    uint8_t tx_skip;
    uint8_t rx_skip;
    //uint8_t rx_master_sect_2_skip;
    uint8_t rx_ext_skip[MAX_FRAMES];
    uint8_t max_rx_len[MAX_BC_NUM];
    uint8_t max_tx_len[MAX_BC_NUM];

    uint8_t rx_ext_cn;

    rwip_time_t intv;


    uint16_t scan_interval;
    uint16_t scan_window;
    uint16_t scan_start_random;

    uint8_t synced_1st;
    uint8_t link_end_req;

    uint8_t master_sect_1_end;
    uint8_t master_sect_2_start;                     //master sect 1 and last must be master tx slot

    uint8_t slave_tx_start;
    uint8_t interface;                              //one frame, txrxtx

    //uint8_t paring_num;
    uint32_t rx_err;
    uint16_t slave_rx_err;
    uint8_t slave_rx_mode_err;

    uint16_t master_rx_err[MAX_SLAVE_W_TX_NUM];
    uint16_t master_rx_frame_end[MAX_SLAVE_W_TX_NUM];
    uint8_t master_tx_skip[MAX_SLAVE_W_TX_NUM];    
    uint8_t master_link_status;
    uint8_t slave_rx_status_mask_bit;

    //for slave sync with master
    uint8_t  slot_index_0_boundary;          //slot is exactly 2.5ms * N
    uint8_t  last_rssi;
    uint8_t  rxsync_1st_present;

    uint32_t base_cnt_rxsync_1st;
    uint16_t fine_cnt_rxsync_1st;
    uint32_t base_cnt_rxsync_last;
    uint16_t fine_cnt_rxsync_last;

#ifdef DYN_0p5us_CN
    int16_t adjust_0p5us_cn_1st;
    int16_t adjust_0p5us_cn_last;
#endif

    int32_t swap_cn;

    //master tx  dymanic sync with external audio source (usb/optical/slave i2s)
    int8_t dynamic_sync;                   //0->no bynamic sync, 1->+0.5us, -1->0.5us
    uint8_t sync_step;
    uint16_t sync_int;
    uint16_t sync_int_cnt;
    uint32_t sync_frac;
    uint32_t sync_frac_cnt;
#ifdef DYN_0p5us_CN
    int16_t adjust_0p5us_cn;
#endif

    uint8_t frac_ov;

    uint32_t duration_min;
    uint32_t duration_min_w_rx_resp;

    uint8_t rx_resp;                //need slave to send rx_resp, 0->rx not secc, others->rx succ and skip slots
    uint8_t sched_shrink;           //if rx_resp = 1, shrink tx slot depends on rx  


    uint8_t rx_buf;                 //store rx info
    uint8_t rx_len;

    //for multi mics
    uint8_t max_mics;
};


#define BLE_PREFETCH_TIME_US       (90)             //based on test, run 64Mhz/sdram debug, 85usng, 86ok
#define BLE_PREFETCHABORT_TIME_US  (BLE_PREFETCH_TIME_US+30)


/// Margin for event time reservation (in half-us)
#define BLE_RESERVATION_TIME_MARGIN_HUS    (2 * BLE_PREFETCHABORT_TIME_US)

#endif