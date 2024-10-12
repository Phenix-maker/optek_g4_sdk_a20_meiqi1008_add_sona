#ifndef __LED_DEVICE_H__
#define __LED_DEVICE_H__

#if defined LED_5050RGB
#include "LED_5050RGB.h"
#endif

typedef struct _APP_LED_DEVICE {
	void		(*init) (void);
	void		(*open) (void);
	void		(*close) (void);
	void		(*fini) (void);
    void		(*disp_source) (U8 audio_source);
    void		(*disp_bt_status) (U8 bt_status, U32 flash_time);
    void        (*disp_music_play) (U32 music_level);
    void        (*disp_standby_status) (U8 standby_status);
} APP_LED_DEVICE;

extern const APP_LED_DEVICE app_led_device;

void led_none_void(void);
void led_none_disp_source(U8 audio_source);
void led_none_disp_bt_status (U8 bt_status, U32 flash_time);
void led_none_disp_music_play(U32 music_level);
void led_none_disp_standby_status(U8 standby_status);

#define LED_NULL_FUNC   led_none_void,\
                        led_none_void,\
                        led_none_void,\
                        led_none_void,\
                        led_none_disp_source,\
                        led_none_disp_bt_status,\
                        led_none_disp_music_play,\
                        led_none_disp_standby_status



#endif /* __LED_DEVICE_H__ */
