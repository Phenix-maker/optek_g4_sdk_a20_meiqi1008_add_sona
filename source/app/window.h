#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "app_key.h"

typedef struct {
	U32	combo_times : 7;
	U32	combo_end   : 1;
	U32 hold_times  : 8;
} KEY_HANDLE_PARAM_STRU;

typedef struct {
	APP_KEY key_id;

    /**
     *************************************************************************************
     * @brief this is the key down handler.
     *
     * @param[in]  param.combo_times  Combo conut
	 * 
	 * @retval Maximum interval between combos
     *************************************************************************************
     */
	U32	(*key_down) (KEY_HANDLE_PARAM_STRU param);

    /**
     *************************************************************************************
     * @brief this is the key up handler.
     *
     * @param[in]  param.combo_times  Combo conut
     * @param[in]  param.hold_times  The number of times the hold time expires
	 * 
	 * @retval don't care
     *************************************************************************************
     */
	U32 (*key_up) (KEY_HANDLE_PARAM_STRU param);
	
	/* key hold time, unit ms */
	U32 hold_time;

    /**
     *************************************************************************************
     * @brief this is the key hold handler.
     *
     * @param[in]  param.combo_times  Combo conut
     * @param[in]  param.hold_times  The number of times the hold time expires
	 * 
	 * @retval the next hold time, 0 means infinity
     *************************************************************************************
     */
	U32	(*key_hold) (KEY_HANDLE_PARAM_STRU param);
} APP_NAV_KEY;

typedef struct {
	U16	index;					//index to string menu or to disc info
	U8	attribute;
	U8	status;
} WINDOW_LIST;

typedef struct {
	I16	start_line;
	I16	end_line;
	I16 start_pos;
	I16 end_pos;
} REGION;

typedef struct {	
	char * desc_string;
	void (*draw_region) (void);	   //func to draw region
	const APP_NAV_KEY *window_key; //window key handler
	U32 window_key_number;         //window key number
	U32	window_time;               //window time
	void (*timer_handler) (void);  //window time function

	REGION region;                 //window region
	unsigned char const *title;	   //window title name
	void (*draw_title) (void);     //func to draw title	
	WINDOW_LIST	*list;             //list	
	I16 list_entry;			
} WINDOW;


#define MAX_LCD_LINE    1
#define MAX_LCD_POS     10

#define WINDOW_RESERVES {0,MAX_LCD_LINE,0,MAX_LCD_POS},\
                         NULL,\
                         NULL,\
                         NULL,\
                         0\

#define KEY_TABLE(x)     x, (sizeof(x)/sizeof(x[0]))


#define VOID_FUNC_TO_KEY_HANDLER(func_name) U32 func_name##_keyhandler(KEY_HANDLE_PARAM_STRU param){\
	func_name();\
	return 0;\
}

#endif //__WINDOW_H__
