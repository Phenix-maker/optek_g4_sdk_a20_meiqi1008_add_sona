#include "os_config.h"

#include "sdk_com.h"

#include "system_task.h"

#ifdef SPDIF_ENABLE
#include "spdif_process.h"
#endif

#ifdef USB_HOST_ENABLE
#include "app_media_process.h"
#endif


/*
 ****************************************************************************************
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */
#define QUEUE_DEEP_LENGTH         16

#define TASK_SYSTEM_STACK_SIZE    1024

#define DELAY_TIME_MS             50


/*
 ****************************************************************************************
 * STRUCT DEFINITION
 ****************************************************************************************
 */
// typedef struct {
// 	U32 parm;
// } system_msg_stru;


// typedef struct {
//     U64 tick;
//     QueueHandle_t task_queue;
//     TaskHandle_t pxCreatedTask;
// } SYSTEM_TASK_ENV_STRU;


SYSTEM_TASK_ENV_STRU system_task_env;

extern U8 decode_block_process_timer;

/**
 *************************************************************************************
 * @brief Count system ticks and run modeule detection
 *
 * @param[in]  param      Preserve API
 *
 *************************************************************************************
 */
static void system_task (void *param)
{
    system_msg_stru qMsg;
	decode_block_process_timer = FALSE;

    while (1)
    {
        if ( xQueueReceive( system_task_env.task_queue, (void *)&qMsg, pdMS_TO_TICKS(DELAY_TIME_MS) ) != pdTRUE)
        {
            qMsg.parm = UI_FIT_TIMER;

            #ifdef SPDIF_ENABLE
           	if ( (global_vari.audio_source == SOURCE_OPTICAL) ||
                (global_vari.audio_source == SOURCE_ARC) )
           	{
                spdif_detect_process(qMsg.parm);
           	}
            #endif

            #ifdef USB_HOST_ENABLE
           	if (global_vari.audio_source == SOURCE_USB_MSC)
           	{
                switch (usb_msc_process_id)
                {
                    case USB_MSC_DUMMY_PROCESS:
                        //app_nav_usb_msc_process(&qMsg);
                        break;
                    case USB_MSC_READ_FS_PROCESS:
                        //app_nav_usb_read_fs_process(&qMsg);
                        break;
                    case USB_MSC_MP3_PROCESS:
						if (decode_block_process_timer == FALSE)
						{
                        	app_nav_mp3_process(&qMsg);
						}
                        break;   
                    default :
                        break;
                }               
           	}
            #endif

			continue;
        }
        else
        {
            #ifdef SPDIF_ENABLE
           	if ( (global_vari.audio_source == SOURCE_OPTICAL) ||
                (global_vari.audio_source == SOURCE_ARC) )
           	{
                spdif_detect_process(qMsg.parm);
            }
            #endif

            #ifdef USB_HOST_ENABLE
           	if (global_vari.audio_source == SOURCE_USB_MSC)
           	{
                switch (usb_msc_process_id)
                {
                    case USB_MSC_DUMMY_PROCESS:
                        //app_nav_usb_msc_process(&qMsg);
                        break;
                    case USB_MSC_READ_FS_PROCESS:
                        app_nav_usb_read_fs_process(&qMsg);
                        break;
                    case USB_MSC_MP3_PROCESS:
                        app_nav_mp3_process(&qMsg);
                        break;                        
                    default :
                        break;
                }
           	}
            #endif
        }
        
        system_task_env.tick++;
    }
}


/**
 *************************************************************************************
 * @brief Create system task
 *************************************************************************************
 */
void system_task_init(void)
{
    system_task_env.task_queue = xQueueCreate( QUEUE_DEEP_LENGTH, sizeof(system_msg_stru) );

	if (xTaskCreate(system_task, "system task", TASK_SYSTEM_STACK_SIZE, 0, TASK_SYSTEM_PRIORITY, &system_task_env.pxCreatedTask) != TRUE)
    {
		DBG_Assert(FALSE);
    }
    else
    {

    }
}


/**
 *************************************************************************************
 * @brief Create system task
 *
 * @param[in]  parm      parameter send to system_task_env.task_queue
 *
 *************************************************************************************
 */
void system_msgSend ( U32 parm )
{
	system_msg_stru qMsg;

    qMsg.parm = parm;

	if ( xQueueSend( system_task_env.task_queue, (void *)&qMsg, 0 ) != pdTRUE )
	{
		printf_error("system task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}
