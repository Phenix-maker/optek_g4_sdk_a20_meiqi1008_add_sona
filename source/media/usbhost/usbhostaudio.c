#include "c_def.h"
#include "debug.h"
#include "oem.h"


#ifdef USB_HOST_AUDIO_ENABLE
#include "mem_reloc.h"

#include "IRP.h"
#include "hstack.h"
#include "usbmsc.h"

#include "hcd.h"
#include "usbd.h"
#include "fifo.h"
#include "audio_com.h"
#include "app_media_common.h"
#include "app_timer.h"
#include "app_sdram.h"
#include <string.h>
#include "usbhostaudio.h"
#include "usbhost_audio_driver.h"
#include "usb.h"
#include "otg.h"

#ifdef OPTEK_DSRC_ENABLE
#include "optek_dsp_ip.h"
#include "optek_d2as_table.h"
#endif


extern U32 omitcnt;
extern U8 usbhost_audio_connected;

U8 src_ok_flag;	//if succeed for USB to find a sample frequency.
FIFO UHA_MicInFifo;

void usbhost_audio_device_get_except_src_config(U32 *sample_freq, U8 *sample_bits, U8 *num_freq);
int usbhost_audio_device_src_config_get(void);

U8 SwapIntf( USB_DEVICE * usbDev, U32 BITS_RES, U32 SamFreqV );

int usbhost_audio_device_src_setting(void)
{
	U8 ret;
	U32 stream_in_sr;
	U8 stream_bit;
	USB_DEVICE * usbDev = &singleUsbDevice;
	
	DBG_Printf("u_a_d_src_setting ......\n\r");

	if (!usbd_get_usbhost_audio_device_status())
	{
		DBG_Printf("No usbhostAudio\n\r");
		return FALSE;
	}

	if (global_vari.audio_source == SOURCE_OPTICAL)
	{
		goto SWAP_IF_INIT;
	}


	stream_in_sr = audio_env.playing_stream_sample_rate;	//init to stream sample rate

#ifdef OPTEK_DSRC_ENABLE
	extern U32 UHA_SAMPLE_FREQ;
	
	int retsrc;

	retsrc = usbhost_audio_device_src_config_get();//jj???	
	if (retsrc == 0) 
	{
		return FALSE;
	}
	//Force to 44 or 48, for test!
	retsrc = USBHOST_AUDIO_SRC_48K_BIT | USBHOST_AUDIO_SRC_44P1K_BIT;

	if ( (retsrc & USBHOST_AUDIO_SRC_44P1K_BIT) && (retsrc & USBHOST_AUDIO_SRC_48K_BIT) )
	{
		//don't need to  use sw src
		d2as_need_flag = FALSE;
		UHA_SAMPLE_FREQ = stream_in_sr;
	}
	else if (retsrc & USBHOST_AUDIO_SRC_44P1K_BIT && stream_in_sr != 44100 )
	{
		//must use sw src
		d2as_need_flag = TRUE;
		UHA_SAMPLE_FREQ = 44100;
		app_media_data.playing_stream_sample_rate = 44100;

		//need to SRC to UHA_SAMPLE_FREQ from SwapIntf
	}
	else if (retsrc & USBHOST_AUDIO_SRC_48K_BIT && stream_in_sr != 48000 )
	{
		//must use sw src
		d2as_need_flag = TRUE;
		UHA_SAMPLE_FREQ = 48000;
		app_media_data.playing_stream_sample_rate = 48000;
	}
	else
	{
		d2as_need_flag = FALSE;
		DBG_Printf ("\nSRC unncsry:%d for %d\n\r", retsrc, stream_in_sr);
	}


	DBG_Printf ("\nd2as_need_flag=%d\n\r", d2as_need_flag);
	
	if (d2as_need_flag)
	{
		extern void *optek_d2as1; //for 32kHz,64kHz,128kHz
		
		if (stream_in_sr == 48000)
		{
			//jj Init SRC as input source sample rate 48000Hz (output to 44100Hz/P160_M12_coeffs) for SRC operation ! P160_M12_coeffs
			//be ready to change 44.1kHz songs to 48kHz songs
			optek_d2as1 = optek_dsrc_open(optek_d2as1, SPDIF_D2AS_M, SPDIF_D2AS_N, P160_M12_coeffs, 0, SPDIF_48K_T0_44P1K_BASE_STEP, 
                                         SPDIF_48K_T0_44P1K_BASE_INTER_D, SPDIF_D2AS_HP_MASK, SPDIF_D2AS_CHAN, PROCESSING_DATA_16b);
			DBG_Printf ("\n<dsrc> open for 48 to 44.1 kHz)\n\r" );
		}	
		else if (stream_in_sr == 44100)
		{
			//jj Init SRC as input source sample rate 44100Hz (output to 48000Hz/P192_M12_coeffs) for SRC operation ! P192_M12_coeffs
			//be ready to change 48kHz songs to 44.1kHz songs
			optek_d2as1 = optek_dsrc_open(optek_d2as1, SPDIF_D2AS_M, SPDIF_D2AS_N, P192_M12_coeffs, 0, SPDIF_44P1K_TO_48K_BASE_STEP,
										SPDIF_44P1K_TO_48K_BASE_INTER_D, SPDIF_D2AS_HP_MASK, SPDIF_D2AS_CHAN, PROCESSING_DATA_16b);
			DBG_Printf ("\n<dsrc> open for 44.1 to 48 kHz)\n\r" );
		}
	}
	else
	{
		DBG_Printf ("\n<dsrc> is NOT open!\n\r" );
	}
#endif //OPTEK_DSRC_ENABLE


#ifdef WAV_DATA_TEST
	//only for wecall 16 bits/16KHz
	stream_out_sr = 44100;
	stream_bit = 24;
#else

U32 stream_out_sr;
#ifdef OPTEK_DSRC_ENABLE
	stream_out_sr = UHA_SAMPLE_FREQ;
	stream_bit = app_media_data.playing_stream_sample_bits;
#else
	stream_out_sr = audio_env.playing_stream_sample_rate;
	stream_bit = audio_env.playing_stream_sample_bits;
#endif

	DBG_Printf("   Played stream:%x/%x\n\r", stream_bit, stream_out_sr);
#endif


SWAP_IF_INIT:
	if (global_vari.audio_source == SOURCE_OPTICAL)
	{
		stream_out_sr = audio_env.playing_stream_sample_rate;
		stream_bit = audio_env.playing_stream_sample_bits;
	}

	int size = MIC_FIFO_BUF_SIZE;
	fifo_init (&UHA_MicInFifo, (U8 *)micFifo_buf, (size), TRUE);//JJ+0

	ret = SwapIntf(usbDev, stream_bit, stream_out_sr);	
	omitcnt = 0;	//remove head noise data, set to 0

	if (ret == enURBRST_SUCESS)
	{
		DBG_Printf("\n   src swap ok.\n\r");

		src_ok_flag = TRUE;
		return ret;
	}
	else //Some device doesn't response to set sample freq (Stall), so assume it works with the only sample freq, otherwise exception.
	{
		//See details in usbhost_audio_device_get_except_src_config().
		U32 sample_freq; 
		U8 sample_bits, num_freq;

		DBG_Printf("\n   src swap error!!!\n\r");
		
		usbhost_audio_device_get_except_src_config(&sample_freq, &sample_bits, &num_freq);
		if ( (num_freq == 1) && 
			(stream_out_sr == sample_freq) &&
			(stream_bit == sample_bits) )
		{
			src_ok_flag = TRUE;
			ret = enURBRST_SUCESS;
		}
		else
		{
			src_ok_flag = FALSE;
			ret = enURBRST_FAIL;
		}
		
		return ret; //wanted config not found 
	}
}


U16 USBDevSampleRateList(void);

//Get a int value with bit value indicate support sample freq list in usbhostaudio.h.
int usbhost_audio_device_src_config_get(void)
{
	int ret;
	
	if (!usbd_get_usbhost_audio_device_status())
	{
		DBG_Printf("No usbhostAudio!\n\r");
		return FALSE;
	}

	ret = USBDevSampleRateList();
	DBG_Printf("usbHost audio src config:0x%x\n\r", ret);

	return ret;
}


#endif //USB_HOST_ENABLE

