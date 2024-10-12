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


#ifdef LC3_ENCODE_ENABLE

#include "regmap.h"
#include "mem_reloc.h"

/* turn off stdlib function warnings in visual studio */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "xa_lc3_codec_api.h"
#include "xa_error_handler.h"
#include "tinywavein_c.h"


//#define SHOW_SIZES
#define INIT_SYMBOL 0

#ifdef OUTPUT_BUF_PING_PONG_TEST
pUWORD8  pb_out_buf_ping = NULL;
pUWORD8  pb_out_buf_pong = NULL;
WORD32 pb_out_buf_pp_count = 0;
#endif


#ifdef __XTENSA__
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

#define PROFILER_FRAME   -1 // -1 - all w/o stop for get mcps
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
    { 0, 0, 0, 0, 0, 0, "lc3_enc" }
};

#endif


/* struct to hold command line arguments */
typedef struct
{
    pWORD8 inputFilename;
    pWORD8 outputFilename;
    WORD32   bitrate;
    pWORD8 bitrate_file;
    float  frame_ms; //NUM Frame length in ms. NUM must be 10 (default) or 7.5
    WORD32 dc; //0:Do not use delay compensation, 1:Compensate delay in decoder (default), Split delay equally between encoder and decoder
    WORD32   hide_counter;
    pWORD8 epf;
    WORD32   epmode;
    WORD32   ept;
} Arguments_enc;

/* local helper functions */
static VOID parseCmdl_enc(WORD32 ac, pWORD8* av, Arguments_enc* arg);
static VOID exit_if(WORD32 condition, const pWORD8 message);
VOID deinterleave(pWORD16 in, pWORD32 out, WORD32 n, WORD32 channels, WORD32 bipsin);
VOID write_bitstream_frame(FILE* bitstream_file, pUWORD8 bytes, WORD32 size);
static FILE *  open_bitstream_writer(const char *file, UWORD32 samplerate, WORD32 bitrate, WORD16 channels, UWORD32 signal_len, float frame_ms, WORD32 epmode);
WORD16 loopy_read16(FILE* f);
static WORD64 loopy_read64(FILE *f);
static VOID safe_fclose(FILE* f);


VOID xa_lc3_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_lc3_error_info;
extern const xa_error_info_struct xa_testbench_error_info;

#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20

#if 0
#define PARAMFILE "paramfilesimple_encode.txt"
#else
#define PARAMFILE "./test/build/paramfilesimple_encode.txt"
#endif

/*****************************************************************************/
/* Error codes for the testbench                                             */
/*****************************************************************************/
#define XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED                0xFFFF8000
#define XA_TESTBENCH_FATAL_FILE_OPEN_FAILED                0xFFFF8001

#ifdef __PROFILE__
/* The CPU_LOAD_WINDOW specifies the time period before the CPU load
is calculated. A 20 ms window is less than a full output buffer for
a stereo stream encoded at the highest sample rate (48 kHz). */
#define CPU_LOAD_WINDOW 0.02 /* seconds */
#endif

/*****************************************************************************/
/* Global variables                                                          */
/*****************************************************************************/

static const pWORD8 USAGE_MESSAGE_ENC =
"\n Usage: xa_lc3_enc_test [OPTIONS] <INPUT> <OUTPUT> <BITRATE>\n"
"\n"
"  INPUT is wav file, OUTPUT is bitstream file.\n"
"  BITRATE is specified in bits per second. Alternatively a switching file can\n"
"  be provided.\n"
"\n Options:\n"
"  -h           Show this message.\n"
"  -q           Hide frame counter.\n"
"\n Core codec options:\n"
"  -frame_ms    NUM Frame length in ms. NUM must be 10 (default) or 7.5.\n"
"  -swf FILE    Use a bitrate switching file instead of fixed bitrate.\n"
"  -epf FILE    Enable packet loss simulation using error pattern from FILE.\n"
"  -dc NUM                 0: Don't use delay compensation\n"
"                          1: Compensate delay in decoder (default)\n"
"                          2: Split delay equally between encoder and decoder\n"
"  -ept         Use together with -epf FILE to create bitstream triggering\n"
"               PLC via special value of lastnz.\n"
"";

static const pWORD8 MISSING_ARGUMENT_MESSAGE = "Not enough parameters! Use -h to show help.";
static const pWORD8 INVALID_ARGUMENT_MESSAGE = "Incorrect Input parameters! Use -h to show help.";

static WAVEFILEIN* input_wav;
static FILE* output_bitstream;
static FILE* error_pattern_file;
static FILE* bitrate_switching_file;


/*******************************************************************************
*
*  Function name : xa_lc3_enc_main_process
*
*  Description   :
*                  
*
*  parameters    : WORD32 argc (Arguments count)
*                  pWORD8 argv[] (Argument strings)
*
*  Return value  : none
******************************************************************************/

Arguments_enc arg;
WORD32 *sample_buf = NULL;

xa_lc3_enc_init_cfg_t enc_init_cfg;
xa_codec_handle_t encoder_state = NULL;

//xa_error_info_struct *p_proc_codec_err_info;
//xa_error_info_struct *p_proc_testbench_err_info;

pWORD32 inp_buf = NULL;
pUWORD8 out_buf = NULL;

WORD32 nSamples_in;

UWORD32 nSamplesFile =0xffffffff;

#ifdef LC3_HEADER_ADDED
U16 lc3_enc_header[9];
#endif

//XA_ERRORCODE xa_lc3_enc_main_process(WORD32 argc, pWORD8 argv[])
XA_ERRORCODE xa_lc3_enc_main_init(U32 bitrate,U16 samplerate,U8 channels,float frame_Time_ms)
{
    //Arguments_enc arg;
    //UWORD32     nSamplesFile =0xffffffff;
    WORD32     result;
    //WORD32     nBytes = 0, nSamples, real_bitrate, nSamples_in;
    WORD32     nBytes = 0, nSamples, real_bitrate;
    UWORD32     nSamplesRead;
    WORD32     frame = 1;
	#if 0
    WORD32   sample_buf[XA_LC3_MAX_CHANNELS * XA_LC3_MAX_SAMPLES];
	#else
    //WORD32   *sample_buf;
	#endif
    xa_error_info_struct *p_proc_codec_err_info;
    xa_error_info_struct *p_proc_testbench_err_info;
    WORD32      handle_size, scratch_size, inp_buf_size, out_buf_size;
	#if 0
    xa_lc3_enc_init_cfg_t enc_init_cfg;
    xa_codec_handle_t encoder_state = NULL;
	#endif
    pWORD32    scratch = NULL;
	#if 0
    pWORD32     inp_buf = NULL;
    pUWORD8     out_buf = NULL;
	#endif
    XA_ERRORCODE error_code = XA_NO_ERROR;
    input_wav = NULL;
    output_bitstream = NULL;
    error_pattern_file = NULL;
    bitrate_switching_file = NULL;
    WORD32 dc2_extra_frame = 0;
    WORD32 delay = 0;
    int byte_length;
    WORD32 samples;

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


#if 0//def __MCPS_MEASUREMENT__
    xt_iss_client_command("all", "disable");
#endif


	//init mem
	//codec_malloc_init();

	sample_buf = codec_malloc(channels * samples * 2);
	

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
	#if 0
    fprintf(stderr, "\n");
    fprintf(stderr, " %s library version %s\n",
        xa_lc3_get_lib_name_string(),
        xa_lc3_get_lib_version_string());
    fprintf(stderr, " API version: %s\n", xa_lc3_get_lib_api_version_string());
    fprintf(stderr, " Cadence, Inc. https://www.cadence.com\n");
    fprintf(stderr, "\n");
	#else
    DBG_Printf("\n\r");
    DBG_Printf(" %s library version %s\n",
        xa_lc3_get_lib_name_string(),
        xa_lc3_get_lib_version_string());
    DBG_Printf(" API version: %s\n\r", xa_lc3_get_lib_api_version_string());
    //DBG_Printf(" Cadence, Inc. https://www.cadence.com\n");
    //DBG_Printf("\n\r");
	#endif
	
    /******************************/
    /* Get arguments              */
    /******************************/
    //exit_if(argc <= 2, USAGE_MESSAGE_ENC);

    /* Default settings */
    memset(&arg, 0, sizeof(arg));

    enc_init_cfg.bitrate = bitrate;
    enc_init_cfg.sampleRate = samplerate;
    enc_init_cfg.nChannels = channels;
    enc_init_cfg.bipsIn = XA_LC3_BIPS_IN_DEF_VAL;

	#if 0
	parseCmdl_enc(argc, argv, &arg);
    exit_if((arg.ept && !arg.epf), "Usage of -ept only with -epf FILE!");
	#else
	arg.bitrate = bitrate;
	arg.frame_ms = frame_Time_ms;
	arg.dc = 0;
	arg.hide_counter = 0;
	//arg.epmode = 0x05;
	arg.epmode = 0x04;
	#endif
	
	
	if(enc_init_cfg.bipsIn == XA_LC3_BIPS_VAL_32)
	{
	    DBG_Printf("32-bit input not supported!\n\r");
	}

    /************************************************************
    *                     Get Memory sizes from library        *
    ************************************************************/
    handle_size = xa_lc3_enc_get_handle_byte_size(&enc_init_cfg);
    if (handle_size < 0)
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", handle_size);

    scratch_size = xa_lc3_enc_get_scratch_byte_size(&enc_init_cfg);
    DBG_Printf("scratch_size: %d bytes\n\r", scratch_size);
    if (scratch_size < 0)
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", scratch_size);

    out_buf_size = XA_LC3_MAX_BYTES;

    if (enc_init_cfg.bipsIn == XA_LC3_BIPS_VAL_16)
        inp_buf_size = channels * samples * sizeof(WORD16);
    else //enc_init_cfg.bipsIn = XA_LC3_BIPS_VAL_24
        inp_buf_size = channels * samples * sizeof(WORD32);
	
    DBG_Printf("inp_buf_size: %d bytes\n\r", inp_buf_size);


    /************************************************************
    *                     Allocate Memory                       *
    ************************************************************/

    /* State handle */
	#if 0
    encoder_state = (xa_codec_handle_t)malloc(handle_size);
	#else
	encoder_state = (xa_codec_handle_t)codec_malloc(handle_size);
	#endif

	if (encoder_state == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API state alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(encoder_state, INIT_SYMBOL, handle_size);
    /* Scratch */
	#if 0
	scratch = (pWORD32)malloc(scratch_size);
	#else
	scratch = (pWORD32)codec_malloc(scratch_size);
	#endif
	
    if (scratch == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API scratch alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }    

    memset(scratch, INIT_SYMBOL, scratch_size);

    /* Input buffer */
	#if 0
    inp_buf = (pWORD32)malloc(inp_buf_size);
	#else
    inp_buf = (pWORD32)codec_malloc(inp_buf_size);	
	#endif
    if (inp_buf == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(inp_buf, INIT_SYMBOL, inp_buf_size);

    /*Output buffer */
	#if 0
	out_buf = (pUWORD8)malloc(out_buf_size);
	#else
	out_buf = (pUWORD8)codec_malloc(out_buf_size);
	#endif
	
    if (out_buf == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    memset(out_buf, INIT_SYMBOL, out_buf_size);


#if 0//def SHOW_SIZES
    fprintf(stderr, "\n Persistent state size: %d bytes\n", handle_size);
    fprintf(stderr, " Scratch buffer size:   %d bytes\n", scratch_size);
    fprintf(stderr, " Input buffer size:     %d bytes\n", inp_buf_size);
    fprintf(stderr, " Output buffer size:    %d bytes\n\n", out_buf_size);
//#else
    DBG_Printf("\n Persistent state size: %d bytes\n\r", handle_size);
    DBG_Printf(" Scratch buffer size:   %d bytes\n\r", scratch_size);
    DBG_Printf(" Input buffer size:     %d bytes\n\r", inp_buf_size);
    DBG_Printf(" Output buffer size:    %d bytes\n\r", out_buf_size);
#endif

    /**********************************************************************
    *                Initialization
    ***********************************************************************/
    
    error_code = xa_lc3_enc_init(encoder_state, (pWORD32)scratch, &enc_init_cfg);
     _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code); 
            
    WORD16 frame_dms = (WORD16)(arg.frame_ms*10);
    error_code = xa_lc3_enc_set_param(encoder_state, XA_LC3_ENC_PARAM_FRAME_DMS, &frame_dms);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code); 
    //DBG_Printf("frame_dms set: %d\n\r", frame_dms);
   
    error_code = xa_lc3_enc_set_param(encoder_state, XA_LC3_ENC_PARAM_BITRATE, &arg.bitrate);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code); 
    //DBG_Printf("arg.bitrate set: %d\n\r", arg.bitrate);

    if(arg.dc)
    {
        error_code = xa_lc3_enc_get_param(encoder_state, XA_LC3_ENC_PARAM_DELAY_COMPENSATION, &delay);
                     _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
	    DBG_Printf("delay samples get: %d\n\r", delay);

        delay = delay/arg.dc;
    }
    error_code = xa_lc3_enc_get_param(encoder_state, XA_LC3_ENC_PARAM_FRAME_LENGTH, &nSamples);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
    //DBG_Printf("nSamples get: %d\n\r", nSamples);

    error_code = xa_lc3_enc_get_param(encoder_state, XA_LC3_ENC_PARAM_REAL_BITRATE, &real_bitrate);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
    //DBG_Printf("real_bitrate get: %d\n\r", real_bitrate);

    /* Open Output Bitstream File */
    nSamples_in = nSamples * enc_init_cfg.nChannels;

	#if 0
    output_bitstream = 
    open_bitstream_writer(arg.outputFilename, enc_init_cfg.sampleRate, real_bitrate, enc_init_cfg.nChannels,
                          nSamplesFile, arg.frame_ms, arg.epmode);
    exit_if(output_bitstream == NULL, "Error creating bitstream file!");
	#else
	
	#if 0
	static FILE *open_bitstream_writer(const char *file, UWORD32 samplerate, WORD32 bitrate, WORD16 channels, 
                                   UWORD32 signal_len, float frame_ms, WORD32 epmode);

    //FILE* f_use = fopen(file, "wb");
    //if (f_use)
    {
        UWORD16 header[9] = {0xcc1c,        sizeof(header), samplerate / 100,
                              bitrate / 100, channels,       (UWORD16)(frame_ms * 100),
                              epmode,        signal_len,     signal_len >> 16};
        fwrite(&header, sizeof(header), 1, f_use);
    }
	#endif

	#ifdef LC3_HEADER_ADDED
	//UWORD16 header[9];
	lc3_enc_header[0] = 0xcc1c;
	lc3_enc_header[1] = sizeof(lc3_enc_header); //18
	lc3_enc_header[2] = enc_init_cfg.sampleRate/100;
	
	lc3_enc_header[3] = real_bitrate / 100;
	lc3_enc_header[4] = enc_init_cfg.nChannels;
	lc3_enc_header[5] = arg.frame_ms*100; 

	lc3_enc_header[6] = arg.epmode;

	//signal_len
	//nSamplesFile = nSamples_in;
	lc3_enc_header[7] = nSamplesFile; //signal_len
	lc3_enc_header[8] = nSamplesFile>>16; //signal_len >> 16
	#endif

	#endif

    /* open auxillary files */
	#if 0
    if (arg.epf)
    {
        error_pattern_file = fopen(arg.epf, "rb");
        exit_if(error_pattern_file == NULL, "Error opening error pattern file!");
    }
    if (arg.bitrate_file)
    {
        bitrate_switching_file = fopen(arg.bitrate_file, "rb");
        exit_if(bitrate_switching_file == NULL, "Error opening bitrate switching file!");
    }
	#endif

    //nSamples_in = nSamples * enc_init_cfg.nChannels;

    /* Print out info */
	#if 0
    fprintf(stderr, " Sample rate:        %i\n", enc_init_cfg.sampleRate);
    fprintf(stderr, " Number of channels: %i\n", enc_init_cfg.nChannels);
    fprintf(stderr, " Frame length:       %i\n", nSamples);
    fprintf(stderr, " Target Bitrate:     %i\n", arg.bitrate);
    fprintf(stderr, " Real Bitrate:       %i\n\n", real_bitrate);
	#else
	//delayms(10);
    DBG_Printf(" Sample rate:        %d\n\r", enc_init_cfg.sampleRate);
    DBG_Printf(" Number of channels: %d\n\r", enc_init_cfg.nChannels);
    DBG_Printf(" Frame length:       %d\n\r", nSamples);
    DBG_Printf(" Target Bitrate:     %d\n\r", arg.bitrate);
    DBG_Printf(" Real Bitrate:       %d\n\r", real_bitrate);
	#endif

#ifdef OUTPUT_BUF_PING_PONG_TEST

    pb_out_buf_ping = (pUWORD8)malloc(out_buf_size);
    if (pb_out_buf_ping == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    pb_out_buf_pong = (pUWORD8)malloc(out_buf_size);
    if (pb_out_buf_pong == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

#endif
    

	#if 0
    /* delay compensation */
    if (arg.dc == 2)
    {
		ReadWavInt(input_wav, sample_buf, enc_init_cfg.nChannels * delay, &nSamplesRead);
    }
	#endif


	asm("nop");

	#if 0
	/* Encoder loop */
    while (1)
    {
        /* read bitrate switching file and set new bitrate */
        if (bitrate_switching_file)
        {
            WORD32  new_bitrate32 = (WORD32)(loopy_read64(bitrate_switching_file));
            new_bitrate32 = new_bitrate32*enc_init_cfg.nChannels;
            error_code = xa_lc3_enc_set_param(encoder_state, XA_LC3_ENC_PARAM_BITRATE, &new_bitrate32);
            _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
        }

        /* read audio data */
        result = ReadWavInt(input_wav, sample_buf, nSamples * enc_init_cfg.nChannels, &nSamplesRead);
        /* zero out rest of last frame */
        memset(sample_buf + nSamplesRead, 0, (nSamples * enc_init_cfg.nChannels - nSamplesRead) * sizeof(sample_buf[0]));
        exit_if(result == __TWI_ERROR, "Error reading wav file!");

        if (arg.dc != 2)
        {
            if (nSamplesRead == 0)
            {
                break;
            }
        }
        else
        {
            if (nSamplesRead != (nSamples *(enc_init_cfg.nChannels)))
            {
                WORD16 padded_samples = (((nSamples * (enc_init_cfg.nChannels)) -  nSamplesRead) / enc_init_cfg.nChannels);
                
                WORD16  delay_samples  = 0;
                error_code = xa_lc3_enc_get_param(encoder_state, XA_LC3_ENC_PARAM_DELAY_COMPENSATION, &delay_samples);
                  _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
                  
                delay_samples = delay_samples/2;

                if (padded_samples >= delay_samples)
                {
                    if (dc2_extra_frame == 1)
                    {
                        break;
                    }
                    dc2_extra_frame = 1;
                }
            }
        }

        if (arg.ept && loopy_read16(error_pattern_file))
        {
            nBytes = -1; /* tell encoder packet is lost and trigger PLC */
        }

        /* deinterleave channels */
        deinterleave(sample_buf, inp_buf, nSamples, enc_init_cfg.nChannels, enc_init_cfg.bipsIn);


#ifdef SCRATCH_TRASH_TEST
        memset(scratch, 0x13, scratch_size);
#endif
        
#ifdef OUTPUT_BUF_PING_PONG_TEST
        if ((pb_out_buf_pp_count++) & 1)
        {
            out_buf = pb_out_buf_ping;
        }
        else
        {
            out_buf = pb_out_buf_pong;
        }
#endif


#ifdef __PROF_ALLOCATE__
        if (PROFILER_FRAME == -1)
        {
            PROFILER_START
        }
        else if (PROFILER_FRAME == frame)
        {
            PROFILER_START
        }
#endif
#ifdef __MCPS_MEASUREMENT__
#ifdef __PROF_ALLOCATE__
        if (PROFILER_FRAME == -1)
            xt_iss_client_command("all", "enable");
#else
        xt_iss_client_command("all", "enable");
#endif				
        times(&start);
#endif
        
        error_code = xa_lc3_enc_process(encoder_state, inp_buf, out_buf, (pUWORD32)&nSamples_in, (pUWORD32)&nBytes);

#ifdef __MCPS_MEASUREMENT__
        times(&stop);
        xt_iss_client_command("all", "disable");
        mcpsforAlgs[0].cycles = stop.tms_utime - start.tms_utime;
#endif
#ifdef __PROF_ALLOCATE__
        if (PROFILER_FRAME == -1)
        {
            PROFILER_STOP
        }
        else if (PROFILER_FRAME == frame)
        {
            PROFILER_STOP
                exit(0);
        }
#endif
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

        write_bitstream_frame(output_bitstream, out_buf, nBytes);

        if (!arg.hide_counter)
        {
            printf("\rProcessing frame %i\n", frame);
            fflush(stdout);
        }

#ifdef __MCPS_MEASUREMENT__
        {
            WORD32 cnt = 0;
            {
                u_seconds = ((double)nSamples * 1000000) / enc_init_cfg.sampleRate;
                mcpsforAlgs[cnt].mcps_curr = (double)mcpsforAlgs[cnt].cycles / u_seconds;
                mcpsforAlgs[cnt].mcps_sum += mcpsforAlgs[cnt].mcps_curr;
                mcpsforAlgs[cnt].mcps_ave = mcpsforAlgs[cnt].mcps_sum / frame;
                if (mcpsforAlgs[cnt].mcps_peak < mcpsforAlgs[cnt].mcps_curr)
                {
                    mcpsforAlgs[cnt].mcps_peak = mcpsforAlgs[cnt].mcps_curr;
                    mcpsforAlgs[cnt].peak_frame = frame;
                }

                fprintf(stderr, "%s [%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",
                    mcpsforAlgs[cnt].nameAlg, frame, mcpsforAlgs[cnt].mcps_curr,
                    mcpsforAlgs[cnt].mcps_ave, mcpsforAlgs[cnt].mcps_peak,
                    mcpsforAlgs[cnt].peak_frame);

            }
        }
#endif
        frame++;
    }
	#endif
	
#if 0
	puts("\nProcessing done!");

    free(out_buf);
    free(inp_buf);
    free(scratch);
    free(encoder_state);

	#ifdef __MCPS_MEASUREMENT__
    {
        WORD32 cnt = 0;
        {
            FILE *mcps_log_file = fopen("mcps_report.txt", "at");

            fprintf(stderr, "---------- [%s] ----------\n", mcpsforAlgs[cnt].nameAlg);
            fprintf(stderr, "Peak MCPS = %f\n", mcpsforAlgs[cnt].mcps_peak);
            fprintf(stderr, "Peak frame = %d\n", mcpsforAlgs[cnt].peak_frame);
            fprintf(stderr, "Average MCPS = %f\n", mcpsforAlgs[cnt].mcps_ave);
            fprintf(stderr, "Total frames  = %d\n", frame);
            fprintf(mcps_log_file, "Encoder: ");
            fprintf(mcps_log_file, " %s  %s ", arg.inputFilename, arg.outputFilename);

            if (arg.bitrate_file == NULL)
                fprintf(mcps_log_file, "%d\n ", arg.bitrate);
            else
                fprintf(mcps_log_file, " %s\n", arg.bitrate_file);
            fprintf(mcps_log_file, "Peak MCPS = %f\n", mcpsforAlgs[cnt].mcps_peak);
            fprintf(mcps_log_file, "Peak frame = %d\n", mcpsforAlgs[cnt].peak_frame);
            fprintf(mcps_log_file, "Average MCPS = %f\n", mcpsforAlgs[cnt].mcps_ave);
            fprintf(mcps_log_file, "Total frames  = %d\n", frame);
            fclose(mcps_log_file);
        }

        mcpsforAlgs[cnt].mcps_peak = 0;
        mcpsforAlgs[cnt].peak_frame = 0;
        mcpsforAlgs[cnt].mcps_ave = 0;
        mcpsforAlgs[cnt].cycles = 0;
        mcpsforAlgs[cnt].mcps_sum = 0;
        mcpsforAlgs[cnt].mcps_curr = 0;
    }
	#endif

    printf("Done.\n\n\n");
#endif

    return XA_NO_ERROR;

} /* xa_lc3_enc_main_process */

int xa_lc3_enc_preprocess(U16 *pInBuf, U32 samples) __attribute__ ((section (".internal_ram_1_text")));
int xa_lc3_enc_preprocess(U16 *pInBuf, U32 samples)
{
    if (enc_init_cfg.nChannels == 1)
    {
        int i;
        short *pIn = pInBuf;
        short *pOut = inp_buf;

        pIn += 1;
        
        for (i=0;i<samples;i++)
        {
            *pOut++ = *pIn;
            pIn += 2;
        } 
 
        if (enc_init_cfg.sampleRate == 24000)
        {

        }
    }
    else
        deinterleave(pInBuf, inp_buf, samples, enc_init_cfg.nChannels, enc_init_cfg.bipsIn);

    return 1;
}

WORD32 frame = 1;
int xa_lc3_enc_frame(U16 *pInBuf, U32 samples, U8 **pOutBuf, U32 *pOutDataLen) __attribute__ ((section (".internal_ram_1_text")));
int xa_lc3_enc_frame(U16 *pInBuf, U32 samples, U8 **pOutBuf, U32 *pOutDataLen)
{
    XA_ERRORCODE error_code = XA_NO_ERROR;
    xa_error_info_struct *p_proc_codec_err_info;
    WORD32 dc2_extra_frame = 0;
	WORD32 i_out_bytes;
    WORD32 nBytes = 0, nSamples, nSamples_in;
    UWORD32 nSamplesRead;
    int byte_length;

	nSamples = samples;
    nSamples_in = nSamples * enc_init_cfg.nChannels;
    p_proc_codec_err_info = (xa_error_info_struct*)&xa_lc3_error_info;

	*pOutDataLen = 0;

    xa_lc3_enc_preprocess(pInBuf,samples);

	#if 0
	if (arg.dc == 2)
    {
		#if 0
		ReadWavInt(input_wav, sample_buf, enc_init_cfg.nChannels * delay, &nSamplesRead);
		#else
		nSamplesRead = enc_init_cfg.nChannels * delay;
		#endif
    }
	#endif

	/* Encoder loop */
	//while
	{
#if 0        
		#if 0
		/* read bitrate switching file and set new bitrate */
        if (bitrate_switching_file)
        {
            
            WORD32  new_bitrate32 = (WORD32)(loopy_read64(bitrate_switching_file));
            new_bitrate32 = new_bitrate32*enc_init_cfg.nChannels;
            error_code = xa_lc3_enc_set_param(encoder_state, XA_LC3_ENC_PARAM_BITRATE, &new_bitrate32);
            _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
        }
		#endif

        /* read audio data */
		#if 0
        result = ReadWavInt(input_wav, sample_buf, nSamples * enc_init_cfg.nChannels, &nSamplesRead);
		#else
		byte_length = samples * enc_init_cfg.nChannels * sizeof (WORD16);
		CFasm_memcpy(sample_buf, pInBuf, byte_length);
		nSamplesRead = samples*enc_init_cfg.nChannels;
        #endif

		/* zero out rest of last frame */
        //memset(sample_buf + nSamplesRead, 0, (nSamples * enc_init_cfg.nChannels - nSamplesRead) * sizeof(sample_buf[0]));
        memset(sample_buf + nSamplesRead, 0, (nSamples * enc_init_cfg.nChannels - nSamplesRead) * sizeof(sample_buf[0]));
        //exit_if(result == __TWI_ERROR, "Error reading wav file!");

        if (arg.dc != 2)
        {
            if (nSamplesRead == 0)
            {
                //break;
                asm("nop");
            }
        }
        else
        {
            if (nSamplesRead != (nSamples *(enc_init_cfg.nChannels)))
            {
                WORD16 padded_samples = (((nSamples * (enc_init_cfg.nChannels)) -  nSamplesRead) / enc_init_cfg.nChannels);
                
                WORD16  delay_samples  = 0;
                error_code = xa_lc3_enc_get_param(encoder_state, XA_LC3_ENC_PARAM_DELAY_COMPENSATION, &delay_samples);
                  _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
                  
                delay_samples = delay_samples/2;

                if (padded_samples >= delay_samples)
                {
                    if (dc2_extra_frame == 1)
                    {
                        //break;
                    }
                    dc2_extra_frame = 1;
                }
            }
        }

        if (arg.ept && loopy_read16(error_pattern_file))
        {
            //nBytes = -1; /* tell encoder packet is lost and trigger PLC */
        }

        /* deinterleave channels */
        deinterleave(sample_buf, inp_buf, nSamples, enc_init_cfg.nChannels, enc_init_cfg.bipsIn);
#endif
#ifdef SCRATCH_TRASH_TEST
        memset(scratch, 0x13, scratch_size);
#endif
        
#ifdef OUTPUT_BUF_PING_PONG_TEST
        if ((pb_out_buf_pp_count++) & 1)
        {
            out_buf = pb_out_buf_ping;
        }
        else
        {
            out_buf = pb_out_buf_pong;
        }
#endif


#if 0//def __PROF_ALLOCATE__
        if (PROFILER_FRAME == -1)
        {
            PROFILER_START
        }
        else if (PROFILER_FRAME == frame)
        {
            PROFILER_START
        }
#endif

#if 0//def __MCPS_MEASUREMENT__
#ifdef __PROF_ALLOCATE__
        if (PROFILER_FRAME == -1)
            xt_iss_client_command("all", "enable");
#else
        xt_iss_client_command("all", "enable");
#endif				
        times(&start);
#endif
        
        //optek_hifi2_stereo_16b_to_mono(inp_buf,pInBuf,samples<<1);
        //CFasm_memcpy(inp_buf, pInBuf, samples*2);
/*
        if (enc_init_cfg.nChannels == 1)
        {
            int i;
            short *pIn = pInBuf;
            short *pOut = inp_buf;

            pIn += 1;
            
            for (i=0;i<samples;i++)
            {
                *pOut++ = *pIn;
                pIn += 2;
            }            
        }
        else
            deinterleave(pInBuf, inp_buf, nSamples, enc_init_cfg.nChannels, enc_init_cfg.bipsIn);
*/
        error_code = xa_lc3_enc_process(encoder_state, inp_buf, out_buf, (pUWORD32)&nSamples_in, (pUWORD32)&nBytes);
/*
        if (error_code != XA_NO_ERROR)
        {
            DBG_Printf("\rdecode error\n");
        }
*/

#if 0//def __MCPS_MEASUREMENT__
        times(&stop);
        xt_iss_client_command("all", "disable");
        mcpsforAlgs[0].cycles = stop.tms_utime - start.tms_utime;
#endif

#if 0//def __PROF_ALLOCATE__
        if (PROFILER_FRAME == -1)
        {
            PROFILER_STOP
        }
        else if (PROFILER_FRAME == frame)
        {
            PROFILER_STOP
                exit(0);
        }
#endif

        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

		#if 0
		write_bitstream_frame(output_bitstream, out_buf, nBytes);
		#else
		*pOutDataLen = nBytes;
		*pOutBuf = out_buf;
		#endif
		
        if (!arg.hide_counter)
        {
			#if 0
            printf("\rProcessing frame %i\n", frame);
            fflush(stdout);
			#else
            //DBG_Printf("\rProcessing frame %i\n", frame);
			#endif
        }

#if 0//def __MCPS_MEASUREMENT__
        {
            WORD32 cnt = 0;
            {
                u_seconds = ((double)nSamples * 1000000) / enc_init_cfg.sampleRate;
                mcpsforAlgs[cnt].mcps_curr = (double)mcpsforAlgs[cnt].cycles / u_seconds;
                mcpsforAlgs[cnt].mcps_sum += mcpsforAlgs[cnt].mcps_curr;
                mcpsforAlgs[cnt].mcps_ave = mcpsforAlgs[cnt].mcps_sum / frame;
                if (mcpsforAlgs[cnt].mcps_peak < mcpsforAlgs[cnt].mcps_curr)
                {
                    mcpsforAlgs[cnt].mcps_peak = mcpsforAlgs[cnt].mcps_curr;
                    mcpsforAlgs[cnt].peak_frame = frame;
                }

                fprintf(stderr, "%s [%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",
                    mcpsforAlgs[cnt].nameAlg, frame, mcpsforAlgs[cnt].mcps_curr,
                    mcpsforAlgs[cnt].mcps_ave, mcpsforAlgs[cnt].mcps_peak,
                    mcpsforAlgs[cnt].peak_frame);

            }
        }
#endif

        //frame++;
    }
}


  // Set cache attribute to Write Back No Allocate when the last argument is -wbna
VOID set_wbna(pWORD32 argc, pWORD8 argv[])
{
#if 0
    if (*argc > 1 && !strcmp(argv[*argc - 1], "-wbna"))
    {
#ifdef __XCC__
        extern WORD8 _memmap_cacheattr_wbna_trapnull;
        xthal_set_cacheattr((unsigned)& _memmap_cacheattr_wbna_trapnull);
#endif
        (*argc)--;
    }
#endif
}

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

WORD32 lc3_main(WORD32 argc, pWORD8 argv[])
{
#if 0
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
                WORD8 bitrate_file[XA_MAX_CMD_LINE_LENGTH] = "";
                WORD8 epf[XA_MAX_CMD_LINE_LENGTH] = "";

                for (pos = 0; pos < fargc && fargv[pos][0] == '-'; pos++)
                {
                    if (!strcmp(fargv[pos], "-epf") && pos + 1 < fargc)
                    {
                        pos++;
                        strcat(epf, pb_input_file_path);
                        strcat(epf, fargv[pos]);
                        strcpy(fargv[pos], epf);
                    }
                    if (!strcmp(fargv[pos], "-swf") && pos + 1 < fargc)
                    {
                        pos++;
                        strcat(bitrate_file, pb_input_file_path);
                        strcat(bitrate_file, fargv[pos]);
                        strcpy(fargv[pos], bitrate_file);
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

                if (atoi(fargv[pos]) == 0)
                {
                    strcat(bitrate_file, pb_input_file_path);
                    strcat(bitrate_file, fargv[pos]);
                    strcpy(fargv[pos], bitrate_file);
                }

                for (j = 0; j < fargc; j++)
                {
                    pargv[j] = fargv[j];
                }

                fprintf(stdout, "\n");
                xa_lc3_enc_main_process(fargc, pargv);                

                CloseWavIn(input_wav);
                safe_fclose(output_bitstream);
                safe_fclose(error_pattern_file);
                safe_fclose(bitrate_switching_file);

            }
        }
        err_code = XA_NO_ERROR;
    }
    else
    {
        err_code = xa_lc3_enc_main_process(argc - 1, &argv[1]);

        CloseWavIn(input_wav);
        safe_fclose(output_bitstream);
        safe_fclose(error_pattern_file);
        safe_fclose(bitrate_switching_file);
    }

    return err_code;
#endif
} 

static VOID parseCmdl_enc(WORD32 ac, pWORD8* av, Arguments_enc* arg)
{
    WORD32 pos = 0;
    WORD32 isTrue;
    arg->frame_ms = 10;
    arg->dc       = 1;
    /* parse options in any order */
    for (; pos < ac && av[pos][0] == '-'; pos++)
    {
        isTrue=0;
        if (!strcmp(av[pos], "-h"))
        {
            printf(USAGE_MESSAGE_ENC, av[0]);
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
            puts("Using error pattern file for frame loss simulation!");
            isTrue=1;
        }
        if (!strcmp(av[pos ], "-frame_ms") && pos + 1 < ac)
        {
            arg->frame_ms = (float)atof(av[++pos]);
            isTrue=1;
        }
        
        /* delay compensation */
        if (!strcmp(av[pos], "-dc") && pos + 1 < ac)
        {
            arg->dc = atoi(av[++pos]);
            isTrue=1;
            exit_if(arg->dc < 0 || arg->dc > 2, "dc musst be 0, 1 or 2!");
        }

        /* trigger PLC with special decoder modes */
        if (!strcmp(av[pos], "-ept"))
        {
            arg->ept = 1;
            isTrue=1;
            puts("Simulating frame loss by writing special values into lastnz variable!");
        }

        /* Bitrate switching file */
        if (!strcmp(av[pos], "-swf") && pos + 1 < ac)
        {
            arg->bitrate_file = av[++pos];
            isTrue=1;
            puts("Using bitrate switching file!");
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

    /* Bitrate */
    exit_if(pos >= ac, MISSING_ARGUMENT_MESSAGE);
    arg->bitrate = atoi(av[pos]);
    if (arg->bitrate == 0)
    {
        arg->bitrate = 64000; /* dummy value */
        arg->bitrate_file = av[pos];
        puts("Using bitrate switching file!");
    }
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
VOID deinterleave(pWORD16 in, pWORD32 out, WORD32 n, WORD32 channels, WORD32 bipsin) __attribute__ ((section (".internal_ram_1_text")));

VOID deinterleave(pWORD16 in, pWORD32 out, WORD32 n, WORD32 channels, WORD32 bipsin)
{
    WORD32 ch, i;
    if (bipsin == 16)
    {
        WORD16 *p_out = (WORD16 *)out;

        for (ch = 0; ch < channels; ch++)
        {
            for (i = 0; i < n; i++)
            {
                *p_out++ = in[i * channels + ch];
            }
        }
    }
    else //bipsin = 24
    {
        pWORD32 p_out = out;

        for (ch = 0; ch < channels; ch++)
        {
            for (i = 0; i < n; i++)
            {
                *p_out++ = in[i * channels + ch];
            }
        }
    }
}
VOID write_bitstream_frame(FILE* bitstream_file, pUWORD8 bytes, WORD32 size)
{
    WORD32      i = 0;
    UWORD16 nbytes = size;
    fwrite(&nbytes, sizeof(nbytes), 1, bitstream_file);
    for (i = 0; i < size; i++)
    {
        putc(bytes[i], bitstream_file);
    }
}
/* close file ignoring NULL pointer */
static VOID safe_fclose(FILE* f)
{
    if (f != NULL)
        fclose(f);
}
#if 0
FILE* open_bitstream_writer(const pWORD8 file, WORD32 samplerate, WORD32 bitrate, WORD32 channels)
{
    FILE* f_use = fopen(file, "wb");

    if (f_use)
    {
        UWORD16 tmp = samplerate / 100;
        fwrite(&tmp, sizeof(tmp), 1, f_use);
        tmp = bitrate / 100;
        fwrite(&tmp, sizeof(tmp), 1, f_use);
        tmp = channels;
        fwrite(&tmp, sizeof(tmp), 1, f_use);
    }

    return f_use;
}
#else
static FILE *open_bitstream_writer(const char *file, UWORD32 samplerate, WORD32 bitrate, WORD16 channels, UWORD32 signal_len, float frame_ms, WORD32 epmode)
{
    FILE* f_use = fopen(file, "wb");

    if (f_use)
    {
        UWORD16 header[9] = {0xcc1c,        sizeof(header), samplerate / 100,
                              bitrate / 100, channels,       (UWORD16)(frame_ms * 100),
                              epmode,        signal_len,     signal_len >> 16};
        fwrite(&header, sizeof(header), 1, f_use);
    }

    return f_use;
}
#endif
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

static WORD64 loopy_read64(FILE *f)
{
    int64_t tmp = 0;
    if (fread(&tmp, sizeof(tmp), 1, f) != 1)
    {
        fseek(f, 0, SEEK_SET);
        fread(&tmp, sizeof(tmp), 1, f);
    }
    return tmp;
}

#endif //LC3_ENCODE_ENABLE
