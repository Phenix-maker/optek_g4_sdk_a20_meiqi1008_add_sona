#ifndef __AUDIO_OUTPUT_H__
#define __AUDIO_OUTPUT_H__


//#define OUTPUT_USED_FIFO

#ifdef OUTPUT_USED_FIFO
//128*2*2 -> 16bit 2ch output, 128*2*8 -> 16bit 8ch output
#define OUT_FRAME_SIZE    (128*2*8) //16bit 8ch output

#ifdef ATMOS_CODEC_CONFIG_XML_BY_OPTEK
#define OUT_BUFFER_SIZE    (OUT_FRAME_SIZE*52)
#else
#define OUT_BUFFER_SIZE    (OUT_FRAME_SIZE*56)
#endif //ATMOS_CODEC_CONFIG_XML_BY_OPTEK

#endif //OUTPUT_USED_FIFO


void audio_output_init(void);
void audio_output_start(U32 *p_data, U32 size);
void audio_output_put_next_frame_dataPtr(U32 *p_data, U32 size);
U32 audio_output_ready_to_receive_dataPtr(void);

U32 output_fifo_free_space(void);
void output_fifo_put_data(U8 *pBuf, unsigned long len);


#endif //__AUDIO_OUTPUT_H__
