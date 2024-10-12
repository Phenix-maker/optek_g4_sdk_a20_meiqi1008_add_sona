#ifndef __USBDEV_H__
#define __USBDEV_H__


//#define USB_SPEAKER_DONGLE
#ifndef USB_SPEAKER_DONGLE
//#define USB_SPEAKER_AUTO_DETECT
#endif

/***************************************
PRODUCT_ID   0x1112  for 2 Channels
PRODUCT_ID   0x1116  for 6 Channels
PRODUCT_ID   0x1118  for 8 Channels
***************************************/
#define	VENDOR_ID    0x0417
#define	PRODUCT_ID   0x1111



/* Allow USB AUDIO_SUB_FRAME_SIZE=3, AUDIO_BIT_RESOLUTION=24 */
//#define AudioData24BIT


/* JJ+ for USB audio class 2.0 */
//#define ADC20

//JJ!!!+ for USB duplex both downwards and upwards pipe
//Normal undef below macros, i.e. both Speaker and Mic exist!
#undef ONLYSpeakerCFG
#undef ONLYMicCFG


#undef ENABLE_MICFU
//#define ENABLE_MICFU

#ifdef HID_FUNC_ENABLE
#define HIDFUNC
#endif

#ifdef HIDFUNC
	/* for HID Consumer device! */
	#define HIDKEY
	/* for HID Data device! */
	#define HIDDATA
#endif

/* for USB CDC ACM */
//#define CDCFUNC

/* 2nd ISO IN Intf1/endp3In (#define	ENDP_NUM_ISO_INFB 3) for USB Audio Device Async ! */
//#define USBDEV_ISOIN_ASYNC

/* Only function of USB Speaker */
//#define ONLYSpeakerCFG

/* Only function of USB Microphone */
//#define ONLYMicCFG

/* JJ+ for DFU */
//#define DFUFUNC

//Enable USB SOF Interrupt
#define SOFINTEN

/* Window XP/7 can support only up to 44.1, 48Khz and 16bits !!! */
/* USB_AUDIO_SRC_44100
 * USB_AUDIO_SRC_48000
 * USB_AUDIO_SRC_88200
 * USB_AUDIO_SRC_96000
 * USB_AUDIO_SRC_176400
 * USB_AUDIO_SRC_192000
 */

#define USB_AUDIO_SRC_48000


/* second USB Audio Out should be mandatory ! */
/*
 * AUDIO_CHANNELS
 * 2
 * 6
 * 8
 */

#define	AUDIO_CHANNELS           8
#define	AUDIO_SUB_FRAME_SIZE     2  //pcm16
#define	AUDIO_BIT_RESOLUTION     16		


#define	AUDIO_CHANNELS2          2
#define	AUDIO_CHANNELS3          3
#define	AUDIO_CHANNELS4          4
#define	AUDIO_CHANNELS5          5
#define	AUDIO_CHANNELS6          6
#define	AUDIO_CHANNELS8          8



#define AUDIOIN_CHANNELS         2  /*for Audio In*/
#define AUDIOIN_SUB_FRAME_SIZE   2  //pcm16, Force MIC audio IN to 16bits !!!
#define AUDIOIN_BIT_RESOLUTION   16

//-------------------------------------------------------
#ifdef USB_AUDIO_SRC_44100
#define	AUDIO_SAMPLE_FREQ       44100
//for USB Audio In MIC.
#define	AUDIOIN_SAMPLE_FREQ     44100

//for second USB Audio Out.
#define	AUDIO_SAMPLE_FREQ2      44100
#endif

//-------------------------------------------------------
#ifdef USB_AUDIO_SRC_48000
//for first USB Audio Out.
#define AUDIO_SAMPLE_FREQ       48000

//for USB Audio In MIC.
/* !!! Note xiaomi6 cannot support 16KHz/24KHz !!! */
#define AUDIOIN_SAMPLE_FREQ     48000

//for second USB Audio Out.
#define AUDIO_SAMPLE_FREQ2      48000

//#define AUDIO_SAMPLE_RATE_HALF
#endif

//-------------------------------------------------------
#ifdef USB_AUDIO_SRC_88200
//for first USB Audio Out.
#define	AUDIO_SAMPLE_FREQ      88200
//for USB Audio In MIC.
#define	AUDIOIN_SAMPLE_FREQ    88200

//for second USB Audio Out.
#define	AUDIO_SAMPLE_FREQ2     44100
#endif

//-------------------------------------------------------
#ifdef USB_AUDIO_SRC_96000
//for first USB Audio Out.
#define	AUDIO_SAMPLE_FREQ      96000

//for USB Audio In MIC.
#define	AUDIOIN_SAMPLE_FREQ    48000

//for second USB Audio Out.
#define	AUDIO_SAMPLE_FREQ2     48000
#endif

//-------------------------------------------------------
#ifdef USB_AUDIO_SRC_176400
//for first USB Audio Out.
#define	AUDIO_SAMPLE_FREQ     176400

//for USB Audio In MIC.
#define	AUDIOIN_SAMPLE_FREQ   176400

//for second USB Audio Out.
#define	AUDIO_SAMPLE_FREQ2    44100
#endif

//-------------------------------------------------------
#ifdef USB_AUDIO_SRC_192000
//for first USB Audio Out.
#define	AUDIO_SAMPLE_FREQ     192000

//for USB Audio In MIC.
#define	AUDIOIN_SAMPLE_FREQ   192000

//for second USB Audio Out.
#define	AUDIO_SAMPLE_FREQ2    48000
#endif




typedef struct {
	BOOL	USBOutEnable;		//USB Speaker
	BOOL	USBInEnable;		//USB Mic, ONLYMicCFG, USB_DEVICE_IN_OUT_CFG
	BOOL	USBOutMixInEnable;	//Mix USB Out with other data then USB In.

	BOOL	SOFINTEnable;		//Enable SOF Interrupt

	BOOL	ADC20Enable;		//ADC20
	
	BOOL	ISOIN_ASYNCEnable;	//USBDEV_ISOIN_ASYNC

	BOOL	HIDFuncEnable;		//HIDFUNC
	BOOL 	HIDKEYEnable;		//HIDKEY

	BOOL 	USBAUDIOdata_BITs;	//USBAUDIO_BITs 0: 16bits, 1: 24bits

	BOOL 	SECOND_OUTEPEnable;	//SECOND_OUTEP, 2nd Ep for USB Audio Out.

	//-------- Audio Out/In formats --------
	U32		usb_AUDIO_SAMPLE_FREQ;		//AUDIO_SAMPLE_FREQ		48000, for first USB Audio Out.
	U32		usb_AUDIO_SAMPLE_FREQ2;		//AUDIO_SAMPLE_FREQ		44100, for second USB Audio Out.

	U8		usb_AUDIO_CHANNELS;			//AUDIO_CHANNELS			2
	U8		usb_AUDIO_SUB_FRAME_SIZE;	//AUDIO_SUB_FRAME_SIZE		2		//pcm16
	U8 		usb_AUDIO_BIT_RESOLUTION;	//AUDIO_BIT_RESOLUTION		16		

	U32		usb_AUDIOIN_SAMPLE_FREQ;	//AUDIOIN_SAMPLE_FREQ	48000, for USB Audio In MIC.

	U8		usb_AUDIOIN_CHANNELS;		//AUDIOIN_CHANNELS			2		/*for Audio In*/
	U8		usb_AUDIOIN_SUB_FRAME_SIZE;	//AUDIOIN_SUB_FRAME_SIZE	2		//pcm16, Force MIC audio IN to 16bits !!!
	U8		usb_AUDIOIN_BIT_RESOLUTION;	//AUDIOIN_BIT_RESOLUTION	16

} USB_DEV_FEATURE_CONF;

extern USB_DEV_FEATURE_CONF usbdev_featureconf;

/*
	device
*/

#define	VOLUME_MASTER_CHAN_ONLY


typedef enum {
	enUSB_DEV_STACK_ATTR_NULL	=	0,
	enUSB_DEV_STACK_ATTR_PC_SPEAKER,
	enUSB_DEV_STACK_ATTR_APPLE_LIGHTNING,
	enUSB_DEV_STACK_ATTR_USB_MICROPHONE
}	enum_USB_DEVICE_STACK_ATTR;

typedef enum {
	//Telephony Page
	enHUTTELEPHONY_HOOK	=	0x01,
	enHUTTELEPHONY_DROP	=	0x04,
	enHUTTELEPHONY_MUTE	=	0x04
}	enum_USB_HUTTELEPHONY_KEYCODE;

typedef enum {
	//Consumer Page
	enHUTCONSUMER_VOLUP	=	0x01,
	enHUTCONSUMER_VOLDOWN =	0x02,
	enHUTCONSUMER_PLAY	=	0x04,
	enHUTCONSUMER_NEXT	=	0x08,
	enHUTCONSUMER_PREV	=	0x10,
	enHUTCONSUMER_STOP	=	0x20,
	enHUTCONSUMER_FF	=	0x40,
	enHUTCONSUMER_RW	=	0x80
}	enum_USB_HUTCONSUMER_KEYCODE;



int usb_devStart(enum_USB_DEVICE_STACK_ATTR attr);
int usb_devStop(void);

int usb_devID_get(U16 *pVendorID, U16 *pProductID);
int usb_devAudioSampleRate_get(U32 *pSampleRate);

void daudio_SetFBSamplerateCorrection(int value);

#endif //__USBDEV_H__

