#ifndef __USBDEV_CONF_H__
#define __USBDEV_CONF_H__


extern U8 AUDIO_CHANNELS_cfg;


//	device descriptor

#define	USB_RELEASE_NUM			0x200
	//for IAD. 0x01 Interface Association Descriptor, iadclasscode_r10.pdf
#ifdef ADC20
#define IP_VERSION 				0x20
#define	DEVICE_CLASS			0xEF
#define	DEVICE_SUBCLASS			0x02
#define	DEVICE_PROTOCOL			0x01
#else
#define IP_VERSION 				0x00
#define	DEVICE_CLASS			0x00
#define	DEVICE_SUBCLASS			0x00
#define	DEVICE_PROTOCOL			0x00
#endif

#define	CTL_ENDP_SIZE			CTL_ENDP_MAX_PKT

#define	DEVICE_RELEASE_NUM		0x200

#define	CONFIGURATION_NUM		0x01

//static const U8 deviceDescriptor[USB_DEVICE_DESCRIPTOR_LENGTH] __USB_DEV_BSS = {
U8 deviceDescriptor[USB_DEVICE_DESCRIPTOR_LENGTH]  = {  //USB20 9.6.1
	USB_DEVICE_DESCRIPTOR_LENGTH,   //l=18
	DESCRIPTOR_TYPE_DEVICE,
	BYTE_SPLIT1(USB_RELEASE_NUM),	//bcdUSB
	BYTE_SPLIT2(USB_RELEASE_NUM),
	
	DEVICE_CLASS,		//0x00 common, 0xEF IAD
	DEVICE_SUBCLASS,	//0x00 common, 0x02 IAD
	DEVICE_PROTOCOL,	//0x00 common, 0x01 IAD
	CTL_ENDP_SIZE,	//bMaxPacketSize 64 bytes,Maximum packet size for endpoint zero

	BYTE_SPLIT1(VENDOR_ID),
	BYTE_SPLIT2(VENDOR_ID),
	BYTE_SPLIT1(PRODUCT_ID),
	BYTE_SPLIT2(PRODUCT_ID),
	BYTE_SPLIT1(DEVICE_RELEASE_NUM),	//bcdDevice
	BYTE_SPLIT2(DEVICE_RELEASE_NUM),

	eSTR_INDEX_MANUFACTURE,
	eSTR_INDEX_PRODUCT,
	eSTR_INDEX_SERIAL_NUM,
	CONFIGURATION_NUM   //Number of possible configurations
};

//JJ+ DESCRIPTOR_TYPE_DEVICE_QUALIFIER USB20 9.6.2 Device_Qualifier
U8 QdeviceDescriptor[USB_DEVICE_DESCRIPTOR_LENGTH]  = {  //USB20 9.6.1
	USB_DEVICE_QUALIFIER_DESCRIPTOR_LENGTH,   //l=10
	DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
	BYTE_SPLIT1(USB_RELEASE_NUM),	//bcdUSB
	BYTE_SPLIT2(USB_RELEASE_NUM),
	
	DEVICE_CLASS,		//0x00 common, 0xEF IAD
	DEVICE_SUBCLASS,	//0x00 common, 0x02 IAD
	DEVICE_PROTOCOL,	//0x00 common, 0x01 IAD
	CTL_ENDP_SIZE,	//bMaxPacketSize 64 bytes,Maximum packet size for endpoint zero

	CONFIGURATION_NUM,	//Number of possible configurations
	0					//bReserved
};

//	config descriptor

#if defined(ONLYSpeakerCFG) || defined(ONLYMicCFG)
#define	AUDIO_CTL_HEADER_LEN		0x09
#else	//one more INTF2 for ADC10, but not for ADC20 !!!
	#ifdef ADC20
	#define	AUDIO_CTL_HEADER_LEN		0x09
	#else
	#define	AUDIO_CTL_HEADER_LEN		0x0A
	#endif
#endif

#ifdef ADC20
#define AUDIO_CTL_CSD_LEN			0x08
#ifdef CXDDISABLE
#define AUDIO_CTL_CXD_LEN			0x00
#else
#define AUDIO_CTL_CXD_LEN			0x08
#endif
#define	AUDIO_INPUT_TERMINAL_LEN	0x11
#define	AUDIO_OUTPUT_TERMINAL_LEN	0x0c
#else
#define AUDIO_CTL_CSD_LEN			0x00
#define AUDIO_CTL_CXD_LEN			0x00
#define	AUDIO_INPUT_TERMINAL_LEN	0x0c
#define	AUDIO_OUTPUT_TERMINAL_LEN	0x09
#endif

#ifdef DISABLE_VOLUME_CTL
		#define	AUDIO_FEATURE_UNIT_LEN		0x00
#else

	#ifdef	VOLUME_MASTER_CHAN_ONLY
		#ifdef ADC20
		#define	AUDIO_FEATURE_UNIT_LEN		0x12
		#else
		#if (AUDIO_CHANNELS == 2)					//L + R
		#define	AUDIO_FEATURE_UNIT_LEN		0x0A
		#elif (AUDIO_CHANNELS == 6)					//L + R + C + Lfe + Ls + Rs
		#define	AUDIO_FEATURE_UNIT_LEN		0x0E
		#elif (AUDIO_CHANNELS == 8)					//L + R + C + Lfe + Ls + Rs + Lc + Rc
		#define	AUDIO_FEATURE_UNIT_LEN		0x10
		#endif

		#endif //ADC20
	#else
		#define	AUDIO_FEATURE_UNIT_LEN		0x0d
	#endif //VOLUME_MASTER_CHAN_ONLY

#endif //DISABLE_VOLUME_CTL

#ifdef ENABLE_MICFU
	#define AUDIOMIC_FEATURE_UNIT_LEN AUDIO_FEATURE_UNIT_LEN
#else	
	#define AUDIOMIC_FEATURE_UNIT_LEN 0
#endif //ENABLE_MICFU

//9+a +c+a+9 = 32, 49 for now
#ifdef ONLYSpeakerCFG//JJ
#define	AUDIO_CTL_INTF_TOTAL_LEN	( USB_INTERFACE_DESCRIPTOR_LENGTH + AUDIO_CTL_HEADER_LEN + AUDIO_CTL_CSD_LEN + AUDIO_CTL_CXD_LEN + \
			AUDIO_INPUT_TERMINAL_LEN + AUDIO_FEATURE_UNIT_LEN + AUDIO_OUTPUT_TERMINAL_LEN )
#else
//add 3 more Terms of Mic, 9+a +c+a+9+ c+0+9 = 47 now, add AUDIO_CTL_CSD_LEN
 #ifndef ONLYMicCFG
 #define	AUDIO_CTL_INTF_TOTAL_LEN	( USB_INTERFACE_DESCRIPTOR_LENGTH + AUDIO_CTL_HEADER_LEN + AUDIO_CTL_CSD_LEN/* + AUDIO_CTL_CSD_LEN*/ + AUDIO_CTL_CXD_LEN + \
			AUDIO_INPUT_TERMINAL_LEN + AUDIO_FEATURE_UNIT_LEN + AUDIO_OUTPUT_TERMINAL_LEN  + \
			AUDIO_INPUT_TERMINAL_LEN + AUDIOMIC_FEATURE_UNIT_LEN + AUDIO_OUTPUT_TERMINAL_LEN )
 #else
 #define	AUDIO_CTL_INTF_TOTAL_LEN	( USB_INTERFACE_DESCRIPTOR_LENGTH + AUDIO_CTL_HEADER_LEN + AUDIO_CTL_CSD_LEN/* + AUDIO_CTL_CSD_LEN*/ + AUDIO_CTL_CXD_LEN + \
			AUDIO_INPUT_TERMINAL_LEN + AUDIOMIC_FEATURE_UNIT_LEN + AUDIO_OUTPUT_TERMINAL_LEN )
 #endif
#endif

#ifdef ADC20
#define	USB_IAD_LENGTH			8
#define	AUDIO_AS_GENERAL_INTF_LEN	0x10
#define	AUDIO_TYPE_I_FORMAT_LEN		0x06
#define	AUDIO_CLASS_STD_ENDP_LEN	0x07
#define	AUDIO_CLASS_ISO_ENDP_LEN	0x08
//AS ISO feedback EP len
#define	AUDIO_AC_INT_ENDP_LEN		0x07
#else
#define	USB_IAD_LENGTH			0
//AS ISO feedback EP len
#define	AUDIO_AC_INT_ENDP_LEN		0x00
#define	AUDIO_AS_GENERAL_INTF_LEN	0x07
#define	AUDIO_TYPE_I_FORMAT_LEN		0x0b
#define	AUDIO_CLASS_STD_ENDP_LEN	0x09
#define	AUDIO_CLASS_ISO_ENDP_LEN	0x07
#endif

//9*2+7+11+9+7 = 52
#define AUDIO_STREAM_INTF_TOTAL_LEN	( USB_INTERFACE_DESCRIPTOR_LENGTH*2 + \
			AUDIO_AS_GENERAL_INTF_LEN + AUDIO_TYPE_I_FORMAT_LEN	+		\
			AUDIO_CLASS_STD_ENDP_LEN + AUDIO_CLASS_ISO_ENDP_LEN )

/* to support only 16 bits data */
//#define SECOND_OUTEP

#ifdef SECOND_OUTEP
#define AUDIO_STREAM_INTF2_TOTAL_LEN	( USB_INTERFACE_DESCRIPTOR_LENGTH + \
			AUDIO_AS_GENERAL_INTF_LEN + AUDIO_TYPE_I_FORMAT_LEN	+		\
			AUDIO_CLASS_STD_ENDP_LEN + AUDIO_CLASS_ISO_ENDP_LEN )
#else
#define AUDIO_STREAM_INTF2_TOTAL_LEN (0)
#endif

// Intf1/Alt2
#define ALT2_INTF

// Intf1/Alt3
#define ALT3_INTF

// Intf1/Alt4
#define ALT4_INTF


#define AUDIO_STREAM_ALT2INTF_TOTAL_LEN (0)
#define AUDIO_STREAM_ALT3INTF_TOTAL_LEN (0)
#define AUDIO_STREAM_ALT4INTF_TOTAL_LEN (0)

#ifdef ALT2_INTF
	#define AUDIO_STREAM_ALT2INTF_TOTAL_LEN	( USB_INTERFACE_DESCRIPTOR_LENGTH + \
			AUDIO_AS_GENERAL_INTF_LEN + AUDIO_TYPE_I_FORMAT_LEN	+		\
			AUDIO_CLASS_STD_ENDP_LEN + AUDIO_CLASS_ISO_ENDP_LEN )
#endif

#ifdef ALT3_INTF
	#define AUDIO_STREAM_ALT3INTF_TOTAL_LEN	( USB_INTERFACE_DESCRIPTOR_LENGTH + \
			AUDIO_AS_GENERAL_INTF_LEN + AUDIO_TYPE_I_FORMAT_LEN	+		\
			AUDIO_CLASS_STD_ENDP_LEN + AUDIO_CLASS_ISO_ENDP_LEN )
#endif

#ifdef ALT4_INTF
	#define AUDIO_STREAM_ALT4INTF_TOTAL_LEN	( USB_INTERFACE_DESCRIPTOR_LENGTH + \
			AUDIO_AS_GENERAL_INTF_LEN + AUDIO_TYPE_I_FORMAT_LEN	+		\
			AUDIO_CLASS_STD_ENDP_LEN + AUDIO_CLASS_ISO_ENDP_LEN )
#endif




/************************  DEV_CONFIGURATION_TOTAL_LEN  ************************/
//Pay atten: 
//	AUDIO_CLASS_STD_ENDP_LEN:	len of Async Ep Descriptor
//	0x19 + 0x7:					len of HID Descriptor
//	AUDIO_CTL_INTF_TOTAL_LEN:
//	AUDIO_AC_INT_ENDP_LEN:		FB ep for ADC20

#ifdef HIDFUNC
	#define DATAonCONTROLPIPE

	#if 0//#ifdef HIDKEY
	#ifdef HIDDATA
	#define HID_LEN (0x19 + 0x7)
	#else
	#define HID_LEN (0x19)
	#endif
	#else  //no HIDKEY
	  #ifdef DATAonCONTROLPIPE
		#define HID_LEN (0x19)
	  #else
		//2 Eps for HID
		//#define HID_LEN (0x19 + 0x07)
	  #endif
	#endif
#else	
	#define HID_LEN (0x00)
#endif

#ifdef CDCFUNC
	#define CDC_LEN (58)
#else	
	#define CDC_LEN (0x00)
#endif

#ifdef ONLYSpeakerCFG//JJ 0x6D

 #ifdef HIDFUNC
	#ifdef USBDEV_ISOIN_ASYNC
	//#ifndef HIDKEY
	 	#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
			AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_CLASS_STD_ENDP_LEN + AUDIO_AC_INT_ENDP_LEN + HID_LEN)
	#else
		#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
			AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_AC_INT_ENDP_LEN + HID_LEN)
	#endif
 #else
	#ifdef DFUFUNC
		#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
			AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_AC_INT_ENDP_LEN + 0x09 + 0x09)
	#else
		#ifdef USBDEV_ISOIN_ASYNC
		#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
			AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_CLASS_STD_ENDP_LEN + AUDIO_AC_INT_ENDP_LEN )		
		#else
		#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
			AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_AC_INT_ENDP_LEN  + AUDIO_STREAM_ALT2INTF_TOTAL_LEN  + AUDIO_STREAM_ALT3INTF_TOTAL_LEN  + AUDIO_STREAM_ALT4INTF_TOTAL_LEN)
		#endif //USBDEV_ISOIN_ASYNC
	#endif
 #endif //HIDFUNC

#else //ONLYSpeakerCFG
//9+48+52+52 = 109+52 = 161, 162, 9+79+52+52 = 192 (0xc0) for now, 0x19 Desc of HID  [Here only ADC20]
 #ifdef HIDFUNC
 
  #ifndef ONLYMicCFG
	#ifndef SECOND_OUTEP

	#ifdef USBDEV_ISOIN_ASYNC
	#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
			AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_CLASS_STD_ENDP_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_AC_INT_ENDP_LEN + HID_LEN)
	#else 	//USBDEV_ISOIN_ASYNC
	#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
			AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_AC_INT_ENDP_LEN  + AUDIO_STREAM_ALT2INTF_TOTAL_LEN  + AUDIO_STREAM_ALT3INTF_TOTAL_LEN  + AUDIO_STREAM_ALT4INTF_TOTAL_LEN + HID_LEN + CDC_LEN)
/*	#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
			AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_AC_INT_ENDP_LEN + HID_LEN)*/
	#endif 	//USBDEV_ISOIN_ASYNC
	
	#endif //SECOND_OUTEP
  #else
 	//No Asnyc, but with HID !
	#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
			AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + HID_LEN)
  #endif //ONLYMicCFG

 #else //no HIDFUNC
		#ifdef USBDEV_ISOIN_ASYNC
		
		  #ifndef ONLYMicCFG
			#ifndef SECOND_OUTEP
													//9+0+ \  61+52+9+52+0
			#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
					AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_CLASS_STD_ENDP_LEN + AUDIO_STREAM_INTF_TOTAL_LEN)
			#else
			#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
					AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_STREAM_INTF2_TOTAL_LEN + AUDIO_AC_INT_ENDP_LEN)
			#endif
		  #else
			#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
					AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN)
		  #endif	

		#else //USBDEV_ISOIN_ASYNC
		
		  #ifndef ONLYMicCFG
			#ifndef SECOND_OUTEP
			#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
					AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_AC_INT_ENDP_LEN  + AUDIO_STREAM_ALT2INTF_TOTAL_LEN  + AUDIO_STREAM_ALT3INTF_TOTAL_LEN  + AUDIO_STREAM_ALT4INTF_TOTAL_LEN + CDC_LEN)
/*			#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
					AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN )*/
			#else
			#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
					AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN + AUDIO_STREAM_INTF2_TOTAL_LEN + AUDIO_STREAM_ALT2INTF_TOTAL_LEN  + AUDIO_STREAM_ALT3INTF_TOTAL_LEN  + AUDIO_STREAM_ALT4INTF_TOTAL_LEN)
			#endif
		  #else
			#define	DEV_CONFIGURATION_TOTAL_LEN		(USB_CONFIGURATION_DESCRIPTOR_LENGTH + USB_IAD_LENGTH + \
					AUDIO_CTL_INTF_TOTAL_LEN + AUDIO_STREAM_INTF_TOTAL_LEN )
		  #endif //ONLYMicCFG
		
		#endif //USBDEV_ISOIN_ASYNC

 #endif //HIDFUNC 
#endif //ONLYSpeakerCFG



#define	CONFIG_VALUE	1
//jj#define	CONFIGURATION_ATTRIBUTE		0xc0
#define	CONFIGURATION_ATTRIBUTE		0x80
//jj for less Max Power! #define	CONFIGURATION_MAXPOWER		(500/2)	//500mA
//The USB device is now in the Default state and can draw
//no more than 100 mA from VBUS.
#define	CONFIGURATION_MAXPOWER		(500/2)	//500mA

#define	AUDIO_CTL_INTF_NUM			0
#ifndef ONLYMicCFG
#define	AUDIO_STREAM_INTF_NUM		1
#define	AUDIO_STREAM_INTF_NUM2		2
#else	//9 means unavailable, specific for test MIC only !!!
#define	AUDIO_STREAM_INTF_NUM		9
#define	AUDIO_STREAM_INTF_NUM2		2
#endif	
#define	AUDIO_CTL_ALT_SETTING			0

#define	AUDIO_STRAM_ZEROBAND_ALT_SET		0
#define	AUDIO_STRAM_NON_ZEROBAND_ALT_SET	1
#define	AUDIO_STRAM_NON_ZEROBAND_ALT_SET2	2
#define	AUDIO_STRAM_NON_ZEROBAND_ALT_SET3	3
#define	AUDIO_STRAM_NON_ZEROBAND_ALT_SET4	4

//For DNUSB -- FU -- SPK

#define	AUDIO_INPUT_TERM_ID			0x01
#define	AUDIO_FEATURE_UNIT_ID		0x02
#define	AUDIO_OUTPUT_TERM_ID		0x03

//For MIC -- FU -- UPUSB
#define	AUDIO_INPUTMIC_TERM_ID		0x04
#define	AUDIO_FEATUREMIC_UNIT_ID	0x05
#define	AUDIO_OUTPUTMIC_TERM_ID		0x06

#ifdef ADC20
#define	AUDIO_CLASS_SPEC_VER		0x0200
#else
#define	AUDIO_CLASS_SPEC_VER		0x0100
#endif

#ifdef ONLYSpeakerCFG//JJ
#define	AUDIO_CLASS_SPECIFIC_AC_TOT_LEN		(AUDIO_CTL_HEADER_LEN +		\
			AUDIO_INPUT_TERMINAL_LEN + AUDIO_FEATURE_UNIT_LEN + AUDIO_OUTPUT_TERMINAL_LEN + AUDIO_CTL_CSD_LEN + AUDIO_CTL_CXD_LEN )
#else
//add 3 more Terms: 10 (0x0a) + 12(0x0c) + 9 + 9 + 12(0x0c) + 9 + 9 = 70
#ifndef ONLYMicCFG
#define	AUDIO_CLASS_SPECIFIC_AC_TOT_LEN		(AUDIO_CTL_HEADER_LEN +		\
			AUDIO_INPUT_TERMINAL_LEN + AUDIO_FEATURE_UNIT_LEN + AUDIO_OUTPUT_TERMINAL_LEN + \
			AUDIO_INPUT_TERMINAL_LEN + AUDIOMIC_FEATURE_UNIT_LEN + AUDIO_OUTPUT_TERMINAL_LEN + AUDIO_CTL_CSD_LEN/* + AUDIO_CTL_CSD_LEN*/ + AUDIO_CTL_CXD_LEN )
#else
#define	AUDIO_CLASS_SPECIFIC_AC_TOT_LEN		(AUDIO_CTL_HEADER_LEN +		\
			AUDIO_INPUT_TERMINAL_LEN + AUDIOMIC_FEATURE_UNIT_LEN + AUDIO_OUTPUT_TERMINAL_LEN + AUDIO_CTL_CSD_LEN/* + AUDIO_CTL_CSD_LEN*/ + AUDIO_CTL_CXD_LEN )
#endif
#endif			


#if (AUDIO_CHANNELS == 2)					//L + R
	#define	AUDIO_CHANNEL_CONFIG	0x0003	
#elif (AUDIO_CHANNELS == 6)					//L + R + C + Lfe + Ls + Rs
	#define	AUDIO_CHANNEL_CONFIG	0x003f	
#elif (AUDIO_CHANNELS == 8)					//L + R + C + Lfe + Ls + Rs + Lc + Rc
	#define	AUDIO_CHANNEL_CONFIG	0x063f	
#endif

//L + R
	#define	AUDIOIN_CHANNEL_CONFIG	0x03

#define	FEATURE_UNIT_CTL_SIZE	1

#ifdef	VOLUME_MASTER_CHAN_ONLY
#define	FU_BMA_CTL0				0x03	//mute & vol
#else
#define	FU_BMA_CTL0				0x01	//mute for master channel
#define	FU_BMA_CTL1				0x02	//vol, channle 1, left front
#define	FU_BMA_CTL2				0x02	//vol, channel 2, right front
#endif

#define	ISO_OUT_ENDP_NUM		1
//0x01
#define	ISO_OUT_ENDP_ADDR		(ENDPOINT_ADDR_DIR_OUT | ISO_OUT_ENDP_NUM)
#define	ISO_IN_ENDP_NUM		    1
//0x82
#define	ISO_IN_ENDP_ADDR		(ENDPOINT_ADDR_DIR_IN | ISO_IN_ENDP_NUM)

#ifdef ADC20
//96Khz x 2channels x 16bits
#define	ISO_STD_OUT_ENDP_ATTR	(ENDPOINT_TYPE_ISOCHRONOUS | ENDPOINT_ATTR_SYNC_ASYNC)		
#define	ISO_STD_OUT_ENDP_MAXPKT	1024
//#define	ISO_STD_OUT_ENDP_MAXPKT	(96000*2*2/8000)
//#define	ISO_STD_OUT_ENDP_MAXPKT	(44100*2*2/8000)
#else
#define	ISO_STD_OUT_ENDP_ATTR	(ENDPOINT_TYPE_ISOCHRONOUS | ENDPOINT_ATTR_SYNC_ADAPTIVE)		
#define	ISO_STD_OUT_ENDP_MAXPKT	DISO_MAX_PKT_SIZE
#define	ISO_STD_OUT_ENDP_MAXPKT2	DISO_MAX_PKT_SIZE2
#endif
#define	ISO_CS_OUT_ENDP_ATTR	0x00

#ifdef ADC20
#define	ISO_STD_IN_ENDP_ATTR	(ENDPOINT_TYPE_ISOCHRONOUS | ENDPOINT_ATTR_SYNC_ASYNC)		
#define	ISO_STD_IN_ENDP_MAXPKT	1024
#else
#define	ISO_STD_IN_ENDP_ATTR	(ENDPOINT_TYPE_ISOCHRONOUS | ENDPOINT_ATTR_SYNC_ADAPTIVE)		
//#define	ISO_STD_IN_ENDP_MAXPKT	DISO_MAX_PKT_SIZE
/* For Audio IN 2x16bits, set to fixed 256 ! Not considering Sample Freq. */
//#define	ISO_STD_IN_ENDP_MAXPKT	256
/* !!!!!!!!!!!   Too big cause AUDIN fail !!!!!!!!!!! */
//#define	ISO_STD_IN_ENDP_MAXPKT	384
//#define	ISO_STD_IN_ENDP_MAXPKT	(192+8)
#define	ISO_STD_IN_ENDP_MAXPKT	DISOIN_MAX_PKT_SIZE
#endif
#define	ISO_CS_IN_ENDP_ATTR	    0x00

/////////////////////////////////////////////////////////////////////////////////////
//Device
//  Configuration
//interface 00 - audioControl     AC IF ---------------------------------------------
//          header		<AUDIO_CLASS_SPEC_VER>
//              <AUDIO_STREAM_INTF_NUM>
//              <AUDIO_STREAM_INTF_NUM2>
//          IT
//              <AUDIO_INPUT_TERM_ID> -- AUDIO_OUTPUT_TERM_ID
//          FU
//              <AUDIO_FEATURE_UNIT_ID> -- AUDIO_INPUT_TERM_ID
//          OT
//              <AUDIO_OUTPUT_TERM_ID> -- AUDIO_INPUT_TERM_ID, AUDIO_FEATURE_UNIT_ID
//          IT  ---
//              <AUDIO_INPUTMIC_TERM_ID> -- AUDIO_OUTPUTMIC_TERM_ID
//          FU  ---
//              <AUDIO_FEATUREMIC_UNIT_ID> -- AUDIO_INPUTMIC_TERM_ID
//          OT  ---
//              <AUDIO_OUTPUTMIC_TERM_ID> -- AUDIO_INPUTMIC_TERM_ID, AUDIO_FEATUREMIC_UNIT_ID

//INTERFACE 00 - audioControl 	-----------------------------------------------------
//

//INTERFACE 01 - audioStream --------------------------------------------------------	? ONLYSpeakerCFG
//  alt intf0	-	zero bandwidth
//    <AUDIO_STREAM_INTF_NUM>
//  alt intf1 -	non-zero bandwidth
//    <AUDIO_STREAM_INTF_NUM>
//      general IF
//          <AUDIO_INPUT_TERM_ID>
//      type I format
//  endpoint iso OUT		( 0x01 )
//      <ISO_OUT_ENDP_ADDR>
//  class-iso endpoint
//	endpoint iso IN			( 0x83 )	for Async FB!

//INTERFACE 02 - audioStream --------------------------------------------------------	? ONLYMicCFG
//  alt intf0	-	zero bandwidth
//    <AUDIO_STREAM_INTF_NUM2>
//  alt intf1 -	non-zero bandwidth
//    <AUDIO_STREAM_INTF_NUM2>
//      general IF
//          <AUDIO_INPUTMIC_TERM_ID>
//      type I format
//  endpoint iso IN			( 0x81 )
//      <ISO_IN_ENDP_ADDR>
//  class-iso endpoint

//INTERFACE 03 - HID ----------------------------------------------------------------	? HIDFUNC
//	HID Descriptor
//	endpoint Interrupt IN 	( 0x82 )
//	endpoint Interrupt OUT 	( 0x02 )
/////////////////////////////////////////////////////////////////////////////////////



/*	Audio20: 4.8.2.1 Standard AC Interrupt Endpoint Descriptor, This endpoint is optional.	*/
//static const U8 fsUsbSpeaker_configDescriptor[DEV_CONFIGURATION_TOTAL_LEN]	__USB_DEV_BSS =	{
//static const U8 fsUsbSpeaker_configDescriptor[DEV_CONFIGURATION_TOTAL_LEN] =	{
//Include all Interfaces of Audio spk, Mic and HID Report

static const U8 fsUsbSpeaker_configDescriptor[] =	{
//config    USB20 9.6.3 Table 9-10

	USB_CONFIGURATION_DESCRIPTOR_LENGTH,//l=9
	DESCRIPTOR_TYPE_CONFIGURATION,
	BYTE_SPLIT1(DEV_CONFIGURATION_TOTAL_LEN),//0x09 remove FU of Mic, 0x19 Desc of HID
	BYTE_SPLIT2(DEV_CONFIGURATION_TOTAL_LEN),
#ifdef HIDFUNC			//(? HID ) + (? Speaker ) + (? Mic ) + (? DFU)
 #ifndef ONLYSpeakerCFG//JJ
   #ifndef ONLYMicCFG
	#ifdef CDCFUNC			//
	6,//INTERFACE_NUM,  //now have 4 INTFs:INTF0, INTF1 for OUT, INTF2 for IN, INTF3/ITF4 for HID IN/OUT, INTF5/0, Intf6/0
	#else
	4,//INTERFACE_NUM,  //now have 4 INTFs:INTF0, INTF1 for OUT, INTF2 for IN, INTF3/ITF4 for HID IN/OUT
	#endif
   #else //ONLYMicCFG
	3,//INTERFACE_NUM,  //now have 3 INTFs:INTF0, INTF2 for IN, INTF3 for HID IN
   #endif
 #else //ONLYSpeakerCFG
	3,//INTERFACE_NUM,  //now have 3 INTFs:INTF0, INTF1 for OUT, INTF3 for HID IN
 #endif
#else //no HIDFUNC		//(? Speaker ) + (? Mic ) + (? DFU)
 #ifndef ONLYSpeakerCFG//JJ
   #ifndef ONLYMicCFG
	3,//INTERFACE_NUM,  //now have 3 INTFs:INTF0, INTF1 for OUT, INTF2 for IN
   #else //ONLYMicCFG
	2,//INTERFACE_NUM,  //now have 2 INTFs:INTF0, INTF2 for IN
   #endif
 #else //ONLYSpeakerCFG
	2,//INTERFACE_NUM,  //now have 2 INTFs:INTF0, INTF1 for OUT
 #endif
#endif //HIDFUNC
	//3,//INTERFACE_NUM,  //now have 2 INTFs:INTF0, INTF1 for OUT, DFU
CONFIG_VALUE,       //argument to the SetConfiguration() request to select this configuration
0x00,               //Index of string descriptor describing this configuration
CONFIGURATION_ATTRIBUTE,
CONFIGURATION_MAXPOWER,


	//for IAD
#ifdef ADC20
	//IAD JJ+
	USB_IAD_LENGTH, 				//10, l=8
	DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION,
	0,             					//first Intf index
#ifndef ONLYSpeakerCFG//JJ
#ifndef ONLYMicCFG
	3,          					//Intf count 00, 01, 02 of audio function !!!
#else
	2,								//Intf count 00, 02 of audio function !!!
#endif
#else
	2,								//Intf count 00, 01 of audio function !!!
#endif
	0x01,          					//bFunctionClass AUDIO_FUNCTION
	0x00,    						//bFunctionSubClass 0x00 FUNCTION_SUBCLASS_UNDEFINED
	0x20,							//bFunctionProtocol 0x20 AF_VERSION_02_00
	0x00,                           //Index of a string descriptor
#endif //ADC20


//interface 00 - audioControl 	======================================================================
	//audio control intf 0  USB20 9.6.5 Table 9-12, 4.3.1 Standard AC Interface Descriptor
	USB_INTERFACE_DESCRIPTOR_LENGTH, //10, l=9
	DESCRIPTOR_TYPE_INTERFACE,
	AUDIO_CTL_INTF_NUM,             //Intf index
	AUDIO_CTL_ALT_SETTING,          //only alt0 for INTF0
	0x00,                           //0 EPs,	1 for Interrupt EP available
	INTERFACE_CLASS_AUDIO,          //Class
	AUDIO_INTF_SUBCLASS_CONTROL,    //Subclass
	IP_VERSION,						//bInterfaceProtocol
	0x00,                           //Index of a string descriptor

	//header
	AUDIO_CTL_HEADER_LEN,           //l=9, same for ADC20 Table 4-5: Class-Specific AC Interface Header Descriptor
	AUDIO_DESCRIPTOR_CS_INTERFACE, //20
	AUDIO_DESCRIPTOR_CS_SUB_AC_HEADER,
	BYTE_SPLIT1(AUDIO_CLASS_SPEC_VER),  //1.0.0 or 2.0
	BYTE_SPLIT2(AUDIO_CLASS_SPEC_VER),
#ifdef ADC20
	0x08,							//bCategory  0x00 FUNCTION_SUBCLASS_UNDEFINED, 0x01 DESKTOP_SPEAKER ... 0x08 I/O_BOX
	BYTE_SPLIT1(AUDIO_CLASS_SPECIFIC_AC_TOT_LEN),   //Total number of bytes returned for the class-specific AudioControl interface descriptor.
	BYTE_SPLIT2(AUDIO_CLASS_SPECIFIC_AC_TOT_LEN),	 //-0x09 FU not needed !
	0x00,							//bmControls
#else
	BYTE_SPLIT1(AUDIO_CLASS_SPECIFIC_AC_TOT_LEN),   //Total number of bytes returned for the class-specific AudioControl interface descriptor.
	BYTE_SPLIT2(AUDIO_CLASS_SPECIFIC_AC_TOT_LEN),	//-0x09 2nd FU not needed !
#ifdef ONLYSpeakerCFG//JJ
	0x01,                       //1 stream INTF
	AUDIO_STREAM_INTF_NUM,      //INTF1
#else
 #ifdef ONLYMicCFG//JJ
	0x01,                       //1 stream INTF
	AUDIO_STREAM_INTF_NUM2,      //INTF2
 #else
	0x02,                       //2 stream INTFs number
	AUDIO_STREAM_INTF_NUM,      //INTF1
	AUDIO_STREAM_INTF_NUM2,     //JJ INTF2...
 #endif
#endif //ONLYSpeakerCFG
#endif //ADC20

#ifdef ADC20
	//Clock Source JJ+
	AUDIO_CTL_CSD_LEN,           //l=8, same for ADC20 Table 4-6: Clock Source Descriptor
	AUDIO_DESCRIPTOR_CS_INTERFACE, //20
	AUDIO_DESCRIPTOR_CS_SUB_AC_CLOCK_SOURCE,
	AUDIO_CLKS_ID,				//bClockID = 07, the Clock Source Entity within the audio function.
	0x03,						//bmAttributes 00 external, 01 internal fixed, 10 internal variable, 11 Internal programmable Clock
	0x07,						//bmControls
	0x00,						//bAssocTerminal
	0x00,                   	//iClockSource, Index of a string descriptor

 #ifndef CXDDISABLE
	//Clock Selector JJ+ 
	//!!! For Andoid, cannot enable CXD segment !!! otherwise not go to get range of Sampling Frequency.
	AUDIO_CTL_CXD_LEN,           //l=8, same for ADC20 Table 4-7: Clock Selector Descriptor
	AUDIO_DESCRIPTOR_CS_INTERFACE, //20
	AUDIO_DESCRIPTOR_CS_SUB_AC_CLOCK_SELECTOR,
	AUDIO_CLK_ID,				//bClockID = 08, the Clock Selector Entity within the audio function.
	0x01,						//bNrInPins
	AUDIO_CLKS_ID,				//baCSourceID(1), ID 07 of the Clock Entity to which the Clock Input Pin of this Clock Selector Entity is connected.
	0x03,						//bmControls
	0x00,                   	//iClockSelector, Index of a string descriptor
 #endif
#endif //ADC20

#ifndef ONLYMicCFG
//----- for down pipe Terms (PC -- SPEAKER)			IT1 -- FU2 -- OT3
	//input terminal    audio10 Table 4-3: Input Terminal Descriptor
	AUDIO_INPUT_TERMINAL_LEN,       //l=0x0c, 0x11 for ADC20
	AUDIO_DESCRIPTOR_CS_INTERFACE,
	AUDIO_DESCRIPTOR_CS_SUB_AC_INPUT_TERMINAL, //30
	AUDIO_INPUT_TERM_ID,            //ID = 0x01
	BYTE_SPLIT1(AUDIO_USB_TERM_USB_STREAM),
	BYTE_SPLIT2(AUDIO_USB_TERM_USB_STREAM),
	0x00, //jj- AUDIO_OUTPUT_TERM_ID,   //output terminal ID = 0x03 assoc
 #ifdef ADC20
	#ifdef CXDDISABLE
	AUDIO_CLKS_ID,			//bCSourceID, ID of the Clock Entity to which this Input Terminal is connected.
	#else
	AUDIO_CLK_ID,			//bCSourceID, ID of the Clock Entity to which this Input Terminal is connected.
	#endif
	AUDIO_CHANNELS,			//bNrChannels
	BYTE_SPLIT1(0x00000003),//bmChannelConfig	FL+FR
	BYTE_SPLIT2(0x00000003),
	BYTE_SPLIT3(0x00000003),
	BYTE_SPLIT4(0x00000003),
	0x00,					//iChannelNames
	BYTE_SPLIT1(0x0000),	//bmControls
	BYTE_SPLIT2(0x0000),	//bmControls
 #else
	AUDIO_CHANNELS,
	BYTE_SPLIT1(AUDIO_CHANNEL_CONFIG),
	BYTE_SPLIT2(AUDIO_CHANNEL_CONFIG),
	0x00,                   //Index of a string descriptor, first logic channel
 #endif //ADC20
	0x00,                   //Index of a string descriptor, IT

	//feature unit
	AUDIO_FEATURE_UNIT_LEN, //40 l=9, 0x12 for ADC20
	AUDIO_DESCRIPTOR_CS_INTERFACE,
	AUDIO_DESCRIPTOR_CS_SUB_AC_FEATURE_UNIT,
	AUDIO_FEATURE_UNIT_ID,          //ID = 0x02
	AUDIO_INPUT_TERM_ID,    //bSourceID ID = 0x01
#ifdef ADC20
	BYTE_SPLIT1(0x0000000f),		//bmaControls(0) 4 bytes master channel 0, Mute Control + Volume Control
	BYTE_SPLIT2(0x0000000f),
	BYTE_SPLIT3(0x0000000f),
	BYTE_SPLIT4(0x0000000f),
	BYTE_SPLIT1(0x00000000),		//bmaControls(0) 4 bytes logical channel 1, Mute Control + Volume Control
	BYTE_SPLIT2(0x00000000),
	BYTE_SPLIT3(0x00000000),
	BYTE_SPLIT4(0x00000000),
	BYTE_SPLIT1(0x00000000),		//bmaControls(0) 4 bytes logical channel 2, Mute Control + Volume Control
	BYTE_SPLIT2(0x00000000),
	BYTE_SPLIT3(0x00000000),
	BYTE_SPLIT4(0x00000000),
#else

	1,/*FEATURE_UNIT_CTL_SIZE,*/
	0x03,//0x02,//bmaControls[0]
	#if (AUDIO_CHANNELS == 2)
	0,	//0,//bmaControls[1]
	0,	//0,//bmaControls[2]
	#elif (AUDIO_CHANNELS == 6)
	0,	//0,//bmaControls[1]
	0,	//0,//bmaControls[2]
	0,	//0,//bmaControls[3]
	0,	//0,//bmaControls[4]
	0,	//0,//bmaControls[5]
	0,	//0,//bmaControls[6]
	#elif (AUDIO_CHANNELS == 8)
	0,	//0,//bmaControls[1]
	0,	//0,//bmaControls[2]
	0,	//0,//bmaControls[3]
	0,	//0,//bmaControls[4]
	0,	//0,//bmaControls[5]
	0,	//0,//bmaControls[6]
	0,	//0,//bmaControls[7]
	0,	//0,//bmaControls[8]
	#endif

	#ifndef	VOLUME_MASTER_CHAN_ONLY
	BYTE_SPLIT1(FU_BMA_CTL1),
	BYTE_SPLIT2(FU_BMA_CTL1),
	BYTE_SPLIT1(FU_BMA_CTL2), //50
	BYTE_SPLIT2(FU_BMA_CTL2),
	#endif
#endif //ADC20
	0x00,   //Index of a string descriptor, describing this Feature Unit

	//output terminal   audio10 Table 4-4: Output Terminal Descriptor
	AUDIO_OUTPUT_TERMINAL_LEN,  //l=9, 0x0c for ADC20
	AUDIO_DESCRIPTOR_CS_INTERFACE,
	AUDIO_DESCRIPTOR_CS_SUB_AC_OUTPUT_TERMINAL,
	AUDIO_OUTPUT_TERM_ID,           //ID = 0x03
#if 1
	BYTE_SPLIT1(AUDIO_OUTPUT_TERM_DESKTOP_SPEAKER),
	BYTE_SPLIT2(AUDIO_OUTPUT_TERM_DESKTOP_SPEAKER),
#else
	BYTE_SPLIT1(AUDIO_OUTPUT_TERM_HEADPHONE),
	BYTE_SPLIT2(AUDIO_OUTPUT_TERM_HEADPHONE),
#endif
	0x00, //jj- AUDIO_INPUT_TERM_ID,    //input terminal ID = 0x01 assoc
	AUDIO_INPUT_TERM_ID,//AUDIO_INPUT_TERM_ID,//AUDIO_FEATURE_UNIT_ID, //60 bSourceID this Term is connected
#ifdef ADC20
	#ifdef CXDDISABLE
	AUDIO_CLKS_ID,			//bCSourceID, ID of the Clock Entity to which this Output Terminal is connected.
	#else
	AUDIO_CLK_ID,			//bCSourceID, ID of the Clock Entity to which this Output Terminal is connected.
	#endif
	BYTE_SPLIT1(0x0000),	//bmControls
	BYTE_SPLIT2(0x0000),	//bmControls
#endif //ADC20
	0x00,                   //Index of a string descriptor
#endif	//ONLYMicCFG

	//MIXER_UNIT (0x04)     12, (4, 11) len = 13, 0x01 IT1,0x04 IT4, x02 Chs,0x03 , Chs config
//	0x0D ,0x24 ,0x04 ,0x0C ,0x02 ,0x01 ,0x04 ,0x02 ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,


#ifndef ONLYSpeakerCFG//JJ
//----- for up pipe Terms (MIC -- HOST)			IT4 --     -- OT6
	//input terminal  --> MIC  audio10 Table 4-3: Input Terminal Descriptor
	AUDIO_INPUT_TERMINAL_LEN,       //l=0x0c, 0x11 for ADC20
	AUDIO_DESCRIPTOR_CS_INTERFACE,
	AUDIO_DESCRIPTOR_CS_SUB_AC_INPUT_TERMINAL, //30
	AUDIO_INPUTMIC_TERM_ID,            //ID = 4
	BYTE_SPLIT1(AUDIO_INPUT_TERM_MICROPHONE),    //Terminal is Microphone.     //JJ for mic
	BYTE_SPLIT2(AUDIO_INPUT_TERM_MICROPHONE),
	0x00, //jj- AUDIO_OUTPUTMIC_TERM_ID,   //output terminal ID = 6 assoc
#ifdef ADC20
	#ifdef CXDDISABLE
	AUDIO_CLKS_ID,			//bCSourceID, ID of the Clock Entity to which this Output Terminal is connected.
	#else
	AUDIO_CLK_ID,			//bCSourceID
	#endif
	AUDIOIN_CHANNELS,			//bNrChannels
	BYTE_SPLIT1(0x00000003),//bmChannelConfig	FL+FR
	BYTE_SPLIT2(0x00000003),
	BYTE_SPLIT3(0x00000003),
	BYTE_SPLIT4(0x00000003),
	0x00,					//iChannelNames
	BYTE_SPLIT1(0x0000),	//bmControls
	BYTE_SPLIT2(0x0000),	//bmControls
#else
	AUDIOIN_CHANNELS,
	BYTE_SPLIT1(AUDIOIN_CHANNEL_CONFIG),
	BYTE_SPLIT2(AUDIOIN_CHANNEL_CONFIG),
	0x00,                   //Index of a string descriptor, first logic channel
#endif //ADC20
	0x00,                   //Index of a string descriptor, IT
	
	//output terminal  --> USB IN stream 
	AUDIO_OUTPUT_TERMINAL_LEN,  //l=9, 0x0c for ADC20
	AUDIO_DESCRIPTOR_CS_INTERFACE,
	AUDIO_DESCRIPTOR_CS_SUB_AC_OUTPUT_TERMINAL,
	AUDIO_OUTPUTMIC_TERM_ID,           //ID = 6, to the Ep!
	BYTE_SPLIT1(AUDIO_USB_TERM_USB_STREAM),
	BYTE_SPLIT2(AUDIO_USB_TERM_USB_STREAM),
	0,//jj AUDIO_INPUTMIC_TERM_ID,    //input terminal ID = 4 assoc
	AUDIO_INPUTMIC_TERM_ID,//AUDIO_FEATUREMIC_UNIT_ID, //60 bSourceID ID = 5, this Term is connected
#ifdef ADC20
	#ifdef CXDDISABLE
	AUDIO_CLKS_ID,			//bCSourceID, ID of the Clock Entity to which this Output Terminal is connected.
	#else
	AUDIO_CLK_ID,			//bCSourceID
	#endif
	BYTE_SPLIT1(0x0000),	//bmControls
	BYTE_SPLIT2(0x0000),	//bmControls
#endif //ADC20
	0x00,                   //Index of a string descriptor
#endif //ONLYSpeakerCFG


#ifndef ONLYMicCFG
//interface 01 - audioStream 	======================================================================
	//audio10 Table 4-18: Standard AS Interface Descriptor
	//Down Pipe Interface --> Term --> Ep !!! for USB OUT Speaker 			len=9+9+7+11+9+7
	//alt intf1/0	-	zero bandwidth
	//{ 09 04 01 00 00 01 02 00 00 }
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE,
	AUDIO_STREAM_INTF_NUM,                          // interface 1
	AUDIO_STRAM_ZEROBAND_ALT_SET,
	0x00,
	INTERFACE_CLASS_AUDIO,
	AUDIO_INTF_SUBCLASS_STREAM,
	IP_VERSION,							//bInterfaceProtocol
	0x00, //70

	//alt intf1/1 -	non-zero bandwidth,										AUDIO_CHANNELS6
	//{ 09 04 01 01 02 01 02 00 00 }
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE,
	AUDIO_STREAM_INTF_NUM,                          // interface <1> for OUT
	AUDIO_STRAM_NON_ZEROBAND_ALT_SET,
#ifndef SECOND_OUTEP
 #ifdef USBDEV_ISOIN_ASYNC
	0x02,                               //1 endpoint + 2nd FB ep
 #else
	0x01,                               //1 endpoint
 #endif
#else
 #ifdef USBDEV_ISOIN_ASYNC
	0x02,                               //1 endpoint + 2nd FB ep
 #else
	0x01,                               //1 endpoint
 #endif
#endif //SECOND_OUTEP
	INTERFACE_CLASS_AUDIO,
	AUDIO_INTF_SUBCLASS_STREAM,
	IP_VERSION,							//bInterfaceProtocol
	0x00,

    	//general intf	//AS CS Intf
#ifdef ADC20
    	AUDIO_AS_GENERAL_INTF_LEN, //80, l=7, 0x10 for ADC20
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL,
    	AUDIO_INPUT_TERM_ID,//AUDIO_OUTPUT_TERM_ID,         //  Spk -- TERM ID=0x01!!!
    	0x00,				//bmControls
    	(U8)AUDIO_FORMAT_TYPE_I_PCM,//bFormatType
    	BYTE_SPLIT1(0x00000001),//bmFormats, Frmts20: A.2.1 Audio Data Format Type I Bit Allocations
    	BYTE_SPLIT2(0x00000001),//bmFormats  D0 PCM, D1 PCM8 ...
    	BYTE_SPLIT3(0x00000001),//bmFormats
    	BYTE_SPLIT4(0x00000001),//bmFormats
    	0x02,	//bNrChannels
    	BYTE_SPLIT1(0x00000003),//bmChannelConfig	FL+FR
    	BYTE_SPLIT2(0x00000003),
    	BYTE_SPLIT3(0x00000003),
    	BYTE_SPLIT4(0x00000003),
    	0x00,	//iChannelNames
#else
		//{ 07 24 01 01 01 01 00 }
    	AUDIO_AS_GENERAL_INTF_LEN, //80, l=7
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL,
    	AUDIO_INPUT_TERM_ID,//AUDIO_OUTPUT_TERM_ID,         //  Spk -- TERM ID=0x01!!! Terminal the endpoint connected.
    	0x01,	//? frame delay
    	BYTE_SPLIT1(AUDIO_FORMAT_TYPE_I_PCM),
    	BYTE_SPLIT2(AUDIO_FORMAT_TYPE_I_PCM),
#endif //ADC20

    	//type I format, Frmts20 Table 2-2: Type I Format Type Descriptor
#ifdef ADC20
    	AUDIO_TYPE_I_FORMAT_LEN,            //l=11 = 8+(ns*3), 6 for ADC20
    	AUDIO_DESCRIPTOR_CS_INTERFACE,  //140
    	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE,
    	AUDIO_FORMAT_TYPE_I,
    	AUDIO_SUB_FRAME_SIZE,	//bSubslotSize, 2 bytes
    	AUDIO_BIT_RESOLUTION,	//bBitResolution, 16 bits
#else
		//{ 0B 24 02 01 02 02 10 01 80 BB 00 }
    	AUDIO_TYPE_I_FORMAT_LEN,
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE,
    	AUDIO_FORMAT_TYPE_I, //90
    	AUDIO_CHANNELS6,									//<<<<<<<===============
    	AUDIO_SUB_FRAME_SIZE,
    	AUDIO_BIT_RESOLUTION,
    	0x01,
    	BYTE_SPLIT1(AUDIO_SAMPLE_FREQ), //95
    	BYTE_SPLIT2(AUDIO_SAMPLE_FREQ),
    	BYTE_SPLIT3(AUDIO_SAMPLE_FREQ),
#endif //ADC20

    	//iso endpoint EP1, Table 4-20: Standard AS Isochronous Audio Data Endpoint
    	//{ 09 05 01 05 C8 C8 01 00 83 }
    	AUDIO_CLASS_STD_ENDP_LEN,   //l=9, 7 for ADC20
    	DESCRIPTOR_TYPE_ENDPOINT,
    	ISO_OUT_ENDP_ADDR,         	//100   <0x01> EP1 for USB OUT -- AUDIO_OUTPUT_TERM_ID
//jj    	0x05,						//bmAttributes: 00.01.01 DataEp.Async.Iso	//ISO_STD_OUT_ENDP_ATTR,		//FIXME ?
    	0x09,						//bmAttributes: 00.10.01 10 = Adaptive, 01 = Isochronous	//ISO_STD_OUT_ENDP_ATTR,		//FIXME ?
    	BYTE_SPLIT1(DISO_MAX_PKT_SIZE6),//0X00C8),		//<<<<<<<=============== //BYTE_SPLIT1(ISO_STD_OUT_ENDP_MAXPKT),   //0x400 Max Packet Size
    	BYTE_SPLIT2(DISO_MAX_PKT_SIZE6),//0X00C8), //BYTE_SPLIT2(ISO_STD_OUT_ENDP_MAXPKT),
    	0x01,						//bInterval: Interval for polling endpoint for data transfers
    	0x00,                       //bInterval
#ifndef ADC20
	#ifdef USBDEV_ISOIN_ASYNC
    	0x83,						//bSyncAddress    	
	#else
    	0x00,
	#endif
#endif //ADC20

    	//class-iso endpoint, Table 4-21: Class-Specific AS Isochronous Audio Data Endpoint
    	//{ 07 25 01 00 02 08 00 }
    	AUDIO_CLASS_ISO_ENDP_LEN,   //l=7, 8 for ADC20
    	AUDIO_DESCRIPTOR_CS_ENDPOINT,
    	AUDIO_DESCRIPTOR_CS_SUB_ENDP_GENERAL,
    	ISO_CS_OUT_ENDP_ATTR,           //110
#ifdef ADC20
    	0x00,					//bmControls
#endif //ADC20
    	0x00,//0x02,				//bLockDelayUnits: only applicable for synchronous and adaptive! 0x00,Undefined	0x02, Decoded PCM samples
    	0x00,//BYTE_SPLIT1(0x0008),	//wLockDelay: BYTE_SPLIT1(0x0000)
    	0x00,//BYTE_SPLIT2(0x0008),	//BYTE_SPLIT2(0x0000),

#ifdef USBDEV_ISOIN_ASYNC //Table 4-22: Standard AS Isochronous Synch Endpoint
		//Standard AS ISO feedback EP Desc, 4.10.2 AS Isochronous Feedback Endpoint Descriptor
		//{ 09 05 83 01 03 00 01 05 00 }
    	AUDIO_CLASS_STD_ENDP_LEN,   //l=9
    	DESCRIPTOR_TYPE_ENDPOINT,
    	0x83,					//bEndpointAddress
    	0x01,					//bmAttributes: D3..2=00, No Sync, D1..0=01 Iso	//0x11,
    	BYTE_SPLIT1(0x0003),   	//0x06 Max Packet Size
    	BYTE_SPLIT2(0x0003),
    	0x01,					//bInterval: Must be set to 1.
    	0x05,					//bRefresh: 2^5=32ms, Interval for polling Interrupt ep
    	0x00,					//bSynchAddress: Endpoint, Must be reset to zero.
#endif //USBDEV_ISOIN_ASYNC

#ifdef ADC20
		//Standard AS ISO feedback EP Desc, 4.10.2 AS Isochronous Feedback Endpoint Descriptor
    	AUDIO_AC_INT_ENDP_LEN,   //l=7
    	DESCRIPTOR_TYPE_ENDPOINT,
    	0x81,							//bEndpointAddress
    	0x11,	//D5..4=01 Feedback endpoint, No Sync, D1..0=01 Iso	//0x11,		//bmAttributes 11 = Interrupt
    	BYTE_SPLIT1(0x0004),   			//0x06 Max Packet Size
    	BYTE_SPLIT2(0x0004),
    	0x04,						//Interval for polling Interrupt ep
#endif //ADC20

#ifdef ALT2_INTF
	//alt intf1/2 -	non-zero bandwidth, 									AUDIO_CHANNELS2
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE,
	AUDIO_STREAM_INTF_NUM,                          // interface <1> for OUT
	AUDIO_STRAM_NON_ZEROBAND_ALT_SET2,
	/*
	0x01 => 1 endpoint
	0x02 => 1 endpoint + 1 feedback ep
	*/
	0x01,                               //1 endpoint
	INTERFACE_CLASS_AUDIO,
	AUDIO_INTF_SUBCLASS_STREAM,
	IP_VERSION,							//bInterfaceProtocol
	0x00,

    	//general intf	//AS CS Intf
    	AUDIO_AS_GENERAL_INTF_LEN, //80, l=7
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL,
    	AUDIO_INPUT_TERM_ID,//AUDIO_OUTPUT_TERM_ID,         //  Spk -- TERM ID=0x01!!!
    	0x01,	//? frame delay
    	BYTE_SPLIT1(AUDIO_FORMAT_TYPE_I_PCM),
    	BYTE_SPLIT2(AUDIO_FORMAT_TYPE_I_PCM),

    	//type I format, Frmts20 Table 2-2: Type I Format Type Descriptor
    	AUDIO_TYPE_I_FORMAT_LEN,
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE,
    	AUDIO_FORMAT_TYPE_I,
    	AUDIO_CHANNELS2,//AUDIO_CHANNELS,					//<<<<<<<===============
    	AUDIO_SUB_FRAME_SIZE,
    	AUDIO_BIT_RESOLUTION,
    	0x01,
    	BYTE_SPLIT1(AUDIO_SAMPLE_FREQ),
    	BYTE_SPLIT2(AUDIO_SAMPLE_FREQ),
    	BYTE_SPLIT3(AUDIO_SAMPLE_FREQ),

    	//iso endpoint
    	AUDIO_CLASS_STD_ENDP_LEN,   //l=9, 7 for ADC20
    	DESCRIPTOR_TYPE_ENDPOINT,
    	ISO_OUT_ENDP_ADDR,              //<0x01> EP1 for USB OUT -- AUDIO_OUTPUT_TERM_ID
    	ISO_STD_OUT_ENDP_ATTR,		//FIXME ?
    	BYTE_SPLIT1(ISO_STD_OUT_ENDP_MAXPKT2),				//<<<<<<<===============
    	BYTE_SPLIT2(ISO_STD_OUT_ENDP_MAXPKT2),
    	0x01,						//Interval for polling endpoint for data transfers
#ifndef ADC20
    	0x00,                           
    	0x00,
#endif

    	//class-iso endpoint
    	AUDIO_CLASS_ISO_ENDP_LEN,   //l=7, 8 for ADC20
    	AUDIO_DESCRIPTOR_CS_ENDPOINT,
    	AUDIO_DESCRIPTOR_CS_SUB_ENDP_GENERAL,
    	ISO_CS_OUT_ENDP_ATTR,           
#ifdef ADC20
    	0x00,					//bmControls
#endif
    	0x02,					//0x00,Undefined	0x02, Decoded PCM samples		//bLockDelayUnits
    	BYTE_SPLIT1(0x0008),	//BYTE_SPLIT1(0x0000),	//wLockDelay
    	BYTE_SPLIT2(0x0008),	//BYTE_SPLIT2(0x0000),

#endif	//ALT2_INTF

#ifdef ALT3_INTF
	//alt intf1/3 -	non-zero bandwidth,										AUDIO_CHANNELS8
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE,
	AUDIO_STREAM_INTF_NUM,                          // interface <1> for OUT
	AUDIO_STRAM_NON_ZEROBAND_ALT_SET3,
	/*
	0x01 => 1 endpoint
	0x02 => 1 endpoint + 1 feedback ep
	*/
	0x01,                               //1 endpoint
	INTERFACE_CLASS_AUDIO,
	AUDIO_INTF_SUBCLASS_STREAM,
	IP_VERSION,							//bInterfaceProtocol
	0x00,

    	//general intf	//AS CS Intf
    	AUDIO_AS_GENERAL_INTF_LEN, //80, l=7
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL,
    	AUDIO_INPUT_TERM_ID,//AUDIO_OUTPUT_TERM_ID,         //  Spk -- TERM ID=0x01!!!
    	0x01,	//? frame delay
    	BYTE_SPLIT1(AUDIO_FORMAT_TYPE_I_PCM),
    	BYTE_SPLIT2(AUDIO_FORMAT_TYPE_I_PCM),

    	//type I format, Frmts20 Table 2-2: Type I Format Type Descriptor
    	AUDIO_TYPE_I_FORMAT_LEN,
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE,
    	AUDIO_FORMAT_TYPE_I,
    	AUDIO_CHANNELS8,									//<<<<<<<===============   ?????????????????????????????  8 Channels not work ?
    	AUDIO_SUB_FRAME_SIZE,
    	AUDIO_BIT_RESOLUTION,
    	0x01,
    	BYTE_SPLIT1(AUDIO_SAMPLE_FREQ),
    	BYTE_SPLIT2(AUDIO_SAMPLE_FREQ),
    	BYTE_SPLIT3(AUDIO_SAMPLE_FREQ),

    	//iso endpoint
    	AUDIO_CLASS_STD_ENDP_LEN,   //l=9, 7 for ADC20
    	DESCRIPTOR_TYPE_ENDPOINT,
    	ISO_OUT_ENDP_ADDR,              //<0x01> EP1 for USB OUT -- AUDIO_OUTPUT_TERM_ID
    	ISO_STD_OUT_ENDP_ATTR,		//FIXME ?
    	BYTE_SPLIT1(DISO_MAX_PKT_SIZE8),					//<<<<<<<===============   //锛燂紵锛� Just use 6Ch size instead of 8Ch !
    	BYTE_SPLIT2(DISO_MAX_PKT_SIZE8),
    	0x01,						//Interval for polling endpoint for data transfers
#ifndef ADC20
    	0x00,                           
    	0x00,
#endif

    	//class-iso endpoint
    	AUDIO_CLASS_ISO_ENDP_LEN,   //l=7, 8 for ADC20
    	AUDIO_DESCRIPTOR_CS_ENDPOINT,
    	AUDIO_DESCRIPTOR_CS_SUB_ENDP_GENERAL,
    	ISO_CS_OUT_ENDP_ATTR,           
#ifdef ADC20
    	0x00,					//bmControls
#endif
    	0x00,					//0x00,Undefined	0x02, Decoded PCM samples		//bLockDelayUnits
    	BYTE_SPLIT1(0x0000),	//BYTE_SPLIT1(0x0000),	//wLockDelay
    	BYTE_SPLIT2(0x0000),	//BYTE_SPLIT2(0x0000),

#endif	//ALT3_INTF

#ifdef ALT4_INTF
	//alt intf1/4 -	non-zero bandwidth,										AUDIO_CHANNELS4
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE,
	AUDIO_STREAM_INTF_NUM,                          // interface <1> for OUT
	AUDIO_STRAM_NON_ZEROBAND_ALT_SET4,
	/*
	0x01 => 1 endpoint
	0x02 => 1 endpoint + 1 feedback ep
	*/
	0x01,                               //1 endpoint
	INTERFACE_CLASS_AUDIO,
	AUDIO_INTF_SUBCLASS_STREAM,
	IP_VERSION,							//bInterfaceProtocol
	0x00,

    	//general intf	//AS CS Intf
    	AUDIO_AS_GENERAL_INTF_LEN, //80, l=7
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL,
    	AUDIO_INPUT_TERM_ID,//AUDIO_OUTPUT_TERM_ID,         //  Spk -- TERM ID=0x01!!!
    	0x01,	//? frame delay
    	BYTE_SPLIT1(AUDIO_FORMAT_TYPE_I_PCM),
    	BYTE_SPLIT2(AUDIO_FORMAT_TYPE_I_PCM),

    	//type I format, Frmts20 Table 2-2: Type I Format Type Descriptor
    	AUDIO_TYPE_I_FORMAT_LEN,
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE,
    	AUDIO_FORMAT_TYPE_I,
    	AUDIO_CHANNELS4,									//<<<<<<<===============
    	AUDIO_SUB_FRAME_SIZE,
    	AUDIO_BIT_RESOLUTION,
    	0x01,
    	BYTE_SPLIT1(AUDIO_SAMPLE_FREQ),
    	BYTE_SPLIT2(AUDIO_SAMPLE_FREQ),
    	BYTE_SPLIT3(AUDIO_SAMPLE_FREQ),

    	//iso endpoint
    	AUDIO_CLASS_STD_ENDP_LEN,   //l=9, 7 for ADC20
    	DESCRIPTOR_TYPE_ENDPOINT,
    	ISO_OUT_ENDP_ADDR,              //<0x01> EP1 for USB OUT -- AUDIO_OUTPUT_TERM_ID
    	ISO_STD_OUT_ENDP_ATTR,		//FIXME ?
    	BYTE_SPLIT1(DISO_MAX_PKT_SIZE4),					//<<<<<<<===============
    	BYTE_SPLIT2(DISO_MAX_PKT_SIZE4),
    	0x01,						//Interval for polling endpoint for data transfers
#ifndef ADC20
    	0x00,                           
    	0x00,
#endif

    	//class-iso endpoint
    	AUDIO_CLASS_ISO_ENDP_LEN,   //l=7, 8 for ADC20
    	AUDIO_DESCRIPTOR_CS_ENDPOINT,
    	AUDIO_DESCRIPTOR_CS_SUB_ENDP_GENERAL,
    	ISO_CS_OUT_ENDP_ATTR,           
#ifdef ADC20
    	0x00,					//bmControls
#endif
    	0x02,					//0x00,Undefined	0x02, Decoded PCM samples		//bLockDelayUnits
    	BYTE_SPLIT1(0x0008),	//BYTE_SPLIT1(0x0000),	//wLockDelay
    	BYTE_SPLIT2(0x0008),	//BYTE_SPLIT2(0x0000),

#endif	//ALT4_INTF






#ifdef SECOND_OUTEP
	//alt intf1/2 -	non-zero bandwidth
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE,
	AUDIO_STREAM_INTF_NUM,                          // interface <1> for OUT
	AUDIO_STRAM_NON_ZEROBAND_ALT_SET2,
	0x01,                               //1 endpoint
	INTERFACE_CLASS_AUDIO,
	AUDIO_INTF_SUBCLASS_STREAM,
	IP_VERSION,							//bInterfaceProtocol
	0x00,

    	//general intf	//AS CS Intf
#ifdef ADC20
    	AUDIO_AS_GENERAL_INTF_LEN, //80, l=7, 0x10 for ADC20
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL,
    	AUDIO_INPUT_TERM_ID,//AUDIO_OUTPUT_TERM_ID,         //  Spk -- TERM ID=0x01!!!
    	0x00,				//bmControls
    	(U8)AUDIO_FORMAT_TYPE_I_PCM,//bFormatType
    	BYTE_SPLIT1(0x00000001),//bmFormats, Frmts20: A.2.1 Audio Data Format Type I Bit Allocations
    	BYTE_SPLIT2(0x00000001),//bmFormats  D0 PCM, D1 PCM8 ...
    	BYTE_SPLIT3(0x00000001),//bmFormats
    	BYTE_SPLIT4(0x00000001),//bmFormats
    	0x02,	//bNrChannels
    	BYTE_SPLIT1(0x00000003),//bmChannelConfig	FL+FR
    	BYTE_SPLIT2(0x00000003),
    	BYTE_SPLIT3(0x00000003),
    	BYTE_SPLIT4(0x00000003),
    	0x00,	//iChannelNames
#else
    	AUDIO_AS_GENERAL_INTF_LEN, //80, l=7
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL,
    	AUDIO_INPUT_TERM_ID,//AUDIO_OUTPUT_TERM_ID,         //  Spk -- TERM ID=0x01!!!
    	0x01,	//? frame delay
    	BYTE_SPLIT1(AUDIO_FORMAT_TYPE_I_PCM),
    	BYTE_SPLIT2(AUDIO_FORMAT_TYPE_I_PCM),
#endif

    	//type I format, Frmts20 Table 2-2: Type I Format Type Descriptor
#ifdef ADC20
    	AUDIO_TYPE_I_FORMAT_LEN,            //l=11 = 8+(ns*3), 6 for ADC20
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE,
    	AUDIO_FORMAT_TYPE_I,
    	AUDIO_SUB_FRAME_SIZE,	//bSubslotSize, 2 bytes
    	AUDIO_BIT_RESOLUTION,	//bBitResolution, 16 bits
#else
    	AUDIO_TYPE_I_FORMAT_LEN,
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE,
    	AUDIO_FORMAT_TYPE_I,
    	AUDIO_CHANNELS,
    	AUDIO_SUB_FRAME_SIZE2,
    	AUDIO_BIT_RESOLUTION2,
    	0x01,
    	BYTE_SPLIT1(AUDIO_SAMPLE_FREQ2),
    	BYTE_SPLIT2(AUDIO_SAMPLE_FREQ2),
    	BYTE_SPLIT3(AUDIO_SAMPLE_FREQ2),
#endif

    	//iso endpoint
    	AUDIO_CLASS_STD_ENDP_LEN,   //l=9, 7 for ADC20
    	DESCRIPTOR_TYPE_ENDPOINT,
    	ISO_OUT_ENDP_ADDR,              //<0x01> EP1 for USB OUT -- AUDIO_OUTPUT_TERM_ID
    	ISO_STD_OUT_ENDP_ATTR,		//FIXME ?
    	BYTE_SPLIT1(ISO_STD_OUT_ENDP_MAXPKT),   //0x400 Max Packet Size
    	BYTE_SPLIT2(ISO_STD_OUT_ENDP_MAXPKT),
    	0x01,						//Interval for polling endpoint for data transfers
#ifndef ADC20
    	0x00,                           
    	0x00,
#endif

    	//class-iso endpoint
    	AUDIO_CLASS_ISO_ENDP_LEN,   //l=7, 8 for ADC20
    	AUDIO_DESCRIPTOR_CS_ENDPOINT,
    	AUDIO_DESCRIPTOR_CS_SUB_ENDP_GENERAL,
    	ISO_CS_OUT_ENDP_ATTR,           
#ifdef ADC20
    	0x00,					//bmControls
#endif
    	0x02,					//0x00,Undefined	0x02, Decoded PCM samples		//bLockDelayUnits
    	BYTE_SPLIT1(0x0008),	//BYTE_SPLIT1(0x0000),	//wLockDelay
    	BYTE_SPLIT2(0x0008),	//BYTE_SPLIT2(0x0000),

#ifdef ADC20
		//Standard AS ISO feedback EP Desc, 4.10.2 AS Isochronous Feedback Endpoint Descriptor
    	AUDIO_AC_INT_ENDP_LEN,   //l=7
    	DESCRIPTOR_TYPE_ENDPOINT,
    	0x81,							//bEndpointAddress
    	0x11,	//D5..4=01 Feedback endpoint, No Sync, D1..0=01 Iso	//0x11,		//bmAttributes 11 = Interrupt
    	BYTE_SPLIT1(0x0004),   			//0x06 Max Packet Size
    	BYTE_SPLIT2(0x0004),
    	0x04,						//Interval for polling Interrupt ep
#endif	
#endif	//SECOND_OUTEP

#endif	//ONLYMicCFG


#ifndef ONLYSpeakerCFG //JJ+ add another IF2 for USB in ...
//interface 02 - audioStream 	======================================================================
	//Up Pipe Interface --> Term --> Host !!! USB In Mic						len=9+9+7+9+7
	//alt intf0	-	zero bandwidth
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE,      //115
	AUDIO_STREAM_INTF_NUM2,                         // interface 2
	AUDIO_STRAM_ZEROBAND_ALT_SET,
	0x00,                               //no endpoint
	INTERFACE_CLASS_AUDIO,
	AUDIO_INTF_SUBCLASS_STREAM,     //120
	IP_VERSION,							//bInterfaceProtocol
	0x00, //70

	//alt intf1 -	non-zero bandwidth
//{ 09 04 02 01 01 01 02 00 00 }
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE,
	AUDIO_STREAM_INTF_NUM2,                         // interface 2
	AUDIO_STRAM_NON_ZEROBAND_ALT_SET,
	0x01,                               //1 endpoint
	INTERFACE_CLASS_AUDIO,
	AUDIO_INTF_SUBCLASS_STREAM,
	IP_VERSION,							//bInterfaceProtocol
	0x00,

    	//general intf  //C.3.4.2.1.2 Class-specific AS Interface Descriptor
#ifdef ADC20
    	AUDIO_AS_GENERAL_INTF_LEN, //80, l=7, 0x10 for ADC20
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL,
    	AUDIO_OUTPUTMIC_TERM_ID,               // mic -- TERM ID=0x06!!! Linked to USB Stream
    	0x00,				//bmControls D1..0: Active Alternate Setting, D3..2: Valid Alternate Settings
    	(U8)AUDIO_FORMAT_TYPE_I_PCM,//bFormatType
    	BYTE_SPLIT1(0x00000001),//bmFormats, Frmts20: A.2.1 Audio Data Format Type I Bit Allocations
    	BYTE_SPLIT2(0x00000001),//bmFormats
    	BYTE_SPLIT3(0x00000001),//bmFormats
    	BYTE_SPLIT4(0x00000001),//bmFormats
    	0x02,	//bNrChannels
    	BYTE_SPLIT1(0x00000003),//bmChannelConfig	FL+FR
    	BYTE_SPLIT2(0x00000003),
    	BYTE_SPLIT3(0x00000003),
    	BYTE_SPLIT4(0x00000003),
    	0x00,	//iChannelNames
#else
		//{ 07 24 01 06 01 01 00 }
    	AUDIO_AS_GENERAL_INTF_LEN,          //l=7
    	AUDIO_DESCRIPTOR_CS_INTERFACE,
    	AUDIO_DESCRIPTOR_CS_SUB_AS_GENERAL,
    	AUDIO_OUTPUTMIC_TERM_ID,               // mic -- TERM ID=0x06!!! Linked to USB Stream
//???  	AUDIO_INPUTMIC_TERM_ID,	//AUDIO_OUTPUTMIC_TERM_ID,               // mic -- TERM ID=0x06!!! Linked to USB Stream
    	0x01,	//? frame delay
    	BYTE_SPLIT1(AUDIO_FORMAT_TYPE_I_PCM),
    	BYTE_SPLIT2(AUDIO_FORMAT_TYPE_I_PCM),
#endif

    	//type I format C.3.4.2.1.3 Type I Format Type Descriptor
#ifdef ADC20
    	AUDIO_TYPE_I_FORMAT_LEN,            //l=11 = 8+(ns*3), 6 for ADC20
    	AUDIO_DESCRIPTOR_CS_INTERFACE,  //140
    	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE,
    	AUDIO_FORMAT_TYPE_I,
    	AUDIO_SUB_FRAME_SIZE,	//bSubslotSize, 2 bytes
    	AUDIO_BIT_RESOLUTION,	//bBitResolution
#else
		//{ 0B 24 02 01 02 02 10 01 80 BB 00 }
    	AUDIO_TYPE_I_FORMAT_LEN,            //l=11 = 8+(ns*3)
    	AUDIO_DESCRIPTOR_CS_INTERFACE,  //140
    	AUDIO_DESCRIPTOR_CS_SUB_AS_FORMAT_TYPE,
    	AUDIO_FORMAT_TYPE_I,
    	AUDIOIN_CHANNELS,
    	AUDIOIN_SUB_FRAME_SIZE,
    	AUDIOIN_BIT_RESOLUTION,
    	0x01,                   //One sampling frequency.
    	BYTE_SPLIT1(AUDIOIN_SAMPLE_FREQ),
    	BYTE_SPLIT2(AUDIOIN_SAMPLE_FREQ),
    	BYTE_SPLIT3(AUDIOIN_SAMPLE_FREQ),
#endif

    	//iso endpoint
		//{ 09 05 82 09 80 01 01 00 00 }
    	AUDIO_CLASS_STD_ENDP_LEN,       //150, l=9, 7 for ADC20
    	DESCRIPTOR_TYPE_ENDPOINT,
    	ISO_IN_ENDP_ADDR,           //JJ    <0x82> EP2 for IN -- AUDIO_INPUTMIC_TERM_ID
    	0x05,						//00.01.01 DataEp.Async.Iso	//ISO_STD_IN_ENDP_ATTR,		//D5..4: 10 = Implicit feedback Data,D3..2: 11 = Synchronous, D1..0: 01 = Isochronous
    	BYTE_SPLIT1(ISO_STD_IN_ENDP_MAXPKT),	//384 (x180) Max Packet Size
    	BYTE_SPLIT2(ISO_STD_IN_ENDP_MAXPKT),
    	0x01,						//Interval for polling endpoint for data transfers
#ifndef ADC20
    	0x00,
    	0x00,
#endif

    	//class-iso endpoint
    	//{ 07 25 01 00 02 08 00 }
    	AUDIO_CLASS_ISO_ENDP_LEN,           //l=7, l=8 for ADC20
    	AUDIO_DESCRIPTOR_CS_ENDPOINT,   //160
    	AUDIO_DESCRIPTOR_CS_SUB_ENDP_GENERAL,
    	ISO_CS_OUT_ENDP_ATTR,   //D7: MaxPacketsOnly, No sampling frequency control, 
#ifdef ADC20
		0x00,					//bmControls, no pitch control.
#endif
    	0x00,//0x02,					//bLockDelayUnits, 2: Decoded PCM samples
    	0x00,//jj- BYTE_SPLIT1(0x0008),	//BYTE_SPLIT1(0x0008),	//wLockDelay
    	0x00,//jj- BYTE_SPLIT2(0x0008),	//BYTE_SPLIT2(0x0008),	//165

#endif// #ifndef ONLYSpeakerCFG //JJ+	

#ifdef HIDFUNC
#if 0
//...................HID descriptors of Wecall len=9+9+7 = 0x19
//interface 03 - HID 			======================================================================
	//? OUT or Set_Report() with Control Ep ?
	//IF 03 HID Intf Descriptor, Alt 0, 1 Ep <0x83>, 3 HID Class code, 0 No subclass Alt0 ------
						//0x01 Boot Intf subc, 0x01 Keyboard protocol*/
#ifdef HIDKEY
	//Keyboard 0x09,0x04,0x03,0x00,0x01,0x03,0x01,0x01,0x00,  1 Ep ! Class code (HID code=3)! bInterfaceSubClass=0, bInterfaceProtocol=1 None
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE, 0x03,0x00,0x01,0x03,0x00,0x00,0x00,	//HID headset

	//DESCRIPTOR_TYPE_HID 0x21, HID Descriptor 1.11, 00 country, 01 Number of HID class descriptors,
	//0x21 HID Desc, 0111 Specification release number, 0x00country, 01#of HID class descriptors, 0x22 Report descriptor type, 0017/45 sizeof(HIDReportDescriptor) Total length of Report descriptor
	0x09,0x21,0x11,0x01,0x00,0x01,0x22,sizeof(HIDReportDescriptor),0x00,//0x0045, len of report desc
	
	//Ep Descriptor IN Ep <0x82> <0x81> addr, 03 Interrupt 0010 Maximum packet size 01/0A Interval
	0x07,0x05,0x82,0x03,0x40,0x00,0x0A,

#else	//HID data
	//Keyboard { 0x09 0x04 0x03 0x00 0x02 0x03 0x01 0x01 0x00 }  2 Eps! Class code (HID code=0x03)! bInterfaceSubClass=0 No subclass 1 Boot Interface subclass, bInterfaceProtocol=0 None 1 Keyboard 2 Mouse
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE, 0x03,0x00,0x02,0x03,0x00,0x00,eSTR_INDEX_HID_NUM /*0x00*/,	//HID headset

	//DESCRIPTOR_TYPE_HID 0x21, HID Descriptor 1.11, 00 country, 01 Number of HID class descriptors,
	//0x21 HID Desc, 0111 Specification release number, 0x00country,01#of HID class descriptors, 0x22 Report descriptor type, 0017/45 sizeof(HIDReportDescriptor) Total length of Report descriptor
	0x09,0x21,0x11,0x01,0x00,0x01,0x22,sizeof(HIDReportDescriptor),0x00,//0x0045, len of report desc
	
	//Ep Descriptor IN Ep <0x82> <0x81> addr, 03 Interrupt 0010 Maximum packet size 01/0A Interval
	0x07,0x05,0x82,0x03,0x40,0x00,0x0A,

	//EP <0x85> addr, Out Ep <0x02> Interrupt 0010 Maximum packet size 01 Interval
	0x07,0x05,0x02,0x03,0x40,0x00,0x01,
#endif

#else
//...................HID descriptors of Wecall len=9+9+7 = 0x19
//interface 03 - HID 			======================================================================
	//? OUT or Set_Report() with Control Ep ?
	//IF 03 HID Intf Descriptor, Alt 0, 1 Ep <0x83>, 3 HID Class code, 0 No subclass Alt0 ------
						//0x01 Boot Intf subc, 0x01 Keyboard protocol*/
	//Consumer Keyboard 0x09,0x04,0x03,0x00,0x01,0x03,0x01,0x01,0x00,  1 Ep ! Class code (HID code=3)! bInterfaceSubClass=0, bInterfaceProtocol=1 None
#ifdef DATAonCONTROLPIPE	//Data Out on Control pipe
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE, 0x03,0x00,0x01,0x03,0x00,0x00,0x00,	//HID headset

	//DESCRIPTOR_TYPE_HID 0x21, HID Descriptor 1.11, 00 country, 01 Number of HID class descriptors,
	//0x21 HID Desc, 0111 Specification release number, 0x00country, 01#of HID class descriptors, 0x22 Report descriptor type, 0017/45 sizeof(HIDReportDescriptor) Total length of Report descriptor
	0x09,0x21,0x11,0x01,0x00,0x01,0x22,sizeof(HIDReportDescriptor),0x00,//0x0045, len of report desc
	
	//Ep Descriptor IN Ep <0x82> <0x81> addr, 03 Interrupt 0010 Maximum packet size 01/0A Interval
	0x07,0x05,0x82,0x03,0x40,0x00,0x0A,
#else //2 Eps
	USB_INTERFACE_DESCRIPTOR_LENGTH,    //l=9
	DESCRIPTOR_TYPE_INTERFACE, 0x03,0x00,0x02,0x03,0x00,0x00,0x00,	//HID headset

	//DESCRIPTOR_TYPE_HID 0x21, HID Descriptor 1.11, 00 country, 01 Number of HID class descriptors,
	//0x21 HID Desc, 0111 Specification release number, 0x00country, 01#of HID class descriptors, 0x22 Report descriptor type, 0017/45 sizeof(HIDReportDescriptor) Total length of Report descriptor
	0x09,0x21,0x11,0x01,0x00,0x01,0x22,sizeof(HIDReportDescriptor),0x00,//0x0045, len of report desc
	
	//Ep Descriptor IN Ep <0x82> <0x81> addr, 03 Interrupt 0040 Maximum packet size 01/0A Interval
	0x07,0x05,0x82,0x03,0x40,0x00,0x0A,

	//EP <0x85> addr, Out Ep <0x02>, 03 Interrupt 0040 Maximum packet size 01 Interval
	0x07,0x05,0x02,0x03,0x40,0x00,0x0A,
#endif
#endif

#endif //HIDFUNC


#ifdef DFUFUNC
//interface 04 - DFU 	       ======================================================================
	0x09,    //l=9
	DESCRIPTOR_TYPE_INTERFACE,      //115
	0x03,                         // interface 03
	AUDIO_STRAM_ZEROBAND_ALT_SET,
	0x00,                               //no endpoint
	0xFE,							//bInterfaceClass 		App specific
	0x01,     						//bInterfaceSubclass	DFU
	0x01,							//bInterfaceProtocol	01 Runtime
	0x00, //70

	//DFU Functional Descriptor
	0x09,    //l=9
	0x21,      //115
	0x07,							//bmAttributes
	BYTE_SPLIT1(0x01),     			//wDetachTimeOut
	BYTE_SPLIT2(0x01),
	BYTE_SPLIT1(0x40),				//wTransferSize
	BYTE_SPLIT2(0x40),
	BYTE_SPLIT1(0x110), 			//bcdDFUVersion
	BYTE_SPLIT2(0x110),
#endif //DFUFUNC


#ifdef CDCFUNC
//interface 05 - CDC 	       ====================================================================== 9+5+5+4+5+7 +9+7+7 = (58)
//	Intf5/0, Intf6/0
    0x09,                   //bLength(9);
    0x04,                   //bDescriptorType(Interface);
    0x05,                   //bInterfaceNumber(5);
    0x00,                   //bAlternateSetting(0);
    0x01,                   //bNumEndpoints(1);
    0x02,                   //bInterfaceClass(Communication Interface Class);
    0x02,                   //bInterfaceSubClass(Abstract Control Model);
    0x01,                   //bInterfaceProtocol(Common AT commands);
    0x00,                   //iInterface(0);

    0x05,                   //bLength(5);
    0x24,                   //bDescriptorType(CS_INTERFACE);
    0x00,                   //bDescriptorSubtype(Header Functional Descriptor);
    0x10,0x01,              //bcdCDC(1.10);

    0x05,                   //bLength(5);
    0x24,                   //bDescriptorType(CS_INTERFACE);
    0x01,                   //bDescriptorSubtype(Call Management Functional Descriptor);
    0x00,                   //bmCapabilities(Device does not handles call management itself);
    0x01,                   //bDataInterface(1);

    0x04,                   //bLength(4);
    0x24,                   //bDescriptorType(CS_INTERFACE);
    0x02,                   //bDescriptorSubtype(Abstract Control Management Functional Descriptor);
    0x02,                   //bmCapabilities(Set/Get_Line_Coding,Serial_State,Set_Control_Line_State);

    0x05,                   //bLength(5);
    0x24,                   //bDescriptorType(CS_INTERFACE);
    0x06,                   //bDescriptorSubtype(Union Functional descriptor);
    0x00,                   //bMasterInterface(0);
    0x01,                   //bSlaveInterface0(1);

    0x07,                   //bLength(7);
    0x05,                   //bDescriptorType(Endpoint);
    0x83,                   //bEndpointAddress(EndPoint2 as IN);
    0x03,                   //bmAttributes(Interrupt);
    0x40,0x00,              //wMaxPacketSize(64);
    0xff,                   //bInterval(255ms);

//interface 06 - CDC 	       ======================================================================
    0x09,                   //bLength(9);
    0x04,                   //bDescriptorType(Interface);
    0x06,                   //bInterfaceNumber(6);
    0x00,                   //bAlternateSetting(0);
    0x02,                   //bNumEndpoints(2);
    0x0a,                   //bInterfaceClass(Data Interface Class);
    0x00,                   //bInterfaceSubClass(AData Interface Class SubClass Codes);
    0x00,                   //bInterfaceProtocol(USB SPEC);
    0x00,                   //iInterface(0);

    0x07,                   //bLength(7);
    0x05,                   //bDescriptorType(Endpoint);
    0x84,                   //bEndpointAddress(EndPoint1 as IN);
    0x02,                   //bmAttributes(Bulk);
    0x40,0x00,              //wMaxPacketSize(64);
    0x00,                   //bInterval(Ignored);

    0x07,                   //bLength(7);
    0x05,                   //bDescriptorType(Endpoint);
    0x04,                   //bEndpointAddress(EndPoint1 as OUT);
    0x02,                   //bmAttributes(Bulk);
    0x40,0x00,              //wMaxPacketSize(64);
    0x00,                   //bInterval(Ignored);
#endif

};


#endif //__USBDEV_CONF_H__
