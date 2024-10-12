/*
 * audio_clock_sync.h
 *
 *  Created on: 2024/3/1
 *      Author: Windows
 */

#ifndef __AUDIO_CLOCK_SYNC_H__
#define __AUDIO_CLOCK_SYNC_H__

typedef enum{
	ASOURCE_UNKNOW,
	ASOURCE_USB_HOST,
	ASOURCE_SPDIF,
	ASOURCE_IIS_MASTER,
	ASOURCE_OPTEK_LINK_MASTER,
	ASOURCE_BT_MASTER,
	ASOURCE_TYPE_END
}ASOURCE_TYPE_e;


typedef enum{
	COUNTING_UNIT_UNKNOW,
	COUNTING_UNIT_MS,
	COUNTING_UNIT_SAMPLES,
	COUNTING_UNIT_END
}COUNTING_UNIT_e;

typedef struct{
	U32 srcCount;//unit is ms in usb audio,samples in iis or spdif
	U32 localStartCount;//unit is machine cycle
	U32 localCurCount_L;//unit is machine cycle
	U32 localCurCount_H;//unit is machine cycle 
	U32 count_bak;//unit is machine cycle,use to determine carry
	ASOURCE_TYPE_e src_type;
	COUNTING_UNIT_e counting_unit;
	U8 restart;//restart command,restart when equal TRUE
}sync_w_src_clks_t;

typedef struct{
	U8 sync_w_src_enable;	
	sync_w_src_clks_t sync_clks;
	U32 sync_process_interval_25ms;
	U32 sync_process_dtime;
}sync_w_src_control_t;

extern sync_w_src_control_t  sync_w_src_control;

typedef struct {
	/*
	=0 means speech equal to local,<0 means slow than local,>0 means fast than local,
	the bigger the absolute value, the faster or slower it is
	*/
	i8 src_speed;
} DECODE_INFO_t;

typedef struct{
	U8 need_sync_flag;

    U32 frame_time_us;

} AUDIO_CLOCK_SYNC_STRU;
extern AUDIO_CLOCK_SYNC_STRU audio_clock_sync_vari;

#define USB_AUDIO_DEC_FRAME_SIZE        USB_PCM_BLOCK_SIZE//(USB_PCM_BLOCK_SIZE/2)*audio_in_stream.channel
//(dec_frame_size/(app_main_data.playing_stream_sample_bits*app_main_data.playing_stream_chans/8))
#define USB_AUDIO_DEC_FRAME_SAMPLES     (USB_PCM_BLOCK_SIZE/4)

#define EX_AUDIO_STREAM_THRESHOLD_FOR_USB_DEV      (USB_AUDIO_DEC_FRAME_SIZE+192*4)

#define START_INTERVAL_MS  			(1*1000)//
#define MAX_INTERVAL_MS    			(30*1000)
#define ADJUSTING_INTERVAL_MS    	(4*1000)

#define MsToOsTimerTick(ms)              (ms/5)

void sync_w_src_init(void);
void sync_w_src_loop(void);
void usbdev_clock_sync_irq(void);

#endif /* __AUDIO_CLOCK_SYNC_H__ */
