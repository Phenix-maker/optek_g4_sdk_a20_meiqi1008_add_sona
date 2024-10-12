/*
 *  Copyright (C) Optek
 *  All Rights Reserved
 *
 *  Model specific rtos configuration file.
 */

#ifndef _OS_CONFIG_H_
#define _OS_CONFIG_H_

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "os_type.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"

#include "sdk_com.h"
#include "sdk_config.h"
#include "app_task.h"

#include "interrupt_control.h"


#define OS_MSEC_PER_TICK          configTICK_RATE_HZ

#define SYSTEM_TIMER_PERIOD_MS    5


#define XT_INTS_ON(X)    xt_ints_on(X)
#define XT_INTS_OFF(X)   xt_ints_off(X)


#define TX_INTERRUPT_SAVE_AREA \
                          register int interrupt_save

//interrupt enable or disable
//They cannot be called from interrupt level.

#define TX_DISABLE       interrupt_save = _interrupt_control(XCHAL_EXCM_LEVEL)

#define TX_RESTORE       _interrupt_control(interrupt_save)

//They can be called from interrupt level.
#define TX_iDISABLE       interrupt_save = _interrupt_control(XCHAL_EXCM_LEVEL)

#define TX_iRESTORE      _interrupt_control(interrupt_save)


#define TASK_SLEEP_MS(xms)    vTaskDelay( pdMS_TO_TICKS(xms) );



#define QUEUE_AUDIO_IN_RX_iSEND(qID, msg_buf, xHigherPriorityTaskWoken) \
                                                xQueueSendToBackFromISR( qID, msg_buf, &xHigherPriorityTaskWoken )

#define QUEUE_AUDIO_IN_RX_SEND(qID, msg_buf)    xQueueSend(qID, msg_buf, 0)

#define QUEUE_AUDIO_IN_RX_GET(qID, msg_buf)     xQueueReceive(qID, msg_buf, portMAX_DELAY )

#define QUEUE_AUDIO_DECODE_SET(qID, msg_buf, xTicksWaitTime) \
                                                xQueueSend(qID, msg_buf, xTicksWaitTime)

#define QUEUE_AUDIO_DECODE_GET(qID, msg_buf, xTicksWaitTime) \
                                                xQueueReceive(qID, msg_buf, xTicksWaitTime)


#define EVENT_AUDIO_INPUT_RX_CLR(xEventGroup)   xEventGroupClearBits(xEventGroup, AUDIO_RECEIVE_EVENT)

#define EVENT_AUDIO_INPUT_RX_SET(xEventGroup) \
                                        xEventGroupSetBits(\
						                xEventGroup,\
						                AUDIO_RECEIVE_EVENT,\
						                &xHigherPriorityTaskWoken )


#define EVENT_AUDIO_INPUT_RX_iSET(xEventGroup, xHigherPriorityTaskWoken) \
                                        xEventGroupSetBitsFromISR(\
						                xEventGroup,\
						                AUDIO_RECEIVE_EVENT,\
						                &xHigherPriorityTaskWoken )


#define EVENT_AUDIO_INPUT_RX_GET(xEventGroup, xTicksWaitTime) \
                                        xEventGroupWaitBits(xEventGroup, AUDIO_RECEIVE_EVENT, pdTRUE, pdFALSE, xTicksWaitTime)


#define EVENT_AUDIO_DECODE_CLR(xEventGroup) \
                                        xEventGroupClearBits(xEventGroup, AUDIO_DECODE_EVENT)

//It can be called  from an interrupt routine 
#define EVENT_AUDIO_DECODE_iCLR(xEventGroup) \
                                        xEventGroupClearBitsFromISR(xEventGroup, AUDIO_DECODE_EVENT)

#define EVENT_AUDIO_DECODE_SET(xEventGroup) \
                                        xEventGroupSetBits(xEventGroup, AUDIO_DECODE_EVENT)

//It can be called from an interrupt routine
#define EVENT_AUDIO_DECODE_iSET(xEventGroup, xHigherPriorityTaskWoken) \
                                        xEventGroupSetBitsFromISR(\
						                xEventGroup,\
						                AUDIO_DECODE_EVENT,\
						                &xHigherPriorityTaskWoken )

#define EVENT_AUDIO_DECODE_GET(xEventGroup, xTicksWaitTime) \
                                        xEventGroupWaitBits(xEventGroup, AUDIO_DECODE_EVENT, pdTRUE, pdFALSE, xTicksWaitTime)

#endif //_OS_CONFIG_H_
