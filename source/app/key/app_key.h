#ifndef _APP_KEY_
#define _APP_KEY_
#include "c_def.h"
typedef enum {
	APP_KEY_INVALID				= -1,
	APP_KEY_PLAY                = 0,
	APP_KEY_STOP,
	APP_KEY_SKIPF,
	APP_KEY_SKIPB,
	APP_KEY_PLUS,				//APP_KEY_SEARCH_FORWARD
	APP_KEY_MINUS,				//APP_KEY_SEARCH_BACKWARD
	APP_KEY_FOLDER_F,
	APP_KEY_FOLDER_B,
	APP_KEY_PROGRAM,
	APP_KEY_SD,                 //APP_KEY_OPEN,

	//11
	APP_KEY_CLOSE = 10,
	APP_KEY_BLUETOOTH = 10,
	APP_KEY_EQ,
	APP_KEY_MODE = 12,
	APP_KEY_REC_MODE = 12,
	APP_KEY_REPEAT,
	APP_KEY_SHUFFLE,
	APP_KEY_OPEN_CLOSE,			//for tray load and multi disc
	APP_KEY_FOLDER,				//APP_KEY_DISC_1,
	APP_KEY_ALARM1,				//APP_KEY_DISC_2,
	APP_KEY_ALARM2,				//APP_KEY_DISC_3,
	APP_KEY_DISC_CHANGE = 19,
	APP_KEY_RESUME = 19,

	//21
	APP_KEY_SOURCE,
	APP_KEY_CD,
	APP_KEY_USB,
	APP_KEY_TUNER,
	APP_KEY_AUX,
	APP_KEY_IPHONE = 25,
	APP_KEY_AHB = 25,
	APP_KEY_REC,
	APP_KEY_REC_USB1 = 26,
	APP_KEY_REC_USB2 = 26,
	APP_KEY_REC_SD1 = 26,
	APP_KEY_DEL = 27,
	APP_KEY_CLEAR = 27,
	APP_KEY_MUTE,
	APP_KEY_SLEEP,

	//31
	APP_KEY_POWER,
	APP_KEY_DBB,
	APP_KEY_CLOCK,
	APP_KEY_TIMER,
	APP_KEY_DISP = 34,
	APP_KEY_RANDOM = 34,
	APP_KEY_MAX = 35,
	APP_KEY_XBASS = 35,
	APP_KEY_DIM = 36,
	APP_KEY_DIM_ECO = 36,

	APP_KEY_ASPM = 37, 
	APP_KEY_RDS = 38,
	APP_KEY_K2 = 38,
	APP_KEY_MIX_PLAY_SOURCE = 38,
	APP_KEY_DUMMY1 = 39,
	APP_KEY_MIX_PLAY_SOURCE_STOP = 39,

	//41
	APP_KEY_BASS_TREBLE,
	APP_KEY_TREBLE = 41,
	APP_KEY_TVOUT = 41,
	APP_KEY_PAIRING = 41,
	APP_KEY_DIGIT,			   //APP_KEY_CLK_VIEW
	APP_KEY_ENTER = 43,
	APP_KEY_SELECT = 43,
	APP_KEY_MENU = 42,         //BT MENU


	APP_KEY_TIMER_ON_OFF,
	APP_KEY_PTY = 45,
	APP_KEY_DUMMY = 46,
	APP_KEY_SCAN = 46,			//for DAB scan
	APP_KEY_SPEED_PLUS = 46,	//Speed +
	APP_KEY_SPEED_MINUS = 47,	//Speed -
	APP_KEY_SERVICE = 47,

	APP_KEY_END
} APP_KEY;

/*
 * main board key structure definition
 *****************************************************************************************
 */

#define ADC_INVALID_VAL     0x3D0
#define ADC_INVALID_VAL2    0x3E0
#define ADC_KEY_VAL_MAX     0x3FF

typedef struct {
	U16	ad_min;
	U16 ad_max;
	APP_KEY key;
} KEY_AD_ENTRY;

typedef struct {
	U32	gpio_index;	
	U32	active_lever;
	APP_KEY key;
} KEY_GPIO_ENTRY;

typedef struct {
    KEY_GPIO_ENTRY *key_gpio_entry;
	U16 entries;
} KEY_GPIO_MAP;

typedef struct {
	APP_KEY	key_id1;
	APP_KEY	key_id2;
	APP_KEY mix_key;
} KEY_MIX_ENTRY;

typedef struct {
    KEY_MIX_ENTRY *key_mix_entry;
	U16 entries;
} KEY_MIX_MAP;

/*
 * remote key structure definition
 *****************************************************************************************
 */
typedef struct {
	APP_KEY	key_id;
	U32	rmtkey_val;
} KEY_REMOTE_ENTRY;

typedef struct {
    KEY_REMOTE_ENTRY *key_remote_entry;
	U16 entries;
} KEY_REMOTE_MAP;

APP_KEY vRemoteMapMainKey(U32 rmtkey_code);

void app_gpio_key_init(void);
/**
 ****************************************************************************************
 * @brief Convert gpio pin status to app key id
 *
 * @param[in] gpio_group   group that gpios in
 * @param[in] gpio_status  gpios of this group's status
 * @param[in] app_keys     buffer to store output keyids of the keys is/are down
 * @param[in] max_simult   maximum simultaneous keys
 *
 * @return The number key in down status
 *
 ****************************************************************************************
 */
U8 vGpioMapMainKey(U32 gpio_group, U32 gpio_status, APP_KEY *app_keys, U8 max_simult);
APP_KEY vMainKeyMix(APP_KEY *app_keys, U8 key_cnt);

U8 vGpioMapMainSregKey(U32 gpio_group, U32 gpio_status, APP_KEY *app_keys, U8 max_simult);

APP_KEY vAdMapMainKey (U8 ad_channel, U16 ad_val);

#endif //_APP_KEY_
