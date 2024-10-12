/* Copyright (c) 2007-2012 by Tensilica Inc.  ALL RIGHTS RESERVED.
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


#include "os_config.h"

#include "c_def.h"
#include "oem.h"

#if 1//def SBC_DEC

/* Define ThreadX basic types for this port.  */ 
//#define VOID void

#if 0
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif //0

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#include "xa_sbc_dec_api.h"
#include "xa_sbc_dec_dec.h"

#include "codec.h"

//#include "optek_hifi2_dapp.h"
/*#include "xa_type_def.h"*/
/*#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#include "xa_sbc_dec_api.h"
*/
/*
#include "app_dec.h"
#include "codec.h"
#include "app_main.h"
*/
//#include "codec.h"

#include "fifo.h"

#include "audio_com.h"
#include "audio_codec.h"

//#include "../dsp/optek_src.h"

#ifdef BTDM52_ENABLE

//#define MALLOC_TEST_SET

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 0
#endif	/* SHOW_SIZES */



SBC_DEC_ENV_STRU sbc_dec_env;

void xa_sbc_dec_error_handler_init();
void xa_testbench_error_handler_init();

//extern xa_error_info_struct xa_testbench_error_info;
//extern xa_error_info_struct xa_sbc_dec_error_info;

#if 1
//extern const xa_error_info_struct xa_testbench_error_info;

#ifdef FLAC_DEC_ERROR_DEBUG
extern xa_error_info_struct xa_dec_error_info;
#define xa_sbc_dec_error_info xa_dec_error_info
#else
extern const xa_error_info_struct xa_sbc_dec_error_info;
#endif
#undef _XA_HANDLE_ERROR
#define _XA_HANDLE_ERROR(p_mod_err_info, context, e) \
	if((e) != XA_NO_ERROR) \
	{ \
		xa_sbc_error_handler((p_mod_err_info), (context), (e)); \
		if((e) & XA_FATAL_ERROR) \
		{ \
			return (e); \
		} \
	}
	//asm("break 1,1");
#endif


#if !defined(WAV_HEADER)
#define WAV_HEADER 0
#endif	/* WAV_HEADER */

#if !defined(DISPLAY_MESSAGE)
#define DISPLAY_MESSAGE	0
#endif	/* DISPLAY_MESSAGE */

#if !defined(PROFILE)
#define PROFILE 0
#endif

#if PROFILE
#include <sys/times.h>
#include <xtensa/sim.h>
#endif

#define MAX_MEM_ALLOCS 100
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20

#define PARAMFILE_SBC "paramfilesimple_sbc_dec.txt"

/* This is used to verify the scratch memory behavior */
//#define TEST_SCRATCH

#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED	0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED	0xffff8001


//pVOID g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];
//WORD  g_w_malloc_count;

//FILE *g_pf_inp, *g_pf_out;
static int *g_pf_inp, *g_pf_out;



#if WAV_HEADER

/* Store data in little-endian (Intel) format regardless of the
   target processor byte order. */
void
xa_fwrite (void *buffer, int size, int nwords, FILE *fp)
{
  int i, j;
  char *pb_buf = (char *) buffer;
  
  for (i = 0; i < nwords; i++) {
#if __XTENSA_EL__
    for (j = 0; j < size; j++) {
      putc(pb_buf[i * size + j], fp);
    }
#else
    for (j = size - 1; j >= 0; j--) {
      putc(pb_buf[i * size + j], fp);
    }
#endif
  }
}


static void
write16_bits_lh(FILE *fp, WORD32 i)
{
  putc(i & 0xff, fp);
  putc((i >> 8) & 0xff, fp);
}


static void
write32_bits_lh(FILE *fp, WORD32 i)
{
  write16_bits_lh(fp, (WORD32)(i & 0xffffL));
  write16_bits_lh(fp, (WORD32)((i >> 16) & 0xffffL));
}

WORD32
write_wav_header (FILE *fp, /* file to write */
		  WORD32 pcmbytes, /* total bytes in the wav file */
		  WORD32 freq, /* sample rate */
		  WORD32 channels, /* output channels */
		  WORD32 bits /* bits per sample */)
{
  WORD32 bytes = (bits + 7) / 8;
  fwrite("RIFF", 1, 4, fp); /* label */
  write32_bits_lh(fp, pcmbytes + 44 - 8); /* length in bytes without header */
  fwrite("WAVEfmt ", 1, 8, fp); /* 2 labels */
  write32_bits_lh(fp, 2 + 2 + 4 + 4 + 2 + 2); /* length of PCM format decl area */
  write16_bits_lh(fp, 1); /* is pcm? */
  write16_bits_lh(fp, channels);
  write32_bits_lh(fp, freq);
  write32_bits_lh(fp, freq * channels * bytes); /* bps */
  write16_bits_lh(fp, channels * bytes);
  write16_bits_lh(fp, bits);
  fwrite("data", 1, 4, fp);
  write32_bits_lh(fp, pcmbytes);

  return (ferror(fp) ? -1 : 0);
}

#endif /* WAV_HEADER */

#if 0
void
usage()
{
  fprintf(stdout,
	  "\n"
	  "Usage:  xt-run <binfile> [options] -ifile:<inputfile> -ofile:<outputfile>\n"
	  "        <binfile>     HiFi2 SBC Decoder executable\n"
	  "        <inputfile>   Input SBC file\n"
#if WAV_HEADER
	  "        <outputfile>  Output WAV file\n"
#else
	  "        <outputfile>  Output PCM file\n"
#endif
	  "        -h            Print this help message\n");
  exit(0);
}
#endif

XA_ERRORCODE
xa_sbc_dec_get_config_param (xa_codec_handle_t p_xa_process_api_obj,
			     pWORD32           pi_bitrate,
			     pWORD32           pi_samp_freq,
			     pWORD32           pi_num_chan,
			     pWORD32           pi_pcm_wd_sz)
{
  XA_ERRORCODE err_code = XA_NO_ERROR;
  /* the process API function */
  xa_codec_func_t *p_xa_process_api = xa_sbc_dec;
  xa_error_info_struct *p_proc_err_info = &xa_sbc_dec_error_info;

  /* Data rate */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_SBC_DEC_CONFIG_PARAM_BITRATE, pi_bitrate);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  }
  /* Sampling frequency */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_SBC_DEC_CONFIG_PARAM_SAMP_FREQ, pi_samp_freq);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  }
  /* Number of channels */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_SBC_DEC_CONFIG_PARAM_NUM_CHANNELS, pi_num_chan);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
  }
  /* PCM word size */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_SBC_DEC_CONFIG_PARAM_PCM_WDSZ, pi_pcm_wd_sz);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
  }
  return XA_NO_ERROR;
}


static unsigned int
output_wordsize (unsigned int sample_bits)
{
  /* Round up to the next 2-byte size: 16 -> 2; 24 -> 4. */
  return 2 * ((sample_bits + 15) / 16);
}

/* API obj */
static xa_codec_handle_t xa_process_handle;

pWORD8 pmic_buf;
static void *upStreamHandle;

static pWORD8 pb_inp_buf = NULL;
static pWORD8 pb_out_buf = NULL;

static UWORD32 ui_inp_size;
static WORD32 i_bytes_consumed;
static WORD32 i_bytes_read;
static WORD32 i_buff_size;

WORD8 fist_frame;

XA_ERRORCODE
xa_sbc_dec_init(void)
{
  LOOPIDX i;

  /* Error code */
  XA_ERRORCODE err_code = XA_NO_ERROR;

  /* First part                                        */
  /* Error Handler Init                                */
  /* Get Library Name, Library Version and API Version */
  /* Initialize API structure + Default config set     */
  /* Set config params from user                       */
  /* Initialize memory tables                          */
  /* Get memory information and allocate memory        */

  /* Memory variables */
  //int frameCounter = 0;
 UWORD32 n_mems = 0;
  UWORD32 ui_proc_mem_tabs_size = 0;
  /* API size */
  UWORD32 pui_api_size = 0;
  /* Process initing done query variable */
  UWORD32 ui_init_done = 0; 

  WORD32 i_bitrate = 0, i_samp_freq = 0, i_num_chan = 0, i_pcm_wd_sz = 0;
  pVOID pvoid;

  /* The process API function */
  xa_codec_func_t *p_xa_process_api;
  void *dsp_buf;

  /* The get config from API */
  XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
				      pWORD32           pi_bitrate,
				      pWORD32           pi_samp_freq,
				      pWORD32           pi_num_chan,
				      pWORD32           pi_pcm_wd_sz);

  /* The error init function */
  VOID (*p_error_init)();

  /* The process error info structure */
  xa_error_info_struct *p_proc_err_info;

  /* Stack process struct initing */
  p_xa_process_api = xa_sbc_dec;
  p_get_config_param = xa_sbc_dec_get_config_param;
  p_error_init = xa_sbc_dec_error_handler_init;
  p_proc_err_info = &xa_sbc_dec_error_info;
  /* Stack process struct initing end */

  /* ******************************************************************/
  /* Initialize the error handler                                     */
  /* ******************************************************************/
  (*p_error_init)();

  //not use malloc
  enableCoProcessor (CO_PROCESSOR_MASK_BITS);

  //codec_malloc_init();
  codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

  //g_w_malloc_count = 0;

  /* ******************************************************************/
  /* Get the library name, library version and API version            */
  /* ******************************************************************/

#if DISPLAY_MESSAGE
  /* Get the library name string */
  err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
				 XA_CMD_TYPE_LIB_NAME, pb_process_name);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* Get the library version string */
  err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_LIB_ID_STRINGS,
				 XA_CMD_TYPE_LIB_VERSION, pb_lib_version);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* Display the Tensilica identification message */
  DBG_Printf("\n%s version %s\n", pb_process_name, pb_lib_version);
  DBG_Printf("Tensilica, Inc. http://www.tensilica.com\n\n");
#endif

  DBG_Printf("ver: %s\n", sbc_decode_version_get());

  /* ******************************************************************/
  /* Initialize API structure and set config params to default        */
  /* ******************************************************************/

  /* Get the API size */
  err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_API_SIZE, 0,
				 &pui_api_size);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* Allocate memory for API */
  //g_pv_arr_alloc_memory[g_w_malloc_count] = (pVOID)codec_malloc(pui_api_size);
  
  #ifdef MALLOC_TEST_SET
  pvoid = (pVOID)codec_malloc_w_memtype(CODEC_MEMTYPE_SLOW, pui_api_size, CODEC_MEM_ALIGN_16);
  #else
  pvoid = (pVOID)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, pui_api_size, CODEC_MEM_ALIGN_16);
  #endif

  if (pvoid == NULL) {
    _XA_HANDLE_ERROR(&xa_testbench_error_info, "API struct alloc",
		     XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
  }

  /* API object requires 4 bytes (WORD32) alignment;
   * malloc() provides at least 8-byte alignment.
   */
  DBG_Assert((((unsigned int) pvoid) & 3) == 0);

  /* Set API object with the memory allocated */
  xa_process_handle = (void *) pvoid;

  //g_w_malloc_count++;

  /* Set the config params to default values */
  err_code = (*p_xa_process_api)(xa_process_handle,
				 XA_API_CMD_INIT,
				 XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS,
				 NULL);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* ******************************************************************/
  /* Initialize Memory info tables                                    */
  /* ******************************************************************/

  /* Get memory info tables size */
  err_code = (*p_xa_process_api)(xa_process_handle,
				 XA_API_CMD_GET_MEMTABS_SIZE, 0,
				 &ui_proc_mem_tabs_size);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  #ifdef MALLOC_TEST_SET
  pvoid = (pVOID)codec_malloc_w_memtype(CODEC_MEMTYPE_SLOW, ui_proc_mem_tabs_size, CODEC_MEM_ALIGN_16);
  #else
  pvoid = (pVOID)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, ui_proc_mem_tabs_size, CODEC_MEM_ALIGN_16);
  #endif

  if(pvoid == NULL) {
    _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
		     XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
  }

  /* Memory table requires 4 bytes (WORD32) alignment; malloc()
   * provides at least 8-byte alignment.
   */
  DBG_Assert((((unsigned int) pvoid) & 3) == 0);

  /* Set pointer for process memory tables	*/
  err_code = (*p_xa_process_api)(xa_process_handle,
				 XA_API_CMD_SET_MEMTABS_PTR, 0,
				 (void *) pvoid);

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

  for (i = 0; i < (WORD32) n_mems; i++) {
    int ui_size, ui_alignment, ui_type;
    pVOID pv_alloc_ptr;

    /* Get memory size */
    err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_GET_MEM_INFO_SIZE,
				   i,
				   &ui_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#ifdef TEST_SCRATCH
    if (i == 1)
    {
        scratch_size = ui_size;
    }
#endif

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

    if(ui_type == XA_MEMTYPE_OUTPUT)
    {
      ui_size *= 2;
    }

    #ifdef MALLOC_TEST_SET
    pvoid = (pVOID)codec_malloc_w_memtype(CODEC_MEMTYPE_SLOW, ui_size, CODEC_MEM_ALIGN_16);
    #else
    pvoid = (pVOID)codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, ui_size, CODEC_MEM_ALIGN_16);
    #endif

    if(pvoid == NULL) {
      _XA_HANDLE_ERROR(&xa_testbench_error_info, "Mem tables alloc",
		       XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* The following assertion should never fail because malloc() on
     * Xtensa always returns memory aligned on at least an 8-byte
     * boundary.
     */
    DBG_Assert((((unsigned int) pvoid) % ui_alignment) == 0);

    pv_alloc_ptr = (void *) pvoid;

    //g_w_malloc_count++;

    /* Set the buffer pointer */
    err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_SET_MEM_PTR,
				   i,
				   pv_alloc_ptr);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    if(ui_type == XA_MEMTYPE_INPUT) {
      pb_inp_buf = pv_alloc_ptr;
      ui_inp_size = ui_size;
    }
    if(ui_type == XA_MEMTYPE_OUTPUT) {
      pb_out_buf = pv_alloc_ptr;
    }

#if 1//SHOW_SIZES
    switch (ui_type) {
    case XA_MEMTYPE_INPUT:
      DBG_Printf("Input buffer size: %u bytes\n", ui_size);
      break;
    case XA_MEMTYPE_OUTPUT:
      DBG_Printf("Output buffer size: %u bytes\n", ui_size);
      break;
    case XA_MEMTYPE_SCRATCH:
      DBG_Printf("Scratch buffer size: %u bytes\n", ui_size);
      break;
    case XA_MEMTYPE_PERSIST:
      DBG_Printf("Persistent buffer size: %u bytes\n", ui_size);
      break;
    default:
      DBG_Assert(0);
    }
#endif	/* SHOW_SIZES */
  }

  /* End first part */

  i_bytes_consumed = ui_inp_size;

  /* Second part        */
  /* Initialize process */
  /* Get config params  */

  /* ******************************************************************/
  /* Initialize process in a loop (to handle junk data at beginning)  */
  /* ******************************************************************/
  i_buff_size = ui_inp_size;

  do {
      for(i = 0; i < (WORD32)ui_inp_size - i_bytes_consumed; i++) {
        pb_inp_buf[i] = pb_inp_buf[i + i_bytes_consumed];
      }

      i_bytes_read = xa_fread_bt(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
        sizeof(WORD8),
        i_bytes_consumed,
        g_pf_inp);

      /* New buffer size */
      i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);


    if( i_buff_size <= 0 ) {
      i_buff_size = 0;

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

  } while (!ui_init_done);
  /* Init end of process condition end */

  /* ******************************************************************/
  /* Get config params from API                                       */
  /* ******************************************************************/

  err_code = (*p_get_config_param)(xa_process_handle,
				   &i_bitrate,
				   &i_samp_freq,
				   &i_num_chan,
				   &i_pcm_wd_sz);
  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  //app_main_data.playing_stream_bitrate = i_bitrate;
  //app_main_data.playing_stream_sample_rate = i_samp_freq;
  //app_main_data.playing_stream_chans = i_num_chan;
  //app_main_data.playing_stream_sample_bits = i_pcm_wd_sz;

  audio_in_stream.channel = i_num_chan;
  audio_in_stream.bit_rate = i_bitrate;
  audio_in_stream.bit_width = i_pcm_wd_sz;
  audio_in_stream.sample_speed = i_samp_freq;
  audio_in_stream.sample_rate = i_samp_freq;
  audio_in_stream.sample_speed = audio_stream_speed_get(i_samp_freq);
  audio_clock_freq_set(i_samp_freq);


  int size = (BT_SBC_BLOCK_SIZE*4*4);
  sbc_dec_env.rx_postProcessBuf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

  DBG_Printf("\nData Rate: %d bps\n", i_bitrate);
  DBG_Printf("Sample Rate: %d Hz\n", i_samp_freq);
  DBG_Printf("Number of Channels: %d\n", i_num_chan);
  DBG_Printf("PCM Sample Size: %d bits\n", i_pcm_wd_sz);
  DBG_Printf("Standard: SBC\n");

  fist_frame = TRUE;

  /* End second part */
  return XA_NO_ERROR;
} /* End xa_main_process() */

//XA_ERRORCODE xa_sbc_dec_frame (U8 **pout,U16 *plen) __attribute__ ((section (".internal_ram_1_text")));

XA_ERRORCODE
//xa_sbc_dec_main_process (WORD32  argc,
//			 char   *argv[])
xa_sbc_dec_frame (U8 **pout,U16 *plen)
{
  //*pout = pb_out_buf;
  //*plen = 512;
  //return DECODE_SUCCESS;  

  LOOPIDX i;

  /* Error code */
  XA_ERRORCODE err_code = XA_NO_ERROR;
  XA_ERRORCODE err_code_exec = XA_NO_ERROR;

  UWORD32 ui_exec_done = 0;

  WORD32 i_out_bytes = 0;
  WORD32 i_bitrate = 0, i_samp_freq = 0, i_num_chan = 0, i_pcm_wd_sz = 0;

  /* The process API function */
  xa_codec_func_t *p_xa_process_api;

  /* The get config from API */
  XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
				      pWORD32           pi_bitrate,
				      pWORD32           pi_samp_freq,
				      pWORD32           pi_num_chan,
				      pWORD32           pi_pcm_wd_sz);


  /* The process error info structure */
  xa_error_info_struct *p_proc_err_info;

  /* Stack process struct initing */
  p_xa_process_api = xa_sbc_dec;
  p_get_config_param = xa_sbc_dec_get_config_param;

  p_proc_err_info = &xa_sbc_dec_error_info;

  /* Third part for executing each process */
  do {
    /* Execute process */

    i_buff_size -= i_bytes_consumed;
#if 0
    for(i = 0; i < (WORD32)ui_inp_size - i_bytes_consumed; i++) {
      pb_inp_buf[i] = pb_inp_buf[i + i_bytes_consumed];
    }
#else
    memcpy(pb_inp_buf,pb_inp_buf+i_bytes_consumed,i_buff_size);
#endif
    i_bytes_read = xa_fread_bt(pb_inp_buf + i_buff_size,
			 sizeof(char),
			 ui_inp_size - i_buff_size,
			 g_pf_inp);
/*
    if (i_bytes_read != (ui_inp_size - i_buff_size))
    {
      DBG_printf("read err:%d,%d\r\n",i_bytes_read,(ui_inp_size - i_buff_size));
    }
*/
    /* New buffer size */
    //i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);
     i_buff_size +=  i_bytes_read;

    if( i_buff_size <= 0 ) {

		//asm ("break 1,1");

      i_buff_size = 0;

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

#if PROFILE
    xt_iss_client_command("all","enable");
    times(&start);
#endif

    /* Execute process */
    err_code_exec = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_EXECUTE,
				   XA_CMD_TYPE_DO_EXECUTE,
				   NULL);

#if PROFILE
    times(&stop);
    xt_iss_client_command("all","disable");
#endif

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code_exec);

    /* Checking for end of processing */
    err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_EXECUTE,
				   XA_CMD_TYPE_DONE_QUERY,
				   &ui_exec_done);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Get the output bytes */
    err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_GET_OUTPUT_BYTES,
				   0,
				   &i_out_bytes);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


    err_code = (*p_get_config_param)(xa_process_handle,
				     &i_bitrate,
				     &i_samp_freq,
				     &i_num_chan,
				     &i_pcm_wd_sz);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

#if 0
	if (audio_in_stream.playing_stream_sample_rate != i_samp_freq) {
		//different sample freq
		audio_in_stream.playing_stream_sample_rate = i_samp_freq;
	}

	audio_in_stream.playing_stream_bitrate = i_bitrate;
	audio_in_stream.playing_stream_chans = i_num_chan;
	audio_in_stream.playing_stream_sample_bits = i_pcm_wd_sz;
#endif
    
/*    unsigned int ui_nsamples = i_out_bytes / output_wordsize(i_pcm_wd_sz);
    
    i_total_bytes += (ui_nsamples * i_pcm_wd_sz) / 8;
 */   
    //xa_fwrite((pVOID)pb_out_buf, (i_pcm_wd_sz/8), ui_nsamples, g_pf_out);
 

    /* How much buffer is used in input buffers */
    err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_GET_CURIDX_INPUT_BUF,
				   0,
				   &i_bytes_consumed);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    DBG_Assert(i_bytes_consumed <= i_buff_size);  

    /*output bytes*/
    if (ui_exec_done == FALSE &&  i_out_bytes > 0)
    {
      //ui_exec_done = TRUE;
      /*    
      if (audio_in_stream.playing_stream_chans == 1)
      {
        int i;
        short *buf = pb_out_buf;
        //mono to stereo
        for(i=0; i<i_out_bytes/2; i++)
        {
          buf[i_out_bytes - 2*i -1] = buf[i_out_bytes/2 - i - 1];
          buf[i_out_bytes - 2*i -2] = buf[i_out_bytes/2 - i - 1];
        }

        i_out_bytes *= 2;
      }
      */    		

      //dec_direct_out (pb_out_buf,i_out_bytes);
      #ifdef AUDIO_OUT_CHANNEL_SET_FIXED

      U32 *pSrc = (U32 *)pb_out_buf;
      U32 *pDest = (U32 *)sbc_dec_env.rx_postProcessBuf;   
      int len = i_out_bytes;
      int out_len;
      int in_sample_speed = audio_in_stream.sample_speed;
      int in_channel = audio_in_stream.channel;

      audio_out_data_channel_switch(pSrc, pDest, in_sample_speed, in_channel, len, &out_len);
      *pout = (U8 *)pDest;
      *plen = out_len;      
      #else

    #ifdef ADC_DAC_DATA_WIDTH_24_BIT

      optek_hifi2_16b_to_24b(pb_out_buf, pb_out_buf, i_out_bytes/2);
      i_out_bytes *= 2;
    #endif
      *pout = pb_out_buf;
      *plen = i_out_bytes;
      #endif
      break;
    
    }
    /* Do till the process execution is done */
  } while(!ui_exec_done);

  if (ui_exec_done)
  {
    return DECODE_END;
  }
    
  else
    return DECODE_SUCCESS;
  //return XA_NO_ERROR;
} /* End xa_main_process() */

void sbc_dec_fini(int reason)
{
	DBG_Printf("%s", __func__);
  audio_env.playing_stream_status = STREAM_WAITING_DATA;
}

int sbc_dec_info_get(DECODE_INFO_STRU *info)
{
	info->src_speed = 0;
	return 1;
}

static const DECODE_DEVICE_STRU sbc_decode = 
{
  DECODE_SBC,
	"sbc decode",	
	xa_sbc_dec_init,
	sbc_dec_fini,
	xa_sbc_dec_frame,
  sbc_dec_info_get
};

void sbc_decocde_set(void)
{
  DBG_Printf("%s\r\n", __func__);
  decode_set_msgSend(&sbc_decode);
}

void sbc_decocde_start_w_init(void)
{
	DBG_Printf("%s", __func__);
  decode_start_w_init_msgSend();
}

#endif //BTDM52_ENABLE
#endif  //SBC_DEC
