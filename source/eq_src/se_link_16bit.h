/*
 * se_link_16bit.h
 *
 *  Created on: 2020��5��17��
 *      Author: Administrator
 *
 * @brief   CCHIP EQ
 * @author  CCHIP WuHan EQ
 * @version V8.3
 *
 */
#ifndef __SE_LINK_API_H__
#define __SE_LINK_API_H__

#ifdef DSP_EQ_SETTING_16BIT_DECODE
#include "oem.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef  __cplusplus
extern "C"
{
#endif//__cplusplus

/**
 * @brief  Initialize EQ for MUSIC/MIC
 * @return
 */
void eq_init(int sample_rate, int in_channel, int out_channel);

/**
 * @brief  Uninitialize
 * @return
 */
void eq_uninit(void);

/**
 * @brief  EQ PROCESS
 * @param [in] src <tt>short</tt>: 2 channels src pcm data.(LRLRLRLR)
 * @param [out] dst <tt>short</tt>:             2 channels
 * @return
 */
void eq_process(short *src, short *dst, int sample_rate, int in_channel, int byte_len);

#ifdef  __cplusplus
}
#endif

#endif//__cplusplus

#endif /*  */
