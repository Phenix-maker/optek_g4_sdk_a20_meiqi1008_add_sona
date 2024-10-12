#ifndef	__PRINTF_H__
#define	__PRINTF_H__

#include "sdk_com.h"

/*
 ****************************************************************************************
 * configures
 ****************************************************************************************
 */

#define PRINTF_LEVELNONE    0
#define PRINTF_LEVELFATAL   1
#define PRINTF_LEVELERROR   2
#define PRINTF_LEVELWARN    3
#define PRINTF_LEVELINFO    4
#define PRINTF_LEVELDEBUG   5

#define PRINTF_LEVEL PRINTF_LEVELDEBUG  //log output level control


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void printf_init( const extif_api_stru* ext_if );

/*
 * 		for simple string output, recommend to use puts instread of printf
 * 		puts is much more efficient than printf in this case
 */
int myputs( const char *str );


/*
 * 
 *		format	=	%type
 *		type	=	{
 *						c		-	single byte, char type data
 *						s		-	single byte string
 *						S		-	wide/2byte string
 *						d/D, 	-	signed int
 *						u/U, 	-	unsigned int
 *						x, 		-	unsigned hexadecimal, lower case
 *						X		-	unsigned hexadecimal, upper case
 *						...
 *					}
 *
 *		to print '%', use '%%'
 *		to change output line, add suffix "\r\n"
 *		to output str ended with wide character str, use '%S' & add suffix "\0\0"
 * 
 */

int	myprintf( const char *format, ... );

int	mysnprintf(char* const Buffer, unsigned int buf_size, const char *format, ... );

int myputs_for_interrupt( const char *str );

int	myprintf_for_interrupt( const char *format, ... );

int	mysnprintf_for_interrupt(char* const Buffer, unsigned int buf_size, const char *format, ... );


/*
 ****************************************************************************************
 * 
 ****************************************************************************************
 */
/* log output with level */
#define PRINTF_W_LEVEL(level, fmt...) 	myprintf("[%s]", level); myprintf(fmt);

#if(PRINTF_LEVEL == PRINTF_LEVELNONE)
#define printf_fatal(fmt...)
#define printf_error(fmt...)
#define printf_warn(fmt...)
#define printf_info(fmt...)
#define printf_debug(fmt...)
#elif(PRINTF_LEVEL == PRINTF_LEVELFATAL)
#define printf_fatal(fmt...) 	PRINTF_W_LEVEL("FATAL", fmt)
#define printf_error(fmt...)
#define printf_warn(fmt...)
#define printf_info(fmt...)
#define printf_debug(fmt...)
#elif(PRINTF_LEVEL == PRINTF_LEVELERROR)
#define printf_fatal(fmt...) 	PRINTF_W_LEVEL("FATAL", fmt)
#define printf_error(fmt...) 	PRINTF_W_LEVEL("ERROR", fmt)
#define printf_warn(fmt...)
#define printf_info(fmt...)
#define printf_debug(fmt...)
#elif(PRINTF_LEVEL == PRINTF_LEVELWARN)
#define printf_fatal(fmt...) 	PRINTF_W_LEVEL("FATAL", fmt)
#define printf_error(fmt...) 	PRINTF_W_LEVEL("ERROR", fmt)
#define printf_warn(fmt...) 	PRINTF_W_LEVEL("WARN", fmt)
#define printf_info(fmt...)
#define printf_debug(fmt...)
#elif(PRINTF_LEVEL == PRINTF_LEVELINFO)
#define printf_fatal(fmt...) 	PRINTF_W_LEVEL("FATAL", fmt)
#define printf_error(fmt...) 	PRINTF_W_LEVEL("ERROR", fmt)
#define printf_warn(fmt...) 	PRINTF_W_LEVEL("WARN", fmt)
#define printf_info(fmt...) 	myprintf(fmt)
#define printf_debug(fmt...)
#elif(PRINTF_LEVEL == PRINTF_LEVELDEBUG)
#define printf_fatal(fmt...) 	PRINTF_W_LEVEL("FATAL", fmt)
#define printf_error(fmt...) 	PRINTF_W_LEVEL("ERROR", fmt)
#define printf_warn(fmt...)  	PRINTF_W_LEVEL("WARN", fmt)
#define printf_info(fmt...)  	myprintf(fmt)
#define printf_debug(fmt...) 	PRINTF_W_LEVEL("DEBUG", fmt)
#endif

#endif //__PRINTF_H__
