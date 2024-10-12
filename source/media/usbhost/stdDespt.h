#ifndef	_STD_DESCRIPTOR_H_
#define	_STD_DESCRIPTOR_H_


/* note: USB use LSB first & little Endian */


/* USB request define */
//request type  //USB20 9.3 USB Device Requests��?Table 9-2. Format of Setup Data, B0.D4...0 Recipient
#define	REQUEST_TYPE_RECIPIENT				0
#define	REQUEST_TYPE_RECIPIENT_MASK			0x1F
#define	REQUEST_TYPE_RECIPIENT_DEVICE		(0 << REQUEST_TYPE_RECIPIENT)
#define	REQUEST_TYPE_RECIPIENT_INTERFACE	(1 << REQUEST_TYPE_RECIPIENT)
#define	REQUEST_TYPE_RECIPIENT_ENDPOINT		(2 << REQUEST_TYPE_RECIPIENT)
#define	REQUEST_TYPE_RECIPIENT_OTHER		(3 << REQUEST_TYPE_RECIPIENT)

/* B0.D6...5 Type */
#define	REQUEST_TYPE_TYPE					5
#define	REQUEST_TYPE_TYPE_MASK				(0x03 << 5)
#define	REQUEST_TYPE_TYPE_STANDARD			(0 << REQUEST_TYPE_TYPE)
#define	REQUEST_TYPE_TYPE_CLASS				(1 << REQUEST_TYPE_TYPE)
#define	REQUEST_TYPE_TYPE_VENDOR			(2 << REQUEST_TYPE_TYPE)
#define	REQUEST_TYPE_TYPE_RESERVED			(3 << REQUEST_TYPE_TYPE)

/* B0.D7 Data transfer directrion */
#define	REQUEST_TYPE_DIR					7
#define	REQUEST_TYPE_DIR_MASK				(1 << 7)
#define	REQUEST_TYPE_DIR_H2D				(0 << REQUEST_TYPE_DIR)
#define	REQUEST_TYPE_DIR_D2H				(1 << REQUEST_TYPE_DIR)

#define	REQ_TYPE_IN_STD_DEV		(REQUEST_TYPE_DIR_D2H | REQUEST_TYPE_TYPE_STANDARD | REQUEST_TYPE_RECIPIENT_DEVICE)
#define	REQ_TYPE_OUT_STD_DEV	(REQUEST_TYPE_DIR_H2D | REQUEST_TYPE_TYPE_STANDARD | REQUEST_TYPE_RECIPIENT_DEVICE)

#define	REQ_TYPE_IN_CLASS_DEV	(REQUEST_TYPE_DIR_D2H | REQUEST_TYPE_TYPE_CLASS | REQUEST_TYPE_RECIPIENT_DEVICE)
#define	REQ_TYPE_OUT_CLASS_DEV	(REQUEST_TYPE_DIR_H2D | REQUEST_TYPE_TYPE_CLASS | REQUEST_TYPE_RECIPIENT_DEVICE)

#define	REQ_TYPE_OUT_STD_IF		(REQUEST_TYPE_DIR_H2D | REQUEST_TYPE_TYPE_STANDARD | REQUEST_TYPE_RECIPIENT_INTERFACE)
#define	REQ_TYPE_OUT_STD_ENDP	(REQUEST_TYPE_DIR_H2D | REQUEST_TYPE_TYPE_STANDARD | REQUEST_TYPE_RECIPIENT_ENDPOINT)


/*request */   //USB20 9.4 Standard Device Requests, Table 9-4. Standard Request Codes
#define	USB_REQUEST_GET_STATUS				0x00
#define	USB_REQUEST_CLEAR_FEATURE			0x01
#define	USB_REQUEST_SET_FEATURE				0x03
#define	USB_REQUEST_SET_ADDRESS				0x05
#define	USB_REQUEST_GET_DESCRIPTOR			0x06
#define	USB_REQUEST_SET_DESCRIPTOR			0x07
#define	USB_REQUEST_GET_CONFIGURATION		0x08
#define	USB_REQUEST_SET_CONFIGURATION		0x09
#define	USB_REQUEST_GET_INTERFACE			0x0A
#define	USB_REQUEST_SET_INTERFACE			0x0B
#define	USB_REQUEST_SYNCH_FRAME				0x0C

/* for test */
#define	USB_VENDOR_REQ_WRITE			0x01
#define	USB_VENDOR_REQ_READ				0x02

#define	USB_VENDER_REQ_TYPE_WRITE		(REQUEST_TYPE_RECIPIENT_ENDPOINT | REQUEST_TYPE_TYPE_VENDOR \
									|REQUEST_TYPE_DIR_H2D)
#define	USB_VENDER_REQ_TYPE_READ		(REQUEST_TYPE_RECIPIENT_ENDPOINT | REQUEST_TYPE_TYPE_VENDOR \
									|REQUEST_TYPE_DIR_D2H)

/* in mass storage spec */
#define	USB_REQUEST_BULK_ONLY_MASS_STORAGE_RESET	0xFF
#define	USB_REQUEST_GET_MAX_LUN						0xFE

#define	USB_REQUEST_TYPE_BULK_MASS_RESET			0x21	//0x20 ?
#define	USB_REQUEST_TYPE_GET_MAX_LUN				0xA1	//0x80 ?

/* in PTP/MTP usb still image spec */
#define	USB_REQUEST_MTP_CANCEL_TYPE			0x21
#define	USB_REQUEST_MTP_CANCEL_CODE			0x64

#define	USB_REQUEST_MTP_CANCEL_DATA_LEN		6

#define	USB_REQUEST_MTP_GETSTAT_TYPE		0xA1
#define	USB_REQUEST_MTP_GETSTAT_CODE		0x67

#define	USB_REQUEST_MTP_GET_STAT_MIN_LEN	(4)


typedef struct {    //USB20 Table 9-2
	U8	bmRequestType;
	U8	bRequest;
	U16 wValue;
	U16 wIndex;
	U16 wLength;
}	__attribute__  ((packed)) USB_DEVICE_REQEUST;
#define	SETUP_PKT_SIZE	8


/* USB descriptor define */

/* Descriptor type: bDescriptorType */  
/* USB20 9.4 Standard Device Requests, Table 9-5. Descriptor Types */
#define	DESCRIPTOR_TYPE_DEVICE					0x01
#define	DESCRIPTOR_TYPE_CONFIGURATION			0x02
#define	DESCRIPTOR_TYPE_STRING					0x03
#define	DESCRIPTOR_TYPE_INTERFACE				0x04
#define	DESCRIPTOR_TYPE_ENDPOINT				0x05
#define	DESCRIPTOR_TYPE_DEVICE_QUALIFIER		0x06
#define	DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG		0x07
#define	DESCRIPTOR_TYPE_INTERFACE_POWER			0x08

//JJ+ DFU
#define DESCRIPTOR_TYPE_DFU						0x21

//JJ+HID descriptor type
//#define DESCRIPTOR_TYPE_HID						0x21
//JJ+ report descriptor
#define DESCRIPTOR_TYPE_REPORT					0x22


//jj+
#define	DESCRIPTOR_TYPE_HUB                  	0x29


// in ECN (Engineer Change Notice)  //USB20 InterfaceAssociationDescriptor_ecn.pdf
#define	DESCRIPTOR_TYPE_OTG						0x09
#define	DESCRIPTOR_TYPE_DEBUG					0x0A
#define	DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION	0x0B

//Device Descriptor: bDeviceClass   //http://www.usb.org/developers/defined_class
#define	DEVICE_CLASS_AT_INTERFACE			0x00
#define	DEVICE_CLASS_COMMUNICATION			0x02
#define	DEVICE_CLASS_HUB					0x09
#define	DEVICE_CLASS_DIAGNOSTIC				0xDC
#define	DEVICE_CLASS_WIRELESS				0xE0
#define	DEVICE_CLASS_MISCELLANEOUS			0xEF
#define	DEVICE_CLASS_VENDOR_SPECIFIC		0xFF

//Interface Descriptor: bInterfaceClass //http://www.usb.org/developers/defined_class
#define	INTERFACE_CLASS_AUDIO				0x01
#define	INTERFACE_CLASS_CDC_CONTROL			0x02
#define	INTERFACE_CLASS_HID					0x03
#define	INTERFACE_CLASS_PHYSICAL			0x05
#define	INTERFACE_CLASS_IMAGE				0x06
#define	INTERFACE_CLASS_PRINTER				0x07
#define	INTERFACE_CLASS_MASS_STORAGE		0x08
#define	INTERFACE_CLASS_HUB					0x09
#define	INTERFACE_CLASS_CDC_DATA			0x0A
#define	INTERFACE_CLASS_SMART_CARD			0x0B
#define	INTERFACE_CLASS_CONTENT_SECURITY	0x0D
#define	INTERFACE_CLASS_VIDEO				0x0E
#define	INTERFACE_CLASS_DIAGNOSTIC			0xDC
#define	INTERFACE_CLASS_WIRELESS			0xE0
#define	INTERFACE_CLASS_APP_SPECIFIC		0xFE
#define	INTERFACE_CLASS_VENDOR_SPECIFIC		0xFF

/* USB20 9.4 Table 9-6. Standard Feature Selectors */
#define	FEATURE_SELECTOR_DEV_RMT_WAKEUP		0x01
#define	FEATURE_SELECTOR_ENDPT_HALT			0x00
#define	FEATURE_SELECTOR_DEV_TEST_MODE		0x02

/* for image device */
// subclass:bInterfaceSubClass
#define	IMAGE_DEVICE_INTERFACE_SUBCLASS		0x01
#define	IMAGE_DEVICE_INTERFACE_PROTOCOL		0x01


/* for HID */
#define	HID_CS_REQUEST_GET_REPORT			0x01
#define	HID_CS_REQUEST_GET_IDLE				0x02
#define	HID_CS_REQUEST_GET_PROTOCOL			0x03
#define	HID_CS_REQUEST_SET_REPORT			0x09
#define	HID_CS_REQUEST_SET_IDLE				0x0A
#define	HID_CS_REQUEST_SET_PROTOCOL			0x0B


/* for audio class  */
/* Audio20 final.pdf, Appendix A.5 Appendix A. Audio Device Class Codes */
#define	AUDIO_INTF_SUBCLASS_UNDEFINED		0x00
#define	AUDIO_INTF_SUBCLASS_CONTROL			0x01
#define	AUDIO_INTF_SUBCLASS_STREAM			0x02
#define	AUDIO_INTF_SUBCLASS_MIDI			0x03

/* audio class specific descriptor type */
#define	AUDIO_DESCRIPTOR_CS_UNDEFFINED		0x20
#define	AUDIO_DESCRIPTOR_CS_DEVICE			0x21
#define	AUDIO_DESCRIPTOR_CS_CONFIGURATION	0x22
#define	AUDIO_DESCRIPTOR_CS_STRING			0x23
#define	AUDIO_DESCRIPTOR_CS_INTERFACE		0x24
#define	AUDIO_DESCRIPTOR_CS_ENDPOINT		0x25

/* audio class specific descriptor audio control sub type */
/* Audio20, A.9 Audio Class-Specific AC Interface Descriptor Subtypes */
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_UNDEFINED		0x00
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_HEADER			0x01
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_INPUT_TERMINAL	0x02
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_OUTPUT_TERMINAL	0x03
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_MIXER_UNIT		0x04
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_SELECTOR_UNIT	0x05
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_FEATURE_UNIT		0x06
#ifdef ADC20
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_EFFECT_UNIT		0x07
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_PROCESS_UNIT		0x08
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_EXTENSION_UNIT	0x09
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_CLOCK_SOURCE		0x0A
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_CLOCK_SELECTOR	0x0B
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_CLOCK_MULTIPLIER	0x0C
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_SAMPLE_RATE_CONVERTER	0x0D
#else
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_PROCESS_UNIT		0x07
#define	AUDIO_DESCRIPTOR_CS_SUB_AC_EXTENSION_UNIT	0x08
#endif

/* audio class specific descriptor audio stream sub type */
/* Audio20, A.10 Audio Class-Specific AS Interface Descriptor Subtypes */
#define	AUDIO_DESCRIPTOR_CS_SUB_AS_UNDEFINED		0x00
#define	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL			0x01
#define	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE		0x02
#define	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_SPECIFIC	0x03

/* audio class specific endpoint descriptor subtype */
/* Audio20, A.13 Audio Class-Specific Endpoint Descriptor Subtypes */
#define	AUDIO_DESCRIPTOR_CS_SUB_ENDP_UNDEFINED		0x00
#define	AUDIO_DESCRIPTOR_CS_SUB_ENDP_GENERAL		0x01

/* audio class specific request code */
/* Audio20, A.14 Audio Class-Specific Request Codes */
#define	AUDIO_CS_REQUEST_UNDEFINED		0x00
#define	AUDIO_CS_REQUEST_SET_CUR		0x01
#define	AUDIO_CS_REQUEST_GET_CUR		0x81
#define	AUDIO_CS_REQUEST_SET_MIN		0x02
#define	AUDIO_CS_REQUEST_GET_MIN		0x82
#define	AUDIO_CS_REQUEST_SET_MAX		0x03
#define	AUDIO_CS_REQUEST_GET_MAX		0x83
#define	AUDIO_CS_REQUEST_SET_RES		0x04
#define	AUDIO_CS_REQUEST_GET_RES		0x84
#define	AUDIO_CS_REQUEST_SET_MEM		0x05
#define	AUDIO_CS_REQUEST_GET_MEM		0x85
#define	AUDIO_CS_REQUEST_GET_STAT		0xFF

#ifdef USB_DEV_ENABLE
//JJ+ Clock Source Control Selectors
#define AUDIO_CS_CONTROL_UNDEFINED    	0x00
#define AUDIO_CS_SAM_FREQ_CONTROL       0x01
#define AUDIO_CS_CLOCK_VALID_CONTROL    0x02

//JJ+ Clock Selector Control Selectors
#define AUDIO_CX_CONTROL_UNDEFINED		0x00
#define AUDIO_CX_CLOCK_SELECTOR_CONTROL	0x01

#define AUDIO_REQ_GET_MASK              0x80
#define AUDIO_REQ_GET_CUR               0x81
#define AUDIO_REQ_GET_MIN               0x82
#define AUDIO_REQ_GET_MAX               0x83
#define AUDIO_REQ_GET_RES               0x84

#define AUDIO_REQ_SET_CUR               0x01
#define AUDIO_REQ_SET_MIN               0x02
#define AUDIO_REQ_SET_MAX               0x03
#define AUDIO_REQ_SET_RES               0x04

#define AUDIO_REQ_CUR					0x01
#define AUDIO_REQ_RANGE                 0x02
#define AUDIO_REQ_MEM                   0x03
#endif

// terminal control selector //Audio20, A.17.4 Terminal Control Selectors
#define	AUDIO_CLASS_TEMINAL_CS_UNDEFINED		0x00
#define	AUDIO_CLASS_TEMINAL_CS_COPY_PROTECT		0x01

// feature unit control selector //Audio20, A.17.7 Feature Unit Control Selectors
#define	AUDIO_CLASS_FU_CS_UNDEFINED			0x00
#define	AUDIO_CLASS_FU_CS_MUTE				0x01
#define	AUDIO_CLASS_FU_CS_VOLUME			0x02
#define	AUDIO_CLASS_FU_CS_BASS				0x03
#define	AUDIO_CLASS_FU_CS_MID				0x04
#define	AUDIO_CLASS_FU_CS_TREBLE			0x05
#define	AUDIO_CLASS_FU_CS_EQ				0x06
#define	AUDIO_CLASS_FU_CS_AGC				0x07
#define	AUDIO_CLASS_FU_CS_DELAY				0x08
#define	AUDIO_CLASS_FU_CS_BASS_BOOST		0x09
#define	AUDIO_CLASS_FU_CS_LOUDNESS			0x0A

// endpoint control selector //Audio20, A.17.14 Endpoint Control Selectors
#define	AUDIO_CLASS_EP_CS_UNDEFINED			0x00
#define	AUDIO_CLASS_EP_CS_SAMPLE_FREQ		0x01
#define	AUDIO_CLASS_EP_CS_PITCH				0x02


// usb terminal type //Termt20 final.pdf, Table 2-1: USB Terminal Types
//USB streaming     0x0101      
#define	AUDIO_USB_TERM_UNDEFINED	0x0100
#define	AUDIO_USB_TERM_USB_STREAM	0x0101
#define	AUDIO_USB_TERM_VENDOR		0x01FF

//JJ+
// input terminal type //Termt20 final.pdf, Table 2-3: Output Terminal Types
#define	AUDIO_INPUT_TERM_UNDEFINED			0x0200
#define	AUDIO_INPUT_TERM_MICROPHONE		    0x0201

// output terminal type //Termt20 final.pdf, Table 2-3: Output Terminal Types
#define	AUDIO_OUTPUT_TERM_UNDEFINED			0x0300
#define	AUDIO_OUTPUT_TERM_SPEAKER			0x0301
#define	AUDIO_OUTPUT_TERM_HEADPHONE			0x0302
#define	AUDIO_OUTPUT_TERM_DISPLAY_AUDIO		0x0303
#define	AUDIO_OUTPUT_TERM_DESKTOP_SPEAKER	0x0304
#define	AUDIO_OUTPUT_TERM_ROOM_SPEAKER		0x0305
#define	AUDIO_OUTPUT_TERM_COMMU_SPEAKER		0x0306
#define	AUDIO_OUTPUT_TERM_SUBWOOFER			0x0307


// audio format type //Frmts20 final.pdf, A.1 Format Type Codes
#define	AUDIO_FORMAT_TYPE_UNDEFINED		0x00
#define	AUDIO_FORMAT_TYPE_I				0x01
#define	AUDIO_FORMAT_TYPE_II			0x02
#define	AUDIO_FORMAT_TYPE_III			0x03

// audio format type I code //Frmts20 final.pdf, A.2.1 Audio Data Format Type I Bit Allocations
#define	AUDIO_FORMAT_TYPE_I_UNDEFINED	0x0000
#define	AUDIO_FORMAT_TYPE_I_PCM			0x0001
#define	AUDIO_FORMAT_TYPE_I_PCM8		0x0002
#define	AUDIO_FORMAT_TYPE_I_IEEE_FLOAT	0x0003
#define	AUDIO_FORMAT_TYPE_I_ALAW		0x0004
#define	AUDIO_FORMAT_TYPE_I_MULAW		0x0005


//Audio Function Protocol
#define IP_VERSION_UNDEFINED            0x00
#define IP_VERSION_02_00                0x20


// protocol:bInterfaceProtocol

// below 2 are for mass storage only

//subclass code:bInterfaceSubClass
#define	SUBCLASS_CODE_RBC					0x01
#define	SUBCLASS_CODE_SFF8020i_MMC2			0x02
#define	SUBCLASS_CODE_QIC157				0x03
#define	SUBCLASS_CODE_UFI					0x04
#define	SUBCLASS_CODE_SFF8070i				0x05
#define	SUBCLASS_CODE_SCSI					0x06

//transport protocol:bInterfaceProtocol
#define	PROTOCOL_CODE_CBI_INT				0x00
#define	PROTOCOL_CODE_CBI_NO_INT			0x01
#define	PROTOCOL_CODE_BULK_ONLY				0x50

typedef struct { //USB20 9.5 Table 9-8. Standard Device Descriptor
	U8	bLength;
	U8	bDescriptorType;
	U16 bcdUSB;
	U8	bDeviceClass;
	U8	bDeviceSubClass;
	U8	bDeviceProtocol;
	U8	bMaxPacketSize0;
	U16	idVendor;
	U16	idProduct;
	U16	bcdDevice;
	U8	iManufacturer;
	U8	iProduct;
	U8	iSerialNumber;
	U8	bNumConfigurations;
}	__attribute__  ((packed)) USB_DEVICE_DESCRIPTOR;
#define	USB_DEVICE_DESCRIPTOR_LENGTH			18


#ifdef USB_DEV_ENABLE
//JJ+ USB20 9.6.2 Device_Qualifier
typedef struct {
	U8	bLength;
	U8	bDescriptorType;
	U16 bcdUSB;
	U8	bDeviceClass;
	U8	bDeviceSubClass;
	U8	bDeviceProtocol;
	U8	bMaxPacketSize0;
	U8	bNumConfigurations;
	U8  bReserved;
}	__attribute__  ((packed)) USB_DEVICE_QUALIFIER_DESCRIPTOR;
#define	USB_DEVICE_QUALIFIER_DESCRIPTOR_LENGTH	10
#endif

//USB20 9.5 Table 9-10. Standard Configuration Descriptor
typedef struct {
	U8	bLength;
	U8	bDescriptorType;
	U16	wTotalLength;
	U8	bNumInterfaces;
	U8	bConfigurationValue;
	U8	iConfiguration;
	U8	bmAttributes;
	U8	bMaxPower;
}	__attribute__  ((packed)) USB_CONFIGURATION_DESCRIPTOR;
#define	USB_CONFIGURATION_DESCRIPTOR_LENGTH		9

//USB20 9.5 Table 9-12. Standard Interface Descriptor
typedef struct {
	U8	bLength;
	U8	bDescriptorType;
	U8	bInterfaceNumber;
	U8	bAlternateSetting;
	U8	bNumEndpoints;
	U8	bInterfaceClass;
	U8	bInterfaceSubClass;
	U8	bInterfaceProtocol;
	U8	iInterface;
}	__attribute__  ((packed)) USB_INTERFACE_DESCRIPTOR;
#define	USB_INTERFACE_DESCRIPTOR_LENGTH			9

//USB20 9.5 Table 9-13. Standard Endpoint Descriptor
typedef struct {
	U8	bLength;
	U8	bDescriptorType;
	U8	bEndpointAddress;
	U8	bmAttributes;
	U16	wMaxPacketSize;
	U8	bInterval;
	
	//for USB Host Audio Clasee 1.0
	U8	bRefresh; //UAC1.0
	U8  bSynchAddress; //UAC1.0
}	__attribute__  ((packed)) USB_ENDPOINT_DESCRIPTOR;
#define	USB_ENDPOINT_DESCRIPTOR_LENGTH			7

//USB20 9.5 Table 9-15. String Descriptor Zero, Specifying Languages Supported by the Device
typedef struct {
	U8	bLength;
	U8	bDescriptorType;

	U16	wLANGID0;
	//...
	U16	wLANGIDn;
}	USB_STRING_LANGID_DESCRIPTOR;
#define	USB_STRING_LANGID_DESCRIPTOR_MIN_LENGTH		4

//USB20 9.5 Table 9-16. UNICODE String Descriptor
typedef struct {
	U8	bLength;
	U8	bDescriptorType;

	U16	bString0;
	//...
	U16	bStringn;
}	USB_STRING_UNICODE_DESCRIPTOR;


typedef struct {    //JJ+ USB20 11.23.2.1 Hub Descriptor
	U8	bDescLength;
	U8	bDescriptorType;
	U8 	bNbrPorts;
	U16	wHubCharacteristics;
	U8	bPwrOn2PwrGood;
	U8	bHubContrCurrent;
	U8	DeviceRemovable;	//max 8 ports
	U8	PortPwrCtrlMask;	//max 8 ports
	U8	bReserved;	//for variable PortPwrCtrlMask
}	__attribute__  ((packed)) USB_HUB_DESCRIPTOR;
#define	USB_HUB_DESCRIPTOR_LENGTH			10


#ifdef USB_DEV_ENABLE
typedef struct {    //JJ+ IAD
	U8	bLength;
	U8	bDescriptorType;
	U8	bFirstInterface;
	U8	bInterfaceCount;
	U8	bFunctionClass;
	U8	bFunctionSubClass;
	U8	bFunctionProtocol;
	U8	iFunction;
}	__attribute__  ((packed)) USB_IA_DESCRIPTOR;
#endif



/*	language identifier define */
#define	LANGID_CHINESE_TAIWAN		0x0404
#define	LANGID_CHINESE_PRC			0x0804
#define	LANGID_ENGLISH_USA			0x0409
#define	LANGID_ENGLISH_UK			0x0809

#ifdef	BIG_ENDIAN
#define	LANGID_ENGLISH_USA_E		0x0409
#else
#define	LANGID_ENGLISH_USA_E		0x0904
#endif


/* feature selector */
#define	FEATURE_SELECTOR_REMOTE_WAKEUP	0x01	//receipt: device
#define	FEATURE_SELECTOR_ENDPOINT_STALL	0x00	//receipt: endpoint
#define	FEATURE_SELECTOR_TEST_MODE		0x02	//receipt: device

#define	TESTMOD_SELECTOR_RESERVED		0x00
#define	TESTMOD_SELECTOR_TEST_J			0x01
#define	TESTMOD_SELECTOR_TEST_K			0x02
// ...


//USB20 9.6.6 Endpoint, Table 9-13. Standard Endpoint Descriptor
/* bEndpointAddress */
#define	ENDPOINT_ADDR_DIR_MASK			0x80
#define	ENDPOINT_ADDR_DIR_IN			0x80    //Host IN
#define	ENDPOINT_ADDR_DIR_OUT			0x00    //Host OUT

/* bmAttributes */
#define	ENDPOINT_ATTRIBUTE_TYPE_MASK	0x03
#define	ENDPOINT_TYPE_CONTROL			0x00
#define	ENDPOINT_TYPE_ISOCHRONOUS		0x01
#define	ENDPOINT_TYPE_BULK				0x02
#define	ENDPOINT_TYPE_INTERRUPT			0x03

#define	ENDPOINT_ATTR_SYNC_TYPE_MASK	0x0c
#define	ENDPOINT_ATTR_SYNC_NO_SYNC		(0 << 2)
#define	ENDPOINT_ATTR_SYNC_ASYNC		(1 << 2)
#define	ENDPOINT_ATTR_SYNC_ADAPTIVE		(2 << 2)
#define	ENDPOINT_ATTR_SYNC_SYNC			(3 << 2)


#endif //_STD_DESCRIPTOR_H_
