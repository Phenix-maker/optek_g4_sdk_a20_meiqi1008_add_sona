#ifndef __AUDIO_COAXIAL_INPUT_H__
#define __AUDIO_COAXIAL_INPUT_H__


extern U8 coaxialRX_Dma_Opened;


void audio_coaxial_input_init(void);
void audio_coaxial_input_start(U32 *p_data, U32 size);
void audio_coaxial_input_receive_stop_req(void);
void audio_coaxial_input_put_next_frame_dataPtr(U32 *p_data, U32 size);
U32 audio_coaxial_input_ready_to_receive_dataPtr(void);

void audio_coaxial_input_rx_enable(void);
void audio_coaxial_input_rx_disable(void);

#endif //__AUDIO_COAXIAL_INPUT_H__

