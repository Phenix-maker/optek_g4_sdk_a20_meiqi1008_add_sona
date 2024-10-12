/*
 * sona_dsp.c
 *
 *  Created on: 2024/9/19
 *      Author: zero
 */
#include "os_config.h"
#include "regmap.h"
#include "mem_reloc.h"

//#include "codec_malloc.h"
#include "sona_dsp_codec_malloc.h"

#include "audio_device.h"


#ifdef DSP_SONA_MODULE_ENABLE
#include "sona_aef.h"
#include "sona_dsp.h"
#include "sona_dsp_pc_tools_by_uart.h"

#define DSP_SONA_MODULE_FUNCTION_ENABLE

//#define DBG_Printf(...)

//#define __INTERNAL_RAM_SONA_TEXT __attribute__ ((section (".internal_ram_1_text")))

#define SONA_DSP_DECODE_BLOCK_SIZE  AEF_LEN*4*AEF_IN_CH
// int sona_temp_src_buf[SONA_DSP_DECODE_BLOCK_SIZE];
// int sona_dtx_temp_dest_buf[SONA_DSP_DECODE_BLOCK_SIZE];
int *p_sona_temp_src;
int *p_sona_temp_dest;
int *p_sona_temp_inbuffer;
int *p_sona_temp_outbuffer;
int *p_sona_temp_buf;

int sona_dsp_malloc_print_size;
int sona_dsp_L2_malloc_print_size;

/*
	int srate;	 //48k/44.1k	
	int insrc_type; // 0-BT, 1-LINE
	int high_pass_mode; //0-high_pass disable, 1-high pass enable;
	int phase;			//0-normal,1-invert;
	int stereoOrMono;  //0-stereo, 1-momo;
	float woofer_gain_dB; 
	float woofer_volume_dB; //-6 ~ 6dB
*/
tSonaConfig sona_config_info;
int DspSonaInit_en;
void dsp_sona_module_init (void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef DSP_SONA_MODULE_FUNCTION_ENABLE
    int ret = 0xFF;

    char *flowStr = NULL;
    char *paramStr = NULL;

    DspSonaInit_en = FALSE;

    #if 1
    audio_vari.high_pass_mode = 0;
    audio_vari.phase = 0;
    audio_vari.stereoOrMono = 0;
    audio_vari.woofer_gain_dB = 0;
    audio_vari.woofer_volume_dB = 0;
    #endif

    sona_config_info.srate = audio_in_stream.sample_rate;
    sona_config_info.insrc_type = global_vari.audio_source;
    sona_config_info.high_pass_mode = audio_vari.high_pass_mode;
    sona_config_info.phase = audio_vari.phase;
    sona_config_info.stereoOrMono = audio_vari.stereoOrMono;
    sona_config_info.woofer_gain_dB = audio_vari.woofer_gain_dB;
    sona_config_info.woofer_volume_dB = audio_vari.woofer_volume_dB;    

    sona_dsp_malloc_print_size = 0;
    sona_dsp_L2_malloc_print_size = 0;  


    #ifdef DSP_SONA_MODULE_INIT
    //sona_dsp_codec_malloc_init(sona_dsp_codec_fast_mem, SONA_DSP_CODEC_FAST_MEM_SIZE, sona_dsp_codec_slow_mem, SONA_DSP_CODEC_SLOW_MEM_SIZE);

    ret = sona_aef_init((tSonaConfig *)&sona_config_info, NULL, NULL);

    if (ret == 0)
    {
        DspSonaInit_en = TRUE;
        DBG_Printf("success!!!\n\r");
    }
    else
    {
        DBG_Printf("error!!!\n\r");
    }
    #endif


    #ifdef SONA_DSP_PC_TOOLS_BY_UART_FIFO
    sona_fifo_init();
    #endif
    
    tTuningConfig sona_PCTuningConfig;
    sona_PCTuningConfig.type = 0;
    sona_PCTuningConfig.comm_write = sona_uart_command_write;
    sona_aef_tuning_open(&sona_PCTuningConfig);
    set_sona_dsp_pc_tools_by_uart_flag(FALSE);

    #ifdef SONA_DSP_PC_TOOLS_DTS_VIRTUAL_X_SET
    sona_aef_tuning_set_dtxFile_CB(sona_tools_for_writeDtxFile);
    #endif

    #endif
}

void sona_dsp_malloc_buf_init (void)
{
    #ifdef DSP_SONA_MODULE_CODE
    int size = (AEF_LEN*4*4);
    p_sona_temp_src = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

    size = (AEF_LEN*4*4);
    p_sona_temp_dest = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

    size = (AEF_LEN*4*4);
    p_sona_temp_inbuffer = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
    
    size = (AEF_LEN*4*4);
    p_sona_temp_outbuffer = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);    

    size = (AEF_LEN*4*4);
    p_sona_temp_buf = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);
    #endif
}
void sona_dsp_buf_free(void)
{
    #ifdef DSP_SONA_MODULE_CODE
    codec_free(p_sona_temp_src);
    codec_free(p_sona_temp_dest);
    codec_free(p_sona_temp_inbuffer);
    codec_free(p_sona_temp_outbuffer);
    codec_free(p_sona_temp_buf);
    #endif
}
int fix_data_to_xch(int *pDest, int *pSrc, int x_channel, int len)
{
    //DBG_Printf("%s\n\r",__func__);
    #ifdef DSP_SONA_MODULE_FUNCTION_ENABLE
    int i,j;

    for (i = 0; i < (len/4/x_channel); i++)
    {
        for (j = 0; j < x_channel; j++)
        {
            pDest[((len/4/x_channel)*j)+i]  = pSrc[(x_channel*i)+j];
        }
    }
    
    #endif
    return 1;
}

int fix_data_from_xch(int *pDest, int *pSrc, int x_channel, int len)
{
    //DBG_Printf("%s\n\r",__func__);
    #ifdef DSP_SONA_MODULE_FUNCTION_ENABLE
    int i,j;

    for (i = 0; i < x_channel; i++)
    {
        for (j = 0; j < (len/4/x_channel); j++)
        {            
            pDest[(x_channel*j)+i] = pSrc[((len/4/x_channel)*i)+j];
        }
    } 
    #endif       

    return 1;
}

//for expanding from 24bit 2-channel to 4-channel
void audio_data_process_24bit_2ch_in_to_4ch_out (int *pDestBuf, int *pSrcBuf, int byte_len)
{
	int *pDataBuf;
	int i;

	pDataBuf = pDestBuf;

	for (i=0; i<(byte_len/4/2); i++)
	{
		pDataBuf[i*4+0] = pSrcBuf[i*2+0];
		pDataBuf[i*4+1] = pSrcBuf[i*2+1];
		pDataBuf[i*4+2] = pSrcBuf[i*2+0];
		pDataBuf[i*4+3] = pSrcBuf[i*2+1];
	}
}
void audio_data_process_24bit_4ch_in_to_4ch_out(int *pDestBuf,int* pSrcBuf,int byte_len)
{
    int *pDataBuf;
	int i,tmp[2];

	pDataBuf = pDestBuf;
	for (i=0; i<(byte_len/4); i++)
    {
        tmp[0] = pSrcBuf[i*4+0];
        tmp[1] = pSrcBuf[i*4+1];

		pDataBuf[i*4+0] = pSrcBuf[i*4+2]; // determined by woofer, it is like l/r/woofer/0
		pDataBuf[i*4+1] = pSrcBuf[i*4+2];
		pDataBuf[i*4+2] = tmp[0];
		pDataBuf[i*4+3] = tmp[1];

    }

}
void dsp_sona_module_24bit_process (int *pSrc, int in_channel, int in_byte_len, int *pDest, int out_channel, int *out_byte_len)
{
    //DBG_Printf("%s:%d,%d\n\r",__func__, in_channel, out_channel);
    #ifdef DSP_SONA_MODULE_FUNCTION_ENABLE 

    #ifdef DSP_SONA_MODULE_CODE
    memset(p_sona_temp_src, 0x00, in_byte_len);
    memset(p_sona_temp_dest, 0x00, in_byte_len);
    
    switch (in_channel)
    {
    case 2:
        switch (AEF_IN_CH)
        {
        case 4:
            audio_data_process_24bit_2ch_in_to_4ch_out(p_sona_temp_buf, pSrc, in_byte_len);
            in_byte_len *= 2;
            break;
        
        case 2:
        default:
            memcpy(p_sona_temp_buf, pSrc, in_byte_len);
            break;
        }
        break;
    
    case 4:
    default:
        memcpy(p_sona_temp_buf, pSrc, in_byte_len);
        break;
    }

    #if 0
    memcpy(pDest, p_sona_temp_buf, in_byte_len);
    *out_byte_len = in_byte_len;
    #else
    //[(l/r/0/0/0/0),(l/r/0/0/0/0),...](4ch) ==> [(l/l/l...),(r/r...),...](4ch)
    fix_data_to_xch(p_sona_temp_src, p_sona_temp_buf, AEF_IN_CH, in_byte_len);

    #if 0
    memcpy(p_sona_temp_dest, p_sona_temp_src, in_byte_len);
    #else
    sona_aef_proc(p_sona_temp_src, p_sona_temp_dest);
    #endif

    *out_byte_len = in_byte_len;

    //[(l/l/l...),(r/r...),...](4ch) ==> [(l/r/0/0/0/0),(l/r/0/0/0/0),...](4ch)
    fix_data_from_xch(pDest, p_sona_temp_dest, out_channel, *out_byte_len);

    #endif
    #endif
    #endif
}
void Updata_sona_info(void)
{
    #if 0
    audio_vari.high_pass_mode = 0;
    audio_vari.phase = 0;
    audio_vari.stereoOrMono = 0;
    audio_vari.woofer_gain_dB = 0;
    audio_vari.woofer_volume_dB = 6;
    #endif

    sona_config_info.srate = audio_in_stream.sample_rate;
    sona_config_info.insrc_type = global_vari.audio_source;
    sona_config_info.high_pass_mode = audio_vari.high_pass_mode;
    sona_config_info.phase = audio_vari.phase;
    sona_config_info.stereoOrMono = audio_vari.stereoOrMono;
    sona_config_info.woofer_gain_dB = audio_vari.woofer_gain_dB;
    sona_config_info.woofer_volume_dB = audio_vari.woofer_volume_dB;    
    
    sona_aef_update((tSonaConfig *)&sona_config_info);
}
#endif
