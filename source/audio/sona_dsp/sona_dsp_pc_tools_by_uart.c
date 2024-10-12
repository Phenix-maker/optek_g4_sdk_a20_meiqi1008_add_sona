/*
 * sona_dsp_pc_tools_by_uart.c
 *
 *  Created on: 2024/5/16
 *      Author: Windows
 */

#include "os_config.h"

#include "common.h"

#ifdef SONA_DSP_PC_TOOLS_BY_UART
#include "hal_uart.h"
#include "Audio_Device.h"

#include "sona_aef.h"
#include "sona_dsp.h"

#include "hw_timer.h"

#include "optek_hifi2_dapp.h"


#include "sona_console_task.h"
#include "sona_dsp_pc_tools_by_uart.h"

#include "sona_dsp_codec_malloc.h"


#define SONA_DSP_PC_TOOLS_FUNC_ENABLE
//#define DBG_Printf(...)

#define PRINTF_TEST



char sona_receive_buf[1024];

void sona_uart_command_read_callback(int param, U8 status)
{
    DBG_iPrintf("--sona read command finish-%d-\n",param);
    int i;
    for (i = 0; i < 10; i++)
    {
        DBG_iPrintf("sona_receive_buf[%d]:0x%d\n\r", i, sona_receive_buf[i]);
    }


}

void sona_uart_command_write_callback(int param, U8 status)
{
    //DBG_Printf("--sona write command finish-%d-\n",param);
}

void sona_uart_command_write_callback_uart_1(int param, U8 status)
{
    //DBG_Printf("--sona write command finish-%d-\n",param);
}

void sona_uart_command_write(char *data, int len)
{
    if (get_sona_dsp_pc_tools_by_uart_flag() == TRUE)
    {
        #ifdef SONA_DSP_PC_TOOLS_BY_UART_0
        hal_uart0_write(data, len, sona_uart_command_write_callback, 0);      
        #elif (defined SONA_DSP_PC_TOOLS_BY_UART_1)
        hal_uart1_write(data, len, sona_uart_command_write_callback, 0);
        #elif (defined SONA_DSP_PC_TOOLS_BY_UART_2)
        hal_uart2_write(data, len, sona_uart_command_write_callback, 0);
        #endif
    }
}

void sona_uart_command_function_write(char *data, int len)
{
    //DBG_Printf("%s:%d\n\r", __func__, len);
    #ifdef SONA_DSP_PC_TOOLS_FUNC_ENABLE
    if (get_sona_dsp_pc_tools_by_uart_flag() == TRUE)
    {
        sona_aef_feed_recvData(data, len);
    }
	#ifdef DTS_VIRTUAL_X_CFG_SEND_BY_UART    
    else if (get_sona_dsp_pc_tools_by_uart_flag() > TRUE)
    {
        if((data[0] == 's' && data[1] == '0')) {
        sona_tools_for_writeDtxFile(data,len);
		}
		else {
		printf_warn("exit send dts vx cfg file %s:%d %x\n\r", __func__, len,data[0]);
		set_sona_dsp_pc_tools_by_uart_flag(FALSE);
		}
    }
    #endif
    #endif
}

int sona_aef_tuning_enable;
void set_sona_dsp_pc_tools_by_uart_flag(char con)
{
    #ifdef SONA_DSP_PC_TOOLS_FUNC_ENABLE
    sona_aef_tuning_enable = con;
    #endif
    //DBG_Printf("%s:%d\n\r", __func__, sona_aef_tuning_enable);
}
int get_sona_dsp_pc_tools_by_uart_flag(void)
{
    //DBG_Printf("%s:%d\n\r", __func__, sona_aef_tuning_enable);
    #ifdef SONA_DSP_PC_TOOLS_FUNC_ENABLE
    return sona_aef_tuning_enable;
    #endif
}

void sona_dsp_pc_tools_by_uart_init(void)
{
    DBG_Printf("%s\n\r", __func__);
    #ifdef SONA_DSP_PC_TOOLS_FUNC_ENABLE
    set_sona_dsp_pc_tools_by_uart_flag(FALSE);

    // sona_dsp_pc_tools_by_uart_flag_switch();
    #endif

    #ifdef DTS_VIRTUAL_X_CFG_SEND_BY_UART
        #ifdef SONA_DSP_PC_TOOLS_BY_UART_FIFO
        sona_fifo_init();
        #endif  
        set_sona_dsp_pc_tools_by_uart_flag(TRUE*2);
    #endif
}

void sona_dsp_pc_tools_by_uart_open(void)
{
    DBG_Printf("%s\n\r", __func__);
    #ifdef SONA_DSP_PC_TOOLS_FUNC_ENABLE
    if (DspSonaInit_en == TRUE)
    {
        #ifdef SONA_DSP_PC_TOOLS_BY_UART_0
        #ifndef UART0_USED_FOR_DEBUG
        hal_uart0_open(1, 1, UART0_BAUDRATE);
        #endif
        sona_console_cmd_init(&uart0_api);
        sona_console_cmd_start();        
        #elif (defined SONA_DSP_PC_TOOLS_BY_UART_1)
        #ifndef UART1_USED_FOR_DEBUG
        hal_uart1_open(1, 1, UART1_BAUDRATE);
        #endif
        sona_console_cmd_init(&uart1_api);
        sona_console_cmd_start();
        #elif (defined SONA_DSP_PC_TOOLS_BY_UART_2)
        #ifndef UART2_USED_FOR_DEBUG
        hal_uart2_open(1, 1, UART2_BAUDRATE);
        #endif
        sona_console_cmd_init(&uart2_api);
        sona_console_cmd_start(); 
        #endif

        #ifdef SONA_DSP_PC_TOOLS_BY_UART_FIFO
        sona_fifo_init();
        #endif
        
        tTuningConfig sona_PCTuningConfig;
        sona_PCTuningConfig.type = 0;
        sona_PCTuningConfig.comm_write = sona_uart_command_write;
        sona_aef_tuning_open(&sona_PCTuningConfig);
        set_sona_dsp_pc_tools_by_uart_flag(TRUE);

        #ifdef SONA_DSP_PC_TOOLS_DTS_VIRTUAL_X_SET
        sona_aef_tuning_set_dtxFile_CB(sona_tools_for_writeDtxFile);
        #endif
    }
    #endif
}

void sona_dsp_pc_tools_by_uart_close(void)
{
    DBG_Printf("%s\n\r", __func__);
    #ifdef SONA_DSP_PC_TOOLS_FUNC_ENABLE
    set_sona_dsp_pc_tools_by_uart_flag(FALSE);

    sona_aef_tuning_close();
    #endif
}


void sona_dsp_pc_tools_by_uart_flag_switch(void)
{
    DBG_Printf("%s:%d\n\r", __func__, sona_aef_tuning_enable);
    #ifdef SONA_DSP_PC_TOOLS_FUNC_ENABLE
    if (get_sona_dsp_pc_tools_by_uart_flag() == TRUE)
    {
        sona_dsp_pc_tools_by_uart_close();

        #ifdef SONA_DSP_PC_TOOLS_BY_UART_0
        #ifndef UART0_USED_FOR_DEBUG
        hal_uart0_open(1, 1, UART0_BAUDRATE);
        #endif
	    printf_init(&uart0_api);        
        #elif (defined SONA_DSP_PC_TOOLS_BY_UART_1)
        #ifndef UART1_USED_FOR_DEBUG
        hal_uart1_open(1, 1, UART1_BAUDRATE);
        #endif
	    printf_init(&uart1_api);
        #elif (defined SONA_DSP_PC_TOOLS_BY_UART_2)
        #ifndef UART2_USED_FOR_DEBUG
        hal_uart2_open(1, 1, UART2_BAUDRATE);
        #endif
	    printf_init(&uart2_api);
        #endif  

    }
    else
    {
        sona_dsp_pc_tools_by_uart_open();     

        // hal_uart2_open(1, 1, UART2_BAUDRATE);
	    // printf_init(&uart2_api);

        hal_uart_fifo_open(1, 1, UART_FIFO_BAUDRATE);
	    printf_init(&uart_fifo_api);

        // #ifndef SONA_DSP_PC_TOOLS_DTS_VIRTUAL_X_BUFF
        // Dts_Virtual_x_setting_buf = sona_dsp_codec_malloc_w_memtype(CODEC_MEMTYPE_SLOW, DTS_VIRTUAL_X_SET_BUF_SIZE, CODEC_MEM_ALIGN_8);
        // #endif

    }
    #endif
}

#ifdef SONA_DSP_PC_TOOLS_DTS_VIRTUAL_X_SET
#ifdef SONA_DSP_PC_TOOLS_DTS_VIRTUAL_X_BUFF
char Dts_Virtual_x_setting_buf[DTS_VIRTUAL_X_SET_BUF_SIZE] __INTERNAL_RAM_BUF_SONA_TEXT;
#else
char *Dts_Virtual_x_setting_buf;
#endif
int Dts_Virtual_x_setting_len;
int Dts_Virtual_x_setting_flag;
void *virtualX_Instance_test;
void Dts_Virtual_x_setting_flag_set(U8 con)
{
    Dts_Virtual_x_setting_flag = con;
    DBG_Printf("Dts_Virtual_x_setting_flag:%d\n\r", Dts_Virtual_x_setting_flag);
}
int Dts_Virtual_x_setting_flag_get(void)
{
    DBG_Printf("Dts_Virtual_x_setting_flag:%d\n\r", Dts_Virtual_x_setting_flag);
    return Dts_Virtual_x_setting_flag;
}
int Dts_Virtual_x_setting_flag_get_noprint(void)
{
    static int flag_bck;
    if (Dts_Virtual_x_setting_flag != flag_bck)
    {
        flag_bck = Dts_Virtual_x_setting_flag;
        DBG_Printf("Dts_Virtual_x_setting_flag:%d\n\r", Dts_Virtual_x_setting_flag);
    }
    return Dts_Virtual_x_setting_flag;
}
void sona_tools_Dts_Virtual_x_setting(void)
{
    if (Dts_Virtual_x_setting_flag_get())
    {
        Dts_Virtual_x_setting_flag_set(FALSE);
    }

    #ifdef DTS_VIRTUAL_X_ENABLE

    if (virtualX_Instance_test == NULL)
    {
        Dts_Virtual_x_setting_flag_set(TRUE);
        //virtualX_Instance = virtualX_Instance_test;
        DBG_Printf("%s:0x%x\n\r", __func__, virtualX_Instance_test);
    }
    else
    {
		Dts_Virtual_x_setting_flag_set(FALSE);
    }
    #else
    Dts_Virtual_x_setting_flag_set(FALSE);
    #endif

    
}
int sona_tools_for_writeDtxFile(char*data, int size)
{
    DBG_Printf("%s:%d,0x%x\n\r", __func__, size, Dts_Virtual_x_setting_buf);
    #ifdef SONA_DSP_PC_TOOLS_FUNC_ENABLE
    Dts_Virtual_x_setting_len = size;
    #ifndef SONA_DSP_PC_TOOLS_DTS_VIRTUAL_X_BUFF
    memset(Dts_Virtual_x_setting_buf, 0x00, DTS_VIRTUAL_X_SET_BUF_SIZE);
    #else
    memset(Dts_Virtual_x_setting_buf, 0x00, sizeof(Dts_Virtual_x_setting_buf));
    #endif
    //DBG_Printf("%s\n\r", data);
    memcpy(Dts_Virtual_x_setting_buf, data, size);
    DBG_Printf("%s\n\r", Dts_Virtual_x_setting_buf);
    if (Dts_Virtual_x_setting_len > 0)
    {
        // virtualX_Instance_bck = virtualX_Instance;
        // virtualX_Instance = NULL;
        virtualX_Instance_test = NULL;
        sona_tools_Dts_Virtual_x_setting();
    }
    #endif
    return 1;
}

#endif
#endif
