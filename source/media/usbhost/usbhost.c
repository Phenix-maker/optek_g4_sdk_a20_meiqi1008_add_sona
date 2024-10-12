#include "os_config.h"


#ifdef USB_HOST_ENABLE

#include "stdDespt.h"
#include "IRP.h"
#include "usbmsc.h"
#include "usbd.h"

#include "usbhost.h"


//===========================================================================
//	USB host
//===========================================================================

int usb_hostStart(enum_USB_HOST_STACK_ATTR attr )
{
	usbmsc_startUSBhostEx(attr);
	return 1;
}

int usb_hostStop(void)
{
	usbmsc_stopUSBhost();
	return 1;
}

#endif //USB_HOST_ENABLE
