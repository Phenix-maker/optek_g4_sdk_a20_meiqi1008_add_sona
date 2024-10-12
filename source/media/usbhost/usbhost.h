#ifndef	__USB_H__
#define __USB_H__

#include "usbmsc.h"


/* host */
int usb_hostStart(enum_USB_HOST_STACK_ATTR attr );
int usb_hostStop(void);


#endif
