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
#include "xa_msbc_enc_api.h"
#if __XTENSA_EB__
#include <xtensa/tie/xt_hifi2.h>
#endif

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif  /* SHOW_SIZES */


#ifdef mSBC_ENC

VOID xa_msbc_enc_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_msbc_enc_testbench_error_info;
extern xa_error_info_struct xa_msbc_enc_error_info;


#if !defined(WAV_HEADER)
#define WAV_HEADER 0
#endif  /* WAV_HEADER */

#if !defined(DISPLAY_MESSAGE)
#define DISPLAY_MESSAGE	0
#endif  /* DISPLAY_MESSAGE */

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
#define PARAMFILE ".\\test\\build\\paramfilesimple_msbc_enc.txt"

/* This is used to verify the scratch memory behavior */
//#define TEST_SCRATCH

#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED       0xffff8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED       0xffff8001

//static pVOID g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];
//static WORD  g_w_malloc_count;
//FILE *g_pf_inp, *g_pf_out;


#define assert DBG_Assert

#define malloc codec_malloc


#if WAV_HEADER

#if __XTENSA_EB__
#define word16_order(p) (AE_SHA32(p))
#define word32_order(p) (word16_swap(p))

static WORD32 word16_swap(WORD32 data)
{
  data = AE_SHA32(data);
  return (data << 16)+ ((data >> 16) & 0xFFFF);
}

#else
#define word32_order(p) (p)
#define word16_order(p) ((p))
#endif

/* Read data in little-endian (Intel) format regardless of the
   target processor byte order. */
WORD32 xa_fread (void *buffer, int size, int nwords, FILE *fp)
{
  int i, j, n=0, c;
  char *pb_buf = (char *) buffer;
  
  for (i = 0; i < nwords; i++) {
#if __XTENSA_EL__
    for (j = 0; j < size; j++) {
      c = getc(fp);
      if (c == EOF) return n;
      pb_buf[i * size + j] = c;
    }
#else
    for (j = size - 1; j >= 0; j--) {
      c = getc(fp);
      if (c == EOF) return n;
      pb_buf[i * size + j] = c;
    }
#endif
    n++;
  }
  return n;
}

int
read_wav_header (FILE *FilePtr, /* file to write */
                 pWORD32 DataSize, /* total bytes in the wav file */
                 pWORD32 SamplingRate, /* sample rate */
                 pWORD32 NumOfChannels, /* output channels */
                 pWORD32 BitsPerSample /* bits per sample */)
{

  char  WaveTag[4];
  WORD16 BytesPerSample;
  WORD32 longtmp;
  WORD16 shorttmp;
  WORD32 sRate;
  WORD16 nChannels, bitpSample;
  
  fseek(FilePtr, 0, SEEK_SET);

  /* RIFF chunk - 12 bytes */
  fread(&WaveTag, 4, 1, FilePtr);
  if(strncmp("RIFF", WaveTag, 4) != 0)
    {
      return 1;
    }
  fread(&longtmp, 4, 1, FilePtr);
  fread(&WaveTag, 4, 1, FilePtr);
  if(strncmp("WAVE", WaveTag, 4) != 0)
    {
      return 1;
    }

  /* FORMAT chunk - 24 bytes */
  fread(&WaveTag, 4, 1, FilePtr);
  if(strncmp("fmt ", WaveTag, 4) != 0)
    {
      return 1;
    }

  fread(&longtmp, 4, 1, FilePtr);
  fread(&shorttmp, 2, 1, FilePtr);
  fread(&nChannels,  2, 1, FilePtr);

  *NumOfChannels = (WORD32) word16_order(nChannels);
  if(*NumOfChannels > 1){
      return 1;
  }

  fread(&sRate,  4, 1, FilePtr);
  *SamplingRate = word32_order(sRate);
  if(*SamplingRate != 16000){
      return 1;
  }

  fread(&longtmp, 4, 1, FilePtr);
  fread(&BytesPerSample,  2, 1, FilePtr);
  BytesPerSample = word16_order(BytesPerSample);
  fread(&bitpSample,  2, 1, FilePtr);
  *BitsPerSample =(WORD32) word16_order(bitpSample);
    

  /* DATA chunk - Data length + 8 bytes */
  fread(&WaveTag, 4, 1, FilePtr);
  if(strncmp("data", WaveTag, 4) != 0)
    {
      return 1;
    }
  fread(DataSize,  4, 1, FilePtr);
  *DataSize = word32_order(*DataSize);

  *DataSize = *DataSize / BytesPerSample;
  return 0;
}

#endif /* WAV_HEADER */






XA_ERRORCODE
xa_msbc_enc_get_config_param (xa_codec_handle_t p_xa_process_api_obj,
                             pWORD32           pi_bitrate)
{
  XA_ERRORCODE err_code = XA_NO_ERROR;
  /* the process API function */
  xa_codec_func_t *p_xa_process_api = xa_msbc_enc;
  xa_error_info_struct *p_proc_err_info = &xa_msbc_enc_error_info;

  /* Data rate */
  {
    err_code = (*p_xa_process_api)(p_xa_process_api_obj,
                                   XA_API_CMD_GET_CONFIG_PARAM,
                                   XA_MSBC_ENC_CONFIG_PARAM_BITRATE, pi_bitrate);
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

XA_ERRORCODE
 xa_msbc_enc_init (void)
{
  LOOPIDX i;
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
  pb_inp_buf = NULL;
  pb_out_buf = NULL;

  WORD32 i_bytes_consumed, i_bytes_read;
  WORD32 i_buff_size;

  WORD32 i_total_bytes = 0;
  WORD32 i_data_size, i_bitrate,i_pcm_wd_sz;

  /* The process API function */
  xa_codec_func_t *p_xa_process_api = NULL;
  pVOID pvoid;

  /* The get config from API */
  XA_ERRORCODE (*p_get_config_param) (xa_codec_handle_t p_xa_process_api_obj,
                                      pWORD32           pi_bitrate);

  DBG_Printf("\n\n\n\n\n\n\n *****************msbc enc init start****************** \r\n");
  /* The error init function */
  VOID (*p_error_init)();

  /* The process error info structure */
  xa_error_info_struct *p_proc_err_info;

  /* Stack process struct initing */
  p_xa_process_api = xa_msbc_enc;
  p_get_config_param = xa_msbc_enc_get_config_param;
  p_error_init = xa_msbc_enc_error_handler_init;
  p_proc_err_info = &xa_msbc_enc_error_info;
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
    _XA_HANDLE_ERROR(&xa_msbc_enc_testbench_error_info, "API struct alloc",
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
    _XA_HANDLE_ERROR(&xa_msbc_enc_testbench_error_info, "Mem tables alloc",
                     XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
  }

  /* Memory table requires 4 bytes (WORD32) alignment; malloc()
   * provides at least 8-byte alignment.
   */
  assert((((unsigned int) pvoid) & 3) == 0);

  /* Set pointer for process memory tables      */
  err_code = (*p_xa_process_api)(xa_process_handle,
                                 XA_API_CMD_SET_MEMTABS_PTR, 0,
                                 (void *) pvoid);

  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  /* initialize the API, post config, fill memory tables        */
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
      _XA_HANDLE_ERROR(&xa_msbc_enc_testbench_error_info, "Mem tables alloc",
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
#endif  /* SHOW_SIZES */
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

  err_code = (*p_get_config_param)(xa_process_handle, &i_bitrate);
  _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

  DBG_Printf( "\nData Rate: %d bps\n", i_bitrate);
/*  DBG_Printf( "Sample Rate: %d Hz\n", i_samp_freq);
  DBG_Printf( "Number of Channels: %d\n", i_num_chan);
  DBG_Printf( "PCM Sample Size: %d bits\n", i_pcm_wd_sz);
  */
    DBG_Printf("\n\n****************msbc enc init end********************\r\n\n\n\n\n\n\n");

  /* End second part */
} 


XA_ERRORCODE
 xa_msbc_enc_main_process(U16 *pInBuf, U32 samples, U8 **pOutBuf, U32 *pOutDataLen) __attribute__ ((section (".internal_ram_1_text")));
/* End xa_main_process() */
XA_ERRORCODE
 xa_msbc_enc_main_process(U16 *pInBuf, U32 samples, U8 **pOutBuf, U32 *pOutDataLen)
{
	int err_code = XA_NO_ERROR;
	int err_code_exec = XA_NO_ERROR;
	xa_codec_func_t *p_xa_process_api;

	/* The process error info structure */
	xa_error_info_struct *p_proc_err_info;

	/* Stack process struct initing */
	p_xa_process_api = xa_msbc_enc;
	p_proc_err_info = &xa_msbc_enc_error_info;
  
	WORD32 i_buff_size;

	/* Process initing done query variable */
	UWORD32 ui_exec_done;


	WORD32 i_out_bytes;
	
    /* Execute process */
	//DBG_Assert(samples*sizeof(WORD16) == ui_inp_size);

	CFasm_memcpy (pb_inp_buf, pInBuf, ui_inp_size);
	i_buff_size = 57;//i_bytes_read;

    /* Set number of bytes to be processed */
/*    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_SET_INPUT_BYTES,
                                   0,
                                   &i_buff_size);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
*/

    /* Execute process */
    err_code_exec = (*p_xa_process_api)(xa_process_handle,
                                        XA_API_CMD_EXECUTE,
                                        XA_CMD_TYPE_DO_EXECUTE,
                                        NULL);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code_exec);


    /* Checking for end of processing */
/*    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_EXECUTE,
                                   XA_CMD_TYPE_DONE_QUERY,
                                   &ui_exec_done);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);
*/
    /* Get the output bytes */
    err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_GET_OUTPUT_BYTES,
                                   0,
                                   &i_out_bytes);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

	*pOutDataLen = i_out_bytes;
	*pOutBuf = pb_out_buf;
	
    /* How much buffer is used in input buffers */
 /*   err_code = (*p_xa_process_api)(xa_process_handle,
                                   XA_API_CMD_GET_CURIDX_INPUT_BUF,
                                   0,
                                   &i_bytes_consumed);

    _XA_HANDLE_ERROR(p_proc_err_info, "", err_code);

    DBG_Assert(i_bytes_consumed == i_buff_size);*/
    
 //   fwrite((pVOID)pb_out_buf, sizeof(char), i_out_bytes, g_pf_out);
    
 //   fflush(g_pf_out);

}

const U8 zero_data[60] = {
	0x01, 0x08, 0xAD, 0x00, 0x00, 0xC5, 0x00, 0x00, 0x00, 0x00, 0x77, 0x6D, 0xB6, 0xDD, 0xDB, 0x6D, 
	0xB7, 0x76, 0xDB, 0x6D, 0xDD, 0xB6, 0xDB, 0x77, 0x6D, 0xB6, 0xDD, 0xDB, 0x6D, 0xB7, 0x76, 0xDB, 
	0x6D, 0xDD, 0xB6, 0xDB, 0x77, 0x6D, 0xB6, 0xDD, 0xDB, 0x6D, 0xB7, 0x76, 0xDB, 0x6D, 0xDD, 0xB6, 
	0xDB, 0x77, 0x6D, 0xB6, 0xDD, 0xDB, 0x6D, 0xB7, 0x76, 0xDB, 0x6C, 0x00, 
};

#if 0

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
  FILE *param_file_id;
  int err_code = XA_NO_ERROR;

  char curr_cmd[XA_MAX_CMD_LINE_LENGTH] = "";
  WORD32 fargc, curpos;
  WORD32 processcmd = 0;

  char fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];

  char *pargv[XA_MAX_ARGS];

  char pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
  char pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

  xa_testbench_error_handler_init();

  if(argc == 1)
    {
      param_file_id = fopen(PARAMFILE, "r");
      if (param_file_id == NULL)
        {
          err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
          xa_error_handler(&xa_testbench_error_info,
                           "Parameter File", err_code);
          exit(1);
        }
      
      /* Process one line at a time */
      while(fgets(curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id))
        {
          curpos = 0;
          fargc = 0;
          /* if it is not a param_file command and if */
          /* CLP processing is not enabled */
          if(curr_cmd[0] != '@' && !processcmd)
            {   /* skip it */
              continue;
            }

          while(sscanf(curr_cmd + curpos, "%s", fargv[fargc]) != EOF)
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

          if(fargc < 1) /* for blank lines etc. */
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

                      g_pf_inp = NULL;
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

                      g_pf_out = NULL;
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
                xa_msbc_enc_main_process(fargc, pargv);

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

      for(i = 1; i < argc; i++)
        {
          fprintf(stdout, "%s ", argv[i]);

	  /* Show help message */
	  if(!strcmp(argv[i], "-h"))
	    {
	      usage();
	    }
        
          if(!strncmp(argv[i], "-ifile:", 7))
            {
              char *pb_arg_val = argv[i] + 7;
              char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

              file_flag++;
              err_code = XA_NO_ERROR;
              strcat(pb_input_file_name, pb_input_file_path);
              strcat(pb_input_file_name, pb_arg_val);

              g_pf_inp = NULL;
              g_pf_inp = fopen(pb_input_file_name, "rb");
              if(g_pf_inp == NULL)
                {
                  fprintf(stdout, "Input file: %s\n", pb_input_file_name);
                  err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                  xa_error_handler(&xa_testbench_error_info,
                                   "Input File", err_code);
                }
            }

          if(!strncmp(argv[i], "-ofile:", 7))
            {
              char *pb_arg_val = argv[i] + 7;
              char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";

              file_flag++;
              strcat(pb_output_file_name, pb_output_file_path);
              strcat(pb_output_file_name, pb_arg_val);

              g_pf_out = NULL;
              g_pf_out = fopen(pb_output_file_name, "wb");
              if(g_pf_out == NULL)
                {
                  fprintf(stdout, "Output file: %s\n", pb_output_file_name);
                  err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                  xa_error_handler(&xa_testbench_error_info,
                                   "Output File", err_code);
                }
            }

        }
      g_w_malloc_count = 0;

      fprintf(stdout, "\n");
      if(file_flag != 2)
        {
          err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
          xa_error_handler(&xa_testbench_error_info,
                           "Input or Output File", err_code);
        }

      if(err_code == XA_NO_ERROR)
        xa_msbc_enc_main_process(argc - 1, &argv[1]);

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
} /* end xa_param_file_process */
#endif

#endif //mSBC_ENC

