#ifndef	__DAUDIO_H__
#define __DAUDIO_H__


#define	FU_MASTER_CHAN_NUM		0
#define	FU_LEFT_FRONT_CHAN_NUM	1
#define	FU_RIGHT_FRONT_CHAN_NUM	2
#define	PC_VOL_VAL_MINUM		0x8000

#define	VOL_RESOLUTION		256		//1dB

typedef struct {
	I16	cur;
	I16	min;
	I16	max;
	U16	res;
}	VOLUME_SETTING;

const char *optek_lib_usb_device_date_get(void);
const char *optek_lib_usb_device_time_get(void);

//USB Lib optek_lib_usb_device.a release version
const char *optek_lib_usb_device_version_get(void);
	
extern VOLUME_SETTING volSetting;

#ifdef	VOLUME_MASTER_CHAN_ONLY
int daudio_getVol( VOLUME_SETTING **vol );
int daudio_setCurVol( I16 vol );
#else
int daudio_getVol( int ch, VOLUME_SETTING **vol );
int daudio_setCurVol( int ch, U16 vol );
#endif
int daudio_getChan(void);
void daudio_reportChan(U8 channel_cfg);

int daudio_setASintf( int intf, int altintf );
int daudio_getASintf( void );

int daudio_isMute( void );
int daudio_muteSet( int mute );


int daudio_submitIsoOut( void );
int daudio_removeIsoOut( void );

int daudio_submitIsoIn( void );
int daudio_removeIsoIn( void );

int daudio_init( void);
int daudio_open(void);
int daudio_close(void);

#endif //__DAUDIO_H__
