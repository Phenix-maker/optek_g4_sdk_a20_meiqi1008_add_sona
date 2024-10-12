#ifndef _SPDIF_DATA_PARSE_C_
#define _SPDIF_DATA_PARSE_C_


#define SYNC_WORD_SWAP


enum
{
	enSPDIF_DATA_ENCODING,//NO PCM
	enSPDIF_DATA_PCM      //PCM
};


//for AAC(ADTS) DATA TYPE
typedef enum
{
	SPDIF_STREAM_UNKNOWN,
	SPDIF_STREAM_AC3,
	SPDIF_STREAM_AAC,
	SPDIF_STREAM_PCM,
	SPDIF_STREAM_EC3,
	SPDIF_STREAM_MAT,
	SPDIF_STREAM_THD,//mlp etc.

	SPDIF_STREAM_DTS,
	SPDIF_STREAM_DTS_TYPE1 = SPDIF_STREAM_DTS,
	SPDIF_STREAM_DTS_TYPE2,
	SPDIF_STREAM_DTS_TYPE3,
	SPDIF_STREAM_DTS_TYPE4,

	SPDIF_STREAM_UNLOCK,
	SPDIF_STREAM_TEMP,
	SPDIF_STREAM_LAST,
} enSPDIF_STREAM_TYPE;

//AAC DATA TYPE:7
enum {
	MPEG2_AAC_RESERVE_SAMPLE_RATE = 0,      //32 frames, 1024 samples of frames
};

//AAC DATA TYPE:19
//for AAC(ADTS) STREAM SUBDATA TYPE
enum {
	MPEG2_AAC_HALF_LOW_SAMPLE_RATE = 0,     //64 frames, 2048 samples of frames
	MPEG2_AAC_QUARTER_LOW_SAMPLE_RATE = 1,  //128 frames, 4096 samples of frames
	MPEG2_AAC_RESERVE1_LOW_SAMPLE_RATE = 2,
	MPEG2_AAC_RESERVE2_LOW_SAMPLE_RATE = 3
};

//AAC DATA TYPE:20
//for AAC(ADTS) STREAM SUBDATA TYPE
enum {
	MPEG4_AAC_NORMAL = 0,                  //32 frames, 1024 samples of frames
	MPEG4_AAC_HALF_LOW_SAMPLE_RATE = 1,    //64 frames, 2048 samples of frames
	MPEG4_AAC_QUARTER_LOW_SAMPLE_RATE = 2, //128 frames, 4096 samples of frames
	MPEG4_AAC_DOUBLE_HIGH_SAMPLE_RATE = 3, //16 frames, 512 samples of frames
};


//Sync dword 72F8,1F4E:72F81F4E, get from spdif rx

//Sync word 1
//72,F8:72F8 
#define PREAMBLE_PA     0xF872

//Sync word 2
//1F,4E:1F4E
#define PREAMBLE_PB     0x4E1F

//72,F8,1F,4E:72F81F4E
//#define PREAMBLE_PAPB   0xF8724E1F
//#define PREAMBLE_PAPB_R   0x4E1FF872


//Burst-info
//Data type
//Bits of PC: 4-0
#define PREAMBLE_PC_AC3               1


#define PREAMBLE_PC_MPEG1_LAY_1       4 //frame size:384
#define PREAMBLE_PC_MPEG1_LAY_2_or_3  5 //frame size:1152
#define PREAMBLE_PC_MPEG2             6 //frame size:1152

#define PREAMBLE_PC_MPEG2_AAC1        7 //frame size:1024

#define PREAMBLE_PC_MPEG2_LAYER_1     8 //frame size, 768,low sampling frequency
#define PREAMBLE_PC_MPEG2_LAYER_2     9 //frame size, 2304,low sampling frequency
#define PREAMBLE_PC_MPEG2_LAYER_3    10 //frame size, 1152,low sampling frequency


#define PREAMBLE_PC_DTS              11
#define PREAMBLE_PC_DTS_TYPE_1       PREAMBLE_PC_DTS
#define PREAMBLE_PC_DTS_TYPE_2       12
#define PREAMBLE_PC_DTS_TYPE_3       13

#define PREAMBLE_PC_ATRAC            14
#define PREAMBLE_PC_ATRAC_2_OR_3     15
#define PREAMBLE_PC_ATRAC_X          16

#define PREAMBLE_PC_DTS_TYPE_4       17

#define PREAMBLE_PC_WMA              18

#define PREAMBLE_PC_MPEG2_AAC2_0     19

#define PREAMBLE_PC_MPEG4_AAC        20

#define PREAMBLE_PC_EC3              21

//for DD ATMOS
#define PREAMBLE_PC_MAT              22

//for DD ATMOS
#define PREAMBLE_PC_MLP              22




//Sub data type
//Bits of PC: 6-5

//11
#define PREAMBLE_PC_SUB_DTS_TYPE_1      (0<<5)

//12
#define PREAMBLE_PC_SUB_DTS_TYPE_2      (0<<5)

//13
#define PREAMBLE_PC_SUB_DTS_TYPE_3      (0<<5)

//14
#define PREAMBLE_PC_SUB_DTS_TYPE_4      (0<<5)

//15
#define PREAMBLE_PC_SUB_ATRAC           (0<<5)

//16
#define PREAMBLE_PC_SUB_ATRAC_2_OR_3    (0<<5)

//17
#define PREAMBLE_PC_SUB_ATRAC_X         (0<<5)

//18
#define PREAMBLE_PC_SUB_WMA_TYPE_1      0
#define PREAMBLE_PC_SUB_WMA_TYPE_2      1
#define PREAMBLE_PC_SUB_WMA_TYPE_3      2
#define PREAMBLE_PC_SUB_WMA_TYPE_4      3

//19
#define PREAMBLE_PC_SUB_MPEG2_AAC_0          0 //BIT4-BIT0, for low sample frequency
#define PREAMBLE_PC_SUB_MPEG2_AAC_1          1 //BIT4-BIT0, for low sample frequency
#define PREAMBLE_PC_SUB_MPEG2_AAC_2_RSVD     2 //BIT4-BIT0, reserved
#define PREAMBLE_PC_SUB_MPEG2_AAC_3_RSVD     2 //BIT4-BIT0, reserved

//20
#define PREAMBLE_PC_SUB_MPEG4_AAC_0     (0<<5) //frame size:1024 
#define PREAMBLE_PC_SUB_MPEG4_AAC_1     (1<<5) //frame size:2048
#define PREAMBLE_PC_SUB_MPEG4_AAC_2     (2<<) //frame size:4096
#define PREAMBLE_PC_SUB_MPEG4_AAC_3     (3<<5) //frame size:512

//21
#define PREAMBLE_PC_SUB_EC3_0           (0<<5)
#define PREAMBLE_PC_SUB_DTS_1           (0<<5) //reserved
#define PREAMBLE_PC_SUB_DTS_2           (0<<5) //reserved
#define PREAMBLE_PC_SUB_DTS_3           (0<<5) //reserved

//22
#define PREAMBLE_PC_SUB_MAT_0           (0<<5)
#define PREAMBLE_PC_SUB_MAT_1           (0<<5) //reserved
#define PREAMBLE_PC_SUB_MAT_2           (0<<5) //reserved
#define PREAMBLE_PC_SUB_MAT_3           (0<<5) //reserved


//Number of bits or number of
//bytes according to data-type
//PD


void spdif_stream_preProcess_init(void);



/**
 ****************************************************************************************
 * @brief Analyze IIS or SPDIF streams and extract valid audio data
 *
 * @param[in]   inpBuf     Pointer to the start of the input buffer data
 * @param[in]   inpSize    Size, in bytes, of the input data.
 * @param[out]  outpBuf    Pointer to the start of the output data buffer.
 * @param[out]  outpSize   Pointer to the variable that stores the size, in bytes
 * @param[out]  consumed   Pointer to the variable that stores the amount, in bytes, of input data processed.
 * @return The spdif stream type
 *
 ****************************************************************************************
 */

enSPDIF_STREAM_TYPE spdif_stream_preProcess(U32 *inpBuf, U16 inpSize, U8 **outpBuf, U16 *outpSize, U16 *consumed);

U32 spdif_stream_frame_payload_byte_size(void);
U8 spdif_stream_is_bigEndian(void);


const char *optek_spdif_parse_version_get (void);
const char *app_spdif_parse_data_get(void);
const char *app_spdif_parse_time_get(void);


#endif //_SPDIF_DATA_PARSE_C_
