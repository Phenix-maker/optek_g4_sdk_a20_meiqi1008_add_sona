/*
 * Copyright (c) 2005-2017 Cadence Design Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_error_handler                                         */
/*                                                                           */
/*  Description   : Called Prints the status error code from the err_info    */
/*                                                                           */
/*  Inputs        : xa_error_info_struct *p_mod_err_info (Error info struct) */
/*                  WORD8 *pb_context (Context of error)                     */
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

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "mem_reloc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"



/*****************************************************************************/
/* xa_testbench ErrorCode Definitions                                        */
/*****************************************************************************/
/*****************************************************************************/
/* Class 0: Memory & File Manager Errors                                     */
/*****************************************************************************/
/* Non Fatal Errors */
/* Fatal Errors */
const char *ppb_xa_testbench_mem_file_man_fatal[] = {
	"Memory Allocation Error",
	"File Open Failed",
    "File Read Failed",
    "Failed to do Init",
};

/*****************************************************************************/
/* error info structure                                                      */
/*****************************************************************************/
/* The Module's Error Info Structure */
#ifdef XA_ERROR_HHANDLER_DEBUG
xa_error_info_struct xa_testbench_error_info =
#else
const xa_error_info_struct xa_testbench_error_info =
#endif
{
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
        /* The Message Pointers */
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
        { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
          NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
	}
};

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_testbench_error_handler_init                          */
/*                                                                           */
/*  Description   : Initialize the error struct with string pointers         */
/*                                                                           */
/*  Inputs        : none                                                     */
/*                                                                           */
/*  Globals       : xa_error_info_struct xa_testbench_error_info             */
/*                  pWORD8 ppb_xa_testbench_mem_file_man_fatal               */
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
/*        29 07 2005       Ittiam                Created                     */
/*                                                                           */
/*****************************************************************************/

VOID xa_testbench_error_handler_init()
{
#ifdef XA_ERROR_HHANDLER_DEBUG
	/* The Message Pointers	*/
	xa_testbench_error_info.ppppb_error_msg_pointers[1][ 0] =
		ppb_xa_testbench_mem_file_man_fatal;
#endif
}

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_error_handler                                         */
/*                                                                           */
/*  Description   : Called Prints the status error code from the err_info    */
/*                                                                           */
/*  Inputs        : xa_error_info_struct *p_mod_err_info (Error info struct) */
/*                  WORD8 *pb_context (Context of error)                     */
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

XA_ERRORCODE xa_error_handler(xa_error_info_struct *p_mod_err_info,
			      const char *pb_context, XA_ERRORCODE code) __INTERNAL_RAM_TEXT;

XA_ERRORCODE xa_error_handler(xa_error_info_struct *p_mod_err_info,
			      const char *pb_context, XA_ERRORCODE code)
{
	if(code == XA_NO_ERROR)
	{
		return XA_NO_ERROR;
	}

#ifndef XA_ERROR_HHANDLER_DEBUG

	WORD is_fatal     = XA_ERROR_SEVERITY(code);
	WORD err_class    = XA_ERROR_CLASS(code);
	WORD err_sub_code = XA_ERROR_SUBCODE(code);

	if (!is_fatal)
	{
		/*
		*DBG_Puts("non ");
		*/
	}
	else
	{
		DBG_Printf("Codec Err\n\r");
	}

	return XA_NO_ERROR;

#else

#ifndef XA_ERROR_HHANDLER_DEBUG
	WORD is_fatal     = XA_ERROR_SEVERITY(code);
	WORD err_class    = XA_ERROR_CLASS(code);
	WORD err_sub_code = XA_ERROR_SUBCODE(code);

	if (!is_fatal)
	{
		//DBG_Puts("non ");
	}
	else
	{
		DBG_Puts("dec error\n\r");
	}

	return XA_NO_ERROR;
#else
	WORD is_fatal     = XA_ERROR_SEVERITY(code);
	WORD err_class    = XA_ERROR_CLASS(code);
	WORD err_sub_code = XA_ERROR_SUBCODE(code);

	if (!is_fatal)
	{
		/*
		*DBG_Printf("non ");
		*/
	}

	if (p_mod_err_info->pb_module_name != NULL)
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

	if ( (p_mod_err_info->ppppb_error_msg_pointers[is_fatal][err_class] == NULL) ||
		(p_mod_err_info->ppppb_error_msg_pointers[is_fatal][err_class][err_sub_code] == NULL) )
	{
		DBG_Printf(" Unknown Error [fatal][err_class][err_id]:[%d][%d][%d]\n\r", 
				 is_fatal, err_class, err_sub_code);
    }
	else
	{
		DBG_Printf("%s\n\r", p_mod_err_info->ppppb_error_msg_pointers
	 		[is_fatal][err_class][err_sub_code]);
	}

	return XA_NO_ERROR;
#endif

#endif
}
