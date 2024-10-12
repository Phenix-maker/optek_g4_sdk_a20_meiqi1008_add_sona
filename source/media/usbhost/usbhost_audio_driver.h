#ifndef __USBHOST_AUDIO_DRIVER_H__
#define __USBHOST_AUDIO_DRIVER_H__



//JJ+ 5.2.3.2.1 Set Endpoint Control Request
#define	FILL_USB_GETEPREQUEST( PREQ,	REQTYPE,	REQUEST,	VALUE,	INDEX,	LENGTH ) \
	do {	\
		(PREQ)->bmRequestType	=	REQTYPE;	\
		(PREQ)->bRequest		=	REQUEST;	\
		(PREQ)->wValue			=	(VALUE);		\
		(PREQ)->wIndex			=	(INDEX);		\
		(PREQ)->wLength			=	(LENGTH);		\
	} while(0)	


//jj+ for Audio dev

typedef struct _HAUDIO_ENDP {
	//Endpoint Descriptor
	U8	bEndpointAddress;
	U8  bmAttributes;
	U16 wMaxPacketSize;
	U8	bInterval;
	U8	bRefresh;		//UAC1.0
	U8	bSynchAddress;	//UAC1.0
} HAUDIO_ENDP;

typedef struct _HAUDIO_FORMAT {
	U8	bFormatType;
	U8	bNrChannels;
	U8	bSubFrameSize;
	U8	bBitResolution;
	U8	bSamFreqType;
	U32	sampleFreq[10];	//maximum 10 sample freqs
} HAUDIO_FORMAT;

#ifdef UAC20
/* 4.9.2 Class-Specific AS Interface Descriptor */

typedef struct _HAUDIO_HEADER_UAC2 {
	U8 bLength;
	U8 bDescriptorType;
	U8 bDescriptorSubtype;
	U8 bTerminalLink;
	U8 bmControls;
	U8 bFormatType;
	U32 bmFormats;
	U8 bNrChannels;
	U32 bmChannelConfig;
	U8 iChannelNames;
} HAUDIO_HEADER_UAC2;

typedef struct _HAUDIO_FORMAT_UAC2 {//Frmts20: 2.3.1.6 Table 2-2: Type I Format Type Descriptor
	U8	bFormatType;
	U8	bSubslotSize;
	U8	bBitResolution;
} HAUDIO_FORMAT_UAC2;
#endif	//UAC20

typedef struct _HAUDIO_STREAM_INTF {
	// Interface Descriptor
	U8 	bInterfaceNumber;
	U8 	bAlternateSetting;
	U8	bNumEndpoints;
	U8 	bInterfaceClass;
	U8	bInterfaceSubClass;
	U8	bInterfaceProtocol;
	HAUDIO_FORMAT Haudio_format[2];	//In a same Interface and AlterSet, Out and In formats relate to below Eps!
#ifdef UAC20
	HAUDIO_HEADER_UAC2 Haudio_header2[2];//AS_GENERAL header
	HAUDIO_FORMAT_UAC2 Haudio_format2[2];//format for UAC2
#endif
	HAUDIO_ENDP Haudio_endp[2];		//Out Ep and In Ep, 0 for OUT, 1 for IN
	HAUDIO_ENDP Haudio_FBendp[2];	//Feedback inverted EP for Out Ep and In Ep, 0 for OUT, 1 for IN
	BOOL	Haudio_FBenable;
}	HAUDIO_STREAM_INTF;

typedef struct _HAUDIO_ActiveConf {
	U8 numConfig;
	U8 ActiveConfig;
	U8 FU_OutPathTIDs[3]; 	//for FU from USBstream to Speaker
	U8 FU_InPathTIDs[3]; 	//for FU from Microphone to USBstream
	HAUDIO_STREAM_INTF Haudio_Inst[10];	//maximum 10-16 Audio Intf Instances
}	HAUDIO_ActiveConf;



typedef struct {     //USB AUDIO AS FORMAT_TYPE Descriptor
	U8	bLength;
	U8	bDescriptorType;
	U8	bDescriptorSubtype;
	U8	bFormatType;
	U8	bNrChannels;
	U8	bSubframeSize;
	U8	bBitResolution;
	U8 	bSamFreqType;
	U8	tSamFreq[10][3];	//support maximum 10 sample frequencies
}	__attribute__  ((packed)) AUDIOAS_FMT_DESCRIPTOR;
#define	AUDIOAS_FMT_DESCRIPTOR_LENGTH		11

#ifdef UAC20
typedef struct {     //USB AUDIO AS FORMAT_TYPE Descriptor
	U8	bLength;
	U8	bDescriptorType;
	U8	bDescriptorSubtype;
	U8	bFormatType;
	U8	bSubslotSize;
	U8	bBitResolution;
}	__attribute__  ((packed)) AUDIOAS_FMT_DESCRIPTOR_UAC2;
#define	AUDIOAS_FMT_DESCRIPTOR_UAC2_LENGTH		6
#endif	//UAC20

typedef struct {     //CS Iso USB AUDIO EP Descriptor
	U8	bLength;
	U8	bDescriptorType;
	U8	bDescriptorSubtype;	//EP_GENERAL
	U8	bmAttributes;
	U8	bLockDelayUnits;
	U16	wLockDelay;
}	__attribute__  ((packed)) AUDIOEP_DESCRIPTOR;
#define	AUDIOEP_DESCRIPTOR_LENGTH		7

#ifdef UAC20
typedef struct {     //CS AS Intf Descriptor
	U8	bLength;
	U8	bDescriptorType;
	U8	bDescriptorSubtype;	//EP_GENERAL
	U8	bTerminalLink;
	U8  bmControls;
	U8	bFormatType;
	U32	bmFormats;
	U8  bNrChannels;
	U32 bmChannelConfig;
	U8 	iChannelNames;
}	__attribute__  ((packed)) AUDIOEP_HEADER_DESCRIPTOR_UAC2;
#define	AUDIOEP_HEADER_DESCRIPTOR_UAC2_LENGTH		16
#endif

//jj Enable or Disable Feedback Sync IRP function working!
//#define AyncFB_FUNCTION

BOOL getStringsD(USB_DEVICE * usbDev);

U8 usbhost_audio_device_get_SampleFreqList(USB_DEVICE * usbDev);


#endif //__USBHOST_AUDIO_DRIVER_H__
