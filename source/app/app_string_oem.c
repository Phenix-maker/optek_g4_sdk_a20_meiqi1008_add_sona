#include "c_def.h"
#include "oem.h"

#include "app_string_oem.h"


#ifdef DISPLAY_MODULE_ENABLE
#ifdef LCD_MODULE_ENABLE
const U8 STRING_NULL[]                      = {"                    \0\0"};

const U8 STRING_SOURCE_MODE_INTERNAL[]      = {"Internal Audio\0\0"};
const U8 STRING_SOURCE_MODE_AUX[]           = {"AUX Mode\0\0"};
const U8 STRING_SOURCE_MODE_USB_MSC[]       = {"USB MSC Mode\0\0"};
const U8 STRING_SOURCE_MODE_USB_SPEAKER[]   = {"USB Speaker Mode\0\0"};
const U8 STRING_SOURCE_MODE_BT[]            = {"BlueThooth Mode\0\0"};
const U8 STRING_SOURCE_MODE_OPTICAL[]       = {"Optical Mode\0\0"};
const U8 STRING_SOURCE_MODE_COAX[]          = {"Coaxial Mode\0\0"};
const U8 STRING_SOURCE_MODE_ARC[]           = {"ARC Mode\0\0"};
const U8 STRING_SOURCE_MODE_HDMI[]          = {"HDMI Mode\0\0"};
const U8 STRING_SOURCE_MODE_HDMI_ARC[]      = {"HDMI ARC Mode\0\0"};

const U8 STRING_BT_MODE_INIT[]              = {"BT Init\0\0"};
const U8 STRING_BT_MODE_OPEN[]              = {"BT Open\0\0"};
const U8 STRING_BT_MODE_CONNECTED[]         = {"BT Connected\0\0"};
const U8 STRING_BT_MODE_DISCONNECTED[]      = {"BT disconnected\0\0"};
const U8 STRING_BT_MODE_PAIRING[]           = {"BT Pairing\0\0"};
const U8 STRING_BT_MODE_NO_PAIRED[]         = {"BT NO Paired\0\0"};

const U8 STRING_BT_MODE_RELINK[]           = {"BT relink\0\0"};

const U8 STRING_BT_MODE_PLAYING[]           = {"BT Playing\0\0"};
const U8 STRING_BT_MODE_PAUSE[]             = {"BT Pause\0\0"};
const U8 STRING_BT_MODE_STOPPED[]           = {"BT Stopped\0\0"};

const U8 STRING_USB_MSC_MODE_INIT[]         = {"USB Init\0\0"};
const U8 STRING_USB_MSC_MODE_OPEN[]         = {"USB Open\0\0"};
const U8 STRING_USB_MSC_MODE_INFO[]         = {"USB Info\0\0"};
const U8 STRING_USB_MSC_MODE_READING[]      = {"USB Reading\0\0"};
const U8 STRING_USB_MSC_MODE_PLAY[]         = {"USB Playing\0\0"};
const U8 STRING_USB_MSC_MODE_PAUSE[]        = {"USB Pause\0\0"};
const U8 STRING_USB_MSC_MODE_STOP[]         = {"USB Stop\0\0"};

const U8 STRING_ARC_MODE_INIT[]             = {"ARC Init\0\0"};
const U8 STRING_ARC_MODE_OPEN[]             = {"ARC Open\0\0"};
const U8 STRING_ARC_MODE_CEC_CONNECTED[]    = {"CEC CONNECTED\0\0"};
const U8 STRING_ARC_MODE_CEC_DISCONNECTED[] = {"CEC DISCONNECTED\0\0"};

const U8 STRING_DECODE_MODE_DD_AC3[]        = {"DD AC3 Test\0\0"};
const U8 STRING_DECODE_MODE_DD_AC3_2[]        = {"DD AC3 2.0 Test\0\0"};
const U8 STRING_DECODE_MODE_DD_AC3_5[]        = {"DD AC3 5.1 Test\0\0"};
const U8 STRING_DECODE_MODE_DDp_EC3[]       = {"DD+ EC3 Test\0\0"};
const U8 STRING_DECODE_MODE_DTS[]           = {"DTS Test\0\0"};
const U8 STRING_DECODE_MODE_PCM[]           = {"PCM Test\0\0"};

const U8 STRING_STANDBY_MODE_ON[]           = {"Standby Mode\0\0"};
const U8 STRING_POWER_ON[]                  = {"Power On\0\0"};

const U8 STRING_MAX_VOLUME[]                = {"MAX Vol\0\0"};
const U8 STRING_MIN_VOLUME[]                = {"MIN Vol\0\0"};
#endif
#endif




#if (defined USB_SPEAKER && defined USB_DEV_ENABLE)
const U8 STRING_USB_SPEAKER_APP[] = {"PC APP"}; //for PC IN
#endif

#ifdef UPGRADE_PROJECT_ENABLE
const U16 upgrade_bin[] = {'O','p','t','e','k','_','U','S','B','_','S','D','K','_','u','p','g','r','a','d','e','.','b','i','n',0x0};
#endif




#ifdef BT_HCI_ENABLE

#ifndef BOOT_FROM_FLASH
const u8 bt_name[BT_NAME_STRING_MAX_LEN] = {"OPTEK BT SWilson\0\0"};           //max len is 64
const u8 bt_init_name[BT_NAME_STRING_MAX_LEN] = {"OPTEK BT SWilson Init\0\0"}; //max len is 64
#else
const u8 bt_name[BT_NAME_STRING_MAX_LEN] = {"OPTEK BT D7\0\0"};           //max len is 64
const u8 bt_init_name[BT_NAME_STRING_MAX_LEN] = {"OPTEK BT D7 Init\0\0"}; //max len is 64
#endif

const u8 bt_upgrade_file_name[BT_NAME_STRING_MAX_LEN] = {"BlueSea_BT_SDK_upgrade.bin\0\0"}; //max len is 64

#endif //BT_HCI_ENABLE



#ifdef UPGRADE_ENABLE

#ifdef RODATA_RELOCATION
const U8 STRING_USB_UPGRADE_ALL[] __attribute__((section(".upgrade_rodata"))) = {"UPGUSBEFA"};
const U8 STRING_USB_UPGRADE_PARTIAL[] __attribute__((section(".upgrade_rodata"))) = {"UPGUSBEFP"};
const U8 STRING_SD_UPGRADE_ALL[] __attribute__((section(".upgrade_rodata"))) = {"UPG SDEFA"};
const U8 STRING_SD_UPGRADE_PARTIAL[] __attribute__((section(".upgrade_rodata"))) = {"UPG SDBEFP"};
const U8 STRING_UPGRADE_SUCCESS[] __attribute__((section(".upgrade_rodata"))) = {"UPGSuc"};
const U8 STRING_UPGRADE_ERROR[] __attribute__((section(".upgrade_rodata"))) = {"UPGErr"};

const U8 STRING_UPGRADE[] __attribute__((section(".upgrade_rodata"))) = {"Upgrade"};
const U8 STRING_UPGRADE_Y_N[] __attribute__((section(".upgrade_rodata"))) = {"Upgrade???"};
const U8 STRING_PLEASE_USE[] __attribute__((section(".upgrade_rodata"))) = {"Please Use"};
const U8 STRING_UPGRADE_DISC[] __attribute__((section(".upgrade_rodata"))) = {"Upgrade Disc"};

const U8 STRING_ERASE_FLASH[] __attribute__((section(".upgrade_rodata"))) = {"Erase"};
const U8 STRING_WRITE_FALSH[] __attribute__((section(".upgrade_rodata"))) = {"Program"};
const U8 STRING_FLASH_PROG_ERROR[] __attribute__((section(".upgrade_rodata"))) = {"Error"};
const U8 STRING_FLASH_PROG_SUCCESS[] __attribute__((section(".upgrade_rodata"))) = {"Success"};
#else
const U8 STRING_USB_UPGRADE_ALL[] = {"UPGUSBEFA"};
const U8 STRING_USB_UPGRADE_PARTIAL[] = {"UPGUSBEFP"};
const U8 STRING_SD_UPGRADE_ALL[] = {"UPG SDEFA"};
const U8 STRING_SD_UPGRADE_PARTIAL[] = {"UPG SDBEFP"};
const U8 STRING_BT_UPGRADE_ALL[] = {"UPG BTEFA"};
const U8 STRING_BT_UPGRADE_PARTIAL[] = {"UPG BTEFP"};

const U8 STRING_UPGRADE_SUCCESS[] = {"UPGSuc"};
const U8 STRING_UPGRADE_ERROR[] = {"UPGErr"};

const U8 STRING_UPGRADE[] = {"Upgrade"};
const U8 STRING_UPGRADE_Y_N[] = {"Upgrade???"};
const U8 STRING_UPGRADE_YES_NO [] = {"Y(\1),N(\6)\0\0"};
const U8 STRING_PLEASE_USE[] = {"Please Use"};
const U8 STRING_UPGRADE_DISC[] = {"Upgrade Disc"};

const U8 STRING_ERASE_FLASH[] = {"Erase"};
const U8 STRING_WRITE_FALSH[] = {"Program"};
const U8 STRING_FLASH_PROG_ERROR[] = {"Error"};
const U8 STRING_FLASH_PROG_SUCCESS[] = {"Success"};
#endif

#else

#if 0
const U8 STRING_USB_UPGRADE_ALL[] __attribute__((section(".upgrade_rodata"))) = {"UPGUSBEFA"};
const U8 STRING_USB_UPGRADE_PARTIAL[] __attribute__((section(".upgrade_rodata"))) = {"UPGUSBEFP"};
//const U8 STRING_SD_UPGRADE_ALL[] __attribute__((section(".upgrade_rodata"))) = {"UPG SDEFA"};
//const U8 STRING_SD_UPGRADE_PARTIAL[] __attribute__((section(".upgrade_rodata"))) = {"UPG SDBEFP"};

const U8 STRING_UPGRADE[] __attribute__((section(".upgrade_rodata"))) = {"Upgrade"};
const U8 STRING_FLASH_PROG_ERROR[] __attribute__((section(".upgrade_rodata"))) = {"Error"};
const U8 STRING_FLASH_PROG_SUCCESS[] __attribute__((section(".upgrade_rodata"))) = {"Success"};
#else
const U8 STRING_USB_UPGRADE_ALL[]     = {"UPGUSBEFA 2M"};
const U8 STRING_USB_UPGRADE_PARTIAL[] = {"UPGUSBEFP 2M"};
//const U8 STRING_SD_UPGRADE_ALL[]      = {"UPG SDEFA 2M"};
//const U8 STRING_SD_UPGRADE_PARTIAL[]  = {"UPG SDEFP 2M"};

const U8 STRING_BT_UPGRADE_ALL[]      = {"UPG BTEFA 2M"};
const U8 STRING_BT_UPGRADE_PARTIAL[]  = {"UPG BTEFP 2M"};

const U8 STRING_UPGRADE[]            = {"Upgrade"};
const U8 STRING_FLASH_PROG_ERROR[]   = {"Error"};
const U8 STRING_FLASH_PROG_SUCCESS[] = {"Success"};
#endif

#endif //UPGRADE_ENABLE
