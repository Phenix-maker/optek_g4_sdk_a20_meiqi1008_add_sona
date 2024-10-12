#ifndef __APP_SOURCE_H__
#define __APP_SOURCE_H__

#include "window.h"

/**
 *************************************************************************************
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */
 /*
 * SOURCE_INTERNAL
 * SOURCE_AUX
 * SOURCE_OPTICAL
 * SOURCE_HDMI
 * SOURCE_ARC
 * SOURCE_USB_MSC
 * SOURCE_BT
 * SOURCE_USB_SPEAKER
 */
#define DEFAULT_APP_SOURCE     SOURCE_AUX//SOURCE_BT




/**
 *************************************************************************************
 * STRUCT DEFINITION
 *************************************************************************************
 */
/**
 * @def APP_SOURCE_STRU
 *
 * @brief consisting of some source information
 */
typedef struct {
    char *string;
    enumSOURCE_ID id;
    void (* switch_to_source)(void);
    void (* exit_from_source)(void);
} APP_SOURCE_STRU;



/*
 *************************************************************************************
 * FUNCTION DECLARATION
 ****************************************************************************************
 */
extern const APP_SOURCE_STRU internal_source;
extern const APP_SOURCE_STRU aux_source;
extern const APP_SOURCE_STRU optical_source;
extern const APP_SOURCE_STRU hdmi_source;
extern const APP_SOURCE_STRU arc_source;
extern const APP_SOURCE_STRU coaxial_source;

extern const APP_SOURCE_STRU usb_msc_source;
extern const APP_SOURCE_STRU usb_speaker_source;
extern const APP_SOURCE_STRU bt_source;
extern const APP_SOURCE_STRU hfp_source;
extern const APP_SOURCE_STRU opteklink_slave_source;

extern const APP_SOURCE_STRU standby_source;

extern const APP_SOURCE_STRU *source_table[];
extern const APP_SOURCE_STRU *source_table_for_sw[];

void app_source_finish(void);

void app_source_convert_to_internal(void);
void app_source_convert_to_aux(void);
void app_source_convert_to_hdmi(void);
void app_source_convert_to_optical(void);
void app_source_convert_to_arc(void); 
void app_source_convert_to_coaxial(void);
void app_source_convert_to_usb_msc(void);
void app_source_convert_to_usb_speaker(void);
void app_source_convert_to_bt(void);
APP_SOURCE_STRU* get_source_entry_by_source_id(enumSOURCE_ID src_id);

void app_source_next_enter(void);
void enter_fix_source(U8 audio_source);
U32 app_source_next_enter_for_keyfunction(KEY_HANDLE_PARAM_STRU param);


#endif //_APP_SOURCE_H__
