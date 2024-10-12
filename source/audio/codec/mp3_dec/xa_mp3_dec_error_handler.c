/* Copyright (c) 2005, 2006 by Tensilica Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of
 * Tensilica Inc. and its licensors, and are licensed to the recipient
 * under the terms of a separate license agreement.  They may be
 * adapted and modified by bona fide purchasers under the terms of the
 * separate license agreement for internal use, but no adapted or
 * modified version may be disclosed or distributed to third parties
 * in any manner, medium, or form, in whole or in part, without the
 * prior written consent of Tensilica Inc.
 */

/*****************************************************************************/
/*                                                                           */
/*  File Name        : xa_mp3_dec_error_handler.c                            */
/*                                                                           */
/*  Description      : Error related functions of mp3 decoder                */
/*                                                                           */
/*  List of Functions: 1. xa_main_process                                    */
/*                                                                           */
/*  Issues / Problems: None                                                  */
/*                                                                           */
/*  Revision History :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*        29 07 2005       Tejaswi/Vishal        Created                     */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* File includes                                                             */
/*****************************************************************************/
//#include "tx_port.h"

#include "c_def.h"
#include "debug.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"

//#include "test.h"



#define WORD	signed int


/*****************************************************************************/
/* Process select hash defines                                               */
/*****************************************************************************/
//#define DISPLAY_MESSAGE

/*****************************************************************************/
/* Constant hash defines                                                     */
/*****************************************************************************/
//#define MAX_STACK_PROC 10
//#define MAX_MEM_ALLOCS 100
//#define XA_MAX_CMD_LINE_LENGTH 300
//#define XA_MAX_ARGS 20
//#define XA_SCREEN_WIDTH 80
//#define PARAMFILE "paramfilesimple.txt"

/*****************************************************************************/
/* Global memory constants                                                   */
/*****************************************************************************/
/*****************************************************************************/
/* mp3_dec ErrorCode Definitions                                             */
/*****************************************************************************/
/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Nonfatal Errors (none for MP3 decode) */
/* Fatal Errors */
//const char *ppb_xa_mp3_dec_api_fatal[] = {
const char * const ppb_xa_mp3_dec_api_fatal[] = {
    "NULL Pointer: Memory Allocation Error", 
    "Memory Allocation Error: Alignment requirement not met",
    "Invalid Command",
    "Invalid Command Type/Index"
};

/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
//const char *ppb_xa_mp3_dec_config_non_fatal[] = {
const char * const ppb_xa_mp3_dec_config_non_fatal[] = {
    "PCM sample width adjusted to 16 bits",
    "PCM sample width adjusted to 24 bits",
    "Invalid Decode-Frame Stream Position",
    "Mch-Not Supported",
    "Invalid parameter value"
};

/* Fatal Errors */
//const char *ppb_xa_mp3_dec_config_fatal[] = {
const char * const ppb_xa_mp3_dec_config_fatal[] = {
  "Invalid Sample Frequency",
  "Invalid Number of Channels"
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
//const char *ppb_xa_mp3_dec_execute_non_fatal[] = {
const char * const ppb_xa_mp3_dec_execute_non_fatal[] = {
    "Need more bits", 
    "Cannot step back in bitstream", 
    "Changed number of channels ", 
    "Changed layer"              , 
    "Changed sampling frequency ", 
    "Next sync word not found. Moving ahead",
    "CRC check failed",
    "Insufficient Main Audio Data",
    "Mult-Channel / Extension CRC error",
    "Muliti-Channel / Extension stream error"
    ,"Invalid Bitrate and Mode combination"
#ifdef ENABLE_SCF_CRC
    ,"",
    "",
    "",
    "",
    "",
    "",
    "ScF CRC error cnt 1",
    "ScF CRC error cnt 2",
    "ScF CRC error cnt 3",
    "ScF CRC error cnt 4"
#endif
};

/* Fatal Errors */
//const char *ppb_xa_mp3_dec_execute_fatal[] = {
const char * const ppb_xa_mp3_dec_execute_fatal[] = {
  "MPEG1 not supported", 
  "Unsupported layer", 
  "Overloaded internal bitstream buffer", 
  "Erroneous bitstream"    
};

/*****************************************************************************/
/* xa_testbench ErrorCode Definitions                                        */
/*****************************************************************************/
/*****************************************************************************/
/* Class 0: Memory & File Manager Errors                                     */
/*****************************************************************************/
/* Non Fatal Errors */
/* Fatal Errors */
#if 0
const char * const ppb_xa_testbench_mem_file_man_fatal[] = {
  "Memory Allocation Error", 
  "File Open Failed"
};
#endif

/*****************************************************************************/
/* error info structure                                                      */
/*****************************************************************************/
#if 0
/* The Module's Error Info Structure */
xa_error_info_struct xa_testbench_error_info = 
{
#if 1
	/* The Module Name	*/
	"xa_testbench",
	{
		/* The Class Names	*/
		"Memory & File Manager",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	},

	{
		/* The Message Pointers	*/
		{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
		  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },

		{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
		  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
	}
#endif
};
#endif

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_testbench_error_handler_init                          */
/*                                                                           */
/*  Description   : Initialize the error struct with string pointers         */
/*                                                                           */
/*  Inputs        : none                                                     */
/*                                                                           */
/*  Globals       : xa_error_info_struct xa_testbench_error_info             */
/*                  const char *ppb_xa_testbench_mem_file_man_fatal          */
/*                                                                           */
/*  Processing    : Init the struct with error string pointers               */
/*                                                                           */
/*  Outputs       : none                                                     */
/*                                                                           */
/*  Returns       : none                                                     */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*                                                                           */
/*****************************************************************************/
#if 0
void xa_testbench_error_handler_init()
{
#if 1
	/* The Message Pointers	*/
	xa_testbench_error_info.ppppb_error_msg_pointers[1][ 0] = 
		ppb_xa_testbench_mem_file_man_fatal;
#endif
}
#endif

/*****************************************************************************/
/* error info structure                                                      */
/*****************************************************************************/
/* The Module's Error Info Structure */
const xa_error_info_struct xa_mp3_dec_error_info = 
{
	/* The Module Name	*/
	"Tensilica MP3 Decoder",
	{
		/* The Class Names	*/
		"API",
		"Configuration",
		"Execution",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		""
	},

	{
	  /* The Message Pointers	*/
	  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
	    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },

	  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
	    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
	}
};

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_mp3_dec_error_handler_init                            */
/*                                                                           */
/*  Description   : Initialize the error struct with string pointers         */
/*                                                                           */
/*  Inputs        : none                                                     */
/*                                                                           */
/*  Globals       : xa_error_info_struct xa_mp3_dec_error_info               */
/*                  const char *ppb_xa_mp3_dec_api_fatal                     */
/*                  const char *ppb_xa_mp3_dec_config_non_fatal              */
/*                  const char *ppb_xa_mp3_dec_execute_non_fatal             */
/*                  const char *ppb_xa_mp3_dec_execute_fatal                 */
/*                                                                           */
/*  Processing    : Init the struct with error string pointers               */
/*                                                                           */
/*  Outputs       : none                                                     */
/*                                                                           */
/*  Returns       : none                                                     */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*                                                                           */
/*****************************************************************************/

void xa_mp3_dec_error_handler_init()
{
#ifdef XA_ERROR_HHANDLER_DEBUG

#ifdef XA_MP3_DEC_ERROR_DEBUG
	xa_testbench_error_info.pb_module_name = xa_mp3_dec_error_info.pb_module_name;
	xa_testbench_error_info.ppb_class_names[0] = xa_mp3_dec_error_info.ppb_class_names[0];

	/* The Message Pointers	*/
	xa_testbench_error_info.ppppb_error_msg_pointers[1][ 0] = 
		ppb_xa_mp3_dec_api_fatal;
	xa_testbench_error_info.ppppb_error_msg_pointers[0][ 1] = 
		ppb_xa_mp3_dec_config_non_fatal;
	xa_testbench_error_info.ppppb_error_msg_pointers[1][ 1] = 
		ppb_xa_mp3_dec_config_fatal;
	xa_testbench_error_info.ppppb_error_msg_pointers[0][ 2] = 
		ppb_xa_mp3_dec_execute_non_fatal;
	xa_testbench_error_info.ppppb_error_msg_pointers[1][ 2] = 
		ppb_xa_mp3_dec_execute_fatal;

#else

	/* The Message Pointers	*/
	xa_mp3_dec_error_info.ppppb_error_msg_pointers[1][ 0] = 
		ppb_xa_mp3_dec_api_fatal;
	xa_mp3_dec_error_info.ppppb_error_msg_pointers[0][ 1] = 
		ppb_xa_mp3_dec_config_non_fatal;
	xa_mp3_dec_error_info.ppppb_error_msg_pointers[1][ 1] = 
		ppb_xa_mp3_dec_config_fatal;
	xa_mp3_dec_error_info.ppppb_error_msg_pointers[0][ 2] = 
		ppb_xa_mp3_dec_execute_non_fatal;
	xa_mp3_dec_error_info.ppppb_error_msg_pointers[1][ 2] = 
		ppb_xa_mp3_dec_execute_fatal;
#endif

#endif
}

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_error_handler                                         */
/*                                                                           */
/*  Description   : Called Prints the status error code from the err_info    */
/*                                                                           */
/*  Inputs        : xa_error_info_struct *p_mod_err_info (Error info struct) */
/*                  const char *pb_context (Context of error)                */
/*                  XA_ERRORCODE code (Error code)                           */
/*                                                                           */
/*  Globals       : none                                                     */
/*                                                                           */
/*  Processing    : whenever any module calls the errorhandler,  it  informs */
/*                  it about the module for which it is called and a context */
/*                  in which it was  called  in addition to  the  error_code */
/*                  the message is displayed  based  on the  module's  error */
/*                  message  array  that maps to  the error_code the context */
/*                  gives specific info in which the error occured  e.g. for */
/*                  testbench   module,  memory  allocator   can   call  the */
/*                  errorhandler   for  memory  inavailability  in   various */
/*                  contexts like input_buf or output_buf e.g.  for  mp3_enc */
/*                  module, there can be various instances running.  context */
/*                  can be used to  identify  the  particular  instance  the */
/*                  error handler is being called for                        */
/*                                                                           */
/*  Outputs       : None                                                     */
/*                                                                           */
/*  Returns       : XA_ERRORCODE error_value  (Error value)                  */
/*                                                                           */
/*  Issues        : none                                                     */
/*                                                                           */
/*  Revision history :                                                       */
/*                                                                           */
/*        DD MM YYYY       Author                Changes                     */
/*        29 07 2005       Tejaswi/Vishal        Created                     */
/*                                                                           */
/*****************************************************************************/
#if 0
XA_ERRORCODE xa_error_handler(xa_error_info_struct *p_mod_err_info, 
							const char *pb_context,
							XA_ERRORCODE code)
{
#if 1
	if (code == XA_NO_ERROR)
	{
		return XA_NO_ERROR;
	}

	{
		WORD is_fatal     = XA_ERROR_SEVERITY(code) != 0;
		WORD err_class    = XA_ERROR_CLASS(code);
		WORD err_sub_code = XA_ERROR_SUBCODE(code);
	
		DBG_Printf("Codec Err\n");
		if (!is_fatal)
		{
			DBG_Printf("non ");
		}

		DBG_Printf("fatal error: ");

		if ((p_mod_err_info->pb_module_name != NULL) &&
			(p_mod_err_info->pb_module_name[0] != '\0'))
		{
			DBG_Printf("%s: ", p_mod_err_info->pb_module_name);
		}
		
		if (p_mod_err_info->ppb_class_names[err_class] != NULL)
		{
			DBG_Printf("%s: ", p_mod_err_info->ppb_class_names[err_class]);
		}
	
		if (pb_context != NULL)
		{
			DBG_Printf("%s: ", pb_context);
		}

		DBG_Printf("%s\n", p_mod_err_info->ppppb_error_msg_pointers[is_fatal][err_class][err_sub_code]);
	}
#endif

  return XA_NO_ERROR;
}
#endif
