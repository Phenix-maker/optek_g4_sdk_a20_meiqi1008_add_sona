#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hal_gpio.h"
#include "hal_dma.h"

#include "sdk_com.h"
#include "audio_com.h"

struct {
    U32 *p_data;
    U32 size;//4bytes unit
    U32 underflow_flag;
    U32 zero_buf[64];
} audio_output_env;

static void audio_output_transfer_cb(void);

void audio_output_init(void)
{
    memset(&audio_output_env, 0, sizeof(audio_output_env));
    #ifdef AUDIO_OUTPUT_FROM_IIS1
    hal_dma_transfer_init(AUDIO_OUTPUT_DMA_ID, (U32 *)(DA_I2S_OUT_FIFO_ADDR), NULL, SOURCE_DMA_IIS_ST_OUT,
                        DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 1, 0, 1, audio_output_transfer_cb);
    #elif ( (defined AUDIO_OUTPUT_FROM_IIS2) || (defined AUDIO_OUTPUT_FROM_PWM) || (defined AUDIO_OUTPUT_FROM_DAC) )
    hal_dma_transfer_init(AUDIO_OUTPUT_DMA_ID, (U32 *)(DA_PP_CLASSD_FIFO_ADDR), NULL, SOURCE_DMA_DA_PP,
                        DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 1, 0, 1, audio_output_transfer_cb);
    #else
	#error Must select IIS1, IIS2 or PWM for audio out!
    #endif

    DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(AUDIO_OUTPUT_DMA_ID)));
}

void audio_output_start(U32 *p_data, U32 size)
{
    DBG_Assert(p_data != NULL);
    
    hal_dma_send_data(AUDIO_OUTPUT_DMA_ID, p_data, size);
    audio_output_env.size = 0;
}

void audio_output_put_next_frame_dataPtr(U32 *p_data, U32 size)
{
    DBG_Assert(p_data != NULL);

    #ifdef DOLBY_DDPLUS71_DEC_ENABLE
    //While playing 7_difmus_3.ec3 by PC at CPU_FREQ_BASE_32X, come here!
    #else
    DBG_Assert(audio_output_env.size == 0);
    #endif

    audio_output_env.p_data = p_data;
    audio_output_env.size = size;
}

U32 audio_output_ready_to_receive_dataPtr(void)
{
    return (audio_output_env.size == 0);
}

static void audio_output_transfer_cb(void) __INTERNAL_RAM_TEXT;
static void audio_output_transfer_cb(void)
{
    if (audio_output_env.size)
    {
        hal_dma_send_data(AUDIO_OUTPUT_DMA_ID, audio_output_env.p_data, audio_output_env.size);
        audio_output_env.size = 0;
    }
    else
    {
        hal_dma_send_data(AUDIO_OUTPUT_DMA_ID, audio_output_env.zero_buf, sizeof(audio_output_env.zero_buf)/4);
        audio_output_env.underflow_flag = 1;
		
        #ifdef DOLBY_DDPLUS71_DEC_ENABLE
        //While playing 7_difmus_3.ec3 by PC at CPU_FREQ_BASE_32X, come here!
        #else
		DBG_Assert(FALSE);
		#endif
    }

    BaseType_t xHigherPriorityTaskWoken, xResult;

	// xHigherPriorityTaskWoken must be initialised to pdFALSE.
	xHigherPriorityTaskWoken = pdFALSE;

    xResult = EVENT_AUDIO_DECODE_iSET(global_vari.eventGroup, xHigherPriorityTaskWoken);
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
}
