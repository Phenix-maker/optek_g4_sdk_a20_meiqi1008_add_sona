#include "os_config.h"

#include "c_def.h"
#include "oem.h"
#include "regmap.h"

#include "hw_pll.h"

#include "debug.h"
#include "fifo.h"

#include "audio_com.h"


#ifdef STREAM_BUFFER_USED_POINTER
U32 stream_size;
#endif


AUDIO_ENV_S audio_env;

void audio_com_init(void)
{

}


int audio_stream_speed_get(U32 sample_rate)
{
	int ret;

	DBG_Printf("audio stream sample rate:%d\n\r", sample_rate);

	switch (sample_rate)
	{
	case 32000:
	case 44100:
	case 48000:
		ret = SAMPLE_SPEED_1X;
		break;

	case 64000:
	case 88200:
	case 96000:
		ret = SAMPLE_SPEED_2X;
		break;

	case 128000:
	case 176400:
	case 192000:
		ret = SAMPLE_SPEED_4X;
		break;

	case 256000:
	case 352800:
	case 384000:
		ret = SAMPLE_SPEED_4X;
		break;

	case 768000:
		DBG_Assert(FALSE);
		ret = SAMPLE_SPEED_8X;
	break;

	default:
		ret = SAMPLE_SPEED_1X;
		break;
	}

	return ret;
}

void audio_clock_freq_set(U32 stream_srf)
{
	DBG_Printf ("%s:%d\n\r", __func__, stream_srf);

	switch (stream_srf)
	{
	case 196000:
		hw_pll_audio_clock_set(SF_BASE_DET, SPDIF_SF_4x);
		break;

	//for 4x speed
	case 192000:
		hw_pll_audio_clock_set(SF_BASE_48000, SPDIF_SF_4x);
		break;

	case 176400:
		hw_pll_audio_clock_set(SF_BASE_44100, SPDIF_SF_4x);
		break;

	case 128000:
		hw_pll_audio_clock_set(SF_BASE_32000, SPDIF_SF_4x);
		break;
	
	//for 2x speed
	case 96000:
		hw_pll_audio_clock_set(SF_BASE_48000, SPDIF_SF_2x);
		break;

	case 88200:
		hw_pll_audio_clock_set(SF_BASE_44100, SPDIF_SF_2x);
		break;
		
	case 64000:
		hw_pll_audio_clock_set(SF_BASE_32000, SPDIF_SF_2x);
		break;


	//for 1x speed
	case 48000:
		hw_pll_audio_clock_set(SF_BASE_48000, SPDIF_SF_1x);
		break;

	case 44100:
		hw_pll_audio_clock_set(SF_BASE_44100, SPDIF_SF_1x);
		break;

	case 32000:
		hw_pll_audio_clock_set(SF_BASE_32000, SPDIF_SF_1x);
		break;

	default:
		DBG_Assert(FALSE);
		DBG_Printf ("sample rate setting error\n\r");
		break;
	}
}
