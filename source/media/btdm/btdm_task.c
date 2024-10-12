#include "os_config.h"

#include "sdk_com.h"

#include "btdm_task.h"
#include "interrupt.h"
#include "hw_timer.h"
#include "hal_uart.h"
#include "hw_timer.h"
#include "fifo.h"
#include "ht_hci_main.h"

#ifdef BTDM52_ENABLE

// #define BTDM52_DEBUG

#ifdef BTDM52_DEBUG
#define BT_WR_RG(addr, value)        *((volatile U32 *)(addr | 0x17000400)) = (value)
#define BT_RD_RG(addr)               *((volatile U32 *)(addr | 0x17000400))

#define BLE_WR_RG(addr, value)        *((volatile U32 *)(addr | 0x17000800)) = (value)
#define BLE_RD_RG(addr)               *((volatile U32 *)(addr | 0x17000800))

#define DM_WR_RG(addr, value)        *((volatile U32 *)(addr | 0x17000000)) = (value)
#define DM_RD_RG(addr)               *((volatile U32 *)(addr | 0x17000000))

#define OPL_WR_RG(addr, value)        *((volatile U32 *)(addr | 0x17000B00)) = (value)
#define OPL_RD_RG(addr)               *((volatile U32 *)(addr | 0x17000B00))

#endif

/*
 ****************************************************************************************
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */
#define TASK_BTDM_STACK_SIZE    (1024*3)
#define HCI_RX_C_FIFO_SIZE    (1024+512)
#define HCI_RX_H_FIFO_SIZE    (1024+512*2)
/*
 ****************************************************************************************
 * STRUCT DEFINITION
 ****************************************************************************************
 */

typedef struct {
    U64 tick;
    U8 *p_read_buf;
    U32 read_size_req;
    U8 fifo_C_buf[HCI_RX_C_FIFO_SIZE];

    U8  fifo_H_buf[HCI_RX_H_FIFO_SIZE];
    FIFO *phci_H_rx_fifo;   

    FIFO hci_C_rx_fifo;
    TaskHandle_t pxCreatedTask;
} BTDM_TASK_ENV_STRU;

BTDM_TASK_ENV_STRU btdm_task_env;

void uart_read_end(void);
void uart_write_end(void);

void rwip_schedule_callback(void) __attribute__ ((section (".internal_ram_1_text")));
void rwip_schedule_callback(void)
{
	SOFT8_INT_SET;
}

void btdm_controller_sch(void) __attribute__ ((section (".internal_ram_1_text")));
void btdm_controller_sch(void)
{
	int ret;

    //printf_info("sc\n");
    ret = xEventGroupSetBitsFromISR(global_vari.eventGroup, BT_CONTROLLER_EVENT, NULL);
    if( ret == pdTRUE )
    {
        portYIELD_FROM_ISR();
    }
}

void rf_reg_init(void)
{
	//rf_switch(1);
}

void bt_assert (BOOL condition)
{
    debug_assert(condition);
}  

void hci_C_rx_event(void);
//call by host task
void hci_C_rx_data_put(char *buf, int size)
{
    fifo_put_data(&btdm_task_env.hci_C_rx_fifo, buf, size);

    xEventGroupSetBits(global_vari.eventGroup, BT_CONTROLLER_RX_EVENT);
}

//call by btdm task
void hci_C_rx_event(void)
{
    if (btdm_task_env.read_size_req)
    {
        if (fifo_get_data_len(&btdm_task_env.hci_C_rx_fifo) >= btdm_task_env.read_size_req)
        {
            fifo_get_data(&btdm_task_env.hci_C_rx_fifo, btdm_task_env.p_read_buf, btdm_task_env.read_size_req);
            btdm_task_env.p_read_buf = NULL;
            btdm_task_env.read_size_req = 0;
            uart_read_end();
        }
    }    
}

void hci_C_rx_data_put(char *buf, int size);
//call by bt host hci
void uart0_tx_send(char *buf, int size)
{
	hci_C_rx_data_put(buf, size);
}

//call by bt host
void hci_uart_fifo_init (FIFO *pFifo)
{
    btdm_task_env.phci_H_rx_fifo = pFifo;
	fifo_init (pFifo, btdm_task_env.fifo_H_buf, HCI_RX_H_FIFO_SIZE, TRUE);
}

//call by btdm task
void hci_H_rx_data_put(char *buf, int size)
{
    fifo_put_data(btdm_task_env.phci_H_rx_fifo, buf, size);

    xEventGroupSetBits(global_vari.eventGroup, UART0_RX_EVENT);
}

void rwip_schedule_enter(void) __attribute__ ((section (".internal_ram_1_text")));
void rwip_schedule_enter(void)
{
	unsigned long uxBits;

    btdm_host_init();

	while(1)                        
	{	
#ifdef BTDM_USE_ONETASK        
		uxBits  = xEventGroupWaitBits(global_vari.eventGroup, BT_CONTROLLER_EVENT|BT_CONTROLLER_RX_EVENT|BT_HOST_INTERNAL_MSG|UART0_RX_EVENT|BT_A2DP_TX_EVENT, pdTRUE, pdFALSE, pdMS_TO_TICKS(100));

        if ( (uxBits & BT_CONTROLLER_RX_EVENT) == BT_CONTROLLER_RX_EVENT) 
        {
            hci_C_rx_event();
        }
        
		if ( (uxBits & BT_CONTROLLER_EVENT) == BT_CONTROLLER_EVENT)
		{
			rwip_schedule();	
		}
        
        if ((uxBits & UART0_RX_EVENT) == UART0_RX_EVENT)
        {
            btdm_host_schedule(HCI_RX_EVENT);
        }

        if ((uxBits & BT_HOST_INTERNAL_MSG) == BT_HOST_INTERNAL_MSG)
        {
            btdm_host_schedule(HOST_MSG_EVENT);
        }

        if ((uxBits & BT_A2DP_TX_EVENT) == BT_A2DP_TX_EVENT)
        {
            btdm_host_schedule(A2DP_TX_EVENT);
        }

        if ((uxBits & (BT_HOST_INTERNAL_MSG | BT_CONTROLLER_EVENT | BT_CONTROLLER_RX_EVENT | UART0_RX_EVENT | BT_A2DP_TX_EVENT)) == 0 )
        {
            btdm_host_schedule(_100MS_TICK_EVENT);
        }
#else
		uxBits  = xEventGroupWaitBits(global_vari.eventGroup, BT_CONTROLLER_EVENT|BT_CONTROLLER_RX_EVENT, portMAX_DELAY);
		
        if ( (uxBits & BT_CONTROLLER_RX_EVENT) == BT_CONTROLLER_RX_EVENT) 
        {
            hci_C_rx_event();
        }

		if ( (uxBits & BT_CONTROLLER_EVENT) == BT_CONTROLLER_EVENT)
		{
			rwip_schedule();	
		}
#endif                
	}    
}

void uart_read_set(U8 *bufptr, U32 size) __attribute__ ((section (".internal_ram_1_text")));
void uart_read_set(U8 *bufptr, U32 size)
{
    if (fifo_get_data_len(&btdm_task_env.hci_C_rx_fifo) >= size)
    {
        fifo_get_data(&btdm_task_env.hci_C_rx_fifo, bufptr, size);
        uart_read_end();
    }
    else
    {
        btdm_task_env.p_read_buf = bufptr;
        btdm_task_env.read_size_req = size;
    }
}

void hci_H_rx_data_put(char *buf, int size);
void uart_write_set(U8 *bufptr, U32 size) __attribute__ ((section (".internal_ram_1_text")));
void uart_write_set(U8 *bufptr, U32 size)
{
    hci_H_rx_data_put(bufptr, size);
    uart_write_end();
}

//function declarations
void rf_ant_sel(U8 tx_sel, U8 rx_sel);

struct pwr_cntrl_stru{
  uint8_t pwr_min;
  uint8_t pwr_max;
  uint8_t pwr_step;
  uint8_t pwr_msk;
};

void btdm_debug_pin(U8 pin, U8 con);

/**
 ****************************************************************************************
 * @brief power control setting, note that call after rf initialation
 *
 * @param[in] pa_sel         PA selection:1->pa1 max 13dbm 0->pa0 max 4dbm  default:0
 * @param[in] cntl           power control settings default:{0xf, 0x10, 0x7f, 0x7f}
 *
 * @return  NULL
 ****************************************************************************************
 */
void pwr_cntrl_set(uint8_t pa_sel ,struct pwr_cntrl_stru *cntrl);

/**
 *************************************************************************************
 * @brief btdm stack run
 *
 * @param[in]  param      Preserve API
 *
 *************************************************************************************
 */
static void btdm_task (void *param)
{
    //initialization
    U32 val;

	MISC_CLKGATE_CONTROLs *pGate = &val;

	val = REG_MISC_CLKGATE;
	pGate->AHB_BTDM_IF_gate = 1;
	pGate->AHB_MDM_IF_pclk_gate = 1;
	REG_MISC_CLKGATE = val;

    //DBG_Printf("btdm52 controller ver:%s,build time:%s %s\r\n",btdm52_controller_version(),btdm52_controller_building_time(),btdm52_controller_building_date());


    btdm_task_env.p_read_buf = NULL;
    btdm_task_env.read_size_req = 0;
    fifo_init(&btdm_task_env.hci_C_rx_fifo, btdm_task_env.fifo_C_buf, HCI_RX_C_FIFO_SIZE, TRUE);
    //hal_uart0_open(1, 1, UART0_BAUDRATE);

    void uart_read_set(U8 *bufptr, U32 size);
    void uart_write_set(U8 *bufptr, U32 size);

    hw_uart_function_set(uart_read_set,uart_write_set);

    extern 	U8  NVDS_buffer[0x0100];

    memset(NVDS_buffer,0xff,sizeof(NVDS_buffer));
    //write_callback_function_set(NVDS_write_callback);

    /* BT ADDR TEST */
    NVDS_buffer[0] = 'N';
    NVDS_buffer[1] = 'V';
    NVDS_buffer[2] = 'D';
    NVDS_buffer[3] = 'S';
    NVDS_buffer[4] = 0x01;
    NVDS_buffer[5] = 0x06;
    NVDS_buffer[6] = 0x06;
    NVDS_buffer[7] = 0x09;
    NVDS_buffer[8] = 0x01;
    NVDS_buffer[9] = 0x01;
    NVDS_buffer[10] = 0x3F;
    NVDS_buffer[11] = 0x9F;
    NVDS_buffer[12] = 0x94;

    delayus(10);
    rw_lib_init();
    delayus(150);
    hw_optek_rf_test();

#if 0//pwr cntl
    struct pwr_cntrl_stru pwr_cntl_str;
    pwr_cntl_str.pwr_min = 0xf;
    pwr_cntl_str.pwr_max = 0x70;
    pwr_cntl_str.pwr_step = 0x8;
    pwr_cntl_str.pwr_msk = 0x7f;

    //choose hpa/lpa
    pwr_cntrl_set(1 ,&pwr_cntl_str);
    //pwr_cntrl_set(0 ,&pwr_cntl_str);
#endif

/*
    tx power
    0x10->0,2db
    0x13->1.6db
*/
    struct pwr_cntrl_stru pwr_cntl_str;
    pwr_cntl_str.pwr_min = 0xf;
    //pwr_cntl_str.pwr_max = (0x80 | 0x13);    //0x08->PA_1, 0x30->tx power, max is 0x7F 
    pwr_cntl_str.pwr_max = (0x80 | 20);    //0x08->PA_1, 0x30->tx power, max is 0x7F 
    pwr_cntl_str.pwr_step = 0x4;//0x8;
    pwr_cntl_str.pwr_msk = 0x7f;//all bit

    //select hpa/lpa
    pwr_cntrl_set(1 ,&pwr_cntl_str);          //hpa
    //pwr_cntrl_set(0 ,&pwr_cntl_str);            //lpa

    //ant0 rf power is about 6dBm bigger than ant1
    //rf_ant_sel(1, 1);               //only for test
    rf_ant_sel(0, 0);                 //must be 0

	val = REG_MISC_CLKGATE;
	pGate = (MISC_CLKGATE_CONTROLs *) &val;

	val = REG_MISC_CLKGATE;

	pGate->AHB_BTDM_IF_gate = 0;
	pGate->AHB_MDM_IF_pclk_gate = 0;

	REG_MISC_CLKGATE = val;	
    delayus(10);

#ifdef BTDM52_DEBUG
    MISC_BTDM_CONTROLs *btdm_config;
    btdm_config = (MISC_BTDM_CONTROLs *) &val;

    val = REG_MISC_BTDM;
    btdm_config->bt_dbg_en = 1;

    #if 1 //ble dbg
    btdm_config->bt_dbg_sel_1 = 0;
    btdm_config->bt_dbg_sel_2 = 0;
    #elif 1 //rf dbg
    /*
        * pin[24:26] -> rf_ga_ip
        * pin[27:29] -> pga_ga_ip
        * pin[30]  ->  ovf_pll
        * pin[31]  ->  unf_pll
        */
    btdm_config->bt_dbg_sel_1 = 2;
    btdm_config->bt_dbg_sel_2 = 2;
    #elif 1 //sys dbg
    btdm_config->bt_dbg_sel_1 = 3;
    btdm_config->bt_dbg_sel_2 = 3;
    #else //bt dbg
    btdm_config->bt_dbg_sel_1 = 4;
    btdm_config->bt_dbg_sel_2 = 4;
    #endif

    REG_MISC_BTDM = val;

    //set gpio functions to output pulse
    GPIO_PINs *p_gpip_pin = (GPIO_PINs *) &val;
    val = REG_GPIO0_FUNC;
    p_gpip_pin->bit_24 = 1; //apwm 1p
    p_gpip_pin->bit_25 = 1; //apwm 1n
    // p_gpip_pin->bit_26 = 1; //apwm 2p
    // p_gpip_pin->bit_27 = 1; //apwm 2n
    // p_gpip_pin->bit_28 = 1; //apwm 3p
    //p_gpip_pin->bit_29 = 1; //apwm 3n
    //p_gpip_pin->bit_30 = 1; //apwm 4p
    //p_gpip_pin->bit_31 = 1; //apwm 4n
    REG_GPIO0_FUNC = val;

    val = REG_GPIO0_DOE;
    p_gpip_pin->bit_26 = 1; //apwm 2p
    p_gpip_pin->bit_27 = 1; //apwm 2n
    p_gpip_pin->bit_28 = 1; //apwm 3p
    p_gpip_pin->bit_29 = 1; //apwm 3n
    p_gpip_pin->bit_30 = 1; //apwm 4p
    p_gpip_pin->bit_31 = 1; //apwm 4n
    REG_GPIO0_DOE = val;
#endif

#if 1//def SET_DATA_IN_PARALLEL_WAY
	MISC_BTDM_2_CONTROLs *btdm_2_config;
	val = REG_MISC_BTDM_2;
	btdm_2_config = (MISC_BTDM_2_CONTROLs *) &val;
	
	//btdm_2_config->bt_rx_if = 1;
	//btdm_2_config->bt_tx_if = 1;
	btdm_2_config->bt_rx_if = 0;
	btdm_2_config->bt_tx_if = 0;

    //btdm_2_config->bt_clk_src = 0;
    btdm_2_config->bt_clksel = 12;

	REG_MISC_BTDM_2 = val;
#else//serial
	MISC_BTDM_2_CONTROLs *btdm_2_config;
	val = REG_MISC_BTDM_2;
	btdm_2_config = (MISC_BTDM_2_CONTROLs *) &val;
	
	btdm_2_config->bt_rx_if = 0;
	btdm_2_config->bt_tx_if = 0;
	REG_MISC_BTDM_2 = val;
#endif

    delayus(10);
	val = REG_MISC_CLKGATE;

	pGate->AHB_BTDM_IF_gate = 1;
	pGate->AHB_MDM_IF_pclk_gate = 1;

	REG_MISC_CLKGATE = val;
    delayus(10);

#ifdef BTDM52_DEBUG
    BT_WR_RG(0x00000050, 0x00000080 + 0x26);//rx tx enable
#endif    
	xt_ints_on(LEVEL2_INT19_MASK);
	SOFT8_INT_ENABLE;
    XT_INTS_ON(LEVEL1_INT1_MASK);
    //task loop
    rwip_schedule_enter();
}

void btdm_debug_pin(U8 pin, U8 con) __attribute__ ((section (".internal_ram_1_text")));
void btdm_debug_pin(U8 pin, U8 con)
{
    #ifdef BTDM52_DEBUG
    if (con)
        REG_GPIO0_DOUT |= (1<<pin);
    else
        REG_GPIO0_DOUT &= ~(1<<pin);
    #endif
}

void btdm_debug_pin_high(void)
{
    btdm_debug_pin(31, 1);
}

void btdm_debug_pin_low(void)
{
    btdm_debug_pin(31, 0);
}

/**
 *************************************************************************************
 * @brief Create btdm task
 *************************************************************************************
 */
void btdm_task_init(void)
{
	if (xTaskCreate(btdm_task, "btdm task", TASK_BTDM_STACK_SIZE, 0, TASK_BTDM_PRIORITY, &btdm_task_env.pxCreatedTask) != TRUE)
    {
		DBG_Assert(FALSE);
    }
    else
    {

    }
}

void suspend_btdm_task(void)
{
    vTaskSuspend(btdm_task_env.pxCreatedTask);
}

void resume_btdm_task(void)
{
    vTaskResume(btdm_task_env.pxCreatedTask);
}

#endif
