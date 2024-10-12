#include "os_config.h"

#include "hw_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "event_groups.h"
#include "mem_reloc.h"

#include "audio_com.h"

#ifdef USB_HOST_ENABLE
#include "usbd.h"
#endif

#include "app_media.h"
#include "system_task.h"

#include "app_media_config.h"


#if ( (defined USB_HOST_ENABLE) || (defined USB_DEV_ENABLE) )

const U16 ROOT_DIR[] = {0x5C,0x0};
const U16 PARENT_DIR[] = {'.','.',0x0};
const U16 ALL_FILE[] = { (U16) '*', (U16) '.', (U16) '*', 0x0};


const U16 RECORD_FOLDER[] = {'R','E','C','O','R','D',0x0};
const U16 SYSTEM_FOLDER[] = {'S','y','s','t','e','m',0x0};
const U16 usb_test_file[]={'u','s','b','t','e','s','t','.','b','i','n',0x0};


//U16 file_name [MAX_DISP_FILE_FOLDER_NAME+1];
//U16 folder_name[MAX_DISP_FILE_FOLDER_NAME+1];


#define	USB_WAIT_IRP_EVENT_MAX_TIME	(3000 / OS_MSEC_PER_TICK + 1)	//3s
#define	USB_WAIT_HTD_EVENT_MAX_TIME	(1000)			//FIXME

volatile U8 usbDevType;
U8 ChPair51 = 0;
//U8 gUSBMesg;


#ifdef USB_HOST_ENABLE
extern U8 usb_lunNum[USB_LUN_OPEN_MAX];
extern volatile U8 usb_lunCount;
extern volatile U8 usb_lunIndex;
extern U8 wMediaMute;
#endif //USB_HOST_ENABLE

void *mutex_fs = NULL;
void *mutex_usb = NULL;
void *mutex_stream = NULL;


#ifdef USB_HOST_ENABLE
U8 micFifo_buf[MIC_FIFO_BUF_SIZE];
#endif //USB_HOST_ENABLE

void *gUsbQueue;



//The event's functions is used for the variable of "event_grop" .
int os_event_set(unsigned long event_mask)
{
	unsigned long ret;

	ret = xEventGroupSetBits(global_vari.eventGroup, event_mask);
	if ( (ret & event_mask) != event_mask)
	{
		return FALSE;
	}

	return TRUE;
}

int os_event_iset(unsigned long event_mask)
{
	unsigned long ret;

	BaseType_t xHigherPriorityTaskWoken, xResult;

	// xHigherPriorityTaskWoken must be initialised to pdFALSE.
	xHigherPriorityTaskWoken = pdFALSE;

    xResult = xEventGroupSetBitsFromISR( global_vari.eventGroup, event_mask, xHigherPriorityTaskWoken);
	if( xResult == pdPASS)
	{
		//If the priority of the timer daemon task is higher than the priority of the currently running task (the task the interrupt interrupted)
        if (xHigherPriorityTaskWoken)
        {
		    portYIELD_FROM_ISR();
        }
	}
    else
    {
		//pdFALSE will be returned if the timer service queue was full
        DBG_Assert(FALSE); 
    }

	return TRUE;
}

int os_event_clr(unsigned long event_mask)
{
	unsigned long ret;

	ret = xEventGroupClearBits(global_vari.eventGroup, event_mask);
	if ( (ret & event_mask) != event_mask)
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

int os_event_iclr(unsigned long event_mask)
{
	unsigned long ret;

	ret = xEventGroupClearBitsFromISR(global_vari.eventGroup, event_mask);
	if ( (ret & event_mask) != event_mask)
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

int os_event_get(unsigned long event_mask)
{
	unsigned long ret;

	ret = xEventGroupWaitBits(global_vari.eventGroup, event_mask, pdTRUE, pdFALSE, portMAX_DELAY);
	if ( (ret & event_mask) != event_mask)
	{
		DBG_Assert(FALSE);
	}

	return ret;
}

int os_event_time_out_get(unsigned long event_mask, xTicksWaitTime)
{
	unsigned long ret;

	ret = xEventGroupWaitBits(global_vari.eventGroup, event_mask, pdTRUE, pdFALSE, xTicksWaitTime);
	if ( (ret & event_mask) != event_mask)
	{
		DBG_Assert(FALSE);
	}

	return ret;
}


int os_event_get_for_wait_xms(unsigned long event_mask, unsigned long wait_xms)
{
	unsigned long ret;
	unsigned long wait_time_ms;

	wait_time_ms = ( (wait_xms + OS_MSEC_PER_TICK - 1) / OS_MSEC_PER_TICK );

	ret = xEventGroupWaitBits(global_vari.eventGroup, event_mask, pdTRUE, pdFALSE, wait_time_ms);
	if ( (ret & event_mask) != event_mask)
	{
		DBG_Assert(FALSE);
	}

	return ret;
}

int os_queue_send(void *queueID, unsigned long *msg_buf)
{
	U32 ret;

	ret = xQueueSend( queueID, (void *)msg_buf, 0 );
	if (ret != TRUE)
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

int os_queue_isend(void *queueID, unsigned long *msg_buf)
{
	U32 ret;

	BaseType_t xHigherPriorityTaskWoken, xResult;

    //xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    xResult = QUEUE_AUDIO_IN_RX_iSEND(queueID, msg_buf, xHigherPriorityTaskWoken);
    if( xResult == pdPASS)
    {
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }
    else
    {
        DBG_Assert(FALSE);
    }

	return TRUE;
}

int os_queue_send_wait_forever(void *queueID, unsigned long *msg_buf)
{
	U32 ret;

	ret = xQueueSend( queueID, (void *)msg_buf, portMAX_DELAY);
	if (ret != TRUE)
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

int os_queue_receive(void *queueID, unsigned long *msg_buf)
{
	U32 ret;

	ret = xQueueReceive( queueID, (void *)msg_buf, portMAX_DELAY );
	if (ret != TRUE)
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

int os_queue_receive_for_wait_xms(void *queueID, unsigned long *data, unsigned long wait_xms)
{
	unsigned long ret;
	unsigned long wait_time_ms;

	wait_time_ms = ( (wait_xms + OS_MSEC_PER_TICK - 1) / OS_MSEC_PER_TICK );

	ret = xQueueReceive( queueID, (void *)data, wait_time_ms );
	if (ret != TRUE)
	{
		//if using TX_NO_WAIT, it can be here.
		return FALSE;
	}

	return TRUE;
}

int os_queue_ireceive(void *queueID, unsigned long *msg_buf)
{
	return FALSE;
}

int os_queue_clear(void *queueID)
{
	unsigned long ret;

	ret = xQueueReset(queueID);
	if (ret)
	{
		ret = TRUE; //flush queue successfully
	}
	else
	{
		ret = FALSE; //flush queue failed
	}

	return ret;	
}

void uHddMsgSend (U16 msg, U32 parm1, U16 parm2)
{
#if (defined USB_HOST_ENABLE || defined SD_ENABLE)
	HDD_MSG uMsg;
	U32 *data = (U32 *)&uMsg;

	uMsg.msg = msg;
	uMsg.parm1 = parm1;
	uMsg.parm2 = parm2;

	QUEUE_HDD_SEND;
#endif
}

void usbmsg_sendIRP( void * irp )
{
	U32 *data = (U32 *)irp;

	USB_IRP_EVENT_CLR;
	QUEUE_USB_SEND;
}

void usbmsg_isendIRP( void * irp )
{
	U32 *data = (U32 *)irp;
	QUEUE_USB_iSEND;
}


void usbmsg_IRPcompleteFunc( void )
{
	USB_IRP_EVENT_SET;
}

void usbmsg_waitIRPcomplete(void)
{
	USB_IRP_EVENT_GET;
}

void usbmsg_setHTDevent( void )
{
	USB_HTD_EVENT_iSET;
}

void usbmsg_clearHTDevent(void )
{
	USB_HTD_EVENT_CLR;
}

int usbmsg_waitHTDevent( void )
{
	int temp;
	//if USB_HTD_EVENT_GET called a funtion
	temp = USB_HTD_EVENT_GET;
	return temp;
}

int os_mutex_lock(unsigned long *mutexID)
{
	if (mutexID == NULL)
	{
		return FALSE;
	}

	if (xSemaphoreTake( mutexID, portMAX_DELAY) != TRUE)
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

int os_mutex_ilock(unsigned long *mutexID)
{
	if (mutexID == NULL)
	{
		return FALSE;
	}

	if (xSemaphoreTakeFromISR( mutexID, NULL) != TRUE)
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

int os_mutex_lock_for_timeout(unsigned long *mutexID, unsigned long wait_xms)
{
	int wait_time_ticks;


	if (mutexID == NULL)
		return FALSE;

#ifdef RTOS_ENABLE

	wait_time_ticks = ( (wait_xms + OS_MSEC_PER_TICK - 1) / OS_MSEC_PER_TICK );

	#if defined THREAD_RTOS
	if (tx_mutex_get( (TX_MUTEX *)mutexID, wait_time_ticks) != TX_SUCCESS)
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
	#elif defined FREE_RTOS
	if (xSemaphoreTake( mutexID, wait_time_ticks) != TRUE)
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
	#else
	return FALSE;
	#endif

#else

	return FALSE;

#endif //RTOS_ENABLE
}

int os_mutex_unlock(unsigned long *mutexID)
{

	if (mutexID == NULL)
		return FALSE;

	if( xSemaphoreGive( mutexID ) != TRUE )
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

int os_mutex_iunlock(unsigned long *mutexID)
{

	if (mutexID == NULL)
		return FALSE;

	if( xSemaphoreGiveFromISR( mutexID, NULL ) != TRUE )
	{
		DBG_Assert(FALSE);
		return FALSE;
	}

	return TRUE;
}

int fs_thread_lock(void)
{
	unsigned int ret;

	ret = os_mutex_lock(&mutex_fs);
	return ret;
}

int fs_thread_unlock(void)
{
	unsigned int ret;

	ret = os_mutex_unlock(&mutex_fs);
	return ret;
}

U8 audio_iso_mute;
void app_audio_iso_rcv_enable (void)
{
	audio_iso_mute = FALSE;
}

void app_audio_iso_rcv_disable (void)
{
	audio_iso_mute = TRUE;
}

BOOL uiDacMute;

void app_dac_mute_enable (void)
{
	uiDacMute = TRUE;
	decode_pasue_set();
}

void app_dac_mute_disable (void)
{
	uiDacMute = FALSE;
	decode_pasue_clr();
}

void app_cmd_DecoderExit(void)
{
	//force the hdd to idle state
	app_media_mute_enable();
	app_nav_sleep(5);
	
	STREAM_MUTEX_LOCK;
	STREAM_MUTEX_UNLOCK;

	/*
	*app_dac_mute_eanble
	*/
	decode_pasue_clr();

	uiDecDecodeFrameExit ();
}

void app_nav_esdProtectTimerOut_callback(void)
{
	DBG_Printf("esdProtectTimerOut_callback\r\n");
}

#endif //( (defined USB_HOST_ENABLE) || (defined USB_DEV_ENABLE) )