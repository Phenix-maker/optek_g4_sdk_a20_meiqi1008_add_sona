#ifndef __USBHOSTAUDIO__
#define __USBHOSTAUDIO__


#include "usbd.h"

//#define DRIVER_TOGETHER

/* support up to 16bits PCM data, 2 channels, 44.1KHz or 48KHz, 96KHz */
//#define ONLY16BITS

#define LOOPWHOLECFG

/* Test with wav data memory */
//#define WAV_DATA_TEST

/* Test with sine wave signal for Host audio out */
//#define SINEWAV

//sample freq list
#define USBHOST_AUDIO_SRC_16K_BIT     (1<<0)
#define USBHOST_AUDIO_SRC_32K_BIT     (1<<1)
#define USBHOST_AUDIO_SRC_44P1K_BIT   (1<<2)
#define USBHOST_AUDIO_SRC_48K_BIT     (1<<3)
#define USBHOST_AUDIO_SRC_64K_BIT     (1<<4)
#define USBHOST_AUDIO_SRC_88P2K_BIT   (1<<5)
#define USBHOST_AUDIO_SRC_96K_BIT     (1<<6)
#define USBHOST_AUDIO_SRC_128K_BIT    (1<<7)
#define USBHOST_AUDIO_SRC_176P4K_BIT  (1<<8)
#define USBHOST_AUDIO_SRC_192K_BIT    (1<<9)


extern U32 UHA_SAMPLE_FREQ, UHA_BitResolution, UHA_STREAM_CHANS;
extern U8 src_ok_flag;

int haudio_submitIsoOut( USB_DEVICE * usbDev );
int haudio_removeIsoOut( USB_DEVICE * usbDev );
int haudio_submitIsoIn(  USB_DEVICE * usbDev );
int haudio_removeIsoIn(  USB_DEVICE * usbDev );

//jj called from deviceConnectAction()
void HostAudioOutLaunch(USB_DEVICE * usbDev);
void HostAudioOutUnLaunch(USB_DEVICE * usbDev);

U8 installAudioDeviceDriver( USB_DEVICE * usbDev );

int usbhost_audio_device_src_setting(void);

#endif //__USBHOSTAUDIO__
