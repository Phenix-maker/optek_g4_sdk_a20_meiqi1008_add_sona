#include "os_config.h"

#include "hw_pll.h"
#include "hw_timer.h"

#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "audio_input_receive_task.h"
#include "audio_aux_input.h"

#include "codec.h"
#include "internal_dec.h"

#define Q31_SCALE (1 << 31)
#define VOLUME_SCALE(volume) ((int32_t)((volume) * Q31_SCALE))
#define CHANNEL_NUM 6

typedef struct {
    uint32_t frequency;
    float volume;
    size_t sample_index;             // Sample index for controlling the period
    size_t one_period_samples;      // Half period samples
    int32_t fixedPoint_volume;
    int32_t nfixedPoint_volume;
    int32_t update_requested; // Flag to indicate frequency update
} AudioChannel;

// Define channels with different frequencies, volumes, and update functions
AudioChannel audio_channels[CHANNEL_NUM];

void set_audio_channel(int32_t channel_index, uint32_t frequency, float volume) {
    audio_channels[channel_index].frequency = frequency;
    audio_channels[channel_index].volume = volume;
    audio_channels[channel_index].update_requested = true;
}

void generate_alternating_wave(int16_t *buffer, size_t num_channels, size_t num_samples) {
    // Iterate over each sample
    for (size_t i = 0; i < num_samples; ++i) {
        // Iterate over each channel
        for (size_t j = 0; j < num_channels; ++j) {

            // Check if the sample index reached half period samples
            if (audio_channels[j].sample_index == audio_channels[j].one_period_samples) {
                audio_channels[j].sample_index = 0;  // Reset index

                // Update half period samples if frequency update is requested
                if (audio_channels[j].update_requested) {
                    audio_channels[j].update_requested = false;
                    audio_channels[j].one_period_samples = (size_t)(audio_in_stream.sample_rate / audio_channels[j].frequency);
                    audio_channels[j].fixedPoint_volume = VOLUME_SCALE(audio_channels[j].volume);
                    audio_channels[j].nfixedPoint_volume = -VOLUME_SCALE(audio_channels[j].volume);
                }
            }

            // Change sign every half period
            int32_t sample_value = (audio_channels[j].sample_index < audio_channels[j].one_period_samples / 2)
                                       ? audio_channels[j].fixedPoint_volume
                                       : audio_channels[j].nfixedPoint_volume;

            // Assign the sample value to the buffer
            buffer[i * num_channels + j] = (int16_t)(sample_value>>16);
            audio_channels[j].sample_index++;
        }
    }
}


typedef struct {
    U8 *read_buffer[2];
    U8 *rx_postProcessBuf;
    U16 discard_frame_cnt; //discard some frames when decoding start
    U8 first_frame;        //for sync
    U8 index;
} INTERNAL_DEC_ENV_STRU;

INTERNAL_DEC_ENV_STRU internal_dec_env = {0};


#define PROCESS_SAMPLES (256)
 
void internal_dec_init(void)
{
   int size;
 
	DBG_Printf("%s", __func__);

	#ifdef MCLK_IN_AS_AUDIO_OUT_CLK
	if (audio_in_stream.sample_speed == SAMPLE_SPEED_4X)
	{
		hw_audio_mi2s_osr_div_set(2);
	}
	else
	{
		hw_audio_mi2s_osr_div_set(0);
	}
	//MClk:external clock
	hw_audio_mi2s_mclk_switch(TRUE);
	#else
	hw_audio_mi2s_osr_div_set(4);
	//MClk:internal clock
	hw_audio_mi2s_mclk_switch(FALSE);
	#endif

	audio_in_stream.channel = AUDIO_IN_STREAM_CHANNEL;
	audio_in_stream.sample_rate = AUDIO_IN_STREAM_SAMPLE_RATE;
	audio_in_stream.sample_speed = audio_stream_speed_get(audio_in_stream.sample_rate);

	hw_audio_mi2s_in_channel_set(audio_in_stream.channel);

	audio_out_stream.channel = AUDIO_OUT_STREAM_CHANNEL;
	hw_audio_mi2s_out_channel_set(audio_out_stream.channel);

	audio_clock_freq_set(audio_in_stream.sample_rate);

	codec_malloc_init(codec_fast_mem, CODEC_FAST_MEM_SIZE, codec_slow_mem, CODEC_SLOW_MEM_SIZE);

	size = (AUDIO_OUT_STREAM_CHANNEL*PROCESS_SAMPLES*4);
	internal_dec_env.read_buffer[0] = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_32);

	set_audio_channel(0, 100, 0.3);
	set_audio_channel(1, 100, 0.3);
	set_audio_channel(2, 100, 0.1);
	set_audio_channel(3, 1000, 0.3);
	set_audio_channel(4, 10000, 0.3);
	set_audio_channel(5, 15000, 0.3);
}

void internal_dec_fini(int reason)
{
	DBG_Printf("%s", __func__);

}

int internal_dec_info_get(DECODE_INFO_STRU *info)
{
	info->src_speed = 0;

	return 1;
}

/*******************************************************************************************************************
 * 
 * 
 *******************************************************************************************************************/


int internal_dec_decode_frame(U8 **pout, U16 *plen)
{	
    int i;
    short *p = internal_dec_env.read_buffer[0];

#if 1
#if 1
	    // Generate alternating wave
    generate_alternating_wave(p, AUDIO_OUT_STREAM_CHANNEL, PROCESS_SAMPLES);

#else
    int len = INTERNAL_PCM_BLOCK_SIZE;
	for (i = 0; i < len/2/2/8; i++ )
	{
		*p++ = L_LEVEL;
		*p++ = R_LEVEL;
	}
	for (i = 0; i < len/2/2/8; i++ )
	{
		*p++ = -(L_LEVEL);
		*p++ = -(R_LEVEL);
	}

	for (i = 0; i < len/2/2/8; i++ )
	{
		*p++ = L_LEVEL;
		*p++ = R_LEVEL;
	}
	for (i = 0; i < len/2/2/8; i++ )
	{
		*p++ = -(L_LEVEL);
		*p++ = -(R_LEVEL);
	}

	for (i = 0; i < len/2/2/8; i++ )
	{
		*p++ = L_LEVEL;
		*p++ = R_LEVEL;
	}
	for (i = 0; i < len/2/2/8; i++ )
	{
		*p++ = -(L_LEVEL);
		*p++ = -(R_LEVEL);
	}

	for (i = 0; i < len/2/2/8; i++ )
	{
		*p++ = L_LEVEL;
		*p++ = R_LEVEL;
	}
	for (i = 0; i < len/2/2/8; i++ )
	{
		*p++ = -(L_LEVEL);
		*p++ = -(R_LEVEL);
	}
#endif					
#else
    int len = INTERNAL_PCM_BLOCK_SIZE;
	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = 0x1000;
		*p++ = 0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = -0x1000;
		*p++ = -0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = 0x1000;
		*p++ = 0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = -0x1000;
		*p++ = -0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}
	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = 0x1000;
		*p++ = 0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = -0x1000;
		*p++ = -0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}
	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = 0x1000;
		*p++ = 0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	for (i = 0; i < len/2/8/8; i++ )
	{
		*p++ = -0x1000;
		*p++ = -0x6000;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}
#endif

	*pout = internal_dec_env.read_buffer[0];
	*plen = INTERNAL_PCM_BLOCK_SIZE;

	return DECODE_SUCCESS;
}

static const DECODE_DEVICE_STRU internal_decode = 
{
    DECODE_INTERNAL,
	"internal decode",	
	internal_dec_init,
	internal_dec_fini,
	internal_dec_decode_frame,
    internal_dec_info_get
};

void internal_decocde_set(void)
{
    DBG_Printf("%s\r\n", __func__);
    decode_set_msgSend(&internal_decode);
}

void internal_decocde_start_w_init(void)
{
	DBG_Printf("%s", __func__);
    decode_start_w_init_msgSend();
}
