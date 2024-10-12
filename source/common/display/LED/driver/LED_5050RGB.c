#include "c_def.h"
#include "debug.h"
#include "oem.h"


#ifdef LED_5050RGB
#include "os_config.h"
#include "hw_timer.h"
#include "hw_spi.h"
#include "hal_gpio.h"
#include "flash_boot.h"
#include "DisplayModule.h"
#include "LED_5050RGB.h"

#define LED_5050RGB_FUNC_ENABLE


#define SPI_SELECT              spi_transfer_size_set(32, 0)

#define Led_Write_data(x)       Spi_Write_32Bits(x); \
                                delayus(1)

#define Led_Write_reset         Spi_Write_32Bits(0x00000000); \
                                delayus(1)                                

void 



LED_5050RGB_init(void)
{
	DBG_Printf("%s\n\r", __func__);
    #ifdef LED_5050RGB_FUNC_ENABLE

    #endif
}

void LED_5050RGB_open(void)
{
	DBG_Printf("%s\n\r", __func__);
    #ifdef LED_5050RGB_FUNC_ENABLE

    #endif  
}


void LED_5050RGB_close(void)
{
	DBG_Printf("%s\n\r", __func__);
    #ifdef LED_5050RGB_FUNC_ENABLE

    #endif
}

void LED_5050RGB_fini(void)
{
	DBG_Printf("%s\n\r", __func__);
    #ifdef LED_5050RGB_FUNC_ENABLE

    #endif
}

void LED_5050RGB_disp_source(U8 audio_source)
{
	DBG_Printf("%s\n\r", __func__);
    #ifdef LED_5050RGB_FUNC_ENABLE
	switch (audio_source)
	{
		case SOURCE_AUX:

			LED_5050RGB_oneLed_set(128,0,0);

			break;

		case SOURCE_ARC:
            LED_5050RGB_oneLed_set(128,128,0);
			break;

		case SOURCE_HDMI:
            LED_5050RGB_oneLed_set(128,128,128);
			break;

		case SOURCE_COAXIAL:
            LED_5050RGB_oneLed_set(128,0,128);
			break;

		case SOURCE_OPTICAL:
            LED_5050RGB_oneLed_set(0,128,128);
			break;			

		case SOURCE_BT:
            LED_5050RGB_oneLed_set(0,0,255);
			break;

		case SOURCE_USB_MSC:
            LED_5050RGB_oneLed_set(255,255,255);
			break;

		case SOURCE_USB_SPEAKER:
            LED_5050RGB_oneLed_set(255,0,255);
			break;

		case SOURCE_INTERNAL:
            LED_5050RGB_oneLed_set(0,255,0);
			break;

		default:
            LED_5050RGB_oneLed_set(0,255,0);
			break;
	}

    #endif
}

void LED_5050RGB_disp_bt_status (U8 bt_status, U32 flash_time)
{
    switch (bt_status)
    {
        case dispBT_CONNECTED:
            #ifdef LED_MODULE_ENABLE
            LED_5050RGB_oneLed_set(0,0,255);
            #endif
            break;

        case dispBT_DISCONNECTED:
            #ifdef LED_MODULE_ENABLE
            if (flash_time%5 == 0)
            {
                LED_5050RGB_oneLed_set(0,0,255);
            }
            else
            {
                LED_5050RGB_oneLed_set(0,0,0);
            }            
            #endif
            break;

        case dispBT_PAIRING:
            #ifdef LED_MODULE_ENABLE
            if (flash_time%2 == 0)
            {
                LED_5050RGB_oneLed_set(0,0,255);
            }
            else
            {
                LED_5050RGB_oneLed_set(0,0,0);
            }            
            #endif
            break;

        case dispBT_AVRCP_PLAYING:
            #ifdef LED_MODULE_ENABLE
            LED_5050RGB_oneLed_set(0,0,255);
            #endif
            break;

        case dispBT_AVRCP_PAUSED:
            #ifdef LED_MODULE_ENABLE
            LED_5050RGB_oneLed_set(0,0,255);
            #endif
            break;

        case dispBT_AVRCP_STOPPED:
            #ifdef LED_MODULE_ENABLE
            LED_5050RGB_oneLed_set(0,0,255);
            #endif
            break;

        default:
            #ifdef LED_MODULE_ENABLE
            if (flash_time%5 == 0)
            {
                LED_5050RGB_oneLed_set(0,0,255);
            }
            else
            {
                LED_5050RGB_oneLed_set(0,0,0);
            }            
            #endif
            break;
    }    
}

void LED_5050RGB_disp_music_play(U32 music_level)
{
    LED_RGB_SPI_DATA *spi_data = (LED_RGB_SPI_DATA *)&music_level;
    
    if(music_level == 0)
    {
        LED_5050RGB_oneLed_set(0xFF,0XFF,0XFF);
    }
    else
    {
        LED_5050RGB_oneLed_set(spi_data->spi_red_data, spi_data->spi_green_data, spi_data->spi_blue_data);
    }
}


void LED_5050RGB_oneLed_set(U8 Red, U8 Green, U8 Blue)
{
    //DBG_Printf("%s:%d,%d,%d\n\r", __func__, Red, Green, Blue);

    U32 red_data    = 0x00000000;
    U32 green_data  = 0x00000000;
    U32 blue_data   = 0x00000000;
    int i;

    SPI_SELECT;

    if (Red > 0x00)
    {
        for (i=0;i<8;i++)
        {
            if (Red&(0x01<<i))
            {
                red_data |= (0xe<<(i*4));
            }
            else
            {
                red_data |= (0x8<<(i*4));
            }
        }
    }
    else
    {
        red_data = 0x88888888;
    }

    if (Blue > 0x00)
    {
        for (i=0;i<8;i++)
        {
            if (Blue&(0x01<<i))
            {
                blue_data |= (0xe<<(i*4));
            }
            else
            {
                blue_data |= (0x8<<(i*4));
            }
        }
    }
    else
    {
        blue_data = 0x88888888;
    }

    if (Green > 0x00)
    {
        for (i=0;i<8;i++)
        {
            if (Green&(0x01<<i))
            {
                green_data |= (0xe<<(i*4));
            }
            else
            {
                green_data |= (0x8<<(i*4));
            }
        }
    }
    else
    {
        green_data = 0x88888888;
    }    
    #if 0
    #define LED_COUNT_MAX   25
    U32 tmp = Red|(Green<<8)|(Blue<<16);
    U8 count = 0;
    U32 max_tmp = (0x574F-0x100)/(LED_COUNT_MAX-1);
    for (i=0;i<(LED_COUNT_MAX+1);i++)
    {
        if (tmp <= (max_tmp*i))
        {
            count = i;
            break;
        }

        count = i;

    }

    for (i=0;i<count;i++)
    {
        Led_Write_data(red_data);
        Led_Write_data(green_data);
        Led_Write_data(blue_data);
    }

    for (i=0;i<(25-count);i++)
    {
        Led_Write_data(0x88888888);
        Led_Write_data(0x88888888);
        Led_Write_data(0x88888888);
    }
    #else
    for (i=0;i<25;i++)
    {
        Led_Write_data(red_data);
        Led_Write_data(green_data);
        Led_Write_data(blue_data);
    }



    // Led_Write_data(green_data);
    // Led_Write_data(blue_data);
    // Led_Write_data(red_data);

    // Led_Write_data(blue_data);
    // Led_Write_data(red_data);
    // Led_Write_data(green_data);
    #endif
    Led_Write_reset;
}
#endif
