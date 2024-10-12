#ifndef __SONA_CONSOLE_TASK__
#define __SONA_CONSOLE_TASK__

void sona_console_cmd_init(const extif_api_stru* ext_if);
void sona_console_cmd_start(void);

#ifdef SONA_DSP_PC_TOOLS_BY_UART_FIFO
void sona_fifo_init(void);
void sona_fifo_data_receive(U8 temp);
void sona_fifo_data_analyse(void);
#endif

#endif//__SONA_CONSOLE_TASK__
