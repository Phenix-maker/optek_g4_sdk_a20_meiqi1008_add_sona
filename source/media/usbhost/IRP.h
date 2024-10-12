#ifndef	_IRP_H_
#define	_IRP_H_



typedef enum {
	enIRPT_HUB,
	enIRPT_MSC_CLASS,
	enIRPT_USBD,
	enIRPT_MTP,
	enIRPT_IPOD,
	enIRP_USB_OPEN,
	enIRP_USB_CLOSE,
	enIRPT_DEV_STACK,
	enIRPT_AOA_STACK,

	enIRPT_HOSTAUDIO
}	enum_IRP_TARGET;

//ensure same value as enum_MSC_RESULT in usbmsc.h
typedef enum {
	enIRPR_SUBMIT,
	enIRPR_SUCCESS,
	enIRPR_FAIL,
	enIRPR_CANCLE,
	enIRPR_DEV_DISCONNECT
}	enum_IRP_RESULT;

typedef enum {
	enIRP_OPEN,
	enIRP_CLOSE,	
	enIRP_GET_DEVINFO,
	enIRP_READ,
	enIRP_WRITE,
	enIRP_IDLE_TIME_OUT,
	enIRP_LUN_POLL_TIME_OUT,
	enIRP_LUN_OPEN,
	enIRP_LUN_CLOSE,

	enIRP_USBHOST_STOP,
	enIRP_USBHOST_START,

	enIRP_HUB_POLL,
	enIRP_HUB_NOTIFY,

	enIRP_OTG_NOTIFY, //0d

	enIRP_TYPE_MTP_INT_POLL,
	enIRP_TYPE_MTP_IF,
	enIRP_TYPE_MTP_SPECIFIC, //10

	enIRP_TYPE_IPOD_NOTIFY,
	enIRP_TYPE_IPOD_PKT_TX,
	enIRP_TYPE_IPOD_PKT_RX,
	enIRP_TYPE_IPOD_LINGO_RX_REQ,
	enIRP_TYPE_IPOD_AUDIO_RX_REQ,
	enIRP_TYPE_IPOD_SET_SAMPLE_RATE,
	enIRP_TYPE_IPOD_SET_INTF,
	enIRP_TYPE_IPOD_BACK2MSC,
	enIRP_TYPE_IPOD_TIMEOUT,
	
	enIRP_TYPE_DEV_START,       //1A
	enIRP_TYPE_DEV_STOP,        //1B
	enIRP_TYPE_DEV_SETUP_PKT,   //1C

	enIRP_TYPE_AOA_SET_ZERO_BANDWIDTH,
	enIRP_TYPE_AOA_START_RX,
	enIRP_TYPE_AOA_HID_KEY,
	enIRP_TYPE_AOA_HID_KEY_DOWN_UP,  //0x20

	//for Host Audio
	enIRP_TYPE_HOSTAUDIO_RX,
	enIRP_TYPE_HOSTAUDIO_TX
}	enum_IRP_TYPE;

#define	IRP_PARAM_DEV_CONNECT		(0x01 << 0)
#define	IRP_PARAM_DEV_DISCONNECT	(0x01 << 1)	
#define	IRP_PARAM_DEV_OVERCURRENT	(0x01 << 2)	

#define	IRP_PARAM_HOST_CONNECT		(0x01 << 0)
#define	IRP_PARAM_HOST_DISCONNECT	(0x01 << 1)	

#define	IRP_PARAM_LOW_SPEED_DEV		(0x01 << 16)
#define	IRP_PARAM_OVERCURRENT_DET	(0x01 << 17)

typedef void (*PIRP_COMPLETE) ( U8 irpResult, U32 iprTransLen);

typedef	struct {
	U8	result;
	U8	target;
	U8	type;	
	U8	targetDevice;
	U32	param;
	
	void *buffer;
	U32	totalLen;	//bytes
	U32	actualLen;
	U32	lba;
	U16	sectors;

	PIRP_COMPLETE complete;
}	IRP;

#define	FILL_IPHONE_IRP( PIRP, TARGET, TYPE, PPKT, PARAM, COMPLETE )	\
	do {	\
			memset( PIRP, 0, sizeof(IRP) );					\
			(PIRP)->target			=	TARGET;				\
			(PIRP)->type			=	TYPE;				\
			(PIRP)->buffer			=	PPKT;				\
			(PIRP)->param			=	PARAM;				\
			(PIRP)->complete		=	COMPLETE;			\
	}	while(0)


#define	FILL_MTP_IRP( PIRP, TARGET, BUF, COMPLETE, TYPE )	\
	do {	\
			memset( PIRP, 0, sizeof(IRP) );					\
			(PIRP)->target			=	TARGET;				\
			(PIRP)->buffer			=	BUF;				\
			(PIRP)->complete		=	COMPLETE;			\
			(PIRP)->type			=	TYPE;				\
	}	while(0)

#define	FILL_NORMAL_IRP( PIRP, TARGET, TYPE, TARGET_DEV, PARAM, BUF, TOTAL_LEN, RESULT, COMPLETE ) \
		do {	\
			(PIRP)->target			=	TARGET;				\
			(PIRP)->type			=	TYPE;				\
			(PIRP)->targetDevice	=	TARGET_DEV;			\
			(PIRP)->param			=	PARAM;				\
			(PIRP)->buffer			=	BUF;				\
			(PIRP)->totalLen		=	TOTAL_LEN;			\
			(PIRP)->actualLen		=	0;					\
			(PIRP)->lba				=	0;					\
			(PIRP)->sectors			=	0;					\
			(PIRP)->result			=	RESULT;				\
			(PIRP)->complete		=	COMPLETE;			\
		} while(0)

#define	FILL_DATA_IRP( PIRP, TARGET, TYPE, TARGET_DEV, LUN, BUF, LBA, SECTOR_CNT, RESULT, COMPLETE )	\
		do {	\
			(PIRP)->target			=	TARGET;				\
			(PIRP)->type			=	TYPE;				\
			(PIRP)->targetDevice	=	TARGET_DEV;			\
			(PIRP)->param			=	LUN;				\
			(PIRP)->buffer			=	BUF;				\
			(PIRP)->totalLen		=	0;					\
			(PIRP)->actualLen		=	0;					\
			(PIRP)->lba				=	LBA;				\
			(PIRP)->sectors			=	SECTOR_CNT;			\
			(PIRP)->result			=	RESULT;				\
			(PIRP)->complete		=	COMPLETE;			\
		} while(0)


// ugly but simple
#define	IRP_HUB_PARAM_IRP_TYPE( irp )	( (irp)->lba )
#define	IRP_HUB_PARAM_USB_HUB( irp )	( (irp)->buffer )
#define	IRP_HUB_PARAM_PORT( irp )		( (irp)->targetDevice )
#define	IRP_HUB_PARAM_HUB_STAT( irp )	( (irp)->totalLen )
#define	IRP_HUB_PARAM_PORT_STAT( irp )	( (irp)->actualLen )

typedef enum {
	enIRP_HUB_HUB,
	enIRP_HUB_PORT
}	enum_IRP_HUB_TYPE;

#define	FILL_HUB2USBD_IRP( PIRP, TARGET, TYPE, PORT, PARAM, USB_HUB, HUB_STAT, PORT_STAT, IRP_TYPE ) \
		do {	\
			(PIRP)->target			=	TARGET;				\
			(PIRP)->type			=	TYPE;				\
			(PIRP)->targetDevice	=	PORT;				\
			(PIRP)->param			=	PARAM;				\
			(PIRP)->buffer			=	USB_HUB;			\
			(PIRP)->totalLen		=	HUB_STAT;			\
			(PIRP)->actualLen		=	PORT_STAT;			\
			(PIRP)->lba				=	IRP_TYPE;			\
			(PIRP)->sectors			=	0;					\
			(PIRP)->result			=	0;					\
			(PIRP)->complete		=	NULL;				\
		} while(0)




#define	FILL_OTG_IRP( PIRP, TARGET, PARAM ) \
		do {	\
			(PIRP)->target			=	TARGET;				\
			(PIRP)->type			=	enIRP_OTG_NOTIFY;	\
			(PIRP)->targetDevice	=	0;					\
			(PIRP)->param			=	PARAM;				\
			(PIRP)->buffer			=	NULL;				\
			(PIRP)->totalLen		=	0;					\
			(PIRP)->actualLen		=	0;					\
			(PIRP)->lba				=	0;					\
			(PIRP)->sectors			=	0;					\
			(PIRP)->result			=	0;					\
			(PIRP)->complete		=	NULL;				\
		} while(0)

#define	FILL_IPOD_INTERNAL_IRP( PIRP, TYPE, PARAM1, PARAM2 ) \
		do {	\
			(PIRP)->target			=	enIRPT_IPOD;		\
			(PIRP)->type			=	TYPE;				\
			(PIRP)->targetDevice	=	0;					\
			(PIRP)->param			=	PARAM1;				\
			(PIRP)->buffer			=	NULL;				\
			(PIRP)->totalLen		=	PARAM2;				\
			(PIRP)->actualLen		=	0;					\
			(PIRP)->lba				=	0;					\
			(PIRP)->sectors			=	0;					\
			(PIRP)->result			=	0;					\
			(PIRP)->complete		=	NULL;				\
		} while(0)

#endif //_IRP_H_
