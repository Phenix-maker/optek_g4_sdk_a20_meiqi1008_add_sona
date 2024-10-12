#ifndef __ARC_PROCESS_H__
#define __ARC_PROCESS_H__


enum {
	ARC_CEC_DISCONNCETED,
	ARC_CEC_CONNCETED,
};


typedef enum {
	enARC_CEC_PRO_UNKNOWN,
	enARC_CEC_PRO_DISCONNCETED,
	enARC_CEC_PRO_CONNCETED,
	enARC_CEC_PRO_I2C_READ_ERROR,
	enARC_CEC_PRO_I2C_READ_SUCCESS,
	enARC_CEC_PRO_HEADER_FOUND,
	enARC_CEC_PRO_PHYSICAL_ADDRESS_0_FOUND,
	enARC_CEC_PRO_PHYSICAL_ADDRESS_1_FOUND,
} enARC_CEC_PROCESS_STATUS;


extern U8 arc_cec_process_status;

extern U8 arc_cec_connect_status;

void arc_detect_plug(int param);
void arc_detect_unplug(int param);

void cec_i2c_read_and_report_addr(void);


#endif //__ARC_PROCESS_H__

