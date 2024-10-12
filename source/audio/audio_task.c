#include "os_config.h"

#include "mem_reloc.h"

#include "hal_gpio.h"

#include "sdk_com.h"
#include "audio_output.h"
#include "audio_aux_input.h"
#include "audio_codec.h"
#include "audio_task.h"

#include "interrupt.h"
#include "fifo.h"

#ifdef BT_TX_ROLE_ENABLE
#include "app_bt_tx.h"
#endif

#ifdef AUDIO_CLOCK_SYNC_ENABLE
#include "audio_clock_sync.h"
#endif

#include "bt_common.h"
#include "xa_sbc_dec_dec.h"
// #define PEAKLEVEL_ENABLE

#include "codec_malloc.h"
#ifdef PEAKLEVEL_ENABLE
#include "peak_level_meter.h"
static peak_level_det peak_det_handle;
static U32 max_peak;
#endif

#ifdef DSP_EQ_SETTING_ENABLE
#ifdef DSP_EQ_SETTING_24BIT_DECODE
#include "se_link_24bit.h"
#else
#include "se_link_16bit.h"
#endif
#endif

#include "tools.h"
#include "app_media.h"

#ifdef DSP_SONA_MODULE_ENABLE
#include "sona_aef.h"
#include "sona_dsp.h"
#endif
#ifndef DEBUG_MODE
#define DBG_Printf
#endif


/*
 * CONSTANT DEFINTIONS
 ****************************************************************************************
 */
#define QUEUE_LENGTH              16


#ifdef ATMOS_CODEC_ENABLE
#define TASK_SYSTEM_STACK_SIZE      (1024*16)
#else
#define TASK_SYSTEM_STACK_SIZE      (1024*4)
#endif

#define STREAM_FIFO_SIZE            (100000)

AUDIO_TASK_ENV_STRU audio_task_env __attribute__(( aligned (16) ));


/*******************************************************************************************************************
 * 
 *  get N,       process N+1,get N+1,     process N+2,get N+2,     process N+3,...
 * |__________tx 1__________|__________tx 2__________|__________tx 3__________|...
 * 
 * process:decode->mix1->dsp->mix2
 * 
 *******************************************************************************************************************/




static void audio_task (void *param)
{
    int *pSrc;
    int out_size;
    AUDIO_TASK_MSG_STRU qMsg;
    int * tmp;
    // tmp = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, 128*2*4, CODEC_MEM_ALIGN_32);
    xt_ints_on(LEVEL3_INT9_MASK);
    audio_task_env.queue = xQueueCreate( QUEUE_LENGTH, sizeof(AUDIO_TASK_MSG_STRU) );

    audio_output_init();
    memset(audio_task_env.ouput_buffer, 0, sizeof(audio_task_env.ouput_buffer));
    audio_task_env.dec_pause_flag = FALSE;
    
    audio_output_start(audio_task_env.ouput_buffer[0], sizeof(audio_task_env.ouput_buffer[0])/4);
    EVENT_AUDIO_DECODE_CLR(global_vari.eventGroup);

    audio_task_env.dec_frame_in_samples = 120;//240;
    audio_task_env.index = 1;
    audio_output_put_next_frame_dataPtr(audio_task_env.ouput_buffer[audio_task_env.index], sizeof(audio_task_env.ouput_buffer[0])/4);

    while(1)
    {
        #ifdef OPL_MASTER_ENABLE
        app_change_mode_check();
        #endif
        EVENT_AUDIO_DECODE_GET(global_vari.eventGroup, portMAX_DELAY);
        audio_task_env.index = !audio_task_env.index;

        if (QUEUE_AUDIO_DECODE_GET(audio_task_env.queue, (void *)&qMsg, pdMS_TO_TICKS(0)) == pdPASS )
		{
			switch(qMsg.msg_id)
			{
				case DECODE_SET:
					if (!audio_task_env.decoding_flag)
					{
						audio_task_env.decodeHandle = qMsg.decodeHandle;

						DBG_Printf("codec sel:%s\r\n", audio_task_env.decodeHandle->dec_name);
					}
                    else
                    {
                        printf_info("dec set msg fail!\r\n");
                        decode_set_msgSend(qMsg.decodeHandle);
                    }                    
					break;

				case DECODE_START:
					if (!audio_task_env.decoding_flag)
					{
                        audio_task_env.decoding_exit_req = FALSE;
						audio_task_env.decoding_flag = TRUE;

						if (qMsg.param == TRUE)
						{
							audio_task_env.decodeHandle->init();
                            #ifdef OPTEK_LINK_ENABLE
                            optek_link_codec_init(audio_in_stream.sample_rate, audio_task_env.decodeHandle->decode_id == DECODE_SBC);
                            #endif
                            tmp = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, 128*2*4, CODEC_MEM_ALIGN_32);
                            memset(tmp,0,128*2*4);

                            #ifdef DSP_EQ_SETTING_ENABLE
                            int in_channel = audio_in_stream.channel;
                            int out_channel = 2;//AUDIO_OUT_STREAM_CHANNEL;
                            int sample_rate = audio_in_stream.sample_rate;
                            eq_init(sample_rate, in_channel, out_channel);
                            #endif

                            #ifdef DSP_SONA_MODULE_INIT
                            sona_dsp_malloc_buf_init();
                            #endif
                        }
                        #if defined BT_TX_ROLE_ENABLE && defined A2DP_SOURCE

                        Optek_psm_tx_fifo_init();

                        #ifdef SBC_ENCODE_ENABLE

                        #ifdef A2DP_CODEC_SR_48000
                        //DBG_Assert (app_main_data.playing_stream_sample_rate == 48000);
                        xa_sbc_enc_init(48000);
                        #else
                        //DBG_Assert (app_main_data.playing_stream_sample_rate == 44100);
                        xa_sbc_enc_init(44100);
                        #endif
                        #endif //SBC_ENCODE_ENABLE

                        #endif //BT_TX_ROLE_ENABLE	

						DBG_Printf("codec start:%s\r\n", audio_task_env.decodeHandle->dec_name);
					}
                    else
                    {
                        DBG_Assert(FALSE);
                    }				
					break;

				case DECODE_INIT:
					if (!audio_task_env.decoding_flag)
					{
						audio_task_env.decodeHandle->init();
                        #ifdef OPTEK_LINK_ENABLE
                        optek_link_codec_init(audio_in_stream.sample_rate, audio_task_env.decodeHandle->decode_id == DECODE_SBC);
                        #endif
                        tmp = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, 128*2*4, CODEC_MEM_ALIGN_32);
                        memset(tmp,0,128*2*4);
                        #ifdef DSP_EQ_SETTING_ENABLE
                        int in_channel = audio_in_stream.channel;
                        int out_channel = 2;//AUDIO_OUT_STREAM_CHANNEL;
                        int sample_rate = audio_in_stream.sample_rate;
                        eq_init(sample_rate, in_channel, out_channel);
                        #endif    
						#ifdef DSP_SONA_MODULE_INIT
                        sona_dsp_malloc_buf_init();
                        #endif                     
                    }
                    else
                    {
                        DBG_Assert(FALSE);
                    }

                    #ifdef LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE
                    sync_w_src_init();
                    #endif
                    
					break;

				case DECODE_EXIT:
					if (audio_task_env.decoding_flag)
					{
						DBG_Printf("codec exit:%s\r\n", audio_task_env.decodeHandle->dec_name);
						
                        audio_task_env.decoding_flag = FALSE;
						audio_task_env.decodeHandle->fini(DECODE_USER_EXIT);
                        audio_task_env.decoding_exit_req = FALSE;
                            
                        // tmp = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, 128*2*4, CODEC_MEM_ALIGN_32);

                        #ifdef DSP_EQ_SETTING_ENABLE
                        eq_uninit();
                        #endif       
						#ifdef DSP_SONA_MODULE_INIT
                        sona_dsp_buf_free();
                        #endif                     
					}
					break;

				default:
					break;
			}
		}

        if (audio_output_ready_to_receive_dataPtr())
        {
            if (audio_task_env.decoding_flag && !audio_task_env.dec_pause_flag)
            {
                #ifdef LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE
				sync_w_src_loop();
				#endif

                if (audio_task_env.decodeHandle->decode_frame(&pSrc, &out_size) == DECODE_SUCCESS && (audio_task_env.decoding_exit_req == FALSE))
                {
                    #ifdef SBC_ENCODE_ENABLE
                    if ((otk_psm_conn_present() == TRUE) && (bt_get_hp_status() == TRUE) && (bt_status.bt_a2dp_status == enBT_A2DP_STREAMING))
                    {
                        //input data is stereo and 16bit
                        xa_sbc_enc_and_send(pSrc, out_size>>1, NULL, NULL); //input size range:0~2048
                        xEventGroupSetBits(global_vari.eventGroup, BT_A2DP_TX_EVENT);
                    }
                    #endif
                    
                    #ifdef OPTEK_LINK_ENABLE
					optek_link_put_tx_data(pSrc, out_size);
                    #endif
                    #if 0 //(SOURCE_AUX == global_vari.audio_source)
                    {
                        optek_vol_24b_process(pSrc,pSrc,0x0b4ce07c,out_size/4); // 0x0b4ce07c_3db
                    }
                    #endif
                    #ifdef DSP_EQ_SETTING_ENABLE
                    int in_sample_rate = audio_in_stream.sample_rate;
                    int in_channel = audio_in_stream.channel;
                    #ifdef DSP_EQ_SETTING_24BIT_DECODE
                    #if 0//16bit in out
                    #if (IIS_OUTPUT_CHANNEL_BIT_NUM == 24)
                    optek_hifi2_24b_to_16b((short *)pSrc, (int *)pSrc, out_size/4);
                    out_size /= 2;
                    #endif

                    eq_process_16bit((int *)pSrc, (int *)pSrc, in_sample_rate, in_channel, out_size);

                    #if (IIS_OUTPUT_CHANNEL_BIT_NUM == 24)
                    optek_hifi2_16b_to_24b((int *)pSrc, (short *)pSrc, out_size/2);
                    out_size *= 2;
                    #endif

                    #else
                    #if (IIS_OUTPUT_CHANNEL_BIT_NUM == 16)
                    optek_hifi2_16b_to_24b((int *)se_link_buf_in, (short *)pSrc, out_size/2);
                    out_size *= 2;
                    #else
                    se_link_buf_in = pSrc;
                    #endif



                    memcpy(se_link_sub_buf_in, se_link_buf_in, out_size);

                    optek_hifi2_stereo_24b_to_2mono(se_link_sub_buf_in, se_link_sub_buf_in, out_size/4);

                    if(FULL_RANGE_OUTPUT_KEY_STATUS == FALSE)
                    {
                         optek_hifi2_stereo_24b_to_2mono(se_link_buf_in, se_link_buf_in, out_size/4);
                    }

                    eq_process_sub((int *)se_link_sub_buf_in, (int *)se_link_sub_buf_out, in_sample_rate, in_channel, out_size);


                    eq_process((int *)se_link_buf_in, (int *)se_link_buf_out, in_sample_rate, in_channel, out_size);

                    pSrc = audio_task_env.ouput_buffer[audio_task_env.index];

                    //audio_data_process_2_2ch_in_to_4ch_out(pSrc, se_link_buf_out, se_link_sub_buf_out, out_size);
                    
                    //sub i2s2 d0, dac i2s2 d1
                    audio_data_process_2_2ch_in_to_4ch_out(pSrc, se_link_sub_buf_out, se_link_buf_out, out_size);
                    out_size *= 2;


					//eq_process((int *)se_link_buf_in, (int *)se_link_buf_out, in_sample_rate, in_channel, out_size);

                    #if (IIS_OUTPUT_CHANNEL_BIT_NUM == 16)
                    optek_hifi2_24b_to_16b((short *)pSrc, (int *)se_link_buf_out, out_size/4);
                    out_size /= 2;
                    #else
                    //pSrc = se_link_buf_out;
                    #endif
                    #endif
                    #else
                    #if (IIS_OUTPUT_CHANNEL_BIT_NUM == 24)
                    optek_hifi2_24b_to_16b((short *)pSrc, (int *)pSrc, out_size/4);
                    out_size /= 2;
                    #endif

                    eq_process((short *)pSrc, (short *)pSrc, in_sample_rate, in_channel, out_size);

                    #if (IIS_OUTPUT_CHANNEL_BIT_NUM == 24)
                    optek_hifi2_16b_to_24b((int *)pSrc, (short *)pSrc, out_size/2);
                    out_size *= 2;
                    #endif
                    #endif
                    #else
                    
                    #ifdef DSP_SONA_MODULE_CODE
                    #if (IIS_OUTPUT_CHANNEL_BIT_NUM == 16)
                    optek_hifi2_16b_to_24b((int *)p_sona_temp_inbuffer, (short *)pSrc, out_size/2);
                    out_size *= 2;
                    #else
                    memcpy(p_sona_temp_inbuffer, pSrc, out_size);
                    #endif

                    if (DspSonaInit_en == TRUE)
                    {
                        #if 0
                        dsp_sona_module_24bit_process ((int *)pSrc, 2, out_size, (int *)pSrc, 4, &out_size);
                        #else
                        int sona_i;
                        int sona_process_loop = out_size/4/2/AEF_LEN; //aligned with 2*4*64 bytes  2--channel,int = 4 bytes,64 samples
                        int sona_process_in_len = AEF_LEN*4*2;
                        int sona_process_out_len;
                        int out_len = 0;
                        Updata_sona_info();
                        // memset(p_sona_temp_inbuffer, 0, out_size);
                        // memcpy(p_sona_temp_inbuffer, pSrc, out_size);
                        // memset(p_sona_temp_outbuffer, 0, out_size*2);

                        for (sona_i = 0; sona_i < sona_process_loop; sona_i++)
                        {
                            dsp_sona_module_24bit_process ((int *)&p_sona_temp_inbuffer[(AEF_LEN*2)*sona_i], 2, sona_process_in_len, (int *)&p_sona_temp_outbuffer[(AEF_LEN*4)*sona_i], 4, &sona_process_out_len);
                            out_len += sona_process_out_len;
                        }

                        out_size = out_len;
                        audio_data_process_24bit_4ch_in_to_4ch_out(p_sona_temp_outbuffer,p_sona_temp_outbuffer,out_size);

                        #endif
                    }
                    else
                    {
                        //memcpy(p_sona_temp_outbuffer, p_sona_temp_inbuffer, out_size);
                        audio_data_process_24bit_2ch_in_to_4ch_out(p_sona_temp_outbuffer, p_sona_temp_inbuffer, out_size);
                        out_size *= 2;
                    }


                    #if (IIS_OUTPUT_CHANNEL_BIT_NUM == 16)
                    optek_hifi2_24b_to_16b((short *)pSrc, (int *)p_sona_temp_outbuffer, out_size/4);
                    out_size /= 2;
                    #else
                    pSrc = p_sona_temp_outbuffer;
                    #endif
                    #endif



                    #if 0
                //  audio_task_env.ouput_buffer[audio_task_env.index]
                    memcpy(tmp,pSrc,out_size);
                    optek_hifi2_stereo_24b_to_2mono(pSrc, pSrc, out_size/4);
                    // audio_data_process_2ch_in_to_4ch_out( audio_task_env.ouput_buffer[audio_task_env.index], pSrc, out_size);
                    audio_data_process_2_2ch_in_to_4ch_out(audio_task_env.ouput_buffer[audio_task_env.index], pSrc, tmp, out_size);

                    pSrc =  audio_task_env.ouput_buffer[audio_task_env.index];
                    // pSrc = se_link_sub_buf_out;
                    out_size *= 2;
			        #endif
                    #endif
                }
                else
                {
                    DBG_Printf("decode end\n");
                    decode_exit_msgSend();

                    pSrc = audio_task_env.ouput_buffer[audio_task_env.index];

                    U32 out_samples;
                    if (0 == audio_task_env.dec_frame_in_samples)
                    {
                        out_samples = 240;
                    }

                    memset(pSrc, 0, out_samples*4);
                    out_size = out_samples;
                }

                out_size /= 4;

                #ifdef PEAKLEVEL_ENABLE
				
				#ifdef PEAKLEVEL_8CH
                uint16_t *p_in = pSrc;
                u32 sumL = 0,sumR = 0;
                static U16 cnt;

                if(cnt++ == 1000)
				{
					printf_info("ch0 :0x%x, ch1 :0x%x, ch2 :0x%x, ch3 :0x%x, ch4 :0x%x, ch5 :0x%x, ch6 :0x%x, ch7 :0x%x\n",\
					(abs(*p_in++)),(abs(*p_in++)),(abs(*p_in++)),(abs(*p_in++)),(abs(*p_in++)),(abs(*p_in++)),(abs(*p_in++)),(abs(*p_in++)));

					cnt = 0;
				}
				
                #else //PEAKLEVEL_8CH

                U32 peak = peak_level_meter_24bit(&peak_det_handle, pSrc, out_size<<2);

                static U16 cnt;

                if (peak > max_peak)
                    max_peak = peak;
                    
                if (cnt++ == 2000)
                {
                    cnt = 0;
                    DBG_Printf("Peak Level=0x%x\r\n", max_peak>>16);
                    max_peak = 0;
                }
				#endif //PEAKLEVEL_8CH
				
                #endif //PEAKLEVEL_ENABLE
            }

            else
            {
                pSrc = audio_task_env.ouput_buffer[audio_task_env.index];

                U32 out_samples;
                if (0 == audio_task_env.dec_frame_in_samples)
                {
                    out_samples = 240;
                }
                else
                {
                    out_samples = audio_task_env.dec_frame_in_samples;
                }

                memset(pSrc, 0, out_samples*4);
                out_size = out_samples;
            }
            
            memcpy(audio_task_env.ouput_buffer[audio_task_env.index], pSrc, out_size*4);
            audio_output_put_next_frame_dataPtr(audio_task_env.ouput_buffer[audio_task_env.index], out_size);
        }
        else
        {
            DBG_Assert(FALSE);
        }
    }
}



/**
 ****************************************************************************************
 * @brief Audio task create
 *
 * @return will not return
 *
 ****************************************************************************************
 */
void audio_task_init(void)
{
	if (xTaskCreate(audio_task, "audio task", TASK_SYSTEM_STACK_SIZE, 0, TASK_AUDIO_PRIORITY, &audio_task_env.pxCreatedTask) != TRUE)
    {
		DBG_Assert(FALSE);
    }
}


/**
 ****************************************************************************************
 * @brief Set new decode by sending a new handle to audio_task_env.queue
 *
 * @param[in]  decodeHandle     	Pointer to audio_task_env.decodeHandle
 *
 * @return will not return
 *
 ****************************************************************************************
 */
void decode_set_msgSend ( DECODE_DEVICE_STRU *decodeHandle )
{
	AUDIO_TASK_MSG_STRU qMsg;
	printf_info("dec set msgSend\r\n");

    qMsg.msg_id = DECODE_SET;
    qMsg.decodeHandle = decodeHandle;

    if (QUEUE_AUDIO_DECODE_SET(audio_task_env.queue, &qMsg, pdMS_TO_TICKS(0)) != pdTRUE )
	{
		printf_error("audio task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}

void decode_set_msgSend_from_isr ( DECODE_DEVICE_STRU *decodeHandle )
{
	AUDIO_TASK_MSG_STRU qMsg;
    AUDIO_TASK_MSG_STRU *qMsgData = (AUDIO_TASK_MSG_STRU *)&qMsg;

    DBG_iPrintf("%s\n\r", __func__);

    qMsg.msg_id = DECODE_SET;
    qMsg.decodeHandle = decodeHandle;

    BaseType_t xHigherPriorityTaskWoken, xResult;

    //xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    xResult = QUEUE_AUDIO_IN_RX_iSEND(audio_task_env.queue, qMsgData, xHigherPriorityTaskWoken);
    if( xResult == pdPASS)
    {
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }
    else
    {
        DBG_Assert(FALSE);
    }
}

void decode_init_msgSend (void)
{
	AUDIO_TASK_MSG_STRU qMsg;
	printf_info("dec init msgSend\r\n");

    qMsg.msg_id = DECODE_INIT;

    if (QUEUE_AUDIO_DECODE_SET(audio_task_env.queue, &qMsg, pdMS_TO_TICKS(0)) != pdTRUE )
	{
		printf_error("audio task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}

void decode_init_msgSend_from_isr ( void ) __INTERNAL_RAM_TEXT;
void decode_init_msgSend_from_isr (void)
{
	AUDIO_TASK_MSG_STRU qMsg;

    qMsg.msg_id = DECODE_INIT;

    BaseType_t xHigherPriorityTaskWoken, xResult;

    //xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    xResult = QUEUE_AUDIO_IN_RX_iSEND(audio_task_env.queue, &qMsg, xHigherPriorityTaskWoken);
    if( xResult == pdPASS)
    {
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }
    else
    {
        DBG_Assert(FALSE);
    }

}

void decode_start_w_init_msgSend ( void )
{
	AUDIO_TASK_MSG_STRU qMsg;

    printf_info("start with init req\r\n");
 
    qMsg.msg_id = DECODE_START;
    qMsg.param = TRUE;//with initial

    if (QUEUE_AUDIO_DECODE_SET(audio_task_env.queue, &qMsg, pdMS_TO_TICKS(0)) != pdTRUE )
	{
		printf_error("audio task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}

void decode_start_w_init_msgSend_from_isr ( void ) __INTERNAL_RAM_TEXT;
void decode_start_w_init_msgSend_from_isr ( void )
{
	AUDIO_TASK_MSG_STRU qMsg;
    AUDIO_TASK_MSG_STRU *qMsgData = (AUDIO_TASK_MSG_STRU *)&qMsg;

    //DBG_iPrintf("%s\n\r", __func__);

    qMsg.msg_id = DECODE_START;
    qMsg.param = TRUE;//with initial

    BaseType_t xHigherPriorityTaskWoken, xResult;

    //xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    xResult = QUEUE_AUDIO_IN_RX_iSEND(audio_task_env.queue, qMsgData, xHigherPriorityTaskWoken);
    if( xResult == pdPASS)
    {
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }
    else
    {
        DBG_Assert(FALSE);
    }
}

void decode_start_init_msgSend ( void )
{
	AUDIO_TASK_MSG_STRU qMsg;

    printf_info("start init req\r\n");
 
    qMsg.msg_id = DECODE_START;
    qMsg.param = FALSE;//without initial

    if (QUEUE_AUDIO_DECODE_SET(audio_task_env.queue, &qMsg, pdMS_TO_TICKS(0)) != pdTRUE )
	{
		printf_error("audio task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}

void decode_start_msgSend ( void )
{
	AUDIO_TASK_MSG_STRU qMsg;

    printf_info("start req\r\n");
    
    qMsg.msg_id = DECODE_START;
    qMsg.param = FALSE;
    
    if (QUEUE_AUDIO_DECODE_SET(audio_task_env.queue, &qMsg, pdMS_TO_TICKS(0)) != pdTRUE )
	{
		printf_error("audio task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}


void decode_start_msgSend_form_isr ( void ) __INTERNAL_RAM_TEXT;
void decode_start_msgSend_form_isr ( void )
{
	AUDIO_TASK_MSG_STRU qMsg;
    AUDIO_TASK_MSG_STRU *qMsgData = (AUDIO_TASK_MSG_STRU *)&qMsg;
    
    qMsg.msg_id = DECODE_START;
    qMsg.param = FALSE;
    
    BaseType_t xHigherPriorityTaskWoken, xResult;

    //xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    xResult = QUEUE_AUDIO_IN_RX_iSEND(audio_task_env.queue, qMsgData, xHigherPriorityTaskWoken);
    if( xResult == pdPASS)
    {
        if (xHigherPriorityTaskWoken)
        {
            portYIELD_FROM_ISR();
        }
    }
    else
    {
        DBG_Assert(FALSE);
    }
}

void decode_exit_msgSend ( void )
{
	AUDIO_TASK_MSG_STRU qMsg;

    printf_info("decode exit req\r\n");

    qMsg.msg_id = DECODE_EXIT;
    qMsg.param = DECODE_NONE;

    if (QUEUE_AUDIO_DECODE_SET(audio_task_env.queue, &qMsg, pdMS_TO_TICKS(0)) != pdTRUE )
	{
		printf_error("audio task queue is full\r\n");
		DBG_Assert(FALSE);
	}
}

void decode_exit_wait_forever_msgSend ( void )
{
	AUDIO_TASK_MSG_STRU qMsg;
    
	printf_info("decode exit start\r\n");
	
	decode_exit_req_set();
	
    qMsg.msg_id = DECODE_EXIT;
    qMsg.param = DECODE_NONE;

    if (QUEUE_AUDIO_DECODE_SET(audio_task_env.queue, &qMsg, pdMS_TO_TICKS(0)) != pdTRUE )
	{
		printf_error("audio task queue is full\r\n");
		DBG_Assert(FALSE);
	}

    while(1)
    {
        if (audio_task_env.decoding_flag == 0)
        {
            asm("nop");
            break;
        }
    }

	printf_info("decode exit end\r\n");
}

int decode_type_get( void ) __INTERNAL_RAM_TEXT;
int decode_type_get( void )
{
    return audio_task_env.decodeHandle->decode_id;
}

int decode_type_is_set( void ) __INTERNAL_RAM_TEXT;
int decode_type_is_set( void )
{
    return (audio_task_env.decodeHandle->decode_id != DECODE_NONE);
}

int decode_flag_get ( void )
{
    return audio_task_env.decoding_flag;
}

int decode_exit_req_set ( void )
{
    DBG_Printf("%s\n\r", __func__);
    audio_task_env.decoding_exit_req = TRUE;

    return 0;
}

int decode_exit_req_set_from_isr ( void )
{
    //DBG_iPrintf("%s\n\r", __func__);
    audio_task_env.decoding_exit_req = TRUE;
}

int decode_exit_req_get( void ) __INTERNAL_RAM_TEXT;
int decode_exit_req_get ( void )
{
    return audio_task_env.decoding_exit_req;
}

int decode_pause_status_get ( void )
{
    return audio_task_env.dec_pause_flag;
}

int decode_pause_enable ( void )
{
    audio_task_env.dec_pause_flag = TRUE;

    return 0;
}

int decode_pause_disable ( void )
{
    audio_task_env.dec_pause_flag = FALSE;

    return 0;
}

int decode_pasue_set ( void )
{
    DBG_Printf("%s\n\r", __func__);
    audio_task_env.dec_pause_flag = TRUE;

    return 0;
}

int decode_pasue_clr ( void )
{
    DBG_Printf("%s\n\r", __func__);
    audio_task_env.dec_pause_flag = FALSE;

    return 0;
}

int decode_pasue_get ( void )
{
    //DBG_Printf("%s\n\r", __func__);
    return audio_task_env.dec_pause_flag;

}
int decode_frame_samples_set ( U16 samples )
{
    audio_task_env.dec_frame_in_samples = samples;

    return 0;
}

int decode_frame_samples_get ( void )
{
    return audio_task_env.dec_frame_in_samples;
}
#ifdef AUDIO_CLOCK_SYNC_ENABLE
int cur_decode_src_speed_get(void)
{
	if (audio_task_env.decodeHandle->decode_info_get != NULL)
	{
		DECODE_INFO_t info;
		audio_task_env.decodeHandle->decode_info_get(&info);
		return info.src_speed;
	}
	else
	{
		return 0;
	}
}
#endif
