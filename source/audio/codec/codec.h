#ifndef _CODEC_H_
#define _CODEC_H_

#include "codec_malloc.h"


#define	CO_PROCESSOR_MASK_BITS      0x03

#define MAX_CODEC_MEM_SIZE          0x34000 //only for dd plus test


typedef struct{
	U8 valid;
	U8 busy;
	U16 bytes;
} SECOND_OUTPUT;

extern SECOND_OUTPUT second;

extern U8 wNoMoreFrames;
extern U16 decoder_buf[];

extern U32 codec_malloc_used;

extern U8 *spdif_parser_buf;
extern U16 spdif_parsed_data_size;
extern U16 spdif_parsed_data_index;

extern char codec_enc_in_buf[];
extern unsigned long codec_read_len;
extern char *pAudioOrginalOut;


static __inline__ void enableCoProcessor( unsigned int mask )
{
	__asm__ __volatile__ (
		"wsr.cpenable   %0 \n\t"
		"esync\n\t"
		:
		: "a" (mask)
		);
}


long xa_ftell(void *ptr);
int xa_fseek( void *stream, long offset, int origin );
int xa_fread(void *buffer, int size, int count, void *fp);
int xa_fread_bt(void *buffer, int size, int count, void *fp);

int Decode_Wait_EnoughSpace(void);
int Decode_OutputBuffer(U32 *buffer,int out_bytes);
int Decode_ClearOutputBuffer(int out_bytes);

U32 codec_get_mem_used(void);

int audio_data_check_out_status(int *pSrc, int *pDest, int byte_size);
int audio_data_byte_check_out_status(char *pSrc, char *pDest, int byte_size);
int audio_data_convert_from_32bit_to_24bit(int *pSrc, int byte_size);

int xa_audio_parameter_get(U32 sample_rate, U32 input_channel, U32 output_channel);
int optek_lib_dsp_dd_ac3_set(int val);
int optek_lib_dsp_dd_ec3_set(int val);
int optek_lib_dsp_dts_set(int val);

void flash_libxa_ec3_dec_text_copy(void);
void flash_libxa_dts_dmp_dec_text_copy(void);

#endif //_CODEC_H_
