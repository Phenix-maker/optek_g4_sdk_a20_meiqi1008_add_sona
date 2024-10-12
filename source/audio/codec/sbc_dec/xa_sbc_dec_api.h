/* Copyright (c) 2009-2012 by Tensilica Inc.  ALL RIGHTS RESERVED.
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

#ifndef __XA_SBC_DEC_API_H__
#define __XA_SBC_DEC_API_H__



#if 0//ndef xa_codec_func_t

typedef struct xa_codec_opaque { } *xa_codec_handle_t;

typedef XA_ERRORCODE xa_codec_func_t(xa_codec_handle_t p_xa_module_obj,
									 signed int     i_cmd,
									 signed int     i_idx,
									 void          *pv_value);
#endif

/*****************************************************************************/
/* SBC Decoder specific API definitions                                      */
/*****************************************************************************/

/* sbc_dec-specific configuration parameters */
enum xa_config_param_sbc_dec {
  XA_SBC_DEC_CONFIG_PARAM_PCM_WDSZ     = 0,
  XA_SBC_DEC_CONFIG_PARAM_SAMP_FREQ    = 1,
  XA_SBC_DEC_CONFIG_PARAM_NUM_CHANNELS = 2,
  XA_SBC_DEC_CONFIG_PARAM_BITRATE      = 3
};

/* commands */
#include "xa_apicmd_standards.h"

/* sbc_dec-specific commands */
/* (none) */

/* sbc_dec-specific command types */
/* (none) */

/* error codes */
#include "xa_error_standards.h"

#define XA_CODEC_SBC_DEC	17

/* sbc_dec-specific error codes */
/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
/* (none) */

/* Fatal Errors */
/* (none) */

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/
/* Nonfatal Errors */
enum xa_error_nonfatal_execute_sbc_dec {
  XA_SBC_DEC_EXECUTE_NONFATAL_BAD_SBC_FRAME  = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_SBC_DEC, 0),
  XA_SBC_DEC_EXECUTE_NONFATAL_CRC_ERROR      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_SBC_DEC, 1),
  XA_SBC_DEC_EXECUTE_NONFATAL_BAD_BLOCK      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_SBC_DEC, 2),
  XA_SBC_DEC_EXECUTE_NONFATAL_NEED_MORE_DATA = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_SBC_DEC, 3)
};

/* Fatal Errors */
enum xa_error_fatal_execute_sbc_dec {
  XA_SBC_DEC_EXECUTE_FATAL_INVALID_STREAM = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_SBC_DEC, 0)

};

#if 1
#include "xa_type_def.h"
#else
#ifndef xa_codec_func_t
typedef struct xa_codec_opaque { } *xa_codec_handle_t;

typedef XA_ERRORCODE xa_codec_func_t(xa_codec_handle_t p_xa_module_obj,
									 signed int     i_cmd,
									 signed int     i_idx,
									 void          *pv_value);
#endif
#endif

#if defined(__cplusplus)
extern "C" {
#endif	/* __cplusplus */

//#undef CODEC_LIB_IN_MASK_ROM
#if 1//
xa_codec_func_t xa_sbc_dec;
#else
#define xa_sbc_dec ROM_SBC_DECODER_ADDR
#endif

#if defined(__cplusplus)
}
#endif	/* __cplusplus */
#endif /* __XA_SBC_DEC_API_H__ */
