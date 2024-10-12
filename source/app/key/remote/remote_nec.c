#include "os_config.h"


#ifdef REMOTE_ENABLE

#include "regmap.h"

#include "mem_reloc.h"

#include "interrupt.h"
#include "hw_gpio.h"
#include "hw_timer.h"

#include "hal_gpio.h"

#include "remote.h"
#include "app_key.h"
#include "app_task.h"


#ifdef REMOTE_NEC_FORMAT

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

static void SoftInt1Isr(void)   __INTERNAL_RAM_TEXT;


// State of the remote key decoder
#define RC_IDLE          0
#define RC_LEADER_ON     1
#define RC_LEADER_OFF    2
#define RC_CUSTOM        3
#define RC_DATA          3
#define RC_DATA1         4
#define RC_DATA2         5
#define RC_DATA_END      6
#define RC_FRAME_END     7


#define REMOTE_CONTROL_DEBUG
#define REMOTE_KEY_VALUE_DEBUG

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

#ifdef STANDBY_ECO_PROJECT
volatile U8 remote_eco_status;
#endif

//value 0x00FF/0x01CE/0x81CE
#define CUSTOM_CODE   0x00FF


#define REPEAT_TIMERS    1


//Counter incr = RC_TB_DEN/RC_TB_NUM
#ifdef REMOTER_USED_GPIO_INTERRUPT
#ifdef SYS_CLK_VOLATILE
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



//H/L level hold time == ((9000 +/- xxx)*RC_TB_VAL)
#define LEADER_ON_PULSE_MIN       ((9000 - 1000)*RC_TB_VAL)
#define LEADER_ON_PULSE_MAX       ((9000 + 1000)*RC_TB_VAL)

//for standby eco
#define LEADER_ON_PULSE1_MIN       ((4500 - 1000)*RC_TB_VAL)
#define LEADER_ON_PULSE1_MAX       ((4500 + 1000)*RC_TB_VAL)

#define LEADER_OFF_PULSE_MIN      ((4500 - 500)*RC_TB_VAL)
#define LEADER_OFF_PULSE_MAX      ((4500 + 500)*RC_TB_VAL)


#define LEADER_1T_PULSE_MIN       ((1125 - 600)*RC_TB_VAL)
#define LEADER_1T_PULSE_MAX       ((1125 + 400)*RC_TB_VAL)

#define LEADER_2T_PULSE_MIN       ((2250 - 650)*RC_TB_VAL)
#define LEADER_2T_PULSE_MAX       ((2250 + 700)*RC_TB_VAL)

#define FRAME_END_PULSE_MIN       ((560 - 100)*RC_TB_VAL)
#define FRAME_END_PULSE_MAX       ((560 + 100)*RC_TB_VAL)


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

	remote_key = 0xFFFFFFFF;
	remote_key_state = KEY_UP;
	remote_repeat_valid = FALSE;
	remote_state = RC_IDLE;

	#ifdef STANDBY_ECO_PROJECT
	remote_eco_status = FALSE;
	#endif

	#ifdef SYS_CLK_VOLATILE
	Remote_Timer_Init();
	#endif

	#ifdef REMOTER_USED_GPIO_INTERRUPT
	hal_gpio_mux_func_select( REMOTE_INT_PIN, MUX_SEL_GPIO_INPUT);

	REMOTE_INT_CLEAR;

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

	hal_gpio_mux_func_select(REMOTE_INT_PIN, MUX_SEL_GPIO_INPUT);
	#endif

	XT_INTS_ON(LEVEL1_INT3_MASK);

	#ifndef REMOTE_DEBUG
	#ifdef SOFT1_INT_USED
	SOFT1_INT_ENABLE;
	#endif

	#endif
}

#ifdef REMOTER_USED_GPIO_INTERRUPT
void remoteHardISR(void)
#else
void remoteHardISR(int pin_level, int time)
#endif
{
	static U8 count;
	static U32 data;
	static U8 repeat_count = 0;
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
	#ifdef SYS_CLK_VOLATILE
	if ( (pulse_width > leader_on_pulse_width_max) )
	#else
	if ( (pulse_width > LEADER_ON_PULSE_MAX) )
	#endif
	{
		return;
	}
	#endif


	switch (remote_state)
	{
	case RC_IDLE:
		#ifdef REMOTER_USED_GPIO_INTERRUPT
		INT_ON_RISING_EDGE;
		remote_state = RC_LEADER_ON;
		
		#ifdef REMOTE_CONTROL_DEBUG
		DBG_Puts("RC IDLE\n\r");
		DBG_RMTPrintf("RC IDLE:%d\n\r", pulse_width);
		#endif
		
		break;
		#endif

	case RC_LEADER_ON:
		temp_min = LEADER_ON_PULSE_MIN;
		temp_max = LEADER_ON_PULSE_MAX;
		
	#ifdef STANDBY_ECO_PROJECT
		if (remote_eco_status)
		{
			remote_eco_status = FALSE;


			#ifdef SYS_CLK_VOLATILE
			if (pulse_width > leader_on_pulse_width_min && pulse_width < leader_on_pulse_width_max)
			#else
			if (pulse_width > LEADER_ON_PULSE1_MIN && pulse_width < LEADER_ON_PULSE1_MAX)
			#endif
			{
				remote_state = RC_LEADER_OFF;

				#ifdef REMOTE_CONTROL_DEBUG
				DBG_RMTPrintf("RC0 L On ok:%d\n\r", pulse_width);
				#endif
			}
			else
			{
				remote_state = RC_IDLE;

				#ifdef REMOTE_CONTROL_DEBUG
				DBG_RMTPrintf("RC L On Err:%d\n\r", pulse_width);
				#endif
			}
		}
		else
		{
			#ifdef SYS_CLK_VOLATILE
			if (pulse_width > leader_on_pulse_width_min && pulse_width < leader_on_pulse_width_max)
			#else
			if (pulse_width > LEADER_ON_PULSE_MIN && pulse_width < LEADER_ON_PULSE_MAX)
			#endif
			{
				remote_state = RC_LEADER_OFF;

				#ifdef REMOTE_CONTROL_DEBUG
				DBG_RMTPrintf("RC0 L On ok:%d\n\r", pulse_width);
				#endif
			}
			else
			{
				remote_state = RC_IDLE;

				#ifdef REMOTE_CONTROL_DEBUG
				DBG_RMTPrintf("RC L On Err:%d, %d, %d\n\r", pulse_width, temp_min, temp_max);
				#endif
			}
		}

	#else

		#ifdef SYS_CLK_VOLATILE
		if (pulse_width > leader_on_pulse_width_min && pulse_width < leader_on_pulse_width_max)
		#else
		if (pulse_width > LEADER_ON_PULSE_MIN && pulse_width < LEADER_ON_PULSE_MAX)
		#endif
		{
			remote_state = RC_LEADER_OFF;

			#ifdef REMOTE_CONTROL_DEBUG
			DBG_RMTPrintf("RC L On Ok:%d, %d, %d\n\r", pulse_width, temp_min, temp_max);
			#endif
		}
		else
		{
			remote_state = RC_IDLE;

			#ifdef REMOTE_CONTROL_DEBUG
			DBG_RMTPrintf("RC L On Err:%d, %d, %d\n\r", pulse_width, temp_min, temp_max);
			#endif
		}
		
	#endif //STANDBY_ECO_PROJECT

		INT_ON_FALLING_EDGE;
		break;

	case RC_LEADER_OFF:
		temp_min = LEADER_OFF_PULSE_MIN;
		temp_max = LEADER_OFF_PULSE_MAX;
		#ifdef SYS_CLK_VOLATILE
		if (pulse_width > leader_off_pulse_width_min && pulse_width < leader_off_pulse_width_max)
		#else
		if (pulse_width > LEADER_OFF_PULSE_MIN && pulse_width < LEADER_OFF_PULSE_MAX)
		#endif
		{
			remote_state = RC_DATA;
			data = 0;
			count = 0;
			repeat_count = 0;
			remote_repeat_valid = FALSE;

			#ifdef REMOTE_CONTROL_DEBUG
			DBG_RMTPrintf("RC L Off ok:%d\n\r", pulse_width);
			#endif
		} 
		else
		{
			#ifdef SYS_CLK_VOLATILE
			if (pulse_width > leader_2t_pulse_width_min && pulse_width < leader_2t_pulse_width_max)
			#else
			if (pulse_width > LEADER_2T_PULSE_MIN && pulse_width < LEADER_2T_PULSE_MAX)
			#endif
			{
				// repeat code, send the last key again
				// before sending, check if this key is repeatable.
				if (remote_repeat_valid)
				{
					app_timer_remoteKeyUpTimer_set(150);
				}


				#ifdef REMOTE_CONTROL_DEBUG
				DBG_RMTPrintf("RC RPT Ok:%d\n\r", pulse_width);
				#endif

				//INT_ON_FALLING_EDGE;
				#ifdef REMOTER_USED_GPIO_INTERRUPT
				remote_state = RC_IDLE;
				#else
				remote_state = RC_FRAME_END;
				#endif
			}
			else
			{
				#ifdef REMOTE_CONTROL_DEBUG
				DBG_RMTPrintf("RC L Off or RPT Err:%d, %d, %d\n\r", pulse_width, temp_min, temp_max);
				#endif

				remote_state = RC_IDLE;
			}
		}
		break;

	case RC_DATA:
		temp_min = LEADER_1T_PULSE_MIN;
		temp_max = LEADER_1T_PULSE_MAX;
		#ifdef REMOTER_USED_GPIO_INTERRUPT
		asm("nop");
		#else
		pulse_count++;
		if (pulse_count % 2 == 1)
		{
			pre_pulse_width = pulse_width;
			break;
		}
		else
		{
			pulse_width += pre_pulse_width;
		}
		#endif
		
		count++;

		#ifdef SYS_CLK_VOLATILE
		if (pulse_width > leader_1t_pulse_width_min && pulse_width < leader_1t_pulse_width_max)
		#else
		if (pulse_width > LEADER_1T_PULSE_MIN && pulse_width < LEADER_1T_PULSE_MAX)
		#endif
		{
			data <<= 1;
		}
		else
		{
			#ifdef SYS_CLK_VOLATILE
			if (pulse_width > leader_2t_pulse_width_min && pulse_width < leader_2t_pulse_width_max)
			#else
			if (pulse_width > LEADER_2T_PULSE_MIN && pulse_width < LEADER_2T_PULSE_MAX)
			#endif
			{
				data = (data << 1) | 1;
			}
			else
			{
				remote_state = RC_IDLE;
				remote_repeat_valid = FALSE;

				#ifdef REMOTE_CONTROL_DEBUG
				DBG_RMTPrintf("RC Data Err:%d %d\n\r", count, pulse_width);
				#endif

				break;
			}
		}

		if (count == 32)
		{
			#ifdef REMOTER_USED_GPIO_INTERRUPT
			remote_state = RC_IDLE;
			#else
			remote_state = RC_FRAME_END;
			#endif

			#ifdef REMOTE_CONTROL_DEBUG
			RmtValPrintf("RC Data:0x%X\n\r", data);
			#endif

			remote_key = data;
			remote_key_bak = data;
			remote_repeat_valid = TRUE;

			#ifndef REMOTE_DEBUG
			//remote key TO map key
			remote_key = vRemoteMapMainKey (remote_key);
			#endif

			#ifdef SOFT1_INT_USED
			SOFT1_INT_SET;
			#else
			SoftInt1Isr();
			#endif

			app_timer_remoteKeyUpTimer_set(150);
		}
		break;

	#ifndef REMOTER_USED_GPIO_INTERRUPT
	case RC_FRAME_END:
		temp_min = FRAME_END_PULSE_MIN;
		temp_max = FRAME_END_PULSE_MAX;		

		remote_state = RC_IDLE;
		if (pulse_width > FRAME_END_PULSE_MIN && pulse_width < FRAME_END_PULSE_MAX)
		{
			#ifdef REMOTE_CONTROL_DEBUG
			DBG_RMTPrintf("RC End Ok\n\r");
			#endif
		}
		else
		{
			#ifdef REMOTE_CONTROL_DEBUG
			DBG_RMTPrintf("RC End Err: %d %d %d\n\r", pulse_width, temp_min, temp_max);
			#endif
		}
		break;
	#endif

	default:
		remote_state = RC_IDLE;
		break;
	}

}



static void SoftInt1Isr(void)
{
#ifndef REMOTE_DEBUG
	U32 rmt_key;

	/*
	*if (app_main_data.key_process_state == KEY_HOLD)
	*{
	*	return;
	*}
	*/
	
	remote_key_state = KEY_DOWN;
	key_msgSend_formIsr ( REMOTE_KEY , KEY_DOWN, remote_key);
	DBG_RMTPrintf("RMT:key down 0x%x\n\r", remote_key);
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

#endif //REMOTE_NEC_FORMAT


#endif //REMOTE_ENABLE
