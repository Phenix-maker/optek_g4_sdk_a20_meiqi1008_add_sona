#ifndef __AUDIO_ARC_INPUT_H__
#define __AUDIO_ARC_INPUT_H__


extern U8 arcRx_Dma_Opened;


void audio_arc_input_init(void);
void audio_arc_input_start(U32 *p_data, U32 size);
void audio_arc_input_receive_stop_req(void);
void audio_arc_input_put_next_frame_dataPtr(U32 *p_data, U32 size);
U32 audio_arc_input_ready_to_receive_dataPtr(void);


void audio_arc_input_rx_enable(void);
void audio_arc_input_rx_disable(void);


#endif //_AUDIO_ARC_INPUT_H_
