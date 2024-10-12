#ifndef _SONA_AEF_H
#define _SONA_AEF_H

#ifdef __cplusplus
extern "C" {
#endif

#define AEF_LEN 64 // frame len, if need others, plz contact us

#define AEF_IN_CH 4 // 0/1:BT-L/R, 2/3:LINE-L/R 
#define AEF_OUT_CH 3 // 0/1 out-A/B, 2: woofer



/**************************************************
   sona audio effect APIs

**************************************************/
typedef struct  
{
	int srate;		
	int insrc_type; // 0-BT, 1-LINE
	int high_pass_mode; //0-high_pass disable, 1-high pass enable;
	int phase;			//0-normal,1-invert;
	int stereoOrMono;  //0-stereo, 1-momo;
	float woofer_gain_dB; 
	float woofer_volume_dB; //-6 ~ 6dB
}tSonaConfig;

/*
	return: 0:ok, -1:failed
*/
int sona_aef_init(tSonaConfig * info, const char*flowStr, const char*paramStr);
int sona_aef_update(tSonaConfig * info);
/*
	in data channel sequence is BT-L/BT-R/LINE_:/LINE_R
	out data channel sequence is OUT-A/OUT-B/woofer
*/
void sona_aef_proc(int audioDataIn[AEF_IN_CH][AEF_LEN], int audioDataOut[AEF_OUT_CH][AEF_LEN]);
//clear internal data status
void sona_aef_clear(void);
//destroy resource
void sona_aef_free(void);

/*
	return woofer level by dB, used for overload led indicator
*/
float sona_aef_get_woofer_level(void);

/**************************************************
   sona audio effect tuning tool APIs

**************************************************/
typedef struct
{
	int type; // 0-uart/hid ... 1-tcp/ip
	void (*comm_write)(char*data, int len); //write tuning data to PC-tool, when type=0
}tTuningConfig;
void sona_aef_tuning_open(tTuningConfig*config);
/*
	feed data to sonalib from uart/hid
*/
void sona_aef_feed_recvData(char*data, int len);
void sona_aef_tuning_close(void);


#ifdef __cplusplus
}
#endif

#endif


