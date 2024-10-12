/* Copyright (c) 2005-2014 by Tensilica Inc.  ALL RIGHTS RESERVED.
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

#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"

#include "c_def.h"
#include "debug.h"

#ifdef SBC_ENCODE_ENABLE

#define MAX_STACK_PROC 10
#define MAX_MEM_ALLOCS 100
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define XA_SCREEN_WIDTH 80

/*****************************************************************************/
/* sbc_enc ErrorCode Definitions                                             */
/*****************************************************************************/
/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Nonfatal Errors (none for SBC encode) */
/* Fatal Errors */
const char *ppb_xa_sbc_enc_api_fatal[] = {
  "NULL Pointer: Memory Allocation Error", 
  "Memory Allocation Error: Alignment requirement not met",
  "Invalid Command",
  "Invalid Command Type/Index",
  "Invalid API Sequence "  
};

/* Nonfatal Errors */
const char *ppb_xa_sbc_enc_api_non_fatal[] = {
  "Invalid API Sequence "
};


/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
const char *ppb_xa_sbc_enc_config_non_fatal[] = {
  "",
  ""
};

/* Fatal Errors */
const char *ppb_xa_sbc_enc_config_fatal[] = {
  "Subbands not supported! Use 4 or 8",
  "Blocks not supported! Use 4, 8, 12 or 16",
  "Invalid channel mode",
  "Unsupported sampling rate",
  "Bitpool value not supported"
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
const char *ppb_xa_sbc_enc_execute_non_fatal[] = {
  "Need more data", 
  "", 
  "", 
  "", 
  "", 
  "",
  "",
  ""
};

/* Fatal Errors */
const char *ppb_xa_sbc_enc_execute_fatal[] = {
  "Output buffer overflow", 
};

/*****************************************************************************/
/* xa_testbench ErrorCode Definitions                                        */
/*****************************************************************************/
/*****************************************************************************/
/* Class 0: Memory & File Manager Errors                                     */
/*****************************************************************************/
/* Non Fatal Errors */
/* Fatal Errors */
const char *ppb_sbc_enc_xa_testbench_mem_file_man_fatal[] = {
  "Memory Allocation Error", 
  "File Open Failed"
};

/*****************************************************************************/
/* error info structure                                                      */
/*****************************************************************************/
/* The Module's Error Info Structure */
xa_error_info_struct xa_sbc_enc_testbench_error_info = 
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
		/* The Message Pointers	*/
		{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
		  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL },
		{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
		  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
	}
};

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_sbc_enc_testbench_error_handler_init                  */
/*                                                                           */
/*  Description   : Initialize the error struct with string pointers         */
/*                                                                           */
/*  Inputs        : none                                                     */
/*                                                                           */
/*  Globals       : xa_error_info_struct xa_sbc_enc_testbench_error_info     */
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

VOID xa_sbc_enc_testbench_error_handler_init()
{
	/* The Message Pointers	*/
	xa_sbc_enc_testbench_error_info.ppppb_error_msg_pointers[1][ 0] = 
		ppb_sbc_enc_xa_testbench_mem_file_man_fatal;
}

/*****************************************************************************/
/* error info structure                                                      */
/*****************************************************************************/
/* The Module's Error Info Structure */
xa_error_info_struct xa_sbc_enc_error_info = 
{
	/* The Module Name	*/
	"Tensilica SBC Encoder",
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
/*  Function name : xa_sbc_enc_error_handler_init                            */
/*                                                                           */
/*  Description   : Initialize the error struct with string pointers         */
/*                                                                           */
/*  Inputs        : none                                                     */
/*                                                                           */
/*  Globals       : xa_error_info_struct xa_sbc_enc_error_info               */
/*                  const char *ppb_xa_sbc_enc_api_non_fatal                */
/*                  const char *ppb_xa_sbc_enc_api_fatal                     */
/*                  const char *ppb_xa_sbc_enc_config_non_fatal              */
/*                  const char *ppb_xa_sbc_enc_execute_non_fatal             */
/*                  const char *ppb_xa_sbc_enc_execute_fatal                 */
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

VOID xa_sbc_enc_error_handler_init()
{
	/* The Message Pointers	*/
    xa_sbc_enc_error_info.ppppb_error_msg_pointers[0][ 0] =
        ppb_xa_sbc_enc_api_non_fatal;
	xa_sbc_enc_error_info.ppppb_error_msg_pointers[1][ 0] = 
		ppb_xa_sbc_enc_api_fatal;
	xa_sbc_enc_error_info.ppppb_error_msg_pointers[0][ 1] = 
		ppb_xa_sbc_enc_config_non_fatal;
	xa_sbc_enc_error_info.ppppb_error_msg_pointers[1][ 1] = 
		ppb_xa_sbc_enc_config_fatal;
	xa_sbc_enc_error_info.ppppb_error_msg_pointers[0][ 2] = 
		ppb_xa_sbc_enc_execute_non_fatal;
	xa_sbc_enc_error_info.ppppb_error_msg_pointers[1][ 2] = 
		ppb_xa_sbc_enc_execute_fatal;
}

/*****************************************************************************/
/*                                                                           */
/*  Function name : xa_sbc_enc_error_handler                                 */
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
/*                  contexts like input_buf or output_buf e.g.  for  sbc_enc */
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

XA_ERRORCODE
xa_sbc_enc_error_handler(xa_error_info_struct *p_mod_err_info, 
		 const char *pb_context,
		 XA_ERRORCODE code) {
  if (code == XA_NO_ERROR) {
    return XA_NO_ERROR;
  }
  {
    WORD is_fatal     = XA_ERROR_SEVERITY(code) != 0;
    WORD err_class    = XA_ERROR_CLASS(code);
    WORD err_sub_code = XA_ERROR_SUBCODE(code);
	
    DBG_Printf("\n");
    if (!is_fatal) {
      DBG_Printf("non ");
    }
    DBG_Printf("fatal error: ");

    if ((p_mod_err_info->pb_module_name != NULL) &&
	(p_mod_err_info->pb_module_name[0] != '\0')) {
      DBG_Printf("%s: ", p_mod_err_info->pb_module_name);
    }
    if (p_mod_err_info->ppb_class_names[err_class] != NULL) {
      DBG_Printf("%s: ", p_mod_err_info->ppb_class_names[err_class]);
    }
    if (pb_context != NULL) {
      DBG_Printf("%s: ", pb_context);
    }
    DBG_Printf("%s\n", p_mod_err_info->ppppb_error_msg_pointers[is_fatal][err_class][err_sub_code]);
  }
  return XA_NO_ERROR;
}


//"Usage: \n"
//"sbc_encoder [-jsv] [-lblk_len] [-nsubbands] [-p] [-rrate] -ooutputfile -iinputfile \n"
//"            [-s] use the stereo mode for stereo signals \n"
//"            [-v] verbose mode \n"
//"            [-j] allow the use of joint coding for stereo signals \n"
//"            [-lblk_len] blk_len specifies the APCM block length, out of [4,8,12,16] \n"
//"            [-nsubbands] subbands specifies the number of subbands, out of [4,8] \n"
//"            [-p] a simple psycho acoustic model is used \n"
//"            [-rrate] specifies the bit rate in bps \n"
//"            -ooutputfile specifies the name of the bitstream output file \n"
//"            -iinputfile specifies the audio input file, the major audio formats are supported \n"
#endif