#ifndef __APP_WINDOW_H__
#define __APP_WINDOW_H__

#include "window.h"

#define WINDOW_DEEPTH       		6

#define WINDOW_PRINTF_ENABLE
#ifdef WINDOW_PRINTF_ENABLE
#define WINDOW_PRINTF_COUNT       	5
#endif

typedef struct {
	WINDOW *window;								//please note, it may be const, so save sram space
	I16 list_index;								//first showing list entry
	I16 list_cursor;							//cursor pos
	u8	win_state;								//state for this window
	u16 win_cn;									//cn for this window	
} APP_WINDOW;

typedef struct {
	APP_WINDOW app_window[WINDOW_DEEPTH];
	I16 window_index;
	TimerHandle_t timer;								//cn for this window	
}APP_WINDOW_DATA;
extern APP_WINDOW_DATA app_window_data;

extern WINDOW * pCurrent_window;

APP_NAV_KEY *search_key_handle(U16 key_id);

void app_window_init(void);

void app_window_timer_expired(void);
void app_window_time_reset (void);
U32 app_window_time_get (void);
void app_window_time_set (U32 ms);
void app_window_time_start (void);
void app_window_timer_count_init(void);
U32 app_window_timer_count_get(void);
void app_window_timer_count_set(U8 count);
void app_window_timer_count_plus(void);
void app_window_disp_state_init(void);
void app_nav_window_set(const WINDOW *pWindow, U8 increase);
void app_nav_window_set_no_draw(const WINDOW *pWindow, U8 increase);
void app_window_draw_region_none (void);
void app_window_cancel (void);
void app_window_cancel_no_draw (void);

WINDOW * app_window_current_get(void);

void app_window_readingcd_window_init (void);


extern const WINDOW aux_window;
extern const WINDOW arc_window;
extern const WINDOW bt_window;
extern const WINDOW coaxial_window;
extern const WINDOW internal_window;
extern const WINDOW optical_window;
extern const WINDOW usb_msc_window;
extern const WINDOW usb_speaker_window;

extern const WINDOW standby_release_window;
extern const WINDOW standby_window;

extern const WINDOW volume_window;

#endif //__APP_WINDOW_H__
