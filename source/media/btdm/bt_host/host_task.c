#include "os_config.h"

#include "sdk_com.h"

#include "btdm_task.h"
#include "interrupt.h"
#include "hw_timer.h"
#include "hal_uart.h"
#include "hw_timer.h"
#include "fifo.h"
#include "ht_hci_main.h"
#include "btdm_task.h"

#ifndef BTDM_USE_ONETASK
/*
 ****************************************************************************************
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */
#define TASK_HOST_STACK_SIZE    2048

/*
 ****************************************************************************************
 * STRUCT DEFINITION
 ****************************************************************************************
 */

typedef struct {
    U64 tick; 
    TaskHandle_t pxCreatedTask;
} HOST_TASK_ENV_STRU;

HOST_TASK_ENV_STRU host_task_env;

void _taskBtHCI (void *pvParameters)
{
  U32 uxBits;

  btdm_host_init();
  while(1)
  {
    uxBits = xEventGroupWaitBits(global_vari.eventGroup, UART0_RX_EVENT|BT_HOST_INTERNAL_MSG, pdTRUE, pdFALSE, 100);

    if ((uxBits & UART0_RX_EVENT) == UART0_RX_EVENT)
    {
      btdm_host_schedule(RX_EVENT);
    }
    
    if ((uxBits & BT_HOST_INTERNAL_MSG) == BT_HOST_INTERNAL_MSG)
    {
      btdm_host_schedule(MSG_EVENT);
    }

    if ((uxBits & (UART0_RX_EVENT | BT_HOST_INTERNAL_MSG)) == 0 )
    {
      btdm_host_schedule(_100MS_TICK_EVENT);
    }
  }
}
#endif

void taskBtHCI (void *pvParameters);
/**
 *************************************************************************************
 * @brief Create host task
 *************************************************************************************
 */
void host_task_init(void)
{
#ifndef BTDM_USE_ONETASK  
  if (xTaskCreate(_taskBtHCI, "host task", TASK_HOST_STACK_SIZE, 0, TASK_HOST_PRIORITY, &host_task_env.pxCreatedTask) != TRUE)
  {
    DBG_Assert(FALSE);
  }
  else
  {

  }
#endif  
}