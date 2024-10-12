#include "os_config.h"

#include "app_window.h"
#include "app_task.h"

#ifdef OPTEK_LINK_ENABLE
#include "optek_link_commons.h"
#endif

// #define WINDOW_DEEPTH       6

// struct {	
// 	struct {
// 		WINDOW *window;								//please note, it may be const, so save sram space
// 		I16 list_index;								//first showing list entry
// 		I16 list_cursor;							//cursor pos
// 		u8	win_state;								//state for this window
// 		u16 win_cn;									//cn for this window	
// 	} app_window[WINDOW_DEEPTH];

// 	I16 window_index;
// 	TimerHandle_t timer;
// } app_window_data;

APP_WINDOW_DATA app_window_data;

//static WINDOW * pCurrent_window = NULL; 
WINDOW * pCurrent_window = NULL; 

void app_window_init(void)
{
	pCurrent_window = NULL;
}

WINDOW * app_window_current_get(void)
{
	return pCurrent_window;
}

void app_window_timer_expired(void)
{
	#ifdef OPTEK_LINK_RX_QUALITY_DEBUG
	optek_link_rx_quality_print();
	#endif

	if (pCurrent_window->timer_handler != NULL)
	{
		pCurrent_window->timer_handler();
	}
}

void app_window_time_reset (void)
{
	app_task_timer_ms_set(TIMER_WindowTimer, 0);
}

U32 app_window_time_get (void)
{
	return pCurrent_window->window_time;
}

void app_window_time_set (U32 ms)
{
	app_task_timer_ms_set(TIMER_WindowTimer, ms);
}

void app_window_time_start (void)
{
	app_window_time_set(pCurrent_window->window_time);
}

const APP_NAV_KEY key_null = {0xFFFF,NULL,NULL,0,NULL};

APP_NAV_KEY *search_key_handle(U16 key_id)
{
	int i;
	WINDOW *window = pCurrent_window;

	APP_NAV_KEY *app_nav_key = window->window_key;

	if (app_nav_key != NULL)
	{	
		for (i=0; i<window->window_key_number; i++)
		{
			if (app_nav_key->key_id == key_id)
			{
				return app_nav_key;
			}
			
			app_nav_key++;
		}
	}
	
	return &key_null;
}

void app_window_timer_count_init(void)
{
	app_window_data.app_window[app_window_data.window_index].win_cn = 0;
}

U32 app_window_timer_count_get(void)
{
	return app_window_data.app_window[app_window_data.window_index].win_cn;
}

void app_window_timer_count_set(U8 count)
{
	app_window_data.app_window[app_window_data.window_index].win_cn = count;
}

void app_window_timer_count_plus(void)
{
	app_window_data.app_window[app_window_data.window_index].win_cn++;
}

void app_window_disp_state_init(void)
{
	app_window_data.app_window[app_window_data.window_index].win_state = 0;
}

void app_nav_window_set(const WINDOW *pWindow, U8 increase)
{
	if (increase)
		app_window_data.window_index++;
	else
		app_window_data.window_index = 0;

	app_window_data.app_window[app_window_data.window_index].list_index = 0;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	app_window_data.app_window[app_window_data.window_index].window	= (WINDOW *) pWindow;

	app_window_data.app_window[app_window_data.window_index].win_cn = 0;
	
	pCurrent_window = (WINDOW *) pWindow;
	#ifdef USB_HOST_ENABLE
	if (pCurrent_window == (WINDOW *)&readingcd_window)
	{
		asm("nop");
	}

	if (pCurrent_window == (WINDOW *)&nav_mp3_stop_window)
	{
		asm("nop");
	}
	#endif
	app_window_time_set (pWindow->window_time);

	if (pCurrent_window->draw_region)
	{
		pCurrent_window->draw_region ();
	}
}

void app_nav_window_set_no_draw(const WINDOW *pWindow, U8 increase)
{
	if (increase)
		app_window_data.window_index++;
	else
		app_window_data.window_index = 0;

	app_window_data.app_window[app_window_data.window_index].list_index = 0;
	app_window_data.app_window[app_window_data.window_index].list_cursor = 0;
	app_window_data.app_window[app_window_data.window_index].window	= (WINDOW *) pWindow;
	app_window_data.app_window[app_window_data.window_index].win_cn = 0;
	
	pCurrent_window = (WINDOW *) pWindow;
	app_window_time_set (pWindow->window_time);
}

void app_window_draw_region_none (void)
{

}

void app_window_cancel (void)
{
	DBG_Assert ( app_window_data.window_index >= 0 );

	if ( app_window_data.window_index > 0)
	{
		app_window_data.window_index--;

		pCurrent_window = app_window_data.app_window[app_window_data.window_index].window;

		app_window_time_set (pCurrent_window->window_time);

		pCurrent_window->draw_region ();
	}
}

void app_window_cancel_no_draw (void)
{
	DBG_Assert ( app_window_data.window_index >= 0 );

	if ( app_window_data.window_index > 0)
	{
		app_window_data.window_index--;
	
		pCurrent_window = app_window_data.app_window[app_window_data.window_index].window;

		app_window_time_set (pCurrent_window->window_time);
	}
}




