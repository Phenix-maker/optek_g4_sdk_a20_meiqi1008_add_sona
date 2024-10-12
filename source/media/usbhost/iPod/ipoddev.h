#ifndef	__IPOD_DEV_H__
#define	__IPOD_DEV_H__


#define	IPOD_VID        0x05ac
#define	IPOD_PID_MASK   0xff00
#define	IPOD_PID        0x1200


//request for ipod power
#define	IPOD_POWER_REQ_TYPE     0x40
#define	IPOD_POWER_REQUEST      0x40

#define	EXTRA_CURRENT_SUSPEND0          0
#define	EXTRA_CURRENT_SUSPEND100        100
#define	EXTRA_CURRENT_SUSPEND500        500

#define	EXTRA_CURRENT_NON_SUSPEND_MIN       (-500)
#define	EXTRA_CURRENT_NON_SUSPEND_MAX       500 //1A charge
#define	EXTRA_CURRENT_NON_SUSPEND_MAX_IPAD  500 //2.1A charge
#define	EXTRA_CURRENT_NON_SUSPEND0          0


typedef	struct {
	PIPE	*ctl;
	PIPE	*hidIntIn;
	PIPE	*isoIn;
	PIPE	*mscBulkIn;
	PIPE	*mscBulkOut;
}	IPOD_PIPES;


struct _USB_DEVICE;
struct _IPOD_DESPT;
typedef	struct _IPOD_DEVICE {

	int	active;

	int type;
	struct _USB_DEVICE	*usbdev;
	struct _IPOD_DESPT	*ipoddespt;

	IPOD_PIPES	pipes;

}	IPOD_DEVICE;



typedef	struct _IPOD_PKT {	
	U8	start;
	int	pktlen;
	int lingoID;
	int	cmdID;
	int	transID;

	U8	*buf;
	int	maxSize;
	int	size;
	int	valid;

	int (*callback) ( struct _IPOD_PKT *ipkt );
}	IPOD_PKT;



IPOD_PKT* ipodev_pktGet( void );
int ipodev_pktWrite( IPOD_PKT *pkt, U8 *buf, int len );
int ipodev_pktParse( IPOD_PKT *pkt );

int ipodev_init( void );
int ipodev_open( int type );
int ipodev_close( void );


IPOD_DEVICE	*ipodev_getDev( void );


#endif //__IPOD_DEV_H__
