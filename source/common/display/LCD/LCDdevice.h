#ifndef	_LCD_DEVICE_H_
#define	_LCD_DEVICE_H_


#define PRESET_NUMER_CLR		0xFFFF

#if defined LCD_M66005
#include "..\ui\oem\LCD_M66005.h"
#elif defined LCD_PT6302
#include "..\ui\oem\LCD_PT6302.h"
#elif defined LCD_ML9208A
#include "..\ui\oem\LCD_ML9208A.h"
#elif defined LCD_TY1621
#include "..\ui\oem\LCD_TY1621.h"
#elif defined LCD_ET8861
#include "..\ui\oem\LCD_ET8861.h"
#elif defined LCD_TY1622
#include "..\ui\oem\LCD_TY1622.h"
#elif defined LCD_ET8862
#include "..\ui\oem\LCD_ET8862.h"
#elif defined LCD_SR80I
#include "..\ui\oem\LCD_SR80I.h"
#elif defined LCD_ST7032
#include "LCD_ST7032.h"
#else
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

#define	MAX_LCD_MODULE_POS			14
#define MAX_LCD_MODULE_LINE			1
#endif

#ifdef OLED_ELW1301
typedef enum {
	SCROLL_NONE,
	SCROLL_ONCE,
	SCROLL_TWICE,
	SCROLL_FOREVER,
	SCROLL_RDS_TEXT,
	SCROLL_DAB_DLS_TEXT,

	SCROLL_MODE_LAST
} SCROLL_MODE;


typedef struct {
	U8 line;
	U8 *buf;
	U8 buf_index;
	U8 mode;
	U8 count;
	U8 start_pos;
//	U8 end;

	U16 wait_ms;
} LCD_SCROLL;
#endif
typedef struct _APP_LCD_DEVICE {
	void		(*init) (void);
	void		(*open) (void);
	void		(*close) (void);
	void		(*fini) (void);
	void		(*clear) (LCD_RANGE lcd_range);
	void		(*display_icon) (LCD_ICON parm1, U32 parm2);
	void		(*display_char) (const U8 *buf, U16 line, U16 pos);
	void		(*display_char_fix_len) (const U8 *buf, U16 line, U16 spos, U16 epos);
	void		(*display_dim_set) (U8 val);
	void		(*display_full) (U8 con);
	void		(*clear_icon) (void);
	void		(*clear_icon_partial) (void);
	void 		(*led_light_set) (U8 num, U8 con);
	void 		(*display_level) (U16 Level_L, U16 Level_R, U8 index);
	void 		(*display_source) (U8 audio_source);
	void		(*display_music_play) (U32 music_level);
} APP_LCD_DEVICE;

extern const APP_LCD_DEVICE app_lcd_device;


void lcd_none_void(void);
void lcd_none_clear(LCD_RANGE lcd_range);
void lcd_none_display_icon(LCD_ICON parm1, U32 parm2);
void lcd_none_display_char(const U8 *buf, U16 line, U16 pos);
void lcd_none_display_char_fix_len(const U8 *buf, U16 line, U16 spos, U16 epos);
void lcd_none_display_dim_set(U8 val);
void lcd_none_led_light_set(U8 num, U8 con);
void lcd_none_display_level(U16 Level_L, U16 Level_R,U8 index);
void lcd_none_display_source(U8 audio_source);
void lcd_none_display_music_play(U32 music_level);


#define LCD_NULL_DEVICE 	lcd_none_void,\
							lcd_none_void,\
							lcd_none_void,\
							lcd_none_void,\
							lcd_none_clear,\
							lcd_none_display_icon,\
							lcd_none_display_char,\
							lcd_none_display_char_fix_len,\
							lcd_none_display_dim_set,\
							lcd_none_void,\
							lcd_none_void,\
							lcd_none_led_light_set,\
							lcd_none_display_level,\
							lcd_none_display_source,\
							lcd_none_display_music_play

//for disp string start position
#define LCD_DISP_START_0_POS		0
#define LCD_DISP_START_1_POS		1
#define LCD_DISP_START_2_POS		2
#define LCD_DISP_START_3_POS		3
#define LCD_DISP_START_4_POS		4
#define LCD_DISP_START_5_POS		5
#define LCD_DISP_START_6_POS		6

#define LCD_DISP_BUF_SIZE			18

#endif //_LCD_DEVICE_H_
