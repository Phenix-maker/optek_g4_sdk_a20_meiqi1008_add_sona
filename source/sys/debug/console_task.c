/**
 ****************************************************************************************
 *
 * @file console_cmd.c
 *
 * @brief console command (from uart etc.) implement.
 *
 ****************************************************************************************
 */

#include "os_config.h"

#include "hw_timer.h"
#include "hal_uart.h"

#include "app_task.h"
#include "app_window.h"

#ifdef DSPEQ_SETTING_ENABLE
#include "optek_eq_tune_by_uart.h"
#endif

#ifdef DSP_HIFI_LIB_OLD
#include "optek_hifi2_dapp.h"
#else
#include "optek_hifi2_dapp_strided.h"
#endif

/*
 * command parser states
 ****************************************************************************************
 */
typedef enum 
{
	WAIT_NEXT_CMD = 0,
    RECEIVING_CMD_HEADER,	
    RECEIVING_CMD_PARAM,
} enumCONSOLE_CMD_STATE;

#define CONSOLE_CMD_TEST                  1
#define CONSOLE_extCMD_TEST               2
#define CONSOLE_CMD_SYSTEM_SUSPEND        3
#define CONSOLE_CMD_CHANGE_TASK_PRIORITY  4
#define CONSOLE_CMD_KEY_SHORT             5
#define CONSOLE_CMD_KEY_HOLD              6
#define CONSOLE_CMD_KEY_OPERATE           7

#ifdef DSP_HIFI_LIB_OLD
#define CONSOLE_extCMD_DSP				  8
#else
#define CONSOLE_extCMD_LR_DSP			  8
#define CONSOLE_extCMD_SUB_DSP		  	  10
#define CONSOLE_extCMD_CENTRE_DSP		  11
#define CONSOLE_extCMD_DRC_DSP			  12
#define CONSOLE_extCMD_DSP				  13
#endif

#ifdef DSP_EQ_SETTING_ENABLE
#define CONSOLE_extCMD_EQ_DSP		  9
#endif

#define CONSOLE_CMD_ID_MAX                0xFF

#define CMD_PRE_HEADER       0X5A
#define extCMD_PRE_HEADER    0XA5//extern param

typedef struct {
	U16 cmd_id;
	U16 param_buffer_size;
	U8 *param_buffer;
	void (*handler)(u16 param);
	char *help_string;
} CMD_HANDLE_STRU;

typedef struct {
	U8 pre_header;//CMD_PRE_HEADER or extCMD_PRE_HEADER
	U8 cmd_id;
	U8 param_h;//reuse for extcmd, means param len 
	U8 param_l;
} __attribute__((packed)) CMD_HEADER_STRU;

typedef struct {
    // pointer to External interface api
    const extif_api_stru* ext_if;	
	U8 cmd_parse_state;
	U16 param_len;
	U32 console_cmd_monitor;
	CMD_HEADER_STRU cmd_header;
	const CMD_HANDLE_STRU *pcmd_handle;
	TimerHandle_t timer_handle; 
} CONSOLE_CMD_ENV_STRU;

CONSOLE_CMD_ENV_STRU console_cmd_env;

void handler_for_test(U16 param)
{
	printf_info("hello console command:%d\n",param);
}

void system_suspend(U16 param)
{
	TX_INTERRUPT_SAVE_AREA;

	printf_info("suspend the system\n");

	delayms(10);
	TX_DISABLE;
	while(1);

}


void task_priority_change(U16 param)
{

}

void sim_key_short_hit(U16 param)
{

}

void sim_key_hold(U16 param)
{
	
}

void key_operate(U16 param)
{
	printf_info("key cmd received\r\n");
	key_msgSend(CONSOLE_KEY, KEY_DOWN, param);
	key_msgSend(CONSOLE_KEY, KEY_UP, param);
}

U8 extCMD_test_buff[100];
void extCMD_handler_for_test(U16 param_len)
{
	printf_info("extern console command: param len:%d , param[0]:%d, last param:%d\n", param_len, extCMD_test_buff[0], extCMD_test_buff[param_len-1]);
}


#ifndef DSPEQ_SETTING_ENABLE
#define DSP_DATA_BYTE_LEN 504
#ifdef DSP_HIFI_LIB_OLD
U8 extCMD_Uart_rx_buff[DSP_DATA_BYTE_LEN];
void extCMD_handler_for_DSP_data(U16 param_len)
{
 	DBG_Printf("%s\n\r",__func__);
	printf_info("extern console command: param len:%d , param[0]:%d, last param:%d\n", param_len, extCMD_Uart_rx_buff[0], extCMD_Uart_rx_buff[param_len-1]);
 	app_nav_eq_set_process((OPTEK_DSP_RXs *)extCMD_Uart_rx_buff);
}
#endif
#else
#define DSP_DATA_BYTE_LEN 504
U8 extCMD_Uart_rx_Lr_buff[DSP_DATA_BYTE_LEN];
void extCMD_handler_for_Lr_DSP_data(U16 param_len)
{
	DBG_Printf("%s\n\r",__func__);
	printf_info("extern console command: param len:%d , param[0]:%d, last param:%d\n", param_len, extCMD_Uart_rx_Lr_buff[0], extCMD_Uart_rx_Lr_buff[param_len-1]);

	app_nav_eq_set_process(optek_dsp_lr, (OPTEK_DSP_RXs *)extCMD_Uart_rx_Lr_buff);

}

U8 extCMD_Uart_rx_Centre_buff[DSP_DATA_BYTE_LEN];
void extCMD_handler_for_Centre_DSP_data(U16 param_len)
{
	DBG_Printf("%s\n\r",__func__);
	printf_info("extern console command: param len:%d , param[0]:%d, last param:%d\n", param_len, extCMD_Uart_rx_Centre_buff[0], extCMD_Uart_rx_Centre_buff[param_len-1]);
	
	app_nav_eq_set_process(optek_dsp_center, (OPTEK_DSP_RXs *)extCMD_Uart_rx_Centre_buff);

}

U8 extCMD_Uart_rx_Sub_buff[DSP_DATA_BYTE_LEN];
void extCMD_handler_for_Sub_DSP_data(U16 param_len)
{
	DBG_Printf("%s\n\r",__func__);
	printf_info("extern console command: param len:%d , param[0]:%d, last param:%d\n", param_len, extCMD_Uart_rx_Sub_buff[0], extCMD_Uart_rx_Sub_buff[param_len-1]);
	
	app_nav_eq_set_process(optek_dsp_sub, (OPTEK_DSP_RXs *)extCMD_Uart_rx_Sub_buff);

}
#endif

#ifdef DSP_EQ_SETTING_ENABLE
U8 extCMD_Uart_eq_rx_buff[1024 * 4];

void extCMD_handler_for_EQ_DSP_data(U16 param_len)
{
	DBG_Printf("%s\n\r", __func__);
	printf_info("eq command: param len:%d , param[0]:%d, last param:%d\n", param_len, extCMD_Uart_eq_rx_buff[0], extCMD_Uart_eq_rx_buff[param_len - 1]);
	selink_set((int*) extCMD_Uart_eq_rx_buff, param_len / 4);
}

#endif //DSP_EQ_SETTING_ENABLE

const CMD_HANDLE_STRU cmd_table[] = {
	{CONSOLE_CMD_TEST, 0, NULL, handler_for_test, "command test"},
	{CONSOLE_CMD_SYSTEM_SUSPEND, 0, NULL, system_suspend, "this is system comand"},
	{CONSOLE_CMD_CHANGE_TASK_PRIORITY, 0, NULL, task_priority_change, "task priority change test"},
	{CONSOLE_CMD_KEY_SHORT, 0, NULL, sim_key_short_hit, "sim key short hit"},
	{CONSOLE_CMD_KEY_HOLD, 0, NULL, sim_key_hold, "sim key hold"},
	{CONSOLE_CMD_KEY_OPERATE, 0, NULL, key_operate, "key operate start"},
	{CONSOLE_extCMD_TEST, sizeof(extCMD_test_buff), extCMD_test_buff, extCMD_handler_for_test, "extern command test"},
	#ifndef DSPEQ_SETTING_ENABLE
	#ifdef DSP_HIFI_LIB_OLD
	{CONSOLE_extCMD_DSP, sizeof(extCMD_Uart_rx_buff), extCMD_Uart_rx_buff, extCMD_handler_for_DSP_data, "extern dsp setting"},
	#else
	{CONSOLE_extCMD_LR_DSP, sizeof(extCMD_Uart_rx_Lr_buff), extCMD_Uart_rx_Lr_buff, extCMD_handler_for_Lr_DSP_data, "extern LR setting"},
	{CONSOLE_extCMD_CENTRE_DSP, sizeof(extCMD_Uart_rx_Centre_buff), extCMD_Uart_rx_Centre_buff, extCMD_handler_for_Centre_DSP_data,"extern CENTRE setting"},
	{CONSOLE_extCMD_SUB_DSP, sizeof(extCMD_Uart_rx_Sub_buff), extCMD_Uart_rx_Sub_buff, extCMD_handler_for_Sub_DSP_data, "extern SUB setting"},
	#endif
	#endif
	#ifdef DSP_EQ_SETTING_ENABLE
	{CONSOLE_extCMD_EQ_DSP, sizeof(extCMD_Uart_eq_rx_buff), extCMD_Uart_eq_rx_buff, extCMD_handler_for_EQ_DSP_data, "dsp eq setting"}
	#endif
};

static void receive_done_callback(int parm, U32 status)
{
	int i;

	switch(console_cmd_env.cmd_parse_state)
	{
		case WAIT_NEXT_CMD:
			if (console_cmd_env.cmd_header.pre_header == CMD_PRE_HEADER || console_cmd_env.cmd_header.pre_header == extCMD_PRE_HEADER)
			{
				console_cmd_env.cmd_parse_state = RECEIVING_CMD_HEADER;
				console_cmd_env.cmd_header.cmd_id = CONSOLE_CMD_ID_MAX;
				console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.cmd_id, 3, receive_done_callback, 0);

				xTimerStartFromISR(console_cmd_env.timer_handle, 0);
				//start moniter, if you don't receive the header after 2ms, return to wait next cmd state 
			}
			else
			{
				console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.pre_header, 1, receive_done_callback, 0);
			}
			break;

		case RECEIVING_CMD_HEADER:

			console_cmd_env.pcmd_handle = NULL;

			xTimerStopFromISR(console_cmd_env.timer_handle, 0);

			for (i = 0; i < sizeof(cmd_table)/sizeof(cmd_table[0]); i++)
			{
				if (console_cmd_env.cmd_header.cmd_id == cmd_table[i].cmd_id)
				{
					console_cmd_env.pcmd_handle = &cmd_table[i];

					if (console_cmd_env.cmd_header.pre_header == CMD_PRE_HEADER)
					{		
						console_cmd_env.cmd_parse_state = WAIT_NEXT_CMD;
						console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.pre_header, 1, receive_done_callback, 0);

						if (console_cmd_env.pcmd_handle->handler != NULL)
						{
							func_w_one_param_msgSend_formIsr( console_cmd_env.pcmd_handle->handler, (U32)(console_cmd_env.cmd_header.param_h<<8 | console_cmd_env.cmd_header.param_l));
						}
					}
					else if (console_cmd_env.cmd_header.pre_header == extCMD_PRE_HEADER)
					{
						console_cmd_env.param_len = console_cmd_env.cmd_header.param_h<<8 | console_cmd_env.cmd_header.param_l;

						printf_info("-----%d,%d-----\n",console_cmd_env.pcmd_handle->param_buffer_size,console_cmd_env.param_len);
						if ((console_cmd_env.pcmd_handle->param_buffer_size >=  console_cmd_env.param_len) && (console_cmd_env.pcmd_handle->param_buffer != NULL))
						{
							console_cmd_env.cmd_parse_state = RECEIVING_CMD_PARAM;
							console_cmd_env.ext_if->read(console_cmd_env.pcmd_handle->param_buffer, console_cmd_env.param_len, receive_done_callback, 0);
							xTimerStartFromISR(console_cmd_env.timer_handle, 0);
						}
						else
						{
							//parameter is too long to cmd handler to receive, please check
							console_cmd_env.cmd_parse_state = WAIT_NEXT_CMD;
							console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.pre_header, 1, receive_done_callback, 0);							
						}
					}
					else
					{
						console_cmd_env.cmd_parse_state = WAIT_NEXT_CMD;
						console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.pre_header, 1, receive_done_callback, 0);							
					}
				}
			}
			if (console_cmd_env.pcmd_handle == NULL)
			{
				console_cmd_env.cmd_parse_state = WAIT_NEXT_CMD;
				console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.pre_header, 1, receive_done_callback, 0);
			}
			break;

		case RECEIVING_CMD_PARAM:
		
			xTimerStopFromISR(console_cmd_env.timer_handle, 0);
			if (console_cmd_env.pcmd_handle->handler != NULL)
			{
				func_w_one_param_msgSend_formIsr( console_cmd_env.pcmd_handle->handler, (U32)console_cmd_env.param_len);
			}

			console_cmd_env.cmd_parse_state = WAIT_NEXT_CMD;
			console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.pre_header, 1, receive_done_callback, 0);

			break;

		default:
			break;
	}
}

static void console_cmd_receive_timeout( TimerHandle_t xTimer )
{
	int i;
	
	if (console_cmd_env.cmd_parse_state == RECEIVING_CMD_HEADER)
	{
		if (console_cmd_env.cmd_header.cmd_id != 0xff)
		{
			for (i = 0; i < sizeof(cmd_table)/sizeof(cmd_table[0]); i++)
			{
				if (console_cmd_env.cmd_header.cmd_id == cmd_table[i].cmd_id)
				{
					if (cmd_table[i].help_string)
						printf_info("%s,timeout\n",cmd_table[i].help_string);
				}
			}
		}
	}
	else if (console_cmd_env.cmd_parse_state == RECEIVING_CMD_PARAM)
	{
		if (console_cmd_env.pcmd_handle->help_string)
			printf_info("%s,timeout\n",console_cmd_env.pcmd_handle->help_string);
	}

	//restart for next command
	console_cmd_env.cmd_parse_state = WAIT_NEXT_CMD;
	console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.pre_header, 1, receive_done_callback, 0);				
}

void console_cmd_init(const extif_api_stru* ext_if)
{
	console_cmd_env.cmd_parse_state = WAIT_NEXT_CMD;
	console_cmd_env.ext_if = ext_if;

    /* create console timer to moniter extern transimition */
	console_cmd_env.timer_handle = xTimerCreate("system Timer", pdMS_TO_TICKS(200), pdFALSE, ( void * ) 0, console_cmd_receive_timeout);

	DBG_Assert(console_cmd_env.timer_handle != NULL);
}

void console_cmd_start(void)
{
	printf_info("Console Command Start\n\r");

	console_cmd_env.cmd_parse_state = WAIT_NEXT_CMD;
	console_cmd_env.console_cmd_monitor = 0;
	console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.pre_header, 1, receive_done_callback, 0);
}

void console_cmd_stop(void)
{
	//break function
	DBG_Assert(FALSE);
}


//#define CONSOLE_CMD_TEST_ENABLE

#ifdef CONSOLE_CMD_TEST_ENABLE
void console_cmd_test(void)
{
	console_cmd_init(&uart0_api);
	console_cmd_start();

	while(1)
	{
		delayms(1);
		if (console_cmd_env.console_cmd_monitor)
		{
			if (console_cmd_env.console_cmd_monitor-- == 0)
			{
				//restart for next command
				console_cmd_env.cmd_parse_state = WAIT_NEXT_CMD;
				console_cmd_env.ext_if->read(&console_cmd_env.cmd_header.pre_header, 1, receive_done_callback, 0);				
			}
		}	
	}
}
#endif //CONSOLE_CMD_TEST
