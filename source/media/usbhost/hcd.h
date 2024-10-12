#ifndef	__HCD_H__
#define	__HCD_H__


#ifdef USB_HOST_AUDIO_ENABLE
#define HCD_V2
#endif


typedef enum {
	enCSTAGE_SETUP,
	enCSTAGE_DATA,
	enCSTAGE_STATUS,
	enCSTAGE_COMPLETE
}	enum_HTD_CONTROL_STAGE;

typedef enum {
	enHTD_SUCCEED,
	enHTD_FAIL,
	enHTD_TIMEOUT,
	enHTD_CANCEL
}	enum_HTD_RESULT;


#ifndef	__HTD_CALL_BACK__
#define	__HTD_CALL_BACK__
typedef	int (*PHTD_CALLBACK) ( struct _HTD *htd ); 
#endif

#define	SETUP_PKT_SIZE	8
typedef	struct _HTD {
	U8	*buf0;		//patch
	U8	*buffer;
	U32	len;
	U32	transLen;
	U8	zeroPkt;

	U8	inPkts;		//used in multiple buffer

	U8	*setupPkt;
	U8	ctlTransStat;
	U8	transResult;
	U8	dataIn;
	U8	shortPkt;

	U8	firstTrans;
	PIPE	*pipe;
	U32	eventMsk;

	PHTD_CALLBACK	callback;
	struct _HTD	*next;
	int active;
	int	complete;
	int	waitEvent;
	int transPerFrame;

	U8	result;
}	HTD;



typedef enum {
	enTRANS_PIO32	=	0,
	enTRANS_PIO64,
	enTRANS_DMA32,
	enTRANS_DMA64,
	enTRANS_MAX
}	enum_TRANS_METHOD;
int hcd_setTransMethod( int trans );

//enum_USB_SPEED in usbd.h
int hcd_getDevSpeed( void );
int hcd_resetPort( void );
int hcd_isDevConnected( void );

int hcd_addHtd( HTD *htd );
int hcd_devUnplug( void );

HTD *hcd_removeHtd( PIPE *pipe );

int hcd_setScheduleMode( int sof );

int hcd_init(void);
int hcd_open(void);
int hcd_close(void);

#endif //__HCD_H__
