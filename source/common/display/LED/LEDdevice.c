#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "LEDdevice.h"

#define APP_LED_FUNC_ENABLE

#ifdef LED_MODULE_ENABLE

#ifdef LED_5050RGB
#include "LED_5050RGB.h"
const APP_LED_DEVICE app_led_device = {LED_5050RGB_FUNC};
#elif defined LED_GPIO_LIGHT
#include "LED_gpio_light.h"
const APP_LED_DEVICE app_led_device = {LED_GPIO_LIGHT_FUNC};
#else
const APP_LED_DEVICE app_led_device = {LED_NULL_FUNC};
#endif

void led_none_void(void)
{

}
void led_none_disp_source(U8 audio_source)
{

}

void led_none_disp_bt_status (U8 bt_status, U32 flash_time)
{
    
}

#endif

