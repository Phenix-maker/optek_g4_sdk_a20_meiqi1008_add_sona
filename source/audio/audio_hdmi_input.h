#ifndef __AUDIO_HDMI_INPUT_H__
#define __AUDIO_HDMI_INPUT_H__


extern U8 hdmiRx_Dma_Opened;


void audio_hdmi_input_rx_init(void);
void audio_hdmi_input_rx_start(U32 *p_data, U32 size);
void audio_hdmi_input_rx_stop_req(void);
void audio_hdmi_input_rx_put_next_frame_dataPtr(U32 *p_data, U32 size);
U32 audio_hdmi_input_ready_to_rx_dataPtr(void);

void audio_hdmi_input_rx_stop_set(void);
void audio_hdmi_input_rx_stop_clr(void);

void audio_hdmi_input_rx_enable(void);
void audio_hdmi_input_rx_disable(void);

#endif //_AUDIO_HDMI_INPUT_H_
