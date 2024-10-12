/*
 * app_standby_source.h
 *
 *  Created on: 2024/2/21
 *      Author: Windows
 */

#ifndef __APP_STANDBY_SOURCE_H__
#define __APP_STANDBY_SOURCE_H__

void app_source_convert_to_standby(void);
U32 app_standby_enter_keyfunction(KEY_HANDLE_PARAM_STRU param);

void app_source_convert_to_standby_enter(void);
void app_source_convert_to_standby_release(void);
U8 app_source_get_standby_mode_status(void);

#endif /* __APP_STANDBY_SOURCE_H__ */
