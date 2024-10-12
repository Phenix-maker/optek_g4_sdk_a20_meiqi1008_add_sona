#ifndef __SPDIF_PROCESS_H__
#define __SPDIF_PROCESS_H__


enum {
	STREAM_SPDIF_SR_DETECT_IDLE,
	STREAM_SPDIF_SR_DETECT,
	STREAM_SPDIF_SR_REDETECT,
	STREAM_SPDIF_SR_DETECT_32k,
	STREAM_SPDIF_SR_REDETECT_32k,
	STREAM_SPDIF_SR_DETECT_FINISH,
};

typedef struct
{
    U16 data_type;
    U16 detect_status;
 
} SPDIF_IN_STREAM_STRUC;

extern SPDIF_IN_STREAM_STRUC spdif_in_stream;

extern U8 spdif_rx_down_sample;

void spdif_in_detect_process_init(void);
int spdif_in_stream_data_type_get(void);
void spdif_detect_process (U32 spdif_parm);

void spdif_in_stream_rx_status_set(void);
void spdif_in_stream_rx_status_clr(void);
int spdif_in_stream_rx_status_get(void);

#endif //__SPDIF_PROCESS_H__
