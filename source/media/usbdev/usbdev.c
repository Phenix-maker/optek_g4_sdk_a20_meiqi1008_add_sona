#include "os_config.h"


#ifdef USB_DEV_ENABLE
#include "regmap.h"

#include "mem_reloc.h"


#include "hal_dma.h"

#include "fifo.h"
#include "stdDespt.h"
#include "IRP.h"
#include "usbmsc.h"
#include "usbd.h"
#include "usbdev.h"
#include "denumAudio.h"
#include "semphr.h"
#include "app_media_config.h"

#define DBG_USBDev_Printf

extern void *mutex_fs;
extern void *mutex_usb;



U32 usbTransLen = 0;
U8 usbErr = 0;

static void usbirpCompleteCallback(U8 iprResult, U32 iprActualLen )
{
	usbErr = iprResult;
	usbTransLen = iprActualLen;

	usbmsg_IRPcompleteFunc();
}


//===========================================================================
//	USB device
//===========================================================================

U8 usbdev_start_flag = FALSE;

//FIMXE: same as usb_host_start, attr
int usb_devStart(enum_USB_DEVICE_STACK_ATTR attr)
{
	IRP	irp;

	if (usbdev_start_flag == FALSE)
	{
		DBG_USBDev_Printf ("%s usbSlave open\n\r", __func__);

		USB_MUTEX_LOCK;
		FILL_NORMAL_IRP( &irp, enIRPT_DEV_STACK, enIRP_TYPE_DEV_START,	0,
			attr,	0,	0,	enIRPR_SUBMIT, usbirpCompleteCallback );    //(PIRP)->param = attr !!!

		usbmsg_sendIRP( &irp );
		usbmsg_waitIRPcomplete();
		DBG_assert( usbErr == enIRPR_SUCCESS );
		usbdev_start_flag = TRUE;

		USB_MUTEX_UNLOCK;
		return ( usbErr == enIRPR_SUCCESS );
	}
	else
	{
		DBG_USBDev_Printf ("%s usbSlave is open\n\r", __func__);
	}
}

int usb_devStop(void)
{
	IRP	irp;

	if (usbdev_start_flag == TRUE)
	{
		DBG_USBDev_Printf ("%s usbSlave close\n\r", __func__);

		USB_MUTEX_LOCK;
		FILL_NORMAL_IRP( &irp, enIRPT_DEV_STACK, enIRP_TYPE_DEV_STOP,	0,
			0,	0,	0,	enIRPR_SUBMIT, usbirpCompleteCallback );

		usbmsg_sendIRP( &irp );
		usbmsg_waitIRPcomplete();
		DBG_assert( usbErr == enIRPR_SUCCESS );
		usbdev_start_flag = FALSE;

		USB_MUTEX_UNLOCK;
		return ( usbErr == enIRPR_SUCCESS );
	}
	else
	{
		DBG_USBDev_Printf ("%s usbSlave is close\n\r", __func__);
	}
}


int usb_devID_get(U16 *pVendorID, U16 *pProductID)
{
	*pVendorID = VENDOR_ID;
	*pProductID = PRODUCT_ID;
	
	return TRUE;
}


#ifdef USB_DEV_ENABLE
const U8 usbAudioStringManufactureDescriptor[STR_MANUFACTURE_LENGTH] = {
	STR_MANUFACTURE_LENGTH,
	DESCRIPTOR_TYPE_STRING,
		'O',0,
		'p',0,
		't',0,
		'e',0,
		'k',0
};


const U8 usbAudioStringProductDescriptor[STR_PRODUCT_LENGTH] = {
	STR_PRODUCT_LENGTH,
	DESCRIPTOR_TYPE_STRING,
		'O',0,
		'T',0,
		'K',0,
		' ',0,
		'U',0,
		'S',0,
		'B',0,
		' ',0,
		's',0,
		'p',0,
		'k',0,
		'-',0,
		'm',0,
		'i',0,
		'c',0
};


const U8 usbAudioStringSerialNoDescriptor[STR_SERIAL_NUM_MIN_LENGTH] = {
	STR_SERIAL_NUM_MIN_LENGTH,
	DESCRIPTOR_TYPE_STRING,
		'0',0,
		'1',0,
		'2',0,
		'7',0,
		'8',0,
		'9',0,
		'A',0,
		'B',0,
		'C',0,
		'D',0,
		'E',0,
		'F',0
};

const U8 usbAudioStringHIDDescriptor[STR_HID_MIN_LENGTH] = {
	STR_HID_MIN_LENGTH,
	DESCRIPTOR_TYPE_STRING,
		'a',0,
		'n',0,
		'd',0,
		'l',0,
		'i',0,
		'n',0,
		'k',0
};


#ifdef USB_SPEAKER
#ifndef USB_DEV_UPDATA_MIC_ENABLE
FIFO UsbDevUpFifo;
#endif
static int changeCnt = 0;
static U8 samplesN = 0;
static short Vdata = 0x5555;
extern U8 usb_highspeed_flag;

//Normal case: AUX data (from DMA2) send to USB In.
U16 adc_pcm_data_read(short *pdata) __INTERNAL_RAM_TEXT; 


//The function is called by USB Dev Interrupt !!!
U16 adc_pcm_data_read(short *pdata)
{
#ifdef USB_DEV_UPDATA_MIC_ENABLE

/************************************************************************
For 44100 Sample Frequency:
	if ( (counter%10) == 0)	//for 44.1kHz sample rate, 44100 x 2 Channel x 2 Bytes /(1000) [125us] = 176.4 Bytes
	{
		counter = 0;
		len = 180;
	}
	else
	{
		len = 176;
	}

For 48000 Sample Frequency:
	len = 192;				//for 48kHz sample rate, 48000 x 2 Channel x 2 Bytes /(1000) [125us] = 192 Bytes
*************************************************************************/
	U32 *pADCDest;
	short *pIn, *pOut;
	U32 wlen, dlen, inlen;

	// Due to App to set len to specifc value !
	if (usb_highspeed_flag) //jj
	inlen = AUDIOIN_SAMPLE_FREQ/1000 * AUDIOIN_CHANNELS * AUDIOIN_SUB_FRAME_SIZE/8;
	else
	inlen = AUDIOIN_SAMPLE_FREQ/1000 * AUDIOIN_CHANNELS * AUDIOIN_SUB_FRAME_SIZE;

	#if (SHARE_LINK_MODE != SL_TWO_WAY_AV) && (SHARE_LINK_MODE != SL_TWO_WAY_VV)	//case that need ADC input!
	pADCDest = DMA_2_DEST;			//current fifo write address
	fifo_put_data_by_dma (&UsbDevUpFifo, pADCDest);
	#endif

	wlen = fifo_get_fifo_data_wlen(&UsbDevUpFifo);
	dlen = fifo_get_data_len(&UsbDevUpFifo);


	if (wlen < 0x4000)
	{
		if (dlen > 512)
		{
			fifo_discard_data(&UsbDevUpFifo, (dlen - 512));
		}
		memset(pdata, 0, inlen);
	}
	else
	{
		if (dlen < inlen )
		{
			memset(pdata, 0, inlen);
		}
		else
		{
			// if (dlen >= (480*4))
			// {
			// 	fifo_discard_data(&AdcInFifo, (dlen - 480*2));
			// }

			fifo_get_data (&UsbDevUpFifo, pdata, inlen);
		}
	}

	return inlen;
#else
	return 0;
#endif
}

#ifdef HIDFUNC
int dhid_submitIntIn(U8* pU8data);	//USB HID API in lib.

//HID data send to USB HID In.
U16 usb_hid_data_in(short *pdata) __INTERNAL_RAM_TEXT; 

U16 usb_hid_data_in(short *pdata)
{
	if (usbdev_featureconf.HIDFuncEnable)
	{
		return dhid_submitIntIn(pdata);
	}
	else
	{
		return 0;
	}
}

extern FIFO HIDOutFifo;

//Get HID data from USB HID Out pipe.
int usb_hid_hid_data_out() __INTERNAL_RAM_TEXT; 

int usb_hid_hid_data_out()
{
	U8 pBuf[1024];
	
	if (usbdev_featureconf.HIDFuncEnable)
	{
		fifo_get_data(&HIDOutFifo, pBuf, 64);
	}
}
#endif //HIDFUNC


int Samplerate_offset;
void daudio_SetFBSamplerateCorrection(int sample_rate_offset)
{
	Samplerate_offset = sample_rate_offset;
}

//return the delta value applied to USB device feedback Sample rate on theoretical value
void daudio_GetFBSamplerateCorrection(int *value)
{
	*value = Samplerate_offset;
}

#endif //(defined USB_SPEAKER)

#endif //USB_DEV_ENABLE


#ifdef USB_DEV_MICROPHONE
//===========================================================================
//	for usb microphone
//===========================================================================

#define	STR_MANUFACTURE_LENGTH    (2 + 5*2)
const U8 StringManufactureDescriptor[STR_MANUFACTURE_LENGTH] = {
	STR_MANUFACTURE_LENGTH,
	DESCRIPTOR_TYPE_STRING,
	'o',0,
	'p',0,
	't',0,
	'e',0,
	'k',0
};

#define	STR_PRODUCT_LENGTH        (2 + 14*2)
const U8 StringProductDescriptor[STR_PRODUCT_LENGTH] = {
	STR_PRODUCT_LENGTH,
	DESCRIPTOR_TYPE_STRING,
	'U',0,
	'S',0,
	'B',0,
	' ',0,
	'm',0,
	'i',0,
	'c',0,
	'r',0,
	'o',0,
	'p',0,
	'h',0,
	'o',0,
	'n',0,
	'e',0
};

#define	STR_SERIAL_NUM_LENGTH     (2 + 11*2)
const U8 StringSerialNoDescriptor[STR_SERIAL_NUM_LENGTH] = {
	STR_SERIAL_NUM_LENGTH,
	DESCRIPTOR_TYPE_STRING,
	'0',0,
	'1',0,
	'2',0,
	'3',0,
	'4',0,
	'A',0,
	'B',0,
	'C',0,
	'D',0,
	'E',0,
	'F',0
};
#endif

#endif //USB_DEV_ENABLE
