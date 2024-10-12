/*
 * se_link_24bit.h
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

#include "oem.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifdef DSP_EQ_SETTING_24BIT_DECODE
#ifdef  __cplusplus
extern "C"
{
#endif//__cplusplus

/**
 * @brief  Initialize EQ for MUSIC/MIC
 * @return
 */
// void eq_init();
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
// void eq_process(short *src, short *dst);
void eq_process(int *src, int *dst, int sample_rate, int in_channel, int byte_len);

void eq_process_16bit(int *src, int *dst, int sample_rate, int in_channel, int byte_len);

void eq_process_sub(int *src, int *dst, int sample_rate, int in_channel, int byte_len);
void wheq_vol_set(int vol);

extern int *se_link_buf_in;
extern int *se_link_buf_out;

extern int *se_link_sub_buf_in;
extern int *se_link_sub_buf_out;


#ifdef  __cplusplus
}
#endif//__cplusplus
#endif

#endif /*  */
