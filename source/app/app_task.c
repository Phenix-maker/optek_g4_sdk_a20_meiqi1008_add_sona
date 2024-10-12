#include "c_def.h"

#include "os_config.h"

#include "debug.h"

#include "sdk_com.h"
#include "app_entry.h"
#include "app_window.h"
#include "app_task.h"

#ifdef BTDM52_ENABLE
#include "bt_common.h"
#endif

#ifdef SAVE_MODULE_ENABLE
#include "app_save.h"
#endif

#ifdef AUDIO_PROCESSOR
#include "audio_device.h"
#endif

/*
 ****************************************************************************************
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */
#define QUEUE_DEEPTH_LENGTH        32

#define TASK_APP_FUNC_STACK_SIZE   (2048 +2048)

typedef enum {
	enTYPE_FUNC_WO_PARAM = 0,
	enTYPE_FUNC_W_1PARAM,
	enTYPE_FUNC_W_2PARAM,
	enTYPE_KEY
} enumMSG_TYPE;

/*
 ****************************************************************************************
 * STRUCT DEFINITION
 ****************************************************************************************
 */
typedef struct {
	U32 msg_type;

	union
    {
		struct {
			pFUNCTION pfunc;
		} func_wo_parm;

		struct {
			pFUNCTION_W_ONE_PARM pfunc;
			U32 param;
		} func_w_1parm;

		struct {
			pFUNCTION_W_TWO_PARM pfunc;
			U32 param1;
			U32 param2;
		} func_w_2parm;

		struct {
			U16 key_soruce;
			U16 key_action;
			U32 key_id;
			U32 action_moment;//ues system timer tick
		} key;
    };

} APP_MSG_STRU;


struct task_timer {
	U32 nearest_timer_id;
	U32 nearest_timer_tick;
	U32 task_timers[TIMER_MAX];
};

typedef struct {
	struct task_timer timer;
    QueueHandle_t task_queue;
    TaskHandle_t task_handle;
} APP_TASK_ENV_STRU;


APP_TASK_ENV_STRU app_task_env = {0};

/*
 ****************************************************************************************
 * key process interface
 ****************************************************************************************
 */
typedef enum {
	KEY_PRESSED,
	KEY_RELEASED,
	KEY_WAIT_COMB_KEY
} enumKEY_PROCESS_STATUS;


typedef struct {
	U8 key_lock;
	U8 processing_key_status;
	U32 processing_key_source;
	U32 processing_key_id;
	U32 combo_time;
	KEY_HANDLE_PARAM_STRU key_param;
	WINDOW *processing_key_window;
} KEY_EVENT_PROCESS_ENVS;

static KEY_EVENT_PROCESS_ENVS key_process_envs;

void app_key_lock(U8 enable)
{
	key_process_envs.key_lock = enable;
}


/*
 ****************************************************************************************
 * task timer interface
 ****************************************************************************************
 */
static void task_timer_init(void)
{
	U32 i;

	for (i = 0; i < sizeof(app_task_env.timer.task_timers)/sizeof(app_task_env.timer.task_timers[0]); i++)
	{
		app_task_env.timer.task_timers[i] = portMAX_DELAY;
	}

	app_task_env.timer.nearest_timer_tick = portMAX_DELAY;
	app_task_env.timer.nearest_timer_id = TIMER_MAX;	
}

static void task_timer_update_nearest_timer(void)
{
	U32 i;

	if (xTaskGetCurrentTaskHandle() != app_task_env.task_handle)
	{
		func_wo_param_msgSend(task_timer_update_nearest_timer);
		return;
	}

	if (TIMER_MAX)
	{
		app_task_env.timer.nearest_timer_tick = app_task_env.timer.task_timers[0];
		app_task_env.timer.nearest_timer_id = 0;

		for (i = 1; i < sizeof(app_task_env.timer.task_timers)/sizeof(app_task_env.timer.task_timers[0]); i++)
		{
			if (app_task_env.timer.nearest_timer_tick > app_task_env.timer.task_timers[i])
			{
				app_task_env.timer.nearest_timer_tick = app_task_env.timer.task_timers[i];
				app_task_env.timer.nearest_timer_id = i;
			}
		}
	}
}

static void app_task_timer_tick_set(U32 timer_id, U32 tick)
{
	U32 old_tick;

	if (timer_id < TIMER_MAX)
	{
		if (xTaskGetCurrentTaskHandle() != app_task_env.task_handle)
		{
			func_w_two_param_msgSend(app_task_timer_tick_set, timer_id, tick);
			return;
		}

		old_tick = app_task_env.timer.task_timers[timer_id];

		app_task_env.timer.task_timers[timer_id] = tick;

		if (app_task_env.timer.task_timers[timer_id] < old_tick)
		{
			if (app_task_env.timer.nearest_timer_tick > app_task_env.timer.task_timers[timer_id])
			{
				app_task_env.timer.nearest_timer_tick = app_task_env.timer.task_timers[timer_id];
				app_task_env.timer.nearest_timer_id = timer_id;
			}
		}
		else if (app_task_env.timer.task_timers[timer_id] > old_tick)
		{
			if (app_task_env.timer.nearest_timer_id == timer_id)
			{
				task_timer_update_nearest_timer();
			}
		}
	}
	else
	{
		printf_error("timer id is not correct\r\n");
	}
}

void app_task_timer_ms_set(U32 timer_id, U32 ms)
{
	if (ms)
	{
		app_task_timer_tick_set(timer_id, xTaskGetTickCount() + pdMS_TO_TICKS(ms));
	}
	else
	{
		app_task_timer_tick_set(timer_id, portMAX_DELAY);
	}
}

static void app_task (void *param)
{
	APP_MSG_STRU qMsg;
	APP_NAV_KEY *pApp_Nav_Key;
	I32 diff_tick;
	U32 timer_id;

	key_process_envs.processing_key_status = KEY_RELEASED;
	key_process_envs.key_param.combo_times = 0;
	key_process_envs.key_param.combo_end = 0;

	task_timer_init();

	app_key_lock(FALSE);

	app_PostTaskSch_init();

	while(1)
	{
		if (app_task_env.timer.nearest_timer_tick != portMAX_DELAY)
		{
			//at least one timer is running
			diff_tick = (I32)(app_task_env.timer.nearest_timer_tick - xTaskGetTickCount());

			if (diff_tick <= 0)
			{
				diff_tick = 0;
			}

			if ( xQueueReceive( app_task_env.task_queue, (void *)&qMsg, diff_tick ) != pdTRUE )
			{
				diff_tick = (I32)(app_task_env.timer.nearest_timer_tick - xTaskGetTickCount());

				if (diff_tick <= 0)
				{
					// a timer expired

					timer_id = app_task_env.timer.nearest_timer_id;
					app_task_timer_tick_set(app_task_env.timer.nearest_timer_id, portMAX_DELAY);			

					switch (timer_id)
					{
						case TIMER_KeyHold:
							DBG_Assert(key_process_envs.processing_key_status == KEY_PRESSED);

							key_process_envs.combo_time = 0;

							key_process_envs.key_param.hold_times++;

							if (pApp_Nav_Key->key_hold)
							{
								U32 hold_time;
								
								hold_time = pApp_Nav_Key->key_hold(key_process_envs.key_param);
								if (hold_time)
								{
									app_task_timer_ms_set(TIMER_KeyHold, hold_time);
								}
							}
							break;

						case TIMER_CombKey:					
							DBG_Assert(key_process_envs.processing_key_status == KEY_WAIT_COMB_KEY);

							if (key_process_envs.processing_key_window != app_window_current_get())
							{
								printf_info("combo Abort cause by window change\r\n");
								key_process_envs.processing_key_status = KEY_RELEASED;
								break;
							}

							key_process_envs.key_param.combo_end = 1;
							printf_info("combo key up\r\n");

							if (pApp_Nav_Key->key_up)
							{
								pApp_Nav_Key->key_up(key_process_envs.key_param);
							}

							key_process_envs.key_param.combo_end = 0;

							//a comb key end 
							key_process_envs.processing_key_status = KEY_RELEASED;
							break;	

						case TIMER_WindowTimer:
							app_window_timer_expired();
							break;

						case TIMER_BT_Relink:
							//bt relink
							#ifdef BT_RELINK_ENABLE
							app_nav_bt_relink();
							#endif						
						break;

						case TIMER_BT_Pairing:
							//bt relink
							#ifdef BT_PAIRING_ENABLE
							bt_status.bt_main_status = enBT_PAIRING;
							if (bt_status.bt_scan_mode == enBT_BOTH_SCAN_DISABLE)
							{
								app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE);
							}
							app_task_timer_ms_set(TIMER_BT_Pairing_TimeOut, 1000);
							#endif						
						break;		

						case TIMER_BT_Pairing_TimeOut:
							//bt relink
							#ifdef BT_PAIRING_ENABLE
							bt_status.bt_main_status = enBT_DISCONNECTED;
							if (bt_status.bt_scan_mode == enBT_BOTH_SCAN_DISABLE)
							{
								app_bt_set_scan_enable_mode(enBT_BOTH_SCAN_ENABLE);
							}
							#endif						
						break;

						case TIMER_SAVE_GlobalVari:
							#ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
							app_save_system_data();
							#endif
						break;		

						case TIMER_MUTE_OFF:
						#ifdef AUDIO_AMP_DEV_ENABLE
							AudioAmpDev[0].Set_Mute(FALSE);
						#endif
						break;		
						#ifdef SONA_DSP_PC_TOOLS_BY_UART_FIFO
						case TIMER_Sona_UartData_Analyse:
                            sona_fifo_data_analyse();
                            app_task_timer_ms_set(TIMER_Sona_UartData_Analyse, 100);
							break;
						#endif 							
						default:
							DBG_Assert(FALSE);
							break;
					}	
				}

				continue;
			}
		}
		else
		{
			if ( xQueueReceive( app_task_env.task_queue, (void *)&qMsg, portMAX_DELAY ) != pdTRUE )
			{
				DBG_Assert(FALSE);
				continue;
			}			
		}

		switch (qMsg.msg_type)
		{
			case enTYPE_FUNC_WO_PARAM:
				if (qMsg.func_wo_parm.pfunc)
				{
					qMsg.func_wo_parm.pfunc();
				}
				else
				{
					DBG_Assert(FALSE);
				}
				break;

			case enTYPE_FUNC_W_1PARAM:
				if (qMsg.func_w_1parm.pfunc)
				{
					qMsg.func_w_1parm.pfunc( qMsg.func_w_1parm.param );
				}
				else
				{
					DBG_Assert(FALSE);
				}
				break;

			case enTYPE_FUNC_W_2PARAM:
				if (qMsg.func_w_2parm.pfunc)
				{
					qMsg.func_w_2parm.pfunc( qMsg.func_w_2parm.param1, qMsg.func_w_2parm.param2 );
				}
				else
				{
					DBG_Assert(FALSE);
				}
				break;

			case enTYPE_KEY:	
				switch(key_process_envs.processing_key_status)
				{
				case KEY_PRESSED:
					/*ignore the other source's keys while one key are being processed*/
					if (key_process_envs.processing_key_source != qMsg.key.key_soruce)
					{
						break;
					}

					if (qMsg.key.key_action == KEY_UP)
					{						
						printf_info("app key up:%d\n\r", key_process_envs.processing_key_id);
						app_task_timer_tick_set(TIMER_KeyHold, portMAX_DELAY);

						if (key_process_envs.processing_key_window != app_window_current_get())
						{
							//a key end
							printf_info("key Abort caused by window change\r\n");
							key_process_envs.processing_key_status = KEY_RELEASED;
							break;
						}

						if (key_process_envs.combo_time == 0)
						{
							if (pApp_Nav_Key->key_up)
							{
								pApp_Nav_Key->key_up(key_process_envs.key_param);
							}

							//a key end
							key_process_envs.processing_key_status = KEY_RELEASED;
						}
						else
						{
							app_task_timer_ms_set(TIMER_CombKey , key_process_envs.combo_time);
							key_process_envs.processing_key_status = KEY_WAIT_COMB_KEY;
						}
					}
					else
					{
						//can not get here
						key_process_envs.processing_key_status = KEY_RELEASED;
						DBG_Assert(FALSE);
					}
					break;

				case KEY_WAIT_COMB_KEY:
					/*ignore the other soruce's keys while one key are being processed*/
					if (key_process_envs.processing_key_source != qMsg.key.key_soruce)
					{
						break;
					}

					app_task_timer_tick_set(TIMER_CombKey, portMAX_DELAY);
					
					if (key_process_envs.processing_key_window != app_window_current_get())
					{
						//End this key, whether it's the same key or not
						printf_info("combo Abort cause by window change\r\n");
						key_process_envs.processing_key_status = KEY_RELEASED;
						break;
					}

					if (key_process_envs.processing_key_id == qMsg.key.key_id)
					{
						// combo key detect, so combo times increase
						key_process_envs.key_param.combo_times++;
						key_process_envs.processing_key_status = KEY_PRESSED;

						if (pApp_Nav_Key->hold_time)
							app_task_timer_tick_set(TIMER_KeyHold, pdMS_TO_TICKS(pApp_Nav_Key->hold_time) + qMsg.key.action_moment);

						key_process_envs.key_param.hold_times = 0;

						if (pApp_Nav_Key->key_down)
						{
							key_process_envs.combo_time = pApp_Nav_Key->key_down(key_process_envs.key_param);
						}
						else
						{
							DBG_Assert(FALSE);
							key_process_envs.combo_time = 0;
						}

						break;
					}
					else
					{
						// a new key in
						key_process_envs.key_param.combo_end = 1;
						printf_info("combo Abort cause by new key\r\n");

						//combo Abort
						if (pApp_Nav_Key->key_up)
						{
							pApp_Nav_Key->key_up(key_process_envs.key_param);
						}

						key_process_envs.key_param.combo_end = 0;

						//need process the new key, so not break
						key_process_envs.processing_key_status = KEY_RELEASED;
					}

				case KEY_RELEASED:

					if (qMsg.key.key_action == KEY_DOWN)
					{
						// a new key start
						key_process_envs.key_param.combo_times = 0;
						pApp_Nav_Key = search_key_handle(qMsg.key.key_id);
						
						key_process_envs.processing_key_status = KEY_PRESSED;
						key_process_envs.processing_key_source = qMsg.key.key_soruce;						
						key_process_envs.processing_key_id = qMsg.key.key_id;
						key_process_envs.processing_key_window = app_window_current_get();

						printf_info("app key down:%d\n\r", qMsg.key.key_id);

                        #ifdef WINDOW_PRINTF_ENABLE
                        printf_info("current window:%s\n\r", key_process_envs.processing_key_window->desc_string);
                        #endif

						if (pApp_Nav_Key->hold_time)
						{
							app_task_timer_tick_set(TIMER_KeyHold, pdMS_TO_TICKS(pApp_Nav_Key->hold_time) + qMsg.key.action_moment);
						}

						key_process_envs.key_param.hold_times = 0;

						if (pApp_Nav_Key->key_down)
						{
							if (key_process_envs.processing_key_source == MAIN_BOARD_KEY)
							{
								key_process_envs.combo_time = pApp_Nav_Key->key_down(key_process_envs.key_param);
							}
							else
							{
								key_process_envs.combo_time = 0;
							}
						}
						else
						{
							key_process_envs.combo_time = 0;
						}
					}		
					break;

				default:
					DBG_Assert(FALSE);
					break;
				}
				break;

			default:
				DBG_Assert(FALSE);//can not get here
				break;
		}
	}
}

void app_task_init( void )
{
    app_task_env.task_queue = xQueueCreate( QUEUE_DEEPTH_LENGTH, sizeof(APP_MSG_STRU) );
	
    if (xTaskCreate( app_task, "app task", TASK_APP_FUNC_STACK_SIZE, 0, TASK_APPFUNC_PRIORITY, &app_task_env.task_handle) != pdTRUE )
    {
		DBG_Assert(FALSE);
    }
}

void func_wo_param_msgSend ( pFUNCTION func )
{
	APP_MSG_STRU qMsg;

	qMsg.msg_type = enTYPE_FUNC_WO_PARAM;
	qMsg.func_wo_parm.pfunc = func;

	if ( xQueueSend( app_task_env.task_queue, (void *)&qMsg, 0 ) != pdTRUE )
	{
		printf_error("app func task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}

void func_wo_param_msgSend_formIsr ( pFUNCTION_W_ONE_PARM func )
{
	APP_MSG_STRU qMsg;

	qMsg.msg_type = enTYPE_FUNC_WO_PARAM;
	qMsg.func_wo_parm.pfunc = func;

	///context switch in next tick
	if ( xQueueSendFromISR( app_task_env.task_queue, (void *)&qMsg, pdFALSE ) != pdTRUE )
	{
		//queue is full
		DBG_Assert(FALSE);
	}
}

void func_w_one_param_msgSend ( pFUNCTION func, U32 parm )
{
	APP_MSG_STRU qMsg;

	qMsg.msg_type = enTYPE_FUNC_W_1PARAM;
	qMsg.func_w_1parm.pfunc = func;
	qMsg.func_w_1parm.param = parm;

	if ( xQueueSend( app_task_env.task_queue, (void *)&qMsg, 0 ) != pdTRUE )
	{
		printf_error("app func task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}

void func_w_one_param_msgSend_formIsr ( pFUNCTION_W_ONE_PARM func, U32 parm )
{
	APP_MSG_STRU qMsg;

	qMsg.msg_type = enTYPE_FUNC_W_1PARAM;
	qMsg.func_w_1parm.pfunc = func;
	qMsg.func_w_1parm.param = parm;

	//context switch in next tick
	if ( xQueueSendFromISR( app_task_env.task_queue, (void *)&qMsg, pdFALSE ) != pdTRUE )
	{
		//queue is full
		DBG_Assert(FALSE);
	}
}

void func_w_two_param_msgSend ( pFUNCTION_W_TWO_PARM func , U32 parm1, U32 parm2 )
{
	APP_MSG_STRU qMsg;

	qMsg.msg_type = enTYPE_FUNC_W_2PARAM;
	qMsg.func_w_2parm.pfunc = func;
	qMsg.func_w_2parm.param1 = parm1;
	qMsg.func_w_2parm.param2 = parm2;

	if ( xQueueSend( app_task_env.task_queue, (void *)&qMsg, 0 ) != pdTRUE )
	{
		printf_error("app func task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}

void func_w_two_param_msgSend_formIsr ( pFUNCTION_W_TWO_PARM func , U32 parm1, U32 parm2 )
{
	APP_MSG_STRU qMsg;

	qMsg.msg_type = enTYPE_FUNC_W_2PARAM;
	qMsg.func_w_2parm.pfunc = func;
	qMsg.func_w_2parm.param1 = parm1;
	qMsg.func_w_2parm.param2 = parm2;

	//context switch in next tick
	if ( xQueueSendFromISR( app_task_env.task_queue, (void *)&qMsg, pdFALSE ) != pdTRUE )
	{
		//queue is full
		DBG_Assert(FALSE);
	}
}

void key_msgSend ( U16 key_soruce , U16 action, U32 key_id)
{
	APP_MSG_STRU qMsg;

	qMsg.msg_type = enTYPE_KEY;
	qMsg.key.key_soruce = key_soruce;
	qMsg.key.key_action = action;
	qMsg.key.key_id = key_id;
	qMsg.key.action_moment = xTaskGetTickCount();

	if ( xQueueSend( app_task_env.task_queue, (void *)&qMsg, 0 ) != pdTRUE )	
	{
		printf_error("app func task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}

void key_msgSend_formIsr ( U16 key_soruce , U16 action, U32 key_id)
{
	APP_MSG_STRU qMsg;

	qMsg.msg_type = enTYPE_KEY;
	qMsg.key.key_soruce = key_soruce;
	qMsg.key.key_action = action;
	qMsg.key.key_id = key_id;
	qMsg.key.action_moment = xTaskGetTickCountFromISR();

	//context switch in next tick
	if ( xQueueSendFromISR( app_task_env.task_queue, (void *)&qMsg, pdFALSE ) != pdTRUE )
	{
		//queue is full
		DBG_Assert(FALSE);
	}
}
