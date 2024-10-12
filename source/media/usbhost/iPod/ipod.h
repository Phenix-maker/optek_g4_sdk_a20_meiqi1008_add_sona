#ifndef	__IPOD_H__
#define	__IPOD_H__


#define	IPOD_MEM_SIZE_DOWN

#define	IPOD_WAIT_TIME_OUT	(3000 / OS_MSEC_PER_TICK + 1)	//3s

//config ipod feature

//dock port only, disable usb port ipod function
//#define IPOD_DISABLE_USB_PORT
//#define IPOD_USB_AUDIO_CHARGE_ONLY

//FIXME: debug
#define	ENABLE_DAUDIO

//#define ANALOG_LINE_OUT

//#define IPOD_VIDEO_OUT
//#define IPOD_LINE_IN
//#define IPOD_NO_CHARGE

//#define IPOD_ACC_POWER_WAIT

//#define APPS_SUPPORT

//#define IPOD_VOLUME_SET
//#define IPOD_VOLUME_SYNC
//#define IPOD_VOL_NO_LINE_OUT

//#define IAP_TIME_OUT

#ifdef	APPS_SUPPORT
//#undef ENABLE_DAUDIO
#undef	IPOD_VIDEO_OUT
#undef	IPOD_LINE_IN

//#define APPS_BEAUTIFUL
#define APPS_OPTEK
//#define APPS_HARBOUR_LIGHT
//#define APPS_STRING_BEAUTIFUL

#define	APPS_STRING_PHILIPS
//#define APPS_STRING_HARBOUR_LIGHT

//#define APPS_AUTO_LAUNCH
//#define APPS_UPGRADE_ENABLE

#endif

//#define	JP_SR80I



typedef	enum {
	enIPOD_CONNECT,
	enIPOD_DISCONNECT,
	enIPOD_IPHONE_SEND_PKT
}	enum_IPOD_MSG;

typedef	struct {
	int	msg;
	U32	param1;
	U32	param2;
}	IPOD_MSG;

typedef struct {
	U8	major;
	U8	minor;
	U8	revision;
}	IPOD_SOFT_VER;

typedef	struct {
	U8	support;
	U8	lingo;
	U8	major;
	U8	minor;
}	LINGO_VER;
#define	MAX_LINGO_VER		16

#define	MAX_IPOD_MODEL_NUM	64
#define	MAX_IPOD_NAME		64
#define	MAX_IPOD_SERIAL_NUM	64

#define LINGO_OPTION_SIZE	8

typedef	struct {
	int	type;
	int	initOK;
	int	trkInfoGot;

	U8 appsAutoLaunch;

#ifdef APPS_SUPPORT
	//communication with apps
	U8	appsComm;
#endif

	IPOD_SOFT_VER	vSoft;
	
	U16	name[MAX_IPOD_NAME];
	U16	serialnum[MAX_IPOD_SERIAL_NUM];

	U32	modelid;
	U16	modelNum[MAX_IPOD_MODEL_NUM];
	int modelOk;

	LINGO_VER	vlingo[MAX_LINGO_VER];
	int vlingOk;

	U8 maxPlayLoadSupport;
	U16 maxPayloadSize;

	LINGO_VER	vlingoDaudio;

	U8	lgen_option[LINGO_OPTION_SIZE];
	U8	lsrmt_option[LINGO_OPTION_SIZE];
	U8	ldsp_option[LINGO_OPTION_SIZE];
	U8	lda_option[LINGO_OPTION_SIZE];

	int idps;
	int rxInit;
}	IPOD_DEV_INFO;



typedef	enum {
	enIPOD_IPHONE,
	enIPOD_IPAD,
	enIPOD_OTHERS
}	enum_IPOD_MODEL;
int ipod_getModelTypes( void );


U32	endianConvertU32 ( U32 b );
U16	endianConvertU16 ( U16 b );
U32	byteToU32b( U8 *ptr );
U8 *U32ToPtrb( U32 d, U8 *ptr );
U16 byteToU16b( U8 *ptr );

int ipod_audioRxCtl( int ena );
int ipod_isAutoLaunchSupport( void );

U8 ipodDigital_converted_to_ipodUart(void);

U32 ipod_getDevid( void );

#endif //__IPOD_H__
