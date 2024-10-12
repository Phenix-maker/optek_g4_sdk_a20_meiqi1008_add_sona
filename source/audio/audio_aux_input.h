#ifndef __AUDIO_AUX_INPUT_H__
#define __AUDIO_AUX_INPUT_H__


extern U8 auxRx_Dma_Opened;


void audio_aux_input_init(void);
void audio_aux_input_rx_start(U32 *p_data, U32 size);
void audio_aux_input_receive_stop_req(void);
void audio_aux_input_put_next_frame_dataPtr(U32 *p_data, U32 size);
U32 audio_aux_input_ready_to_receive_dataPtr(void);

void audio_aux_input_rx_stop_set(void);
void audio_aux_input_rx_stop_clr(void);

void audio_aux_input_rx_enable(void);
void audio_aux_input_rx_disable(void);

void audio_aux_input_rx_enable(void );
void audio_aux_input_rx_disable(void );

#endif //__AUDIO_AUX_INPUT_H__