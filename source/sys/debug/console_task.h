#ifndef _CONSOLE_TASK_
#define _CONSOLE_TASK_

void console_cmd_init(const extif_api_stru* ext_if);
void console_cmd_start(void);

#ifdef DSP_EQ_SETTING_ENABLE
extern U8 extCMD_Uart_eq_rx_buff[1024 * 4];
#endif

#endif//_CONSOLE_TASK_