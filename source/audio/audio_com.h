#ifndef _AUDIO_COM_H_
#define _AUDIO_COM_H_

#include "fifo.h"


#define STREAM_BUFFER_USED_POINTER //need to change the elf32xtensa.x file

#define STREAM_BUFFER_SIZE_UNFIXED


#define STREAM_BUF_MAX_SIZE    (1024*4)


typedef struct {
	FIFO stream_fifo;

	U8 playing_stream_status;
	U32 playing_stream_bitrate;
	U32 playing_stream_sample_rate;
	U8 playing_stream_chans;
	U8 playing_stream_sample_percent;
	U8 playing_stream_sample_bits;

	U32 playing_index	
} AUDIO_ENV_S;


extern AUDIO_ENV_S audio_env;


#ifdef STREAM_BUFFER_USED_POINTER
extern U8 STREAM_BUFFER_START[];
extern U8 STREAM_BUFFER_END[];
extern U8 *stream_buffer;
#else
extern U8 stream_buffer[STREAM_BUF_MAX_SIZE];
#endif

void audio_com_init(void);

#define AUDIO_FIFO_STREAM_OPEN(stream_buffer,len)    fifo_init(&audio_env.stream_fifo,stream_buffer,len,TRUE)

#define AUDIO_FIFO_STREAM_FLUSH()                    fifo_file_stream_flush(&audio_env.stream_fifo)

#define AUDIO_FIFO_STREAM_DISCARD_ALL_DATA()         fifo_discard_all_data(&audio_env.stream_fifo)
#define AUDIO_FIFO_STREAM_DISCARD_DATA(len)          fifo_discard_data(&audio_env.stream_fifo,len)

#define AUDIO_FIFO_STREAM_CREATE(start_pos,stream_len,entry,stream_margin) \
                                               fifo_create_file_stream(&audio_env.stream_fifo,start_pos,stream_len,entry,stream_margin)

#define AUDIO_FIFO_STREAM_FIFO_LEN()           audio_env.stream_fifo.fifo_length    //buf len

#define AUDIO_FIFO_STREAM_STREAM_LEN()         audio_env.stream_fifo.stream_len    //steam len(file len)

#define AUDIO_FIFO_STREAM_FIFO_RLEN()          audio_env.stream_fifo.rLen

#define AUDIO_FIFO_STREAM_FIFO_WLEN()          audio_env.stream_fifo.wLen

#define AUDIO_FIFO_STREAM_DATA_LEN()           fifo_get_data_len(&audio_env.stream_fifo)

#define AUDIO_FIFO_STREAM_FREE_LEN()           fifo_get_free_space(&audio_env.stream_fifo)

#define AUDIO_FIFO_STREAM_GET_DATA(buffer,len) fifo_get_data(&audio_env.stream_fifo, (U8 *)buffer, len)

#define AUDIO_FIFO_STREAM_IS_VAILD()           audio_env.stream_fifo.valid

#define AUDIO_FIFO_STREAM_PUT_DATA(ptr,len)    fifo_put_data (&audio_env.stream_fifo, ptr, len)

#define AUDIO_FIFO_STREAM_SEEK(pos)            fifo_file_stream_seek (&audio_env.stream_fifo,pos)



enum {
	SAMPLE_RATE_32K = 32000,
	SAMPLE_RATE_44P1K = 44100,
	SAMPLE_RATE_48K = 48000,
	SAMPLE_RATE_64K = 64000,
	SAMPLE_RATE_88P2K = 88200,
	SAMPLE_RATE_96K = 96000,
	SAMPLE_RATE_128K = 128000,
	SAMPLE_RATE_176P4K = 176400,
	SAMPLE_RATE_192K = 192000,
	SAMPLE_RATE_196K = 196000,
};


int audio_stream_speed_get(U32 sample_rate);
void audio_clock_freq_set(U32 stream_srf);

#endif //_AUDIO_COM_H_

