/**
 ****************************************************************************************
 *
 * @file sona_console_cmd.c
 *
 * @brief console command (from uart etc.) implement.
 *
 ****************************************************************************************
 */

#include "os_config.h"

#include "hw_timer.h"
#include "hal_uart.h"
#include "fifo.h"

#include "app_task.h"
#include "app_window.h"

#ifdef DSP_SONA_MODULE_ENABLE
#include "sona_aef.h"
#include "sona_dsp.h"
#include "sona_dsp_pc_tools_by_uart.h"
#include "sona_console_task.h"
#endif

/*
 * command parser states
 ****************************************************************************************
 */
typedef enum 
{
	SONA_WAIT_NEXT_CMD = 0,
    SONA_RECEIVING_CMD_HEADER,	
    SONA_RECEIVING_CMD_PARAM,
} enumSONA_CONSOLE_CMD_STATE;

#define SONA_CONSOLE_CMD_TEST                   1
#define SONA_CONSOLE_extCMD_TEST                2
#define SONA_CONSOLE_CMD_SYSTEM_SUSPEND         3
#define SONA_CONSOLE_CMD_CHANGE_TASK_PRIORITY   4
#define SONA_CONSOLE_CMD_KEY_SHORT              5
#define SONA_CONSOLE_CMD_KEY_HOLD               6
#define SONA_CONSOLE_CMD_KEY_OPERATE            7
#define SONA_CONSOLE_extCMD_DSP				    8
#define SONA_CONSOLE_CMD_ID_MAX                 0xFF

#define SONA_CMD_PRE_HEADER                     0X5A
#define SONA_extCMD_PRE_HEADER                  0XFA//extern param

typedef struct {
	//U16 cmd_id;
	U16 param_buffer_size;
	U8 *param_buffer;
	void (*handler)(U16 param);
	char *help_string;
} SONA_CMD_HANDLE_STRU;

typedef struct {
	U8 pre_header;
	//U8 cmd_id;
	U8 param_l; //low
	U8 param_h; //high
} __attribute__((packed)) SONA_CMD_HEADER_STRU;

typedef struct {
    // pointer to External interface api
    const extif_api_stru* ext_if;	
	U8 cmd_parse_state;
	U16 param_len;
	U32 console_cmd_monitor;
	SONA_CMD_HEADER_STRU cmd_header;
	const SONA_CMD_HANDLE_STRU *pcmd_handle;
	TimerHandle_t timer_handle; 
} SONA_CONSOLE_CMD_ENV_STRU;

SONA_CONSOLE_CMD_ENV_STRU sona_console_cmd_env;

void sona_handler_for_test(U16 param)
{
	printf_info("sona hello console command:%d\n",param);
}

void sona_system_suspend(U16 param)
{
	TX_INTERRUPT_SAVE_AREA;

	DBG_Printf("%s\n\r", __func__);

	delayms(10);
	TX_DISABLE;
	while(1);
}


void sona_task_priority_change(U16 param)
{

}

void sona_sim_key_short_hit(U16 param)
{

}

void sona_sim_key_hold(U16 param)
{
	
}

void sona_key_operate(U16 param)
{
	printf_info("sona key cmd received\r\n");
	key_msgSend(CONSOLE_KEY, KEY_DOWN, param);
	key_msgSend(CONSOLE_KEY, KEY_UP, param);
}

U8 sona_extCMD_test_buff[100];
void sona_extCMD_handler_for_test(U16 param_len)
{
	printf_info("sona extern console command: param len:%d , param[0]:%d, last param:%d\n", 
                param_len, sona_extCMD_test_buff[0], sona_extCMD_test_buff[param_len-1]);
}

#if 1//def DSPEQ_SETTING_ENABLE
#define DSP_DATA_BYTE_LEN   (1024*10)
U8 sona_extCMD_Uart_rx_buff[DSP_DATA_BYTE_LEN];// __INTERNAL_RAM_BUF_SONA_TEXT;
void sona_extCMD_handler_for_DSP_data(U16 param_len)
{
	printf_info("sona extern console command: param len:%d , param[0]:%d, last param:%d\n", 
                param_len, sona_extCMD_Uart_rx_buff[0], sona_extCMD_Uart_rx_buff[param_len-1]);
	#ifdef SONA_DSP_PC_TOOLS_BY_UART
    sona_uart_command_function_write(&sona_extCMD_Uart_rx_buff[0], param_len+3);
	#endif
}
#endif

const SONA_CMD_HANDLE_STRU sona_cmd_table[] = {
	// {SONA_CONSOLE_CMD_TEST, 0, NULL, sona_handler_for_test, "sona command test"},
	// {SONA_CONSOLE_CMD_SYSTEM_SUSPEND, 0, NULL, sona_system_suspend, "sona this is system comand"},
	// {SONA_CONSOLE_CMD_CHANGE_TASK_PRIORITY, 0, NULL, sona_task_priority_change, "sona task priority change test"},
	// {SONA_CONSOLE_CMD_KEY_SHORT, 0, NULL, sona_sim_key_short_hit, "sona sim key short hit"},
	// {SONA_CONSOLE_CMD_KEY_HOLD, 0, NULL, sona_sim_key_hold, "sona sim key hold"},
	// {SONA_CONSOLE_CMD_KEY_OPERATE, 0, NULL, sona_key_operate, "sona key operate start"},
	// {SONA_CONSOLE_extCMD_TEST, sizeof(sona_extCMD_test_buff), sona_extCMD_test_buff, sona_extCMD_handler_for_test, "sona extern command test"},
	#if 1//def DSPEQ_SETTING_ENABLE
	{sizeof(sona_extCMD_Uart_rx_buff), sona_extCMD_Uart_rx_buff, sona_extCMD_handler_for_DSP_data, "sona extern dsp setting"}
	#endif
};

static void sona_receive_done_callback(int parm, U32 status)
{
	int i;

	switch(sona_console_cmd_env.cmd_parse_state)
	{
		case SONA_WAIT_NEXT_CMD:
			if (sona_console_cmd_env.cmd_header.pre_header == SONA_CMD_PRE_HEADER || sona_console_cmd_env.cmd_header.pre_header == SONA_extCMD_PRE_HEADER)
			{
				sona_console_cmd_env.cmd_parse_state = SONA_RECEIVING_CMD_HEADER;
				//sona_console_cmd_env.cmd_header.cmd_id = SONA_CONSOLE_CMD_ID_MAX;
				//sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.cmd_id, 3, sona_receive_done_callback, 0);
                sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.param_l, 2, sona_receive_done_callback, 0);

				xTimerStartFromISR(sona_console_cmd_env.timer_handle, 0);
				//start moniter, if you don't receive the header after 2ms, return to wait next cmd state 
			}
			else
			{
				sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.pre_header, 1, sona_receive_done_callback, 0);
			}
			break;

		case SONA_RECEIVING_CMD_HEADER:

			sona_console_cmd_env.pcmd_handle = NULL;

			xTimerStopFromISR(sona_console_cmd_env.timer_handle, 0);

			for (i = 0; i < sizeof(sona_cmd_table)/sizeof(sona_cmd_table[0]); i++)
			{
				//if (sona_console_cmd_env.cmd_header.cmd_id == sona_cmd_table[i].cmd_id)
				{
					sona_console_cmd_env.pcmd_handle = &sona_cmd_table[i];

					if (sona_console_cmd_env.cmd_header.pre_header == SONA_CMD_PRE_HEADER)
					{		
						sona_console_cmd_env.cmd_parse_state = SONA_WAIT_NEXT_CMD;
						sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.pre_header, 1, sona_receive_done_callback, 0);

						if (sona_console_cmd_env.pcmd_handle->handler != NULL)
						{
							func_w_one_param_msgSend_formIsr( sona_console_cmd_env.pcmd_handle->handler, (U32)(sona_console_cmd_env.cmd_header.param_h<<8 | sona_console_cmd_env.cmd_header.param_l));
						}
					}
					else if (sona_console_cmd_env.cmd_header.pre_header == SONA_extCMD_PRE_HEADER)
					{
						sona_console_cmd_env.param_len = (sona_console_cmd_env.cmd_header.param_h<<8 | sona_console_cmd_env.cmd_header.param_l) + 3;

						if ((sona_console_cmd_env.pcmd_handle->param_buffer_size >=  sona_console_cmd_env.param_len) && (sona_console_cmd_env.pcmd_handle->param_buffer != NULL))
						{
							sona_console_cmd_env.cmd_parse_state = SONA_RECEIVING_CMD_PARAM;
                            sona_console_cmd_env.pcmd_handle->param_buffer[0] = SONA_extCMD_PRE_HEADER;
                            sona_console_cmd_env.pcmd_handle->param_buffer[1] = sona_console_cmd_env.cmd_header.param_l;
                            sona_console_cmd_env.pcmd_handle->param_buffer[2] = sona_console_cmd_env.cmd_header.param_h;
							sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.pcmd_handle->param_buffer[3], sona_console_cmd_env.param_len, sona_receive_done_callback, 0);
							xTimerStartFromISR(sona_console_cmd_env.timer_handle, 0);
						}
						else
						{
							//parameter is too long to cmd handler to receive, please check
							sona_console_cmd_env.cmd_parse_state = SONA_WAIT_NEXT_CMD;
							sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.pre_header, 1, sona_receive_done_callback, 0);							
						}
					}
					else
					{
						sona_console_cmd_env.cmd_parse_state = SONA_WAIT_NEXT_CMD;
						sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.pre_header, 1, sona_receive_done_callback, 0);							
					}
				}
			}
			if (sona_console_cmd_env.pcmd_handle == NULL)
			{
				sona_console_cmd_env.cmd_parse_state = SONA_WAIT_NEXT_CMD;
				sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.pre_header, 1, sona_receive_done_callback, 0);
			}
			break;

		case SONA_RECEIVING_CMD_PARAM:
		
			xTimerStopFromISR(sona_console_cmd_env.timer_handle, 0);
			if (sona_console_cmd_env.pcmd_handle->handler != NULL)
			{
				func_w_one_param_msgSend_formIsr( sona_console_cmd_env.pcmd_handle->handler, (U32)sona_console_cmd_env.param_len);
			}

			sona_console_cmd_env.cmd_parse_state = SONA_WAIT_NEXT_CMD;
			sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.pre_header, 1, sona_receive_done_callback, 0);

			break;

		default:
			break;
	}
}

static void sona_console_cmd_receive_timeout( TimerHandle_t xTimer )
{
	int i;
	
	if (sona_console_cmd_env.cmd_parse_state == SONA_RECEIVING_CMD_HEADER)
	{
		//if (sona_console_cmd_env.cmd_header.cmd_id != 0xff)
		{
			for (i = 0; i < sizeof(sona_cmd_table)/sizeof(sona_cmd_table[0]); i++)
			{
				//if (sona_console_cmd_env.cmd_header.cmd_id == sona_cmd_table[i].cmd_id)
				{
					if (sona_cmd_table[i].help_string)
						printf_info("%s\n",sona_cmd_table[i].help_string);
				}
			}
		}
	}
	else if (sona_console_cmd_env.cmd_parse_state == SONA_RECEIVING_CMD_PARAM)
	{
		if (sona_console_cmd_env.pcmd_handle->help_string)
			printf_info("%s\n",sona_console_cmd_env.pcmd_handle->help_string);
	}

	//restart for next command
	sona_console_cmd_env.cmd_parse_state = SONA_WAIT_NEXT_CMD;
	sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.pre_header, 1, sona_receive_done_callback, 0);				
}

void sona_console_cmd_init(const extif_api_stru* ext_if)
{
	sona_console_cmd_env.cmd_parse_state = SONA_WAIT_NEXT_CMD;
	sona_console_cmd_env.ext_if = ext_if;

    /* create console timer to moniter extern transimition */
	sona_console_cmd_env.timer_handle = xTimerCreate("sona system Timer", pdMS_TO_TICKS(50), pdFALSE, ( void * ) 1, sona_console_cmd_receive_timeout);

	DBG_Assert(sona_console_cmd_env.timer_handle != NULL);
}

void sona_console_cmd_start(void)
{
	printf_info("sona Console Command Start\n\r");

	sona_console_cmd_env.cmd_parse_state = SONA_WAIT_NEXT_CMD;
	sona_console_cmd_env.console_cmd_monitor = 0;
	sona_console_cmd_env.ext_if->read(&sona_console_cmd_env.cmd_header.pre_header, 1, sona_receive_done_callback, 0);
}

void sona_console_cmd_stop(void)
{
	//break function
	DBG_Assert(FALSE);
}

#ifdef SONA_DSP_PC_TOOLS_BY_UART_FIFO

#define SONA_UART_FIFO_LEN  (1024*10)

FIFO sona_uartfifo;
U8 sona_uartfifo_buf[SONA_UART_FIFO_LEN];// __INTERNAL_RAM_BUF_SONA_TEXT;

U8 sona_uartfifo_analyse_buf[SONA_UART_FIFO_LEN];// __INTERNAL_RAM_BUF_SONA_TEXT;
static U8 sona_fifo_data_receive_wait = 1; // wait 100ms.
void sona_fifo_init(void)
{
    DBG_Printf("%s\n\r", __func__);
    
    memset(sona_uartfifo_buf, 0, SONA_UART_FIFO_LEN);
    fifo_init(&sona_uartfifo, sona_uartfifo_buf, SONA_UART_FIFO_LEN, TRUE);

    fifo_discard_all_data(&sona_uartfifo);

    app_task_timer_ms_set(TIMER_Sona_UartData_Analyse, 1000);

}

#define SONA_INPUT_DATA_SIZE    1
void sona_fifo_data_receive(U8 temp)
{
    int free_space_len = fifo_get_free_space(&sona_uartfifo);
    int write_space_len = fifo_get_fifo_data_wlen(&sona_uartfifo);
    if (free_space_len < SONA_INPUT_DATA_SIZE)
    {
        DBG_Printf("sona fifo free buf overflow:%d,%d\n\r", free_space_len, write_space_len);
        //fifo_discard_all_data(&sona_uartfifo);
        //return;
    }
    sona_fifo_data_receive_wait = 1;
    //int wlen = fifo_put_data_one_byte(&sona_uartfifo, &temp, SONA_INPUT_DATA_SIZE);
    int wlen = fifo_put_data_one_byte(&sona_uartfifo, &temp);

    if (wlen != SONA_INPUT_DATA_SIZE)
    {
        DBG_Printf("sona put fifo overflow:%d,%d\n\r", SONA_INPUT_DATA_SIZE, wlen);
    }

}

void sona_fifo_data_analyse(void)
{
    DBG_Printf("%s\n\r", __func__);
	if (sona_fifo_data_receive_wait)
	{
        //DBG_Printf("wait uart rx data finish.\n\r");
		sona_fifo_data_receive_wait--;
		return;
	}
	#ifdef DTS_VIRTUAL_X_CFG_SEND_BY_UART
    if (get_sona_dsp_pc_tools_by_uart_flag() >= TRUE)
	#else
    if (get_sona_dsp_pc_tools_by_uart_flag() == TRUE)
	#endif
    {
        int readlen = fifo_get_data_len(&sona_uartfifo);
        int getlen = 0;
        memset(sona_uartfifo_analyse_buf, 0x0, sizeof(sona_uartfifo_analyse_buf));
        if (readlen > 0)
        {
            getlen = fifo_get_data(&sona_uartfifo, sona_uartfifo_analyse_buf, readlen);

            if (getlen != readlen)
            {
                DBG_Printf("sona get fifo underflow:%d,%d\n\r", getlen, readlen);
                return;
            }
            else
            {
                //DBG_Printf("sona fifo len:%d,%d\n\r", getlen, readlen);
            }
        }
        else
        {
            return;
        }

        sona_uart_command_function_write(sona_uartfifo_analyse_buf, getlen);        
    }
}
#endif
