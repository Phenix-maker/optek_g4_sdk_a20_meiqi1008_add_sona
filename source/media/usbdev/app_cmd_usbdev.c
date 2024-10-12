#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"


#if (defined USB_SPEAKER && defined USB_DEV_ENABLE)

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_uart.h"
#include "hw_audio_i2s.h"
#include "hw_timer.h"

#include "fifo.h"
#include "app_timer.h"
#include "app_key.h" 
#include "string.h"
#include "app_window.h"
#include "audio_com.h"
#include "audio_codec.h"
#include "tools.h"
#include "usbmsc.h"
#include "usbdev.h"
#include "daudio.h"
#include "audio_task.h"
#include "app_media_config.h"
#include "debug.h"
#include "audio_usb_speaker_input.h"




#ifdef USB_SPEAKER_AUTO_DETECT
U8 usb_host_state;
#endif

#define HIDOutFifoBufSize 100
FIFO HIDOutFifo;
U8 HIDOutFifoBuf[HIDOutFifoBufSize];

U8 hidBuffer[64];	//64bytes for FS HID, 1024 for HS. consumer usage: Report ID/keycode/00



#ifdef USB_SPEAKER
void app_nav_usbSpeaker_process (AUDIO_TASK_MSG_STRU *process_msg)
{
	STREAM_ID s;
	int wlen, pos;

	switch (process_msg->param)
	{	
	case UI_FIT_TIMER:
		break;

	case UI_STREAM_UNDERFLOW:
		app_dac_mute_enable();
		audio_env.playing_stream_status = STREAM_WAITING_DATA;
		break;

	default:
		break;
	}
}
#endif


#ifdef USB_HOST_AUDIO_ENABLE
void app_nav_usbhost_audio_pocess (USB_UI_MSG *process_msg)
{
	switch (process_msg->parm1)
	{
	case UI_FIT_TIMER:
		break;

	default:
		break;
	}
}
#endif //USB_HOST_AUDIO_ENABLE



void app_cmd_usbSpeaker_volChange( void )
{

}

#define USBSPEAKER_DEFAULT_VOL -12//db

VOLUME_SETTING usbSpeaker_volSetting  = {
	USBSPEAKER_DEFAULT_VOL*10,	//cur
	-48*10,		//min   -72db
	0,		//max  0db
	10		//res: 1dB
};


int usb_speaker_vol_gain;
I16 usbSpeaker_vol;

int app_cmd_usbSpeaker_getVol_setting( const VOLUME_SETTING **vol )
{
	usbSpeaker_volSetting.cur	= usbSpeaker_vol;
	
	*vol	=	&usbSpeaker_volSetting;

	return 0;
}

void app_cmd_usbSpeaker_vol_init(void)
{
	usbSpeaker_vol = USBSPEAKER_DEFAULT_VOL*10;
	usb_speaker_vol_gain = 0x08000000;
}

void app_cmd_usbSpeaker_volSet( I16 vol )
{
	usbSpeaker_vol = vol;
	vol = (vol - volSetting.max); 
	DBG_Printf("vol:%d (0.1db)\r\n", vol);
	
	usb_speaker_vol_gain = 0x08000000;
}


//called by interrupt
void app_cmd_usbSpeaker_chanGet( U8 channel )
{
	DBG_iPrintf("usbSpeaker_chanGet=%d\n\r", channel);
	
	audio_in_stream.channel = channel;
	hw_audio_mi2s_in_channel_set(audio_in_stream.channel);
	
	AUDIO_FIFO_STREAM_DISCARD_ALL_DATA();
}


int daudio_isMute( void );



#if	0//def AUDIO_PROCESSOR
#define AMP_MUTE_ON              AUDIOdevice.Set_Mute(TRUE)
#define AMP_MUTE_OFF             AUDIOdevice.Set_Mute(FALSE)

#define AMPLIFIER_MUTE_ON        AUDIOdevice.Set_Mute(TRUE)
#define AMPLIFIER_MUTE_OFF       AUDIOdevice.Set_Mute(FALSE)
#else
#define AMP_MUTE_ON
#define AMP_MUTE_OFF

#define AMPLIFIER_MUTE_ON
#define AMPLIFIER_MUTE_OFF
#endif

void app_cmd_usbSpeaker_muteChange( void )
{
	if( daudio_isMute() )
	{
		DBG_Printf( "mute on: usb speaker\n\r");
		//AMPLIFIER_MUTE_ON;
	}
	else
	{
		DBG_Printf( "mute off: usb speaker\n\r");
		//AMPLIFIER_MUTE_OFF;
	}
}

extern U8 AUDIO_CHANNELS_cfg;

static void usbSpeakerStreamConfig( void )
{
	STREAM_ID s;
	U8 *pStreamBuf;
	U32 size;
	U32 len = STREAM_LENTH_UNKNOWN;

	DBG_Printf( "*** %s\n\r", __func__);

	AUDIO_FIFO_STREAM_FLUSH();

	#ifdef STREAM_BUFFER_SIZE_UNFIXED
	pStreamBuf = USB_SPEAKER_STREAM_START;
	size = USB_SPEAKER_STREAM_END - USB_SPEAKER_STREAM_START;
	#else
		pStreamBuf = stream_buffer;
		#ifdef STREAM_BUFFER_USED_POINTER
		size = STREAM_BUF_MAX_SIZE;
		#else
		size = sizeof(stream_buffer);
		#endif
	#endif

	daudio_SetFBSamplerateCorrection(0);

	AUDIO_FIFO_STREAM_FLUSH();

	AUDIO_FIFO_STREAM_OPEN(pStreamBuf, size);
	AUDIO_FIFO_STREAM_FLUSH();
	AUDIO_FIFO_STREAM_CREATE(0, STREAM_LENTH_UNKNOWN, 0, 0);


	audio_env.playing_stream_status = STREAM_WAITING_DATA;
	memset((char*)&usbdev_featureconf, 0, sizeof(usbdev_featureconf));
	
	#ifdef HIDFUNC //init FIFO only for HID Out!
	fifo_init(&HIDOutFifo, HIDOutFifoBuf, HIDOutFifoBufSize, TRUE);

	usbdev_featureconf.HIDFuncEnable = TRUE;
	#ifdef HIDKEY
	usbdev_featureconf.HIDKEYEnable = TRUE;
	#else
	usbdev_featureconf.HIDKEYEnable = FALSE;
	#endif
	#else
	usbdev_featureconf.HIDFuncEnable = FALSE;
	usbdev_featureconf.HIDKEYEnable = FALSE;
	#endif

	usbdev_featureconf.USBAUDIOdata_BITs = 0;	// 16bits
	usbdev_featureconf.ADC20Enable = FALSE;


	#ifdef ONLYMicCFG
	usbdev_featureconf.USBOutEnable = FALSE;
	#else
	usbdev_featureconf.USBOutEnable = TRUE;
	#endif

	if (usbdev_featureconf.USBOutEnable)
	{
		usbdev_featureconf.usb_AUDIO_SAMPLE_FREQ = AUDIO_SAMPLE_FREQ;
		usbdev_featureconf.usb_AUDIO_SAMPLE_FREQ2 = AUDIO_SAMPLE_FREQ2;

		usbdev_featureconf.usb_AUDIO_CHANNELS		=AUDIO_CHANNELS_cfg;//AUDIO_CHANNELS;
		usbdev_featureconf.usb_AUDIO_SUB_FRAME_SIZE = AUDIO_SUB_FRAME_SIZE;
		usbdev_featureconf.usb_AUDIO_BIT_RESOLUTION = AUDIO_BIT_RESOLUTION;

		//Below features only available when there is a USB OUT!
		#ifdef USBDEV_ISOIN_ASYNC
		usbdev_featureconf.ISOIN_ASYNCEnable = TRUE;
		#else
		usbdev_featureconf.ISOIN_ASYNCEnable = FALSE;
		#endif

		usbdev_featureconf.USBOutMixInEnable = FALSE;
	}

	#ifdef ONLYSpeakerCFG
	usbdev_featureconf.USBInEnable = FALSE;
	#else
	usbdev_featureconf.USBInEnable = TRUE;
	#endif

	if (usbdev_featureconf.USBInEnable)
	{
		usbdev_featureconf.usb_AUDIOIN_SAMPLE_FREQ = AUDIO_SAMPLE_FREQ;
		usbdev_featureconf.usb_AUDIOIN_CHANNELS 	  = AUDIOIN_CHANNELS;
		usbdev_featureconf.usb_AUDIOIN_SUB_FRAME_SIZE = AUDIOIN_SUB_FRAME_SIZE;
		usbdev_featureconf.usb_AUDIOIN_BIT_RESOLUTION = AUDIOIN_BIT_RESOLUTION;
	}

	usbdev_featureconf.SOFINTEnable = FALSE;
	
	#ifdef SOFINTEN
	usbdev_featureconf.SOFINTEnable = TRUE;
	#endif
}

extern WINDOW hdmi_window;
void app_cmd_switch2usbSpeaker( void )
{
	DBG_Printf("%s\n\r", __func__);

	//config stream
	usbSpeakerStreamConfig();

	global_vari.audio_source = SOURCE_USB_SPEAKER;

	//usbdev start
	//IRP of enIRPT_DEV_STACK, enIRP_TYPE_DEV_START
	usb_devStart(enUSB_DEV_STACK_ATTR_PC_SPEAKER);

	app_audio_iso_rcv_enable();

	AMPLIFIER_MUTE_OFF;

	DBG_Printf( "switch to usb speaker\n\r");
}


void app_cmd_usbSpeakerConnect( void )
{	
	DBG_Printf("%s\n\r", __func__);

	usbDevType = enUSBH_DEV_IPOD_AUDIO;

	//config stream
	usbSpeakerStreamConfig();
}

void app_cmd_usbSpeakerDisconnect( void )
{

}

void app_cmd_msgUsbDevConnect( void )
{
	func_wo_param_msgSend( app_cmd_usbSpeakerConnect );
}
void app_cmd_msgUsbDevDisonnect( void )
{
	func_wo_param_msgSend( app_cmd_usbSpeakerDisconnect );
}

void app_cmd_usbHostConnect( void )
{	
	DBG_Printf("%s\n\r", __func__);

	audio_usb_speaker_input_rx_start(NULL, 0);

	usbDevType = enUSBH_DEV_IPOD_AUDIO;
}

void app_cmd_usbHostDisconnect( void )
{

}

void app_cmd_msgUsbHostConnect( void )
{
	func_wo_param_msgSend( app_cmd_usbHostConnect );
}

void app_cmd_msgUsbHostDisonnect( void )
{
	func_wo_param_msgSend( app_cmd_usbHostDisconnect );
}


/************************************************/
/******************** application ***************/
/************************************************/

#ifdef USB_SPEAKER_AUTO_DETECT
void app_nav_pc_usb_host_connect(void)
{
	DBG_Printf("pc connected\r\n");

	if (global_vari.audio_source == MEDIA_USB_SPEAKER)
	{
		app_cmd_switch2usbSpeaker();
	}
	else
	{

	}
}

void app_nav_pc_usb_host_disconnect(void)
{	
	DBG_Printf("pc disconnected\r\n");

	app_audio_iso_rcv_disable();
	usb_devStop();
}
#endif


extern void app_audio_clock_freq_setting(U32 stream_srf);
extern int usb_hostStop(void);


#ifdef USB_SPEAKER_AUTO_DETECT
void PC_USB_Detect_Polling(void) __INTERNAL_RAM_TEXT;

void PC_USB_Detect_Polling(void)
{
	static U8 count = 0;
	U8 tmp;
	
	tmp = !PC_USB_DETECT;
	
	if (usb_host_state)
	{
		if (tmp == USB_DEVICE_DISCONNECT)
		{
			count ++;
			if (count > 2)
			{
				usb_host_state = FALSE;
				count = 0;
				func_wo_param_msgSend(app_nav_pc_usb_host_disconnect);
				DBG_Printf("PC usbHOST unPlug\n\r");
			}
		}
		else
		{
			count = 0;
		}
	}
	else
	{
		if (tmp == USB_DEVICE_CONNECT)
		{
			count ++;
			if (count > 2)
			{
				usb_host_state = TRUE;
				count = 0;

				if (global_vari.audio_source == MEDIA_USB_SPEAKER)
				{
					func_wo_param_msgSend(app_nav_pc_usb_host_connect);
				}

				DBG_Printf("PC usbHOST Plug\n\r");
			}
		}
		else
		{
			count = 0;
		}
	}
}
#endif


//The function is called by USB Dev Interrupt !!!
//translen: byte length of U8 *buffer.
//return size put to FIFO.

U32 usbdev_daudio_put_data_callback_from_isr(U8 *buffer, U32 translen)
{
	U32 size,dlen;

	size = (U32) AUDIO_FIFO_STREAM_PUT_DATA (buffer, translen);

	if (size != translen)
	{
		DBG_iPrintf("usbdev rx overflow:%d;%d\n\r", size,translen);
	}

	dlen = (U32) AUDIO_FIFO_STREAM_DATA_LEN ();
	if (audio_in_stream.prepare_status == AUDIO_IN_STREAM_PREPARE_WAITING)
	{
		daudio_SetFBSamplerateCorrection(0);

		DBG_iPuts("usbdev dec start\n\r");

		if (audio_usb_speaker_input_rx_status_get() == FALSE)
		{
			audio_usb_speaker_input_rx_enable();
		}

		usbdev_decocde_set_from_isr();
		usbdev_decocde_start_w_init_from_isr();

		audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_READY;
	}

	return (size == translen );
}


/*
 * For USB HID Out put data hidOutbuf[] to FIFO!
 * translen: byte length of U8 *buffer. Generally fixed to 64 Bytes.
 * return size put to FIFO.
 */
U32 app_cmd_usbHidOut_PUT_DATA(U8 *buffer, U32 translen)
{
	U32 size;

	DBG_Printf("HidOut_PUT_DATA translen %x %x %x %x %x %x %x %x %x %x, %d\n", 
					buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], translen);

	size = fifo_put_data(&HIDOutFifo, buffer, translen);
	DBG_iPrintf("PUT_DATA len=%d\n", size);

	if (size == 0)
	{
		DBG_iPrintf("PUT_DATA overflow!\n");
	}
	
	DBG_assert( size == translen );
	return (size == translen );
}

/****************************************************************************/
/* HIDreq_SetReport is called to send HID data in SetReport_Data[] with     */
/* Report_ID first byte from Host to Dev.                                   */
/* And callback to HIDdataOUT_callback()									*/
/****************************************************************************/
extern U8 SetReport_Data[];
void HIDdataOUT_callback(void)
{
	DBG_Printf("%s\n\r", __func__);
	DBG_Printf("%x-%x-%x-%x\n %x-%x-%x-%x\n  %x-%x-%x-%x\n %x-%x-%x-%x\n\r",
				SetReport_Data[0], SetReport_Data[1], SetReport_Data[2], SetReport_Data[3],
				SetReport_Data[4], SetReport_Data[5], SetReport_Data[6], SetReport_Data[7],
				SetReport_Data[8], SetReport_Data[9], SetReport_Data[10], SetReport_Data[11],
				SetReport_Data[12], SetReport_Data[13], SetReport_Data[14], SetReport_Data[15]);
}

/****************************************************************************/
/* HIDdataIN_Buf is called to send HID data in hidbuftestRpt[] with         */
/* Report_ID first byte to Host, total length hidbuftestRpt[] cannot        */
/* exceed 64.	                                                            */
/* And callback to HIDdataIN_callback()										*/
/****************************************************************************/
void HIDdataIN_callback(void)
{
	DBG_Printf("%s\n\r", __func__);
}

#ifdef HIDFUNC

/****************************************************************************************/
/* HIDkeyIN_Buf() is called by g_submitIntInKdown() and g_submitIntInKup() in App		*/
/* to send HID key in hidBuffer[] to Host.												*/
/****************************************************************************************/

void _dhid_submitIntIn_finished_callback(void);

extern const APP_NAV_KEY APP_NAV_PC_SPEAKER_KEY [];

U8 ReportID;

int g_submitIntInKup( void )	//HID Input, called by UI
{
	DBG_Printf("%s\n\r", __func__);
	//usbd_isDevActive(NULL);	//test only!
//	int ret = 0, len = 3;//pipe->maxpkt;	//3 bytes for consumer INPUT
	int ret = 0, len = 2;//pipe->maxpkt;	//3 bytes for consumer INPUT

	memset(hidBuffer, 0x0, 64);

	if (usbdev_featureconf.HIDKEYEnable)
	{
//		hidBuffer[0] = 0x01;	////Report ID
		hidBuffer[0] = ReportID;	////Report ID
		hidBuffer[1] = 0; //consumer key up
		hidBuffer[2] = 0; //consumer key up

		ret = HIDkeyIN_Buf(hidBuffer, len);
		DBG_USBDev_Printf("dstk_transfer ret:0x%x\n\r", ret);
	}

	return ret;
}

int g_submitIntInKdown( U32 processing_key_id )	//HID Input, called by UI
{
	U16 keyval = 0xFF;
	U8 ReportID_value = 0x00;
	U8 hidKey_data = 0x00;

	int ret = FALSE;
	int len = 2;

	switch (processing_key_id)
	{
	case APP_KEY_PLAY:
		/* code */
		keyval = 0;//?
		break;

	case APP_KEY_SKIPF:
		/* code */
		keyval = 2;//?
		break;

	case APP_KEY_SKIPB:
		/* code */
		keyval = 3;//?
		break;

	case APP_KEY_PLUS:
		/* code */
		keyval = 4;//?
		break;

	case APP_KEY_MINUS:
		/* code */
		keyval = 5;//?
		break;

	default:
		DBG_Printf("%s HID key not support: %d\n\r", __func__, processing_key_id);
		return ret;
		break;
	}
	
	DBG_Printf("%s(%d): 0x%x\n\r", __func__, usbdev_featureconf.HIDKEYEnable, keyval);

	if (usbdev_featureconf.HIDKEYEnable)
	{
		switch (keyval)
		{
		case 0:
			/* PLAY/PAUSE */
			ReportID_value 	= 0x19;	// Report ID? bSize=1 byte
			hidKey_data 	= 0x01;	// (U8)HIDKeycodes[1];			
			break;

		case 2:
			/* SKIPF */
			ReportID_value 	= 0x19;	// Report ID? bSize=1 byte
			hidKey_data 	= 0x04;	// (U8)HIDKeycodes[1];
			break;

		case 3:
			/* SKIPB */
			ReportID_value 	= 0x19;	// Report ID? bSize=1 byte
			hidKey_data 	= 0x08;	// (U8)HIDKeycodes[1];
			break;

		case 4:
			/* Vol+ */
			ReportID_value 	= 0x18;	// Report ID? bSize=1 byte
			hidKey_data 	= 0x01;	// (U8)HIDKeycodes[0];
			break;

		case 5:
			/* Vol- */
			ReportID_value 	= 0x18;	// Report ID? bSize=1 byte
			hidKey_data 	= 0x02;	// (U8)HIDKeycodes[1];
			break;			

		default:
			return ret;
			break;
		}

		ReportID = ReportID_value;

		memset(hidBuffer, 0x0, 64);

		hidBuffer[0] = ReportID_value;
		hidBuffer[1] = hidKey_data;



		ret = HIDkeyIN_Buf(hidBuffer, len);	//send key code
	}

	DBG_USBDev_Printf("dstk_transfer ret:%x\n\r", ret);

	return ret;
}
#endif	//HIDFUNC

void hw_audio_mi2s_in_channel_set(u8 channel_nummber);
I16 usbSpeaker_vol, usbSpeaker_chcfg = 2;
void app_cmd_usbSpeaker_CHANNELS_cfg( I16 Channelcfg )
{
	AUDIO_FIFO_STREAM_FLUSH();

	usbSpeaker_chcfg = Channelcfg;
	usbdev_featureconf.usb_AUDIO_CHANNELS = Channelcfg;

	func_w_one_param_msgSend_formIsr( hw_audio_mi2s_in_channel_set, ((Channelcfg/2)-1));

	DBG_Printf("usbSpeaker_chcfg:%d\r\n",usbSpeaker_chcfg);

}


#endif //(defined USB_SPEAKER && defined USB_DEV_ENABLE)

