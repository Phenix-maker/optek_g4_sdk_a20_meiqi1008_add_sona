#ifndef __OPTEK_CEC_CMD_H__
#define __OPTEK_CEC_CMD_H__

#define HDMI_ARC_CONNECTED_TIMEOUT_TIME		3000

typedef enum {
	enCEC_AUDIO_MODE_OFF,
	enCEC_AUDIO_MODE_ON,
} CEC_AUDIO_MODE;

typedef enum {
	enCEC_BROADCAST_ADDRESS,
	enCEC_DIRECT_ADDRESS
} CEC_ADDRESS_SEL;


extern U8 cec_direct_addr;
extern U8 cec_tx_buf_len;
extern U8 cec_tx_request;
extern U8 cec_tx_err_cnt;

extern FIFO cec_rx_fifo; 
extern U8 cec_physical_addr[2];


void cec_cmd_rx_handle (U32 data_len);
void cec_cmd_rx_err (U32 len);
void cec_cmd_tx_err (U32 opecode);

U8 cec_report_physical_address (void);
U8 cec_set_osd_name(U8 *pBuf, U8 header);

void cec_report_device_power_status(void);

void cec_report_power_on_to_power_standby(void);
void cec_report_power_standby_to_power_on(void);


#endif //__OPTEK_CEC_CMD_H__

