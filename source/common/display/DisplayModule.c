/*
 * DisplayModule.c
 *
 *  Created on: 2024/1/29
 *      Author: Windows
 */
#include "os_config.h"
#include "c_def.h"
#include "debug.h"
#include "oem.h"

#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#include "LCDdevice.h"
#include "LEDdevice.h"
#include "bt_common.h"
#include "app_string_oem.h"

#define DISPLAY_FUNCTION_ENABLE
#define DBG_Printf

const APP_DISPLAY_MOUDLE app_display_module = {Display_Module};

void app_display_none_void(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE

	#endif
}

void app_display_module_init(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE
	#ifdef LCD_MODULE_ENABLE
	app_lcd_device.init();
	#endif

	#ifdef LED_MODULE_ENABLE
	app_led_device.init();
	#endif
	#endif
}

void app_display_module_open(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE
	#ifdef LCD_MODULE_ENABLE
	app_lcd_device.open();
	#endif

	#ifdef LED_MODULE_ENABLE
	app_led_device.open();
	#endif	
	#endif
}

void app_display_module_close(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE

	#endif
}

void app_display_module_finish(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE

	#endif
}

void app_display_module_disp_source(U8 audio_source)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE
	#ifdef LCD_MODULE_ENABLE
	app_lcd_device.display_source(audio_source);
	#endif
	#ifdef LED_MODULE_ENABLE
	app_led_device.disp_source(audio_source);
	#endif
	#endif
}

void app_display_module_disp_standby_status(U8 standby_status)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE
	if (standby_status)
	{
		#ifdef LCD_MODULE_ENABLE
		app_lcd_device.clear(LINE123);
		app_lcd_device.display_char(STRING_STANDBY_MODE_ON, 0, 3);
		#endif
		#ifdef LED_MODULE_ENABLE
		app_led_device.disp_standby_status(standby_status);
		#endif
	
	}
	else
	{
		#ifdef LCD_MODULE_ENABLE
		app_lcd_device.clear(LINE123);
		app_lcd_device.display_char(STRING_POWER_ON, 0, 4);
		#endif
		#ifdef LED_MODULE_ENABLE
		app_led_device.disp_standby_status(standby_status);
		#endif	
	}
	#endif	
}

void app_display_module_disp_bt_status(U32 flash_time)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE

	#ifdef BTDM52_ENABLE
    switch (bt_status.bt_main_status)
    {
        case enBT_CONNECTED:
            switch (bt_status.bt_avrcp_status)
            {
                case enBT_AVRCP_PLAYING:
					#ifdef LCD_MODULE_ENABLE
                    app_lcd_device.display_char(STRING_BT_MODE_PLAYING, 1, 3);
					#endif
                    break;

                case enBT_AVRCP_PAUSED:
					#ifdef LCD_MODULE_ENABLE
                    app_lcd_device.display_char(STRING_BT_MODE_PAUSE, 1, 4);
					#endif
                    break;

                case enBT_AVRCP_STOPPED:
					#ifdef LCD_MODULE_ENABLE
                    app_lcd_device.display_char(STRING_BT_MODE_STOPPED, 1, 3);
					#endif
                    break;

                default:
					#ifdef LCD_MODULE_ENABLE
                    app_lcd_device.display_char(STRING_BT_MODE_CONNECTED, 1, 2);
					#endif
                    break;
            }
			#ifdef LED_MODULE_ENABLE
			app_led_device.disp_bt_status(dispBT_CONNECTED, flash_time);
			#endif
            break;

        case enBT_DISCONNECTED:
			#ifdef LCD_MODULE_ENABLE
            app_lcd_device.display_char(STRING_BT_MODE_DISCONNECTED, 1, 1);
			#endif

			#ifdef LED_MODULE_ENABLE
			app_led_device.disp_bt_status(dispBT_DISCONNECTED, flash_time);
			#endif			
            break;

        case enBT_PAIRING:
			#ifdef LCD_MODULE_ENABLE
            app_lcd_device.display_char(STRING_BT_MODE_PAIRING, 1, 3);
			#endif
			
			#ifdef LED_MODULE_ENABLE
			app_led_device.disp_bt_status(dispBT_PAIRING, flash_time);
			#endif			
            break;

        case enBT_RELINK:
			#ifdef LCD_MODULE_ENABLE
            app_lcd_device.display_char(STRING_BT_MODE_RELINK, 1, 3);
			#endif
			
			#ifdef LED_MODULE_ENABLE
			app_led_device.disp_bt_status(dispBT_RELINK, flash_time);
			#endif			
            break;
		case enBT_UPGRAGE:
			#ifdef LED_MODULE_ENABLE
			app_led_device.disp_bt_status(dispBT_UPGRAGE, flash_time);
			#endif			
            break;

        default:
			#ifdef LCD_MODULE_ENABLE
            app_lcd_device.display_char(STRING_BT_MODE_NO_PAIRED, 1, 2);
			#endif
			
			#ifdef LED_MODULE_ENABLE
			app_led_device.disp_bt_status(dispBT_DISCONNECTED, flash_time);
			#endif			
            break;
    }
	#endif

	#endif
}

void app_display_module_disp_usb_status(U8 usb_status)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE
	switch (usb_status)
	{
		case dispUSB_INIT:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_USB_MSC_MODE_INIT, 1, 5);
			
			#endif
			break;

		case dispUSB_OPEN:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_USB_MSC_MODE_OPEN, 1, 5);
			#endif
			break;

		case dispUSB_INFO:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_USB_MSC_MODE_INFO, 1, 5);
			#endif
			break;

		case dispUSB_READING:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_USB_MSC_MODE_READING, 1, 2);
			#endif
			break;

		case dispUSB_PLAYING:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_USB_MSC_MODE_PLAY, 1, 2);
			#endif
			break;

		case dispUSB_PAUSE:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_USB_MSC_MODE_PAUSE, 1, 4);
			#endif
			break;

		case dispUSB_STOP:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_USB_MSC_MODE_STOP, 1, 5);
			#endif
			break;


		default:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			#endif
			break;
	}
	#endif
}

void app_display_module_disp_decode_type(U8 decode_type)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE
	switch (decode_type)
	{
		case dispDeCode_DD_AC3:
			#ifdef LCD_MODULE_ENABLE
			if(audio_out_stream.input_channel == 2)
			{
				app_lcd_device.display_char(STRING_DECODE_MODE_DD_AC3_2, 1, 1);
			}
			else
			{
				app_lcd_device.display_char(STRING_DECODE_MODE_DD_AC3_5, 1, 1);
			}
			#endif
			break;
		
		case dispDeCode_DDp_EC3:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_DECODE_MODE_DDp_EC3, 1, 2);
			#endif
			break;
		
		case dispDeCode_DTS:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_DECODE_MODE_DTS, 1, 4);
			#endif
			break;
		
		case dispDeCode_PCM:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_DECODE_MODE_PCM, 1, 4);
			#endif
			break;
		
		default:
			#ifdef LCD_MODULE_ENABLE
			app_lcd_device.display_char(STRING_NULL, 1, 0);
			#endif
			break;
	}
	#endif
}

void app_display_module_disp_music_play(U32 music_level)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE
	#ifdef LED_MODULE_ENABLE
	app_led_device.disp_music_play(music_level);
	#endif
	#endif
}
U8 combined_string_and_value(U8 *output_buf, U32 output_buf_len, U8 *string_buf, U32 string_buf_len, U32 combined_value, U32 combined_pos)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE
	int i,i_max;
	U8 ret = FALSE;
	DBG_Printf("combined_value:%d\n\r", combined_value);
	if ((combined_pos > 0xFF) || (combined_pos >= output_buf_len))
	{
		return ret;
	}

	if (output_buf_len < string_buf_len)
	{
		return ret;
	}

	memset(output_buf+string_buf_len, ' ', output_buf_len);
	memcpy(output_buf, string_buf, string_buf_len);


	if (combined_value/1000000000)
	{
		i_max = 9;
	}
	else if (combined_value/100000000)
	{
		i_max = 8;
	}
	else if (combined_value/10000000)
	{
		i_max = 7;
	}
	else if (combined_value/1000000)
	{
		i_max = 6;
	}
	else if (combined_value/100000)
	{
		i_max = 5;
	}
	else if (combined_value/10000)
	{
		i_max = 4;
	}
	else if (combined_value/1000)
	{
		i_max = 3;
	}
	else if (combined_value/100)
	{
		i_max = 2;
	}
	else if (combined_value/10)
	{
		i_max = 1;
	}
	else if (combined_value%10)
	{
		i_max = 0;
	}
	else
	{
		i_max = 0;
	}

	DBG_Printf("i_max:%d\n\r", i_max);

	if (combined_pos)
	{
		for (i=0; i<combined_pos; i++)
		{
			output_buf[(string_buf_len-1) + i] = ' ';
		}		
	}


	if (i_max)
	{
		for (i=0; i<i_max; i++)
		{
			output_buf[(string_buf_len-1) + combined_pos + i] = (combined_value/pow(10, (i_max-i))) + 48;
		}		
	}
	output_buf[(string_buf_len-1) + combined_pos + i_max] = (combined_value%10) + 48;

	output_buf[(string_buf_len-1) + combined_pos + i_max + 1] = '\0';

	for (i=0; i<(string_buf_len + combined_pos + i_max + 1); i++)
	{
		DBG_Printf("output_buf[%d]:%c,%d\n\r", i, output_buf[i], output_buf[i]);
	}

	ret = TRUE;

	return ret;
	#endif	
}

void app_display_module_disp_volume_status(U8 value)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DISPLAY_FUNCTION_ENABLE
	U8 volume_display_buf[LCD_DISP_BUF_SIZE] = {"vol disp err\0\0"};
	U8 STRING_VOLUME[] = {"vol:"};
	if (!(combined_string_and_value(volume_display_buf, LCD_DISP_BUF_SIZE, STRING_VOLUME, (sizeof(STRING_VOLUME)/sizeof(STRING_VOLUME[0])), 
									value, 1)))
	{
		DBG_Printf("combined_string_and_value err\n\r");
	}

	#ifdef LCD_MODULE_ENABLE
	app_lcd_device.display_char(volume_display_buf, 1, 3);
	#endif
	#endif
}

#endif

