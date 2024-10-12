#include "os_config.h"
#include "regmap.h"


#ifdef AUDIO_OPTEK_DAPP
#include "codec_malloc.h"

#include "Audio_Device.h"
#include "audio_optek_dapp.h"
#include "optek_eq_tune_by_uart.h"

#ifdef DSP_HIFI_LIB_OLD
#include "optek_hifi2_dapp.h"
#else
#include "optek_hifi2_dapp_strided.h"
#endif

#define AUDIO_OPTEK_DAPP_FUNCTION_ENABLE

const int bypass_table[30*5] =
{
	0x30000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x30000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
};


const int eq_flat_table[EQ_NUM][5] =
{
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

const int eq_classic_table[EQ_NUM][5] =
{
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

const int eq_rock_table[EQ_NUM][5] =
{
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

const int eq_jazz_table[EQ_NUM][5] =
{
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

const int eq_pop_table[EQ_NUM][5] =
{
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
};


const int bass_table[BASS_MAX+1][5] =
{
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x10000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x30000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x50000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x60000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x70000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x7a000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x7F000000,0x00000000,0x00000000,0x00000000,0x00000000,
};
const int treble_table[TREBLE_MAX+1][5] =
{
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x10000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x30000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x50000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x60000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x70000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x7a000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x7F000000,0x00000000,0x00000000,0x00000000,0x00000000,
};

void audio_optek_dapp_init(void)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_OPTEK_DAPP_FUNCTION_ENABLE
    optek_dapp_init();

	#ifndef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
    audio_vari.volume = VOLUME_DEFAULT;
    audio_vari.mute = TRUE;
    audio_vari.bass = BASS_DEFAULT;
    audio_vari.treble = TREBLE_DEFAULT;
    audio_vari.eq_mode = EQ_DEFAULT;
	#endif
    #endif
}

void audio_optek_dapp_open(void)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_OPTEK_DAPP_FUNCTION_ENABLE
	#ifdef DSP_HIFI_LIB_OLD
    optek_dapp_open();
	#else

	#ifndef DSPEQ_SETTING_ENABLE
	U8 * pbuffer = dsp_set_buffer;
	
	optek_dapp_strero = optek_dapp_open(pbuffer, OPTEK_DSP_LR_MAX_EQ, bypass_table, OPTEK_DSP_LR_DC, OPTEK_DSP_LR_DRC, OPTEK_DSP_LR_DBDRC, OPTEK_DSP_LR_3D, OPTEK_DSP_LR_EBASS);
	#else

    U8 * pbuffer = dsp_set_buffer;
	int size = 0;

	#ifdef DSPEQ_TUNING_LR_CHANNEL
	optek_dsp_lr = optek_dapp_open(pbuffer, OPTEK_DSP_LR_MAX_EQ, bypass_table, OPTEK_DSP_LR_DC, OPTEK_DSP_LR_DRC, OPTEK_DSP_LR_DBDRC, OPTEK_DSP_LR_3D, OPTEK_DSP_LR_EBASS);
	//size = MEM_ALIGN((optek_dapp_alloc(OPTEK_DSP_LR_MAX_EQ, OPTEK_DSP_LR_DC, OPTEK_DSP_LR_DRC, OPTEK_DSP_LR_DBDRC, OPTEK_DSP_LR_3D, OPTEK_DSP_LR_EBASS)), 8);
	//pbuffer += size;
	pbuffer += 1024;
	optek_dsp_setting_init(optek_dsp_lr, (OPTEK_DSP_RXs *)dsp_setting);
	//printf_error("0-pbuffer : 0x%x, %d\r\n", pbuffer, size);
	#endif

	#ifdef DSPEQ_TUNING_CENTER_CHANNEL
	optek_dsp_center = optek_dapp_open(pbuffer, OPTEK_DSP_CENTER_MAX_EQ, bypass_table, OPTEK_DSP_CENTER_DC, OPTEK_DSP_CENTER_DRC, OPTEK_DSP_CENTER_DBDRC, OPTEK_DSP_CENTER_3D, OPTEK_DSP_CENTER_EBASS);
	//size = MEM_ALIGN((optek_dapp_alloc(OPTEK_DSP_CENTER_MAX_EQ, OPTEK_DSP_CENTER_DC, OPTEK_DSP_LR_DRC, OPTEK_DSP_LR_DBDRC, OPTEK_DSP_CENTER_3D, OPTEK_DSP_CENTER_EBASS)), 8);
	//pbuffer += size;
	pbuffer += 1024;
	optek_dsp_setting_init(optek_dsp_center, (OPTEK_DSP_RXs *)dsp_setting);
	//printf_error("1-pbuffer : 0x%x, %d\r\n", pbuffer, size);
	#endif

	#ifdef DSPEQ_TUNING_SUB_CHANNEL
	optek_dsp_sub = optek_dapp_open(pbuffer, OPTEK_DSP_SUB_MAX_EQ, bypass_table, OPTEK_DSP_SUB_DC, OPTEK_DSP_SUB_DRC, OPTEK_DSP_SUB_DBDRC, OPTEK_DSP_SUB_3D, OPTEK_DSP_SUB_EBASS);
	//size = MEM_ALIGN((optek_dapp_alloc(OPTEK_DSP_SUB_MAX_EQ, OPTEK_DSP_SUB_DC, OPTEK_DSP_LR_DRC, OPTEK_DSP_LR_DBDRC, OPTEK_DSP_SUB_3D, OPTEK_DSP_SUB_EBASS)), 8);
	//pbuffer += size;
	pbuffer += 1024;
	optek_dsp_setting_init(optek_dsp_sub, (OPTEK_DSP_RXs *)dsp_setting);
	//printf_error("2-pbuffer : 0x%x, %d\r\n", pbuffer, size);
	#endif

	if (pbuffer > (dsp_set_buffer + sizeof(dsp_set_buffer)))
	{
		printf_error("audio dapp open error!\r\n");
	}
	#endif
	#endif

    audio_optek_dapp_set_volume(audio_vari.volume);
    #endif
}

void audio_optek_dapp_set_channel(U8 channel)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_OPTEK_DAPP_FUNCTION_ENABLE

    #endif
}

void audio_optek_dapp_set_volume(U8 volume)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_OPTEK_DAPP_FUNCTION_ENABLE
	int value;

	DBG_Printf("volume is %d\n\r", volume);

	if (volume)
	{
		if (global_vari.audio_source == SOURCE_AUX)
		{
			value = (240+650-60) - volume*650/VOLUME_MAX;
		}
		else
		{
			value = (240+650) - volume*650/VOLUME_MAX;
		}
	}
	else
	{
		value = 240 + 990;
	}
	
	#ifdef DSP_HIFI_LIB_OLD
	optek_dapp_set_vol(0, value);
	optek_dapp_set_vol(1, value);
	#else
	#ifndef DSPEQ_SETTING_ENABLE
	optek_dapp_set_vol(optek_dapp_strero, 0, value);
	optek_dapp_set_vol(optek_dapp_strero, 1, value);
	#else
	#ifdef DSPEQ_TUNING_LR_CHANNEL
	optek_dapp_set_vol(optek_dsp_lr, 0, value);
	optek_dapp_set_vol(optek_dsp_lr, 1, value);
	#endif

	#ifdef DSPEQ_TUNING_CENTER_CHANNEL
	optek_dapp_set_vol(optek_dsp_center, 0, value);
	optek_dapp_set_vol(optek_dsp_center, 1, value);
	#endif

	#ifdef DSPEQ_TUNING_SUB_CHANNEL
	optek_dapp_set_vol(optek_dsp_sub, 0, value);
	optek_dapp_set_vol(optek_dsp_sub, 1, value);
	#endif

	#endif
	#endif

	DBG_Printf("set volume finish:%d db\n\r", 24 - value/10);
    #endif
}

void audio_optek_dapp_set_speaker(U8 speaker, U8 attenuation)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_OPTEK_DAPP_FUNCTION_ENABLE

    #endif
}

void audio_optek_dapp_set_bass_treble(U8 bass, U8 treble)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_OPTEK_DAPP_FUNCTION_ENABLE

	#ifdef DSP_HIFI_LIB_OLD
	optek_dapp_Appoint_eqData_update(BASS_INDEX, bass_table[bass]);
	optek_dapp_Appoint_eqData_update(TREBLE_INDEX, treble_table[treble]);
	#else
	#ifndef DSPEQ_SETTING_ENABLE
	optek_dapp_Appoint_eqData_update(optek_dapp_strero, BASS_INDEX, bass_table[bass]);
	optek_dapp_Appoint_eqData_update(optek_dapp_strero, TREBLE_INDEX, treble_table[treble]);
	#else
	#ifdef DSPEQ_TUNING_LR_CHANNEL
	optek_dapp_Appoint_eqData_update(optek_dsp_lr, BASS_INDEX, bass_table[bass]);
	optek_dapp_Appoint_eqData_update(optek_dsp_lr, TREBLE_INDEX, treble_table[treble]);
	#endif

	#ifdef DSPEQ_TUNING_CENTER_CHANNEL
	//optek_dapp_Appoint_eqData_update(optek_dsp_center, BASS_INDEX, bass_table[bass]);
	optek_dapp_Appoint_eqData_update(optek_dsp_center, TREBLE_INDEX, treble_table[treble]);
	#endif

	#ifdef DSPEQ_TUNING_SUB_CHANNEL
	optek_dapp_Appoint_eqData_update(optek_dsp_sub, BASS_INDEX, bass_table[bass]);
	//optek_dapp_Appoint_eqData_update(optek_dsp_sub, TREBLE_INDEX, treble_table[treble]);
	#endif

	#endif
	#endif
    #endif
}

void audio_optek_dapp_set_mute(U8 con)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_OPTEK_DAPP_FUNCTION_ENABLE
    audio_vari.mute = con;
	if(con)
	{
		audio_optek_dapp_set_volume(0);
	}
	else
	{
		audio_optek_dapp_set_volume(audio_vari.volume);
	}
    #endif
}

void audio_optek_dapp_set_AM_mode(U8 con, U32 kFreq)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_OPTEK_DAPP_FUNCTION_ENABLE

    #endif
}

void audio_optek_dapp_set_eq_mode(U8 mode)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_OPTEK_DAPP_FUNCTION_ENABLE
	int i;
	int (*table)[5];
	int size;

	switch(mode)
	{
		case EQ_FLAT:
			table = eq_flat_table;
			break;
			
		case EQ_CLASSIC:
			table = eq_classic_table;
			break;
			
		case EQ_ROCK:
			table = eq_rock_table;
			break;
			
		case EQ_JAZZ:
			table = eq_jazz_table;
			break;
			
		case EQ_POP:
			table = eq_pop_table;
			break;
			
		default:
			table = eq_flat_table;
			DBG_Printf("eq mode set error\n");
			break;
	}

	size = sizeof(eq_flat_table)/sizeof(eq_flat_table[0]) + EQ_MODE_INDEX;
	
	for (i=EQ_MODE_INDEX; i<size; i++)
	{
		#ifdef DSP_HIFI_LIB_OLD
		optek_dapp_Appoint_eqData_update(i, table);
		#else
		#ifndef DSPEQ_SETTING_ENABLE
		optek_dapp_Appoint_eqData_update(optek_dapp_strero, i, table);
		#else
		#ifdef DSPEQ_TUNING_LR_CHANNEL
		optek_dapp_Appoint_eqData_update(optek_dsp_lr, i, table);
		#endif

		#ifdef DSPEQ_TUNING_CENTER_CHANNEL
		optek_dapp_Appoint_eqData_update(optek_dsp_center, i, table);
		#endif

		#ifdef DSPEQ_TUNING_SUB_CHANNEL
		optek_dapp_Appoint_eqData_update(optek_dsp_sub, i, table);
		#endif

		#endif
		#endif
		table++;
	}
    #endif
}

#endif
