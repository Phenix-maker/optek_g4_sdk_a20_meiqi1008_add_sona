/*
 * LED_gpio_light.c
 *
 *  Created on: 2024/2/3
 *      Author: Windows
 */
#include "c_def.h"
#include "debug.h"
#include "oem.h"


#ifdef LED_GPIO_LIGHT
#include "DisplayModule.h"
#include "LEDdevice.h"
#include "LED_gpio_light.h"
#include "hal_key_adc.h"

#define LED_GPIO_LIGHT_FUNC_ENABLE
#define DBG_Printf

void LED_gpio_light_init(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef LED_GPIO_LIGHT_FUNC_ENABLE
    #if (LED_GPIO_LIGHT_COUNT_MAX > 0)
    LED_0_GPIO_PIN_INIT;
    LED_0_GPIO_PIN_LOW;
    #endif

    #if (LED_GPIO_LIGHT_COUNT_MAX > 1)
    LED_1_GPIO_PIN_INIT;
    LED_0_GPIO_PIN_LOW;
    #endif

    #endif
}
void LED_gpio_light_open(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef LED_GPIO_LIGHT_FUNC_ENABLE

    #endif
}

void LED_gpio_light_close(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef LED_GPIO_LIGHT_FUNC_ENABLE

    #endif
}

void LED_gpio_light_fini(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef LED_GPIO_LIGHT_FUNC_ENABLE

    #endif
}

void LED_gpio_light_disp_source(U8 audio_source)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef LED_GPIO_LIGHT_FUNC_ENABLE
    switch(global_vari.audio_source)
    {
        case SOURCE_BT:
        LED_0_GPIO_PIN_HIGH;
        LED_1_GPIO_PIN_LOW;

        case SOURCE_AUX:
        LED_0_GPIO_PIN_LOW;
        LED_1_GPIO_PIN_HIGH;
        default:
        LED_0_GPIO_PIN_LOW;
        LED_1_GPIO_PIN_LOW;

    }
    #endif
}

void LED_gpio_light_set(U8 led_index, U8 led_light_en)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef LED_GPIO_LIGHT_FUNC_ENABLE
    switch (led_index)
    {
        case enLED_0_INDEX:
            if (led_light_en == TRUE)
            {
                #if (LED_GPIO_LIGHT_COUNT_MAX > 0)
                LED_0_GPIO_PIN_HIGH;
                #endif
            }
            else
            {
                #if (LED_GPIO_LIGHT_COUNT_MAX > 0)
                LED_0_GPIO_PIN_LOW;
                #endif
            }
            break;

        case enLED_1_INDEX:
            if (led_light_en == TRUE)
            {
                #if (LED_GPIO_LIGHT_COUNT_MAX > 1)
                LED_1_GPIO_PIN_HIGH;
                #endif
            }
            else
            {
                #if (LED_GPIO_LIGHT_COUNT_MAX > 1)
                LED_1_GPIO_PIN_LOW;
                #endif
            }
            break;

        default :
            break;
    }
    #endif  
}

void LED_gpio_light_disp_bt_status (U8 bt_status, U32 flash_time)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef LED_GPIO_LIGHT_FUNC_ENABLE
    
    switch (bt_status)
    {
        case dispBT_CONNECTED:
            #ifdef LED_MODULE_ENABLE
            LED_gpio_light_set(enLED_0_INDEX, TRUE);
            LED_gpio_light_set(enLED_1_INDEX, TRUE);
            #endif
            break;

        case dispBT_DISCONNECTED:
            #ifdef LED_MODULE_ENABLE
            if (flash_time%5 == 0)
            {
                LED_gpio_light_set(enLED_0_INDEX, TRUE);
                LED_gpio_light_set(enLED_1_INDEX, TRUE);
                
                // hal_gpio_output_level_set(GPIO0_10_INDEX,GPIO_LOW);
            }
            else
            {
                LED_gpio_light_set(enLED_0_INDEX, FALSE);
                LED_gpio_light_set(enLED_1_INDEX, FALSE);

                // hal_gpio_output_level_set(GPIO0_10_INDEX,GPIO_HIGH);        
            }            
            #endif
            break;
        case dispBT_PAIRING:    
            #ifdef LED_MODULE_ENABLE
            if (flash_time%2 == 0)
            {
                LED_gpio_light_set(enLED_0_INDEX, TRUE);
                LED_gpio_light_set(enLED_1_INDEX, TRUE);
                
                #if 1
                hal_gpio_output_level_set(GPIO0_10_INDEX,GPIO_LOW);
                #endif
            }
            else
            {
                LED_gpio_light_set(enLED_0_INDEX, FALSE);
                LED_gpio_light_set(enLED_1_INDEX, FALSE);
                #if 1
                hal_gpio_output_level_set(GPIO0_10_INDEX,GPIO_HIGH);
                #endif
            }            
            #endif
            break;

        case dispBT_AVRCP_PLAYING:
            #ifdef LED_MODULE_ENABLE
            LED_gpio_light_set(enLED_0_INDEX, TRUE);
            LED_gpio_light_set(enLED_1_INDEX, TRUE);
            #endif
            break;

        case dispBT_AVRCP_PAUSED:
            #ifdef LED_MODULE_ENABLE
            LED_gpio_light_set(enLED_0_INDEX, TRUE);
            LED_gpio_light_set(enLED_1_INDEX, TRUE);
            #endif
            break;

        case dispBT_AVRCP_STOPPED:
            #ifdef LED_MODULE_ENABLE
            LED_gpio_light_set(enLED_0_INDEX, TRUE);
            LED_gpio_light_set(enLED_1_INDEX, TRUE);
            #endif
            // hal_gpio_output_level_set(GPIO0_10_INDEX,GPIO_HIGH);

            break;
        case dispBT_UPGRAGE:
            #ifdef LED_MODULE_ENABLE
            #endif
        default:
            #if 0 //def LED_MODULE_ENABLE
            if (flash_time%5 == 0)
            {
                LED_gpio_light_set(enLED_0_INDEX, TRUE);
                LED_gpio_light_set(enLED_1_INDEX, TRUE);
            }
            else
            {
                LED_gpio_light_set(enLED_0_INDEX, FALSE);
                LED_gpio_light_set(enLED_1_INDEX, FALSE);
            }            
            #endif
            break;
    }    
    
    #endif
}

void LED_gpio_light_disp_music_play(U32 music_level)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef LED_GPIO_LIGHT_FUNC_ENABLE

    #endif
}
void LED_gpio_light_disp_standby_status(U8 standby_status)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef LED_GPIO_LIGHT_FUNC_ENABLE

    #endif
}

#endif

