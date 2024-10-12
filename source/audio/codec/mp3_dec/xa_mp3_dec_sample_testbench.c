/* Copyright (c) 2007 by Tensilica Inc.  ALL RIGHTS RESERVED.
 * These coded instructions, statements, and computer programs are the
 * copyrighted works and confidential proprietary information of
 * Tensilica Inc. and its licensors, and are licensed to the recipient
 * under the terms of a separate license agreement.  They may be
 * adapted and modified by bona fide purchasers under the terms of the
 * separate license agreement for internal use, but no adapted or
 * modified version may be disclosed or distributed to third parties
 * in any manner, medium, or form, in whole or in part, without the
 * prior written consent of Tensilica Inc.
 */

//#include "common.h"
#include <xtensa/tie/xt_hifi2.h>

//#include "tx_port.h"

#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"


#ifdef MP3_DECODE_ENABLE

#include "regmap.h"

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#include "xa_mp3_dec_api.h"
#include "id3_tag_decode.h"

//#include "hw_da_pp.h"

//#include "audio_dec.h"
//#include "app_dac.h"
#include "app_sdram.h"
//#include "app_gpio.h"
//#include "message.h"
#include "codec.h"
#include "app_main.h"
#include "audio_com.h"
#include "tools.h"
//#include "app_cmd.h"

//#include "optek_link.h"

#ifdef TIME_SCALE_CODE
#include "tscale.h"
#endif

#include "audio_com.h"
#include "audio_codec.h"
#include "hw_timer.h"
#include "app_media_com.h"
#include "mp3_decode.h"


//#define SLOW_MEMORY_USED

#define app_audio_clock_freq_setting    audio_clock_freq_set


//#define OPTEK_CODEC_MALLOC

//#undef TIME_SCALE_CODE

#define MP3_DEOCDING_FRAME
#define PROFILE 0

//#define SMALL_INPUT_CHUNK_FEED

#ifdef SMALL_INPUT_CHUNK_FEED
/**
 * Defining SMALL_INPUT_CHUNK_FEED enbables the test application to 
 * feed the data in small chunks of size CHUNK_SIZE to test
 * the partial input feature of the MP3 Decoder library
 */
#if !defined(CHUNK_SIZE)
//#define CHUNK_SIZE 100
#define CHUNK_SIZE 200
#endif
//int varChunkSize = CHUNK_SIZE;
//static int varChunkSize = CHUNK_SIZE;
static int varChunkSize;
#endif


#if 0
#undef DBG_Printf
#define DBG_Printf

#undef DBG_CodecPrintf
#define DBG_CodecPrintf
#endif


//#define XA_ERRORCODE		I32


#define SHOW_SIZES		0


void xa_mp3_dec_error_handler_init();
void xa_testbench_error_handler_init();

//extern const xa_error_info_struct xa_testbench_error_info;
extern const xa_error_info_struct xa_mp3_dec_error_info;


#ifdef XA_MP3_DEC_ERROR_DEBUG
#undef xa_mp3_dec_error_info
#define xa_mp3_dec_error_info xa_testbench_error_info	
#endif


#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED	0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED	0xffff8001

static U32 ui_inp_size;
#ifdef MP3_DEOCDING_FRAME
static U32 i_buff_size;
#endif

pWORD8 pb_inp_buf, pb_out_buf;
WORD32 i_bytes_consumed;
//FILE *g_pf_inp = NULL;
UWORD8 first_inited;

static void xa_shift_input_buffer (char *buf, int buf_size, int bytes_consumed)
{
  int i;
  
  if (bytes_consumed <= 0)
    return;
  
  /* Optimize 2-byte aligned data movement. */
  if ((((int)buf | buf_size | bytes_consumed) & 1) == 0)
  {
      /* Optimize 4-byte aligned data movement. */
      if ((((int)buf | buf_size | bytes_consumed) & 2) == 0)
	{
	  ae_p16x2s *dst = (ae_p16x2s *)buf;
	  ae_p16x2s *src = (ae_p16x2s *)&buf[bytes_consumed];
	  for (i = 0; i < (buf_size - bytes_consumed) >> 2; i++)
	    {
	      dst[i] = src[i];
	    }
	  return;
	}
      
      ae_p16s *dst = (ae_p16s *)buf;
      ae_p16s *src = (ae_p16s *)&buf[bytes_consumed];
      for (i = 0; i < (buf_size - bytes_consumed) >> 1; i++)
	{
	  dst[i] = src[i];
	}
      return;
    }
  
  /* Default, non-aligned data movement. */
  for (i = 0; i < buf_size - bytes_consumed; i++)
    {
      buf[i] = buf[i + bytes_consumed];
    }
}


XA_ERRORCODE xa_mp3_dec_set_config_param (xa_codec_handle_t  p_xa_process_api_obj)
{
	LOOPIDX i;
	XA_ERRORCODE err_code = XA_NO_ERROR;

	/* the process API function */
	xa_codec_func_t *p_xa_process_api = xa_mp3_dec;
	xa_error_info_struct *p_proc_err_info = &xa_mp3_dec_error_info;


	UWORD32 ui_pcm_wd_sz = 16;
	UWORD32 ui_crc_check = 0;

	err_code = (*p_xa_process_api)(p_xa_process_api_obj,
					 XA_API_CMD_SET_CONFIG_PARAM,
					 XA_MP3DEC_CONFIG_PARAM_PCM_WDSZ, &ui_pcm_wd_sz);
	_XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

	err_code = (*p_xa_process_api)(p_xa_process_api_obj,
					 XA_API_CMD_SET_CONFIG_PARAM,
					 XA_MP3DEC_CONFIG_PARAM_CRC_CHECK, &ui_crc_check);
	_XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);

	return XA_NO_ERROR;
}


XA_ERRORCODE xa_mp3_dec_get_config_param (xa_codec_handle_t p_xa_process_api_obj,
										 pWORD32 pi_bitrate,
										 pWORD32 pi_samp_freq,
										 pWORD32 pi_num_chan,
										 pWORD32 pi_pcm_wd_sz
    )
{
	XA_ERRORCODE err_code = XA_NO_ERROR;
	/* the process API function */
	xa_codec_func_t *p_xa_process_api = xa_mp3_dec;
	xa_error_info_struct *p_proc_err_info = &xa_mp3_dec_error_info;

	/* Data rate */
	{
		err_code = (*p_xa_process_api)(p_xa_process_api_obj,
					XA_API_CMD_GET_CONFIG_PARAM,
					XA_MP3DEC_CONFIG_PARAM_BITRATE, pi_bitrate);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
	}

	/* Sampling frequency */
	{
		err_code = (*p_xa_process_api)(p_xa_process_api_obj,
					XA_API_CMD_GET_CONFIG_PARAM,
					XA_MP3DEC_CONFIG_PARAM_SAMP_FREQ, pi_samp_freq);
		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
	}

	/* Number of channels */
	{
		err_code = (*p_xa_process_api)(p_xa_process_api_obj,
					XA_API_CMD_GET_CONFIG_PARAM,
					XA_MP3DEC_CONFIG_PARAM_NUM_CHANNELS, pi_num_chan);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
	}

	/* PCM word size */
	{
		err_code = (*p_xa_process_api)(p_xa_process_api_obj,
					XA_API_CMD_GET_CONFIG_PARAM,
					XA_MP3DEC_CONFIG_PARAM_PCM_WDSZ, pi_pcm_wd_sz);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
	}


	return XA_NO_ERROR;
}


static XA_ERRORCODE
    display_mch_status (xa_codec_handle_t p_xa_process_api_obj)
{
    XA_ERRORCODE err_code = XA_NO_ERROR;
    /* the process API function */
    xa_codec_func_t *p_xa_process_api = xa_mp3_dec;
    xa_error_info_struct *p_proc_err_info = &xa_mp3_dec_error_info;

    /* MCH information */
    /* MCH_STATUS info. */
    {
        int mch_status;
        static const char *mch_status_str[] = {
            "Unsupported",
            "Disabled",
            "Not present",
            "Present"
        };

        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_MCH_STATUS, &mch_status);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        //fprintf(stdout, "\nLayer II Multi-Channel Content: %s\n", mch_status_str[mch_status]);
        DBG_Printf("\nLayer II Multi-Channel Content: %s\n", mch_status_str[mch_status]);
        if (mch_status != XA_MP3DEC_MCH_STATUS_PRESENT)
            return XA_NO_ERROR;
    }

    /* Channel-Config */
    {
        int chan_config;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_CHAN_CONFIG, &chan_config);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        //fprintf(stdout, "\tChannel Configuration: %d\n", chan_config);
        DBG_Printf("\tChannel Configuration: %d\n", chan_config);
    }

    /* Channel-Map information */
    {
        int chan_map;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_CHAN_MAP, &chan_map);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        //fprintf(stdout, "\tChannel Map:           %08x    -   ",    chan_map);
        DBG_Printf("\tChannel Map:0x%04x\n\r", chan_map);
        switch (chan_map){
        case 0xFFF43120:
            //fprintf(stdout, "[X|X|X|Rs|Ls|C|R|L]");
            break;
        case 0xFFFF3120:
            //fprintf(stdout, "[X|X|X|X|S|C|R|L]");
            break;
        case 0xFFFFF120:
            //fprintf(stdout, "[X|X|X|X|X|C|R|L]");
            break;
        case 0xFFF65120:
            //fprintf(stdout, "[X|X|X|R2|L2|C|R|L]");
            break;
        case 0xFFFF4320:
            //fprintf(stdout, "[X|X|X|X|Rs|Ls|R|L]");
            break;
        case 0xFFFFF320:
            //fprintf(stdout, "[X|X|X|X|X|S|R|L]");
            break;
        case 0xFFFFFF20:
            //fprintf(stdout, "[X|X|X|X|X|X|R|L]");
            break;
        case 0xFFFF6520:
            //fprintf(stdout, "[X|X|X|X|R2|L2|R|L]");
            break;
        case 0xFFFFFFF1:
            //fprintf(stdout, "[X|X|X|X|X|X|X|M]");
            break;                           
        case 0xFFFFF651:                 
            //fprintf(stdout, "[X|X|X|X|X|R2|L2|M]");
            break;
        case 0xFF743120:
            //fprintf(stdout, "[X|X|Lfe|Rs|Ls|C|R|L]");
            break;
        case 0xFFF73120:
            //fprintf(stdout, "[X|X|X|Lfe|S|C|R|L]");
            break;
        case 0xFFFF7120:
            //fprintf(stdout, "[X|X|X|X|Lfe|C|R|L]");
            break;
        case 0xFF765120:
            //fprintf(stdout, "[X|X|Lfe|R2|L2|C|R|L]");
            break;
        case 0xFFF74320:
            //fprintf(stdout, "[X|X|X|Lfe|Rs|Ls|R|L]");
            break;
        case 0xFFFF7320:
            //fprintf(stdout, "[X|X|X|X|Lfe|S|R|L]");
            break;
        case 0xFFFFF720:
            //fprintf(stdout, "[X|X|X|X|X|Lfe|R|L]");
            break;
        case 0xFFF76520:
            //fprintf(stdout, "[X|X|X|Lfe|R2|L2|R|L]");
            break;
        case 0xFFFFFF71:
            //fprintf(stdout, "[X|X|X|X|X|X|Lfe|M]");
            break;                           
        case 0xFFFF7651:                 
            //fprintf(stdout, "[X|X|X|X|Lfe|R2|L2|M]");
            break;
        }
        //fprintf(stdout, "\n");

    }

    /* Num of extra channels in mch */
    {
        int num_xchans;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_NUM_XCHANS, &num_xchans);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
        //fprintf(stdout, "\tExtra Channels:        %d\n",
        //    num_xchans);
    }

    /* LFE */
    {
        int lfe_present;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_LFE_PRESENT, &lfe_present);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        //fprintf(stdout, "\tLFE Present:           %s\n",
        //    lfe_present ? "Yes" : "No");
    }

    /* Extension-Stream Presence */
    {
        int extn_present;
        err_code = (*p_xa_process_api)(p_xa_process_api_obj,
            XA_API_CMD_GET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_EXTN_PRESENT, &extn_present);
        _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        //fprintf(stdout, "\tExtension Present:     %s\n",
        //    extn_present ? "Yes" : "No");
    }

    return XA_NO_ERROR;
}


static unsigned int output_wordsize (unsigned int sample_bits)
{
	/* Round up to the next 2-byte size: 16 -> 2; 24 -> 4. */
	return 2 * ((sample_bits + 15) / 16);
}


static void pack_32_to_24_bits (pWORD8 pb_out_ptr, WORD32 input)
{
#if __XTENSA_EL__
	pb_out_ptr[0] = ((WORD32)input >>  8) & 0xff;
	pb_out_ptr[1] = ((WORD32)input >> 16) & 0xff;
	pb_out_ptr[2] = ((WORD32)input >> 24) & 0xff;
#else	/* __XTENSA_EL__ */
	pb_out_ptr[2] = ((WORD32)input >>  8) & 0xff;
	pb_out_ptr[1] = ((WORD32)input >> 16) & 0xff;
	pb_out_ptr[0] = ((WORD32)input >> 24) & 0xff;
#endif	/* __XTENSA_EL__ */
}

/* API obj */
xa_codec_handle_t xa_process_handle;
U8 xa_mp3_skip_frames; //for FF/FR

XA_ERRORCODE xa_mp3_dec_init (void)
{
	LOOPIDX i;
	I32	frame_len;
	U32 eventMask;

#if 1//DISPLAY_MESSAGE
	/* Library Info and Identification strings */
	WORD8 pb_process_name[30] = "";
	WORD8 pb_lib_version[30] = "";
#endif

	/* Error code */
	XA_ERRORCODE err_code = XA_NO_ERROR;
	XA_ERRORCODE err_code_exec = XA_NO_ERROR;



	/* First part                                        */
	/* Error Handler Init                                */
	/* Get Library Name, Library Version and API Version */
	/* Initialize API structure + Default config set     */
	/* Set config params from user                       */
	/* Initialize memory tables                          */
	/* Get memory information and allocate memory        */

	/* Memory variables */
	UWORD32 n_mems;
	UWORD32 ui_proc_mem_tabs_size;
	/* API size */
	UWORD32 pui_api_size;
	/* Process initing done query variable */
	UWORD32 ui_init_done;
	//pWORD8 pb_inp_buf, pb_out_buf;

	//UWORD32 ui_inp_size;
	WORD32 i_bytes_read;
#ifndef MP3_DEOCDING_FRAME
	WORD32 i_buff_size;
#endif

	//WORD32 i_out_bytes, i_total_bytes = 0;
	WORD32 i_bitrate, i_samp_freq, i_num_chan, i_pcm_wd_sz;
	pVOID pvoid;
	
#if ENABLE_ID3
	/* ID3 tag specific declarations */
	UWORD8 id3_buf[128];
	WORD32 id3_v1_found = 0, id3_v1_decoded = 0;
	id3v1_struct id3v1;
	WORD32 cur_pos;
	WORD32 id3_v2_found = 0, id3_v2_complete = 0;
	id3v2_struct id3v2;
#endif

	DBG_Puts ("xa mp3dec init enter\n\r");

	enableCoProcessor(CO_PROCESSOR_MASK_BITS);

	#ifdef OPTEK_CODEC_MALLOC
	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);
	#endif

#ifdef SRC_SIMPLE_VERSION
	Sacro_Init();	//init sample rate conversion
#endif

#ifdef FAST_SEARCH_ENABLE
	app_media_data.playing_stream_media_offset = 0;
#endif

#ifdef SMALL_INPUT_CHUNK_FEED
	varChunkSize = CHUNK_SIZE;
#endif

	/* The process API function */
	xa_codec_func_t *p_xa_process_api;
    /* The set config from argc argv */
    XA_ERRORCODE (*p_set_config_param) (int                argc,
        char              *argv[],
        xa_codec_handle_t  p_xa_process_api_obj);

	/* The get config from API */
	XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
										pWORD32           pi_bitrate,
										pWORD32           pi_samp_freq,
										pWORD32           pi_num_chan,
										pWORD32           pi_pcm_wd_sz);

	/* The error init function */
	void (*p_error_init)();

	/* The process error info structure */
	xa_error_info_struct *p_proc_err_info;

	/* Stack process struct initing */
	p_xa_process_api = xa_mp3_dec;
    p_set_config_param = xa_mp3_dec_set_config_param;

	xa_mp3_skip_frames = 0;

	p_get_config_param = xa_mp3_dec_get_config_param;
	p_error_init = xa_mp3_dec_error_handler_init;
	p_proc_err_info = &xa_mp3_dec_error_info;
	/* Stack process struct initing end */

	/* ******************************************************************/
	/* Initialize the error handler                                     */
	/* ******************************************************************/
	(*p_error_init)();

	/* ******************************************************************/
	/* Get the library name, library version and API version            */
	/* ******************************************************************/

#if 1//DISPLAY_MESSAGE
	/* Get the library name string */
	err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
				XA_CMD_TYPE_LIB_NAME, pb_process_name);

	_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

	/* Get the library version string */
	err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
				XA_CMD_TYPE_LIB_VERSION, pb_lib_version);

	_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

	/* Display the Tensilica identification message */
	//fprintf(stdout, "\n%s version %s\n", pb_process_name, pb_lib_version);
	//fprintf(stdout, "Tensilica, Inc. http://www.tensilica.com\n\n");
	DBG_Printf("%s version %s\n\r", pb_process_name, pb_lib_version);
	delayms(10);
#endif

	/* ******************************************************************/
	/* Initialize API structure and set config params to default        */
	/* ******************************************************************/

	/* Get the API size */
	err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_API_SIZE, 0,
				 &pui_api_size);

	_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

	/* Allocate memory for API */
    //g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(pui_api_size);
	#ifdef SLOW_MEMORY_USED
	pvoid = (pVOID)codec_malloc(pui_api_size);
	#else
	pvoid = (pVOID)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, pui_api_size, CODEC_MEM_ALIGN_4);
	#endif

    //if (g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
	if (pvoid == NULL) {
		_XA_HANDLE_ERROR(&xa_testbench_error_info, "API struct alloc",
						 XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
	}

	/* API object requires 4 bytes (WORD32) alignment;
	* malloc() provides at least 8-byte alignment.
	*/
	//assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & 3) == 0);
	DBG_Assert((((unsigned int) pvoid) & 3) == 0);

	/* Set API object with the memory allocated */
    //xa_process_handle = (xa_codec_handle_t) g_pv_arr_alloc_memory[g_w_malloc_count];
	xa_process_handle = (void *) pvoid;

    //g_w_malloc_count++;

	/* Set the config params to default values */
	err_code = (*p_xa_process_api)(xa_process_handle,
				 XA_API_CMD_INIT,
				 XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS,
				 NULL);

	_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

	/* ******************************************************************/
	/* Set config parameters got from the user present in argc argv     */
	/* ******************************************************************/

    //err_code = (*p_set_config_param)(argc, argv, xa_process_handle);
    err_code = xa_mp3_dec_set_config_param(xa_process_handle);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#ifdef ACTIVATE_VLC_REWIND
    {
		UWORD32 activate_vlc_rewind;
		activate_vlc_rewind = ACTIVATE_VLC_REWIND;
		err_code = (*p_xa_process_api)(xa_process_handle, 
            XA_API_CMD_SET_CONFIG_PARAM,
            XA_MP3DEC_CONFIG_PARAM_ACTIVATE_VLC_REWIND, 
            &activate_vlc_rewind);
           _XA_HANDLE_ERROR(p_proc_err_info, "" , err_code);
    }
#endif

    /* ******************************************************************/
    /* Initialize Memory info tables                                    */
    /* ******************************************************************/

    /* Get memory info tables size */
    err_code = (*p_xa_process_api)(xa_process_handle,
        XA_API_CMD_GET_MEMTABS_SIZE, 0,
        &ui_proc_mem_tabs_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


    //g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_proc_mem_tabs_size);
	#ifdef SLOW_MEMORY_USED
	pvoid = (pVOID)codec_malloc(ui_proc_mem_tabs_size);
	#else
	pvoid = (pVOID)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, ui_proc_mem_tabs_size, CODEC_MEM_ALIGN_4);
	#endif

    //if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
	if(pvoid == NULL) {
        _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
            XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

	/* Memory table requires 4 bytes (WORD32) alignment; malloc()
	* provides at least 8-byte alignment.
	*/
	//assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & 3) == 0);
	DBG_Assert((((unsigned int) pvoid) & 3) == 0);

	/* Set pointer for process memory tables	*/
	err_code = (*p_xa_process_api)(xa_process_handle,
        XA_API_CMD_SET_MEMTABS_PTR, 0,
        //(void *) g_pv_arr_alloc_memory[g_w_malloc_count]);
        pvoid);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    //g_w_malloc_count++;

	/* initialize the API, post config, fill memory tables	*/
	err_code = (*p_xa_process_api)(xa_process_handle,
				 XA_API_CMD_INIT,
				 XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS,
				 NULL);

	_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

	/* ******************************************************************/
	/* Allocate Memory with info from library                           */
	/* ******************************************************************/

	/* Get number of memory tables required */
	err_code = (*p_xa_process_api)(xa_process_handle,
				 XA_API_CMD_GET_N_MEMTABS,
				 0,
				 &n_mems);

	_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

	for (i = 0; i < (WORD32) n_mems; i++)
	{
		int ui_size, ui_alignment, ui_type;
		void *pv_alloc_ptr;

		/* Get memory size */
		err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_GET_MEM_INFO_SIZE,
				   i,
				   &ui_size);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

		/* Get memory alignment */
		err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_GET_MEM_INFO_ALIGNMENT,
				   i,
				   &ui_alignment);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

		/* Get memory type */
		err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_GET_MEM_INFO_TYPE,
				   i,
				   &ui_type);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

        //g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(ui_size);
		#ifdef SLOW_MEMORY_USED
		pvoid = (pVOID)codec_malloc(ui_size);
		#else
		pvoid = (pVOID)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, ui_size, CODEC_MEM_ALIGN_4);
		#endif


        //if(g_pv_arr_alloc_memory[g_w_malloc_count] == NULL) {
		if(pvoid == NULL) {
		  _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
			   XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
		}

		/* The following assertion should never fail because malloc() on
		 * Xtensa always returns memory aligned on at least an 8-byte
		 * boundary.
		 */
		//assert((((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) % ui_alignment) == 0);
		DBG_Assert((((unsigned int) pvoid) % ui_alignment) == 0);

       //pv_alloc_ptr = (void *) g_pv_arr_alloc_memory[g_w_malloc_count];
		pv_alloc_ptr = pvoid;

        //g_w_malloc_count++;

		/* Set the buffer pointer */
		err_code = (*p_xa_process_api)(xa_process_handle,
					   XA_API_CMD_SET_MEM_PTR,
					   i,
					   pv_alloc_ptr);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

		if(ui_type == XA_MEMTYPE_INPUT)
		{
			pb_inp_buf = pv_alloc_ptr;
			ui_inp_size = ui_size;
		}

		if(ui_type == XA_MEMTYPE_OUTPUT)
		{
			pb_out_buf = pv_alloc_ptr;
		}

#if SHOW_SIZES
		switch (ui_type)
		{
			case XA_MEMTYPE_INPUT:
			//fprintf(stdout, "Input buffer size: %u bytes\n", ui_size);
			DBG_Printf("Input buffer size: %u bytes\n\r", ui_size);
			break;
			case XA_MEMTYPE_OUTPUT:
			//fprintf(stdout, "Output buffer size: %u bytes\n", ui_size);
			DBG_Printf("Output buffer size: %u bytes\n\r", ui_size);
			break;
			case XA_MEMTYPE_SCRATCH:
			//fprintf(stdout, "Scratch buffer size: %u bytes\n", ui_size);
			DBG_Printf("Scratch buffer size: %u bytes\n\r", ui_size);
			break;
			case XA_MEMTYPE_PERSIST:
			//fprintf(stdout, "Persistent buffer size: %u bytes\n", ui_size);
			DBG_Printf("Persistent buffer size: %u bytes\n\r", ui_size);
			break;
			default:
			DBG_Assert(0);
		}
#endif	/* SHOW_SIZES */
	}

    /* End first part */

    i_bytes_consumed = ui_inp_size;


#if 1//add for using new mp3 library on May 16th,2019

#if MEM_INPUT
    stream_in_ptr = stream_in;
    stream_in_remaining = stream_in_size;
#endif

#if ENABLE_ID3
    #if 0//
	//call the function below, cann't play the one mp3 of all usbDisk.
	xa_fseek(NULL, 0, SEEK_SET);
	#endif

    /* ID3V1 handling */
    {
        //cur_pos = ftell(g_pf_inp);
		cur_pos = xa_ftell(NULL);

        //fseek(g_pf_inp, -128, SEEK_END);
		xa_fseek(NULL, -128, SEEK_END);
		
        //fread(id3_buf, 1, 128, g_pf_inp);
		xa_fread(id3_buf, 1, 128, NULL);
		
        /* search for ID3V1 */
        id3_v1_found = search_id3_v1(id3_buf + 0);

        if (id3_v1_found) {
            //fprintf(stdout,  "ID3V1 data : \n");
			DBG_Printf("ID3V1 data : \n");

            /* if ID3V1 is found, decode ID3V1 */
            decode_id3_v1(id3_buf + 3, &id3v1);
            id3_v1_decoded = 1;

            //fprintf(stdout,  "\n");
			DBG_Printf("\n");
        }

        //fseek(g_pf_inp, cur_pos, SEEK_SET);
		xa_fseek(NULL, cur_pos, SEEK_SET);
    }

    /* ID3V2 handling */
    {
        WORD32 flag = 0;
        WORD32 continue_flag = 0;
        WORD32 i_fread_bytes;
        pUWORD8 pub_input_ptr;

        i_fread_bytes = xa_fread(pb_inp_buf,
            sizeof(char),
            ui_inp_size,
            //g_pf_inp);
            NULL);

        i_bytes_consumed = 0;
        /* search for ID3V2 */
        id3_v2_found = search_id3_v2((pUWORD8)pb_inp_buf);

        if (id3_v2_found) {
            //fprintf(stdout,  "ID3V2 data : \n");
			DBG_Printf("ID3V2 data : \n");
            /* initialise the max fields */
            init_id3v2_field(&id3v2);

            while (!id3_v2_complete && id3_v2_found) {
                /* if ID3V2 is found, decode ID3V2 */
                id3_v2_complete = decode_id3_v2((const char *const)pb_inp_buf,
                    &id3v2,
                    continue_flag,
                    i_fread_bytes);

                if (!id3_v2_complete) {
                    continue_flag = 1;
                    i_bytes_consumed = id3v2.bytes_consumed;

                    if (i_bytes_consumed < i_fread_bytes)
                        xa_shift_input_buffer((char *)pb_inp_buf, i_fread_bytes, i_bytes_consumed);

                    //fseek(g_pf_inp, i_bytes_consumed, SEEK_SET);
					xa_fseek(NULL, i_bytes_consumed, SEEK_SET);
					
                    pub_input_ptr = (pUWORD8)pb_inp_buf;

                    if ((i_fread_bytes = xa_fread(pub_input_ptr,
                        sizeof(UWORD8),
                        ui_inp_size,
                        //g_pf_inp)) <= 0) {
                        NULL)) <= 0) {
                            //fprintf(stdout, "ID3 Tag Decoding: End of file reached.\n");
                            DBG_Printf("ID3 Tag Decoding: End of file reached.\n");
                            flag = 1;      /* failed */
                            break;
                    }
                    i_buff_size = i_fread_bytes;
                }
            }

            if (id3_v2_complete) {
                //fprintf(stdout,  "\n");
				DBG_Printf("\n");

                i_bytes_consumed = id3v2.bytes_consumed;

                //fseek(g_pf_inp, i_bytes_consumed, SEEK_SET);
				xa_fseek(NULL, i_bytes_consumed, SEEK_SET);
				
                pub_input_ptr = (pUWORD8)pb_inp_buf;

                if ((i_fread_bytes = xa_fread(pub_input_ptr,
                    sizeof(UWORD8),
                    ui_inp_size,
                    //g_pf_inp)) <= 0) {
                    NULL)) <= 0) {
                        //fprintf(stdout, "ID3V2 tag decoding: end of file reached.\n");
						DBG_Printf("ID3V2 tag decoding: end of file reached.\n");
                        flag = 1;      /* failed */
                }

                i_buff_size = i_fread_bytes;
                i_bytes_consumed = 0;
            }

            if (flag) {
                return XA_NO_ERROR;
            }
        }
    }
#endif //ENABLE_ID3

#endif //add for using new mp3 library on May 16th,2019



	wNoMoreFrames = FALSE;


	/* Second part        */
	/* Initialize process */
	/* Get config params  */

	/* ******************************************************************/
	/* Initialize process in a loop (to handle junk data at beginning)  */
	/* ******************************************************************/
	i_buff_size = ui_inp_size;


#ifdef SMALL_INPUT_CHUNK_FEED //add for using new mp3 library on May 16th,2019
    //fseek(g_pf_inp, 0, SEEK_SET);
    xa_fseek(NULL, 0, SEEK_SET);
    memset((char *)pb_inp_buf, 0,  ui_inp_size);
    i_bytes_consumed = varChunkSize;
    i_bytes_consumed = (i_bytes_consumed > ui_inp_size)? ui_inp_size:i_bytes_consumed;
    i_buff_size = i_bytes_consumed;
#endif

#ifdef STREAM_CHANGE_REINIT_TEST //add for using new mp3 library on May 16th,2019
    reInit_err_typ = 0;
    reInit_active = 0;
REINIT_POINT:
    frame = 0;
#endif /* STREAM_CHANGE_REINIT_TEST */


    do {
		xa_shift_input_buffer(pb_inp_buf, ui_inp_size, i_bytes_consumed);

		#ifdef SMALL_INPUT_CHUNK_FEED //add for using new mp3 library on May 16th,2019
        /* Try and Read at least varChunkSize at time or smaller from the file */
        int i_bytes_requested = (i_bytes_consumed < varChunkSize)?i_bytes_consumed:varChunkSize;
        i_bytes_requested = (i_bytes_requested==0)?varChunkSize:i_bytes_requested;
        i_bytes_requested = (i_bytes_requested > (ui_inp_size-(i_buff_size - i_bytes_consumed))) ? \
            ui_inp_size-(i_buff_size - i_bytes_consumed):i_bytes_requested;

        i_bytes_read = xa_fread(pb_inp_buf + (i_buff_size - i_bytes_consumed),
            sizeof(WORD8),
            i_bytes_requested,
            //g_pf_inp);
            NULL);
		#else
        i_bytes_read = xa_fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
            sizeof(char),
            i_bytes_consumed,
            NULL);
		#endif

		#if 0//def RECORD_ENABLE
		if (app_media_data.record_status == TRUE)
		{
			int free_space;

			free_space = fifo_get_free_space (&record_fifo);
			if (free_space >= i_bytes_read)
			{
				//put the data to copy fifo
				fifo_put_data (&record_fifo, pb_inp_buf, i_bytes_read);
			}
			else
			{
				//encoded buf underflow
				DBG_Printf ("mp3 copy buf underflow\n\r");
				DBG_Assert (FALSE);
			}
		}
		#endif

		/* New buffer size */
		i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);

		#ifdef SMALL_INPUT_CHUNK_FEED
        /* Declare input over if bytes read are 0  and there is still space for input filling in the codec input buffer */
        if(( i_buff_size <= 0 ) || ((i_bytes_read == 0) && (i_buff_size<ui_inp_size)) )
		#else
        // if( i_buff_size <= 0 ) 
        if(i_bytes_consumed != i_bytes_read)
		#endif
        {
			if ( i_buff_size <= 0 )
			{
				i_buff_size = 0;
				DBG_Printf ("No Data in Stream\n");
			}

			wNoMoreFrames = TRUE;

			/* Tell that the input is over in this buffer */
			err_code = (*p_xa_process_api)(xa_process_handle,
						XA_API_CMD_INPUT_OVER,
						0,
						NULL);

			_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
		}

		/* Set number of bytes to be processed */
		err_code = (*p_xa_process_api)(xa_process_handle,
					XA_API_CMD_SET_INPUT_BYTES,
					0,
					&i_buff_size);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

		/* Initialize the process */
		err_code = (*p_xa_process_api)(xa_process_handle,
					XA_API_CMD_INIT,
					XA_CMD_TYPE_INIT_PROCESS,
					NULL);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

		/* Checking for end of initialization */
		err_code = (*p_xa_process_api)(xa_process_handle,
					XA_API_CMD_INIT,
					XA_CMD_TYPE_INIT_DONE_QUERY,
					&ui_init_done);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

		/* How much buffer is used in input buffers */
		err_code = (*p_xa_process_api)(xa_process_handle,
					XA_API_CMD_GET_CURIDX_INPUT_BUF,
					0,
					&i_bytes_consumed);

		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

		if (wRequestDecodeFrameExit == TRUE)
		{
			ui_init_done = TRUE;
		}

	} while (!ui_init_done && wNoMoreFrames == FALSE);
	/* Init end of process condition end */

	DBG_Puts ("xa mp3dec init exit\n\r");

	if (wNoMoreFrames == TRUE) {
		//can not find mp3 header
		return XA_FATAL_ERROR;
	}
	else {
		/* ******************************************************************/
		/* Get config params from API                                       */
		/* ******************************************************************/

		err_code = (*p_get_config_param)(xa_process_handle,
					&i_bitrate,
					&i_samp_freq,
					&i_num_chan,
					&i_pcm_wd_sz);
		_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

		app_media_data.playing_stream_bitrate = i_bitrate;
		app_media_data.playing_stream_sample_rate = i_samp_freq;
		app_media_data.playing_stream_chans = i_num_chan;
		app_media_data.playing_stream_sample_bits = i_pcm_wd_sz;

		if (app_media_data.playing_stream_vbr == 0)
		{
			app_media_data.playing_stream_bitrate = i_bitrate * (1000/8); //change to byte rate
			DBG_Puts ("No VBR\n\r");
		}
		else
		{
			app_media_data.playing_stream_bitrate = i_bitrate * (1000/8);
			DBG_Puts ("VBR\n\r");
		}

		//DBG_Printf ("mp3 bps:%d\n\r", app_media_data.playing_stream_bitrate);

		#ifdef FAST_SEARCH_ENABLE
		//app_media_data.playing_stream_media_offset = xa_ftell(NULL);
		//app_media_data.playing_stream_bitrate = (44100 * 4);

		if (app_media_data.playing_stream_vbr == 0)
		{
			app_media_data.playing_stream_total_time = 
				(AUDIO_FIFO_STREAM_STREAM_LEN() - app_media_data.playing_stream_media_offset) / app_media_data.playing_stream_bitrate;

			//DBG_Printf ("Mp3 total time %d\n\r", app_media_data.playing_stream_total_time);		
		}
			
		/*
		if (app_media_data.media == MEDIA_USB_DEVICE)
		{
			app_media_data.playing_stream_total_time =  (app_media_data.pMediaRead->lenth - app_media_data.playing_stream_media_offset) / app_media_data.playing_stream_bitrate;
		}
		else if (app_media_data.media == MEDIA_CD)
		{
			app_media_data.playing_stream_total_time =  (app_media_data.pMediaRead->lenth - app_media_data.playing_stream_media_offset) / app_media_data.playing_stream_bitrate;
		}
		*/
		#endif

		if ( (i_samp_freq == 3200) || (i_samp_freq == 4410) || (i_samp_freq == 4800) )
		{
			DBG_CodecPrintf("bitrate = %dk, ", i_bitrate);
			DBG_CodecPrintf("samp_freq = %dHz, ", i_samp_freq);
			DBG_CodecPrintf("num_chan = %d, ", i_num_chan);
			//DBG_CodecPrintf("pcm_wd_sz = %d, ", i_pcm_wd_sz);
			//DBG_CodecPrintf("total time = %d\n\r", app_media_data.playing_stream_total_time);
			//DBG_CodecPrintf("media_offset = %d\n\r", app_media_data.playing_stream_media_offset);
		}

		#if 0
		app_audio_clock_freq_setting(app_media_data.playing_stream_sample_rate);

		#ifdef APP_OPTEK_D2AS
		app_optek_dsrc_convert_open(app_media_data.playing_stream_sample_rate, AUDIO_OUT_SR_FIXED);
		#endif
		#endif


		#ifdef SRC_NEW_VER
		#ifdef AUDIO_SAMPLE_RATE_SWITCH_TO_48K
		if( src_init( i_samp_freq, i_num_chan, TSAMPLE_RATE_44800 ) )
		{
			DBG_Printf("src init error for sample rate: %d\n\r", i_samp_freq, TSAMPLE_RATE_48000);
		}
		#else
		#if 0
		if( src_init( i_samp_freq, i_num_chan ) )
		{
			DBG_Printf("src init error for sample rate: %d\n\r", i_samp_freq);
		}
		#else
		if( src_init( i_samp_freq, i_num_chan, TSAMPLE_RATE_44100 ) ) 
		{
			DBG_Printf("src init error for sample rate: %d\n\r", i_samp_freq, TSAMPLE_RATE_44100);
		}
		#endif
		#endif
		#endif

		return XA_NO_ERROR;
	}
}


//mp3 decoding fream length:4608 bytes
#ifndef MP3_DEOCDING_FRAME
int xa_mp3_dec_frame (void)
#else
int xa_mp3_dec_frame (U32 **out_buf, U32 *out_len)
#endif
{
	UWORD32 ui_exec_done;
	WORD32 i_bytes_read;
#ifndef MP3_DEOCDING_FRAME
	WORD32 i_buff_size;
#endif
	WORD32 i_out_bytes;

	// The process API function
	//xa_codec_func_t *p_xa_process_api;
	WORD32 i_bitrate, i_samp_freq, i_num_chan, i_pcm_wd_sz;

	// The get config from API
	XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
		pWORD32           pi_bitrate,
		pWORD32           pi_samp_freq,
		pWORD32           pi_num_chan,
		pWORD32           pi_pcm_wd_sz);

	p_get_config_param = xa_mp3_dec_get_config_param;

	/* The process API function */
	xa_codec_func_t *p_xa_process_api;
	/* The process error info structure */
	xa_error_info_struct *p_proc_err_info;

	/* Stack process struct initing */
	p_xa_process_api = xa_mp3_dec;
	p_proc_err_info = &xa_mp3_dec_error_info;
	/* Stack process struct initing end */

	/* Error code */
	XA_ERRORCODE err_code = XA_NO_ERROR;
	XA_ERRORCODE err_code_exec = XA_NO_ERROR;


	/* ******************************************************************/
	/* Initialize process in a loop (to handle junk data at beginning)  */
	/* ******************************************************************/
#ifndef MP3_DEOCDING_FRAME
	i_buff_size = ui_inp_size;
#endif

	/* Third part for executing each process */
	do
	{
		/* Execute process */
		xa_shift_input_buffer(pb_inp_buf, ui_inp_size, i_bytes_consumed);

		i_bytes_read = xa_fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
								sizeof(char),
								i_bytes_consumed,
								NULL);

		#if 0//def RECORD_ENABLE
		if (app_media_data.record_status == TRUE)
		{
			int free_space;
			
			free_space = fifo_get_free_space (&record_fifo);
			if (free_space >= i_bytes_read)
			{
				//put the data to copy fifo
				fifo_put_data (&record_fifo, pb_inp_buf, i_bytes_consumed);
			}
			else
			{
				//encoded buf underflow
				DBG_Printf ("mp3 copy buf underflow\n\r");
				DBG_Assert (FALSE);
			}
		}
		#endif

        /* New buffer size */
        i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);


#ifdef SMALL_INPUT_CHUNK_FEED
		/* Declare input over if bytes read are 0  and there is still space for input filling in the codec input buffer */
		if ( ( i_buff_size <= 0 ) || ((i_bytes_read == 0) && (i_buff_size<ui_inp_size)) )
#else
		if (i_bytes_consumed != i_bytes_read)
#endif
		{
			if ( i_buff_size <= 0 )
	        {
				i_buff_size = 0;
			}

			/* Tell that the input is over in this buffer */
			err_code = (*p_xa_process_api)( xa_process_handle,
											 XA_API_CMD_INPUT_OVER,
											 0,
											 NULL );

			_XA_HANDLE_ERROR( p_proc_err_info, "", err_code );
		}

		/* Set number of bytes to be processed */
		err_code = (*p_xa_process_api)( xa_process_handle,
										XA_API_CMD_SET_INPUT_BYTES,
										0,
										&i_buff_size );

		_XA_HANDLE_ERROR( p_proc_err_info, "", err_code );

#if PROFILE
        xt_iss_client_command("all", "enable");
        times(&start);
#endif

		/* Execute process */
		err_code_exec = (*p_xa_process_api)( xa_process_handle,
											XA_API_CMD_EXECUTE,
											XA_CMD_TYPE_DO_EXECUTE,
											NULL );

#if PROFILE
        times(&stop);
        xt_iss_client_command("all", "disable");
#endif

		_XA_HANDLE_ERROR( p_proc_err_info, "", err_code_exec );

		/* Checking for end of processing */
		err_code = (*p_xa_process_api)( xa_process_handle,
										XA_API_CMD_EXECUTE,
										XA_CMD_TYPE_DONE_QUERY,
										&ui_exec_done );

		_XA_HANDLE_ERROR( p_proc_err_info, "", err_code );

		/* Get the output bytes */
		err_code = (*p_xa_process_api)( xa_process_handle,
									   XA_API_CMD_GET_OUTPUT_BYTES,
									   0,
									   &i_out_bytes );

		_XA_HANDLE_ERROR( p_proc_err_info, "", err_code );

#ifdef STREAM_CHANGE_REINIT_TEST
        if ((err_code_exec == XA_MP3DEC_EXECUTE_NONFATAL_CHANGED_SAMP_FREQ)
               || (err_code_exec == XA_MP3DEC_EXECUTE_NONFATAL_CHANGED_LAYER))
        {
           /* Check if same err. of CHNAGED_XXX has occurred twice consecutively. */
           if (reInit_err_typ == err_code_exec)
           {
                 fprintf(stderr, "going to REINIT\n"); 
                 reInit_err_typ = 0;
                 reInit_active = 1;
                 goto REINIT_POINT;
           }
        }
        /* Update reInit_err_typ variable if the input buff has sufficient data */
        if (err_code_exec != XA_MP3DEC_EXECUTE_NONFATAL_NEED_MORE)
           reInit_err_typ = err_code_exec;
#endif /* STREAM_CHANGE_REINIT_TEST */


#ifdef METADATA_INFO_API //add for using new library
        if (i_out_bytes && (err_code_exec == XA_NO_ERROR)) 
        {

            int orig_or_copy;
            int copyright;
        
            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_MP3DEC_CONFIG_PARAM_ORIGINAL_OR_COPY,
	    		&orig_or_copy);
            _XA_HANDLE_ERROR(p_proc_err_info, "Get Metadata: Original or Copy Error", err_code);
            fprintf(stderr, "Metadata: Original(1) or Copy(0) = %d \n", orig_or_copy);

            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_MP3DEC_CONFIG_PARAM_COPYRIGHT_FLAG,
	    		&copyright);
            _XA_HANDLE_ERROR(p_proc_err_info, "Get Metadata: Copyright_ID Error", err_code);
            fprintf(stderr, "Metadata: Copyright Protected = %d\n", copyright);

			int mch_status;
			err_code = (*p_xa_process_api)(xa_process_handle,
				XA_API_CMD_GET_CONFIG_PARAM,
				XA_MP3DEC_CONFIG_PARAM_MCH_STATUS, &mch_status);
			_XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

            if(mch_status  == 3)
            {
			    xa_mch_ext_hdr_info_t mch_info;

		        err_code = (*p_xa_process_api)(xa_process_handle,
		            XA_API_CMD_GET_CONFIG_PARAM,
		            XA_MP3DEC_CONFIG_PARAM_MCH_EXT_HDR_INFO,
					&mch_info);
		        _XA_HANDLE_ERROR(p_proc_err_info, "Get Metadata: Multichannel Extension Header Info Error", err_code);
		        fprintf(stderr, "Metadata: ext_bit_stream_present = %d\n",         mch_info.ext_bit_stream_present);
				if(mch_info.ext_bit_stream_present){
		        	fprintf(stderr, "Metadata: n_ad_bytes = %d\n",                     mch_info.n_ad_bytes);
				}
		        fprintf(stderr, "Metadata: center = %d\n",                         mch_info.center);
		        fprintf(stderr, "Metadata: surround = %d\n",                       mch_info.surround);
		        fprintf(stderr, "Metadata: lfe = %d\n",                            mch_info.lfe);
		        fprintf(stderr, "Metadata: audio_mix = %d\n",                      mch_info.audio_mix);
		        fprintf(stderr, "Metadata: dematrix_procedure = %d\n",             mch_info.dematrix_procedure);
		        fprintf(stderr, "Metadata: no_of_multi_lingual_ch = %d\n",         mch_info.no_of_multi_lingual_ch);
		        fprintf(stderr, "Metadata: multi_lingual_fs = %d\n",               mch_info.multi_lingual_fs);
		        fprintf(stderr, "Metadata: multi_lingual_layer = %d\n",            mch_info.multi_lingual_layer);

				unsigned char *cpid_ptr = NULL;
		        err_code = (*p_xa_process_api)(xa_process_handle,
		            XA_API_CMD_GET_CONFIG_PARAM,
                    XA_MP3DEC_CONFIG_PARAM_MCH_COPYRIGHT_ID_PTR,
					&cpid_ptr);
                _XA_HANDLE_ERROR(p_proc_err_info, "Get Metadata: Copyright_ID pointer Error", err_code);

				if(cpid_ptr != NULL)
				{
            		fprintf(stderr, "Metadata: Copyright Identifier = %02x \n", (UWORD8)cpid_ptr[0]);
	            	fprintf(stderr, "Metadata: Copyright Number = %02x %02x %02x %02x %02x %02x %02x %02x\n",
								(UWORD8)cpid_ptr[1],
								(UWORD8)cpid_ptr[2],
								(UWORD8)cpid_ptr[3],
								(UWORD8)cpid_ptr[4],
								(UWORD8)cpid_ptr[5],
								(UWORD8)cpid_ptr[6],
								(UWORD8)cpid_ptr[7],
								(UWORD8)cpid_ptr[8]
						);
				}
            }
            fprintf(stderr, "Metadata: ==========================================\n");
		}
#endif /* METADATA_INFO_API */


#if PROFILE // //add for using new library

        /* Assume that if no output was produced, no
        * significant cycles were consumed.
        */
        if (i_out_bytes) {
            clock_t cycles = stop.tms_utime - start.tms_utime;
            int samples = i_out_bytes / (output_wordsize(i_pcm_wd_sz) * i_num_chan);
            double Curr = ((double) cycles / samples * i_samp_freq / 1000000);
            Sum += Curr;
            Ave = Sum / frame;

            if (Peak < Curr) {
                Peak = Curr;
                Peak_frame = frame;
            }

            /* Read the decoded buffer stream position */
            err_code = (*p_xa_process_api)(xa_process_handle,
                XA_API_CMD_GET_CONFIG_PARAM,
                XA_CONFIG_PARAM_GEN_INPUT_STREAM_POS,
                &decoded_frame_strm_pos);

            _XA_HANDLE_ERROR(p_proc_err_info, "Get Decoded Frame Stream Position Error", err_code);

            /*
            Calculate the current time based on the total number of
            samples produced by the decoder and the sample rate:

            time [ms] = samples / sample_rate [kHz]

            Alternatively, the current time can be calculated based on
            the total number of bytes consumed by the decoder and the
            data rate:

            time [ms] = 8 * bytes / data_rate [kbps]
            */
            total_samples += samples;

            unsigned long long total_msec =
                (unsigned long long)((double)total_samples / i_samp_freq * 1000.0);
            int msec = (int)(total_msec % 1000);
            unsigned long long total_seconds = total_msec / 1000;
            int seconds = (int)(total_seconds % 60);
            int minutes = (int)(total_seconds / 60);

            fprintf(stdout, "[%d|%u] %d:%02d.%03d MCPS: %.2f Average: %.2f Peak: %.2f @ [%d]\n",
                frame, decoded_frame_strm_pos, minutes, seconds, msec,
                Curr, Ave, Peak, Peak_frame);
        }
#endif


#if 0
	XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
										pWORD32           pi_bitrate,
										pWORD32           pi_samp_freq,
										pWORD32           pi_num_chan,
										pWORD32           pi_pcm_wd_sz);
		p_get_config_param = xa_mp3_dec_get_config_param;
		WORD32 i_bitrate, i_samp_freq, i_num_chan, i_pcm_wd_sz;
#endif

#if 0
		err_code = (*p_get_config_param)( xa_process_handle,
										&i_bitrate,
										&i_samp_freq,
										&i_num_chan,
										&i_pcm_wd_sz );

		_XA_HANDLE_ERROR( p_proc_err_info, "", err_code );

		if (i_samp_freq != 44100)
		{
			DBG_Printf("err:i samp rate have change:%d\n\r",i_samp_freq);
		}
			
		//asm("break 1,1");
#endif

#if 0
		unsigned int ui_nsamples = i_out_bytes / output_wordsize(i_pcm_wd_sz);
		if (24 == i_pcm_wd_sz)
		{
			DBG_Assert (FALSE);			//24 bit out is not suppoerted

#if 0
			unsigned int i, j;
			for (i = 0, j = 0; i < ui_nsamples; i += 1, j += 3)
			{
				pack_32_to_24_bits(&pb_out_buf[j],
									((WORD32 *) pb_out_buf)[i]);
			}
#endif
		}

		i_total_bytes += (ui_nsamples * i_pcm_wd_sz) / 8;
#endif

		//xa_fwrite((void *)pb_out_buf, (i_pcm_wd_sz/8), ui_nsamples, g_pf_out);

		/* How much buffer is used in input buffers */
		err_code = (*p_xa_process_api)( xa_process_handle,
									   XA_API_CMD_GET_CURIDX_INPUT_BUF,
									   0,
									   &i_bytes_consumed );

		_XA_HANDLE_ERROR( p_proc_err_info, "", err_code );

		//assert(i_bytes_consumed <= i_buff_size);
		DBG_Assert(i_bytes_consumed <= i_buff_size);
		//DBG_Printf("consum:%d\r\n",i_bytes_consumed);
		
#if 0
		//if (app_media_data.playing_stream_vbr == 0)
		{
			err_code = (*p_get_config_param)( xa_process_handle,
											&i_bitrate,
											&i_samp_freq,
											&i_num_chan,
											&i_pcm_wd_sz );

			_XA_HANDLE_ERROR( p_proc_err_info, "", err_code );
			if (err_code == XA_NO_ERROR)
			{
				if ( app_media_data.playing_stream_bitrate != i_bitrate * (1000/8) )
				{
					DBG_Printf("mp3 vbr:%d\n\r", i_bitrate);
					app_media_data.playing_stream_vbr = 1;
				}
			}

			//DBG_Printf("bitrate = %dk\n\r", i_bitrate);
		}
#endif




		#ifdef AUDIO_OUT_CHANNEL_SET_FIXED
		U32 *pSrc = (U32 *)pb_out_buf;
		U32 *pDest = (U32 *)mp3_dec_env.rx_postProcessBuf;
		U32 o_out_len;
		U32 in_sample_speed = app_media_data.playing_stream_sample_rate;
		U8 in_channel = app_media_data.playing_stream_chans;

		audio_out_data_channel_switch(pSrc, pDest, in_sample_speed, in_channel, i_out_bytes, &o_out_len);

		#ifdef MP3_DEOCDING_FRAME
		if (ui_exec_done == FALSE &&  o_out_len > 0)
		{
			*out_buf = (U32 *)pDest;
			*out_len = o_out_len;
			
			return DECODE_SUCCESS;
		}
		else if (ui_exec_done == TRUE)
		{
			*out_buf = (U32 *)pDest;
			*out_len = o_out_len;
			
			return DECODE_END;
		}
		else
		{
			memset(pDest,0,1152*4);
			*out_buf = (U32 *)pDest;
			*out_len = 1152*4;
			
			return DECODE_SUCCESS;
		}
		#else
		/*output bytes*/
		if (ui_exec_done == FALSE &&  i_out_bytes > 0)
		{
			// wait enough space
			if (Decode_OutputBuffer((U32 *)pb_out_buf, i_out_bytes) == FALSE)
			{
				return XA_NO_ERROR;
			}
		}
		#endif


		#else

		#ifdef MP3_DEOCDING_FRAME
		if (ui_exec_done == FALSE &&  i_out_bytes > 0)
		{
			*out_buf = (U32 *)pb_out_buf;
			*out_len = i_out_bytes;
			
			return DECODE_SUCCESS;
		}
		else if (ui_exec_done == TRUE)
		{
			*out_buf = (U32 *)pb_out_buf;
			*out_len = i_out_bytes;
			
			return DECODE_END;
		}
		else
		{
			memset(pb_out_buf,0,1152*4);
			*out_buf = (U32 *)pb_out_buf;
			*out_len = 1152*4;
			
			return DECODE_SUCCESS;
		}
		#else
		/*output bytes*/
		if (ui_exec_done == FALSE &&  i_out_bytes > 0)
		{
			// wait enough space
			if (Decode_OutputBuffer((U32 *)pb_out_buf, i_out_bytes) == FALSE)
			{
				return XA_NO_ERROR;
			}
		}
		#endif
		#endif
	/* Do till the process execution is done */
	} while (1);


	DBG_Printf ("MP3 Decoding Exit\n\r");

	return XA_NO_ERROR;
} /* End xa_main_process() */

extern volatile U8 task_dec_cnt;
void dec_direct_out_for_slave( short *pBuf, U16 len);

//int xa_mp3_decode(void) __INTERNAL_RAM_TEXT;
int xa_mp3_decode(void)
{
#ifdef MP3_DEOCDING_FRAME
	int ret;
	U32 *decoder_buf;
	U32 frame_len;
	U32 eventMask;	

	while (1)
	{
	#ifdef SYSTEM_CRASH_CHECK_ENABLE
		task_dec_cnt = 1;
	#endif
	
#if (SHARE_LINK_ROLE & SL_ROLE_SLAVE)	
		if (app_media_data.share_link_role == SL_ROLE_SLAVE)
		{
			xEventGroupWaitBits(event_grop,AUDIO_DECODE_EVENT, pdTRUE, pdFALSE, portMAX_DELAY);
		}
#endif

		ret = xa_mp3_dec_frame(&decoder_buf, &frame_len);
		if (ret)
		{
			//if (frame_len > 0)
			{
#if defined SRC_NEW_VER
				ret =  Decode_OutputBuffer(decoder_buf, frame_len);
				if (ret == FALSE)
				{
					//asm("nop");
					break;
				}
#elif defined OPTEK_SRC_VER
				optek_src_output (decoder_buf, frame_len);
#endif
			}
		}
		else
		{
			break;
		}

		if (wRequestDecodeFrameExit == TRUE)
		{
			break;
		}
	}

	return ret;

#else
	int ret;

	//ret = xa_mp3_dec_main_process(0, 0x1000);
	ret = xa_mp3_dec_frame();

	return ret;
#endif //MP3_DEOCDING_FRAME
}

/*
int xa_mp3_decode_init(void)
{
	int r;

	r = xa_mp3_dec_init();		//0x1000 no meaning
	
	return r;
}
*/



#if 0//add for using new library
// Set cache attribute to Write Back No Allocate when the last argument is -wbna
void set_wbna(int *argc, char *argv[])
{
    if ( *argc > 1 && !strcmp(argv[*argc-1], "-wbna") ) {
#ifdef __XCC__
        extern char _memmap_cacheattr_wbna_trapnull;
     
        xthal_set_cacheattr((unsigned)&_memmap_cacheattr_wbna_trapnull);
#endif
        (*argc)--;
    }
}

int
    main (int   argc,
    char *argv[])
{
    int err_code = XA_NO_ERROR;

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    xa_testbench_error_handler_init();

    if(argc == 1)
    {
        char pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
        char pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";

        char curr_cmd[XA_MAX_CMD_LINE_LENGTH];

        WORD32 fargc, curpos;
        WORD32 processcmd = 0;

        char fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];
        char *pargv[XA_MAX_ARGS];

        FILE *param_file_id = fopen(PARAMFILE, "r");
        if (param_file_id == NULL)
        {
            MP3DecDisplayCmdLine();

            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
            xa_error_handler(&xa_testbench_error_info,
                "Parameter File", err_code);
            exit(1);
        }

        /* Process one line at a time */
        while(fgets((char *)curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id))
        {
            curpos = 0;
            fargc = 0;
            /* if it is not a param_file command and if */
            /* CLP processing is not enabled */
            if(curr_cmd[0] != '@' && !processcmd)
            {  /* skip it */
                continue;
            }

            while(sscanf((const char *)(curr_cmd + curpos), "%s", fargv[fargc]) != EOF)
            {
                if(fargv[0][0]=='/' && fargv[0][1]=='/')
                    break;
                if(strcmp(fargv[0], "@echo") == 0)
                    break;
                if(strcmp(fargv[fargc], "@New_line") == 0)
                {
                    fgets(curr_cmd + curpos, XA_MAX_CMD_LINE_LENGTH,
                        param_file_id);
                    continue;
                }
                curpos += strlen(fargv[fargc]);
                while(*(curr_cmd + curpos)==' ' || *(curr_cmd + curpos)=='\t')
                    curpos++;
                fargc++;
            }

            if(fargc < 1)  /* for blank lines etc. */
                continue;

            if(strcmp(fargv[0], "@Output_path") == 0)
            {
                if(fargc > 1) strcpy(pb_output_file_path, fargv[1]);
                else strcpy(pb_output_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Input_path") == 0)
            {
                if(fargc > 1) strcpy(pb_input_file_path, fargv[1]);
                else strcpy(pb_input_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Start") == 0)
            {
                processcmd = 1;
                continue;
            }

            if(strcmp(fargv[0], "@Stop") == 0)
            {
                processcmd = 0;
                continue;
            }

            /* otherwise if this a normal command and its enabled for execution */
            if(processcmd)
            {
                int i;

                for(i = 0; i < fargc; i++)
                {
                    fprintf(stdout, "%s ", fargv[i]);
                    pargv[i] = fargv[i];

                    if(!strncmp(fargv[i], "-ifile:", 7))
                    {
                        char *pb_arg_val = fargv[i] + 7;
                        char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                        strcat(pb_input_file_name, pb_input_file_path);
                        strcat(pb_input_file_name, pb_arg_val);

                        g_pf_inp = fopen(pb_input_file_name, "rb");
                        if(g_pf_inp == NULL)
                        {
                            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                            xa_error_handler(&xa_testbench_error_info,
                                "Input File", err_code);
                        }
                    }

                    if(!strncmp(fargv[i], "-ofile:", 7))
                    {
                        char *pb_arg_val = fargv[i] + 7;
                        char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                        strcat(pb_output_file_name, pb_output_file_path);
                        strcat(pb_output_file_name, pb_arg_val);

                        g_pf_out = fopen(pb_output_file_name, "wb");
                        if(g_pf_out == NULL)
                        {
                            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                            xa_error_handler(&xa_testbench_error_info,
                                "Output File", err_code);
                        }
                    }

                }
                g_w_malloc_count = 0;
                fprintf(stdout, "\n");

                if(err_code == XA_NO_ERROR)
                    xa_mp3_dec_main_process(fargc, pargv);

                for(i = 0; i < g_w_malloc_count; i++)
                {
                    if(g_pv_arr_alloc_memory[i])
                        free(g_pv_arr_alloc_memory[i]);
                }

                if(g_pf_inp)
                    fclose(g_pf_inp);
                if(g_pf_out)
                    fclose(g_pf_out);

            }
        }
    }
    else

    {
        int i;
        int file_flag = 0;

#if MEM_INPUT
        file_flag++;
#endif

#if SUPPRESS_OUTPUT
        file_flag++;
#endif

        for(i = 1; i < argc; i++)
        {
            fprintf(stdout, "%s ", argv[i]);
            if(!strncmp(argv[i], "-help", 5))
            {
                MP3DecDisplayCmdLine();
                exit(1);
            }
#if !MEM_INPUT
            if(!strncmp(argv[i], "-ifile:", 7))
            {
                char *pb_arg_val = argv[i] + 7;
                char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                file_flag++;
                err_code = XA_NO_ERROR;
                strcat(pb_input_file_name, pb_arg_val);

                g_pf_inp = fopen(pb_input_file_name, "rb");
                if(g_pf_inp == NULL)
                {
                    fprintf(stdout, "Input file: %s\n", pb_input_file_name);
                    err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                    xa_error_handler(&xa_testbench_error_info,
                        "Input File", err_code);
                }
            }
#endif

#if !SUPPRESS_OUTPUT
            if(!strncmp(argv[i], "-ofile:", 7))
            {
                char *pb_arg_val = argv[i] + 7;
                char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

                file_flag++;
                strcat(pb_output_file_name, pb_arg_val);

                g_pf_out = fopen(pb_output_file_name, "wb");
                if(g_pf_out == NULL)
                {
                    fprintf(stdout, "Output file: %s\n", pb_output_file_name);
                    err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                    xa_error_handler(&xa_testbench_error_info,
                        "Output File", err_code);
                }
            }
#endif /* SUPPRESS_OUTPUT */
        }
        g_w_malloc_count = 0;
        fprintf(stdout, "\n");
        if(file_flag != 2)
        {
            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
            MP3DecDisplayCmdLine();
            xa_error_handler(&xa_testbench_error_info,
                "Input or Output File", err_code);
        }

        if(err_code == XA_NO_ERROR)
            xa_mp3_dec_main_process(argc - 1, &argv[1]);

        for(i = 0; i < g_w_malloc_count; i++)
        {
            if(g_pv_arr_alloc_memory[i])
                free(g_pv_arr_alloc_memory[i]);
        }

        if(g_pf_inp)
            fclose(g_pf_inp);
        if(g_pf_out)
            fclose(g_pf_out);
    }

    return XA_NO_ERROR;
}

void MP3DecDisplayCmdLine(void)
{
    printf("Usage: MP3ExecFileName -ifile:inputfilename -ofile:outputfilename <optional arguments>\n");
    printf("\n");
    printf("The following optional arguments can be given:\n");
    printf("\t[-pcmsz:]  PCM word size 16 or 24. Default value is 16\n");
    printf("\t[-crc:]    Flag to enable header CRC check. Default value is 0\n");
    printf("\t[-mch:]    Flag to enable/disable Multi-Channel decoding. (available only on mp3mch_dec) Default value is 1\n");
    printf("\t[-nonstd:] Relax the standard validity checks for streams. Supported values are:\n");
    printf("\t               0: No relaxation in standard validity checks (Default) \n");
    printf("\t               1: Relax bitrate, mode combination check \n");
    printf("\t[-help]    Prints this text and exit\n");
    printf("\n");
}
#endif //add for using new library



#endif //MP3_DECODE_ENABLE
