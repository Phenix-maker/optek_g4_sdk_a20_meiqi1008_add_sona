#ifndef _REMOTE_H_
#define _REMOTE_H_


//#define REMOTE_CONTROL_DEBUG
#define REMOTE_KEY_VALUE_DEBUG

#define RmtDebugPrintf  printf_info
#define RmtValPrintf    printf_info
#define DBG_RMTPrintf   printf_info

//#define REMOTE_DEBUG  //for get key code value
//#define REMOTER_USED_GPIO_INTERRUPT

#if (defined REMOTE_NEC_FORMAT)

#elif (defined REMOTER_SHARP_AUDIO_FORMAT)

#elif (defined REMOTER_PANASONIC_FORMAT)

#elif (defined REMOTE_PHILIPS_FORMAT)

#elif (defined REMOTE_PHILIPS_FORMAT)

#elif (defined REMOTE_MIXED_FORMAT)

#elif (defined REMOTE_XIAOMI_FORMAT)

#else
#ifdef REMOTE_ENABLE
#undef REMOTE_ENABLE
#endif

#endif //REMOTE_NEC_FORMAT


#define IR_IN_CH_0_PIN                 UART0_RX_GPIO_INDEX
#define IR_IN_CH_0_PIN_MASK            (1<<IR_IN_CH_0_PIN)
#define IR_IN_CH_1_PIN                 UART1_RX_GPIO_INDEX
#define IR_IN_CH_1_PIN_MASK            (1<<IR_IN_CH_1_PIN)
#define IR_IN_CH_2_PIN                 UART2_RX_GPIO_INDEX
#define IR_IN_CH_2_PIN_MASK            (1<<IR_IN_CH_2_PIN)

enum {
	IR_IN_CH_0 = 0,
	IR_IN_CH_1 = 1,
	IR_IN_CH_2 = 2,
	IR_IN_CH_LAST
};


#define REMOTE_INT_PIN                 UART1_RX_GPIO_INDEX
#define REMOTE_INT_PIN_MASK            (1<<(REMOTE_INT_PIN-GPIO0_00_INDEX))

#define REMOTE_PIN_STATE               GPIO0_READ(REMOTE_INT_PIN_MASK)
#define	REMOTE_INT_CLEAR               (REG_GPIO0_INTST |= REMOTE_INT_PIN_MASK)

#define	REMOTE_POSITIVE_INT_CLEAR       REMOTE_INT_CLEAR
#define	REMOTE_NEGATIVE_INT_CLEAR       REMOTE_INT_CLEAR


#define	REMOTE_INT_ENABLE               REG_GPIO0_INTEN |= REMOTE_INT_PIN_MASK
#define	REMOTE_INT_DISABLE              REG_GPIO0_INTEN &= ~REMOTE_INT_PIN_MASK

#define	REMOTE_POSITIVE_INT_DISABLE     REMOTE_INT_DISABLE
#define	REMOTE_NEGATIVE_INT_DISABLE     REMOTE_INT_DISABLE


#define	REMOTE_POSITIVE_INT_ENABLE      REG_GPIO0_INTTP1 |= REMOTE_INT_PIN_MASK
#define	REMOTE_NEGATIVE_INT_ENABLE      REG_GPIO0_INTTP1 &= ~REMOTE_INT_PIN_MASK



typedef enum {
	enRMT_FORMAT_UNKNOWN,
	enRMT_PHILIPS_RC5,
	enRMT_PHILIPS_RC6,
	enRMT_NEC_FORMAT,
	enRMT_SHARP_FORMAT,
	enRMT_SHARP_TV_FORMAT,
	enRMT_SAMSUNG_FORMAT,
	enRMT_SONY_FORMAT,
	enRMT_PANASONIC_FORMAT
} REMOTE_FORMAT;


typedef	struct {
	U8 start_bit	: 1;
	U8 ne_bit		: 1;		/* RC5 */
	U8 RC5_tr_bit	: 1;
	U8 RC6_tr_bit	: 1;
	U8 mode			: 3;		/* RC6 */
	
	U8 control;
	U8 command;
} REMOTE_INFO;


extern REMOTE_INFO remote_info;

extern volatile U32 remote_data;
extern volatile U32 remote_key;

extern volatile U8 remote_key_state;
extern volatile U8 remote_state;
extern volatile U8 remote_repeat_valid;
extern volatile U8 remote_key_cnt;

static U32 pre_pin_level = 1;
static U32 pulse_width_time = 0;

void Remote_Init(void);
void remote_ir_recive(U32 key_val);

void remote_sony_key_down_send(void);

#ifdef REMOTER_USED_GPIO_INTERRUPT
void remoteHardISR(void);
#else
void remoteHardISR(int pin_level, int time);
#endif

#endif //_REMOTE_H_
