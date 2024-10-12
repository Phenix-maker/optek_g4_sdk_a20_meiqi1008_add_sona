#ifndef __USB_DEC_H__
#define __USB_DEC_H__

extern U8 USB_SPEAKER_STREAM_START[];
extern U8 USB_SPEAKER_STREAM_END[];


//for decoding the size of one frame
//dec_frame_size
#define USB_PCM_BLOCK_SIZE        AD_PCM_BLOCK_SIZE

//#define AD_PCM_BLOCK_SAMPLES    (dec_frame_size/(app_main_data.playing_stream_sample_bits*app_main_data.playing_stream_chans/8))

#define MAX_NOISE_GAIN        128


#endif//__USB_DEC_H__
