/*
 * sona_dsp.h
 *
 *  Created on: 2024/9/19
 *      Author: zero
 */

#ifndef __SONA_DSP_H__
#define __SONA_DSP_H__

extern int *p_sona_temp_src;
extern int *p_sona_temp_dest;
extern int *p_sona_temp_inbuffer;
extern int *p_sona_temp_outbuffer;
extern int *p_sona_temp_buf;

extern int sona_dsp_malloc_print_size;
extern int sona_dsp_L2_malloc_print_size;

extern int DspSonaInit_en;

void dsp_sona_module_init (void);
void sona_dsp_malloc_buf_init (void);
void sona_dsp_buf_free(void);

void dsp_sona_module_24bit_process (int *pSrc, int in_channel, int in_byte_len, int *pDest, int out_channel, int *out_byte_len);
void Updata_sona_info(void);


//for expanding from 24bit 2-channel to 4-channel
void audio_data_process_24bit_2ch_in_to_4ch_out (int *pDestBuf, int *pSrcBuf, int byte_len);
void audio_data_process_24bit_4ch_in_to_4ch_out(int *pDestBuf,int* pSrcBuf,int byte_len);

#endif /* __SONA_DSP_H__ */
