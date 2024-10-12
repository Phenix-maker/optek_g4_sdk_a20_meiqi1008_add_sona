/*
 * LED_gpio_light.h
 *
 *  Created on: 2024/2/3
 *      Author: Windows
 */

#ifndef __LED_GPIO_LIGHT__
#define __LED_GPIO_LIGHT__

#if (LED_GPIO_LIGHT_COUNT_MAX > 0)  //overload_led
#define LED_0_GPIO_PIN              GPIO_UNDEFINE_INDEX//GPIO0_19_INDEX
#define LED_0_GPIO_PIN_INIT      	hal_gpio_mux_func_select(LED_0_GPIO_PIN, MUX_SEL_GPIO_OUTPUT)
#define LED_0_GPIO_PIN_HIGH    		hal_gpio_output_level_set(LED_0_GPIO_PIN, GPIO_LOW)
#define LED_0_GPIO_PIN_LOW    		hal_gpio_output_level_set(LED_0_GPIO_PIN, GPIO_HIGH)
#endif

#if (LED_GPIO_LIGHT_COUNT_MAX > 1)  //overload_led1
#define LED_1_GPIO_PIN              GPIO_UNDEFINE_INDEX//GPIO0_14_INDEX// GPIO0_14_INDEX
#define LED_1_GPIO_PIN_INIT      	hal_gpio_mux_func_select(LED_1_GPIO_PIN, MUX_SEL_GPIO_OUTPUT)
#define LED_1_GPIO_PIN_HIGH    		hal_gpio_output_level_set(LED_1_GPIO_PIN, GPIO_LOW)
#define LED_1_GPIO_PIN_LOW    		hal_gpio_output_level_set(LED_1_GPIO_PIN, GPIO_HIGH)
#endif


enum {
    enLED_INDEX_UNDOWN  = -1,
    enLED_0_INDEX,
    enLED_1_INDEX
};

void LED_gpio_light_init(void);
void LED_gpio_light_open(void);
void LED_gpio_light_close(void);
void LED_gpio_light_fini(void);
void LED_gpio_light_disp_source(U8 audio_source);
void LED_gpio_light_disp_bt_status (U8 bt_status, U32 flash_time);
void LED_gpio_light_disp_music_play(U32 music_level);
void LED_gpio_light_disp_standby_status(U8 standby_status);

#define LED_GPIO_LIGHT_FUNC     LED_gpio_light_init,\
                                LED_gpio_light_open,\
                                LED_gpio_light_close,\
                                LED_gpio_light_fini,\
                                LED_gpio_light_disp_source,\
                                LED_gpio_light_disp_bt_status,\
                                LED_gpio_light_disp_music_play,\
                                LED_gpio_light_disp_standby_status

#endif /* __LED_GPIO_LIGHT__ */
