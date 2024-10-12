#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "LCDdevice.h"


void lcd_none_void(void)
{
}
void lcd_none_clear(LCD_RANGE lcd_range)
{
}
void lcd_none_display_icon(LCD_ICON parm1, U32 parm2)
{
}
void lcd_none_display_char(const U8 *buf, U16 line, U16 pos)
{
}
void lcd_none_display_char_fix_len(const U8 *buf, U16 line, U16 spos, U16 epos)
{
}

void lcd_none_display_dim_set(U8 val)
{
}

void lcd_none_led_light_set(U8 num, U8 con)
{
}
void lcd_none_display_level(U16 Level_L, U16 Level_R,U8 index)
{

}

void lcd_none_display_source(U8 audio_source)
{
	
}

void lcd_none_display_music_play(U32 music_level)
{
	
}

#if defined LCD_M66005
const APP_LCD_DEVICE app_lcd_device = {LCD_DEV_M66005};
#elif defined LCD_PT6302
const APP_LCD_DEVICE app_lcd_device = {LCD_DEV_PT6302};
#elif defined LCD_ML9208A
const APP_LCD_DEVICE app_lcd_device = {LCD_DEV_ML9208A};
#elif defined LCD_TY1621
const APP_LCD_DEVICE app_lcd_device = {TY1621};
#elif defined LCD_ET8861
const APP_LCD_DEVICE app_lcd_device = {ET8861};
#elif defined LCD_TY1622
const APP_LCD_DEVICE app_lcd_device = {LCD_DEV_TY1622};
#elif defined LCD_ET8862
const APP_LCD_DEVICE app_lcd_device = {LCD_DEV_ET8862};
#elif defined LCD_SR80I
const APP_LCD_DEVICE app_lcd_device = {SR80I};

#elif defined LCD_ST7032
#include "LCD_ST7032.h"
const APP_LCD_DEVICE app_lcd_device = {ST7032};

#elif defined OLED_ELW1301
#include "oled_elw1301.h"
const APP_LCD_DEVICE app_lcd_device = {ELW1301};

#else
const APP_LCD_DEVICE app_lcd_device = {LCD_NULL_DEVICE};
#endif
