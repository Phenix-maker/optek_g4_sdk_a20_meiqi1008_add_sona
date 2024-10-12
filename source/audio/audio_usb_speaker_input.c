#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"
#include "fifo.h"
#ifdef USB_DEV_ENABLE
#include "hw_timer.h"
#include "hw_pll.h"
#include "hw_audio_adc_dac.h"
#include "hal_gpio.h"
#include "hal_dma.h"


#include "interrupt.h"
#include "codec_malloc.h"
#include "audio_com.h"
#include "audio_input_receive_task.h"
#include "audio_usb_speaker_input.h"
#include "usb_speaker_dec.h"

#ifdef AUDIO_INPUT_RX_DMA_CLOSE
U8 usbslaveRx_Dma_Opened;
#endif

struct {
    U32 *p_data;
    U32 size;//4bytes unit
    U32 overflow_flag;
    U32 dummy_buf[64];
    U32 receive_stop_req;
    U32 receive_stop;
    U32 receive_enable;
} audio_usb_speaker_input_env;



#ifdef USB_DEV_UPDATA_MIC_ENABLE
FIFO MicInFifo;
FIFO UsbDevUpFifo;

U32 usbslave_input_rx_buf[2][AUDIO_INPUT_BUF_SIZE] __attribute__(( aligned (64) ));
U32 *usbdev_up_fifo_buf;
U32 *usbdev_mic_in_fifo_buf;
#endif

static void audio_usb_speaker_input_rx_cb(void);

void audio_usb_speaker_input_rx_init(void)
{
	DBG_Printf("%s\n\r", __func__);
    #ifdef AUDIO_INPUT_FROM_ADC
	hw_audio_adc_init(USB_DEV_MIC_INPUT_ADC_MARK);
    #endif

    memset(&audio_usb_speaker_input_env, 0, sizeof(audio_usb_speaker_input_env));

    #ifndef USB_DEV_UPDATA_MIC_ENABLE
	
	audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_WAITING;

    #else //USB_DEV_UPDATA_MIC_ENABLE

    memset(&usbslave_input_rx_buf, 0, sizeof(usbslave_input_rx_buf));

    usbdev_mic_in_fifo_buf = NULL;
    usbdev_up_fifo_buf = NULL;

	audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_WAITING;

    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
    if (usbslaveRx_Dma_Opened == FALSE)
    {
      	DBG_Printf("usbslave Rx Dma Open\n\r");
      	usbslaveRx_Dma_Opened = TRUE;

        #if ((defined AUDIO_INPUT_FROM_IIS0) || (defined AUDIO_INPUT_FROM_IIS_TDM))
        hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_CD_IN_FIFO_ADDR), SOURCE_DMA_MI2S_IN, 
                              DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_usb_speaker_input_rx_cb);
        #elif (defined AUDIO_INPUT_FROM_IIS1)
        hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_I2S_IN_FIFO_ADDR), SOURCE_DMA_IIS_ST_IN, 
                              DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_usb_speaker_input_rx_cb);
        #elif (defined AUDIO_INPUT_FROM_ADC)
        hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_DMIC_FIFO_ADDR), SOURCE_DMA_DIGIT_MIC_IN, 
                             DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_usb_speaker_input_rx_cb);
        #else
        DBG_Assert(FALSE);
     	#endif

        DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(AUX_IN_RX_DMA_ID)));
    }
    else
    {
        DBG_Printf("usbslave Rx Dma Opened\n\r");
    }
	
    #else //AUDIO_INPUT_RX_DMA_CLOSE

    #if ((defined AUDIO_INPUT_FROM_IIS0) || (defined AUDIO_INPUT_FROM_IIS_TDM))
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_CD_IN_FIFO_ADDR), SOURCE_DMA_MI2S_IN, 
                            DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #elif (defined AUDIO_INPUT_FROM_IIS1)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_I2S_IN_FIFO_ADDR), SOURCE_DMA_IIS_ST_IN, 
                            DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_aux_input_rx_cb);
    #elif (defined AUDIO_INPUT_FROM_ADC)
    hal_dma_transfer_init(AUX_IN_RX_DMA_ID, NULL, (U32 *)(DA_DMIC_FIFO_ADDR), SOURCE_DMA_DIGIT_MIC_IN, 
                            DMA_TRANS_4BYTEs, DMA_BURST_FOUR, 0, 1, 1, audio_usb_speaker_input_rx_cb);
    #else
    DBG_Assert(FALSE);
    #endif

    DMA_GCNTR = (0x00001000 | (DMA_CHANNEL(AUX_IN_RX_DMA_ID)));
	#endif //AUDIO_INPUT_RX_DMA_CLOSE

    #endif //USB_DEV_UPDATA_MIC_ENABLE
}


void hw_audio_mi2s_rx_start(void);

void audio_usb_speaker_input_rx_start(U32 *p_data, U32 size)
{
    DBG_Printf("%s\n\r", __func__);
	
    #ifdef USB_DEV_UPDATA_MIC_ENABLE
    hal_dma_receive_data(AUX_IN_RX_DMA_ID, usbslave_input_rx_buf[0], AUDIO_INPUT_BUF_SIZE/4);
    #endif
}

void audio_usb_speaker_input_put_next_frame_dataPtr(U32 *p_data, U32 size)
{

}

void audio_usb_speaker_input_receive_stop_req(void)
{
    DBG_Printf("%s\n\r", __func__);

    audio_usb_speaker_input_env.receive_stop_req = TRUE;
    audio_usb_speaker_input_rx_disable();
}


U32 audio_usb_speaker_input_ready_to_receive_dataPtr(void)
{
    return (audio_usb_speaker_input_env.size == 0);
}

static void audio_usb_speaker_input_rx_cb(void)
{
    #ifdef USB_DEV_UPDATA_MIC_ENABLE
    static int index = 0;

    index = !index;

    hal_dma_receive_data(AUX_IN_RX_DMA_ID, usbslave_input_rx_buf[index], AUDIO_INPUT_BUF_SIZE/4);

    if (usbdev_mic_in_fifo_buf != NULL)
	{
        int free_space_len = fifo_get_free_space(&MicInFifo);
        if (free_space_len < AUDIO_INPUT_BUF_SIZE)
        {
            //DBG_iPrintf("overflow\n\r");
        }

        int wlen = fifo_put_data(&MicInFifo, usbslave_input_rx_buf[!index], AUDIO_INPUT_BUF_SIZE);
	}

    #endif
}

void audio_usb_speaker_input_rx_enable(void)
{
    DBG_Printf("%s\r\n", __func__);
    audio_usb_speaker_input_env.receive_enable =  TRUE;

    #ifdef AUDIO_INPUT_FROM_ADC
	hw_audio_adc_init(USB_DEV_MIC_INPUT_ADC_MARK);
    #endif

}

void audio_usb_speaker_input_rx_disable(void)
{
    DBG_Printf("%s\r\n", __func__);
    audio_usb_speaker_input_env.receive_enable =  FALSE;

    #ifdef AUDIO_INPUT_RX_DMA_CLOSE
	usbslaveRx_Dma_Opened = FALSE;
    hal_dma_close(USB_IN_DMA_ID);
    #endif
    #ifdef AUDIO_INPUT_FROM_ADC
	hw_audio_adc_init(CLOSE_ALL_ADC_MARK);
    #endif
}

U8 audio_usb_speaker_input_rx_status_get(void)
{
    printf_info("receive_enable:%d\n\r",audio_usb_speaker_input_env.receive_enable);
    return audio_usb_speaker_input_env.receive_enable;
}
#else
FIFO MicInFifo;
U32 *usbdev_mic_in_fifo_buf;
#endif //USB_ENABLE
