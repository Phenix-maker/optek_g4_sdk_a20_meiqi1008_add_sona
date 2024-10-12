#ifndef	__DENUM_AUDIO_H__
#define __DENUM_AUDIO_H__

#include "usbdev.h"


#define	STR_MANUFACTURE_LENGTH    (2 + 5*2)
extern const U8 usbAudioStringManufactureDescriptor[STR_MANUFACTURE_LENGTH];


#define STR_PRODUCT_LENGTH        (2 + 15*2)

extern const U8 usbAudioStringProductDescriptor[STR_PRODUCT_LENGTH];

#define	STR_SERIAL_NUM_MIN_LENGTH    (2 + 12*2)
extern const U8 usbAudioStringSerialNoDescriptor[STR_SERIAL_NUM_MIN_LENGTH];

#define	STR_HID_MIN_LENGTH    (2 + 7*2)
extern const U8 usbAudioStringHIDDescriptor[STR_HID_MIN_LENGTH];


int denum_init(void);
int denum_open(void);
int denum_close(void);

int denum_isendSetupPktMsg( U8 *pkt, int size );


#endif //__DENUM_AUDIO_H__
