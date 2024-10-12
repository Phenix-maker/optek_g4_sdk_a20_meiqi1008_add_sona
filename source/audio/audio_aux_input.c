#include "os_config.h"


#ifdef AUX_ENABLE

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hw_pll.h"
#include "hw_audio_i2s.h"
#include "hw_audio_adc_dac.h"

#include "hal_gpio.h"
#include "hal_dma.h"

#include "fifo.h"
#include "interrupt.h"
#include "spdif_data_parse.h"
#include "spdif_process.h"
#include "audio_com.h"
#include "audio_input_receive_task.h"
#include "audio_aux_input.h"
#include "aux_dec.h"
#include "clock_slave_sync.h"
#include "audio_clock_sync.h"

#ifdef DEBUG_GPIO_PIN0_ENABLE
#undef DEBUG_GPIO_PIN0_ENABLE
#endif

#undef AUDIO_FIFO_ENABLE
#ifdef AUDIO_INPUT_RX_DMA_CLOSE
U8 auxRx_Dma_Opened;
#endif


struct {
    U32 *p_data;
    U32 size;//4bytes unit
    U32 overflow_flag;
    U32 dummy_buf[64];
    U32 receive_stop_req;
    U32 receive_stop;
    U32 receive_enable;
} audio_aux_input_env;


void audio_aux_input_rx_start(U32 *p_data, U32 size);
static void audio_aux_input_rx_cb(void);


void audio_aux_input_rx_init(void)
{
    int *pBuf = audio_input_rx_buf[0];

    DBG_Printf("%s\n\r", __func__);

	memset(&audio_input_rx_buf, 0, sizeof(audio_input_rx_buf));
    memset(&audio_aux_input_env, 0, sizeof(audio_aux_input_env));
    #ifdef AUX_INPUT_FROM_ADC
	hw_audio_adc_init(AUX_MODE_INPUT_ADC_MARK);
    #endif

#ifdef AUX_IN_STREAM_USED
    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
    if (auxRx_Dma_Opened == FALSE)
    {
      	DBG_Printf("aux Rx Dma Open\n\r");
      	auxRx_Dma_Opened = TRUE;

        #if ((defined AUX_INPUT_FROM_IIS0) || (defined AUX_INPUT_FROM_IIS_TDM))
        hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_CD_IN_FIFO_ADDR), SOURCE_DMA_MI2S_IN, 
                              DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
        #elif (defined AUX_INPUT_FROM_IIS1)
        hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_I2S_IN_FIFO_ADDR), SOURCE_DMA_IIS_ST_IN, 
                              DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);        
        #elif (defined AUX_INPUT_FROM_ADC)
        hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_DMIC_FIFO_ADDR), SOURCE_DMA_DIGIT_MIC_IN, 
                             DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
        #else
        DBG_Assert(FALSE);
     	#endif

        DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(AUX_IN_RX_DMA_ID)));
    }
    else
    {
        DBG_Printf("aux Rx Dma Opened\n\r");
    }
    #else
   	DBG_Printf("aux Rx Dma Open\n\r");

    #if ((defined AUX_INPUT_FROM_IIS0) || (defined AUX_INPUT_FROM_IIS_TDM))
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_CD_IN_FIFO_ADDR), SOURCE_DMA_MI2S_IN, 
                            DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #elif (defined AUX_INPUT_FROM_IIS1)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_I2S_IN_FIFO_ADDR), SOURCE_DMA_IIS_ST_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);        
    #elif (defined AUX_INPUT_FROM_ADC)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_DMIC_FIFO_ADDR), SOURCE_DMA_DIGIT_MIC_IN, 
                            DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #else
    DBG_Assert(FALSE);
    #endif

    DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(AUX_IN_RX_DMA_ID)));
    #endif //AUDIO_INPUT_RX_DMA_CLOSE

	audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_WAITING;

	//AUDIO_INPUT_BUF_SIZE -> U32
    audio_aux_input_rx_start(pBuf, AUDIO_INPUT_BUF_SIZE/4);

#else

    #if ((defined AUX_INPUT_FROM_IIS0) || (defined AUX_INPUT_FROM_IIS_TDM))
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_CD_IN_FIFO_ADDR), SOURCE_DMA_MI2S_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #elif (defined AUX_INPUT_FROM_IIS1)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_I2S_IN_FIFO_ADDR), SOURCE_DMA_IIS_ST_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);        
    #elif (defined AUX_INPUT_FROM_ADC)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_DMIC_FIFO_ADDR), SOURCE_DMA_DIGIT_MIC_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #else
    DBG_Assert(FALSE);
  	#endif

    DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(AUX_IN_RX_DMA_ID)));
  
    //aux_decocde_set();
    //aux_decocde_start_w_init();
#endif
}

void audio_aux_input_rx_start(U32 *p_data, U32 size)
{
    DBG_Printf("%s\n\r", __func__);

#ifdef AUX_IN_STREAM_USED
    DBG_Assert(p_data != NULL);
    
    audio_aux_input_rx_enable();

    #ifdef SOFT_INT_7_USED
    SOFT7_INT_ENABLE;
	XT_INTS_ON(LEVEL1_INT1_MASK);
    uiRxBufIndex = 0;
    #endif

    hal_dma_receive_data(AUX_IN_RX_DMA_ID, p_data, size);
    hw_audio_mi2s_rx_start();
#else
    DBG_Assert(p_data != NULL);

    audio_aux_input_env.receive_stop_req = FALSE;

    #if ((defined AUX_INPUT_FROM_IIS0) || (defined AUX_INPUT_FROM_IIS_TDM))
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_CD_IN_FIFO_ADDR), SOURCE_DMA_MI2S_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #elif (defined AUX_INPUT_FROM_IIS1)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_I2S_IN_FIFO_ADDR), SOURCE_DMA_IIS_ST_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);        
    #elif (defined AUX_INPUT_FROM_ADC)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_DMIC_FIFO_ADDR), SOURCE_DMA_DIGIT_MIC_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #else
    DBG_Assert(FALSE);
  	#endif

    audio_aux_input_rx_enable();

    hal_dma_receive_data(AUX_IN_RX_DMA_ID, p_data, size);
    hw_audio_mi2s_rx_start();
#endif
}

void audio_aux_input_rx_stop_req(void)
{
    //DBG_Printf("%s\n\r", __func__);

	audio_aux_input_env.size = 0;
    audio_aux_input_env.receive_stop_req = TRUE;
    audio_aux_input_rx_disable();
}

U8* pcm_dec_next_rx_buffer_get(void);
void audio_aux_input_rx_restart_req(U32 samples)
{
    //DBG_Printf("%s\n\r", __func__);

    audio_aux_input_env.receive_stop_req = FALSE;

    #if ((defined AUX_INPUT_FROM_IIS0) || (defined AUX_INPUT_FROM_IIS_TDM))
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_CD_IN_FIFO_ADDR), SOURCE_DMA_MI2S_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #elif (defined AUX_INPUT_FROM_IIS1)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_I2S_IN_FIFO_ADDR), SOURCE_DMA_IIS_ST_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);        
    #elif (defined AUX_INPUT_FROM_ADC)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_DMIC_FIFO_ADDR), SOURCE_DMA_DIGIT_MIC_IN, 
                          DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #else
    DBG_Assert(FALSE);
  	#endif

    audio_aux_input_rx_start(audio_aux_input_env.p_data, samples);
}


void audio_aux_input_put_next_frame_dataPtr(U32 *p_data, U32 size)
{
    DBG_Assert(p_data != NULL);
    DBG_Assert(audio_aux_input_env.size == 0);

    audio_aux_input_env.p_data = p_data;
    audio_aux_input_env.size = size;
}

U32 audio_aux_input_ready_to_receive_dataPtr(void)
{
    return (audio_aux_input_env.size == 0);
}

static void audio_aux_input_rx_cb(void)
{
#ifdef AUX_IN_STREAM_USED
    #ifndef SOFT_INT_7_USED
    static int index = 0;
    #endif

    AUDIO_INPUT_RX_TAST_MSG *qMsgData = (AUDIO_INPUT_RX_TAST_MSG *)&audio_input_rx_task_msg;

    #ifdef SOFT_INT_7_USED
    qMsgData->source = AUDIO_INPUT_RX_SOURCE_AUX;
    qMsgData->buf = audio_input_rx_buf[uiRxBufIndex];
    qMsgData->byte_length = AUDIO_INPUT_BUF_SIZE;
    uiRxBufIndex = !uiRxBufIndex;
    #else
    qMsgData->source = AUDIO_INPUT_RX_SOURCE_AUX;
    qMsgData->buf = audio_input_rx_buf[index];
    qMsgData->byte_length = AUDIO_INPUT_BUF_SIZE;
    index = !index;
    #endif

    #ifdef SOFT_INT_7_USED
    hal_dma_receive_data(AUX_IN_RX_DMA_ID, audio_input_rx_buf[uiRxBufIndex], qMsgData->byte_length/4);
    #else
    hal_dma_receive_data(AUX_IN_RX_DMA_ID, audio_input_rx_buf[index], qMsgData->byte_length/4);
    #endif

    if (audio_aux_input_env.receive_enable == FALSE)
    {
        return;
    }

	#ifdef DEBUG_GPIO_PIN0_ENABLE
    static int debug_index = 0;
    debug_index++;
    if (debug_index & 0x01)
    {
        DBG_PIN_HIGH(DEBEG_PIN);
    }
    else
    {
        DBG_PIN_LOW(DEBEG_PIN);
    }
	#endif

    #ifdef SOFT_INT_7_USED
	SOFT7_INT_SET;
    #else
    //send event to audio input rx task  
    BaseType_t xHigherPriorityTaskWoken, xResult;

    //xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    xResult = QUEUE_AUDIO_IN_RX_iSEND(audio_input_rx_task_msg.queue, qMsgData, xHigherPriorityTaskWoken);
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
    #endif

#else

	#ifdef DEBUG_GPIO_PIN0_ENABLE
    static int debug_index = 0;
    debug_index++;
    if (debug_index & 0x01)
    {
        DBG_PIN_HIGH(DEBEG_PIN);
    }
    else
    {
        DBG_PIN_LOW(DEBEG_PIN);
    }
	#endif

#if 0//def AUDIO_FIFO_ENABLE
    //audio_aux_input_env.p_data , audio_aux_input_env.size -> fifo
    AUDIO_FIFO_STREAM_PUT_DATA((U8 *)audio_aux_input_env.p_data, audio_aux_input_env.size*4);

    hal_dma_receive_data(AUX_IN_RX_DMA_ID, audio_aux_input_env.p_data, audio_aux_input_env.size);
#else
    if (audio_aux_input_env.size)
    {
        hal_dma_receive_data(AUX_IN_RX_DMA_ID, audio_aux_input_env.p_data, audio_aux_input_env.size);
        audio_aux_input_env.size = 0;
    }
    else
    {
        hal_dma_receive_data(AUX_IN_RX_DMA_ID, audio_aux_input_env.dummy_buf, sizeof(audio_aux_input_env.dummy_buf)/4);
        audio_aux_input_env.overflow_flag = 1;
        DBG_Assert(FALSE);
    }

    //send event to audio task
    #ifdef LOCAL_CLOCK_SYNC_WITH_AUDIO_SOURCE_ENABLE
    // printf_info("src_type = %d\n\r",sync_w_src_control.sync_clks.src_type);
    #if defined AUX_INPUT_FROM_IIS0 || defined AUX_INPUT_FROM_IIS1
    if (sync_w_src_control.sync_clks.src_type == ASOURCE_IIS_MASTER)
    {
        sync_w_src_control.sync_clks.localCurCount_L = read_ccount();
        if (sync_w_src_control.sync_clks.restart == TRUE)
        {
            sync_w_src_control.sync_clks.restart = FALSE;
            sync_w_src_control.sync_clks.localStartCount = sync_w_src_control.sync_clks.localCurCount_L;
            sync_w_src_control.sync_clks.count_bak = sync_w_src_control.sync_clks.localStartCount;
            sync_w_src_control.sync_clks.localCurCount_H = 0;
            sync_w_src_control.sync_clks.srcCount = 0;
        }
        else
        {
            if (sync_w_src_control.sync_clks.localCurCount_L < sync_w_src_control.sync_clks.count_bak)
                sync_w_src_control.sync_clks.localCurCount_H++;//carry
            sync_w_src_control.sync_clks.count_bak = sync_w_src_control.sync_clks.localCurCount_L;	

            #ifdef ADC_DAC_DATA_WIDTH_24_BIT
            sync_w_src_control.sync_clks.srcCount += AUDIO_INPUT_BUF_SIZE/8;//32bit,8bytes per sample	
            #else
            sync_w_src_control.sync_clks.srcCount += AUDIO_INPUT_BUF_SIZE/4;//16bit	
            #endif
        }
    }
    #endif

    #endif
    if (audio_aux_input_env.receive_enable == FALSE)
    {
        return;
    }

    BaseType_t xHigherPriorityTaskWoken, xResult;

	// xHigherPriorityTaskWoken must be initialised to pdFALSE.
	xHigherPriorityTaskWoken = pdFALSE;

	// Set bit 0 and bit 4 in xEventGroup.
    xResult = EVENT_AUDIO_INPUT_RX_iSET(global_vari.eventGroup, xHigherPriorityTaskWoken);
    if( xResult == pdPASS)
    {
        if (xHigherPriorityTaskWoken)//If the priority of the timer daemon task is higher than the priority of the currently running task (the task the interrupt interrupted)
        {
            portYIELD_FROM_ISR();
        }
    }
    else
    {
        DBG_Assert(FALSE);//pdFALSE will be returned if the timer service queue was full
    }
#endif

#endif
}

void audio_aux_input_rx_enable(void)
{
    DBG_Printf("%s\r\n", __func__);
    audio_aux_input_env.receive_enable =  TRUE;
}

void audio_aux_input_rx_disable(void)
{
    DBG_Printf("%s\r\n", __func__);
    audio_aux_input_env.receive_enable =  FALSE;

    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
	auxRx_Dma_Opened = FALSE;
    hal_dma_close(AUX_IN_RX_DMA_ID);
    #endif

    #ifdef AUX_INPUT_FROM_ADC
	hw_audio_adc_init(CLOSE_ALL_ADC_MARK);
    #endif
}

#endif //AUX_ENABLE
