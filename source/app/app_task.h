#ifndef __APP_TASK_H__
#define __APP_TASK_H__

#include "sdk_com.h"



/**
 ****************************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef enum {
	KEY_DOWN,
	KEY_UP,
	KEY_HOLD
} enumKEY_ACTIONs;

typedef enum {
	MAIN_BOARD_KEY,//local key
	REMOTE_KEY,
	CONSOLE_KEY,
	KEY_TYPE_MAX
} enumKeyType;

typedef enum {
	TIMER_KeyHold = 0,
	TIMER_CombKey,
	TIMER_WindowTimer,
	TIMER_BT_Relink,
	TIMER_BT_Pairing,
	TIMER_BT_Pairing_TimeOut,
	TIMER_SAVE_GlobalVari,
	TIMER_LED_CONTRL,
	TIMER_MUTE_OFF,
    #ifdef SONA_DSP_PC_TOOLS_BY_UART_FIFO
	TIMER_Sona_UartData_Analyse,
    #endif
	TIMER_MAX,
} enAPP_TASK_TIMER_ID;


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void app_task_init(void);
void func_wo_param_msgSend ( pFUNCTION func );
void func_wo_param_msgSend_formIsr ( pFUNCTION_W_ONE_PARM func );
void func_w_one_param_msgSend ( pFUNCTION func, U32 parm );
void func_w_one_param_msgSend_formIsr ( pFUNCTION_W_ONE_PARM func, U32 parm );
void func_w_two_param_msgSend ( pFUNCTION_W_TWO_PARM func, U32 parm1, U32 parm2 );
void func_w_two_param_msgSend_formIsr ( pFUNCTION_W_TWO_PARM func, U32 parm1, U32 parm2 );

void key_msgSend ( U16 key_type , U16 action, U32 key_id);
void key_msgSend_formIsr ( U16 key_type , U16 action, U32 key_id);

void app_task_timer_ms_set(U32 timer_id, U32 ms);


#endif //__APP_TASK_H__
