#include "os_config.h"

#include "sdk_config.h"

#include "system_timer.h"
#include "app_task.h"
#include "app_timer.h"
#include "remote.h"



SYSTEM_TIMER_ENV_STRU system_timer_env;

TIMER_DATA timer_data;


/**
 ****************************************************************************************
 * @brief callback function of system timer, runnning period is 5 ms
 *
 * @param[in]  param     	Preserve API
 *
 * @return will not return
 *
 ****************************************************************************************
 */
static void system_timer_handler (void *param)
{
    system_timer_env.timer_tick++;

	#ifdef REMOTE_ENABLE
	if (timer_data.remoteKeyUpTimer)
	{
		timer_data.remoteKeyUpTimer--;
		if (timer_data.remoteKeyUpTimer == 0)
		{
			remote_repeat_valid = FALSE;

			if (remote_key_state == KEY_DOWN)
			{
				remote_key_state = KEY_UP;
				key_msgSend( REMOTE_KEY, KEY_UP, remote_key);
				RmtValPrintf("RMT:key up 0x%x\n\r", remote_key);
			}
		}
	}
    #endif

    app_5ms_timer(system_timer_env.timer_tick);
}


/**
 ****************************************************************************************
 * @brief Initialize function of system timer
 *
 * @param[out]  timerID     						Timer ID
 * @param[out]  system_timer_env.timer_tick     	Timer tick count
 * 
 * @return will not return
 *
 ****************************************************************************************
 */
void system_timer_data_init(void)
{
	memset((void *)&timer_data, 0, sizeof(timer_data));
	memset((void *)&app_timer_data, 0, sizeof(app_timer_data));
}

void system_timer_init(void)
{
	int timerID = 0;

    system_timer_env.timer_tick = 0;

    /* create system timer with automatic reload */
	system_timer_env.handler = xTimerCreate(
									"system Timer",                	       /* Text name to facilitate debugging.  The kernel does not use this itself. */
									pdMS_TO_TICKS(SYSTEM_TIMER_PERIOD_MS), /* The period for the timer.  The plus 1 ensures a period of zero is not specified. */
									pdTRUE,                                /* Auto-reload is set to true. */
									( void * )&timerID,                    /* An identifier for the timer as all the auto reload timers use the same callback. */
									system_timer_handler);                 /* The callback to be called when the timer expires. */

	if (system_timer_env.handler == NULL)
	{
		DBG_Assert( FALSE );
	}
	else
	{
		if ( xTimerStart( system_timer_env.handler, 0) != pdPASS )
		{
 			DBG_Assert( pdFAIL );
		}
	}
}

void app_timer_remoteKeyUpTimer_set(U32 ms_time)
{
	timer_data.remoteKeyUpTimer = ms_time/SYSTEM_TIMER_PERIOD_MS;
}


/**
 ****************************************************************************************
 * @brief Get current timer tick count
 *
 * @param[out]  system_timer_env.timer_tick     	system timer tick count
 *
 * @return will not return
 *
 ****************************************************************************************
 */
U32 system_timer_get(void)
{
    return system_timer_env.timer_tick;
}
