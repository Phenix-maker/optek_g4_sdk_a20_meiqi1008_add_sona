#ifndef	__HOST_STACK_H__
#define	__HOST_STACK_H__


//to keep compatible with old code
/*
	pipe
*/

#define PIPE_ISOCHRONOUS		0
#define PIPE_INTERRUPT			1
#define PIPE_CONTROL			2
#define PIPE_BULK				3

#define	USB_DIR_IN				0x01
#define	USB_DIR_OUT				0x00

#define usb_maxpacket(dev, pipe, out)	(out ? \
				  (dev)->epMaxOutPacket[usb_pipeendpoint(pipe)] \
				: (dev)->epMaxInPacket [usb_pipeendpoint(pipe)] )

#define usb_pipein(pipe)		((pipe) & USB_DIR_IN)
#define usb_pipeout(pipe)		(!usb_pipein(pipe))
#define usb_pipeendpoint(pipe)	(((pipe) >> 1) & 0xf)

#define usb_pipetype(pipe)		(((pipe) >> 5) & 3)
#define usb_pipeisoc(pipe)		(usb_pipetype((pipe)) == PIPE_ISOCHRONOUS)
#define usb_pipeint(pipe)		(usb_pipetype((pipe)) == PIPE_INTERRUPT)
#define usb_pipecontrol(pipe)	(usb_pipetype((pipe)) == PIPE_CONTROL)
#define usb_pipebulk(pipe)		(usb_pipetype((pipe)) == PIPE_BULK)

#define usb_gettoggle(dev, ep, out)			(((dev)->epToggle[out] >> (ep)) & 1)
#define	usb_dotoggle(dev, ep, out)			((dev)->epToggle[out] ^= (1 << (ep)))
#define usb_settoggle(dev, ep, out, bit)	((dev)->epToggle[out] = ((dev)->epToggle[out] & ~(1 << (ep))) | ((bit) << (ep)))


#define usb_sndctrlpipe(endpoint)		((PIPE_CONTROL << 5) | (endpoint << 1))
#define usb_rcvctrlpipe(endpoint)		((PIPE_CONTROL << 5) | (endpoint << 1) | USB_DIR_IN)
#define usb_sndbulkpipe(endpoint)		((PIPE_BULK << 5) | (endpoint << 1))
#define usb_rcvbulkpipe(endpoint)		((PIPE_BULK << 5) | (endpoint << 1) | USB_DIR_IN)
#define usb_sndintpipe(endpoint)		((PIPE_INTERRUPT << 5) | (endpoint << 1))
#define usb_rcvintpipe(endpoint)		((PIPE_INTERRUPT << 5) | (endpoint << 1) | USB_DIR_IN)




typedef	enum {
	enPIPE_CTRL,
	enPIPE_BULK,
	enPIPE_INT,
	enPIPE_ISO
}	enum_PIPE_TYPE;

typedef	enum {
	enHPIPE_CTRL,
	enHPIPE_BULK,
	enHPIPE_INT,
	enHPIPE_ISO
}	enum_HPIPE_TYPE;

struct _USB_DEVICE;
typedef	struct _PIPE {
	U8	type;
	U8	endpnum;
	U8	dirin;

	U16	maxpkt;
	U8	toggle;
	U8	multi;
	U32	interval;

	struct _USB_DEVICE	*usbdev;
	U8	active;
}	PIPE;

//for compatible issue
typedef enum {
	enURBRST_STALL,
	enURBRST_FAIL,
	enURBRST_SUCESS,
	enURBRST_TIMEOUT,
	enURBRST_INT_NAK,
	enURBRST_DEV_DISCONNECT,

	enURBRST_SUCCESS_UNDERRUN,
	enURBRST_TERMINATED
}	enum_URB_RESULT;

/*
	URB
*/


int hstk_init( void );
int hstk_open( void );
int hstk_close( void );


int hstk_trans( PIPE *pipe, U8 dataIn, U8 *setuppkt, U8 *buf, U32 *len, U8 zeroPkt );


#ifndef	__HTD_CALL_BACK__
#define	__HTD_CALL_BACK__
struct _HTD;
typedef	int (*PHTD_CALLBACK) ( struct _HTD *htd ); 
#endif

int hstk_transExt( PIPE *pipe, U8 dataIn, U8 *setuppkt, U8 *buf, U32 *len, U8 zeroPkt, PHTD_CALLBACK callback, int waitEvt );

PIPE *hstk_getCtlPipe(void);


#endif //__HOST_STACK_H__
