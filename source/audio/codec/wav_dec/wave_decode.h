#ifndef WAVE_DECODE_H
#define WAVE_DECODE_H


#define AUDIO_OUTPUT_24BIT

#define WAV_192K_SUPPORT
#define AUDIO_192K_DOWN_SAMPLE

#define AUDIO_176P4K_DOWN_SAMPLE
#define AUDIO_128K_DOWN_SAMPLE

#define WAV_96K_SUPPORT
#define AUDIO_96K_DOWN_SAMPLE


#define AUDIO_192K_DOWN_48K_SAMPLE
#define AUDIO_176P4K_DOWN_44P1K_SAMPLE
#define AUDIO_128K_DOWN_32K_SAMPLE


//the macro below is used AUDIO_88P2K_DOWN_44P1K_SAMPLE or AUDIO_64K_DOWN_32P1K_SAMPLE, too
//#define AUDIO_DOWN_SAMPLE

#define WAVE_DECODE_OUTPUT_BYTES    (AUDIO_OUT_BUF_SIZE*2)


#ifdef AUDIO_192K_DOWN_48K_SAMPLE

#define WAV_192K_24BIT_DEC_OUT_BYTE                 (AUDIO_OUT_BUF_SIZE*4)
#define WAV_96K_24BIT_DEC_OUT_BYTE                  (AUDIO_OUT_BUF_SIZE*2)
#define WAV_48K_24BIT_DEC_OUT_BYTE                  (AUDIO_OUT_BUF_SIZE*1)

#define WAV_192K_24BIT_TO_48K_16BIT_DEC_OUT_BYTE    (AUDIO_OUT_BUF_SIZE*4*2)
#define WAV_96K_24BIT_TO_48K_16BIT_DEC_OUT_BYTE     (AUDIO_OUT_BUF_SIZE*4)

#define WAV_192K_24BIT_TO_16BIT_DEC_OUT_BYTE        (AUDIO_OUT_BUF_SIZE*4)
#define WAV_96K_24BIT_TO_16BIT_DEC_OUT_BYTE         (AUDIO_OUT_BUF_SIZE*2)

#define WAV_192K_16BIT_TO_48K_16BIT_DEC_OUT_BYTE    (AUDIO_OUT_BUF_SIZE*4*2)
#define WAV_96K_16BIT_TO_48K_16BIT_DEC_OUT_BYTE     (AUDIO_OUT_BUF_SIZE*4)

#define WAV_192K_16BIT_DEC_OUT_BYTE                 (AUDIO_OUT_BUF_SIZE*4)
#define WAV_96K_16BIT_DEC_OUT_BYTE                  (AUDIO_OUT_BUF_SIZE*2)

#else

#ifdef AUDIO_192K_DOWN_SAMPLE
#define WAV_192K_24BIT_DEC_OUT_BYTE                 (AUDIO_OUT_BUF_SIZE*2) //1152*2
#define WAV_96K_24BIT_DEC_OUT_BYTE                  (AUDIO_OUT_BUF_SIZE*2) //1152*2
#define WAV_48K_24BIT_DEC_OUT_BYTE                  (AUDIO_OUT_BUF_SIZE*1) //1152*1

#define WAV_192K_24BIT_TO_48K_16BIT_DEC_OUT_BYTE    (AUDIO_OUT_BUF_SIZE*4*2)
#define WAV_96K_24BIT_TO_48K_16BIT_DEC_OUT_BYTE     (AUDIO_OUT_BUF_SIZE*4)

#define WAV_192K_24BIT_TO_16BIT_DEC_OUT_BYTE        (AUDIO_OUT_BUF_SIZE*4)
#define WAV_96K_24BIT_TO_16BIT_DEC_OUT_BYTE         (AUDIO_OUT_BUF_SIZE*2)

#define WAV_192K_16BIT_DEC_OUT_BYTE                 (AUDIO_OUT_BUF_SIZE*4)
#define WAV_96K_16BIT_DEC_OUT_BYTE                  (AUDIO_OUT_BUF_SIZE*2)

#else

#define WAV_192K_24BIT_DEC_OUT_BYTE                 (AUDIO_OUT_BUF_SIZE*2) //1152*2
#define WAV_96K_24BIT_DEC_OUT_BYTE                  (AUDIO_OUT_BUF_SIZE*2) //1152*2
#define WAV_48K_24BIT_DEC_OUT_BYTE                  (AUDIO_OUT_BUF_SIZE*2) //1152*2


#define WAV_192K_24BIT_TO_16BIT_DEC_OUT_BYTE        (AUDIO_OUT_BUF_SIZE*2)
#define WAV_96K_24BIT_TO_16BIT_DEC_OUT_BYTE         (AUDIO_OUT_BUF_SIZE*2)


#define WAV_192K_16BIT_DEC_OUT_BYTE                 (AUDIO_OUT_BUF_SIZE*2)
#define WAV_96K_16BIT_DEC_OUT_BYTE                  (AUDIO_OUT_BUF_SIZE*2)

#endif



#endif



/*****************************************************************************************
WAV header structure
*****************************************************************************************/

typedef struct 
{
    // RIFF header
    char cRIFFHeader[4];		/* "RIFF" */
    unsigned int nRIFFBytes;	/* pcmbytes + 44 - 8 , length in bytes without header */

    // data type
    char cDataTypeID[4];		/* "WAVE" */

    // wave format
    char cFormatHeader[4];	/* "fmt " */
    unsigned int nFormatBytes;	/* 2 + 2 + 4 + 4 + 2 + 2, length of PCM format decl area */

    unsigned short nFormatTag;	/* is pcm? */
    unsigned short nChannels;	/* 2 */
    unsigned int nSamplesPerSec;	/* 44100 */
    unsigned int nAvgBytesPerSec;	/*    44100 * 2 * ((bits + 7) / 8)   */
    unsigned short nBlockAlign;		/*    channels * ((bits + 7) / 8)   */
    unsigned short nBitsPerSample;	/*    bits = 16		*/
    
    // data chunk header
    char cDataHeader[4];			/*	"data"	*/
    unsigned int nDataBytes;		/*	pcmbytes	*/
} __attribute__((packed)) WAVE_HEADER;

typedef enum{
	WAVE_SUCCESS,
	WAVE_HEADER_ERR,
	WAVE_NOT_PCM,
	WAVE_READ_ERR,

	WAVE_STATUS_LAST
} WAVE_STATUS;


typedef struct{
	U8 *out_buf;

	U8 nBlockAlign;
	U32 bytes;
	U32 data_pos;
} WAVE_DECODE_DATA;


extern U8 wav_jump_adjust;
extern WAVE_DECODE_DATA *pwave;

U32 app_dec_wave_adjust_pos (U32 pos);

#endif

