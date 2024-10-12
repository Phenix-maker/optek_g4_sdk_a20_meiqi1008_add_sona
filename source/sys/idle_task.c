#include "os_config.h" 

#include "hw_timer.h"
#include "hw_watchdog.h"

#include "idle_task.h"

/*
 ****************************************************************************************
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */

#define TASK_SYSTEM_STACK_SIZE 1024


/*
 ****************************************************************************************
 * STRUCT DEFINTIONS
 ****************************************************************************************
 */
typedef struct {
    U32 tick;
    TaskHandle_t pxCreatedTask;
} IDLE_TASK_ENV_STRU;

IDLE_TASK_ENV_STRU idle_task_env;

#ifdef FREE_RTOS_DEBUG
static uint8_t CPU_RunInfo[256];
#endif


/*
 ****************************************************************************************
 * FUNCTION DEFINTIONS
 ****************************************************************************************
 */
static void idle_task (void *param)
{
    while(1)
    {
		#if ((defined WATCH_DOG) || !(defined JTAG_DEBUG_MODE))
		watchdog_time_reset();
		#endif

		#ifdef FREE_RTOS_DEBUG
		delayms(10000);//display status of cpu every 10s
		vTaskGetRunTimeStats((char *)CPU_RunInfo);
		printf_info("%s\r\n", CPU_RunInfo);
		#endif

		#if 1//(defined REDUCE_POWER_COMSUMPTION)
		asm("waiti 0");
		#endif

		asm("nop");
    }
}

void idle_task_init(void)
{
	if (xTaskCreate(idle_task, "idle task", TASK_SYSTEM_STACK_SIZE, 0, TASK_IDLE_PRIORITY, &idle_task_env.pxCreatedTask) != TRUE)
    {
		DBG_Assert(FALSE);
    }
}
