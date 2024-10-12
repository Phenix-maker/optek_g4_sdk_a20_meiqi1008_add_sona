#include "os_config.h"


#ifdef REMOTE_ENABLE

#include "regmap.h"

#include "mem_reloc.h"

#include "interrupt.h"
#include "hw_gpio.h"
#include "hw_timer.h"
//#include "hw_ir.h"
#include "hal_gpio.h"

#include "remote.h"
#include "app_key.h"
#include "app_task.h"


#ifdef REMOTE_XIAOMI_FORMAT

//#define REMOTER_USED_GPIO_INTERRUPT

//for waking up by remoter in ECO mode.
//#define STANDBY_ECO_PROJECT

#ifdef REMOTER_USED_GPIO_INTERRUPT
#undef SYS_CLK_VOLATILE
#else
//#define SYS_CLK_VOLATILE
#undef SYS_CLK_VOLATILE
#endif


#ifdef REMOTER_USED_GPIO_INTERRUPT
void remoteHardISR(void) __INTERNAL_RAM_TEXT;
#else
void remoteHardISR(int pin_level, int time) __INTERNAL_RAM_TEXT;
#endif

//static U32 read_ccount2(void) __INTERNAL_RAM_TEXT;
static void SoftInt1Isr(void)   __INTERNAL_RAM_TEXT;


// State of the remote key decoder
#define RC_IDLE          0
#define RC_LEADER_ON     1
#define RC_LEADER_OFF    2
#define RC_DATA          3
#define RC_DATA1         4
#define RC_DATA2         5
#define RC_DATA_END      6
#define RC_FRAME_END     7
#define RC_CUSTOM        8

//#define REMOTE_CONTROL_DEBUG
//#define REMOTE_KEY_VALUE_DEBUG

//#define REMOTE_DEBUG


#if defined(CLK_BASE_FREQ_1X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*1)
#elif defined(CLK_BASE_FREQ_2X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*2)
#elif defined(CLK_BASE_FREQ_3X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*3)
#elif defined(CLK_BASE_FREQ_4X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*4)
#elif defined(CLK_BASE_FREQ_5X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*5)
#elif defined(CLK_BASE_FREQ_6X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*6)
#elif defined(CLK_BASE_FREQ_7X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*7)
#elif defined(CLK_BASE_FREQ_8X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*8)
#elif defined(CLK_BASE_FREQ_9X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*9)
#elif defined(CLK_BASE_FREQ_10X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*10)
#elif defined(CLK_BASE_FREQ_12X)
	#define SYSTEM_CLKS             (CLK_BASE_FREQ*12)
#else
  //#error "The system clk must be defined"
#endif


volatile U32 remote_key;
volatile U32 remote_key_bak;
volatile U8 remote_key_state;
volatile U8 remote_repeat_valid;
volatile U8 remote_state;
volatile U8 remote_key_cnt;

#ifdef STANDBY_ECO_PROJECT
volatile U8 remote_eco_status;
#endif


#define CUSTOM_CODE   0x00FF
//#define CUSTOM_CODE   0x81CE
//#define CUSTOM1_CODE   0x01CE


#define REPEAT_TIMERS    1

#define Remote_Pin_HLevel 128
#define Remote_Pin_LLevel 0

//Counter incr = RC_TB_DEN/RC_TB_NUM
#ifdef REMOTER_USED_GPIO_INTERRUPT
#if 1//def SYS_CLK_VOLATILE
#define RC_TB_DEN           SystemClock
#else
#define RC_TB_DEN           SYSTEM_CLKS
#endif
#else
//32kHz
#define RC_TB_DEN           (32*1000)
#endif

#define RC_TB_NUM           (1000*1000)

#ifdef REMOTER_USED_GPIO_INTERRUPT
#define RC_TB_VAL           (RC_TB_DEN/RC_TB_NUM)
#else
#define RC_TB_VAL           (RC_TB_DEN)/(RC_TB_NUM)
#endif

#define REMOTE_TIME_US(xus)  ((xus*(RC_TB_DEN/1000)+(RC_TB_NUM/1000)/2)/(RC_TB_NUM/1000))

#define LEADER_ON_PULSE_MIN       20
#define LEADER_ON_PULSE_MAX       35

#define LEADER_OFF_PULSE_MIN      15
#define LEADER_OFF_PULSE_MAX      22

#define LEADER_CUSTOM_PULSE_MIN   15
#define LEADER_CUSTOM_PULSE_MAX   22   

#define LEADER_1T_PULSE_MIN       15
#define LEADER_1T_PULSE_MAX       22  

#define LEADER_2T_PULSE_MIN       25
#define LEADER_2T_PULSE_MAX       32

#define LEADER_3T_PULSE_MIN       35
#define LEADER_3T_PULSE_MAX       40

#define LEADER_4T_PULSE_MIN       45
#define LEADER_4T_PULSE_MAX       50

#define FRAME_DATA_END_PULSE_MIN  15
#define FRAME_DATA_END_PULSE_MAX  22

#define FRAME_END_PULSE_MIN       ((10312)*RC_TB_VAL)//330
#define FRAME_END_PULSE_MAX       ((10468)*RC_TB_VAL)//335



// #define LEADER_ON_PULSE_MIN       ((843)*RC_TB_VAL)//26
// #define LEADER_ON_PULSE_MAX       ((1000)*RC_TB_VAL)//32

// #define LEADER_OFF_PULSE_MIN      ((437)*RC_TB_VAL)//14
// #define LEADER_OFF_PULSE_MAX      ((625)*RC_TB_VAL)//20

// #define LEADER_CUSTOM_PULSE_MIN   ((437)*RC_TB_VAL)//14
// #define LEADER_CUSTOM_PULSE_MAX   ((625)*RC_TB_VAL)//20   

// #define LEADER_1T_PULSE_MIN       ((437)*RC_TB_VAL)//14
// #define LEADER_1T_PULSE_MAX       ((625)*RC_TB_VAL)//20  

// #define LEADER_2T_PULSE_MIN       ((718)*RC_TB_VAL)//23
// #define LEADER_2T_PULSE_MAX       ((875)*RC_TB_VAL)//28

// #define LEADER_3T_PULSE_MIN       ((1000)*RC_TB_VAL)//32
// #define LEADER_3T_PULSE_MAX       ((1125)*RC_TB_VAL)//36

// #define LEADER_4T_PULSE_MIN       ((1250)*RC_TB_VAL)//40
// #define LEADER_4T_PULSE_MAX       ((1437)*RC_TB_VAL)//45

// #define FRAME_DATA_END_PULSE_MIN  ((437)*RC_TB_VAL)//14
// #define FRAME_DATA_END_PULSE_MAX  ((625)*RC_TB_VAL)//20

// #define FRAME_END_PULSE_MIN       ((10312)*RC_TB_VAL)//330
// #define FRAME_END_PULSE_MAX       ((10468)*RC_TB_VAL)//335


#define INT_ON_RISING_EDGE        REMOTE_POSITIVE_INT_ENABLE
#define INT_ON_FALLING_EDGE       REMOTE_NEGATIVE_INT_ENABLE



#ifdef SYS_CLK_VOLATILE

static U32 leader_on_pulse_width_min;
static U32 leader_on_pulse_width_max;
static U32 leader_off_pulse_width_min;
static U32 leader_off_pulse_width_max;
static U32 leader_1t_pulse_width_min;
static U32 leader_1t_pulse_width_max;
static U32 leader_2t_pulse_width_min;
static U32 leader_2t_pulse_width_max;

void Remote_Timer_Init(void)
{
	#ifdef STANDBY_ECO_PROJECT
	if (remote_eco_status)
	{
		leader_on_pulse_width_min = LEADER_ON_PULSE1_MIN;
		leader_on_pulse_width_max = LEADER_ON_PULSE1_MAX;
	}
	else
	#endif
	{
		leader_on_pulse_width_min = LEADER_ON_PULSE_MIN;
		leader_on_pulse_width_max = LEADER_ON_PULSE_MAX;
	}

	leader_off_pulse_width_min = LEADER_OFF_PULSE_MIN;
	leader_off_pulse_width_max = LEADER_OFF_PULSE_MAX;

	leader_1t_pulse_width_min = LEADER_1T_PULSE_MIN;
	leader_1t_pulse_width_max = LEADER_1T_PULSE_MAX;

	leader_2t_pulse_width_min = LEADER_2T_PULSE_MIN;
	leader_2t_pulse_width_max = LEADER_2T_PULSE_MAX;
}
#else
void Remote_Timer_Init(void)
{
}
#endif

void Remote_Init(void)
{
	printf_info ("%s\r\n", __func__);

	//U8 state = TRUE;
	
	remote_key = 0xFFFFFFFF;
	//pre_remote_key = 0xFFFFFFFF;
	remote_key_state = KEY_UP;
	remote_repeat_valid = FALSE;
	remote_state = RC_IDLE;
	remote_key_cnt =0;


	#ifdef STANDBY_ECO_PROJECT
	remote_eco_status = FALSE;
	#endif

	#ifdef SYS_CLK_VOLATILE
	Remote_Timer_Init();
	#endif

	#ifdef REMOTER_USED_GPIO_INTERRUPT
	//gpio0_SetFunction(REMOTE_INT_PIN, TRUE);
	//gpio0_SetOutputDisable(REMOTE_INT_PIN);
	//app_gpio_mux_func_select( REMOTE_INT_PIN, MUX_SEL_GPIO_INPUT);
	hal_gpio_mux_func_select( REMOTE_INT_PIN, MUX_SEL_GPIO_INPUT);

	REMOTE_INT_CLEAR;

	//gpio0_SetIntTrigger(REMOTE_INT_PIN, TRIGGER_EDGE);
	//gpio0_SetIntEnable(REMOTE_INT_PIN);
	//gpio0_SetIntNegative(REMOTE_INT_PIN);
	//gpio0_SetIntPositive(REMOTE_INT_PIN);
	//app_gpio_interrupt_set(REMOTE_INT_PIN, TRUE, TRIGGER_EDGE, GPIO_INT_FALLING_EDGE);
	hal_gpio_interrupt_set(REMOTE_INT_PIN, TRUE, TRIGGER_EDGE, GPIO_INT_FALLING_EDGE);

	hw_ir_din_select(IR_IN_CH_1);

	#if defined THREAD_RTOS
	enable_ints( LEVEL1_INT0_MASK );
	#elif defined FREE_RTOS
	xt_ints_on(LEVEL1_INT0_MASK);
	#else
	#error "The RTOS type must be defined"
	#endif

	#else
	
	#if 0
	REG_IR_CTL = 0x13;
	app_gpio_MUXfunction_select(REMOTE_INT_PIN, MUX_SEL_FUNCTION1);
	#else

	U32 val;

	val = REG_MISC_BTDM_2;
	MISC_BTDM_2_CONTROLs *pMiscBtdmCntl2 = (MISC_BTDM_2_CONTROLs *) &val;
	pMiscBtdmCntl2->bt_clk2lp32k = 1;
	REG_MISC_BTDM_2 = val;

	val = REG_MISC_CLKGATE;
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *)&val;
	pGate->IR_controller_gate = 1; //open clk gate
	pGate->AHB_Audio_Controller_gate = 1;
	REG_MISC_CLKGATE = val;
	
	val = REG_IR_CTL;
	IR_CONTROLs* pReg = (IR_CONTROLs *)&val;
	pReg->ir_ena = 1;
	pReg->eco = 1;
	pReg->min_width = 7;
	pReg->int_mask = 1;
	REG_IR_CTL = val;

	val = REG_INTCON_INT_ENA;
	INT_CONTROLs* pIntCNT = (INT_CONTROLs *)&val;
	pIntCNT->irc = 1;
	REG_INTCON_INT_ENA = val;
	#endif

	hal_gpio_mux_func_select(REMOTE_INT_PIN, MUX_SEL_GPIO_INPUT);
	#endif

	XT_INTS_ON(LEVEL1_INT3_MASK);

	#ifndef REMOTE_DEBUG
	#ifdef SOFT1_INT_USED
	SOFT1_INT_ENABLE;
	#endif

	#endif
}

//static U32 pulse_width;

#ifdef REMOTER_USED_GPIO_INTERRUPT
void remoteHardISR(void)
#else
void remoteHardISR(int pin_level, int time)
#endif
{
	//DBG_RMTPrintf("pin level:%d, time :%d\n\r", pin_level, time);

#if 1
	//static U8 remote_state = 0;
	static U8 count;
	//static U16 data1;
	//static U16 data2;
	//static U16 custom;
	static U32 data;

	//static U32 int_count = 0;
	static U32 repeat_count = 0;
	//static U8 remote_repeat_valid = FALSE;
	//static U16 rmt_key;

	volatile U32 tmp;
	volatile U32 pulse_width;
	
	#ifdef REMOTER_USED_GPIO_INTERRUPT
	static U32 preTimerCount = 0;
	#endif

	static U32 pre_pulse_width;
	static U8 pulse_count;
	int temp_min, temp_max;


	#ifdef REMOTER_USED_GPIO_INTERRUPT
	REMOTE_INT_CLEAR;
	//REMOTE_INT_DISABLE;

	tmp = read_ccount();

	if (tmp > preTimerCount)
	{
		pulse_width = tmp - preTimerCount;
	}
	else
	{
		pulse_width = 0x100000000 - preTimerCount + tmp;
	}
	
	preTimerCount = tmp;
	#else
	pulse_width = time;

	//DBG_RMTPrintf("RC S level:%d %d %d\n\r", remote_state, pulse_width, pin_level);
	#ifdef SYS_CLK_VOLATILE
	if ( (pulse_width > leader_on_pulse_width_max) )
	#else
	if ( (pulse_width > FRAME_END_PULSE_MAX) )
	#endif
	{
		return;
	}
	#endif

	//int_count++;
	//if (int_count == 3)
	//{
	//	asm("nop");
	//}
	
	//DBG_RMTPrintf("RC S:%d %d %d\n\r", remote_state, pulse_width, pin_level);

	switch (remote_state)
	{
	case RC_IDLE:
		#ifdef REMOTER_USED_GPIO_INTERRUPT
		INT_ON_RISING_EDGE;
		remote_state = RC_LEADER_ON;
		
		#ifdef REMOTE_CONTROL_DEBUG
		//DBG_Puts("RC IDLE\n\r");
		//DBG_RMTPrintf("RC IDLE:%d\n\r", pulse_width);
		#endif
		
		break;
		#endif

	case RC_LEADER_ON:

		
		#ifdef SYS_CLK_VOLATILE
		if (pulse_width > leader_on_pulse_width_min && pulse_width < leader_on_pulse_width_max)
		#else
		if (pulse_width > LEADER_ON_PULSE_MIN && pulse_width < LEADER_ON_PULSE_MAX)
		#endif
		{
			if(pin_level == Remote_Pin_LLevel)
			{
				remote_state = RC_LEADER_OFF;

				#ifdef REMOTE_CONTROL_DEBUG
				//DBG_Puts("RC L On ok\n\r");
				//DBG_RMTPrintf("RC L On Ok:%d\n\r", pulse_width);
				//DBG_RMTPrintf("RC L On Ok:%d, %d, %d\n\r", pulse_width, temp_min, temp_max);
				#endif
			}
		}
		else
		{
			remote_state = RC_IDLE;

			#ifdef REMOTE_CONTROL_DEBUG
			//DBG_Puts("-\n\r");
			//DBG_Puts("RC L On Err\n\r");
			//DBG_RMTPrintf("RC L On Err:%d\n\r", pulse_width);
			//DBG_RMTPrintf("RC L On Err:%d, %d, %d\n\r", pulse_width, temp_min, temp_max);
			#endif
		}

		INT_ON_FALLING_EDGE;
		
		break;

	case RC_LEADER_OFF:
		#ifdef SYS_CLK_VOLATILE
		if (pulse_width > leader_off_pulse_width_min && pulse_width < leader_off_pulse_width_max)
		#else
		if (pulse_width > LEADER_OFF_PULSE_MIN && pulse_width < LEADER_OFF_PULSE_MAX)
		#endif
		{
			if(pin_level == Remote_Pin_HLevel)
			{
				#if 0
				remote_state = RC_DATA;
				remote_repeat_valid = FALSE;
				#else
				remote_state = RC_CUSTOM;
				#endif
				data = 0;
				count = 0;
				//repeat_count = 0;
				
				app_timer_remoteKeyUpTimer_set(150);

				#ifdef REMOTE_CONTROL_DEBUG
				//DBG_Puts("RC L Off ok\n\r");
				//DBG_RMTPrintf("RC L Off ok:%d\n\r", pulse_width);
				#endif
			}
			else
			{
				remote_state = RC_IDLE;
				data = 0;
				count = 0;
				repeat_count = 0;
				remote_repeat_valid = FALSE;
				//DBG_RMTPrintf("err 0\n\r");
			}
		} 	
		break;

	case RC_CUSTOM:
		if (pulse_width > LEADER_CUSTOM_PULSE_MIN && pulse_width < LEADER_CUSTOM_PULSE_MAX)
		{
			if(pin_level == Remote_Pin_LLevel)
			{
				remote_state = RC_DATA;
			}	
		}
		else
		{
			remote_state = RC_IDLE;
		}
		break;

	case RC_DATA:
	
		//DBG_RMTPrintf("RC DATA:%d,%d\n\r", pulse_width, count);
		count++;

		if(pin_level == Remote_Pin_HLevel)
		{
			#ifdef SYS_CLK_VOLATILE
			if (pulse_width > leader_1t_pulse_width_min && pulse_width < leader_1t_pulse_width_max)
			#else
			if (pulse_width >= LEADER_1T_PULSE_MIN && pulse_width <= LEADER_1T_PULSE_MAX)
			#endif
			{
				data <<= 2;
				
				//RmtValPrintf("0-RC Data :0x%X\n\r", data);
				//DBG_RMTPrintf("0-RC Data pw: %d\n\r", pulse_width);
			}
			else 
			{
				#ifdef SYS_CLK_VOLATILE
				if (pulse_width > leader_2t_pulse_width_min && pulse_width < leader_2t_pulse_width_max)
				#else
				if (pulse_width >= LEADER_2T_PULSE_MIN && pulse_width <= LEADER_2T_PULSE_MAX)
				#endif
				{
					data = (data << 2) | 0x01;
					//RmtValPrintf("1-RC Data :0x%X\n\r", data);
					//DBG_RMTPrintf("1-RC Data pw: %d\n\r", pulse_width);
				}
				#ifdef SYS_CLK_VOLATILE
				else if (pulse_width > leader_2t_pulse_width_min && pulse_width < leader_2t_pulse_width_max)
				#else
				else if (pulse_width >= LEADER_3T_PULSE_MIN && pulse_width <= LEADER_3T_PULSE_MAX)
				#endif
				{
					data = (data << 2) | 0x02;
					//RmtValPrintf("2-RC Data :0x%X\n\r", data);
					//DBG_RMTPrintf("2-RC Data pw: %d\n\r", pulse_width);
				}

				#ifdef SYS_CLK_VOLATILE
				else if (pulse_width > leader_2t_pulse_width_min && pulse_width < leader_2t_pulse_width_max)
				#else
				else if (pulse_width >= LEADER_4T_PULSE_MIN && pulse_width <= LEADER_4T_PULSE_MAX)
				#endif
				{
					data = (data << 2) | 0x03;
					//RmtValPrintf("3-RC Data :0x%X\n\r", data);
					//DBG_RMTPrintf("3-RC Data pw: %d\n\r", pulse_width);
				}
				else
				{
					remote_state = RC_IDLE;
					remote_repeat_valid = FALSE;

					DBG_RMTPrintf("err 3\n\r");
					#ifdef REMOTE_CONTROL_DEBUG
					//DBG_Puts("+\n\r");
					//DBG_Puts("RC Data Err\n\r");
					//DBG_RMTPrintf("RC Data Err:%d %d\n\r", count, pulse_width);
					#endif

					break;
				}
			}
				

			if (count == 10)
			{	
				//DBG_RMTPrintf("err :%d\n\r", remote_repeat_valid);
				remote_state = RC_DATA_END;
			}
			else
			{
				remote_state = RC_CUSTOM;
			}

		}
		else
		{
			remote_state = RC_IDLE;
			data = 0;
			repeat_count = 0;
			remote_key = 0;
			remote_key_bak = 0;
			remote_repeat_valid = FALSE;
			//DBG_RMTPrintf("err 1\n\r");
		}
		break;

	case RC_DATA_END:
		if (pulse_width > FRAME_DATA_END_PULSE_MIN && pulse_width < FRAME_DATA_END_PULSE_MAX)
		{
			//DBG_RMTPrintf("remote_repeat_valid : %d,0x%x, 0x%x, %d, %d\n\r", remote_repeat_valid, remote_key_bak, data, remote_key_state, repeat_count);
			if(pin_level == Remote_Pin_LLevel)
			{
				#if 1
				
				if (remote_repeat_valid)
				{
					#ifdef REMOTE_KEY_VALUE_DEBUG
					DBG_RMTPrintf("D1:0x%x\n\r", data);
					#endif

					if (remote_key_bak == data)
					{
						//DBG_RMTPrintf("remote_key_cnt :%d\n\r", remote_key_cnt);
					
						app_timer_remoteKeyUpTimer_set(150);
					}
					else
					{
						//DBG_RMTPrintf("-remote_key_cnt :%d\n\r", remote_key_cnt);
						app_timer_remoteKeyUpTimer_set(10);
					}

				}
				else
				{
					remote_repeat_valid = TRUE;
					remote_key = data;
					remote_key_bak = data;
					//remote_key_cnt = 0;
					
					#ifndef REMOTE_DEBUG
					//remote key TO map key
					remote_key = vRemoteMapMainKey (remote_key);
					#endif
					
					#ifdef SOFT1_INT_USED
					SOFT0_INT_SET;
					#else
					SoftInt1Isr();
					#endif
					//DBG_RMTPrintf("¡¤-remote_key_cnt :%d\n\r", remote_key_cnt);
					app_timer_remoteKeyUpTimer_set(150);
					

				}
				#endif

				remote_state = RC_FRAME_END;
				//remote_state = RC_IDLE;
			}
			else
			{
				data = 0;
				repeat_count = 0;
				remote_key = 0;
				remote_key_bak = 0;
				remote_repeat_valid = FALSE;
				remote_state = RC_IDLE;
				DBG_RMTPrintf("err 2\n\r");
			}
		}
		break;


	#ifndef REMOTER_USED_GPIO_INTERRUPT
	#if 1
	case RC_FRAME_END:
		temp_min = FRAME_END_PULSE_MIN;
		temp_max = FRAME_END_PULSE_MAX;		

		remote_state = RC_IDLE;
		
		if (pulse_width > FRAME_END_PULSE_MIN && pulse_width < FRAME_END_PULSE_MAX)
		{
			#ifdef REMOTE_CONTROL_DEBUG
			//DBG_RMTPrintf("RC End Ok\n\r");
			#endif
		}
		else
		{
			#ifdef REMOTE_CONTROL_DEBUG
			//DBG_RMTPrintf("RC End Err\n\r");
			//DBG_RMTPrintf("RC End Err: %d %d %d\n\r", pulse_width, temp_min, temp_max);
			#endif
		}
		
		break;
	#endif	
	#endif

	default:
		remote_state = RC_IDLE;
		break;
	}
		
#endif
}



static void SoftInt1Isr(void)
{
#ifndef REMOTE_DEBUG
	U32 rmt_key;
	remote_key_state = KEY_DOWN;
	key_msgSend_formIsr ( REMOTE_KEY , KEY_DOWN, remote_key);
	//DBG_RMTPrintf("RMT:key down 0x%x\n\r", remote_key);
#endif
}

void Remote_Int_Disable(void)
{
	REMOTE_INT_DISABLE;
}

void Remote_Int_Enable(void)
{
	REMOTE_INT_ENABLE;
}
#endif //REMOTE_XIAOMI_FORMAT
#endif //REMOTE_ENABLE
