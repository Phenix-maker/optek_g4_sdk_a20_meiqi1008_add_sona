#ifndef __OLED_ELW1301_H__
#define __OLED_ELW1301_H__

//A0 pin
#define OLED_A0_PIN_INDEX       GPIO0_20_INDEX
#define OLED_A0_PIN_INIT        hal_gpio_mux_func_select(OLED_A0_PIN_INDEX, MUX_SEL_GPIO_OUTPUT)
#define OLED_A0_PIN_HIGH        hal_gpio_output_level_set(OLED_A0_PIN_INDEX, GPIO_HIGH)
#define OLED_A0_PIN_LOW         hal_gpio_output_level_set(OLED_A0_PIN_INDEX, GPIO_LOW)

//RS pin
#define OLED_RES_PIN_INDEX      GPIO0_21_INDEX
#define OLED_RES_PIN_INIT       hal_gpio_mux_func_select(OLED_RES_PIN_INDEX, MUX_SEL_GPIO_OUTPUT)
#define OLED_RES_PIN_HIGH       hal_gpio_output_level_set(OLED_RES_PIN_INDEX, GPIO_HIGH)
#define OLED_RES_PIN_LOW        hal_gpio_output_level_set(OLED_RES_PIN_INDEX, GPIO_LOW)

//power pin
#define OLED_POWER_PIN_INDEX    GPIO0_10_INDEX
#define OLED_POWER_PIN_INIT     hal_gpio_mux_func_select(OLED_POWER_PIN_INDEX, MUX_SEL_GPIO_OUTPUT)
#define OLED_POWER_PIN_HIGH     hal_gpio_output_level_set(OLED_POWER_PIN_INDEX, GPIO_HIGH)
#define OLED_POWER_PIN_LOW      hal_gpio_output_level_set(OLED_POWER_PIN_INDEX, GPIO_LOW)

//device support pos addr 0~0x0F;line addr 0~39
#define	MAX_OLED_POS			5*3
#define MAX_OLED_LINE			1


void Oled_Rs_Write(U16 Logic);

void OLED_ELW1301_init(void);
void OLED_ELW1301_open(void);
void OLED_ELW1301_close(void);
void OLED_ELW1301_fini(void);
void OLED_ELW1301_clear (LCD_RANGE lcd_range);
void OLED_ELW1301_display_icon (LCD_ICON, U32);
void OLED_ELW1301_display_char (const U8 *, U16, U16);
void OLED_ELW1301_display_char_fixed_len (const U8 *buf, U16 line, U16 spos, U16 epos);
void OLED_ELW1301_display_full(void);
void OLED_ELW1301_scroll_string(const U8 *buf, U16 line, U16 pos, SCROLL_MODE mode, U8 wait_sec);
void OLED_ELW1301_disp_scroll(void);
void OLED_ELW1301_disp_vol(U8 vol);
void OLED_ELW1301_disp_source(U8 audio_source);
void OLED_ELW1301_disp_music_play(U32 music_level);

#define ELW1301		OLED_ELW1301_init,	\
					OLED_ELW1301_open,	\
					OLED_ELW1301_close, \
					OLED_ELW1301_fini,	\
					OLED_ELW1301_clear, \
					OLED_ELW1301_display_icon,	\
					OLED_ELW1301_display_char, \
					OLED_ELW1301_display_char_fixed_len, \
					NULL,\
					OLED_ELW1301_display_full,\
					NULL,\
					NULL,\
					NULL,\
					NULL,\
					OLED_ELW1301_scroll_string,\
					OLED_ELW1301_disp_scroll,\
					OLED_ELW1301_disp_vol,\
					OLED_ELW1301_disp_source,\
					OLED_ELW1301_disp_music_play

#endif /* __OLED_ELW1301_H__ */
