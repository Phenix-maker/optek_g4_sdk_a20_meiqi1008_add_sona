#ifndef	_LCD_ST7032_H_
#define	_LCD_ST7032_H_

#ifdef LCD_ST7032
//#define LCD_TEST
#define	MAX_LCD_MODULE_POS			16
#define MAX_LCD_MODULE_LINE			2

typedef	enum 
{
	LINE1	= 0,
	LINE2,
	LINE3,
	LINE4,
	LINE12,
	LINE123,
	LINE34,
	ICON,
	SCREEN
}	LCD_RANGE;


typedef enum
{
	ICON_HOLD,
	ICON_CARD,
	ICON_BATTERY,

	ICON_CD,
	ICON_DISC,
	ICON_DISC1,
	ICON_DISC2,
	ICON_DISC3,
	ICON_NO_DISC,
	ICON_NO_DISC1,

	ICON_MODE,
	ICON_A,
	ICON_B,
	ICON_AB,
	ICON_REPEAT,
	ICON_REPEAT_ONE,
	ICON_REPEAT_ALL,
	ICON_SHUFFLE,
	ICON_DIR,

	ICON_RESUME,
	ICON_INTRO,

	ICON_TRACK,
	ICON_FILE,
	ICON_FOLDER,
	ICON_TIMER,
	ICON_TIMER1,
	ICON_TIMER2,
	ICON_PROG,
	ICON_ESP,

	ICON_EQ,
	ICON_BASS,
	ICON_XBASS,
	ICON_DBB,
	ICON_DSC,
	ICON_MP3,
	ICON_WMA,

	//ID3
	ICON_ARTIST,
	ICON_TITLE,
	ICON_ALBUM,
	ICON_YEAR,
	ICON_GENRE,

	ICON_SELECT,

	ICON_PROG_ON,
	ICON_PROG_OFF,
	ICON_PROG_EDIT,
	ICON_PROG_CLR,
	ICON_PROG_DEL,
	ICON_POWERON_HOLD,
	ICON_POWER_OFF,
	ICON_BEEP_ON,
	ICON_BEEP_OFF,

	ICON_PLAY,
	ICON_PAUSE,
	ICON_REC,
	ICON_RECX2,
	ICON_REC_FLASH,
	ICON_REC_TO_USB,
	ICON_REC_TO_USB1,
	ICON_REC_SD,

	ICON_DELETE,

	ICON_TR_CHARACTER,
	ICON_ALB_CHARACTER,
	ICON_ALL_CHARACTER,
	ICON_ONE_CHARACTER,
	ICON_OCD_CHARACTER,

	ICON_S1S2S3S4,
	ICON_S0S1S2S3,
	ICON_CLEARS1S2S3S4,
	ICON_CLK_COLON,
	ICON_COLON,
	ICON_COLON1,
	ICON_COLON2,
	ICON_DOT,
	ICON_DOT1,
	ICON_DOT2,
	ICON_FM_DOT,
	ICON_FOLDER_DOT,
      
	ICON_USB,
	ICON_SD,

	ICON_TUNER,
	ICON_TUNEBAND,
	ICON_FM,
	ICON_MW,
	ICON_MHZ,
	ICON_KHZ,
	ICON_AM,
	ICON_PM,

	ICON_SLEEP,
	ICON_SNOOZE,
	ICON_NAP,

	ICON_MUTE,
	ICON_MAX,

	ICON_TAPE,
	ICON_TAPEREC,

	ICON_AUX,
	ICON_DOCK,

	ICON_DIM,

	ICON_RDS,
	ICON_NEWS,
	ICON_L0,
	ICON_L1,
	ICON_L2,
	ICON_L3,
	ICON_L4,
	ICON_L5,
	ICON_L6,
	ICON_L7,
	ICON_L8,
	ICON_L9,
	ICON_L10,
	ICON_L11,
	ICON_L12,
	ICON_RMTRECEIVE,
	ICON_STEREO,
	ICON_ST_ON,

	ICON_PHONE,
	ICON_BLUETOOTH,
	ICON_DAB,
	ICON_IPOD,
	ICON_DVD,

	ICON_LOUND,
	ICON_INT,

	ICON_R1_TO_R4,
	ICON_R1,
	ICON_R2,
	ICON_R3,
	ICON_R4,
	ICON_PRESET_NUMBER,

	ICON_L_CLR,
	ICON_L1_TO_L2,
	ICON_L1_TO_L3,
	ICON_L1_TO_L4,
	ICON_L1_TO_L5,
	ICON_L1_TO_L6,
	ICON_L1_TO_L7,

	ICON_DAILY,
	ICON_AUTO,
	ICON_CHAP,

	ICON_MENU,
	ICON_MOVIE,
	ICON_TOTAL,

	//for DVD
	ICON_CD_DISC,
	ICON_PLII,
	ICON_ALL,
	ICON_ONE,
	ICON_VCD,

	ICON_DTS,
	ICON_DIVX,
	ICON_REP_A,
	ICON_REP_B,
	ICON_CHAPTER,

	ICON_DVS,
	ICON_FOLD,
	ICON_1_OF_RPT,
	ICON_1_FOLD, // display ICON_1 and ICON_FOLD

	ICON_DBDIGITAL,
	ICON_LII,
	ICON_AAC,
	ICON_AUTO_SCAN,
	ICON_TUNED,
	ICON_PTYI,
	ICON_TP,
	ICON_TA,

   	ICON_IPOD2,
   	ICON_RADIO1,
   	ICON_RADIO2,
	ICON_BUZZER1,
	ICON_BUZZER2,

	ICON_5,

	ICON_SUNDAY,
	ICON_MONDAY,
	ICON_TUESDAY,
	ICON_WEDNESDAY,
	ICON_THURSDAY,
	ICON_FRIDAY,
	ICON_SATURDAY,
	ICON_WEEKEND,

	ICON_LAST
}	LCD_ICON;

#define LCD_RS_PIN_INDEX        GPIO0_09_INDEX
#define LCD_RS_PIN_INIT      	hal_gpio_mux_func_select(LCD_RS_PIN_INDEX, MUX_SEL_GPIO_OUTPUT)
#define LCD_RS_PIN_HIGH    		hal_gpio_output_level_set(LCD_RS_PIN_INDEX, GPIO_HIGH)
#define LCD_RS_PIN_LOW    		hal_gpio_output_level_set(LCD_RS_PIN_INDEX, GPIO_LOW)


#define LCD_RESET_PIN_INDEX     GPIO_UNDEFINE_INDEX
#define LCD_RESET_PIN_INIT      hal_gpio_mux_func_select(LCD_RESET_PIN_INDEX, MUX_SEL_GPIO_OUTPUT)
#define LCD_RESET_PIN_HIGH    	hal_gpio_output_level_set(LCD_RESET_PIN_INDEX, GPIO_HIGH)
#define LCD_RESET_PIN_LOW    	hal_gpio_output_level_set(LCD_RESET_PIN_INDEX, GPIO_LOW)


void ST7032_init(void);
void ST7032_open(void);
void ST7032_close(void);
void ST7032_fini(void);
void ST7032_clear(LCD_RANGE lcd_range);
void ST7032_display_icon(LCD_ICON, U32);
void ST7032_display_char(const U8 *buf, U16 line, U16 pos);
void ST7032_display_char_fixed_len(const U8 *buf, U16 line, U16 spos, U16 epos);
void ST7032_display_dim_set(U8 val);
void ST7032_display_full(void);
void ST7032_display_clear_icon(void);
void ST7032_display_icon_partial(void);
void ST7032_display_led_light_set(U8 num, U8 con);
void ST7032_display_level(U16 Level_L, U16 Level_R,U8 index);
void ST7032_display_source(U8 audio_source);
void ST7032_display_music_play(U32 music_level);
void lcd_test(void);

#define ST7032		ST7032_init,	\
					ST7032_open,	\
					ST7032_close, \
					ST7032_fini,	\
					ST7032_clear, \
					ST7032_display_icon,	\
					ST7032_display_char, \
					ST7032_display_char_fixed_len, \
					ST7032_display_dim_set, \
					ST7032_display_full, \
					ST7032_display_clear_icon, \
					ST7032_display_icon_partial, \
					ST7032_display_led_light_set, \
					ST7032_display_level, \
					ST7032_display_source, \
					ST7032_display_music_play

void Display_Strings0(U8 *buf, U8 *pDisp);
void Display_Strings1(U8 *buf, U8 *pDisp);



void lcd_test(void);
#endif
#endif //_LCD_ST7032_H_
