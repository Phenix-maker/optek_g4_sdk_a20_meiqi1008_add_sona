#ifndef _AUDIO_CODEC_H_
#define _AUDIO_CODEC_H_


#define AUDIO_DEC_OUT_2_CH    2
#define AUDIO_DEC_OUT_3_CH    3
#define AUDIO_DEC_OUT_4_CH    4
#define AUDIO_DEC_OUT_5_CH    5
#define AUDIO_DEC_OUT_6_CH    6
#define AUDIO_DEC_OUT_8_CH    8


typedef enum {
	DECODE_NONE = 0,
	DECODE_MP3,
	DECODE_WMA,
	DECODE_WAV,
	DECODE_SL_SLAVE,

	DECODE_SBC,
	DECODE_PCM_DOWNSAMPLE,
	DECODE_HFP,
	DECODE_PCM,
	DECODE_OPTICAL,
	DECODE_COAXIAL,

	DECODE_ARC,
	DECODE_DD_AC3,
	DECODE_DDPLUS71,
	DECODE_USB_SPEAKER,
	DECODE_OPL_SLAVE,
	DECODE_DTS_DMP,

	DECODE_LC3,
	DECODE_MSBC,
	DECODE_INTERNAL,
	
	DECODE_UNKNOW = 0xffff
} enumDECODE_ID;

typedef enum {
	/****err code, Analyzed by each codec****/
	DECODE_ERR1,
	DECODE_ERR2,
	DECODE_ERR3,
	DECODE_ERR4,
	DECODE_ERR5,
	DECODE_ERR6,
	DECODE_ERR7,
	DECODE_ERR8,


	DECODE_SUCCESS = 0xFD,//a frame decode sucess
	DECODE_END = 0xFE,   //decode end
	DECODE_USER_EXIT = 0xFF,	
} enumDECODE_FRAME_RETs;

typedef struct {
	/*
	=0 means speech equal to local,<0 means slow than local,>0 means fast than local,
	the bigger the absolute value, the faster or slower it is
	*/
	int src_speed;
} DECODE_INFO_STRU;

typedef struct {
    enumDECODE_ID decode_id;
	char *dec_name;
	void (*init) (void);
	void (*fini) (int reason);//enumDECODE_FRAME_RETs
	enumDECODE_FRAME_RETs (*decode_frame) (U8 **out,U16 *plen);
	int (*decode_info_get) (DECODE_INFO_STRU *info);
} DECODE_DEVICE_STRU;


#endif//_AUDIO_CODEC_H_
