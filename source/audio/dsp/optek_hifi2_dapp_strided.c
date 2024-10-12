/*
	Optek DAPP Digital Audio Post Processing
		Copyright by Optek in May 24, 2016
*/

#include "os_config.h"

#include "common.h"

#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "codec.h"

#ifndef DSP_HIFI_LIB_OLD
#include "optek_hifi2_dapp_strided.h"

#include "peak_level_meter.h"

#include "optek_hifi2_bqriir.h"
#include "optek_hifi2_vol.h"
#include "optek_hifi2_drc_vol.h"
#include "optek_hifi2_drc.h"
#include "optek_hifi2_mixer.h"
#include "optek_hifi2_3D.h"
#include "optek_vol_table.h"
#include "optek_hifi2_ebass.h"


//#define MEM_ALIGN(mem, align)       (uint32_t)(((uintptr_t)mem + (uintptr_t)align-1) & (~((uintptr_t)align-1)))
#define MEM_ALIGN(mem, align)       (uint8_t*)(((uintptr_t)mem + (uintptr_t)align-1) & (~((uintptr_t)align-1)))
//#define MEM_ALIGN(val, align) (((val + 7)>>3)<<3)

#define OPTEK_DAPP_DRC_MASK_BIT           0x03
#define OPTEK_DRC_NORMAL                  0x0	
#define OPTEK_DRC_EQ                      0x1
#define OPTEK_DRC_MULTI_BAND              0x2

#define OPTEK_DAPP_3D_CROSSOVER_MASK_BIT  0x04
#define OPTEK_DAPP_EBASS_MASK_BIT         0x08
#define OPTEK_DAPP_DC_BLOCK_MASK_BIT      0x10


#define OPTEK_DAPP_MAX_SCRATCH_SAMPLES    0x100

int optek_dapp_scrach[OPTEK_DAPP_MAX_SCRATCH_SAMPLES];

#ifndef DSPEQ_SETTING_ENABLE
U8 dsp_set_buffer[1024];
OPTEK_DAPPs *optek_dapp_strero;
#else
U8 dsp_set_buffer[1024*3];
OPTEK_DAPPs *optek_dsp_lr;
OPTEK_DAPPs *optek_dsp_center;
OPTEK_DAPPs *optek_dsp_sub;
#endif

#define OPTEK_DC_BLOCK_MAX_BIQUAD 1
const int dc_block_coef_biquad[OPTEK_DC_BLOCK_MAX_BIQUAD*6] = {
	0x40000000,	0xc0000000,	0x00000000,	0xc020c49c,	0x00000000
};

#define OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER	2
const int coef_drc_hpf_default[OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER*6] = {
	//crossover HPF part, 100Hz/44.1kHz
	0x3f5bc45a, 0x8148774d, 0x3f5bc45a, 0x814a1cc1, 0x3eb92e28,
	0x3f5bc45a, 0x8148774d, 0x3f5bc45a, 0x814a1cc1, 0x3eb92e28,
};

const int coef_drc_lpf_default[(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER)*6] = {
	//crossover LPF part, 100Hz/44.1kHz
	0x0000d2ba, 0x0001a575, 0x0000d2ba, 0x814a1cc1, 0x3eb92e28,
	0x0000d2ba, 0x0001a575, 0x0000d2ba, 0x814a1cc1, 0x3eb92e28,
};

void optek_dapp_init (void)
{
	DBG_Printf ("Optek dsp dapp:%s\n\r", optek_dsp_dapp_version_get());
	DBG_Printf ("Buid time:%s %s\n\r", optek_dsp_dapp_time_get(), optek_dsp_dapp_data_get());
}

int optek_dapp_alloc(uint32_t main_eq_cnt, uint32_t dc_block_enable, uint32_t drc_enable, uint32_t multiband_drc_enable, uint32_t _3d_enable, uint32_t ebass_enable)
{
	int size = 0;

	size += MEM_ALIGN(sizeof(OPTEK_DAPPs), 8);
	size += optek_bqriir_alloc(main_eq_cnt);
	if (dc_block_enable)
		size += optek_bqriir_alloc(OPTEK_DC_BLOCK_MAX_BIQUAD);

	size += optek_drc_vol_alloc();
	if (multiband_drc_enable)
		size += optek_drc_vol_alloc() + optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER)*2;

	return size;
}

void *optek_dapp_open (uint8_t *buffer, uint32_t main_eq_cnt, const int *main_eq_coef, uint32_t dc_block_enable, uint32_t drc_enable, uint32_t multiband_drc_enable, uint32_t _3d_enable, uint32_t ebass_enable)
{
	int n_mems;
	void *objmem;
	OPTEK_DAPPs *optek_dapp;

	int size = 0;
	n_mems = MEM_ALIGN(sizeof(OPTEK_DAPPs), 8);
	objmem = &buffer[size];
	size += n_mems;
	optek_dapp = (OPTEK_DAPPs *)objmem;
	memset(optek_dapp, 0, sizeof(*optek_dapp));

	n_mems = optek_bqriir_alloc(main_eq_cnt);
	objmem = &buffer[size];
	size += n_mems;
	optek_dapp->biquad = optek_bqriir_open(objmem, main_eq_cnt, main_eq_coef);

	if (dc_block_enable)
	{
		optek_dapp->dapp_func_enable_mask = OPTEK_DAPP_DC_BLOCK_MASK_BIT;
		n_mems = optek_bqriir_alloc(OPTEK_DC_BLOCK_MAX_BIQUAD);
		objmem = &buffer[size];
		size += n_mems;

		optek_dapp->dc_block_biquad = optek_bqriir_open(objmem, OPTEK_DC_BLOCK_MAX_BIQUAD,dc_block_coef_biquad);
	}

	n_mems = optek_drc_vol_alloc();
	objmem = &buffer[size];
	size += n_mems;
	optek_dapp->drc_vol =  optek_drc_vol_open (objmem, 0x08000000, 0x08000000, OPTEK_DRC_ATTACK_RATE_0P2DB, OPTEK_DRC_RELEASE_RATE_0P2DB, 
					OPTEK_DRC_LEV_DOWN_RATE_0P01DB, 0x0, DAPP_1MS_SAMPLES_44_1kHz);			//clipping enable 	
	optek_drc_vol_open_vol (optek_dapp->drc_vol, 0x08000000, 0x08000000, 0x08000000, OPTEK_VOL_UP_RATE_0P2DB,
					OPTEK_VOL_DOWN_RATE_0P2DB, 1, 0);	
	optek_drc_vol_set_mute_off(optek_dapp->drc_vol);

	if (drc_enable)
	{
		optek_dapp->dapp_func_enable_mask |= OPTEK_DRC_EQ;

		if (multiband_drc_enable)
		{
			optek_dapp->dapp_func_enable_mask |= OPTEK_DRC_MULTI_BAND;

			n_mems = optek_drc_vol_alloc();
			objmem = &buffer[size];
			size += n_mems;
			optek_dapp->drc_vol_2 =  optek_drc_vol_open (objmem, 0x08000000, 0x08000000, OPTEK_DRC_ATTACK_RATE_0P2DB, OPTEK_DRC_RELEASE_RATE_0P2DB, 
							OPTEK_DRC_LEV_DOWN_RATE_0P001DB, 0x1,DAPP_1MS_SAMPLES_44_1kHz);			//clipping enable 
			optek_drc_vol_open_vol (optek_dapp->drc_vol_2, 0x08000000, 0x08000000, 0x08000000, OPTEK_VOL_UP_RATE_0P2DB,
							OPTEK_VOL_DOWN_RATE_0P2DB, 1, 0);	
			optek_drc_vol_set_mute_off(optek_dapp->drc_vol_2);

			n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
			objmem = &buffer[size];
			size += n_mems;

			optek_dapp->drc_hpf = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER, coef_drc_hpf_default);

			n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
			objmem = &buffer[size];
			size += n_mems;
			optek_dapp->drc_lpf = optek_bqriir_open(objmem, (OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER), coef_drc_lpf_default);
		}	
	}

#if 0//3d		
	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
	objmem = &buffer[size];
	size += n_mems;
	
	optek_dapp->_3D_hpf = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER,coef_3d_hpf);

	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
	objmem = &buffer[size];
	size += n_mems;
	
	optek_dapp->_3D_lpf = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER,coef_3d_lpf);
#endif

#if 0	
	//EBASS
	n_mems = optek_ebass_alloc();
	objmem = &buffer[size];
	size += n_mems;	

	optek_dapp->ebass = optek_ebass_open(objmem,NLD_RISE_TIME,NLD_FALL_TIME);


#ifdef OPTEK_EBASS_CUTOFF
	n_mems = optek_bqriir_alloc(OPTEK_EBASS_MAX_CUTOFF);
	objmem = &buffer[size];
	size += n_mems;	
	optek_dapp->ebass_cutoff = optek_bqriir_open(objmem, OPTEK_EBASS_MAX_CUTOFF,coef_ebass_cutoff);
#endif	//OPTEK_EBASS_CUTOFF

#if 0
	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
	objmem = &buffer[size];
	size += n_mems;
	
	optek_dapp->ebass_hpf = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER,coef_drc_hpf);
	
	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_EBASS_LPF);
	objmem = &buffer[size];
	size += n_mems;
	
	optek_dapp->ebass_lpf = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_EBASS_LPF,coef_ebass_lpf);
#endif

	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_EBASS_BAND);
	objmem = &buffer[size];
	size += n_mems;
	
	optek_dapp->ebass_band = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_EBASS_BAND,coef_ebass_band);


#ifdef OPTEK_EBASS_DELAY
	memset(optek_dapp_ebass_delay,0,sizeof(optek_dapp_ebass_delay));
#endif

#endif

	return (void *)optek_dapp;
}

/*
* void *optek_dapp		*dsp_handle
* int *x 				input buffer point(need used this dsp first data's addr)
* int *r 				output buffer point(if (in_stride > out_stride) need mallac new buffer)
* int N					input buffer size, 32bit size
* int in_stride			input buffer, one sample include x 32bit(channel) data
* int out_stride		output buffer, one sample include x 32bit(channel) data, support setting out_stride <= in_stride
* */

//void optek_dapp_stereo_process (void *optek_dapp, int *x, int N) __INTERNAL_RAM_TEXT;
void optek_dapp_stereo_process (void *optek_dapp, int *x, int *r, int N, int in_stride, int out_stride)
{
	int i, k, samples, index;
	void *mx1, *mx2;

	OPTEK_DAPPs *dapp_obj = optek_dapp;

	//prescale
#if 0
	optek_hifi2_prescale_24b(x, x, N);
#else
	int *pIn = x;
	int *pOut = r;

	for (i = 0; i < N; i += 2)
	{
		*pOut = *pIn >> DAPP_PRE_POST_SCALE;
		*(pOut+1) = *(pIn+1) >> DAPP_PRE_POST_SCALE;
		// *pOut = *pIn >> 1;
		// *(pOut+1) = *(pIn+1) >> 1;


		pIn += in_stride;
		pOut += out_stride;
	}
#endif

	//Biquad
	//optek_bqdiir_stereo_32b_process_strided (dapp_obj->biquad, r, r, N, out_stride, out_stride);
	optek_bqdiir_stereo_32b_process_strided (dapp_obj->biquad, x, r, N, in_stride, out_stride);
	//drc
#if 0	
	if ((dapp_obj->dapp_func_enable_mask & OPTEK_DRC_MULTI_BAND) == OPTEK_DRC_MULTI_BAND)
	{
		if ( (dapp_obj->dapp_func & OPTEK_DAPP_DRC_MASK_BIT) == OPTEK_DRC_EQ)
		{
			//eq drc
			optek_bqdiir_stereo_32b_process_strided(dapp_obj->drc_hpf, r, r, N, in_stride, out_stride);
			optek_drc_soft_vol_stereo_24b_process_strided (dapp_obj->drc_vol, r, r, N, in_stride, out_stride);
		}
		else if ( (dapp_obj->dapp_func & OPTEK_DAPP_DRC_MASK_BIT) == OPTEK_DRC_MULTI_BAND)
		{
			//multi band drc
			i = N;
			index = 0;
			
			while (i > 0)
			{
				if (i > OPTEK_DAPP_MAX_SCRATCH_SAMPLES)
				{
					samples = OPTEK_DAPP_MAX_SCRATCH_SAMPLES;
				}
				else
				{
					samples = i;
				}
				
				//3D crossover filter is enabled
				optek_bqdiir_stereo_32b_process_strided(dapp_obj->drc_lpf, &r[index], optek_dapp_scrach, samples, in_stride, out_stride);
				optek_bqdiir_stereo_32b_process_strided(dapp_obj->drc_hpf, &r[index], &r[index], samples, in_stride, out_stride);
			
				optek_drc_soft_vol_stereo_24b_process_strided (dapp_obj->drc_vol, &r[index], &r[index], samples);
				 (dapp_obj->drc_vol_2, optek_dapp_scrach, optek_dapp_scrach, samples);

				optek_hifi2_plus_24b(&r[index], &r[index], optek_dapp_scrach, samples);
				
				index += samples;
				i -= samples;
			}
		}
		else 
		{
			//normal drc
			optek_drc_soft_vol_stereo_24b_process_strided (dapp_obj->drc_vol, x, x, N);		
		}
	}
	else
#endif	
	{
		//normal drc
		optek_drc_soft_vol_stereo_24b_process_strided (dapp_obj->drc_vol, r, r, N, out_stride, out_stride);		
	}
}

void optek_dapp_mono_process (void *optek_dapp, int *x, int *r, int N, int in_stride, int out_stride)
{
	int i, k, samples, index;
	void *mx1, *mx2;

	OPTEK_DAPPs *dapp_obj = optek_dapp;

	//prescale
#if 0
	optek_hifi2_prescale_24b(x, x, N);
#else
	int *pIn = x;
	int *pOut = r;

	for (i = 0; i < N; i += 1)
	{
		*pOut = *pIn >> DAPP_PRE_POST_SCALE;
		//*pOut = *pIn >> 1;

		pIn += in_stride;
		pOut += out_stride;
	}
#endif

	//Biquad
	optek_bqdiir_mono_32b_process_strided (dapp_obj->biquad, r, r, N, out_stride, out_stride);
	//optek_bqdiir_mono_32b_process_strided (dapp_obj->biquad, x, r, N, in_stride, out_stride);

	//drc
#if 0	
	if ((dapp_obj->dapp_func_enable_mask & OPTEK_DRC_MULTI_BAND) == OPTEK_DRC_MULTI_BAND)
	{
		if ( (dapp_obj->dapp_func & OPTEK_DAPP_DRC_MASK_BIT) == OPTEK_DRC_EQ)
		{
			//eq drc
			optek_bqdiir_stereo_32b_process_strided(dapp_obj->drc_hpf, r, r, N, in_stride, out_stride);
			optek_drc_soft_vol_stereo_24b_process_strided (dapp_obj->drc_vol, r, r, N, in_stride, out_stride);
		}
		else if ( (dapp_obj->dapp_func & OPTEK_DAPP_DRC_MASK_BIT) == OPTEK_DRC_MULTI_BAND)
		{
			//multi band drc
			i = N;
			index = 0;
			
			while (i > 0)
			{
				if (i > OPTEK_DAPP_MAX_SCRATCH_SAMPLES)
				{
					samples = OPTEK_DAPP_MAX_SCRATCH_SAMPLES;
				}
				else
				{
					samples = i;
				}
				
				//3D crossover filter is enabled
				optek_bqdiir_stereo_32b_process_strided(dapp_obj->drc_lpf, &r[index], optek_dapp_scrach, samples, in_stride, out_stride);
				optek_bqdiir_stereo_32b_process_strided(dapp_obj->drc_hpf, &r[index], &r[index], samples, in_stride, out_stride);
			
				optek_drc_soft_vol_stereo_24b_process_strided (dapp_obj->drc_vol, &r[index], &r[index], samples);
				optek_drc_soft_vol_stereo_24b_process_strided (dapp_obj->drc_vol_2, optek_dapp_scrach, optek_dapp_scrach, samples);

				optek_hifi2_plus_24b(&r[index], &r[index], optek_dapp_scrach, samples);
				
				index += samples;
				i -= samples;
			}
		}
		else 
		{
			//normal drc
			optek_drc_soft_vol_stereo_24b_process_strided (dapp_obj->drc_vol, x, x, N);		
		}
	}
	else
#endif	
	{
		//normal drc
		optek_drc_soft_vol_mono_24b_process_strided (dapp_obj->drc_vol, r, r, N, out_stride, out_stride);		
	}
}

#if 0
void optek_dapp_16b_in_process (short *x, int chans, int N)
{
	#if (IIS_INPUT_CHANNEL_BIT_NUM == 16)
	optek_hifi2_16b_to_24b(x, x, N);
	#endif
	
	optek_dapp_process (x, chans, N);

	#if (IIS_OUTPUT_CHANNEL_BIT_NUM == 16)
	optek_hifi2_24b_to_16b(x, x, N);
	#endif	
}
#endif

void * optek_dapp_get_lpf_handle(void *optek_dapp)
{
	return (((OPTEK_DAPPs *)optek_dapp)->drc_lpf);
}

int optek_dapp_10eqData_update(void *optek_dapp, int *coefData)
{
	int ret;
	
	TX_INTERRUPT_SAVE_AREA;
	TX_DISABLE;
	
	ret = optek_bqriir_coeffs_update_all(((OPTEK_DAPPs *)optek_dapp)->biquad, coefData);
	
	
	TX_RESTORE;

	return ret;
}
int optek_dapp_Appoint_eqData_update(void *optek_dapp, int eq_num, int *coefData)
{
	int ret;
	TX_INTERRUPT_SAVE_AREA;
	TX_DISABLE;
	
	ret = optek_bqriir_coeffs_update(((OPTEK_DAPPs *)optek_dapp)->biquad, eq_num, coefData);
	
	
	TX_RESTORE;

	return ret;
}

void optek_dapp_set_Master_vol(void *optek_dapp, int drcNum, int value)
{
	if (drcNum == 0)
	{
		optek_drc_vol_set_master_vol(((OPTEK_DAPPs *)optek_dapp)->drc_vol_2, optek_vol_vol_table[value]);	
	}
	else
	{
		optek_drc_vol_set_master_vol(((OPTEK_DAPPs *)optek_dapp)->drc_vol, optek_vol_vol_table[value]);	
	}
}

void optek_dapp_set_vol(void *optek_dapp, int drcNum, int value)
{
	if (drcNum == 0)
	{
		optek_drc_vol_set_vol(((OPTEK_DAPPs *)optek_dapp)->drc_vol_2, optek_vol_vol_table[value], optek_vol_vol_table[value]);
	}
	else
	{
		optek_drc_vol_set_vol(((OPTEK_DAPPs *)optek_dapp)->drc_vol, optek_vol_vol_table[value], optek_vol_vol_table[value]);
	}
}

int optek_dapp_set_drc(void *optek_dapp, int drcNum, int attack, int release, int attack_rate, int release_rate,
					 uint8_t clipping, uint8_t update_samples)
{
	
	attack = optek_vol_vol_table[attack];
	release = optek_vol_vol_table[release];

	attack_rate = attack_rate*10;
	attack_rate = optek_vol_down_rate[attack_rate];

	release_rate = release_rate*10;
	release_rate = optek_vol_up_rate[release_rate];

	if (drcNum == 0)
	{
		if ((((OPTEK_DAPPs *)optek_dapp)->dapp_func_enable_mask & OPTEK_DRC_MULTI_BAND) == OPTEK_DRC_MULTI_BAND)
		{
			optek_drc_vol_set_drc(((OPTEK_DAPPs *)optek_dapp)->drc_vol_2, attack, release, attack_rate, release_rate,
						OPTEK_DRC_LEV_DOWN_RATE_0P004DB, clipping,DAPP_1MS_SAMPLES_44_1kHz,600);
		}
	}
	else
	{
		if ((((OPTEK_DAPPs *)optek_dapp)->dapp_func_enable_mask & OPTEK_DRC_EQ) == OPTEK_DRC_EQ)
		{
			optek_drc_vol_set_drc(((OPTEK_DAPPs *)optek_dapp)->drc_vol, attack, release, attack_rate, release_rate, 
						OPTEK_DRC_LEV_DOWN_RATE_0P04DB, clipping,DAPP_1MS_SAMPLES_44_1kHz,600);
		}
	}
	return 1;
}

void optek_dapp_set_DRC_mode(void *optek_dapp, int pam)
{
	((OPTEK_DAPPs *)optek_dapp)->dapp_func &= 0xFC;
	((OPTEK_DAPPs *)optek_dapp)->dapp_func |= pam;

	DBG_Printf("optek_dapp->dapp_func:0x%x\n",((OPTEK_DAPPs *)optek_dapp)->dapp_func);
}

int optek_dapp_DBDRC_Filter_update(void *optek_dapp, int type,int *coefData)
{
	int ret;

	if ((((OPTEK_DAPPs *)optek_dapp)->dapp_func_enable_mask & OPTEK_DRC_MULTI_BAND) == 0) return 0;

	if (((OPTEK_DAPPs *)optek_dapp)->dapp_func & OPTEK_DAPP_EBASS_MASK_BIT) return 0;
	
	TX_INTERRUPT_SAVE_AREA;
	TX_DISABLE;

	if (type == 0)
	{
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		
		ret = optek_bqriir_coeffs_update(((OPTEK_DAPPs *)optek_dapp)->drc_hpf, 0, coefData);
		ret = optek_bqriir_coeffs_update(((OPTEK_DAPPs *)optek_dapp)->drc_hpf, 1, coefData);
	}
	else{

		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		
		ret = optek_bqriir_coeffs_update(((OPTEK_DAPPs *)optek_dapp)->drc_lpf, 0, coefData);
		ret = optek_bqriir_coeffs_update(((OPTEK_DAPPs *)optek_dapp)->drc_lpf, 1, coefData);
	}
	
	
	TX_RESTORE;

	return ret;
}

void optek_dapp_16b_in_process (short *x, int chans, int N)
{
	#if (IIS_INPUT_CHANNEL_BIT_NUM == 16)
	optek_hifi2_16b_to_24b(x, x, N);
	#endif
	
	#ifndef DSPEQ_SETTING_ENABLE
	optek_dapp_stereo_process (optek_dapp_strero, x, x, N, 2, 2);
	#else

	#ifdef DSPEQ_TUNING_LR_CHANNEL
	optek_dapp_stereo_process (optek_dsp_lr, x, x, N, 2, 2);
	#endif

	#ifdef DSPEQ_TUNING_CENTER_CHANNEL
	//optek_dapp_stereo_process (optek_dsp_center, x, x, N, 2, 2); //center channel mono
	#endif

	#ifdef DSPEQ_TUNING_SUB_CHANNEL
	//optek_dapp_stereo_process (optek_dsp_sub, x, x, N, 2, 2); //sub channel mono
	#endif

	#endif

	#if (IIS_OUTPUT_CHANNEL_BIT_NUM == 16)
	optek_hifi2_24b_to_16b(x, x, N);
	#endif	
}
#endif
