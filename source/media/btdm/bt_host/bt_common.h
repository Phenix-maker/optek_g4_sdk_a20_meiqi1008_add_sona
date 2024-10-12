#ifndef _BT_COMMON_H_
#define _BT_COMMON_H_

typedef enum
{
    enBT_CLOSED,//default(power on status)
    enBT_OPENING,
	enBT_OPENED,

	enBT_CONNECTED,
	enBT_DISCONNECTED,
	enBT_PAIRING,
	enBT_RELINK,
	enBT_UPGRAGE,

	enBT_ERROR,
	enBT_UNKNOWN = 0xFF
} enBT_MAIN_STATUS;

typedef enum {
    enBT_BOTH_SCAN_DISABLE,          // non-connectable and non-discaverable
    enBT_INQUIRY_SCAN_ENABLE,       //discaveralbe 
    enBT_PAGE_SCAN_ENABLE,         //connectable
    enBT_BOTH_SCAN_ENABLE,        //discaverable and connectable(in pairing status)

	enBT_SCAN_UNKNOWN = 0xFF
}enBT_SCAN_MODE;


/* a2dp status in bt host lib
typedef enum {
	AVDTP_STATE_IDLE,
	AVDTP_STATE_CONFIGURED,
	AVDTP_STATE_OPEN,
	AVDTP_STATE_STREAMING,
	AVDTP_STATE_CLOSING,
	AVDTP_STATE_ABORTING,
} avdtp_state_t;
*/

typedef enum {
	enBT_A2DP_IDLE,
	enBT_A2DP_CONFIGURED,
	enBT_A2DP_OPEN,
	enBT_A2DP_STREAMING, //playing
	enBT_A2DP_CLOSING,
    enBT_A2DP_ABORTING,
	enBT_A2DP_UNKNOWN = 0xFF
} enBT_A2DP_STATUS;



/* play status */
/* avrcp status in bt host lib 
#define AVRCP_PLAY_STATUS_STOPPED	0x00
#define AVRCP_PLAY_STATUS_PLAYING	0x01
#define AVRCP_PLAY_STATUS_PAUSED	0x02
#define AVRCP_PLAY_STATUS_FWD_SEEK	0x03
#define AVRCP_PLAY_STATUS_REV_SEEK	0x04
#define AVRCP_PLAY_STATUS_ERROR		0xFF
#define AVRCP_PLAY_STATUS_UNKNOWN	0xFE
*/

typedef enum {
	enBT_AVRCP_STOPPED,
	enBT_AVRCP_PLAYING,
	enBT_AVRCP_PAUSED,
    enBT_AVRCP_FWD_SEEK,
    enBT_AVRCP_REV_SEEK,
    enBT_AVRCP_REV_ERROR = 0XFF,
	enBT_AVRCP_UNKNOWN = 0xFE
}enBT_AVRCP_STATUS;


typedef struct {
	enBT_MAIN_STATUS bt_main_status;
	enBT_SCAN_MODE   bt_scan_mode;
	enBT_A2DP_STATUS bt_a2dp_status;
	enBT_AVRCP_STATUS bt_avrcp_status;
} BT_STATUS_s;

extern BT_STATUS_s bt_status;


void app_bt_set_scan_enable_mode(U8 scan_mode);

void app_nav_BT_manual_pair(void);
U8 bt_pairing_info_not_none(void);
void app_nav_bt_relink(void);

#ifdef BT_VOLUME_SET
void app_nav_bt_avrcp_vol(void);
#endif

#ifdef BT_VOLUME_SYC
void app_cmd_bt_absolute_vol(U8 vol);
#endif

#endif // _BT_COMMON_H_