#ifndef	_USBD_H_
#define	_USBD_H_

#include "stdDespt.h"
#include "hstack.h"


//#define IPOD_USB_AUDIO_CHARGE_ONLY


#define	GET_CONFIG_1ST_LEN		8
#define	GET_CONFIG_TRY_LOOPS	32

#define	U16_BYTE_SWAP( x16 )	( x16 )

/*
 ****************************************************************************************
 * TYPES DEFINITION
 ****************************************************************************************
 */
/**
 * @struct USB_DEV_REQEUST
 * @brief usb dev request
 */
typedef struct {
	U8	bmRequestType;
	U8	bRequest;
	U16 wValue;
	U16 wIndex;
	U16 wLength;
} __attribute__ ((packed)) USB_DEV_REQEUST;


 //for USB Audio Class 2.0
#define UAC20
extern BOOL UACver2;	// UAC2 version

#ifdef UAC20
//jj+ for UAC2
extern BOOL gIAD;		//IAD descriptor exists


/**
 * @struct USB_IAD_DESCRIPTOR
 * @brief Audio20 4.6 Table 4-3: Standard Interface Association Descriptor jj+
 */
typedef struct {
	U8	bLength;
	U8	bDescriptorType;
	U8	bFirstInterface;
	U8	bInterfaceCount;
	U8	bFunctionClass;
	U8	bFunctionSubClass;
	U8	bFunctionProtocol;
	U8	iFunction;
}	__attribute__  ((packed)) USB_IAD_DESCRIPTOR;

#define	USB_IAD_DESCRIPTOR_LENGTH		8


/**
 * @struct USB_CSD_DESCRIPTOR
 * @brief Audio20 4.7.2.1 Clock Source Descriptor: 
 *			Table 4-6: Clock Source Descriptor jj+
 */
typedef struct {
	U8	bLength;				//8
	U8	bDescriptorType;
	U8	bDescriptorSubtype;
	U8	bClockID;
	U8	bmAttributes;
	U8	bmControls;
	U8	bAssocTerminal;
	U8	iClockSource;
}	__attribute__  ((packed)) USB_CSD_DESCRIPTOR;

#define	USB_CSD_DESCRIPTOR_LENGTH		8


/**
 * @struct USB_CXD_DESCRIPTOR
 * @brief Audio20 4.7.2.2 Clock Selector Descriptor:
 *			Table 4-7: Clock Selector Descriptor jj+
 */
typedef struct {
	U8	bLength;				//variable
	U8	bDescriptorType;
	U8	bDescriptorSubtype;
	U8	bClockID;
	U8	bNrInPins;
	U8	baCSourceID[10];		//p max to 10
	U8	bmControls;
	U8	iClockSelector;
}	__attribute__  ((packed)) USB_CXD_DESCRIPTOR;
#define	USB_CXD_DESCRIPTOR_LENGTH		10


/**
 * @struct USB_CMD_DESCRIPTOR
 * @brief Audio20 4.7.2.3 Clock Multiplier Descriptor:
 *			Table 4-8: Clock Multiplier Descriptor jj+
 */
typedef struct {
	U8	bLength;				//7
	U8	bDescriptorType;
	U8	bDescriptorSubtype;
	U8	bClockID;
	U8	bCSourceID;
	U8	bmControls;
	U8	iClockMultiplier;
}	__attribute__  ((packed)) USB_CMD_DESCRIPTOR;

#define	USB_CMD_DESCRIPTOR_LENGTH		7


/**
 * @struct USB_ENDPOINT_UAC2_DESCRIPTOR
 * @brief USB20 9.5 Table 9-13.
 *			Standard Endpoint Descriptor
 */
typedef struct {     // 
	U8	bLength;	//7
	U8	bDescriptorType;
	U8	bEndpointAddress;
	U8	bmAttributes;
	U16	wMaxPacketSize;
	U8	bInterval;
}	__attribute__  ((packed)) USB_ENDPOINT_UAC2_DESCRIPTOR;

#define	USB_ENDPOINT_UAC2_DESCRIPTOR_LENGTH			7


/**
 * @struct USB_ENDPOINT_UAC2_DESCRIPTOR
 * @brief audio20 4.10.1.2 Class-Specific AS Isochronous Audio Data Endpoint Descriptor
 *			Table 4-34: Class-Specific AS Isochronous Audio Data Endpoint Descriptor
 */
typedef struct {
	U8	bLength;					//8
	U8	bDescriptorType;
	U8	bDescriptorSubtype;
	U8	bmAttributes;
	U8	bmControls;
	U8	bLockDelayUnits;
	U16	wLockDelay;
}	__attribute__  ((packed)) USB_CSENDPOINT_UAC2_DESCRIPTOR;

#define	USB_CSENDPOINT_UAC2_DESCRIPTOR_LENGTH			7


/**
 * @struct USB_ENDPOINT_UAC2_DESCRIPTOR
 * @brief audio20 4.10.2.1 Standard AS Isochronous Feedback Endpoint Descriptor
 *			Table 4-35: Standard AS Isochronous Feedback Endpoint Descriptor
 */
typedef struct {
	U8	bLength;					//7
	U8	bDescriptorType;
	U8	bEndpointAddress;
	U8	bmAttributes;
	U16	wMaxPacketSize;
	U8	bInterval;
}	__attribute__  ((packed)) USB_FBENDPOINT_UAC2_DESCRIPTOR;

#define	USB_FBENDPOINT_UAC2_DESCRIPTOR_LENGTH			7

#endif //UAC20



/**
 * @def FILL_USB_REQUEST
 * @brief 
 */
#define	FILL_USB_REQUEST( PREQ,	REQTYPE,	REQUEST,	VALUE,	INDEX,	LENGTH ) \
	do {	\
		(PREQ)->bmRequestType	=	REQTYPE;	\
		(PREQ)->bRequest		=	REQUEST;	\
		(PREQ)->wValue			=	(VALUE);		\
		(PREQ)->wIndex			=	(INDEX);		\
		(PREQ)->wLength			=	(LENGTH);		\
	} while(0)


/**
 * @def FILL_GET_DESP_USB_REQUEST
 * @brief 
 */
#define	FILL_GET_DESP_USB_REQUEST( PREQ,	REQTYPE,	REQUEST,	DTYPE, DINDEX,	LANGID,	LENGTH ) \
	do {	\
		(PREQ)->bmRequestType	=	REQTYPE;	\
		(PREQ)->bRequest		=	REQUEST;	\
		(PREQ)->wValue			=	(DTYPE << 8) | DINDEX;		\
		(PREQ)->wIndex			=	(LANGID);		\
		(PREQ)->wLength			=	(LENGTH);		\
	} while(0)


/**
 * @def REQUEST_TYPE
 * @brief 
 */
#define	REQUEST_TYPE	\
	(REQUEST_TYPE_DIR_D2H | REQUEST_TYPE_TYPE_STANDARD | REQUEST_TYPE_RECIPIENT_DEVICE)


/**
 * @def REQUEST_TYPE_CLRFEATURE_ENDPSTALL
 * @brief 
 */
#define	REQUEST_TYPE_CLRFEATURE_ENDPSTALL	\
	(REQUEST_TYPE_DIR_H2D | REQUEST_TYPE_TYPE_STANDARD | REQUEST_TYPE_RECIPIENT_ENDPOINT)

#define	USBDEV_STDREQ_CLEAR_FEATURE				(0x0200 | USB_REQUEST_CLEAR_FEATURE)	//3 type
#define	USBDEV_STDREQ_GET_CONFIGURATION			(0x8000 | USB_REQUEST_GET_CONFIGURATION)
#define	USBDEV_STDREQ_GET_DESCRIPTOR			(0x8000 | USB_REQUEST_GET_DESCRIPTOR)
#define	USBDEV_STDREQ_GET_STATUS				(0x8000 | USB_REQUEST_GET_STATUS)		//3 type
#define	USBDEV_STDREQ_SET_ADDRESS				(0x0000 | USB_REQUEST_SET_ADDRESS)
#define	USBDEV_STDREQ_SET_CONFIGURATION			(0x0000 | USB_REQUEST_SET_CONFIGURATION)
#define	USBDEV_STDREQ_SET_FEATURE				(0x0000 | USB_REQUEST_SET_FEATURE)		//3 type
#define	USBDEV_STDREQ_SET_INTERFACE				(0x0100 | USB_REQUEST_SET_INTERFACE)

#define	USBDEV_MSCREQ_GET_MAX_LUN				(0xa100 | USB_REQUEST_GET_MAX_LUN )
#define	USBDEV_MSCREQ_BULK_RESET				(0x2100 | USB_REQUEST_BULK_ONLY_MASS_STORAGE_RESET )

#define	USBDEV_MSREQTYPE_GET_MS_DESPT			0xC0



//jj+ Table 11-15. bmRequestType for USB Hub, Hub Class Requests.
#define USBDEV_STDREQ_GET_STATUS_DEVICE		(0x80)

#define	HUBClassREQTYPE_CLR_HUBFEATURE		(0x20)
#define	HUBClassREQTYPE_CLR_PORTFEATURE		(0x23)

#define	HUBClassREQTYPE_GET_HUBDESCPT		(0xA0)
#define	HUBClassREQTYPE_GET_HUBSTATUS		(0xA0)
#define	HUBClassREQTYPE_GET_PORTSTATUS		(0xA3)

#define	HUBClassREQTYPE_SET_HUBDESCPT		(0x20)
#define	HUBClassREQTYPE_SET_HUBFEATURE		(0x20)
#define	HUBClassREQTYPE_SET_PORTFEATURE		(0x23)

//jj+ Table 9-4. Standard Request Codes
#define DEVREQUEST_GET_STATUS					(0x00)
#define DEVREQUEST_CLEAR_FEATURE				(0x01)
#define DEVREQUEST_SET_FEATURE					(0x03)
#define DEVREQUEST_SET_ADDRESS					(0x05)
#define DEVREQUEST_GET_DESCRIPTOR				(0x06)
#define DEVREQUEST_SET_DESCRIPTOR				(0x07)
#define DEVREQUEST_GET_CONFIGURATION			(0x08)
#define DEVREQUEST_SET_CONFIGURATION			(0x09)
#define DEVREQUEST_GET_INTERFACE				(0x0A)
#define DEVREQUEST_SET_INTERFACE				(0x0B)

//jj+ for Table 11-17. Hub Class Feature Selectors
#define	HUBC_FSEL_C_HUB_LOCAL_POWER 			(0)
#define	HUBC_FSEL_C_HUB_OVER_CURRENT  			(1)
#define	HUBC_FSEL_PORT_CONNECTION  				(0)
#define	HUBC_FSEL_PORT_ENABLE  					(1)
#define	HUBC_FSEL_PORT_SUSPEND  				(2)
#define	HUBC_FSEL_PORT_OVER_CURRENT  			(3)
#define	HUBC_FSEL_PORT_RESET  					(4)
#define	HUBC_FSEL_PORT_POWER  					(8)
#define	HUBC_FSEL_PORT_LOW_SPEED  				(9)
#define	HUBC_FSEL_C_PORT_CONNECTION  			(0x10)
#define	HUBC_FSEL_C_PORT_ENABLE  				(0x11)
#define	HUBC_FSEL_C_PORT_SUSPEND  				(0x12)
#define	HUBC_FSEL_C_PORT_OVER_CURRENT  			(0x13)
#define	HUBC_FSEL_C_PORT_RESET  				(0x14)
#define	HUBC_FSEL_PORT_TEST  					(0x15)
#define	HUBC_FSEL_PORT_INDICATOR  				(0x16)



typedef	struct {
	U16	vid;
	U16	pid;
}	USBDEV_ID;

/*
	BUS / device
*/
#define	USBBUS_MAX_DEVICES			4
#define	USBBUS_DEFAULT_DEV_ADDR		0

#define	USBBUS_MAX_DEV_ADDR			128
#define	USBBUS_ADDR_BUF_SIZE		(USBBUS_MAX_DEV_ADDR/8)

struct _USB_DEVICE;
typedef struct _USB_BUS {
	//struct hcd
	struct _USB_DEVICE	*rootHub;
	struct _USB_DEVICE	*singleDevice;
	
	U8	totalConDevs;
	U32	maxControlFrameLimit;	// 0 is un-limited
	U32	maxBulkFrameLimit;

	U32	bandwidthAlloc;
	U8	addrBuf[USBBUS_ADDR_BUF_SIZE];
}	USB_BUS;

typedef enum {
	enDEV_STAT_DEATTACHED	=	0,
	enDEV_STAT_ATTACHED,
	enDEV_STAT_POWERED,
	enDEV_STAT_DEFAULT,
	enDEV_STAT_ADDRESS,
	enDEV_STAT_CONFIGED,
	enDEV_STAT_SUSPENDED
}	enum_USBDEV_STATE;

typedef enum {
	enSPEED_UNKNOWN,
	enSPEED_LOW,
	enSPEED_FULL,
	enSPEED_HIGH
}	enum_USB_SPEED;

typedef struct {
	USB_CONFIGURATION_DESCRIPTOR	config;
	USB_INTERFACE_DESCRIPTOR		intf;
	USB_ENDPOINT_DESCRIPTOR			endp[2];	
}	__attribute__ ((packed)) USB_DEV_CONFIG;

#define	ENDPOINT_DESCRIPTOR_NUM		0x02
#define	CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH +	\
									USB_INTERFACE_DESCRIPTOR_LENGTH +		\
									USB_ENDPOINT_DESCRIPTOR_LENGTH*ENDPOINT_DESCRIPTOR_NUM )
//	CONFIGURATION_TOTAL_LEN = 32

#define	INTF_MAX_ENDPS				6
typedef	struct {
	USB_INTERFACE_DESCRIPTOR		*intf;
	U8	actEndpIn;
	U8	actEndpOut;
	U8	actEndpIntIn;
	USB_ENDPOINT_DESCRIPTOR			*endps[INTF_MAX_ENDPS];
}	USB_INTERFACE;
#define	CONFIG_MAX_INTFS			6
typedef	struct {
	USB_CONFIGURATION_DESCRIPTOR	*config;	
	U8	activeIntf;
	USB_INTERFACE					intfs[CONFIG_MAX_INTFS];
}	USB_CONFIGURATION;

#define	MAX_CONFIGURATION		6

#define	MAX_STR_DESCRIPTOR		3
#define	MAX_STR_DESP_WORD		30
#define	MAX_STR_DESP_BYTE		(2*MAX_STR_DESP_WORD)

#define	ENDP_ADDR_IN_MASK		0x80


typedef	enum {
	enPIPE_INDX_CTRL	=	0,
	enPIPE_INDX_BULK_OUT,
	enPIPE_INDX_BULK_IN,
	enPIPE_INDX_INT_OUT,
	enPIPE_INDX_INT_IN,
	enPIPE_INDX_ISO_OUT,
	enPIPE_INDX_ISO_IN,
	enPIPE_INDX_MAX			//7
}	enum_PIPE_INDEX;	


#ifdef IPOD_USB_AUDIO
#include "audio.h"

typedef struct {
	USB_INTERFACE_DESCRIPTOR	*intf;
	USB_ENDPOINT_DESCRIPTOR	*bulkIn;
	USB_ENDPOINT_DESCRIPTOR	*bulkOut;
} MSC_INTF;

typedef struct _AUDIO_CTL_INTF {
	USB_INTERFACE_DESCRIPTOR	*intf;
	AUDIO_CTL_INTF_DESPT	*intfClass;
	USB_ENDPOINT_DESCRIPTOR	*intIn;

	U8	*tdud;
	int tdudSize;
	int tdudNums;	//don't parse
} AUDIO_CTL_INTF;

typedef struct _AUDIO_STREAM_INTF {
	USB_INTERFACE_DESCRIPTOR	*intfAlt0;	//zero band
	USB_INTERFACE_DESCRIPTOR	*intfAlt1;
	AUDIO_STREAM_INTF_DESPT		*intfClass;

	TYPEI_FORMAT_DESPT	*format;
	int	fmtNumSampleFreq;
	U8	*sampleFreq;

	USB_ENDPOINT_DESCRIPTOR	*isoEndp;
	AUDIO_ISO_ENDP_DESPT	*isoClassEndp;
} AUDIO_STREAM_INTF;
#endif


//The "IPOD_USB_AUDIO" macro is defined in the file of "oem_drogon.h"
//the head file must use the macro below.
#if (defined AOA_USB_AUDIO || defined USB_HOST_AUDIO_ENABLE)

struct _IPOD_DEVICE;

typedef struct _USB_DEVICE {
	U8	addr;				//addr
	U8	state;				//enum_USBDEV_STATE
	U8	speed;				//enum_USB_SPEED
	U32	epMaxInPacket[16];
	U32 epMaxOutPacket[16];
	U32	epToggle[2];		
	U8	epBulk[2];			
	U8	epInt[2];
	PIPE pipes[enPIPE_INDX_MAX];
	USB_DEVICE_DESCRIPTOR	devDescriptor;
	U16 stringDescriptor[MAX_STR_DESCRIPTOR][MAX_STR_DESP_WORD];

	#define	CONFIG_BUF_SIZE     512 /* changed for AOA & USB Host Audio */

	//CONFIG despt + audioCtl i/f despt + 2x audioStream i/f despt + hid i/f

	U8	activeConfigVal;
	USB_CONFIGURATION	activeConfig;
	U8	configBuffer[CONFIG_BUF_SIZE];
	U32	configBufTotLen;
	U32	configTotLen;

	U8	totLUNs;	//total present
	U8	maxLUN;		//supported
	USB_BUS	*usbBus;
	struct _USB_DEVICE *parent;
	U8	port;
	U8	devType;
	void *classDev;
	char ipodType;
	U8	msDevType;
}	USB_DEVICE;

#else

typedef struct _USB_DEVICE {
	U8	addr;				//addr

	U8	state;				//enum_USBDEV_STATE
	U8	speed;				//enum_USB_SPEED

	U32	epMaxInPacket[16];
	U32 epMaxOutPacket[16];
	U32	epToggle[2];		
	U8	epBulk[2];			
	U8	epInt[2];

	PIPE pipes[enPIPE_INDX_MAX];

	USB_DEVICE_DESCRIPTOR	devDescriptor;
	U16 stringDescriptor[MAX_STR_DESCRIPTOR][MAX_STR_DESP_WORD];

#define	CONFIG_BUF_SIZE		64
	//CONFIG despt + audioCtl i/f despt + 2x audioStream i/f despt + hid i/f

	U8	activeConfigVal;
	USB_CONFIGURATION	activeConfig;
	U8	configBuffer[CONFIG_BUF_SIZE];
	U32	configBufTotLen;
	U32	configTotLen;
	U8	totLUNs;	//total present
	U8	maxLUN;		//supportted
	USB_BUS	*usbBus;
	struct _USB_DEVICE *parent;
	U8	port;
	U8	devType;
	void *classDev;
	char ipodType;
	U8	msDevType;
}	USB_DEVICE;

#endif //(defined AOA_USB_AUDIO || defined USB_HOST_AUDIO_ENABLE)



#ifdef USB_HOST_AUDIO_ENABLE
U8 installAudioDeviceDriver( USB_DEVICE * usbDev );
#endif //USB_HOST_AUDIO_ENABLE



#if (defined IPOD_USB_AUDIO || defined USB_HOST_AUDIO_ENABLE)
#include "hid.h"

typedef	struct {
	U8	type;

	union {
		MSC_INTF	mscIntf;
		HID_INTF	hidIntf;
		AUDIO_CTL_INTF	audioCtlIntf;
		AUDIO_STREAM_INTF	audioStreamIntf;

		//FIXME: add AOA/ADB
	} intfs;

} USB_INTF;

typedef	struct {
	U8	type;
	USB_CONFIGURATION_DESCRIPTOR	*configDespt;

	int	maxIntfs;
	int	intfnum;

	USB_INTF intfs[CONFIG_MAX_INTFS];
} USB_CONFIGS;

#endif //(defined IPOD_USB_AUDIO || defined USB_HOST_AUDIO_ENABLE


typedef enum {
	enINTF_UNKOWN,
	enINTF_IMAGE,
	enINTF_MSC,
	enINTF_HID,
	enINTF_AUDIO_CTL,
	enINTF_AUDIO_STREAM,
	enINTF_AOA,
	enINTF_ADB
}	enum_INTF_TYPE;

typedef enum {
	enCONFIG_UNKOWN,
	enCONFIG_IMAGE,
	enCONFIG_MSC,
	enCONFIG_HID,
	enCONFIG_AUDIO,
	enCONFIG_IPOD_DIGI_AUDIO	//audio+hid
}	enum_CONFIG_TYPE;

typedef enum {
	enNON_IPOD,
	enIPOD_DEV,
	enIPOD_ANALOG_MSC,		//only msc, no hid
	enIPOD_ANALOG_HID,
	enIPOD_DIGITAL_AUDIO,
	enIPOD_CHARGE_ONLY,
	enIPOD_UNKNOWN			//only support uart ?
}	enum_IPOD_TYPE;

typedef	enum {
	enMS_DEV_UNKOWN,
	enMS_DEV_MTP,

	enMS_DEV_MAX
}	enum_MS_DEV_TYPE;

typedef	enum {
	enUSB_DEV_MSC=0,
	enUSB_DEV_HUB,
	
	enUSB_DEV_IMAGE,	//PTP

	enUSB_DEV_IPOD,
	enUSB_DEV_AOA,
	
	enUSB_DEV_UNKOWN,	//0x05
	enUSB_DEV_MTP,

	enUSB_DEV_IPOD_CHARGE_ONLY,
	
	enUSB_DEV_DISCONNECT,	

	enUSB_DEV_TEST,
	enUSB_DEV_TEST_UNKOWN,
	enUSB_DEV_TEST_BYPASS,

#ifdef USB_HOST_AUDIO_ENABLE
	enUSB_DEV_AUDIO,	//0x0c, jj+ for Host Audio
#endif

	enUSB_DEV_MAX
}	enum_USB_DEV_TYPE;


typedef	struct {
	U32	urbMaxTimoutRetry;
	U32	urbMaxUncompRetry;
	U32	urbMaxIntUncompRetry;
}	CONFIG_PARAM;

extern CONFIG_PARAM *pconfigParam;
extern USB_DEVICE singleUsbDevice;

void usbd_init( void );
void usbd_open(void);
void usbd_close( void );


U8 usbd_bulkTransaction( USB_DEVICE *usbDev, U8 *buf, U32 *totalLen, U16 pipe );
U8 usbd_controlTransaction( USB_DEVICE *usbDev, U8 *setupPacket, U8 *buf, U32 *totalLen, U16 pipe );
U8 usbd_intTransaction( USB_DEVICE *usbDev, U8 *buf, U32 *totalLen, U16 pipe, U32 interval );

U8 usbd_mscResetRecovery( USB_DEVICE *usbDev );
U8 usbd_clearFeatureEndpHalt( USB_DEVICE *usbDev, U16 endp );

USB_DEVICE *usbd_getRootHub( void );
BOOL usbd_isDevHub( USB_DEVICE *dev );
BOOL usbd_isDevActive ( USB_DEVICE *dev );

//return enum_USB_DEV_TYPE
int usbd_getActiveDevType(void);


int usbd_mtpRequestCancel( USB_DEVICE *usbDev, U32 transID );
int usbd_mtpRequestGetStatus( USB_DEVICE *usbDev, U16 *statCode );

extern volatile U8 gwUsbDeviceReady;

int usbd_isIpodNoSrc( void );
int usbd_getActiveDev( USB_DEVICE **dev );
int usbd_isIpodExtModeProhibit( void );

int usbd_getCurrentDeviceType( void );
int usbd_get_usbhost_audio_device_status(void);


const char *optek_lib_usb_host_msc_audio_date_get(void);
const char *optek_lib_usb_host_msc_audio_time_get(void);

//optek_lib_usb_host_msc_audio.a release version
const char *optek_lib_usb_host_msc_audio_version_get(void);


#endif //_USBD_H_
