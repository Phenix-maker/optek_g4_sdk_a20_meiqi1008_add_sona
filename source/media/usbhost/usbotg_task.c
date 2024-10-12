#include "os_config.h"



#if ((defined USB_HOST_ENABLE) || (defined USB_DEV_ENABLE))

#include "mem_reloc.h"

#include "hw_timer.h"

#include "app_timer.h"
#include "tools.h"
#include "interrupt.h"
#include "IRP.h"
#include "otg.h"

#ifdef USB_HOST_ENABLE
#include "usbd.h"
#endif

#include "app_media_config.h"


void task_usbotg(void *pvParameters) __INTERNAL_RAM_TEXT;

int dstk_IRPprocess( IRP *irp );
int AOAstack_irpProcess( IRP *irp );
int iap2stk_IRPprocess( IRP *irp );

#ifdef SYSTEM_CRASH_CHECK_ENABLE
volatile U8 task_usbotg_check_cnt;
#endif



#define TASK_USBOTG_STACK_SIZE      (4096)

void usbotg_task(void *pvParameters);

void task_usbotg_init( void )
{
	mutex_usb = xSemaphoreCreateMutex();

	gUsbQueue = xQueueGenericCreate( DEPTH_OF_USBMSG_QUEUE, (WIDTH_OF_USBMSG_QUEUE*4), queueQUEUE_TYPE_SET );

    if (xTaskCreate( usbotg_task, "otg task", TASK_USBOTG_STACK_SIZE, 0, TASK_USB_PRIORITY, NULL) != pdTRUE )
	{
		DBG_Assert(FALSE);
    }
}

void usbotg_task(void *pvParameters)
{
	UINT ret;
	U32 data[WIDTH_OF_USBMSG_QUEUE];
	IRP * irp = (IRP *)data;

	#ifdef USB_INT_CHANGEs_TO_LEVEL1
	SOFT5_INT_ENABLE;
	#endif
	
	usb_highspeed_flag = FALSE;

	while(1)
	{
	#ifdef SYSTEM_CRASH_CHECK_ENABLE
		task_usbotg_check_cnt = 1;
	#endif

		ret = QUEUE_USB_RECEIVE;
		DBG_assert( TRUE == ret );

		switch( irp->target )
		{
		case enIRP_USB_OPEN:
			#ifdef USB_HOST_ENABLE

			#ifndef	OTG_FORCE_FULL_SPEED
			usb_highspeed_flag = TRUE;
			#endif

			irp->target	=	enIRPT_USBD;
			irp->type	=	enIRP_USBHOST_START;

			usbd_IRPprocess( irp );

			if( irp->complete != NULL )
			{
				(*irp->complete) ( irp->result, irp->actualLen );
			}
			#endif

			break;

		case enIRP_USB_CLOSE:
			#ifdef USB_HOST_ENABLE

			irp->target	=	enIRPT_USBD;
			irp->type	=	enIRP_USBHOST_STOP;

			usbd_IRPprocess( irp );

			if( irp->complete != NULL )
			{
				(*irp->complete) ( irp->result, irp->actualLen );
			}
			#endif //USB_HOST_ENABLE

			break;

		case enIRPT_USBD:
			#ifdef USB_HOST_ENABLE
			
			#ifndef	OTG_FORCE_FULL_SPEED
			usb_highspeed_flag = TRUE;
			#endif

			usbd_IRPprocess( irp );

			if( irp->complete != NULL )
			{
				(*irp->complete) ( irp->result, irp->actualLen );
			}
			#endif

			break;

		case enIRPT_MSC_CLASS:
			#ifdef USB_HOST_ENABLE
			mscClass_IRPprocess( irp );
			if( irp->complete != NULL )
			{
				(*irp->complete) ( irp->result, irp->actualLen );
			}
			#endif
			
			break;
		
		#ifdef PTP_MTP_CLASS
		case enIRPT_MTP:
			mtpClass_IRPprocess( irp );
			if( irp->complete != NULL )
			{
				(*irp->complete) ( irp->result, irp->actualLen );
			}
			break;
		#endif

		#ifdef USB_DEV_ENABLE
		case enIRPT_DEV_STACK:
			#ifndef	OTG_FORCE_FULL_SPEED
			usb_highspeed_flag = TRUE;
			#endif

			dstk_IRPprocess( irp );
			if( irp->complete != NULL )
			{
				(*irp->complete) ( irp->result, irp->actualLen );
			}
			break;
		#endif //USB_DEV_ENABLE

		#ifdef APPLE_IAP2_STACK
		case enIRPT_IAP2:
			iap2stk_IRPprocess( irp );
			if( irp->complete != NULL )
			{
				(*irp->complete) ( irp->result, irp->actualLen );
			}
			break;
		#endif

		default:
			HALT;
			break;
		}

		#ifdef SYSTEM_CRASH_CHECK_ENABLE
		task_usbotg_check_cnt = 2;
		#endif
	}
}

#endif //USB_HOST_ENABLE
