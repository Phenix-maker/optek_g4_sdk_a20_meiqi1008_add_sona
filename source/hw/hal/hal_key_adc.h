#ifndef __HAL_SARADC_H__
#define __HAL_SARADC_H__

#include "app_key.h"

#define HPM_SW		GPIO0_26_INDEX
#define F_SW		GPIO0_20_INDEX//GPIO_UNDEFINE_INDEX  GPIO0_20_INDEX
#define PHASE_SW	GPIO0_25_INDEX
#define FRO_SW		GPIO0_24_INDEX
#define SW_BATT		GPIO0_12_INDEX

#define BAT1_LED	GPIO0_28_INDEX
#define BAT2_LED	GPIO0_11_INDEX

#define BAT1_LED_R  GPIO0_16_INDEX
#define BAT1_LED_G  GPIO0_15_INDEX

#define BAT2_LED_R  GPIO0_22_INDEX
#define BAT2_LED_G  GPIO0_21_INDEX

#define F_LED_CTL	GPIO0_03_INDEX
#define I_CTL	    GPIO0_17_INDEX

#define BATT_EN     GPIO0_23_INDEX
#define AC_INT_DET  GPIO0_19_INDEX
#define PVDD_AC_EN  GPIO0_13_INDEX
#define MUTE_CLT    GPIO0_14_INDEX

// #define PSU_IN_12V_EN   GPIO0_28_INDEX
// #define OVER_DETECT GPIO0_13_INDEX

// #define OVER_LED_R  GPIO0_19_INDEX  //RED
// #define OVER_LED_G  GPIO0_14_INDEX  //GREEN

// #define BATT_VOL_MIN    (0x24E)
// #define BATT_VOL        (0x2C5 - 0x24E)

U8 hal_key_adc_detect(APP_KEY *app_keys, U8 max_simult);

#ifdef BATT_DETECT_ENABLE
void hal_adc_vol_convert(U32 tick);
void hal_batt_vol_detect(U32 tick);
void batt1_led(void);
void batt2_led(void);

#endif
#endif /* __HAL_SARADC_H__ */
