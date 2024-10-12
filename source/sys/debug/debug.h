#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "regmap.h"

#include "hal_gpio.h"

#include "printf.h"


// #define JTAG_DEBUG_MODE //Note:elf32xtensa.x files must be modified simultaneously
// #define DEBUG_MODE

//#define DEBUG_GPIO_ENABLE


#ifdef DEBUG_GPIO_ENABLE
#define DEBUG_GPIO_PIN0_ENABLE
//#define DEBUG_GPIO_PIN1_ENABLE
//#define DEBUG_GPIO_PIN2_ENABLE
#endif


#define AUDIO_FIFO_ENABLE


#define UART0_USED_FOR_DEBUG
//#define UART1_USED_FOR_DEBUG
//#define UART2_USED_FOR_DEBUG


void debug_assert (BOOL condition);


#define _DEBUG

#ifdef _DEBUG

#define DBG_Assert(x)           debug_assert(x)
#define	DBG_assert(x)
#define DBG_RMTAssert(x)
#define DBG_FlashAssert(x)      debug_assert(x)
#define DBG_Printf              myprintf
#define BT_Printf               myprintf
#define	DBG_printf              myprintf
#define DBG_ServoPrintf         myprintf
#define DBG_FlashPrintf         myprintf
#define DBG_TPrintf             myprintf
#define DBG_NTCPrintf
#define DBG_USBHPrintf          myprintf
#define DBG_USBIRPPrintf        myprintf
#define DBG_OPTPrintf
#define DBG_KEYPrintf           myprintf
#define DBG_RMTPrintf           myprintf
#define DBG_TrayPrintf          myprintf
#define DBG_CodecPrintf         myprintf

#define DBG_NO_OS_Printf        myprintf

#define DBG_ASFPrintf           DBG_Printf
#define	DBG_Puts                myputs
#define DBG_RMTPuts             myputs

#define DBG_USBDev_Printf       myprintf
#define DBG_USBDev_iPrintf      myprintf_for_interrupt

#define DBG_iPrintf             myprintf_for_interrupt
#define DBG_iPuts               myputs_for_interrupt

#define	HALT
#define	HALT2

#define	DBG_iprintf
#define	DBG_PRINT_FATAL_ERR_LOCATION	myprintf("fatal error: %s(%d): %s\n\r", __FILE__, __LINE__, __FUNCTION__)

#else

#define DBG_Assert(x)
#define	DBG_assert(x)
#define DBG_RMTAssert(x)
#define DBG_FlashAssert(x)
#define DBG_Printf(...)
#define DBG_ServoPrintf(...)
#define DBG_TPrintf(...)
#define DBG_NTCPrintf(...)
#define DBG_USBHPrintf(...)
#define DBG_USBIRPPrintf(...)
#define DBG_OPTPrintf(...)
#define DBG_KEYPrintf(...)
#define DBG_RMTPrintf(...)
#define DBG_TrayPrintf(...)
#define DBG_CodecPrintf(...)
#define DBG_ASFPrintf(...)
#define DBG_FlashPrintf(...)
#define	DBG_Puts
#define DBG_USBDev_Printf(...)

#define DBG_iPrintf(...)
#define DBG_iPuts(...)

#define HALT
#define HALT2

#define	DBG_printf(...)
#define	DBG_iprintf(...)

#define	DBG_PRINT_FATAL_ERR_LOCATION		

#endif //_DEBUG



#define INDEX2MASK(X)   (1<<(X-GPIO0_00_INDEX))

#define GPIO_HIGH(X)    (REG_GPIO0_DOUT |= INDEX2MASK(X))
#define GPIO_LOW(X)     (REG_GPIO0_DOUT &= ~INDEX2MASK(X))

#define GPIO_MASK(X)    (1<<(X-GPIO0_00_INDEX))


#define DEBEG_PIN       UART0_RX_GPIO_INDEX
#define DEBEG_PIN_MASK  (1<<(DEBEG_PIN-GPIO0_00_INDEX))


#define DEBEG_PIN1      UART1_RX_GPIO_INDEX
#define DEBEG_PIN1_MASK (1<<(DEBEG_PIN1-GPIO0_00_INDEX))


#define DEBEG_PIN2      UART2_RX_GPIO_INDEX
#define DEBEG_PIN2_MASK (1<<(DEBEG_PIN2-GPIO0_00_INDEX))



/* The macro below is for DEBUG_GPIO_PIN0_ENABLE */
#define DEBEG_PIN0_CON_USED_DIRECTLY
//#define DEBEG_PIN0_CON_USED_INT_PROTECT

#ifdef DEBEG_PIN0_CON_USED_DIRECTLY

#ifdef DEBEG_PIN0_CON_USED_INT_PROTECT
#define DBG_PIN_HIGH(gpio_index)    {\
                                    register int interrupt_save;\
                                    TX_DISABLE;\
                                    REG_GPIO0_DOUT |= GPIO_MASK(gpio_index);\
                                    TX_RESTORE;\
                                    }

#define DBG_PIN_LOW(gpio_index)     {\
                                    register int interrupt_save;\
                                    TX_DISABLE;\
                                    REG_GPIO0_DOUT &= ~GPIO_MASK(gpio_index);\
                                    TX_RESTORE;\
                                    }
#else
#define DBG_PIN_HIGH(gpio_index)    REG_GPIO0_DOUT |= GPIO_MASK(gpio_index);

#define DBG_PIN_LOW(gpio_index)     REG_GPIO0_DOUT &= ~GPIO_MASK(gpio_index);
#endif //DEBEG_PIN0_CON_USED_INT_PROTECT

#else

#define DBG_PIN_HIGH(gpio_index)    hal_gpio_output_level_set(DEBEG_PIN, GPIO_HIGH);

#define DBG_PIN_LOW(gpio_index)     hal_gpio_output_level_set(DEBEG_PIN, GPIO_LOW);

#endif //DEBEG_PIN0_CON_USED_DIRECTLY



/* The macro below is for DEBUG_GPIO_PIN1_ENABLE */
#define DEBEG_PIN1_CON_USED_DIRECTLY
#define DEBEG_PIN1_CON_USED_INT_PROTECT

#ifdef DEBEG_PIN1_CON_USED_DIRECTLY

#ifdef DEBEG_PIN1_CON_USED_INT_PROTECT
#define DBG_PIN1_HIGH(gpio_index)    {\
                                    register int interrupt_save;\
                                    TX_DISABLE;\
                                    REG_GPIO0_DOUT |= GPIO_MASK(gpio_index);\
                                    TX_RESTORE;\
                                    }

#define DBG_PIN1_LOW(gpio_index)     {\
                                    register int interrupt_save;\
                                    TX_DISABLE;\
                                    REG_GPIO0_DOUT &= ~GPIO_MASK(gpio_index);\
                                    TX_RESTORE;\
                                    }
#else
#define DBG_PIN1_HIGH(gpio_index)   REG_GPIO0_DOUT |= GPIO_MASK(gpio_index);

#define DBG_PIN1_LOW(gpio_index)    REG_GPIO0_DOUT &= ~GPIO_MASK(gpio_index);
#endif //DEBEG_PIN1_CON_USED_INT_PROTECT

#else

#define DBG_PIN1_HIGH(gpio_index)   hal_gpio_output_level_set(DEBEG_PIN1, GPIO_HIGH);

#define DBG_PIN1_LOW(gpio_index)    hal_gpio_output_level_set(DEBEG_PIN1, GPIO_LOW);

#endif //DEBEG_PIN1_CON_USED_DIRECTLY



/* The macro below is for DEBUG_GPIO_PIN2_ENABLE */
#define DEBEG_PIN2_CON_USED_DIRECTLY
#define DEBEG_PIN2_CON_USED_INT_PROTECT

#ifdef DEBEG_PIN2_CON_USED_DIRECTLY

#ifdef DEBEG_PIN2_CON_USED_INT_PROTECT
#define DBG_PIN2_HIGH(gpio_index)    {\
                                    register int interrupt_save;\
                                    TX_DISABLE;\
                                    REG_GPIO0_DOUT |= GPIO_MASK(gpio_index);\
                                    TX_RESTORE;\
                                    }

#define DBG_PIN2_LOW(gpio_index)     {\
                                    register int interrupt_save;\
                                    TX_DISABLE;\
                                    REG_GPIO0_DOUT &= ~GPIO_MASK(gpio_index);\
                                    TX_RESTORE;\
                                    }
#else
#define DBG_PIN2_HIGH(gpio_index)   REG_GPIO0_DOUT |= GPIO_MASK(gpio_index);

#define DBG_PIN2_LOW(gpio_index)    REG_GPIO0_DOUT &= ~GPIO_MASK(gpio_index);
#endif //DEBEG_PIN2_CON_USED_INT_PROTECT

#else

#define DBG_PIN2_HIGH(gpio_index)   hal_gpio_output_level_set(DEBEG_PIN2, GPIO_HIGH);

#define DBG_PIN2_LOW(gpio_index)    hal_gpio_output_level_set(DEBEG_PIN2, GPIO_LOW);

#endif //DEBEG_PIN2_CON_USED_DIRECTLY


void debug_pin_init(void);
void debug_pin_test(void);

void delay_loop(U32 count);

#endif //_DEBUG_H_
