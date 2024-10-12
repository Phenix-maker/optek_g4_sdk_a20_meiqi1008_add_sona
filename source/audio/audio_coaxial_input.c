#include "os_config.h"

#ifdef COAXIAL_ENABLE

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"
#include "hw_pll.h"
#include "hw_audio_i2s.h"

#include "hal_gpio.h"
#include "hal_dma.h"

#include "fifo.h"
#include "interrupt.h"
#include "audio_com.h"
#include "audio_input_receive_task.h"

#ifdef SPDIF_ENABLE
#include "spdif_data_parse.h"
#include "spdif_process.h"
#endif

#include "audio_coaxial_input.h"
#include "clock_slave_sync.h"


#ifdef DEBUG_GPIO_PIN0_ENABLE
#undef DEBUG_GPIO_PIN0_ENABLE
#endif


#ifdef AUDIO_INPUT_RX_DMA_CLOSE
U8 coaxialRX_Dma_Opened;
#endif


void audio_coaxial_input_rx_start(U32 *p_data, U32 size);
static void audio_coaxial_input_rx_cb(void);


struct {
    U32 *p_data;
    U32 size;//4bytes unit
    U32 overflow_flag;
    U32 dummy_buf[64];
    U32 receive_stop_req;
    U32 receive_stop;
    U32 receive_enable
} audio_coaxial_input_env;

#ifdef SOFT_INT_6_USED
static volatile U8 uiRxBufIndex;
#endif


void audio_coaxial_input_rx_init(void)
{
	DBG_Printf("%s\n\r", __func__);

#ifdef AUDIO_IN_STREAM_USED
    int *pBuf = audio_input_rx_buf[0];

	memset(&audio_input_rx_buf, 0, sizeof(audio_input_rx_buf));
    memset(&audio_coaxial_input_env, 0, sizeof(audio_coaxial_input_env));
  
    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
    if (coaxialRX_Dma_Opened == FALSE)
    {
    	DBG_Printf("spdif Rx Dma Open\n\r");
        coaxialRX_Dma_Opened = TRUE;

        hal_dma_transfer_init(SPDIF_IN_RX_DMA_ID, NULL, (U32 *)(DA_SPDIF_IN_FIFO_ADDR), SOURCE_DMA_SPDIF_IN, 
                            DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_coaxial_input_rx_cb);

        DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(SPDIF_IN_RX_DMA_ID)));
    }
    else
    {
    	DBG_Printf("spdif Rx Dma Opened\n\r");
    }
    #else
   	DBG_Printf("spdif Rx Dma Open\n\r");
    hal_dma_transfer_init(SPDIF_IN_RX_DMA_ID, NULL, (U32 *)(DA_SPDIF_IN_FIFO_ADDR), SOURCE_DMA_SPDIF_IN, 
                        DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_optical_input_rx_cb);

    DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(SPDIF_IN_RX_DMA_ID)));
    #endif //AUDIO_INPUT_RX_DMA_CLOSE

    audio_input_stream_rx_int();
    spdif_stream_preProcess_init();

    audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_IDLE;

    audio_coaxial_input_rx_start(pBuf, AUDIO_INPUT_BUF_SIZE/4);

#else

    memset(&audio_coaxial_input_env, 0, sizeof(audio_optical_input_env));

    hal_dma_transfer_init(SPDIF_IN_RX_DMA_ID, NULL, (U32 *)(DA_SPDIF_IN_FIFO_ADDR), SOURCE_DMA_SPDIF_IN, 
                            DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_optical_input_rx_cb);

    DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(SPDIF_IN_RX_DMA_ID)));

#endif //AUDIO_IN_STREAM_USED
}


void hw_audio_mi2s_rx_start(void);

void audio_coaxial_input_rx_start(U32 *p_data, U32 size)
{
#ifdef AUDIO_IN_STREAM_USED
    DBG_Printf("%s\n\r", __func__);
    DBG_Assert(p_data != NULL);
    
    audio_coaxial_input_rx_enable();

    #ifdef SOFT_INT_6_USED
    SOFT6_INT_ENABLE;
	XT_INTS_ON(LEVEL1_INT1_MASK);
    uiRxBufIndex = 0;
    #endif

    hal_dma_receive_data(SPDIF_IN_RX_DMA_ID, p_data, size);
    hw_audio_mi2s_rx_start();
#else
    DBG_Assert(p_data != NULL);
    
    hal_dma_receive_data(SPDIF_IN_RX_DMA_ID, p_data, size);
    audio_optical_input_env.size = 0;
#endif
}

void audio_coaxial_input_rx_stop_req(void)
{
    DBG_Printf("%s\n\r", __func__);

    audio_coaxial_input_env.receive_stop_req = TRUE;
    audio_coaxial_input_rx_disable();
}

U32 audio_coaxial_input_ready_to_rx_dataPtr(void)
{
    return (audio_coaxial_input_env.size == 0);
}

static void audio_coaxial_input_rx_cb(void) __INTERNAL_RAM_TEXT;
static void audio_coaxial_input_rx_cb(void)
{
#ifdef AUDIO_IN_STREAM_USED
    int rx_length;
    int *pRxBuf;
    #ifndef SOFT_INT_6_USED
    static int index = 0;
    #endif

    AUDIO_INPUT_RX_TAST_MSG *qMsgData = (AUDIO_INPUT_RX_TAST_MSG *)&audio_input_rx_task_msg;


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


	#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	if (1)
	{
		static U32 preTimerCount = 0;
		U32 tmp;
		tmp = read_ccount();
		if (tmp > preTimerCount)
		{
			devcur_ccount = tmp - preTimerCount;
		}
		else
		{
			devcur_ccount = 0x100000000 - preTimerCount + tmp;
		}	
		preTimerCount = tmp;

		if (spdif_rx_down_sample == 2)
		{
			//host samples
			host_samples += SPDIF_FRAME_SIZE/(4*2);
		}
		else if (spdif_rx_down_sample == 4)
		{
			//host samples
			host_samples += SPDIF_FRAME_SIZE/(4*4);
		}
		else
		{
			//host samples
			host_samples += SPDIF_FRAME_SIZE/(4);
		}
		
		if (devfirst_ccount == 0)
		{
			devfirst_ccount = devcur_ccount;
			host_samples = 0;
		}
	}
	#endif //CLOCK_SYNC_WITH_MASTER_ENABLE

 
    #ifdef SOFT_INT_6_USED
    qMsgData->source = AUDIO_INPUT_RX_SOURCE_COAXIAL;
    qMsgData->buf = audio_input_rx_buf[uiRxBufIndex];
    qMsgData->byte_length = AUDIO_INPUT_BUF_SIZE;
    uiRxBufIndex = !uiRxBufIndex;
    #else
    qMsgData->source = AUDIO_INPUT_RX_SOURCE_COAXIAL;
    qMsgData->buf = audio_input_rx_buf[index];
    qMsgData->byte_length = AUDIO_INPUT_BUF_SIZE;
    index = !index;
    #endif

    #ifdef SOFT_INT_6_USED
    hal_dma_receive_data(SPDIF_IN_RX_DMA_ID, audio_input_rx_buf[uiRxBufIndex], qMsgData->byte_length/4);
    #else
    hal_dma_receive_data(SPDIF_IN_RX_DMA_ID, audio_input_rx_buf[index], qMsgData->byte_length/4);
    #endif

    if (audio_coaxial_input_env.receive_enable == FALSE)
    {
        return;
    }

    #ifdef SOFT_INT_6_USED
	SOFT6_INT_SET;
    #else
    //send queue to audio input rx task  
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

    if (audio_coaxial_input_env.receive_stop_req)
    {
        audio_coaxial_input_env.receive_stop = TRUE;
        audio_coaxial_input_env.receive_stop_req = FALSE;//clear request
        return;
    }

    if (audio_coaxial_input_env.size)
    {
        hal_dma_receive_data(SPDIF_IN_RX_DMA_ID, audio_coaxial_input_env.p_data, audio_coaxial_input_env.size);
        audio_coaxial_input_env.size = 0;
    }
    else
    {
        hal_dma_receive_data(SPDIF_IN_RX_DMA_ID, audio_coaxial_input_env.dummy_buf, sizeof(audio_coaxial_input_env.dummy_buf)/4);
        audio_coaxial_input_env.overflow_flag = 1;
        DBG_Assert(FALSE);
    }
    //send event to audio task

    BaseType_t xHigherPriorityTaskWoken, xResult;

    //xHigherPriorityTaskWoken must be initialised to pdFALSE.
    xHigherPriorityTaskWoken = pdFALSE;

    //Set bit 0 and bit 4 in xEventGroup.
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
    
#endif //AUDIO_IN_STREAM_USED
}

void audio_coaxial_input_rx_enable(void)
{
    DBG_Printf("%s\r\n", __func__);
    audio_coaxial_input_env.receive_enable =  TRUE;
}

void audio_coaxial_input_rx_disable(void)
{
    DBG_Printf("%s\r\n", __func__);
    audio_coaxial_input_env.receive_enable =  FALSE;

    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
	coaxialRX_Dma_Opened = FALSE;
    hal_dma_close(SPDIF_IN_RX_DMA_ID);
    #endif
}

void audio_coaxial_input_rx_disable_from_isr(void)
{
    audio_coaxial_input_env.receive_enable =  FALSE;
}

#ifdef SOFT_INT_6_USED
void SoftInt6Isr(void) __INTERNAL_RAM_TEXT;
void SoftInt6Isr (void)
{
    int index = 1 - uiRxBufIndex;
    AUDIO_INPUT_RX_TAST_MSG *qMsgData = (AUDIO_INPUT_RX_TAST_MSG *)&audio_input_rx_task_msg;

	SOFT6_INT_CLR;

    qMsgData->source = AUDIO_INPUT_RX_SOURCE_COAXIAL;
    qMsgData->buf = audio_input_rx_buf[index];
    qMsgData->byte_length = AUDIO_INPUT_BUF_SIZE;

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
}
#endif //SOFT_INT_6_USED

#endif //COAXIAL_ENABLE
