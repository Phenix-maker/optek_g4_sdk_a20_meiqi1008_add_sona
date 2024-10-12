/*
 * sona_dsp_pc_tools_by_uart.h
 *
 *  Created on: 2024/5/16
 *      Author: Windows
 */

#ifndef __SONA_DSP_PC_TOOLS_BY_UART_H__
#define __SONA_DSP_PC_TOOLS_BY_UART_H__

void sona_uart_command_write(char *data, int len);
void sona_uart_command_function_write(char *data, int len);

void sona_dsp_pc_tools_by_uart_init(void);
void sona_dsp_pc_tools_by_uart_open(void);
void sona_dsp_pc_tools_by_uart_close(void);

extern int sona_aef_tuning_enable;
void sona_dsp_pc_tools_by_uart_flag_switch(void);
void set_sona_dsp_pc_tools_by_uart_flag(char con);
int get_sona_dsp_pc_tools_by_uart_flag(void);

#ifdef SONA_DSP_PC_TOOLS_DTS_VIRTUAL_X_SET
#define DTS_VIRTUAL_X_SET_BUF_SIZE  40*1024
#ifdef SONA_DSP_PC_TOOLS_DTS_VIRTUAL_X_BUFF
extern char Dts_Virtual_x_setting_buf[DTS_VIRTUAL_X_SET_BUF_SIZE];
#else
extern char *Dts_Virtual_x_setting_buf;
#endif
extern int Dts_Virtual_x_setting_len;
void Dts_Virtual_x_setting_flag_set(U8 con);
int Dts_Virtual_x_setting_flag_get(void);
int Dts_Virtual_x_setting_flag_get_noprint(void);
int sona_tools_for_writeDtxFile(char*data, int size);
#endif

#endif /* __SONA_DSP_PC_TOOLS_BY_UART_H__ */
