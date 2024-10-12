#ifndef __TOOLS_H__
#define __TOOLS_H__


//for example sample: sarmple rate 192kHz to 48kHz
void audio_data_down_sample_from_4x_to_1x (U32 *pBuf, U32 byte_len);

//for example sample: sarmple rate 96kHz to 48kHz
void audio_data_down_sample_from_2x_to_1x (U32 *pBuf, U32 byte_len);

//for expanding from 2 2-channel to 4-channel
void audio_data_process_2_2ch_in_to_4ch_out (int *pDestBuf, int *pSrcBuf, int *pSrcBuf_1, U32 byte_len);

//for example sample: 2 channels to 4 channels
void audio_data_process_2ch_in_to_4ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len);

//for example sample: 2 channels to 8 channels
void audio_data_process_2ch_in_to_8ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len);

//for example sample: 4 channels to 8 channels
void audio_data_process_4ch_in_to_8ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len);

//for example sample: 6 channels to 8 channels
void audio_data_process_6ch_in_to_8ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len);

//for example sample: 2 channels to 10 channels
void audio_data_process_2ch_in_to_10ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len);

//for example sample: 2 channels to 12 channels
void audio_data_process_2ch_in_to_12ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len);

//for example sample: 4 channels to 14 channels
void audio_data_process_4ch_in_to_14ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len);

//for example sample: 6 channels to 16 channels
void audio_data_process_6ch_in_to_16ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len);



int audio_out_data_channel_switch(U32 *pSrc, U32 *pDest, U32 in_sample_speed, U32 in_channel, 
                           U32 in_len, U32 *out_byte_len);

//convert intgers(decimal) to strings
void itoa_dec( int val, char *string );

//convert string to interger(hexadecimal)
int atoi_hex( char *string, long *val);

//convert string to interger(decimal)
int atoi_dec( char *string, long *val);

void  cpu_clock_frequency_check_out(U32 interval_time_ms);

#endif //__TOOLS_H__
