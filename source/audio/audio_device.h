#ifndef _AUDIO_DEVICE_H_
#define _AUDIO_DEVICE_H_

#include "window.h"

typedef enum {
	OUTPUT_CH_0_INDEX = 0,
	OUTPUT_CH_1_INDEX = 1,
	OUTPUT_CH_2_INDEX = 2,
	OUTPUT_CH_3_INDEX = 3,
	OUTPUT_CH_4_INDEX = 4,
	OUTPUT_CH_5_INDEX = 5,
	OUTPUT_CH_6_INDEX = 6,
	OUTPUT_CH_7_INDEX = 7,
	OUTPUT_CH_8_INDEX = 8,
	OUTPUT_CH_9_INDEX = 9,
	OUTPUT_CH_10_INDEX = 10,
	OUTPUT_CH_11_INDEX = 11,
	OUTPUT_CH_12_INDEX = 12,
	OUTPUT_CH_13_INDEX = 13,
	OUTPUT_CH_14_INDEX = 14,
	OUTPUT_CH_15_INDEX = 15

} enumOUTPUTCH_INDEX;

/*
* CEN			Center												中央 
* SUB			Subwoofer											低音炮 
* L				Stereo Left											立体声左 
* R				Stereo Right										立体声右 
* SL			Surround Left										环绕左 
* SR			Surround Right										环绕右 
* SBL			Surround Back Left									后环绕左 
* SBR			Surround Back Right									后环绕右 
* FLH			Front Left Height									前置左高 
* FRH			Front Right Height									前置右高 
* SLH			Surround Left Height								环绕左高 
* SRH			Surround Right Height								环绕右高 
* RSBLH			Rear Surround Left Height							后环绕左高 
* RSBRH			Rear Surround Right Height							后环绕右高 
* TFL			Top Front Left										顶部前左 
* TFC			Top Front Center									顶部前中 
* TFR			Top Front Right										顶部前右 
* TML			Top Middle Left										顶部中左 
* TMC			Top Middle Center									顶部中中 
* TMR			Top Middle Right									顶部中右 
* TRL			Top Rear Left										顶部后左 
* TRC			Top Rear Center										顶部后中 
* TRR			Top Rear Right										顶部后右 
* BFL			Bottom Front Left									底部前左 
* BFC			Bottom Front Center									底部前中 
* BFR			Bottom Front Right									底部前右 
* BML			Bottom Middle Left									底部中左 
* BMC			Bottom Middle Center								底部中中 
* BMR			Bottom Middle Right									底部中右 
* BRL			Bottom Rear Left									底部后左 
* BRC			Bottom Rear Center									底部后中 
* BRR			Bottom Rear Right									底部后右 
* LW			Left Wide											左宽 
* RW			Right Wide											右宽 
* CW			Center Wide											中宽 
* LSW			Left Surround Wide									左环绕宽 
* RSW			Right Surround Wide									右环绕宽 
* RSL			Rear Surround Left									后环绕左 
* RSR			Rear Surround Right									后环绕右 
* RC			Rear Center											后中 
* RHFL			Rear Height Left									后高前左 
* RHFR			Rear Height Right									后高前右 
* RHFC			Rear Height Center									后高前中 
* RHBFL			Rear Height Surround Left							后高环绕前左 
* RHBFR			Rear Height Surround Right							后高环绕前右 
* RHBFC			Rear Height Surround Center							后高环绕前中 
* RFLV 			Front Left Vertical Height							前垂直高左 
* RFRV 			Front Right Vertical Height							前垂直高右 
* RSLV 			Surround Left Vertical Height						环绕垂直高左 
* RSRV 			Surround Right Vertical Height						环绕垂直高右 
* RTFLV			Top Front Left Vertical Height						顶部前垂直高左 
* RTFRV			Top Front Right Vertical Height						顶部前垂直高右 
* RTRLV			Top Rear Left Vertical Height						顶部后垂直高左 
* RTRRV			Top Rear Right Vertical Height						顶部后垂直高右 
* CENH 			Center Height										中央高 
* CENVH 		Center Vertical Height								中央垂直高 
* CWH 			Center Wide Height									中宽高 
* CENVWH 		Center Wide Vertical Height							中宽垂直高 
* */

#define	SOUND_CH_INDEX_L 			 	OUTPUT_CH_0_INDEX 
#define	SOUND_CH_INDEX_R 			 	OUTPUT_CH_1_INDEX 
#define	SOUND_CH_INDEX_C 			 	OUTPUT_CH_2_INDEX 
#define	SOUND_CH_INDEX_SUB 			 	OUTPUT_CH_3_INDEX 
#define	SOUND_CH_INDEX_SL 			 	OUTPUT_CH_4_INDEX 
#define	SOUND_CH_INDEX_SR 			 	OUTPUT_CH_5_INDEX 
#define	SOUND_CH_INDEX_RSL 			 	OUTPUT_CH_6_INDEX 
#define	SOUND_CH_INDEX_RSR 			 	OUTPUT_CH_7_INDEX 
/*
* #define SOUND_CH_INDEX_TSL 			OUTPUT_CH_8_INDEX 
* #define SOUND_CH_INDEX_TSR 			OUTPUT_CH_9_INDEX 
* #define SOUND_CH_INDEX_TSC 			OUTPUT_CH_10_INDEX
* #define SOUND_CH_INDEX_BSL 			OUTPUT_CH_11_INDEX
* #define SOUND_CH_INDEX_BSR 			OUTPUT_CH_12_INDEX
* #define SOUND_CH_INDEX_BSC 			OUTPUT_CH_13_INDEX
* #define SOUND_CH_INDEX_LW 			OUTPUT_CH_14_INDEX
* #define SOUND_CH_INDEX_RW 			OUTPUT_CH_15_INDEX
* */
#define	SOUND_CH_INDEX_REV_8 			OUTPUT_CH_8_INDEX 
#define	SOUND_CH_INDEX_REV_9 			OUTPUT_CH_9_INDEX 
#define	SOUND_CH_INDEX_REV_10 			OUTPUT_CH_10_INDEX
#define	SOUND_CH_INDEX_REV_11 			OUTPUT_CH_11_INDEX
#define	SOUND_CH_INDEX_REV_12 			OUTPUT_CH_12_INDEX
#define	SOUND_CH_INDEX_REV_13 			OUTPUT_CH_13_INDEX
#define	SOUND_CH_INDEX_REV_14			OUTPUT_CH_14_INDEX
#define	SOUND_CH_INDEX_REV_15			OUTPUT_CH_15_INDEX


typedef struct {
	U8 OutputCh_en;
	U8 OutputCh_index;

} OUTPUT_CH_STATUS_SET;


typedef struct{
	U8 volume;
	U8 mute;
	U8 bass;
	U8 treble;
	U8 eq_mode;
	U8 batt_vol;
	U8 batt_vol1;
	#ifdef DSP_SONA_MODULE_ENABLE
	int srate;	 
	// int insrc_type;
	int high_pass_mode; 
	int phase;			
	int stereoOrMono;  
	float woofer_gain_dB; 
	float woofer_volume_dB;
	#endif

	//OUTPUT_CH_STATUS_SET OutputStatusSetCH[AUDIO_OUT_STREAM_CHANNEL];

} AUDIO_VARI_STRU;
extern AUDIO_VARI_STRU audio_vari;

#define VOLUME_MAX		31
#define VOLUME_MIN		0

#define VOLUME_DEFAULT	15


/***************eq layout**********************				
total eq: 0~9
eq mode:0~7
bass:8
treble:9
*********************************************/
#define EQ_MODE_INDEX 				0
#define EQ_NUM 						8

#define BASS_INDEX					8
#define TREBLE_INDEX				9

#define BASS_MAX                    9
#define BASS_MIN                    0
#define BASS_MIDDLE                 5
#define BASS_STEP                   1
#define BASS_DEFAULT                5

#define TREBLE_MAX                  9
#define TREBLE_MIN                  0
#define TREBLE_MIDDLE               5
#define TREBLE_STEP                 1
#define TREBLE_DEFAULT              5

//Equalizer
typedef enum {
	EQ_NORMAL = 0,
	EQ_OFF = 0,
	EQ_FLAT = 0,
	EQ_CLASSIC,
	EQ_ROCK,
	EQ_JAZZ,
	EQ_POP,

	EQ_OPTIMAL,
	EQ_TECHNO,
	EQ_VOCAL,
	EQ_GAME,

	EQ_BASS,
	EQ_BASS_I,
	EQ_BASS_II,

	EQ_MID_TREBLE,
	EQ_HIGH_TREBLE,
	EQ_VOLUME
} enumEQmode_ID;

#define EQ_DEFAULT		EQ_FLAT

typedef struct {
    char *string;
    enumEQmode_ID id;
} AUDIO_EQ_STRU;


typedef struct _ADUIO_DEVICE {
	void		(*init) (void);
	void		(*open) (void);
	void		(*Set_Channel) (U8 channel);
	void		(*Set_Volume) (U8 volume);
	void		(*Set_Bass_Treble) (U8 bass, U8 treble);
	void		(*Set_Eq_Mode) (U8 mode);
	void		(*Set_Mute) (U8 val);
	void		(*Set_AM_Mode) (U8 con, U32 kFeq);
//	void		(*Set_Speaker) (U8 speaker, U8 attenuation);
//	void		(*Set_Surround) (U8 val);
//	void		(*Set_Max) (U8 val);
} AUDIO_DEVICE;

#ifdef AUDIO_PROCESSOR
extern const AUDIO_DEVICE AUDIOdevice;
#endif

#define AMP_DEV_COUNT_MAX	1
extern const AUDIO_DEVICE AudioAmpDev[AMP_DEV_COUNT_MAX];


void Audio_Dev_None_Init(void);
void Audio_Dev_None_Open(void);
void Audio_Dev_None_Set_Channel(U8 channel);
void Audio_Dev_None_Set_Volume(U8 volume);
void Audio_Dev_None_Set_Speaker(U8 speaker, U8 attenuation);
void Audio_Dev_None_Set_Bass_Treble(U8 bass, U8 treble);
void Audio_Dev_None_Set_eq_mode(U8 mode);
void Audio_Dev_None_Set_Mute(U8 con);
void Audio_Dev_None_Set_AM_Mode(U8 con, U32 kFreq);

#define AUDIO_DEV_NONE      	Audio_Dev_None_Init,\
								Audio_Dev_None_Open,\
								Audio_Dev_None_Set_Channel,\
								Audio_Dev_None_Set_Volume,\
								Audio_Dev_None_Set_Bass_Treble,\
								Audio_Dev_None_Set_eq_mode,\
								Audio_Dev_None_Set_Mute,\
								Audio_Dev_None_Set_AM_Mode	//,\
								//	Audio_Dev_None_Set_Speaker

#define AUDIO_DEV_CODEC_MUTE_ON		AUDIOdevice.Set_Mute(TRUE)
#define AUDIO_DEV_CODEC_MUTE_OFF	AUDIOdevice.Set_Mute(FALSE)

#define AUDIO_DEV_AMP_MUTE_ON
#define AUDIO_DEV_AMP_MUTE_OFF


void app_audio_device_volume_set(U8 con);
U32 app_audio_device_volume_plus(KEY_HANDLE_PARAM_STRU param);
U32 app_audio_device_volume_minus(KEY_HANDLE_PARAM_STRU param);
void app_audio_device_bass_set(U8 con);
U32 app_audio_device_bass_plus(KEY_HANDLE_PARAM_STRU param);
U32 app_audio_device_bass_minus(KEY_HANDLE_PARAM_STRU param);
void app_audio_device_treble_set(U8 con);
U32 app_audio_device_treble_plus(KEY_HANDLE_PARAM_STRU param);
U32 app_audio_device_treble_minus(KEY_HANDLE_PARAM_STRU param);
U32 app_audio_device_eq_mode_loop(KEY_HANDLE_PARAM_STRU param);
#endif //_AUDIO_DEVICE_H_

