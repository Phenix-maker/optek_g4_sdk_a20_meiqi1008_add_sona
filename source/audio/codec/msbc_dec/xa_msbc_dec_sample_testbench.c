/*
 * Copyright (c) 2012-2016 Cadence Design Systems, Inc.
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

/* Define ThreadX basic types for this port.  */ 

#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "regmap.h"
#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#include "xa_msbc_dec_api.h"

#include "audio_com.h"

#include "codec.h"

#include "fifo.h"

#include "audio_com.h"
#include "audio_codec.h"

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif	/* SHOW_SIZES */


#ifdef mSBC_DEC

#if 1
VOID xa_msbc_dec_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_msbc_dec_testbench_error_info;
extern xa_error_info_struct xa_msbc_dec_error_info;


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

#define PARAMFILE_MSBC ".\\test\\build\\paramfilesimple_msbc_dec.txt"

/* This is used to verify the scratch memory behavior */
//#define TEST_SCRATCH

#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED	0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED	0xffff8001
#define XA_TESTBENCH_MFMAN_FATAL_CONFIGURATION      0xffff8002

/* Maximum supported loss pattern length in this testbench */
#define MAX_LOSS_PATTERN_LENGTH 2000

#define SBC_FRAME_LOST     0x6B20
#define SBC_FRAME_NORMAL   0x6B21

#define MIN(a,b) ((a>b) ? b : a)

//FILE *g_pf_inp, *g_pf_out;

//FILE *g_pf_loss_pattern;

void *g_pf_inp = NULL, *g_pf_out = NULL;

void *g_pf_loss_pattern = NULL;

#define assert DBG_Assert
#define malloc codec_malloc

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


XA_ERRORCODE
xa_msbc_dec_get_config_param (xa_codec_handle_t p_xa_process_api_obj,
			     pWORD32           pi_bitrate,
			     pWORD32           pi_samp_freq,
			     pWORD32           pi_num_chan,
			     pWORD32           pi_pcm_wd_sz)
{
  XA_ERRORCODE err_code = XA_NO_ERROR;
  /* the process API function */
  xa_codec_func_t *p_xa_process_api = xa_msbc_dec;
  xa_error_info_struct *p_proc_err_info = &xa_msbc_dec_error_info;

  /* Data rate */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_MSBC_DEC_CONFIG_PARAM_BITRATE, pi_bitrate);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  }
  /* Sampling frequency */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_MSBC_DEC_CONFIG_PARAM_SAMP_FREQ, pi_samp_freq);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  }
  /* Number of channels */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_MSBC_DEC_CONFIG_PARAM_NUM_CHANNELS, pi_num_chan);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
  }
  /* PCM word size */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_MSBC_DEC_CONFIG_PARAM_PCM_WDSZ, pi_pcm_wd_sz);
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

static xa_codec_handle_t xa_process_handle;

static UWORD32 ui_inp_size = 0;
static pWORD8 pb_inp_buf, pb_out_buf;

const WORD8 msbc_2frame_data[] = 
{
  0xad,0x0,0x0,0xd,0xed,0xbb,0xcb,0xcc,0x7d,0xdb,
  0x6b,0x5f,0x76,0xda,0xd7,0x5d,0xb6,0xb5,0xc7,0x71,
  0xad,0x35,0x4b,0x95,0x82,0xcb,0x6d,0x63,0x50,0x84,
  0xc6,0x55,0x71,0x49,0x82,0x23,0x6b,0x5f,0x76,0xda,
  0xd7,0xdd,0xb6,0xb5,0xf7,0x6d,0xad,0x7d,0xdb,0x6b,
  0x5f,0x76,0xda,0xd7,0xdd,0xb6,0xb4,

  0xad,0x0,0x0,0xe0,0x10,0x0,0x0,0x0,0x17,0xb6,
  0xd5,0x41,0xcd,0xb5,0x51,0x9b,0x6d,0x54,0x70,0x93,
  0x55,0x7,0x36,0xd5,0x47,0x29,0xb5,0x51,0x2b,0x6d,
  0x54,0x36,0xdb,0x55,0x21,0x34,0xd5,0x4a,0x2d,0xb5,
  0x51,0xb3,0x6d,0x54,0x7a,0xdb,0x55,0x1e,0xb6,0xd5,
  0x43,0x8d,0xb5,0x50,0xf3,0x6d,0x54,
};

int xa_msbc_dec_init (void)
{
  LOOPIDX i;

#if DISPLAY_MESSAGE
  /* Library Info and Identification strings */
  WORD8 pb_process_name[30] = "";
  WORD8 pb_lib_version[30] = "";
#endif

  /* Error code */
  XA_ERRORCODE err_code = XA_NO_ERROR;
  XA_ERRORCODE err_code_exec = XA_NO_ERROR;

  /* API obj */
//  xa_codec_handle_t xa_process_handle = NULL;

  WORD32 loss_pattern_length;
  WORD32 loss_pattern_idx;
  WORD32 loss_pattern_size;
  pWORD16 loss_pattern;
  UWORD32 plc_option;
  xa_msbc_frame_type_t frame_type;
  loss_pattern_length = 0;
  loss_pattern_idx = 0;
  loss_pattern_size = MAX_LOSS_PATTERN_LENGTH * sizeof(WORD16);
  loss_pattern = NULL;
  plc_option = 1;
  frame_type = XA_MSBC_GOOD_FRAME;

  short *hfp_outbuf;

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
  UWORD32 ui_exec_done;
//  pWORD8 pb_inp_buf = NULL;
//  pWORD8 pb_out_buf = NULL;

  UWORD32 ui_inp_size = 0;
  WORD32 i_bytes_consumed;
  WORD32 i_bytes_read;
  WORD32 i_buff_size;

  WORD32 i_out_bytes = 0;
  WORD32 i_total_bytes = 0;
  WORD32 i_bitrate, i_samp_freq, i_num_chan, i_pcm_wd_sz;

  /* The process API function */
  xa_codec_func_t *p_xa_process_api;
  pVOID pvoid;

  /* The get config from API */
  XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
				      pWORD32           pi_bitrate,
				      pWORD32           pi_samp_freq,
				      pWORD32           pi_num_chan,
				      pWORD32           pi_pcm_wd_sz);

  /* The error init function */
  VOID (*p_error_init)();

  //not use malloc
  enableCoProcessor (CO_PROCESSOR_MASK_BITS);  
  //mscb_malloc_init();

  /* The process error info structure */
  xa_error_info_struct *p_proc_err_info;

  /* Stack process struct initing */
  p_xa_process_api = xa_msbc_dec;
  p_get_config_param = xa_msbc_dec_get_config_param;
  p_error_init = xa_msbc_dec_error_handler_init;
  p_proc_err_info = &xa_msbc_dec_error_info;
  /* Stack process struct initing end */

  /* ******************************************************************/
  /* Initialize the error handler                                     */
  /* ******************************************************************/
  (*p_error_init)();

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
  DBG_Printf( "\n%s version %s\n", pb_process_name, pb_lib_version);
  DBG_Printf( "Tensilica, Inc. http://www.tensilica.com\n\n");
#endif

#if 0
    /**************************************************************************
     * Verify the PLC parameters                                               *
     **************************************************************************/
    /* If there is only 2 parameters, no loss param file or -noplc flag were
     * indicated, PLC is enabled by default.
     */
    for (i=1; i<argc; i++)
    {
        if (strncmp(argv[i], "-noplc", 6) == 0)
        {
            plc_option = 0;
        }
    }
#else
	plc_option = 1;
#endif
#if 0
    /* Allocate memory to the buffer that holds the loss_pattern */
    loss_pattern  = (pWORD16) malloc(loss_pattern_size * sizeof(WORD16));

    if (loss_pattern == NULL)
    {
        _XA_HANDLE_ERROR(
            &xa_msbc_dec_testbench_error_info,
            "API loss pattern buffer alloc",
            XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* Read the Loss Pattern File */
    if (g_pf_loss_pattern != NULL)
    {
        /* Read the entire loss pattern file */
        loss_pattern_length = 0;

        while (fread(
                &loss_pattern[loss_pattern_length],
                sizeof(WORD16),
                1,
                g_pf_loss_pattern) == 1 )
        {
#if __XTENSA_EB__
            loss_pattern[loss_pattern_length] =
                AE_SHA32(loss_pattern[loss_pattern_length]);
#endif
            loss_pattern_length++;

            if ( loss_pattern_length >= MAX_LOSS_PATTERN_LENGTH )
            {
                _XA_HANDLE_ERROR(
                    &xa_msbc_dec_testbench_error_info,
                    "API loss pattern too long",
                    XA_TESTBENCH_MFMAN_FATAL_CONFIGURATION);
            }
        }
        fprintf(stderr, "Loss pattern length: %d\n", loss_pattern_length);
    }
    else
    {
        /* Assume no loss of packets */
        loss_pattern_length = 1;
        loss_pattern[0] = SBC_FRAME_NORMAL;
    }
#endif
  /* ******************************************************************/
  /* Initialize API structure and set config params to default        */
  /* ******************************************************************/

  /* Get the API size */
  err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_API_SIZE, 0,
				 &pui_api_size);
#if SHOW_SIZES
        DBG_Printf("\nAPI structure size: %u bytes\n", pui_api_size);
#endif

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* Allocate memory for API */
  pvoid = malloc(pui_api_size);

  if (pvoid == NULL) {
    _XA_HANDLE_ERROR(&xa_msbc_dec_testbench_error_info, "API struct alloc",
		     XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
  }

  /* API object requires 4 bytes (WORD32) alignment;
   * malloc() provides at least 8-byte alignment.
   */
  assert((((unsigned int) pvoid) & 3) == 0);

  /* Set API object with the memory allocated */
  xa_process_handle = (void *) pvoid;

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
#if SHOW_SIZES
        DBG_Printf("\nMEMTABS size: %u bytes\n\n", ui_proc_mem_tabs_size);
#endif

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


  pvoid = malloc(ui_proc_mem_tabs_size);

  if(pvoid == NULL) {
    _XA_HANDLE_ERROR(&xa_msbc_dec_testbench_error_info, "Mem tables alloc",
		     XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
  }

  /* Memory table requires 4 bytes (WORD32) alignment; malloc()
   * provides at least 8-byte alignment.
   */
  assert((((unsigned int) pvoid) & 3) == 0);

  /* Set pointer for process memory tables	*/
  err_code = (*p_xa_process_api)(xa_process_handle,
				 XA_API_CMD_SET_MEMTABS_PTR, 0,
				 (void *) pvoid);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

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

    pvoid = malloc(ui_size);

    if(pvoid == NULL) {
      _XA_HANDLE_ERROR(&xa_msbc_dec_testbench_error_info, "Mem tables alloc",
		       XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* The following assertion should never fail because malloc() on
     * Xtensa always returns memory aligned on at least an 8-byte
     * boundary.
     */
    assert((((unsigned int) pvoid) % ui_alignment) == 0);

    pv_alloc_ptr = (void *) pvoid;


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
//	  hfp_outbuf = malloc(ui_size*3*2*2);
    }

#if SHOW_SIZES
    switch (ui_type) {
    case XA_MEMTYPE_INPUT:
      DBG_Printf( "Input buffer size: %u bytes\n", ui_size);
      break;
    case XA_MEMTYPE_OUTPUT:
      DBG_Printf( "Output buffer size: %u bytes\n", ui_size);
      break;
    case XA_MEMTYPE_SCRATCH:
      DBG_Printf( "Scratch buffer size: %u bytes\n", ui_size);
      break;
    case XA_MEMTYPE_PERSIST:
      DBG_Printf( "Persistent buffer size: %u bytes\n", ui_size);
      break;
    default:
      assert(0);
    }
#endif	/* SHOW_SIZES */
  }

  /* End first part */

  i_bytes_consumed = ui_inp_size;
	int size;
	int rlen;

  /* Second part        */
  /* Initialize process */
  /* Get config params  */

  /* ******************************************************************/
  /* Initialize process in a loop (to handle junk data at beginning)  */
  /* ******************************************************************/
  i_buff_size = ui_inp_size;

  memcpy(pb_inp_buf,msbc_2frame_data,sizeof(msbc_2frame_data));

 i_buff_size = 57;
  do {

    /* Set number of bytes to be processed */
    err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_SET_INPUT_BYTES,
				   0,
				   &i_buff_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Send the PLC information to the API */
    err_code = (*p_xa_process_api)(xa_process_handle,
                   XA_API_CMD_SET_CONFIG_PARAM,
                   XA_MSBC_DEC_CONFIG_PARAM_PLC_OPTION,
                   &plc_option);

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

  DBG_Printf( "\nData Rate: %d bps\n", i_bitrate);
  DBG_Printf( "Sample Rate: %d Hz\n", i_samp_freq);
  DBG_Printf( "Number of Channels: %d\n", i_num_chan);
  DBG_Printf( "PCM Sample Size: %d bits\n", i_pcm_wd_sz);
  DBG_Printf( "Standard: mSBC\n");
  DBG_Printf( "PLC %s\n", (plc_option == 1) ? "enabled" : "disabled");

  /* End second part */
} /* end xa_param_file_process */
int xa_msbc_dec_frame(U8 *pInBuf, U16 inLen, U8 **pOutBuf, U32 *pOutDataLen) __attribute__ ((section (".internal_ram_1_text")));
int xa_msbc_dec_frame(U8 *pInBuf, U16 inLen, U8 **pOutBuf, U32 *pOutDataLen)
{
  /* Error code */
  XA_ERRORCODE err_code = XA_NO_ERROR;
  XA_ERRORCODE err_code_exec = XA_NO_ERROR;  
  WORD32 i_bytes_consumed;
  WORD32 i_buff_size;
  WORD32 i_out_bytes = 0;
  UWORD32 ui_exec_done;
  xa_msbc_frame_type_t frame_type;

  xa_codec_func_t *p_xa_process_api;

  /* The process error info structure */
  xa_error_info_struct *p_proc_err_info;

  /* Stack process struct initing */
  p_xa_process_api = xa_msbc_dec;
  p_proc_err_info = &xa_msbc_dec_error_info;

//printf_info("%d", pInBuf[0]);
    if (pInBuf[0] == 0xAD)
    {
      //*pOutDataLen = 0;
      //*pOutBuf = 0;
      //return;
      frame_type = XA_MSBC_GOOD_FRAME;
    }
    else
    {
  		frame_type = XA_MSBC_BAD_FRAME;
      //DBG_Printf("lost frame\r\n");
    }

    memcpy(pb_inp_buf,pInBuf,57);
    /* New buffer size */
    i_buff_size = 57;

    /* Set number of bytes to be processed */
    err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_SET_INPUT_BYTES,
				   0,
				   &i_buff_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    /* Send the frame type information to the API */
    err_code = (*p_xa_process_api)(xa_process_handle,
                   XA_API_CMD_SET_CONFIG_PARAM,
                   XA_MSBC_DEC_CONFIG_PARAM_FRAME_TYPE,
                   &frame_type);


    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


    /* Execute process */
    err_code_exec = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_EXECUTE,
				   XA_CMD_TYPE_DO_EXECUTE,
				   NULL);

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
    *pOutBuf = pb_out_buf;
    *pOutDataLen = i_out_bytes;
/*
    err_code = (*p_xa_process_api)(xa_process_handle,
			   XA_API_CMD_GET_CURIDX_INPUT_BUF,
			   0,
			   &i_bytes_consumed);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);*/

    assert(i_bytes_consumed <= i_buff_size);
}

#else
#include "os_config.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_memory_standards.h"
#include "xa_msbc_dec_api.h"

#include "audio_com.h"
#include "audio_dec.h"
#include "codec.h"
//#include "app_main.h"
#include "fifo.h"
//#include "app_sdram.h"

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif	/* SHOW_SIZES */


VOID xa_msbc_dec_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_msbc_dec_testbench_error_info;
extern xa_error_info_struct xa_msbc_dec_error_info;


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

#define PARAMFILE_MSBC ".\\test\\build\\paramfilesimple_msbc_dec.txt"

/* This is used to verify the scratch memory behavior */
//#define TEST_SCRATCH

#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED	0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED	0xffff8001
#define XA_TESTBENCH_MFMAN_FATAL_CONFIGURATION      0xffff8002

/* Maximum supported loss pattern length in this testbench */
#define MAX_LOSS_PATTERN_LENGTH 2000

#define SBC_FRAME_LOST     0x6B20
#define SBC_FRAME_NORMAL   0x6B21

#define MIN(a,b) ((a>b) ? b : a)

//FILE *g_pf_inp, *g_pf_out;

//FILE *g_pf_loss_pattern;

void *g_pf_inp = NULL, *g_pf_out = NULL;

void *g_pf_loss_pattern = NULL;

#define assert DBG_Assert
#define malloc codec_malloc
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


XA_ERRORCODE
xa_msbc_dec_get_config_param (xa_codec_handle_t p_xa_process_api_obj,
			     pWORD32           pi_bitrate,
			     pWORD32           pi_samp_freq,
			     pWORD32           pi_num_chan,
			     pWORD32           pi_pcm_wd_sz)
{
  XA_ERRORCODE err_code = XA_NO_ERROR;
  /* the process API function */
  xa_codec_func_t *p_xa_process_api = xa_msbc_dec;
  xa_error_info_struct *p_proc_err_info = &xa_msbc_dec_error_info;

  /* Data rate */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_MSBC_DEC_CONFIG_PARAM_BITRATE, pi_bitrate);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  }
  /* Sampling frequency */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_MSBC_DEC_CONFIG_PARAM_SAMP_FREQ, pi_samp_freq);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  }
  /* Number of channels */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_MSBC_DEC_CONFIG_PARAM_NUM_CHANNELS, pi_num_chan);
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
  }
  /* PCM word size */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
				   XA_API_CMD_GET_CONFIG_PARAM,
				   XA_MSBC_DEC_CONFIG_PARAM_PCM_WDSZ, pi_pcm_wd_sz);
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

XA_ERRORCODE
xa_msbc_dec_main_process (void) __attribute__ ((section (".internal_ram_1_text")));

XA_ERRORCODE
xa_msbc_dec_main_process (void)
{
  LOOPIDX i;

#if DISPLAY_MESSAGE
  /* Library Info and Identification strings */
  WORD8 pb_process_name[30] = "";
  WORD8 pb_lib_version[30] = "";
#endif

  /* Error code */
  XA_ERRORCODE err_code = XA_NO_ERROR;
  XA_ERRORCODE err_code_exec = XA_NO_ERROR;

  /* API obj */
  xa_codec_handle_t xa_process_handle = NULL;

  WORD32 loss_pattern_length;
  WORD32 loss_pattern_idx;
  WORD32 loss_pattern_size;
  pWORD16 loss_pattern;
  UWORD32 plc_option;
  xa_msbc_frame_type_t frame_type;
  loss_pattern_length = 0;
  loss_pattern_idx = 0;
  loss_pattern_size = MAX_LOSS_PATTERN_LENGTH * sizeof(WORD16);
  loss_pattern = NULL;
  plc_option = 1;
  frame_type = XA_MSBC_GOOD_FRAME;

  short *hfp_outbuf;

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
  UWORD32 ui_exec_done;
  pWORD8 pb_inp_buf = NULL;
  pWORD8 pb_out_buf = NULL;

  UWORD32 ui_inp_size = 0;
  WORD32 i_bytes_consumed;
  WORD32 i_bytes_read;
  WORD32 i_buff_size;

  WORD32 i_out_bytes = 0;
  WORD32 i_total_bytes = 0;
  WORD32 i_bitrate, i_samp_freq, i_num_chan, i_pcm_wd_sz;

  /* The process API function */
  xa_codec_func_t *p_xa_process_api;
  pVOID pvoid;

  /* The get config from API */
  XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
				      pWORD32           pi_bitrate,
				      pWORD32           pi_samp_freq,
				      pWORD32           pi_num_chan,
				      pWORD32           pi_pcm_wd_sz);

  /* The error init function */
  VOID (*p_error_init)();

  //not use malloc
  enableCoProcessor (CO_PROCESSOR_MASK_BITS);  
  codec_malloc_init ();

  /* The process error info structure */
  xa_error_info_struct *p_proc_err_info;

  /* Stack process struct initing */
  p_xa_process_api = xa_msbc_dec;
  p_get_config_param = xa_msbc_dec_get_config_param;
  p_error_init = xa_msbc_dec_error_handler_init;
  p_proc_err_info = &xa_msbc_dec_error_info;
  /* Stack process struct initing end */

  /* ******************************************************************/
  /* Initialize the error handler                                     */
  /* ******************************************************************/
  (*p_error_init)();

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
  DBG_Printf( "\n%s version %s\n", pb_process_name, pb_lib_version);
  DBG_Printf( "Tensilica, Inc. http://www.tensilica.com\n\n");
#endif

#if 0
    /**************************************************************************
     * Verify the PLC parameters                                               *
     **************************************************************************/
    /* If there is only 2 parameters, no loss param file or -noplc flag were
     * indicated, PLC is enabled by default.
     */
    for (i=1; i<argc; i++)
    {
        if (strncmp(argv[i], "-noplc", 6) == 0)
        {
            plc_option = 0;
        }
    }
#else
	plc_option = 1;
#endif
#if 0
    /* Allocate memory to the buffer that holds the loss_pattern */
    loss_pattern  = (pWORD16) malloc(loss_pattern_size * sizeof(WORD16));

    if (loss_pattern == NULL)
    {
        _XA_HANDLE_ERROR(
            &xa_msbc_dec_testbench_error_info,
            "API loss pattern buffer alloc",
            XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* Read the Loss Pattern File */
    if (g_pf_loss_pattern != NULL)
    {
        /* Read the entire loss pattern file */
        loss_pattern_length = 0;

        while (fread(
                &loss_pattern[loss_pattern_length],
                sizeof(WORD16),
                1,
                g_pf_loss_pattern) == 1 )
        {
#if __XTENSA_EB__
            loss_pattern[loss_pattern_length] =
                AE_SHA32(loss_pattern[loss_pattern_length]);
#endif
            loss_pattern_length++;

            if ( loss_pattern_length >= MAX_LOSS_PATTERN_LENGTH )
            {
                _XA_HANDLE_ERROR(
                    &xa_msbc_dec_testbench_error_info,
                    "API loss pattern too long",
                    XA_TESTBENCH_MFMAN_FATAL_CONFIGURATION);
            }
        }
        fprintf(stderr, "Loss pattern length: %d\n", loss_pattern_length);
    }
    else
    {
        /* Assume no loss of packets */
        loss_pattern_length = 1;
        loss_pattern[0] = SBC_FRAME_NORMAL;
    }
#endif
  /* ******************************************************************/
  /* Initialize API structure and set config params to default        */
  /* ******************************************************************/

  /* Get the API size */
  err_code = (*p_xa_process_api)(NULL, XA_API_CMD_GET_API_SIZE, 0,
				 &pui_api_size);
#if SHOW_SIZES
        DBG_Printf("\nAPI structure size: %u bytes\n", pui_api_size);
#endif

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* Allocate memory for API */
  pvoid = malloc(pui_api_size);

  if (pvoid == NULL) {
    _XA_HANDLE_ERROR(&xa_msbc_dec_testbench_error_info, "API struct alloc",
		     XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
  }

  /* API object requires 4 bytes (WORD32) alignment;
   * malloc() provides at least 8-byte alignment.
   */
  assert((((unsigned int) pvoid) & 3) == 0);

  /* Set API object with the memory allocated */
  xa_process_handle = (void *) pvoid;

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
#if SHOW_SIZES
        DBG_Printf("\nMEMTABS size: %u bytes\n\n", ui_proc_mem_tabs_size);
#endif

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


  pvoid = malloc(ui_proc_mem_tabs_size);

  if(pvoid == NULL) {
    _XA_HANDLE_ERROR(&xa_msbc_dec_testbench_error_info, "Mem tables alloc",
		     XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
  }

  /* Memory table requires 4 bytes (WORD32) alignment; malloc()
   * provides at least 8-byte alignment.
   */
  assert((((unsigned int) pvoid) & 3) == 0);

  /* Set pointer for process memory tables	*/
  err_code = (*p_xa_process_api)(xa_process_handle,
				 XA_API_CMD_SET_MEMTABS_PTR, 0,
				 (void *) pvoid);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

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

    pvoid = malloc(ui_size);

    if(pvoid == NULL) {
      _XA_HANDLE_ERROR(&xa_msbc_dec_testbench_error_info, "Mem tables alloc",
		       XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

    /* The following assertion should never fail because malloc() on
     * Xtensa always returns memory aligned on at least an 8-byte
     * boundary.
     */
    assert((((unsigned int) pvoid) % ui_alignment) == 0);

    pv_alloc_ptr = (void *) pvoid;


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
	  hfp_outbuf = malloc(ui_size*3*2*2);
    }

#if SHOW_SIZES
    switch (ui_type) {
    case XA_MEMTYPE_INPUT:
      DBG_Printf( "Input buffer size: %u bytes\n", ui_size);
      break;
    case XA_MEMTYPE_OUTPUT:
      DBG_Printf( "Output buffer size: %u bytes\n", ui_size);
      break;
    case XA_MEMTYPE_SCRATCH:
      DBG_Printf( "Scratch buffer size: %u bytes\n", ui_size);
      break;
    case XA_MEMTYPE_PERSIST:
      DBG_Printf( "Persistent buffer size: %u bytes\n", ui_size);
      break;
    default:
      assert(0);
    }
#endif	/* SHOW_SIZES */
  }

  /* End first part */

  i_bytes_consumed = ui_inp_size;
	int size;
	int rlen;

  /* Second part        */
  /* Initialize process */
  /* Get config params  */

  /* ******************************************************************/
  /* Initialize process in a loop (to handle junk data at beginning)  */
  /* ******************************************************************/
  i_buff_size = ui_inp_size;

  do {
 read_data: 	
    for(i = 0; i < (WORD32)ui_inp_size - i_bytes_consumed; i++) {
      pb_inp_buf[i] = pb_inp_buf[i + i_bytes_consumed];
    }
#if 1
    i_bytes_read = xa_fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
			 sizeof(char),
			 i_bytes_consumed,
			 NULL);
#else


	size = app_sdram_stream_get_stream_dlen(s);

	while(size < i_bytes_consumed)
	{
		size = app_sdram_stream_get_stream_dlen(s);

		app_nav_sleep(20);
		if (wRequestDecodeFrameExit == TRUE)
		{
			ui_exec_done = TRUE;
			return XA_NO_ERROR;
		}
	}
	i_bytes_read	= app_sdram_stream_read (s, (U8 *)(pb_inp_buf + (ui_inp_size - i_bytes_consumed)), i_bytes_consumed);
#endif
    /* New buffer size */
    i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);
	U8 *p = pb_inp_buf;
	if (*p != 0xAD)
	{
		for (i = 1;i < i_buff_size;i++)
		{
			if (*++p == 0xAD)
			{
				break;
			}
		}
		i_bytes_consumed = i;
		goto read_data;
	}

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

    /* Send the PLC information to the API */
    err_code = (*p_xa_process_api)(xa_process_handle,
                   XA_API_CMD_SET_CONFIG_PARAM,
                   XA_MSBC_DEC_CONFIG_PARAM_PLC_OPTION,
                   &plc_option);

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

  DBG_Printf( "\nData Rate: %d bps\n", i_bitrate);
  DBG_Printf( "Sample Rate: %d Hz\n", i_samp_freq);
  DBG_Printf( "Number of Channels: %d\n", i_num_chan);
  DBG_Printf( "PCM Sample Size: %d bits\n", i_pcm_wd_sz);
  DBG_Printf( "Standard: mSBC\n");
  DBG_Printf( "PLC %s\n", (plc_option == 1) ? "enabled" : "disabled");

  /* End second part */

XA_ERRORCODE
 xa_msbc_enc_init (void);

	xa_msbc_enc_init();

  void app_dac_receive_pcm_enable(U8 con);
  app_dac_receive_pcm_enable(TRUE);

	U8 bad_frame_flag;
	bad_frame_flag = FALSE;
	U16 bad_consumed_byte = 0;
/* Third part for executing each process */

  do {
 eread_data: 	

    /* Execute process */

    for(i = 0; i < (WORD32)ui_inp_size - i_bytes_consumed; i++) {
      pb_inp_buf[i] = pb_inp_buf[i + i_bytes_consumed];
    }


#if 1
    i_bytes_read = xa_fread(pb_inp_buf + (ui_inp_size - i_bytes_consumed),
			 sizeof(char),
			 i_bytes_consumed,
			 NULL);
#else


	size = app_sdram_stream_get_stream_dlen(s);

	while(size < i_bytes_consumed)
	{
		size = app_sdram_stream_get_stream_dlen(s);

		app_nav_sleep(1);
		if (wRequestDecodeFrameExit == TRUE)
		{
			ui_exec_done = TRUE;
			return XA_NO_ERROR;
		}
	}
	i_bytes_read	= app_sdram_stream_read (s, (U8 *)(pb_inp_buf + (ui_inp_size - i_bytes_consumed)), i_bytes_consumed);
#endif

    /* New buffer size */
    i_buff_size = i_buff_size - (i_bytes_consumed - i_bytes_read);

	U8 *pin = pb_inp_buf;
	if ((*pin != 0xAD)&&i_buff_size)
	{
		for (i = 1;i < i_buff_size;i++)
		{
			if (*++pin == 0xAD)
			{
				break;
			}
		}			
		i_bytes_consumed = i;
		bad_consumed_byte += i_bytes_consumed;
		goto eread_data;
	}
	if (bad_consumed_byte > 30)
		frame_type = XA_MSBC_BAD_FRAME;
	else
		frame_type = XA_MSBC_GOOD_FRAME;		
	bad_consumed_byte = 0;
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

#if 0
    /* Read the loss pattern buffer and save to frame_type */
    if (g_pf_loss_pattern != NULL)
    {
        if (loss_pattern[loss_pattern_idx] == SBC_FRAME_NORMAL)
        {
            frame_type = XA_MSBC_GOOD_FRAME;
        }
        else if (loss_pattern[loss_pattern_idx] == SBC_FRAME_LOST)
        {
            frame_type = XA_MSBC_BAD_FRAME;
        }
        else
        {
            _XA_HANDLE_ERROR(
                    &xa_msbc_dec_testbench_error_info,
                    "Error code type is not allowed",
                    XA_TESTBENCH_MFMAN_FATAL_CONFIGURATION);
        }
    }
    else
    {
        frame_type = XA_MSBC_GOOD_FRAME;
    }

    loss_pattern_idx++;

    if ( loss_pattern_idx == loss_pattern_length )
    {
        loss_pattern_idx=0;
    }
	int p_inp = pb_inp_buf;
	if ((bad_frame_flag)||(p_inp == 0xffffffff))
		frame_type = XA_MSBC_BAD_FRAME;
	else
	frame_type = XA_MSBC_GOOD_FRAME;
#endif
    /* Send the frame type information to the API */
    err_code = (*p_xa_process_api)(xa_process_handle,
                   XA_API_CMD_SET_CONFIG_PARAM,
                   XA_MSBC_DEC_CONFIG_PARAM_FRAME_TYPE,
                   &frame_type);


    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);


    /* Execute process */
    err_code_exec = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_EXECUTE,
				   XA_CMD_TYPE_DO_EXECUTE,
				   NULL);

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
    
    unsigned int ui_nsamples = i_out_bytes / output_wordsize(i_pcm_wd_sz);
    
    i_total_bytes += (ui_nsamples * i_pcm_wd_sz) / 8;
    
//    xa_fwrite((pVOID)pb_out_buf, (i_pcm_wd_sz/8), ui_nsamples, g_pf_out);
    int i;
	if (i_out_bytes)
	{
	short *p = pb_out_buf;
#if 0
	//16K to 48K,mono to stereo
	for (i=0;i<ui_nsamples;i++)
	{
		U16_endian_buf((U8*)p);
		hfp_outbuf[6*i+0] = *p;
		hfp_outbuf[6*i+1] = *p;
		hfp_outbuf[6*i+2] = *p;
		hfp_outbuf[6*i+3] = *p;
		hfp_outbuf[6*i+4] = *p;
		hfp_outbuf[6*i+5] = *p;
		p++;
	}
	dec_direct_out (hfp_outbuf,i_out_bytes*6); 
#else
	//16K to 48K,mono to stereo
	for (i=0;i<ui_nsamples;i++)
	{
		hfp_outbuf[6*i+0] = *p;
		hfp_outbuf[6*i+1] = *p;
		hfp_outbuf[6*i+2] = *p;
		hfp_outbuf[6*i+3] = *p;
		hfp_outbuf[6*i+4] = *p;
		hfp_outbuf[6*i+5] = *p;
		p++;
	}

  void optek_dapp_16b_hfp_out_process (short *x, int chans, int N);
  optek_dapp_16b_hfp_out_process(hfp_outbuf,2,i_out_bytes*(6/2));  // 24bit out

void dec_direct_out_24bit( int *pBuf, U16 len );
  dec_direct_out_24bit (hfp_outbuf,i_out_bytes*12); 
	//dec_direct_out (hfp_outbuf,i_out_bytes*6); 
	  	void mic_msbcEncode( U16 len);
	mic_msbcEncode(i_out_bytes*6);	
	}
#endif
#if 0
    /* How much buffer is used in input buffers */
    //if (frame_type == XA_MSBC_BAD_FRAME)
	if(p_inp == 0xffffffff)
    {
        /* If the input frame is bad, the decoder won't consume any bytes.
         * But we need to discard one frame worth of bytes from the input test file
         * to simulate a lost frame.  In mSBC each frame is 57 bytes, so skip that.
         * Exception: when this is the last input frame, we may have less than 57
         */
        i_bytes_consumed = MIN(57, i_buff_size);
    }
    else
    {
        err_code = (*p_xa_process_api)(xa_process_handle,
				   XA_API_CMD_GET_CURIDX_INPUT_BUF,
				   0,
				   &i_bytes_consumed);
    }

#if 1
	if ((i_bytes_consumed < 57)&&(i_buff_size >= 57)&&!bad_frame_flag){
		pWORD8 p;
		i_bytes_consumed = 57;
		bad_frame_flag = TRUE;
	}
	else{
		bad_frame_flag = FALSE;
	}
#endif
#else
	bad_frame_flag = FALSE;
    err_code = (*p_xa_process_api)(xa_process_handle,
			   XA_API_CMD_GET_CURIDX_INPUT_BUF,
			   0,
			   &i_bytes_consumed);
#endif
    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    assert(i_bytes_consumed <= i_buff_size);


	if (wRequestDecodeFrameExit == TRUE)
	{
		ui_exec_done = TRUE;
	}
    /* Do till the process execution is done */
  } while(!ui_exec_done);
  app_dac_receive_pcm_enable(FALSE);
  return XA_NO_ERROR;
} /* End xa_main_process() */
#endif

#endif //mSBC_DEC

