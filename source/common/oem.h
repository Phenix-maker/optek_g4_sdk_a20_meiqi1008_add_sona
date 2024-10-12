#ifndef __OEM_H__
#define __OEM_H__

#define SPI_FLASH_4BIT_ENABLE


/***********************************************
*******       AUDIO CLOCK SYN SET       ********
***********************************************/
// #define AUDIO_CLOCK_SYNC_ENABLE

#ifdef AUDIO_CLOCK_SYNC_ENABLE
#define LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE
// #define PLL_DIRFT_TEST
// #define IIS_IN_SYNC_W_MASTER
// #define AUDIO_CLOCK_SYNC_PRINTF
#endif

/***********************************************
*******        save module set          ********
***********************************************/
#define SAVE_MODULE_ENABLE

#ifdef SAVE_MODULE_ENABLE
//#define SAVE_MODULE_FROM_EEPROM
#define SAVE_MODULE_FROM_FLASH

#define SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
#ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
#define SAVE_GLOBAL_VARI_STRU_DELAY_TIME      1000
#endif

#ifdef SAVE_MODULE_FROM_EEPROM
#define I2C0_ENABLE

//#define EEPROM_AT24C01
//#define EEPROM_AT24C02
//#define EEPROM_AT24C04
#define EEPROM_AT24C08


#endif
#endif


/***********************************************
*******         AUDIO SOURCE SET        ********
***********************************************/
#define AUX_ENABLE
#define BTDM52_ENABLE
// #define HDMI_ENABLE
// #define OPTICAL_ENABLE
// #define COAXIAL_ENABLE
// #define ARC_ENABLE
// #define USB_HOST_MSC_ENABLE
// #define USB_SPEAKER_ENABLE

/***********************************************
*******         POWER ON/OFF SET        ********
***********************************************/
//#define STANDBY_SOURCE_ENABLE

#ifdef STANDBY_SOURCE_ENABLE
#define SYSTEM_POWER_ON_TO_STANDBY_MODE

#ifdef SYSTEM_POWER_ON_TO_STANDBY_MODE
#define STANDBY_MODE_DELAY_RELEASE

#ifdef STANDBY_MODE_DELAY_RELEASE
#define STANBY_RELEASE_DELAY_TIME_MS    1000 
#endif
#endif
#endif

/*
* HW SUPPORT SETTING
* hardware I2C support
* #define I2C0_ENABLE
* #define I2C1_ENABLE
* #define I2C2_ENABLE
* #define I2C3_ENABLE
* */

#ifdef HDMI_ENABLE
#define SPDIF_ENABLE
#define I2C0_ENABLE
#endif

#ifdef OPTICAL_ENABLE
#define SPDIF_ENABLE
#endif

#ifdef COAXIAL_ENABLE
#define SPDIF_ENABLE
#endif

#ifdef ARC_ENABLE
#define SPDIF_ENABLE
#define CEC_ENABLE
//#define CEC_CMD_RX_POLLING_ENABLE
#define ARC_PLUG_TIMER_ENABLE
#define ARC_UNPLUG_TIMER_ENABLE
#define OPTEK_ARC_CEC_ENABLE

#ifdef OPTEK_ARC_CEC_ENABLE
//#define OPTEK_CEC_CODE_CHANGED
#define SONY_TV_KD_43X85K
#define SONY_TV_KD_49X7500F
#define SAMSUNG_TV_UA43TU8000JXXZ
#define ONE_PLUS_TV_32HD2A00 //Produced in 2000
#define HISENSE_HZ32E35A
#define PHILIPS_40PFF5661_T3

#define ARC_PLUG_AT_STABNDBY_ENABLE
#define ARC_PLUG_AT_OTHER_SRC_ENABLE

#define CEC_SYNC_STATUS_VOLUME
#define CEC_SYNC_STATUS_MUTE
#define CEC_SYNC_STATUS_POWER_ON
#define CEC_SYNC_STATUS_POWER_STANBY
#define CEC_SYNC_STATUS_RELEASE_POWER_STANBY
#endif

#endif

#ifdef BTDM52_ENABLE
//#define BT_TX_ROLE_ENABLE
#ifdef BT_TX_ROLE_ENABLE
#define SBC_ENCODE_ENABLE
#endif

// #define BT_HFP_ENABLE

#ifdef BT_HFP_ENABLE
#define mSBC_DEC
#define mSBC_ENC
#endif

//#define BLE_ENABLE

#ifdef SAVE_MODULE_FROM_EEPROM
#define BT_W_EEPROM
#endif

#define BT_RELINK_ENABLE
#define BT_PAIRING_ENABLE

// #define BT_VOLUME_SET
// #define BT_VOLUME_SYC
#endif


#ifdef USB_HOST_MSC_ENABLE
#define USB_HOST_ENABLE
#endif

#ifdef USB_SPEAKER_ENABLE
#define USB_DEV_ENABLE
#endif

/***********************************************
*******        DISPLAY MODULE SET       ********
***********************************************/
#define DISPLAY_MODULE_ENABLE
#ifdef DISPLAY_MODULE_ENABLE

// #define LCD_MODULE_ENABLE
#define LED_MODULE_ENABLE

#ifdef LCD_MODULE_ENABLE
#define LCD_ST7032

#ifdef LCD_ST7032
#define SPI_ENABLE
#endif
#endif

#ifdef LED_MODULE_ENABLE
// #define LED_5050RGB
#define LED_GPIO_LIGHT

#ifdef LED_5050RGB
#define SPI_ENABLE
#endif

#ifdef LED_GPIO_LIGHT
#define LED_GPIO_LIGHT_COUNT_MAX    2
#endif
#endif
#endif



/***********************************************
*******     USB MSC AND USB DEV SET     ********
***********************************************/
#ifdef USB_HOST_ENABLE
/*
* link with libusbhost_msc_and_audio.a or libusbhost_audio_driver.a
* */
#define USB_HOST_LIB
#define FOR_ESD_PROTECT
#define IPOD_USB_AUDIO_CHARGE_ONLY
//#define USB_HOST_AUDIO_ENABLE
//#define TASK_HDD_PRIORITY_CHANGE

#define FILE_SYSTEM_ENABLE
//#define FILE_DELETE_ENABLE

#define FAST_SEARCH_ENABLE

#define MP3_DECODE_ENABLE
//#define CODEC_LIB_IN_MASK_ROM

#endif //USB_HOST_ENABLE

#define UPGRADE_PROJECT_ENABLE
#define UPGRADE_ENABLE

#ifdef UPGRADE_ENABLE
// #define UPGRADE_BY_USB
#define UPGRADE_BY_BT
#endif


#ifdef USB_DEV_ENABLE
#define USB_SPEAKER
//#define USB_DEV_LIB
#define HID_FUNC_ENABLE
//#define USB_SPEAKER_AUTO_DETECT
#define USB_SPEAKER_VOL_ENABLE

#define USB_DEV_UPDATA_MIC_ENABLE

#ifdef HID_FUNC_ENABLE
#define HID_FUNC_TEST
#endif

#endif //USB_DEV_ENABLE


#if ( (defined USB_HOST_ENABLE) || (defined USB_DEV_ENABLE) )
//#define USB_INT_CHANGEs_TO_LEVEL1
#endif

#define USB_OTG_INIT_FUN_CHANGED

//#define FAST_SEARCH_ENABLE
//#define FOLDER_SWITCH
//#define AUTO_PLAY_ENABLE

//#define WAV_DEC

//#define SYSTEM_CRASH_CHECK_ENABLE



/***********************************************
*******        audio device set         ********
***********************************************/
#define AUDIO_PROCESSOR

#ifdef AUDIO_PROCESSOR
// #define AUDIO_OPTEK_DAPP
#define AUDIO_AMP_DEV_ENABLE
#ifdef AUDIO_AMP_DEV_ENABLE
//#define AUDIO_MA12070P
// #define AUDIO_NTP8928
// #define AUDIO_ACM8625
#define AUDIO_AMP_TAS6582
// #define AUDIO_AMP_TAS5828M
#endif

#ifdef AUDIO_ACM8625
#define I2C0_ENABLE
#endif

#ifdef AUDIO_MA12070P
#define I2C1_ENABLE
#endif

#ifdef AUDIO_AMP_TAS5828M
#define I2C0_ENABLE
#endif

#ifdef AUDIO_AMP_TAS6582
#define I2C0_ENABLE
#endif
#ifdef AUDIO_OPTEK_DAPP
#define AUDIO_CODEC_USED_VOL
#define AUDIO_CODEC_USED_BASS_TREBLE
#define AUDIO_CODEC_USED_EQ_MODE
#define AUDIO_CODEC_USED_MUTE
#endif

#ifdef AUDIO_AMP_DEV_ENABLE
#define AUDIO_AMP_USED_VOL
#define AUDIO_AMP_USED_BASS_TREBLE
#define AUDIO_AMP_USED_EQ_MODE
#define AUDIO_AMP_USED_MUTE
#endif
#endif


/***********************************************
*******        DSP EQ tuning set        ********
***********************************************/
#define DSPEQ_TUNING_ENABLE

#ifdef DSPEQ_TUNING_ENABLE
#define DSPEQ_SETTING_ENABLE

#ifdef DSPEQ_SETTING_ENABLE
#define DSPEQ_TUNING_LR_CHANNEL
#define DSPEQ_TUNING_CENTER_CHANNEL
#define DSPEQ_TUNING_SUB_CHANNEL
#endif
#endif

// #define DSP_EQ_SETTING_ENABLE
/*
* if defined DSP_EQ_SETTING_16BIT_DECODE
* lib need used libeq_16bit.a
*
* if defined DSP_EQ_SETTING_24BIT_DECODE
* lib need used libeq_24bit.a
*/

#ifdef DSP_EQ_SETTING_ENABLE
// #define DSP_EQ_SETTING_16BIT_DECODE
#define DSP_EQ_SETTING_24BIT_DECODE
#endif


#define DSP_SONA_MODULE_ENABLE
#ifdef DSP_SONA_MODULE_ENABLE

#define DSP_SONA_MODULE_INIT
#define DSP_SONA_MODULE_CODE

#define SONA_DSP_MALLOC_PRINTF
#define SONA_DSP_PRINTF_ENABLE

#define SONA_DSP_PC_TOOLS_BY_UART_FIFO

#define SONA_DSP_PC_TOOLS_BY_UART

#ifdef SONA_DSP_PC_TOOLS_BY_UART
#define SONA_DSP_PC_TOOLS_BY_UART_0
// #define SONA_DSP_PC_TOOLS_BY_UART_1
// #define SONA_DSP_PC_TOOLS_BY_UART_2

// #define SONA_DSP_PC_TOOLS_PRINTF_BY_UART_0
// #define SONA_DSP_PC_TOOLS_PRINTF_BY_UART_1
#define SONA_DSP_PC_TOOLS_PRINTF_BY_UART_2
#endif
#endif
/*
* if defined DSP_HIFI_LIB_OLD
* lib need used optek_lib_dsp_dapp_V1.0.8_Q30_old_mode_20240112.zip
*
* if not defined DSP_HIFI_LIB_OLD
* lib need used optek_lib_dsp_dapp_V1.0.a_Q30_for_HiFi4_20240321.zip
*/
//#define DSP_HIFI_LIB_OLD


/***********************************************
*******         OPTEK Link set          ********
***********************************************/


/*******optek link macro*************/
//#define OPTEK_LINK_ENABLE

#ifdef OPTEK_LINK_ENABLE
#define OPL_MASTER_ENABLE
#define OPL_SLAVE_ENBALE

// #define OPL_MODE_TWO_MIC// less than 10ms
// #define OPL_MODE_GAME_HP // less than 15ms
// #define OPL_MODE_GAME_HP_LOW_LATENCY //less than 10ms
// #define OPL_MODE_GAME_HP_w_MIC//less than 15ms
#define OPL_MODE_GAME_2HP_w_MIC//less than 22ms 
#else
#define OPL_MODE_NULL
#endif


#if ((defined OPL_MODE_TWO_MIC && defined OPL_SLAVE_ENBALE) ||\
    (defined OPL_MODE_GAME_HP && defined OPL_SLAVE_ENBALE) ||\
    (defined OPL_MODE_GAME_HP_LOW_LATENCY && defined OPL_SLAVE_ENBALE) ||\
    (defined OPL_MODE_GAME_HP_w_MIC) ||\
    (defined OPL_MODE_GAME_2HP_w_MIC)\    
    )
#define LC3_ENCODE_ENABLE
#endif

#if ((defined OPL_MODE_TWO_MIC && defined OPL_MASTER_ENABLE) ||\
    (defined OPL_MODE_GAME_HP && defined OPL_MASTER_ENABLE) ||\
    (defined OPL_MODE_GAME_HP_LOW_LATENCY && defined OPL_MASTER_ENABLE) ||\
    (defined OPL_MODE_GAME_HP_w_MIC) ||\
    (defined OPL_MODE_GAME_2HP_w_MIC)\    
    )
#define LC3_DECODE_ENABLE
#endif

/*******optek link macro end*************/


#define OPL_ROLE_MASTER  (1<<0)
#define OPL_ROLE_SLAVE   (1<<1)
#define OPL_ROLE_SLAVE2  (1<<2)
#define nonOPL_ROLE      (1<<3)    //classical bluetooth
#define OPL_ROLE_UNDEF   (0)




/***********************************************
*******           SPI TEST set          ********
***********************************************/
//#define SPI_TEST

#ifdef SPI_TEST
#define SPI_ENABLE
//#define SPI_TX_TEST
#define SPI_RX_TEST
//#define SPI_DMA_TX_TEST
//#define SPI_DMA_RX_TEST
#endif

#endif //__OEM_H__
