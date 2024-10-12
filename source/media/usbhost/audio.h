#ifndef	__AUDIO_H__
#define	__AUDIO_H__


typedef struct {
	U8	bLength;
	U8	bDesptType;
	U8	bDesptSubType;
	U16	bcdADC;
	U16	wTotalLen;
	U8	bInCollection;
	U8	baIntfNum;
	
	U8	baIntfNum1;
	U8	baIntfNum2;
	// more ?
}	__attribute__ ((packed)) AUDIO_CTL_INTF_DESPT;

#define	AUDIO_CTL_INTF_DESPT_MIN_SIZE	9

typedef	struct {
	U8	bLength;
	U8	bDesptType;
	U8	bDesptSubType;
	U8	bTermLink;
	U8	bDelay;
	U16	wFormatTag;
}	__attribute__  ((packed)) AUDIO_STREAM_INTF_DESPT;

#define	AUDIO_STREAM_INTF_DESPT_SIZE	7

typedef struct {
	U8	bLength;
	U8	bDesptType;
	U8	bDesptSubType;
	U8	bmAttribute;
	U8	bLockDelayUnit;
	U16	wLockDelay;
}	__attribute__  ((packed)) AUDIO_ISO_ENDP_DESPT;

#define	AUDIO_ISO_ENDP_DESPT_SIZE	7


//format
typedef struct {
	U8	bLength;
	U8	bDesptType;
	U8	bDesptSubType;
	U8	bFormatType;
	U8	bNurChannels;
	U8	bSubFrameSize;
	U8	bBitResolution;
	U8	bSamFreqType;
	
	U8	*sampleFreq;	//3 bytes each
}	__attribute__  ((packed)) TYPEI_FORMAT_DESPT;

#define	TYPEI_FORMAT_DESPT_MIN_SIZE		8
#define	FORMAT_DISCRETE_SAMPLE_FREQ_SIZE	3


//audio data type
#define	AUDIO_FORMAT_TYPE_UNDEFINED		0x00
#define	AUDIO_FORMAT_TYPE_I				0x01
#define	AUDIO_FORMAT_TYPE_II			0x02
#define	AUDIO_FORMAT_TYPE_III			0x03

//audio data format for wFormatTag
//typeI code
#define	AUDIO_DATA_FORMAT_I_UNDEFINED	0x0000
#define	AUDIO_DATA_FORMAT_I_PCM			0x0001
#define	AUDIO_DATA_FORMAT_I_PCM8		0x0002
#define	AUDIO_DATA_FORMAT_I_IEEE_FLOAT	0x0003
#define	AUDIO_DATA_FORMAT_I_ALAW		0x0004
#define	AUDIO_DATA_FORMAT_I_MULAW		0x0005


//audio class descriptor

#define	AUDIO_CLASS_DESPT_CS_UNDEFINED		0x20
#define	AUDIO_CLASS_DESPT_CS_DEVICE			0x21
#define	AUDIO_CLASS_DESPT_CS_CONFIG			0x22
#define	AUDIO_CLASS_DESPT_CS_STRING			0x23
#define	AUDIO_CLASS_DESPT_CS_INTF			0x24
#define	AUDIO_CLASS_DESPT_CS_ENDPT			0x25

#define	AUDIO_CLASS_AC_SUBDESPT_UNDEFINED		0x00
#define	AUDIO_CLASS_AC_SUBDESPT_HEADER			0x01
#define	AUDIO_CLASS_AC_SUBDESPT_INT				0x02
#define	AUDIO_CLASS_AC_SUBDESPT_OUTT			0x03
#define	AUDIO_CLASS_AC_SUBDESPT_MIXERU			0x04
#define	AUDIO_CLASS_AC_SUBDESPT_SELECTORU		0x05
#define	AUDIO_CLASS_AC_SUBDESPT_FEATUREU		0x06
#define	AUDIO_CLASS_AC_SUBDESPT_PROCESSU		0x07
#define	AUDIO_CLASS_AC_SUBDESPT_EXTENSIONU		0x08

#define	AUDIO_CLASS_AS_SUBDESPT_UNDEFINED		0x00
#define	AUDIO_CLASS_AS_SUBDESPT_GENERAL			0x01
#define	AUDIO_CLASS_AS_SUBDESPT_FORMAT_TYPE		0x02
#define	AUDIO_CLASS_AS_SUBDESPT_FORMAT_SPEC		0x03



//request
#define	AUDIO_CLASS_REQUEST_CODE_UNDEFINED		0x00
#define	AUDIO_CLASS_REQUEST_CODE_SET_CUR		0x01
#define	AUDIO_CLASS_REQUEST_CODE_GET_CUR		0x81
#define	AUDIO_CLASS_REQUEST_CODE_SET_MIN		0x02
#define	AUDIO_CLASS_REQUEST_CODE_GET_MIN		0x82
#define	AUDIO_CLASS_REQUEST_CODE_SET_MAX		0x03
#define	AUDIO_CLASS_REQUEST_CODE_GET_MAX		0x83
#define	AUDIO_CLASS_REQUEST_CODE_SET_RES		0x04
#define	AUDIO_CLASS_REQUEST_CODE_GET_RES		0x84
#define	AUDIO_CLASS_REQUEST_CODE_SET_MEM		0x05
#define	AUDIO_CLASS_REQUEST_CODE_GET_MEM		0x85
#define	AUDIO_CLASS_REQUEST_CODE_GET_STAT		0xFF

#define	AUDIO_ENDP_CONTROL_SELECTOR_UNDEFINED	0x00
#define	AUDIO_ENDP_CONTROL_SELECTOR_SAMPLE_FREQ	0x01
#define	AUDIO_ENDP_CONTROL_SELECTOR_PITCH		0x02


#define	AUDIO_CLASS_ENDP_REQ_SET_CUR		( (0x22 << 8) | AUDIO_CLASS_REQUEST_CODE_SET_CUR )


int audio_init(void);
int audio_close(void);

int audio_setSampleRate( int sampleFreq );
int audio_setIntf( int zeroBandwidth );

int audio_submitIsoIn( void );
int audio_removeIsoIn( void );

#endif //__AUDIO_H__
