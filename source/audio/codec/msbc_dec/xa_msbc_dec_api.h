/*
 * Copyright (c) 2012-2016 by Cadence Design Systems, Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of
 * Cadence Design Systems Inc.  They may be adapted and modified by bona fide
 * purchasers for internal use, but neither the original nor any adapted
 * or modified version may be disclosed or distributed to third parties
 * in any manner, medium, or form, in whole or in part, without the prior
 * written consent of Cadence Design Systems Inc.  This software and its
 * derivatives are to be executed solely on products incorporating a Cadence
 * Design Systems processor.
 */


#ifndef __XA_MSBC_DEC_API_H__
#define __XA_MSBC_DEC_API_H__

/*****************************************************************************/
/* SBC Decoder specific API definitions                                      */
/*****************************************************************************/

/* sbc_dec-specific configuration parameters */
enum xa_config_param_msbc_dec {
  XA_MSBC_DEC_CONFIG_PARAM_PCM_WDSZ     = 0,
  XA_MSBC_DEC_CONFIG_PARAM_SAMP_FREQ    = 1,
  XA_MSBC_DEC_CONFIG_PARAM_NUM_CHANNELS = 2,
  XA_MSBC_DEC_CONFIG_PARAM_BITRATE      = 3,
  XA_MSBC_DEC_CONFIG_PARAM_PLC_OPTION   = 4,
  XA_MSBC_DEC_CONFIG_PARAM_FRAME_TYPE   = 5
};

typedef enum
{
    XA_MSBC_BAD_FRAME  = 0,
    XA_MSBC_GOOD_FRAME = 1
}xa_msbc_frame_type_t;


/* commands */
#include "xa_apicmd_standards.h"

/* msbc_dec-specific commands */
/* (none) */

/* msbc_dec-specific command types */
/* (none) */

/* error codes */
#include "xa_error_standards.h"

#define XA_CODEC_MSBC_DEC	18

/* msbc_dec-specific error codes */
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
enum xa_error_nonfatal_execute_msbc_dec {
  XA_MSBC_DEC_EXECUTE_NONFATAL_BAD_MSBC_FRAME = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_MSBC_DEC, 0),
  XA_MSBC_DEC_EXECUTE_NONFATAL_CRC_ERROR      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_MSBC_DEC, 1),
  XA_MSBC_DEC_EXECUTE_NONFATAL_BAD_BLOCK      = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_MSBC_DEC, 2),
  XA_MSBC_DEC_EXECUTE_NONFATAL_NEED_MORE_DATA = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_execute, XA_CODEC_MSBC_DEC, 3)
};

/* Fatal Errors */
enum xa_error_fatal_execute_msbc_dec {
  XA_MSBC_DEC_EXECUTE_FATAL_INVALID_STREAM = XA_ERROR_CODE(xa_severity_fatal, xa_class_execute, XA_CODEC_MSBC_DEC, 0)
};
#if 1
#include "xa_type_def.h"
#else
typedef struct xa_codec_opaque { WORD32 _; } *xa_codec_handle_t;

//typedef int XA_ERRORCODE;

#define PLATFORM_INLINE __inline

typedef XA_ERRORCODE xa_codec_func_t(xa_codec_handle_t p_xa_module_obj,
				     WORD32            i_cmd,
				     WORD32            i_idx,
				     pVOID             pv_value);
#endif



#if defined(__cplusplus)
extern "C" {
#endif	/* __cplusplus */

#if 1
xa_codec_func_t xa_msbc_dec;
#else
#define xa_msbc_dec ROM_MSBC_DECODER_ADDR
#endif
#if defined(__cplusplus)
}
#endif	/* __cplusplus */
#endif /* __XA_MSBC_DEC_API_H__ */
