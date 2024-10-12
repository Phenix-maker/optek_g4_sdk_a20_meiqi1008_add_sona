/*
 * Copyright (c) 2019-2020 Cadence Design Systems, Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "os_config.h"

#include <xtensa/tie/xt_hifi2.h>
#include "c_def.h"
#include "debug.h"
#include "oem.h"


#ifdef LC3_DECODE_ENABLE

//#include "regmap.h"


/* turn off stdlib function warnings in visual studio */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define DISABLE_PLC_API

#include <string.h>
#include <stdlib.h>
#include "xa_lc3_codec_api.h"
#include "xa_error_handler.h"
//#include "tinywaveout_c.h"
#include "assert.h"

//#include "audio_dec.h"

#include "app_main.h"
#include "mem_reloc.h"

#include "audio_com.h"

#include "audio_codec.h"
#include "audio_task.h"

#if 0
extern U32 __lc3_dec_text_rom_start[];
extern U32 __lc3_dec_text_ram_start[];
extern U32 __lc3_dec_text_ram_end[];

//int deocde_text_buf[17*1024/4] __attribute__(( aligned (64) ));
int *p_deocde_text;
extern U8 malloc_buf[0x3600+0x1000+0x500];	
static U32 text_len;
int app_lc3_decoding_code_text_copy_init(void)
{
	int len;
	p_deocde_text = &malloc_buf[0x600];
	text_len = 17*1024;//(__lc3_dec_text_ram_end - __lc3_dec_text_ram_start)*4;
	CFasm_memcpy(p_deocde_text, __lc3_dec_text_rom_start,text_len);
    app_main_data.iram_code_flag = SHARE_IRAM_UNKNOW_CODE;
    //text_len >>= 2;
    //text_len >>= 1;
    //text_len >>= 4;
}
void dsp_memcpy(int *dst,const int *src,int dint_len);
int app_lc3_decoding_code_text_copy(void) __INTERNAL_RAM_TEXT;


int app_lc3_decoding_code_text_copy(void)
{
#if 1 
    if (app_main_data.iram_code_flag != SHARE_IRAM_LC3_DECODE_CODE && app_main_data.iram_code_flag != SHARE_IRAM_BT_CONN_CODE)
    {
        CFasm_memcpy(__lc3_dec_text_ram_start, p_deocde_text,text_len);
        app_main_data.iram_code_flag = SHARE_IRAM_LC3_DECODE_CODE;   
    }
//#else
    //dsp_memcpy(deocde_text_buf,deocde_text_buf,text_len);
#endif    
 #if 0
    int i;
    int *pOut = __lc3_dec_text_ram_start;
    int *pIn = deocde_text_buf;
    for (i=0;i<text_len;i++)
    {
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;                                        
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;

        *pOut++ = *pIn++;
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;                                        
        *pOut++ = *pIn++;
        *pOut++ = *pIn++;
    }
#endif    
}
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define ASSERT(test) assert(test)

//#define SHOW_SIZES
#define INIT_SYMBOL	0

#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define PARAMFILE "paramfilesimple_decode.txt"

/*****************************************************************************/
/* Error codes for the testbench                                             */
/*****************************************************************************/
#define XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED                0xFFFF8000
#define XA_TESTBENCH_FATAL_FILE_OPEN_FAILED                0xFFFF8001

#ifdef OUTPUT_BUF_PING_PONG_TEST
pWORD32  pb_out_buf_ping = NULL;
pWORD32  pb_out_buf_pong = NULL;
WORD32 pb_out_buf_pp_count = 0;
#endif

#if 0//def __XTENSA__
#define __PROF_ALLOCATE__
#ifndef __MCPS_MEASUREMENT__
#define __MCPS_MEASUREMENT__
#endif
#endif

#ifdef __MCPS_MEASUREMENT__
#include <xtensa/tie/xt_hifi2.h>
#include <sys/times.h>
#include <xtensa/sim.h>
/* Following variable shows the frame number that will be profiled */
/* Set profile_frame = 0 to profile all frames */
#endif

#ifdef __PROF_ALLOCATE__
#include <xtensa/xt_profiling.h>
#define PROFILER_START {xt_profile_enable();}
#define PROFILER_STOP  {xt_profile_disable();}

#define PROFILER_FRAME   -1// -1 - all w/o stop for get mcps
#else
#define PROFILER_START
#define PROFILER_STOP
#endif

#ifdef __MCPS_MEASUREMENT__

double u_seconds;
struct tms start, stop;

struct mcpsAlgType {
    clock_t cycles;
    double mcps_peak;
    double mcps_curr;
    double mcps_ave;
    double mcps_sum;
    WORD32 peak_frame;
    const pWORD8 nameAlg;
};

static struct mcpsAlgType mcpsforAlgs[] = {
    { 0, 0, 0, 0, 0, 0, "lc3_dec" }
};

#endif


/* struct to hold command line arguments */
typedef struct
{
    pWORD8 inputFilename;
    pWORD8 outputFilename;
    WORD32 bitrate;
    WORD32 bipsOut;
    WORD32 dc; //delay compensation
    float  frame_ms;
    WORD32 hide_counter;
    WORD32 plcMeth;
    pWORD8 epf; //
    int    epmode;
    int    hrmode;
    pWORD8 edf; //
    
	U16 enc_frame_bytes;
} Arguments_dec;

/* local helper functions */
static VOID parseCmdl_dec(WORD32 ac, pWORD8* av, Arguments_dec* arg);
static VOID exit_if(WORD32 condition, const pWORD8 message);
VOID interleave(pWORD32 in, pWORD16 out, WORD32 n, WORD32 channels, WORD32 bipsout);
#if 0
static FILE *open_bitstream_reader(const char *file, UWORD32 *samplerate, WORD32 *bitrate, WORD16 *channels,UWORD32 *signal_len, float *frame_ms, int *epmode,int *hrmode);
WORD32 read_bitstream_frame(FILE* bitstream_file, pUWORD8 bytes, WORD32 size);
WORD16 loopy_read16(FILE* f);
static VOID safe_fclose(FILE* f);
#endif
VOID xa_lc3_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_lc3_error_info;
extern const xa_error_info_struct xa_testbench_error_info;


/*****************************************************************************/
/* Global variables                                                          */
/*****************************************************************************/

static const pWORD8 USAGE_MESSAGE_DEC =
"Usage: xa_lc3_dec_test [OPTIONS] <INPUT> <OUTPUT> \n"
"\n"
"  INPUT is bitstream file, OUTPUT is wav file.\n"
"\nOptions:\n"
"  -h           Show this message.\n"
"  -q           Hide frame counter.\n"
"\nCore codec options:\n"
"  -bps NUM     Output bits per sample. NUM must be 16 (default) or 24.\n"
"\nPLC options:\n"
"  -dc NUM                 0: Don't use delay compensation\n"
"                          1: Compensate delay in decoder (default)\n"
"                          2: Split delay equally between encoder and decoder\n"
"  -epf FILE    Enable packet loss simulation using error pattern from FILE.\n"
"  -edf FILE    Write error pattern as detected to FILE.\n"
"";

static const pWORD8 MISSING_ARGUMENT_MESSAGE = "Not enough parameters! Use -h to show help.";
static const pWORD8 INVALID_ARGUMENT_MESSAGE = "Incorrect Input parameters! Use -h to show help.";
/*
static WAVEFILEOUT* output_wav = NULL;
static FILE* input_bitstream = NULL;
static FILE* error_pattern_file = NULL;
static FILE* error_detection_file = NULL;
*/
/*******************************************************************************
*
*  Function name : xa_lc3_dec_main_process
*
*  Description   : 
*                  
*
*  parameters    : WORD32 argc (Arguments count)
*                  pWORD8 argv[] (Argument strings)
*
*  Return value  : none
******************************************************************************/

static xa_error_info_struct *p_proc_codec_err_info;

typedef struct
{
    pWORD32 sample_buf;
    pUWORD8 inp_buf;
    pWORD32 out_buf;
    WORD32 nSamples;
    xa_lc3_dec_init_cfg_t dec_init_cfg;    
    xa_codec_handle_t decoder_state;
    UWORD8 frame_bytes;    
} lc3_decode_handle;

//XA_ERRORCODE xa_lc3_dec_main_process(WORD32  argc, pWORD8 argv[])
//XA_ERRORCODE xa_lc3_dec_main_process_init(Arguments_dec *pArg_Dec)
XA_ERRORCODE xa_lc3_dec_handle_init(void **decode_handle,U32 bitrate,U16 samplerate,U8 channels,float frame_Time_ms)
{
    Arguments_dec arg;
    WORD32       delay = 0;
    //WORD32   sample_buf[XA_LC3_MAX_CHANNELS * XA_LC3_MAX_SAMPLES];
    //xa_error_info_struct *p_proc_codec_err_info = NULL;
    xa_error_info_struct *p_proc_testbench_err_info = NULL;
    WORD32      handle_size = 0, scratch_size = 0, inp_buf_size = 0, out_buf_size = 0;
    XA_ERRORCODE error_code = XA_NO_ERROR;
    //xa_lc3_dec_init_cfg_t dec_init_cfg;
    //xa_codec_handle_t decoder_state = NULL;
    pWORD32     scratch = NULL;
    //pUWORD8     inp_buf = NULL;
    //pWORD32     out_buf = NULL;
    lc3_decode_handle *handle;
    WORD32 samples;

    handle = codec_malloc(sizeof(lc3_decode_handle));

	p_proc_codec_err_info = NULL;
	
	handle->decoder_state = NULL;
	handle->inp_buf = NULL;
	handle->out_buf = NULL;

    if (frame_Time_ms == 2.5)
    {
        samples = 120;
    }
    else if (frame_Time_ms == 5.0)
    {
        samples = 240;
    }
    else if (frame_Time_ms == 7.5)
    {
        samples = 360;
    }
    else if (frame_Time_ms == 10.0)
    {
        samples = 480;
    }    
    else
    {
        DBG_Printf("frme time err\r\n");
        DBG_Assert(FALSE);
        samples = 120;
    }

    if (samplerate == 32000/2 || samplerate == 44100/2 || samplerate == 48000/2)
    {
        samples >>= 1;
    }

	handle->sample_buf = codec_malloc(channels * samples * 2);


    /* ******************************************************************/
    /* Initialize the error handler : testbench                         */
    /* ******************************************************************/
    p_proc_testbench_err_info = (xa_error_info_struct*)&xa_testbench_error_info;
    xa_testbench_error_handler_init();

    /* ******************************************************************/
    /* Initialize the error handler : lc3 codeclibrary                 */
    /* ******************************************************************/
    p_proc_codec_err_info = (xa_error_info_struct*)&xa_lc3_error_info;
    xa_lc3_error_handler_init();

    /* Library name version etc print */
/*    fprintf(stderr, "\n");
    fprintf(stderr, " %s library version %s\n",
        xa_lc3_get_lib_name_string(),
        xa_lc3_get_lib_version_string());
    fprintf(stderr, " API version: %s\n", xa_lc3_get_lib_api_version_string());
    fprintf(stderr, " Cadence, Inc. https://www.cadence.com\n");
    fprintf(stderr, "\n");
*/
    /************************************************/
    /* Get arguments                                */
    /************************************************/
	#if 0
    exit_if(argc <= 1, USAGE_MESSAGE_DEC);
	#endif

    /* Default settings */
    memset(&arg, 0, sizeof(arg));
    arg.bipsOut = XA_LC3_BIPS_OUT_DEF_VAL;
	arg.bitrate = bitrate; //
	arg.frame_ms = frame_Time_ms;	
	arg.dc = 0; //1

    float real_frame_time_s;

    if (samplerate == 48000 || samplerate == 48000/2)
       real_frame_time_s = frame_Time_ms/1000;
    else if (samplerate == 44100 || samplerate == 44100/2)
        real_frame_time_s = frame_Time_ms*48000/44100/1000;
    else if (samplerate == 32000 || samplerate == 32000/2)
        real_frame_time_s = frame_Time_ms*48000/32000/1000;

     handle->frame_bytes = (UWORD8)((bitrate/8)*real_frame_time_s);//byterate*time

    handle->dec_init_cfg.bipsOut = arg.bipsOut;
    handle->dec_init_cfg.sampleRate = samplerate;
    handle->dec_init_cfg.nChannels = channels;

    /************************************************************
    *                     Get Memory sizes from library        *
    ************************************************************/
    handle_size = xa_lc3_dec_get_handle_byte_size(&handle->dec_init_cfg);

    if(handle_size < 0)    
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", handle_size);

    scratch_size = xa_lc3_dec_get_scratch_byte_size(&handle->dec_init_cfg);

    if (scratch_size < 0)
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", scratch_size);

    inp_buf_size = XA_LC3_MAX_BYTES;

    if(arg.bipsOut == XA_LC3_BIPS_VAL_16)
        out_buf_size = channels * samples * sizeof(WORD16);
    else //arg.bipsOut = XA_LC3_BIPS_VAL_24
        out_buf_size = channels * samples * sizeof(WORD32);

    /************************************************************
    *                     Allocate Memory                       *
    ************************************************************/
    handle->decoder_state = (xa_codec_handle_t)codec_malloc(handle_size);
    if (handle->decoder_state == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API state alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(handle->decoder_state, INIT_SYMBOL, handle_size);

    //scratch = (pWORD32)malloc(scratch_size);
	scratch = (pWORD32)codec_malloc(scratch_size);
    if (scratch == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API scratch alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(scratch, INIT_SYMBOL, scratch_size);

    //inp_buf = (pUWORD8)malloc(inp_buf_size);
	handle->inp_buf = (pUWORD8)codec_malloc(inp_buf_size);	
    if (handle->inp_buf == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(handle->inp_buf, INIT_SYMBOL, inp_buf_size);

    //out_buf = (pWORD32)malloc(out_buf_size);
	handle->out_buf = (pWORD32)codec_malloc(out_buf_size);
    if (handle->out_buf == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    memset(handle->out_buf, INIT_SYMBOL, out_buf_size);

#ifdef SHOW_SIZES
    DBG_Printf("\nPersistent state size: %d bytes\n", handle_size);
    DBG_Printf("Scratch buffer size:   %d bytes\n", scratch_size);
    DBG_Printf("Input buffer size:     %d bytes\n", inp_buf_size);
    DBG_Printf("frame size:    %d bytes\n\n", arg.enc_frame_bytes);    
    DBG_Printf("Output buffer size:    %d bytes\n\n", out_buf_size);    		
#endif

    /*******************************************************************
    *                  Initialization.                                *
    *******************************************************************/
    error_code = xa_lc3_dec_init(handle->decoder_state, (pWORD32)scratch, &handle->dec_init_cfg);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

    
    WORD16 frame_dms = (WORD16)(arg.frame_ms*10);
    error_code = xa_lc3_dec_set_param(handle->decoder_state, XA_LC3_DEC_PARAM_FRAME_DMS, &frame_dms);
     _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code); 
    
     error_code = xa_lc3_dec_get_param(handle->decoder_state, XA_LC3_DEC_PARAM_FRAME_LENGTH, &handle->nSamples);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

	if(arg.dc)
    {
        error_code = xa_lc3_dec_get_param(handle->decoder_state, XA_LC3_DEC_PARAM_DELAY_COMPENSATION, &delay);
        delay = delay/arg.dc;
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
    }
	
	#if 0
	/* Open Output Wav File */
    output_wav = CreateWav(arg.outputFilename, dec_init_cfg.sampleRate, dec_init_cfg.nChannels, arg.bipsOut);
    exit_if(output_wav == NULL, "Error creating wav file!");
	#endif

	#if 0
	/* open auxillary files */
    if (arg.epf)
    {
        error_pattern_file = fopen(arg.epf, "rb");
        exit_if(error_pattern_file == NULL, "Error opening error pattern file!");
    }
	#endif

	#if 0
    if (arg.edf)
    {
        error_detection_file = fopen(arg.edf, "wb");
        exit_if(error_detection_file == NULL, "Error creating error detection file!");
    }
	#endif

	DBG_Printf("Sample rate:        %d\n", handle->dec_init_cfg.sampleRate);
    DBG_Printf("Number of channels: %d\n", handle->dec_init_cfg.nChannels);
    DBG_Printf("Frame length (in):  %d bytes\n", handle->frame_bytes);
    DBG_Printf("Frame length (out): %d samples\n", handle->nSamples);
    DBG_Printf("Output format:      %d bit\n", arg.bipsOut);
    DBG_Printf("Target Bitrate:     %d\n\n", arg.bitrate);    

	*decode_handle = handle;
    return XA_NO_ERROR;

}

U8 nedd_decode_frames = 0;
void lc3_dec_main_process_init(void)
{
#if 0    
	codec_malloc_init ();
    nedd_decode_frames = 0;
    if (optek_link_mode == BC_SF44K_T7_R1_PT5MS || optek_link_mode == BC_SF44K_PT5MS)
    {
        app_main_data.playing_stream_sample_rate = 44100;
    }
    else
    {
        app_main_data.playing_stream_sample_rate = 48000;
    }
 	app_main_data.playing_stream_chans = 2;   

    if (optek_link_mode == GAME_HEADPHONE_PT7P5MS_T7R3E || optek_link_mode == BC_SF48K_PT7P5MS)
    {
        if (xa_lc3_dec_main_process_init(160000,app_main_data.playing_stream_sample_rate,app_main_data.playing_stream_chans,7.5) != XA_NO_ERROR)
        {
            DBG_Printf("lc3 dec error!--\r\n");
        }
	    app_main_data.frame_time_us = 360*1000000/app_main_data.playing_stream_sample_rate;        
    }
    else
    {
        if (xa_lc3_dec_main_process_init(160000,app_main_data.playing_stream_sample_rate,app_main_data.playing_stream_chans,5.0) != XA_NO_ERROR)
        {
            DBG_Printf("lc3 dec error!--\r\n");
        }
	    app_main_data.frame_time_us = 240*1000000/app_main_data.playing_stream_sample_rate;        
    }



	app_main_data.audio_src_bit = AUDIO_SRC_16BIT;

	//app_main_data.playing_stream_sample_rate = 44100;
	//app_main_data.playing_stream_chans = 2;
	app_main_data.playing_stream_sample_bits = 16;
	app_main_data.playing_stream_bitrate = app_main_data.playing_stream_sample_rate*app_main_data.playing_stream_chans*app_main_data.playing_stream_sample_bits;

	app_audio_clock_freq_setting(app_main_data.playing_stream_sample_rate);
#endif
}

void lc3_dec_main_process_init_for_master(void)
{
#if 0     
    if(app_main_data.optek_link_role == OPL_ROLE_MASTER)
    {
        #ifdef OPL_MODE_WIRELESS_MIC
        if(app_main_data.optek_link_role == OPL_ROLE_MASTER)
        {
            U32 bitrate;
            float ptms;

            if (optek_link_mode == WIRELESS_MIC_PT2P5MS_T7R1 || optek_link_mode == WIRELESS_MIC_48K_PT2P5MS_T7R1M)
            {
                app_main_data.playing_stream_chans = 1;
                ptms = 2.5;
                bitrate = 80000;
            }
            else if (optek_link_mode == WIRELESS_MIC_PT2P5MS_LP)
            {
                app_main_data.playing_stream_chans = 1;
                ptms = 2.5;
                bitrate = 96000;
            }
            else
            {
                ptms = 5.0;
                if (optek_link_mode == WIRELESS_MIC_PT5MS_LP || optek_link_mode == WIRELESS_MIC_PT5MS_T10_R2_LP || optek_link_mode == WIRELESS_MIC_48K_PT5MS_T10_R2M_LP)
                {
                    app_main_data.playing_stream_chans = 1;
                    bitrate = 80000;
                }
                else
                {
                    app_main_data.playing_stream_chans = 2;
                    bitrate = 160000;   
                }        
            }

            if (xa_lc3_dec_main_process_init(bitrate,48000,1,ptms) != XA_NO_ERROR)
            {
                DBG_Printf("lc3 dec error!--\r\n");
            }
        }
        #else
        if (optek_link_mode == GAME_HEADPHONE_PT7P5MS_T7R3E || optek_link_mode == BC_SF48K_PT7P5MS)
        {
            if (xa_lc3_dec_main_process_init(39467,24000,1,7.5) != XA_NO_ERROR)
            {
                DBG_Printf("lc3 dec error!--\r\n");
            }
        }
        else
        {
            #ifdef UPSTREAM_SAMPLERATE_48k
            if (xa_lc3_dec_main_process_init(48000,48000,1,5.0) != XA_NO_ERROR)
            {
                DBG_Printf("lc3 dec error!--\r\n");
            }
            #else
            if (xa_lc3_dec_main_process_init(40000,24000,1,5.0) != XA_NO_ERROR)
            {
                DBG_Printf("lc3 dec error!--\r\n");
            }            
            #endif
        }
        #endif
    }
#endif    
}

#if 0
void lc3_dec_fini(int reason)
{
    DBG_Printf("lc3 dec finish:%d\r\n",reason);
}

extern i32 dritf_samples;
int lc3_dec_info_get(DECODE_INFO_STRU *info)
{
	DBG_Printf("drift samples:%d\n",dritf_samples);
    #if 1
    if (dritf_samples > 40)
        dritf_samples = 40;

    if (dritf_samples < -40)
        dritf_samples = -40;

		info->src_speed = dritf_samples;
    #else
	if (dritf_samples > 0)
		info->src_speed = 1;
	else if(dritf_samples < 0)
		info->src_speed = -1;
	else
		info->src_speed = 0;    
    #endif
	return 1;
}
#endif

U16 get_lc3_decode_frame_bytes(void *decode_handle)
{
    lc3_decode_handle *handle = decode_handle;
    return handle->frame_bytes;
}
XA_ERRORCODE xa_lc3_dec_frame (void *decode_handle,U8 *pIn,U8 inLen,U8 **pout_buf, U16 *out_byte_len)
{
    XA_ERRORCODE error_code = XA_NO_ERROR;

    WORD32 nBytes, nSamples_out;
    lc3_decode_handle *handle = decode_handle;


	*out_byte_len = 0;

    nBytes = inLen;
    memcpy(handle->inp_buf,pIn,nBytes);                
			
    nSamples_out = (handle->nSamples) * (handle->dec_init_cfg.nChannels);

    /* Run Decoder */

    error_code = xa_lc3_dec_process(handle->decoder_state, handle->inp_buf, handle->out_buf, (pUWORD32)&nBytes, (pUWORD32)&nSamples_out);    

    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

    if (nBytes == 0)
    {
        //DBG_PIN_HIGH;
        //DBG_PIN_LOW;
        DBG_Printf("loss frame\r\n");
    }


    /* interleave samples for writing */
    interleave(handle->out_buf, handle->sample_buf, nSamples_out / handle->dec_init_cfg.nChannels, handle->dec_init_cfg.nChannels, handle->dec_init_cfg.bipsOut);

    *pout_buf = handle->sample_buf;
    *out_byte_len = nSamples_out*2; //bytes

    return DECODE_SUCCESS;
}

#if 0
  // Set cache attribute to Write Back No Allocate when the last argument is -wbna
VOID set_wbna(pWORD32 argc, pWORD8 argv[])
{
    if (*argc > 1 && !strcmp(argv[*argc - 1], "-wbna"))
    {
#ifdef __XCC__
        extern WORD8 _memmap_cacheattr_wbna_trapnull;
        xthal_set_cacheattr((unsigned)& _memmap_cacheattr_wbna_trapnull);
#endif
        (*argc)--;
    }
}
#endif


#if 0
/*******************************************************************************
*
*  Function Name : main
*
*  Description   : Parse the parameter file and run the main process
*
*  parameters    : None
*
*  return value  : 0 on success, -1 on error
*
******************************************************************************/
WORD32 main(WORD32 argc, pWORD8 argv[])
{
    FILE* param_file_id;
    WORD8 curr_cmd[XA_MAX_CMD_LINE_LENGTH];
    WORD32 fargc, curpos;
    WORD32 processcmd = 0;
    WORD8 fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];
    pWORD8 pargv[XA_MAX_ARGS];
    WORD8 pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
    WORD8 pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
    WORD32 err_code = XA_NO_ERROR;

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    if (argc == 1)
    {
        param_file_id = fopen(PARAMFILE, "r");
        if (param_file_id == NULL)
        {
            fprintf(stdout, "Error opened Parameter File: %s \n", PARAMFILE);
            exit(1);
        }

        /* Process one line at a time */
        while (fgets(curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id))
        {
            curpos = 0;
            fargc = 0;
            /* if it is not a param_file command and if */
            /* CLP processing is not enabled */
            if (curr_cmd[0] != '@' && !processcmd)
            {   /* skip it */
                continue;
            }

            while (sscanf(curr_cmd + curpos, "%s", fargv[fargc]) != EOF)
            {
                if (fargv[0][0] == '/' && fargv[0][1] == '/')
                    break;
                if (strcmp(fargv[0], "@echo") == 0)
                    break;
                if (strcmp(fargv[fargc], "@New_line") == 0)
                {
                    fgets(curr_cmd + curpos, XA_MAX_CMD_LINE_LENGTH,
                        param_file_id);
                    continue;
                }
                curpos += strlen(fargv[fargc]);
                while (*(curr_cmd + curpos) == ' ' || *(curr_cmd + curpos) == '\t')
                    curpos++;
                fargc++;
            }

            if (fargc < 1)    /* for blank lines etc. */
                continue;

            if (strcmp(fargv[0], "@Output_path") == 0)
            {
                if (fargc > 1) strcpy(pb_output_file_path, fargv[1]);
                else strcpy(pb_output_file_path, "");
                continue;
            }

            if (strcmp(fargv[0], "@Input_path") == 0)
            {
                if (fargc > 1) strcpy(pb_input_file_path, fargv[1]);
                else strcpy(pb_input_file_path, "");
                continue;
            }

            if (strcmp(fargv[0], "@Start") == 0)
            {
                processcmd = 1;
                continue;
            }

            if (strcmp(fargv[0], "@Stop") == 0)
            {
                processcmd = 0;
                continue;
            }

            /* otherwise if this a normal command and its enabled for execution */
            if (processcmd)
            {
                WORD32 j;
                WORD32 pos;
                WORD8 inputFilename[XA_MAX_CMD_LINE_LENGTH] = "";
                WORD8 outputFilename[XA_MAX_CMD_LINE_LENGTH] = "";
                WORD8 edf[XA_MAX_CMD_LINE_LENGTH] = "";
                WORD8 epf[XA_MAX_CMD_LINE_LENGTH] = "";

                for (pos = 0; pos < fargc && fargv[pos][0] == '-'; pos++)
                {
                    if (!strcmp(fargv[pos], "-plcMeth") && pos + 1 < fargc)
                    {
                        pos++;
                    }

                    if (!strcmp(fargv[pos], "-bps") && pos + 1 < fargc)
                    {
                        pos++;
                    }

                    if (!strcmp(fargv[pos], "-epf") && pos + 1 < fargc)
                    {
                        pos++;
                        strcat(epf, pb_input_file_path);
                        strcat(epf, fargv[pos]);
                        strcpy(fargv[pos], epf);
                    }
                    if (!strcmp(fargv[pos], "-edf") && pos + 1 < fargc)
                    {
                        pos++;
                        strcat(edf, pb_output_file_path);
                        strcat(edf, fargv[pos]);
                        strcpy(fargv[pos], edf);
                    }
                }

                strcat(inputFilename, pb_input_file_path);
                strcat(inputFilename, fargv[pos]);
                strcpy(fargv[pos], inputFilename);
                pos++;

                strcat(outputFilename, pb_output_file_path);
                strcat(outputFilename, fargv[pos]);
                strcpy(fargv[pos], outputFilename);
                pos++;

                for (j = 0; j < fargc; j++)
                {
                    pargv[j] = fargv[j];
                }

                fprintf(stdout, "\n");
                xa_lc3_dec_main_process(fargc, pargv);
                
                CloseWav(output_wav);
                safe_fclose(input_bitstream);
                safe_fclose(error_pattern_file);
                safe_fclose(error_detection_file);
            }
        }
        err_code = XA_NO_ERROR;
    }
    else
    {
        err_code = xa_lc3_dec_main_process(argc - 1, &argv[1]);

        CloseWav(output_wav);
        safe_fclose(input_bitstream);
        safe_fclose(error_pattern_file);
        safe_fclose(error_detection_file);
    }

    return err_code;
}
#endif

static VOID parseCmdl_dec(WORD32 ac, pWORD8* av, Arguments_dec* arg)
{
    WORD32 pos = 0;
    WORD32 isTrue = 0;
    arg->bipsOut  = 16;
    arg->dc = 1; 
    arg->plcMeth = 0;

    /* parse options in any order */
    for (; pos < ac && av[pos][0] == '-'; pos++)
    {
        isTrue = 0;
        if (!strcmp(av[pos], "-h"))
        {
            puts(USAGE_MESSAGE_DEC);
            exit(0);
        }

        if (!strcmp(av[pos], "-q"))
        {
            arg->hide_counter = 1;
            isTrue = 1;
        }

        /* error pattern */
        if (!strcmp(av[pos], "-epf") && pos + 1 < ac)
        {
            arg->epf = av[++pos];
            isTrue = 1;
            puts("Using error pattern file for frame loss simulation!");
        }
        /* delay compensation */
        if (!strcmp(av[pos], "-dc") && pos + 1 < ac)
        {
            arg->dc = atoi(av[++pos]);
            isTrue = 1;
            exit_if(arg->dc < 0 || arg->dc > 2, "dc musst be 0, 1 or 2!");
        }
        
#ifndef DISABLE_PLC_API
        /* concealment method */
        if (!strcmp(av[pos], "-PLC_meth") && pos + 1 < ac)
        {
            isTrue = 1;
            arg->plcMeth = atoi(av[++pos]);
        }
#endif /* #ifdef DISABLE_PLC_API */

        /* Bits per sample */
        if (!strcmp(av[pos], "-bps") && pos + 1 < ac)
        {
            isTrue=1;
            arg->bipsOut = atoi(av[++pos]);
            
        }

        /* Error detection pattern */
        if (!strcmp(av[pos], "-edf") && pos + 1 < ac)
        {
            isTrue=1;
            arg->edf = av[++pos];
            puts("Writing error detection file!");
        }
        
        if((isTrue==0)&&(strlen(av[pos])>0))
        {
            puts(INVALID_ARGUMENT_MESSAGE);
            exit(0);
        }
    }

    exit_if(pos + 1 >= ac, MISSING_ARGUMENT_MESSAGE);

    arg->inputFilename = av[pos++];
    arg->outputFilename = av[pos++];

    putchar('\n');
}

/* check condition and if it fails, exit with error message */
static VOID exit_if(WORD32 condition, const pWORD8 message)
{
    if (condition)
    {
        puts(message);
        exit(1);
    }
}

/* close file ignoring NULL pointer */
/*static VOID safe_fclose(FILE* f)
{
    if (f != NULL)
        fclose(f);
}
*/
#if 0
/* read value from file and rewind if end is reached */
WORD16 loopy_read16(FILE* f)
{
    WORD16 tmp = 0;
    if (fread(&tmp, sizeof(tmp), 1, f) != 1)
    {
        fseek(f, 0, SEEK_SET);
        fread(&tmp, sizeof(tmp), 1, f);
    }
    return tmp;
}
#endif

#if 0
FILE* open_bitstream_reader(const pWORD8 file, pWORD32 samplerate, pWORD32 bitrate, pWORD32 channels)
{
    FILE* f_use = fopen(file, "rb");

    if (f_use)
    {
        WORD16 tmp = 0;
        fread(&tmp, sizeof(tmp), 1, f_use);
        *samplerate = tmp * 100;
        fread(&tmp, sizeof(tmp), 1, f_use);
        *bitrate = tmp * 100;
        fread(&tmp, sizeof(tmp), 1, f_use);
        *channels = tmp;
    }

    return f_use;
}
//#else
static FILE *open_bitstream_reader(const char *file, UWORD32 *samplerate, WORD32 *bitrate, WORD16 *channels,
                                   UWORD32 *signal_len, float *frame_ms, WORD32 *epmode,WORD32 *hrmode)
{
    FILE *f_use     = fopen(file, "rb");

    if (f_use)
    {
        UWORD16 header[10] = {0};
        fread(header, sizeof(header), 1, f_use);
        if (header[0] != 0xcc1c)
        {
            /* Old style bitstream header */
            *samplerate = header[0] * 100;
            *bitrate    = header[1] * 100;
            *channels   = header[2];
            *frame_ms   = (float)(1000/ 100.0);
            fseek(f_use, 6, SEEK_SET);
        }
        else
        {
            assert(header[1] >= 18);
            *samplerate = header[2] * 100;
            *bitrate    = header[3] * 100;
            *channels   = header[4];
            *frame_ms   = (float)(header[5] / 100.0);
            *epmode     = header[6];
            *signal_len = (UWORD32)header[7] | ((UWORD32)header[8] << 16);
            *hrmode     = header[1] > 18 ? header[9] : 0;
            fseek(f_use, header[1], SEEK_SET);
        }
    }

    return f_use;
}
#endif
#if 0
WORD32 read_bitstream_frame(FILE* bitstream_file, pUWORD8 bytes, WORD32 size)
{
    WORD32      i = 0;
    UWORD16 nbytes = 0;
    if (fread(&nbytes, sizeof(nbytes), 1, bitstream_file) != 1)
    {
        return -1; /* End of file reached */
    }
    for (i = 0; i < nbytes && i < size; i++)
    {
        bytes[i] = getc(bitstream_file);
    }
    return nbytes;
}
#endif
VOID interleave(pWORD32 in, pWORD16 out, WORD32 n, WORD32 channels, WORD32 bipsout)
{
    WORD32 ch, i;

    if (bipsout == 16)
    {
        pWORD16 p_in16 = (pWORD16)in;

        for (ch = 0; ch < channels; ch++)
        {
            for (i = 0; i < n; i++)
            {
                out[i * channels + ch] = *p_in16++;
            }
        }
    }
    else //bipsout = 24
    {
        pWORD32 p_in24 = in;

        for (ch = 0; ch < channels; ch++)
        {
            for (i = 0; i < n; i++)
            {
                out[i * channels + ch] = *p_in24++;
            }
        }
    }
}

#endif //LC3_DECODE_ENABLE

