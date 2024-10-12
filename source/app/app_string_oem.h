#ifndef __APP_STRING_OEM_H__
#define __APP_STRING_OEM_H__

#ifdef DISPLAY_MODULE_ENABLE
#ifdef LCD_MODULE_ENABLE
extern const U8 STRING_NULL[];
extern const U8 STRING_SOURCE_MODE_INTERNAL[];
extern const U8 STRING_SOURCE_MODE_AUX[];
extern const U8 STRING_SOURCE_MODE_USB_MSC[];
extern const U8 STRING_SOURCE_MODE_USB_SPEAKER[];
extern const U8 STRING_SOURCE_MODE_BT[];
extern const U8 STRING_SOURCE_MODE_OPTICAL[];
extern const U8 STRING_SOURCE_MODE_COAX[];
extern const U8 STRING_SOURCE_MODE_ARC[];
extern const U8 STRING_SOURCE_MODE_HDMI[];
extern const U8 STRING_SOURCE_MODE_HDMI_ARC[];

extern const U8 STRING_BT_MODE_INIT[];
extern const U8 STRING_BT_MODE_OPEN[];
extern const U8 STRING_BT_MODE_CONNECTED[];
extern const U8 STRING_BT_MODE_DISCONNECTED[];
extern const U8 STRING_BT_MODE_PAIRING[];
extern const U8 STRING_BT_MODE_NO_PAIRED[];

extern const U8 STRING_BT_MODE_RELINK[];

extern const U8 STRING_BT_MODE_PLAYING[];
extern const U8 STRING_BT_MODE_PAUSE[];
extern const U8 STRING_BT_MODE_STOPPED[];

extern const U8 STRING_USB_MSC_MODE_INIT[];
extern const U8 STRING_USB_MSC_MODE_OPEN[];
extern const U8 STRING_USB_MSC_MODE_INFO[];
extern const U8 STRING_USB_MSC_MODE_READING[];
extern const U8 STRING_USB_MSC_MODE_PLAY[];
extern const U8 STRING_USB_MSC_MODE_PAUSE[];
extern const U8 STRING_USB_MSC_MODE_STOP[];
extern const U8 STRING_USB_MSC_MODE_PLAY[];

extern const U8 STRING_ARC_MODE_INIT[];
extern const U8 STRING_ARC_MODE_OPEN[];
extern const U8 STRING_ARC_MODE_CEC_CONNECTED[];
extern const U8 STRING_ARC_MODE_CEC_DISCONNECTED[];

extern const U8 STRING_DECODE_MODE_DD_AC3[];
extern const U8 STRING_DECODE_MODE_DD_AC3_2[];
extern const U8 STRING_DECODE_MODE_DD_AC3_5[];
extern const U8 STRING_DECODE_MODE_DDp_EC3[];
extern const U8 STRING_DECODE_MODE_DTS[];
extern const U8 STRING_DECODE_MODE_PCM[];

extern const U8 STRING_STANDBY_MODE_ON[];
extern const U8 STRING_POWER_ON[];

extern const U8 STRING_MAX_VOLUME[];
extern const U8 STRING_MIN_VOLUME[];
#endif
#endif

#ifdef UPGRADE_ENABLE
extern const U8 STRING_USB_UPGRADE_ALL[];
extern const U8 STRING_USB_UPGRADE_PARTIAL[];
extern const U8 STRING_SD_UPGRADE_ALL[];
extern const U8 STRING_SD_UPGRADE_PARTIAL[];
extern const U8 STRING_BT_UPGRADE_ALL[];
extern const U8 STRING_BT_UPGRADE_PARTIAL[];

extern const U8 STRING_UPGRADE_SUCCESS[];
extern const U8 STRING_UPGRADE_ERROR[];

extern const U8 STRING_UPGRADE[];
extern const U8 STRING_UPGRADE_Y_N[];
extern const U8 STRING_UPGRADE_YES_NO [];
extern const U8 STRING_PLEASE_USE[];
extern const U8 STRING_UPGRADE_DISC[];

extern const U8 STRING_ERASE_FLASH[];
extern const U8 STRING_WRITE_FALSH[];
extern const U8 STRING_FLASH_PROG_ERROR[];
extern const U8 STRING_FLASH_PROG_SUCCESS[];
#endif //

#endif //__APP_STRING_OEM_H__
