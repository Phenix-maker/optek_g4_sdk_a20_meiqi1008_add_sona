#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_


#define GPIO_INT_ENABLE


/*
 ****************************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef enum {
	GPIO_GROUP0,
	GPIO_GROUP_SREG,
	GPIO_GROUP_MAX,	
} enumGPIO_GROUP;

typedef enum {
	MUX_SEL_GPIO_INPUT,
	MUX_SEL_GPIO_OUTPUT,
	MUX_SEL_FUNCTION1,
	MUX_SEL_FUNCTION2,
	MUX_SEL_FUNCTION3,
	MUX_SEL_FUNCTION4,
} enumGPIOMUX_SEL;


typedef enum {
	GPIO_LOW,
	GPIO_HIGH,
} enumGPIO_LEVEL;


typedef enum {
	TRIGGER_EDGE,
	TRIGGER_LEVEL,
} enumGPIO_INT_TRIGGER_TYPE;

typedef enum {
	GPIO_INT_LOW_LEVEL,
	GPIO_INT_HIGH_LEVEL,
	GPIO_INT_FALLING_EDGE,
	GPIO_INT_RISING_EDGE,
} enumGPIO_INT_TRIGGER_LEVEL;

typedef enum
{
	GPIO_UNDEFINE_INDEX = -1,
	GPIO0_00_INDEX = 0,
	GPIO0_01_INDEX,
	GPIO0_02_INDEX,
	GPIO0_03_INDEX,
	GPIO0_04_INDEX,
	GPIO0_05_INDEX,
	GPIO0_06_INDEX,
	GPIO0_07_INDEX,
	GPIO0_08_INDEX,
	GPIO0_09_INDEX,
	GPIO0_10_INDEX,
	GPIO0_11_INDEX,
	GPIO0_12_INDEX,
	GPIO0_13_INDEX,
	GPIO0_14_INDEX,
	GPIO0_15_INDEX,
	GPIO0_16_INDEX,
	GPIO0_17_INDEX,
	GPIO0_18_INDEX,
	GPIO0_19_INDEX,
	GPIO0_20_INDEX,
	GPIO0_21_INDEX,
	GPIO0_22_INDEX,
	GPIO0_23_INDEX,
	GPIO0_24_INDEX,
	GPIO0_25_INDEX,
	GPIO0_26_INDEX,
	GPIO0_27_INDEX,
	GPIO0_28_INDEX,
	GPIO0_29_INDEX,
	GPIO0_30_INDEX,
	GPIO0_31_INDEX,

	GPIO_MAX_INDEX
} enumGPIO_INDEX;


typedef enum
{
	SREG_UNDEFINE_INDEX = -1,
	SREG_PWR_INDEX = 0,

	SREG_MAX_INDEX
} enumSREG_INDEX;


/*
 ****************************************************************************************
 * CONSTANT DEFINITIONS
 ****************************************************************************************
 */
#define UART0_TX_GPIO_INDEX    GPIO0_01_INDEX
#define UART0_RX_GPIO_INDEX    GPIO0_02_INDEX

#define UART1_TX_GPIO_INDEX    GPIO0_10_INDEX
#define UART1_RX_GPIO_INDEX    GPIO0_11_INDEX

#define UART2_TX_GPIO_INDEX    GPIO0_09_INDEX
#define UART2_RX_GPIO_INDEX    GPIO0_12_INDEX

#define DEBEG_GPIO_PIN      GPIO_UNDEFINE_INDEX
#define DEBEG_GPIO_PIN1     GPIO_UNDEFINE_INDEX
#define DEBEG_GPIO_PIN2     GPIO_UNDEFINE_INDEX

#define GPIO_INDEX2MASK(X)  (1<<(X-0))

#define GPIOx_HIGH(X)       do{if (X >= 0 && X < GPIO_MAX_INDEX) (REG_GPIO0_DOUT |= GPIO_INDEX2MASK(X));}while(0);
#define GPIOx_LOW(X)        do{if (X >= 0 && X < GPIO_MAX_INDEX) (REG_GPIO0_DOUT &= ~GPIO_INDEX2MASK(X));}while(0);


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/******** app gpio functions' declaration ************/
void hal_gpio_mux_func_select(enumGPIO_INDEX gpio_index, enumGPIOMUX_SEL mux_sel);
void hal_gpio_output_level_set(enumGPIO_INDEX gpio_index, enumGPIO_LEVEL level);
U32 hal_gpio_input_level_read(enumGPIO_INDEX gpio_index);
U32 hal_gpio_group_input_level_read(U8 gpio_group);
void hal_gpio_interrupt_set(enumGPIO_INDEX gpio_index,U8 enable,enumGPIO_INT_TRIGGER_TYPE Trigger_type,enumGPIO_INT_TRIGGER_LEVEL Trigger_level);
U32 hal_gpio_interrupt_st_read_use_index(enumGPIO_INDEX gpio_index);
U32 hal_gpio_interrupt_st_read_use_group(enumGPIO_GROUP gpio_group);

#endif //_HAL_GPIO_H_
