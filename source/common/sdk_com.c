#include "os_config.h"

#include "sdk_com.h"

#ifdef SPDIF_ENABLE
#include "spdif_process.h"
#endif


GLOBAL_VARI_STRU global_vari;
AUDIO_IN_STREAM_STRUC audio_in_stream;
AUDIO_OUT_STREAM_STRUC audio_out_stream;


void sdk_common_init(void)
{
    #ifdef RTOS_ENABLE
    global_vari.eventGroup = xEventGroupCreate();
    #endif

    memset(&audio_in_stream, 0, sizeof(audio_in_stream));
    audio_in_stream.prepare_status = AUDIO_IN_STREAM_PREPARE_IDLE;
    audio_in_stream.sample_rate = 48000;
    audio_in_stream.sample_speed = SAMPLE_SPEED_1X;
	audio_in_stream.channel = 2;

    memset(&audio_out_stream, 0, sizeof(audio_out_stream));
    audio_out_stream.bit_width = 16;
    audio_out_stream.sample_rate = 48000;
    audio_out_stream.bit_rate = 1536000;
    audio_out_stream.status = 0;

    #ifdef SPDIF_ENABLE
    memset(&spdif_in_stream, 0, sizeof(spdif_in_stream));
    #endif
}
