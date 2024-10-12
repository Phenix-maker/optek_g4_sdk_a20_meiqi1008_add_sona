#ifndef	__DSTACK_H__
#define	__DSTACK_H__

#include "usbdev.h"
#include "IRP.h"

typedef	enum {
	enDPIPE_CTRL,
	enDPIPE_BULK,
	enDPIPE_INT,
	enDPIPE_ISO
}	enum_DPIPE_TYPE;

struct _DUSB_DEVICE;
typedef struct {
	U8	type;
	U8	endpt;		//DC endpoint number, not endpAddr
	U8	dirIn;      //per USB Host

	int	maxpkt;
	U8	toggle;
	U8	multi;
	int	interval;

	struct _DUSB_DEVICE *usbdev;
	U8	active;
}	DPIPE;



typedef enum {
	enDEV_STATE_UNKOWN,
	enDEV_STATE_ATTACHED,
	enDEV_STATE_POWERED,
	enDEV_STATE_DEFAULT,
	enDEV_STATE_ADDRESS,
	enDEV_STATE_CONFIGED,
	enDEV_STATE_SUSPEND
}	enum_USB_DEV_STATE;

typedef enum {
	enDEV_SPEED_UNKNOWN,
	enDEV_SPEED_LOW,
	enDEV_SPEED_FULL,
	enDEV_SPEED_HIGH
}	enum_USB_DEV_SPEED;


//for HID
#define	MAX_DEV_PIPES		4
//0 is dummy, ep1 Host OUT 	(in INTERFACE 01)
#define	ENDP_NUM_ISO_OUT	1		
//JJ+, ep2 Host IN			(in INTERFACE 02)
#define	ENDP_NUM_ISO_IN	    1		

//JJ+, ep2 ISO IN for FB	(in INTERFACE 01)//#ifdef USBDEV_ISOIN_ASYNC
#define	ENDP_NUM_ISO_INFB	3

//JJ+, ep2 INT IN for HID	(in INTERFACE 03)
#define	ENDP_NUM_INT_IN	    2	
//JJ+, ep2 INT OUT for HID	(in INTERFACE 03)
#define	ENDP_NUM_INT_OUT    2

#define	CTL_ENDP_MAX_PKT	64


//jj
//FS 2x16x48000
//HS 2x16x48000
//HS 6x16x48000
//FS 8x16x48000

#define	DISO_MAX_PKT_SIZE	(48000*2*8/1000)
#define	DISO_MAX_PKT_SIZE6	(48000*2*6/1000)
#define	DISO_MAX_PKT_SIZE2	(48000*2*2/1000)
#define	DISO_MAX_PKT_SIZE8	(48000*2*8/1000)
#define	DISO_MAX_PKT_SIZE4	(48000*2*4/1000)



#define	DISOIN_MAX_PKT_SIZE	(192)


typedef struct _DUSB_DEVICE {
	U8	addr;
	int	state;

	DPIPE *ctlPipe;
	DPIPE *ipipe[MAX_DEV_PIPES];	//ipipe[1]: Mic In	ipipe[2]: HID In
	DPIPE *opipe[MAX_DEV_PIPES];	//opipe[1]:	Spk Out opipe[2]: HID Out
}	DUSB_DEVICE;


struct _DTD;
typedef	int (*PDTD_CALLBACK) ( struct _DTD *dtd ); 

typedef enum {
	enDCTL_SETUP,
	enDCTL_DATA,
	enDCTL_STATUS,
	enDCTL_COMPLETE
}	enum_DEV_CTL_STAGE;

typedef struct _DTD {
	U8	*buffer;	//original
	int	len;
	U8	*transbuf;	//increase, working data buffer
	int	translen;	//transfered data len
	U8	zeropkt;

	U8	dirIn;		//ctl only
	int ctlStage;
	DPIPE *pipe;

	U8	shortpkt;
	U8	first;		//for DCD OUT pipe, used to arm endpoint
	
	U8	active;
	U8	waitEvent;

	PDTD_CALLBACK	callback;
	U8	complete;
	int	result;

	DUSB_DEVICE	*usbdev;
	struct _DTD *next;
}	DTD;


typedef enum {
	enDTD_SUCESS,
	enDTD_STALL,
	enDTD_FAIL,
	enDTD_TIMEOUT,
	enDTD_HOST_DISCONNECT,

	enDTD_SUCCESS_UNDERRUN,
	enDTD_TERMINATED
}	enum_DTD_RESULT;

int dstk_dtdPut( DTD *dtd );

DUSB_DEVICE *dstk_getDev( void );

DPIPE *dstk_getCtlPipe( void );
//usb speaker
DPIPE *dstk_getIsoOutPipe( void );
DPIPE *dstk_getIsoInPipe( void );

// endpnum + dir = endpAddr
DPIPE *dstk_getPipes( U8 endpAddr );

int dstk_busReset( void );

int dstk_transfer( DPIPE *pipe, int dirIn, U8 *buf, int *len, PDTD_CALLBACK callback, int waitEvent );
int dstk_IRPprocess( IRP *irp );

#endif //__DSTACK_H__
