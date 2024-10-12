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
#ifdef DSP_HIFI_LIB_OLD
#include "optek_hifi2_dapp.h"

#include "peak_level_meter.h"
//#include "audio_dec.h"

#include "optek_hifi2_bqriir.h"
#include "optek_hifi2_vol.h"
#include "optek_hifi2_drc_vol.h"
#include "optek_hifi2_drc.h"
#include "optek_hifi2_mixer.h"
#include "optek_hifi2_3D.h"
#include "optek_vol_table.h"
#include "optek_hifi2_ebass.h"



#define OPTEK_DAPP_MAX_BIQUAD	10
#define OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER	2

#define OPTEK_EBASS_MAX_CUTOFF	2



#define OPTEK_DC_BLOCK_MAX_BIQUAD 1
const int dc_block_coef_biquad[OPTEK_DC_BLOCK_MAX_BIQUAD*6] = {
	//Biquad 1
	0x40000000,
	0x40000000,
	0xc0000000,
	0x00000000,
	0xc020c49c,
	0x00000000

};

/*
* 	//hp filter:50Hz
*	0x40000000,
*	0x3FADAD3B,   
*	0x80A4A58A,   
*	0x3FADAD3B,   
*	0x80A50F6F,   
*	0x3F5BC45A,
* 
* */

const int coef_biquad[OPTEK_DAPP_MAX_BIQUAD*6] = {
	//Biquad 1
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,

	//Biquad 2
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,

	//Biquad 3
	0x40000000,
	0x40000000,   
	0x0,
	0x0,
	0x0,
	0x0,

	//Biquad 4
	0x40000000,
	0x40000000,   
	0x0,
	0x0,
	0x0,   
	0x0,

	//Biquad 5
	0x40000000,
	0x40000000,   
	0x0,
	0x0,
	0x0,
	0x0,

	//Biquad 6
	0x40000000,
	0x40000000,   
	0x0,
	0x0,
	0x0,   
	0x0,

	//Biquad 7
	0x40000000,
	0x40000000,   
	0x0,
	0x0,
	0x0,
	0x0,

	//Biquad 8
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,

	//Biquad 9
	0x40000000,
	0x40000000,
	0x0,   
	0x0,   
	0x0,
	0x0,

	//Biquad 10		
	0x40000000,
	0x40000000,
	0x0,   
	0x0,   
	0x0,   
	0x0,
};

/*
* 	//crossover HPF part, 100Hz/44.1kHz
*	0x40000000,
*	0x3f5bc45a,   
*	0x8148774d,   
*	0x3f5bc45a,   
*	0x814a1cc1,   
*	0x3eb92e28,
* */
const int coef_drc_hpf[OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER*6] = {

	//Biquad 1
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,

	//Biquad 2
	0x40000000,
	0x40000000,   
	0x0,
	0x0,
	0x0,
	0x0

};

/*
* 	//crossover LPF part, 100Hz/44.1kHz	
*	0x40000000,
*	0x0000d2ba,   
*	0x0001a575,   
*	0x0000d2ba,   
*	0x814a1cc1,   
*	0x3eb92e28,
* 
* 	//used for subw lpf, 200Hz
*	0x40000000,
*	0x0003428b,
*	0x00068517,
*	0x0003428b,
*	0x829428f4,
*	0x3d78e13c,
* */

#ifdef OPTEK_NLD_INTEGRATOR
const int coef_drc_lpf[(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER+1)*6] = {
#else
const int coef_drc_lpf[(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER)*6] = {
#endif	//OPTEK_NLD_INTEGRATOR

#ifdef OPTEK_NLD_INTEGRATOR
	//should not be changed, dc block
	0x40000000,
	0x40000000,
	0xc0000000,
	0x0,   
	0xc051eb85,
	0x0
#endif //OPTEK_NLD_INTEGRATOR

	//Biquad 1, Also used for subw lpf, 200Hz
	0x40000000,
	0x40000000,   
	0x0,
	0x0,
	0x0,
	0x0,
	
	//Biquad 2
	0x40000000,
	0x40000000,   
	0x0,
	0x0,
	0x0,
	0x0

};

#define OPTEK_DAPP_MAX_EBASS_BAND	5
/*
* 	//crossover LPF part, 250Hz/44.1kHz	
*	0x40000000,
*	0x00051187,
*	0x000a230d,
*	0x00051187,
*	0x833923e7,
*	0x3cdb2233,
* 	//crossover HPF part, 100Hz/44.1kHz	
*	0x40000000,
*	0x3f5bc45a,
*	0x8148774d,
*	0x3f5bc45a,
*	0x814a1cc1,   
*	0x3eb92e28,
* */
const int coef_ebass_band[OPTEK_DAPP_MAX_EBASS_BAND*6] = {

	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,

	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,
	
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,

	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,

	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0

};

/*
* 	//ebass dc block
*	0x40000000,
*	0x40000000,
*	0xc0000000,
*	0x0,   
*	0xc051eb85,
*	0x0,
* */
const int coef_ebass_dc_block[1*6] = {

	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0

};

/*
* 	//ebass cutoff, 20Hz/44100
*	0x40000000,
*	0x3fee1ec0,   
*	0x8023c280,
*	0x3fee1ec0,   
*	0x8023d383,   
*	0x3fdc4e83,
* 
* 	0x40000000,
*	0x3fd4f28c,   
*	0x80561ae8,   
*	0x3fd4f28c,   
*	0x80562be4,   
*	0x3fa9f614
* */

const int coef_ebass_cutoff[OPTEK_EBASS_MAX_CUTOFF*6] = {

	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,
	
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0

};


/*
* //100Hz/44.1kHz
* Low pass filer
* 
* ans =
* 
* 0000d2ba   0001a575   0000d2ba   40000000   814a1cc1   3eb92e28
* 
* High pass filer
* 
* ans =
* 
* 3f5bc45a   8148774d   3f5bc45a   40000000   814a1cc1   3eb92e28
* 
* //120Hz/44.1kHz
* Low pass filer
* 
* ans =
* 
* 00012ed7   00025daf   00012ed7   40000000   818c2109   3e789a55
* 
* High pass filer
* 
* ans =
* 
* 3f3b1e53   8189c35a   3f3b1e53   40000000   818c2109   3e789a55
* 
* //300Hz for mobile phone speaker
* Low pass filer
* 
* ans =
* 
* 00074339   000e8672   00074339   40000000   83de14ec   3c3ef7f7
* 
* High pass filer
* 
* ans =
* 
* 3e1838c3   83cf8e7a   3e1838c3   40000000   83de14ec   3c3ef7f7
* 
* //650Hz for mobile phone speaker
* Low pass filer
* 
* ans =
* 
* 0020f317   0041e62e   0020f317   40000000   885f04a3   3824c7b9
* 
* High pass filer
* 
* ans =
* 
* 3bf170c6   881d1e75   3bf170c6   40000000   885f04a3   3824c7b9
* 
* */



const int coef_3d_hpf[OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER*6] = {
	
	//Biquad 1
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,
	
	//Biquad 2
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0

};

const int coef_3d_lpf[OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER*6] = {

	//Biquad 1
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,
	
	//Biquad 2
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0

};


#ifdef OPTEK_DSP_MX1
#define OPTEK_DAPP_MAX_MIX1_BAND	2

const int coef_mix1_band[OPTEK_DAPP_MAX_MIX1_BAND*6] = {

	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,
	
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,

	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,
	
	0x40000000,
	0x40000000,
	0x0,
	0x0,
	0x0,
	0x0,

};
#endif


const int msbc_dsp_coef[MSBC_MAX_BIQUAD*6] = {
	0x40000000,
	0x063f9666,
	0x0c7f2ccc,
	0x063f9666,
	0xc3a90445,
	0x15555555,

	0x40000000,
	0x063f9666,
	0x0c7f2ccc,
	0x063f9666,
	0xc3a90445,
	0x15555555

};



/*
* //added by Luo on Nov. 2th,2020
* const int sr_24k_dsp_coef[SR_24K_MAX_BIQUAD*6] = {
* 	//LPF:sample rate is 44.1kHz,CF:10kHz
* 	0x40000000,0x10169c1d,0x202d383a,0x10169c1d,0xf50a618e,0x0b500ee7,
* 	0x40000000,0x10169c1d,0x202d383a,0x10169c1d,0xf50a618e,0x0b500ee7,
* };
* */



#ifdef BT_HFP_CVSD_FUNC_ENABLE
//added by Luo on Nov. 2th,2020
const int cvsd_dsp_coef[CVSD_MAX_BIQUAD*6] = {
/*		low pass 3400, -76db at 4000*/
	0x40000000, 0x2dc18b5d, 0xb0dbef3c, 0x2dc18b5d, 0x8f3326b0, 0x3d2bdf46,
	0x40000000, 0x2ad5ee74, 0xb6c92c54, 0x2ad5ee74, 0x94e47f6d, 0x379089cf,
	0x40000000, 0x26e38de3, 0xbf5e33a0, 0x26e38de3, 0x9b99ec39, 0x318b632c,
	0x40000000, 0x21a9e3c8, 0xcb1dcd66, 0x21a9e3c8, 0xa3d17bf1, 0x2aa01905,
	0x40000000, 0x1b01be76, 0xda573031, 0x1b01be76, 0xadd8267f, 0x2282869f,
	0x40000000, 0x1332d873, 0xec5b124e, 0x1332d873, 0xb9553cf3, 0x196b8641,
	0x40000000, 0x0b842d1d, 0xfe28ea9d, 0x0b842d1d, 0xc47fefc5, 0x10b15513,
	0x40000000, 0x0677e4ee, 0x09e21202, 0x0677e4ee, 0xcbcba3bd, 0x0b063821
};
#endif //BT_HFP_CVSD_FUNC_ENABLE


#define DAPP_MAX_CHAN	1

#define OPTEK_DAPP_DRC_MASK_BIT           0x03
#define OPTEK_DRC_NORMAL                  0x0	
#define OPTEK_DRC_EQ                      0x1
#define OPTEK_DRC_MULTI_BAND              0x2

#define OPTEK_DAPP_3D_CROSSOVER_MASK_BIT  0x04
#define OPTEK_DAPP_EBASS_MASK_BIT         0x08


#define OPTEK_DAPP_MAX_SCRATCH_SAMPLES    0x100

int optek_dapp_scrach[OPTEK_DAPP_MAX_SCRATCH_SAMPLES];

#ifdef OPTEK_EBASS_DELAY
#define OPTEK_EBASS_DELAY_MAX 512							/*phase match for 70Hz cutoff 100Hz*/
int optek_dapp_ebass_delay[OPTEK_DAPP_MAX_SCRATCH_SAMPLES+OPTEK_EBASS_DELAY_MAX];
#endif	//OPTEK_EBASS_DELAY



#define DAPP_CHAN_INDEX			0

typedef struct
{
	void *biquad[DAPP_MAX_CHAN];
	//DRC
	void *drc_vol[DAPP_MAX_CHAN];
	void *drc_vol_2[DAPP_MAX_CHAN];			/*multi band drc*/
	void *drc_hpf[DAPP_MAX_CHAN];
	void *drc_lpf[DAPP_MAX_CHAN];
	//3D
	uint32_t _3D_factor[DAPP_MAX_CHAN];
	void *_3D_hpf[DAPP_MAX_CHAN];
	void *_3D_lpf[DAPP_MAX_CHAN];

	//VBASS
	void *ebass[DAPP_MAX_CHAN];
	uint32_t ebass_factor[DAPP_MAX_CHAN];

#ifdef OPTEK_EBASS_CUTOFF
	void *ebass_cutoff[DAPP_MAX_CHAN];
#endif
	void *ebass_band[DAPP_MAX_CHAN];

	//mixer
	uint32_t mx1_factor[DAPP_MAX_CHAN];
	uint32_t mx2_factor[DAPP_MAX_CHAN];

	void *mx1_biquad[DAPP_MAX_CHAN];

	void *dc_block_biquad[DAPP_MAX_CHAN];

	//setting
	uint8_t dapp_func[DAPP_MAX_CHAN];
} OPTEK_DAPPs __attribute__((packed));

OPTEK_DAPPs optek_dapp __attribute__ ((section (".d_dsp_bss")));

#ifdef OPTEK_EBASS_CUTOFF
#define OPTEK_DAPP_ALLOC_MAX    (2160)
#elif defined OPTEK_DSP_MX1
#define OPTEK_DAPP_ALLOC_MAX    (2444)
#else
#define OPTEK_DAPP_ALLOC_MAX    (2144+16+100+60)
#endif	//OPTEK_EBASS_CUTOFF

uint8_t optek_dapp_alloc_buf[OPTEK_DAPP_ALLOC_MAX] __attribute__ ((section (".d_dsp_bss")));


void optek_dapp_init (void)
{
	memset (&optek_dapp,0,sizeof(optek_dapp));

	DBG_Printf ("Optek dsp dapp:%s\n\r", optek_dsp_dapp_version_get());
	DBG_Printf ("Buid time:%s %s\n\r", optek_dsp_dapp_time_get(), optek_dsp_dapp_data_get());
}

void optek_dapp_open (void)
{
	int n_mems;
	void *objmem;

	int size = 0;
	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;

	optek_dapp.biquad[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_BIQUAD,coef_biquad);

	n_mems = optek_bqriir_alloc(OPTEK_DC_BLOCK_MAX_BIQUAD);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;

	optek_dapp.dc_block_biquad[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_DC_BLOCK_MAX_BIQUAD,dc_block_coef_biquad);

	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;

	optek_dapp.drc_hpf[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER,coef_drc_hpf);

#ifdef OPTEK_NLD_INTEGRATOR
	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER+1);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;

	optek_dapp.drc_lpf[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, (OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER+1),coef_drc_lpf);
#else
	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;
	optek_dapp.drc_lpf[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, (OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER),coef_drc_lpf);
#endif		//OPTEK_NLD_INTEGRATOR
		
	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;
	
	optek_dapp._3D_hpf[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER,coef_3d_hpf);

	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;
	
	optek_dapp._3D_lpf[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER,coef_3d_lpf);


	n_mems = optek_drc_vol_alloc();
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;

	optek_dapp.drc_vol[DAPP_CHAN_INDEX] =  optek_drc_vol_open (objmem, 0x50000000, 0x474cd1b8, OPTEK_DRC_ATTACK_RATE_0P2DB, OPTEK_DRC_RELEASE_RATE_0P2DB, 
					//OPTEK_DRC_LEV_DOWN_RATE_0P04DB, 0x0,DAPP_1MS_SAMPLES_44_1kHz);		//clipping disable 
					OPTEK_DRC_LEV_DOWN_RATE_0P01DB, 0x1,DAPP_1MS_SAMPLES_44_1kHz);			//clipping enable 
		
	optek_drc_vol_open_vol (optek_dapp.drc_vol[DAPP_CHAN_INDEX], 0x08000000, 0x04000000, 0x04000000, OPTEK_VOL_UP_RATE_0P2DB,
					 OPTEK_VOL_DOWN_RATE_0P2DB, 1, 0);	
		
	optek_drc_vol_set_mute_off(optek_dapp.drc_vol[DAPP_CHAN_INDEX]);
	
	n_mems = optek_drc_vol_alloc();
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;
	
	//optek_dapp.drc_vol_2[DAPP_CHAN_INDEX] =  optek_drc_vol_open (objmem, 0x50000000, 0x474cd1b8, OPTEK_DRC_ATTACK_RATE_0P2DB, OPTEK_DRC_RELEASE_RATE_0P2DB, 
	optek_dapp.drc_vol_2[DAPP_CHAN_INDEX] =  optek_drc_vol_open (objmem, 0x78000000, 0x74000000, OPTEK_DRC_ATTACK_RATE_0P2DB, OPTEK_DRC_RELEASE_RATE_0P2DB, 
		//OPTEK_DRC_LEV_DOWN_RATE_0P04DB, 0x0,DAPP_1MS_SAMPLES_44_1kHz);		//clipping disable 
		OPTEK_DRC_LEV_DOWN_RATE_0P001DB, 0x1,DAPP_1MS_SAMPLES_44_1kHz);			//clipping enable 
	
	optek_drc_vol_open_vol (optek_dapp.drc_vol_2[DAPP_CHAN_INDEX], 0x08000000, 0x04000000, 0x04000000, OPTEK_VOL_UP_RATE_0P2DB,
					 OPTEK_VOL_DOWN_RATE_0P2DB, 1, 0);	

	optek_drc_vol_set_mute_off(optek_dapp.drc_vol_2[DAPP_CHAN_INDEX]);
	
	
	//EBASS
	n_mems = optek_ebass_alloc();
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;	
#ifdef OPTEK_NLD_INTEGRATOR
	optek_dapp.ebass[DAPP_CHAN_INDEX] = optek_ebass_open(objmem,INTEGRATOR_FACTOR_48000,OPTEK_EBASS_MIN_FREQ_SAMPLES, OPTEK_EBASS_ATTACK_THESHOLD,OPTEK_EBASS_RELEASE_THESHOLD,
		OPTEK_EBASS_ATTACK_RATE,OPTEK_EBASS_RELEASE_RATE,OPTEK_DRC_LEV_DOWN_RATE_0P0001DB,DAPP_1MS_SAMPLES_44_1kHz);
#else
	optek_dapp.ebass[DAPP_CHAN_INDEX] = optek_ebass_open(objmem,NLD_RISE_TIME,NLD_FALL_TIME);
#endif	//OPTEK_NLD_INTEGRATOR


#ifdef OPTEK_EBASS_CUTOFF
	n_mems = optek_bqriir_alloc(OPTEK_EBASS_MAX_CUTOFF);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;	
	optek_dapp.ebass_cutoff[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_EBASS_MAX_CUTOFF,coef_ebass_cutoff);
#endif	//OPTEK_EBASS_CUTOFF

#if 0
	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;
	
	optek_dapp.ebass_hpf[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_BIQUAD_FOR_CROSSOVER,coef_drc_hpf);
	
	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_EBASS_LPF);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;
	
	optek_dapp.ebass_lpf[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_EBASS_LPF,coef_ebass_lpf);
#endif

	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_EBASS_BAND);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;
	
	optek_dapp.ebass_band[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_EBASS_BAND,coef_ebass_band);


#ifdef OPTEK_EBASS_DELAY
	memset(optek_dapp_ebass_delay,0,sizeof(optek_dapp_ebass_delay));
#endif

	if (size > OPTEK_DAPP_ALLOC_MAX)
		asm ("break 1,1");


#ifdef OPREK_DAPP_DEBUG

#if 0 //3D test
	//EQ normal, no 3d filter. no Enhanced BASS
	optek_dapp.dapp_func[DAPP_CHAN_INDEX] = OPTEK_DAPP_3D_CROSSOVER_MASK_BIT;
	//optek_dapp._3D_factor[DAPP_CHAN_INDEX] = 0x08000000;		//0db
	optek_dapp._3D_factor[DAPP_CHAN_INDEX] = 0x04000000;		//1/2
#endif

#if 1 //dual band drc test
	//EQ normal, no 3d filter. no Enhanced BASS
	optek_dapp.dapp_func[DAPP_CHAN_INDEX] = OPTEK_DAPP_EBASS_MASK_BIT; //OPTEK_DRC_MULTI_BAND;//OPTEK_DRC_EQ; //OPTEK_DRC_MULTI_BAND;


	//optek_dapp.ebass_factor[DAPP_CHAN_INDEX] = 0x00080000;			//0db
	optek_dapp.ebass_factor[DAPP_CHAN_INDEX] = 0x00200000;				//+12db


	//optek_drc_vol_set_vol (optek_dapp.drc_vol[DAPP_CHAN_INDEX], 0x04000000, 0x04000000);
	//optek_drc_vol_set_vol (optek_dapp.drc_vol_2[DAPP_CHAN_INDEX], 0x04000000, 0x04000000);

	optek_drc_vol_set_vol (optek_dapp.drc_vol[DAPP_CHAN_INDEX], 0x08000000, 0x08000000);
	optek_drc_vol_set_vol (optek_dapp.drc_vol_2[DAPP_CHAN_INDEX], 0x08000000, 0x08000000);


	optek_drc_vol_set_mute_off(optek_dapp.drc_vol[DAPP_CHAN_INDEX]);
	optek_drc_vol_set_mute_off(optek_dapp.drc_vol_2[DAPP_CHAN_INDEX]);
#endif

#else

	//EQ normal, no 3d filter. no Enhanced BASS
	optek_dapp.dapp_func[DAPP_CHAN_INDEX] = 0;
	//disable 3d
	optek_dapp._3D_factor[DAPP_CHAN_INDEX] = 0;
#endif //OPREK_DAPP_DEBUG


#ifdef OPTEK_DSP_MX1
	//enable mix1
	optek_dapp.mx1_factor[DAPP_CHAN_INDEX] = 0x08000000;

	n_mems = optek_bqriir_alloc(OPTEK_DAPP_MAX_MIX1_BAND);
	objmem = &optek_dapp_alloc_buf[size];
	size += n_mems;
	optek_dapp.mx1_biquad[DAPP_CHAN_INDEX] = optek_bqriir_open(objmem, OPTEK_DAPP_MAX_MIX1_BAND,coef_mix1_band);
#else
	//disable mix1
	optek_dapp.mx1_factor[DAPP_CHAN_INDEX] = 0;
#endif

	//disable mix2
	optek_dapp.mx2_factor[DAPP_CHAN_INDEX] = 0;

#ifdef DSPEQ_SETTING_ENABLE
	optek_dsp_setting_init();
#endif	
}

void optek_dapp_close (void)
{

}

void optek_dapp_fini (void)
{

}

void optek_dapp_ebass_delay_init (void)
{
#ifdef OPTEK_EBASS_DELAY
	memset(optek_dapp_ebass_delay,0,sizeof(optek_dapp_ebass_delay));
#endif
}

#if defined OPTEK_DSP_MX1 || defined PROMPT_TONE_ENABLE
void mix1_processing(int * pmusic, int N) __INTERNAL_RAM_TEXT;

#include "peak_level_meter.h"

#if 1
//U32 mic_buf[1024/4];
U32 mic_buf[512/4];
#else
U32 mic_buf[((MP3_DEC_FRAME_SIZE*3)/4)/2] __attribute__ ((section (".codec_memory_bss1")));
#endif

extern U8 mic_opened;
extern peak_level_det mic_peak_44p1K;
#endif


void mix1_processing(int * pmusic, int N)
{
#ifdef OPTEK_DSP_MX1
	if (mic_opened == FALSE)
	{
		return;
	}

	OPTEK_DAPPs *dapp_obj = &optek_dapp;
	
	U32 *pADCDest;
	U32 wlen,dlen;
	int byte_len;
	int samples;
	int i; 

	//mx2 for prompt sound
	U8 slice = 1;

	while(N > sizeof(mic_buf)/2)
	{
		slice <<= 1;
		N >>= 1;
	}

	int *p = pmusic;

	do
	{
		samples = N;
		byte_len = samples<<1;

		pADCDest = DMA_3_DEST;			//current fifo write address
		fifo_put_data_by_dma (&MicInFifo, pADCDest);

		wlen = fifo_get_fifo_data_wlen(&MicInFifo);
		dlen = fifo_get_fifo_data_len(&MicInFifo);

		if (wlen < 0x4000)
		{
			//discrad some data for pop noise and fix the latency
			if (dlen > byte_len)
			{
				fifo_discard_data(&MicInFifo, (dlen - byte_len));
			}
			//memset(pMicInSampleStore,0,MicInSampleStoreSize);
			memset(mic_buf,0,sizeof(mic_buf));
		}
		else
		{
			if (dlen < (byte_len) )
			{
				DBG_Printf ("Mic underflow\n\r");
				//memset(mic_buf,0,sizeof(mic_buf));
			}
			else
			{
				if (dlen >= MicInSampleStoreSize)
				{
					fifo_discard_data(&MicInFifo, (dlen - MicInSampleStoreSize)+512);
					DBG_Puts("Mic overflow\n\r");
				}
				fifo_get_data (&MicInFifo, mic_buf, byte_len);


				#if 1//for checking mic in
				U32 peak;
				static U16 cnt;
				
				peak = peak_level_meter_16bit(&peak_det_handle2, mic_buf, byte_len);
				if (peak > max_peak_2)
				{
					max_peak_2 = peak;
				}
				
				if (cnt++ == 1000)
				//if (cnt++ == 500)
				{
					cnt = 0;
					DBG_Printf("mic_peak 0x%x\n\r", max_peak_2);
					max_peak_2 = 0;
				}
				#endif
				

				optek_hifi2_stereo_16b_to_mono(mic_buf, mic_buf, samples); //1 channel stereo OUTPUT

				samples >>= 1;
				byte_len = samples<<1;

				#ifdef MIC_DODGE_ENABLE
				U16 mic_peak_level = peak_level_meter_16bit(&mic_peak_44p1K, (short *)mic_buf, byte_len);
				#endif
				
				#ifdef MIC_EQ_ENABLE
				optek_mic_eq_processing(mic_buf, byte_len);
				#endif
				
				#ifdef MIC_REVERB
				//if (reverb_sw && !mute_flag)
				{
					optek_hifi2_mic_reverb_process(mic_reverb_obj,mic_buf,samples,mic_buf);
				}
				//fifo_put_data (&mic_fifo, (U8*) mic_buf, byte_len);
				#endif
				
				#ifdef MIC_ECHO
				optek_hifi2_mic_echo_process(mic_echo_obj,mic_buf,samples,mic_buf);
				#endif
	
				//fifo_put_data (&mic_fifo, (U8*) mic_buf,byte_len);
				#ifdef MIC_DODGE_ENABLE
				optek_mic_dodge_processing(pmusic,N,mic_peak_level);
				#endif
			}
		}

		optek_hifi2_24bstereo_16bmono_mixer(p,0x08000000,mic_buf,dapp_obj->mx1_factor[DAPP_CHAN_INDEX],p,N);
		p += N;
		
	} while(--slice);
#endif	
}

/*
	x->input/output
	chans->how many channels
	N->samples
*/

void bqdiir_stereo_process(int *x, int N)
{
	OPTEK_DAPPs *dapp_obj = &optek_dapp;
	optek_bqdiir_stereo_16b_process (dapp_obj->dc_block_biquad[DAPP_CHAN_INDEX],x,x,N);
}

#define NULL	(void *) 0


#if 0//(defined MI2S_OUTPUT_4CHANEL && defined LR_SUB_DSPEQ)
#if 0
U32 RxOutBuf[1152];
#else
U32 RxOutBuf[MP3_DEC_FRAME_SIZE*3/4] __attribute__ ((section (".codec_memory_bss1")));
#endif
#endif


void optek_dapp_process (int *x, int chans, int N) __INTERNAL_RAM_TEXT;
void optek_dapp_process (int *x, int chans, int N)
{
	int i,k,samples,index;
	void *mx1, *mx2;
	#if 1//def MI2S_OUTPUT_4CHANEL
	int s;
	int *OutBuf;
	int temp1, temp2;
	#endif

	OPTEK_DAPPs *dapp_obj = &optek_dapp;
	
	
	#if 1//def MI2S_OUTPUT_4CHANEL
	U8 OutLch_DataXch = 0;
	U8 OutRch_DataXch = 1;
	#if 1//def AUDIO_OUTPUT_LR_CH
	OutLch_DataXch = 0;
	OutRch_DataXch = 1;
	#elif defined AUDIO_OUTPUT_RL_CH
	OutLch_DataXch = 1;
	OutRch_DataXch = 0;
	#endif
	#endif

	#if 0//def MI2S_OUTPUT_6CHANEL
	U8 OutLch_DataXch = 0;
	U8 OutRch_DataXch = 1;
	#ifdef AUDIO_OUTPUT_LR_CH
	OutLch_DataXch = 0;
	OutRch_DataXch = 1;
	#elif defined AUDIO_OUTPUT_RL_CH
	OutLch_DataXch = 1;
	OutRch_DataXch = 0;
	#endif
	#endif

	#if 0//def MI2S_OUTPUT_8CHANEL
	U8 OutLch_DataXch = 0;
	U8 OutRch_DataXch = 1;
	#ifdef AUDIO_OUTPUT_LR_CH
	OutLch_DataXch = 0;
	OutRch_DataXch = 1;
	#elif defined AUDIO_OUTPUT_RL_CH
	OutLch_DataXch = 1;
	OutRch_DataXch = 0;
	#endif
	#endif


	#if 1//(defined MI2S_OUTPUT_4CHANEL || defined MI2S_OUTPUT_6CHANEL || defined MI2S_OUTPUT_8CHANEL)
	//OutBuf = (int *)&awOutStore.awOutSampleStore[awOut_contrls.awIndex][0];
	#else
	if(app_main_data.share_link_role == SL_ROLE_MASTER)
	{
		OutBuf = (int *)&awOutStore.awOutSampleStore[awOut_contrls.awIndex][0];
	}
	else
	{
		//OutBuf = (int *)RxOutBuf;
	}
	#endif


	if (chans != 2)
	{
		return; //support multi chan later
	}

	mx1 = NULL;
	mx2 = NULL;
	//getting data for mx1 and mx2


	//3D
	if (dapp_obj->_3D_factor[DAPP_CHAN_INDEX] != 0)
	{		
		//3D is enabled
		if ( (dapp_obj->dapp_func[DAPP_CHAN_INDEX] & OPTEK_DAPP_3D_CROSSOVER_MASK_BIT) != 0)
		{
			//3D filter is enabled
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
				optek_bqdiir_stereo_24b_process(dapp_obj->_3D_lpf[DAPP_CHAN_INDEX], optek_dapp_scrach, &x[index], samples);
				optek_bqdiir_stereo_24b_process(dapp_obj->_3D_hpf[DAPP_CHAN_INDEX], &x[index], &x[index], samples);

				optek_hifi2_3D(&x[index], &x[index], dapp_obj->_3D_factor[DAPP_CHAN_INDEX], samples);
				
				optek_hifi2_plus_24b(&x[index], &x[index], optek_dapp_scrach, samples);
				
				index += samples;
				i -= samples;
			}
		}
		else
		{
			//3D crossover filter is disabled
			optek_hifi2_3D(x, x, dapp_obj->_3D_factor[DAPP_CHAN_INDEX], N);
		}
	} 


	#ifdef OPREK_DAPP_DEBUG
	//only for debug
	{
		extern int loop_cn;
		loop_cn++;

		if (loop_cn > 800)
		{
			//optek_vol_24b_process(x,x,0x08000000,N);		//test small signal, -62db NLD works, <-62db->zero out
			optek_vol_24b_process(x,x,0x00000000,N);		//test small signal, -62db NLD works, <-62db->zero out
		}
		else if (loop_cn > 400) 
		{
			optek_vol_24b_process(x,x,0x02000000,N);		//test small signal, -62db NLD works, <-62db->zero out
			//optek_vol_24b_process(x,x,0x0000C000,N);		//test small signal, -62db NLD works, <-62db->zero out
		}

		if (loop_cn == 1200) {
			asm ("nop");
		}
	}
	#endif //OPREK_DAPP_DEBUG


	//ebass
	if ( (dapp_obj->dapp_func[DAPP_CHAN_INDEX] & OPTEK_DAPP_EBASS_MASK_BIT) != 0)
	{
		//EBASS is enabled
		i = N;
		index = 0;

		#ifdef OPTEK_EBASS_CUTOFF
		optek_bqdiir_stereo_24b_process (dapp_obj->ebass_cutoff[DAPP_CHAN_INDEX],x,x,N);
		#endif //OPTEK_EBASS_CUTOFF

		
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
			optek_bqdiir_stereo_24b_process(dapp_obj->drc_lpf[DAPP_CHAN_INDEX], optek_dapp_scrach, &x[index], samples);

			#ifdef OPTEK_EBASS_DELAY
			optek_bqdiir_stereo_24b_process(dapp_obj->drc_hpf[DAPP_CHAN_INDEX], &optek_dapp_ebass_delay[OPTEK_EBASS_DELAY_MAX], &x[index], samples);
			{
				ae_p24x2s ae_x; 
				const ae_p24x2s *pd = (const ae_p24x2s *)(&x[index]-2);
				const ae_p24x2s *ps = (const ae_p24x2s *)(optek_dapp_ebass_delay-2);

				for (k=0; k<samples/(2*4); k++)
				{
					//x[index+k] = optek_dapp_ebass_delay[k];
					LD_P24X2F_IU(ae_x, ps, 1);	
					ST_P24X2F_IU(ae_x, pd, 1);
					LD_P24X2F_IU(ae_x, ps, 1);	
					ST_P24X2F_IU(ae_x, pd, 1);
					LD_P24X2F_IU(ae_x, ps, 1);	
					ST_P24X2F_IU(ae_x, pd, 1);
					LD_P24X2F_IU(ae_x, ps, 1);	
					ST_P24X2F_IU(ae_x, pd, 1);
				}

				pd = (const ae_p24x2s *)(optek_dapp_ebass_delay-2);
				ps = (const ae_p24x2s *)(&optek_dapp_ebass_delay[samples]-2);

				for (k=0; k<OPTEK_EBASS_DELAY_MAX/(2*4); k++)
				{
					//optek_dapp_ebass_delay[k] = optek_dapp_ebass_delay[samples+k];
					LD_P24X2F_IU(ae_x, ps, 1);	
					ST_P24X2F_IU(ae_x, pd, 1);
					LD_P24X2F_IU(ae_x, ps, 1);	
					ST_P24X2F_IU(ae_x, pd, 1);
					LD_P24X2F_IU(ae_x, ps, 1);	
					ST_P24X2F_IU(ae_x, pd, 1);
					LD_P24X2F_IU(ae_x, ps, 1);	
					ST_P24X2F_IU(ae_x, pd, 1);
				}
			}
			
			#if 0
			for (k=0; k<samples; k++)
			{
				x[index+k] = optek_dapp_ebass_delay[k];
			}
			
			for (k=0; k<OPTEK_EBASS_DELAY_MAX; k++)
			{
				optek_dapp_ebass_delay[k] = optek_dapp_ebass_delay[samples+k];
			}
			#endif

			#else
			optek_bqdiir_stereo_24b_process(dapp_obj->drc_hpf[DAPP_CHAN_INDEX], &x[index], &x[index], samples);
			#endif //OPTEK_EBASS_DELAY


			#ifndef OPREK_EBASS_HPF_FILTER_DEBUG
			optek_ebass_NLD(dapp_obj->ebass[DAPP_CHAN_INDEX], optek_dapp_scrach, optek_dapp_scrach,
							dapp_obj->ebass_band[DAPP_CHAN_INDEX], samples);


			#ifdef OPREK_EBASS_NLD_POST_FILTER_DEBUG
			optek_vol_ebass_24b_process (optek_dapp_scrach, optek_dapp_scrach, dapp_obj->ebass_factor[DAPP_CHAN_INDEX], samples);
			CFasm_memcpy(&x[index], optek_dapp_scrach, samples*4);
			#else
			
			optek_vol_ebass_24b_process (optek_dapp_scrach, optek_dapp_scrach, dapp_obj->ebass_factor[DAPP_CHAN_INDEX], samples);
			optek_hifi2_plus_24b(&x[index], &x[index], optek_dapp_scrach, samples);
			#endif //OPREK_EBASS_NLD_POST_FILTER_DEBUG
			#endif //OPREK_EBASS_HPF_FILTER_DEBUG	
			
			index += samples;
			i -= samples;
		}
	}


	//prescale
	optek_hifi2_prescale_24b(x,x,N);

	//Biquad
	optek_bqdiir_stereo_24b_process (dapp_obj->biquad[DAPP_CHAN_INDEX],x,x,N);

	//drc
	if ( (dapp_obj->dapp_func[DAPP_CHAN_INDEX] & OPTEK_DAPP_DRC_MASK_BIT) == OPTEK_DRC_EQ)
	{
		//eq drc
		optek_bqdiir_stereo_24b_process(dapp_obj->drc_hpf[DAPP_CHAN_INDEX], x, x, N);
		optek_drc_soft_vol_stereo_24b_process (dapp_obj->drc_vol[DAPP_CHAN_INDEX], x, x, N);
		
		
		#if (defined MI2S_OUTPUT_4CHANEL && defined LR_SUB_DSPEQ)

		#ifdef HIGH_LFE_CH_CHANGE
		for(s=0; s<N/2; s++)
		{
			OutBuf[s*4+0] = x[s*2+0];
			OutBuf[s*4+1] = x[s*2+1];

			#ifdef LFE_CH_STEREO_OUTPUT
			OutBuf[s*4+2] = x[s*2+0]; //Lfe
			OutBuf[s*4+3] = x[s*2+1]; //Lfe
			#else//LFE_CH_MONO_OUTPUT
			OutBuf[s*4+2] = ((x[s*2+0] +x[s*2+1])>>1);  //Lfe
			OutBuf[s*4+3] = 0; //Lfe
			#endif
		}
		#else//LFE_HIGH_CH_CHANGE
		for(s=0; s<N/2; s++)
		{
			#ifdef LFE_CH_STEREO_OUTPUT
			OutBuf[s*4+0] = x[s*2+0]; //Lfe
			OutBuf[s*4+1] = x[s*2+1]; //Lfe
			#else//LFE_CH_MONO_OUTPUT
			OutBuf[s*4+0] = ((x[s*2+0] +x[s*2+1])>>1);  // Lfe
			OutBuf[s*4+1] = 0; //Lfe
			#endif
			
			OutBuf[s*4+2] =	x[s*2+0];
			OutBuf[s*4+3] =	x[s*2+1];
		}
		#endif

		#endif //(defined MI2S_OUTPUT_4CHANEL && defined LR_SUB_DSPEQ)
	}
	else if ( (dapp_obj->dapp_func[DAPP_CHAN_INDEX] & OPTEK_DAPP_DRC_MASK_BIT) == OPTEK_DRC_MULTI_BAND)
	{
		#define AUDIO_MULTI_CH_NORMAL_OUT_CHECK
		
		#ifdef AUDIO_MULTI_CH_NORMAL_OUT_CHECK
		
		goto AUDIO_MULTI_CH_NORMAL_OUT;
		
		#else
		//multi band drc
		int *pOutBuf, *pInBuf, *pInBuf1;
		
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
			optek_bqdiir_stereo_24b_process(dapp_obj->drc_lpf[DAPP_CHAN_INDEX], optek_dapp_scrach, &x[index], samples);
			optek_bqdiir_stereo_24b_process(dapp_obj->drc_hpf[DAPP_CHAN_INDEX], &x[index], &x[index], samples);
		
			optek_drc_soft_vol_stereo_24b_process (dapp_obj->drc_vol[DAPP_CHAN_INDEX], &x[index], &x[index], samples);
			optek_drc_soft_vol_stereo_24b_process (dapp_obj->drc_vol_2[DAPP_CHAN_INDEX], optek_dapp_scrach, optek_dapp_scrach, samples);
			
			
			#if (defined MI2S_OUTPUT_4CHANEL && defined LR_SUB_DSPEQ)
			//DBG_printf("MI2S_OUTPUT_4CHANEL\r\n");

			s = 0; 
			pOutBuf = (int *)&OutBuf[index*2+s*4+0];
			pInBuf = (int *)&x[index+s*2+OutLch_DataXch];
			pInBuf1 = (int *)&optek_dapp_scrach[s*2+OutLch_DataXch];
			
			for(s=0; s<samples/2; s++)
			{
				#ifdef DOLBY_AC3_DEC
				if (decode_type == DECODE_DD_AC3)
				{
					continue;
				}
				#endif

				#ifdef DOLBY_DDPLUS71_DEC_ENABLE
				if ( (decode_type == DECODE_DDPLUS71) )
				{
					continue;
				}
				#endif

				#ifdef DTSDMP_DEC_EANBLE
				if (decode_type == DECODE_DTSDMP)
				{
					continue;
				}
				#endif
				
				/*
				if(app_main_data.outchannel == CHANNEL_LL)
				{
					OutBuf[index*2+s*4+0] = (optek_dapp_scrach[s*2+0]); //Lfe
					OutBuf[index*2+s*4+1] = 0;//Lfe

					OutBuf[index*2+s*4+2] = x[index+s*2+0];
					OutBuf[index*2+s*4+3] = x[index+s*2+0];
				}
				else if(app_main_data.outchannel == CHANNEL_RR)
				{
					OutBuf[index*2+s*4+0] = (optek_dapp_scrach[s*2+1]); //Lfe
					OutBuf[index*2+s*4+1] = 0;//Lfe

					OutBuf[index*2+s*4+2] = x[index+s*2+1];
					OutBuf[index*2+s*4+3] = x[index+s*2+1];
				}
				else if(app_main_data.outchannel == CHANNEL_LR)
				*/
				{
					#if 0
					#ifdef HIGH_LFE_CH_CHANGE
					OutBuf[index*2+s*4+0] = x[index+s*2+OutLch_DataXch];
					OutBuf[index*2+s*4+1] = x[index+s*2+OutRch_DataXch];

					#ifdef LFE_CH_STEREO_OUTPUT
					OutBuf[index*2+s*4+2] = optek_dapp_scrach[s*2+OutLch_DataXch]; //Lfe
					OutBuf[index*2+s*4+3] = optek_dapp_scrach[s*2+OutRch_DataXch]; //Lfe
					#else//LFE_CH_MONO_OUTPUT
					OutBuf[index*2+s*4+2] = ( (optek_dapp_scrach[s*2+OutLch_DataXch]>>1) +
											(optek_dapp_scrach[s*2+OutRch_DataXch]>>1) ); //Lfe
					OutBuf[index*2+s*4+3] = 0; //Lfe
					#endif
					
					#else//LFE_HIGH_CH_CHANGE
					
					#ifdef LFE_CH_STEREO_OUTPUT
					OutBuf[index*2+s*4+0] = optek_dapp_scrach[s*2+OutLch_DataXch]; //Lfe
					OutBuf[index*2+s*4+1] = optek_dapp_scrach[s*2+OutRch_DataXch]; //Lfe
					#else//LFE_CH_MONO_OUTPUT
					OutBuf[index*2+s*4+0] = ( (optek_dapp_scrach[s*2+OutLch_DataXch]>>1) +
											(optek_dapp_scrach[s*2+OutRch_DataXch]>>1) ); //Lfe
					OutBuf[index*2+s*4+1] = 0; //Lfe
					#endif
					
					OutBuf[index*2+s*4+2] = x[index+s*2+OutLch_DataXch];
					OutBuf[index*2+s*4+3] = x[index+s*2+OutRch_DataXch];
					#endif
					#endif

					#if 1
					temp1 = *pInBuf++;
					temp2 = *pInBuf++;
					
					*pOutBuf++ = temp1;
					*pOutBuf++ = temp2;

					#ifdef LFE_CH_STEREO_OUTPUT
					*pOutBuf++ = *pInBuf1++; //Lfe
					*pOutBuf++ = *pInBuf1++; //Lfe
					#else
					*pOutBuf++ = (*pInBuf1++ + *pInBuf1++)>>1); //Lfe
					*pOutBuf++ = 0; //Lfe
					#endif

					*pOutBuf++ = temp1;
					*pOutBuf++ = temp2;
					#endif
				}
			}
			
			#else
			
			//optek_hifi2_3D(&x[index], &x[index], dapp_obj->3D_factor[DAPP_CHAN_INDEX], samples);
			
			optek_hifi2_plus_24b(&x[index], &x[index], optek_dapp_scrach, samples);
			#endif //(defined MI2S_OUTPUT_4CHANEL && defined LR_SUB_DSPEQ)
			
			index += samples;
			i -= samples;
		}
		#endif //AUDIO_MULTI_CH_NORMAL_OUT_CHECK
	}
	else 
	{
		AUDIO_MULTI_CH_NORMAL_OUT:
	
		//normal drc
		optek_drc_soft_vol_stereo_24b_process (dapp_obj->drc_vol[DAPP_CHAN_INDEX], x, x, N);		

		#if 0//def DOLBY_AC3_DEC
		if ( (decode_type == DECODE_DD_AC3) )
		{
			return;
		}
		#endif

		#if 0//def DOLBY_DDPLUS71_DEC_ENABLE
		if ( (decode_type == DECODE_DDPLUS71) )
		{
			return;
		}
		#endif

		#if 0//def DTSDMP_DEC_EANBLE
		if ( (decode_type == DECODE_DTSDMP) )
		{
			return;
		}
		#endif

		#if 0
		DBG_PIN_HIGH;
		#endif
		
		#if (defined MI2S_OUTPUT_4CHANEL && defined LR_SUB_DSPEQ)
		
		for(s=0; s<N/2; s++)
		{
			#if 1
			//spend min:20us
			//Using pointers speed up 30% than using arrays
			temp1 = *x++;
			temp2 = *x++;
			
			#ifdef LFE_CH_STEREO_OUTPUT
			*OutBuf++ = temp1; //Lfe
			*OutBuf++ = temp2; //Lfe
			#else
			*OutBuf++ = (*x++ + *x)>>1); //Lfe
			*OutBuf++ = 0; //Lfe
			#endif

			*OutBuf++ = temp1;
			*OutBuf++ = temp2;
			#endif
		}
		
		#elif (defined MI2S_OUTPUT_6CHANEL && defined LR_SUB_DSPEQ)
		
		for(s=0; s<N/2; s++)
		{
			#if 1
			//spend min:26us
			//Using pointers speed up 50% than using arrays
			temp1 = *x++;
			temp2 = *x++;
			
			#ifdef LFE_CH_STEREO_OUTPUT
			*OutBuf++ = temp1; //Lfe
			*OutBuf++ = temp2; //Lfe
			#else
			*OutBuf++ = (*x++ + *x)>>1); //Lfe
			*OutBuf++ = 0; //Lfe
			#endif

			*OutBuf++ = temp1;
			*OutBuf++ = temp2;
			*OutBuf++ = temp1;
			*OutBuf++ = temp2;
			#endif

			#if 0
			static U16 cnt = 999;
			if (++cnt == 20000)
			{
				cnt = 0;
				DBG_Printf("6p_l:0x%x, 6p_r:0x%x\n\r", OutBuf[s*6+4], OutBuf[s*6+5]);
			}
			#endif
		}

		#elif (defined MI2S_OUTPUT_8CHANEL && defined LR_SUB_DSPEQ)

		for(s=0; s<N/2; s++)
		{
			#if 1
			//spend min:33us
			//Using pointers speed up 50% than using arrays
			temp1 = *x++;
			temp2 = *x++;
			
			#ifdef LFE_CH_STEREO_OUTPUT
			*OutBuf++ = temp1; //Lfe
			*OutBuf++ = temp2; //Lfe
			#else
			*OutBuf++ = (*x++ + *x)>>1); //Lfe
			*OutBuf++ = 0; //Lfe
			#endif

			*OutBuf++ = temp1;
			*OutBuf++ = temp2;
			*OutBuf++ = temp1;
			*OutBuf++ = temp2;
			*OutBuf++ = temp1;
			*OutBuf++ = temp2;
			#endif

			#if 0
			static U16 cnt = 999;
			if (++cnt == 20000)
			{
				cnt = 0;
				DBG_Printf("8p_l:0x%x, 8p_r:0x%x\n\r", OutBuf[s*7+4], OutBuf[s*7+5]);
			}
			#endif
		}
		
		#endif //(defined MI2S_OUTPUT_4CHANEL && defined LR_SUB_DSPEQ)

		#if 0
		DBG_PIN_LOW;
		#endif
	}

	#if 0//def OPTEK_DSP_MX1
	//mx1
	//if (!(mx1 == NULL || dapp_obj->mx1_factor[DAPP_CHAN_INDEX] == 0))
	if (!(dapp_obj->mx1_factor[DAPP_CHAN_INDEX] == 0))
	{
		//mix1 enable
		//optek_hifi2_stereo_24b_mixer(x, x, mx1, dapp_obj->mx1_factor[DAPP_CHAN_INDEX], N);
		mix1_processing(x, N);
	}
	#endif

	#if 0
	//mx2 for prompt sound
	U8 slice = 1;
	U16 samples = N>>1;
	while(samples > sizeof(mic_buf)/2)
	{
		slice <<= 1;
		samples >>= 1;
	}
	int *p = x;
	
	do
	{
		if (prompt_tone_audio_data_read(mic_buf,samples))
		{
			optek_hifi2_24bstereo_16bmono_mixer(p, 0x08000000, mic_buf, 0x04000000, p, samples<<1);
		}
		
		p += (samples<<1);
	} while (--slice);
	#endif

	/*
	if (!(mx2 == NULL || dapp_obj->mx2_factor[DAPP_MAX_CHAN] == 0))
	{
		//mix2 enable
		//optek_hifi2_stereo_24b_mixer(x,x,mx2,dapp_obj->mx2_factor[DAPP_MAX_CHAN],N);
	}
	*/

	asm("nop");
}

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

void * optek_dapp_get_lpf_handle(void)
{
	return (optek_dapp.drc_lpf[DAPP_CHAN_INDEX]);
}

#ifdef OPTEK_DSP_TASK
void taskDSP(void) __INTERNAL_RAM_TEXT;
int  cpregsBackup[XCHAL_CP1_SA_SIZE/4] __attribute__(( aligned (8) ));

void taskDSP (void)
{
	U32 eventMask,i;
	U32 *pADCDest;
	u32 dlen,wlen;	
	while(1)
    {
		tx_event_flags_get(&event_grop, DAPP_EVENT, TX_OR_CLEAR, &eventMask, TX_WAIT_FOREVER);

		if (sDacOutIsrBuf[DACOUTISRBUF_from_DEC].wSize > 0)
			continue;

		enableCoProcessor (CO_PROCESSOR_MASK_BITS);
		xthal_save_cpregs(cpregsBackup,1);

		if (gDecReadyDataLen)
		{			
			/***16 bits -> 24 bits*****/
			optek_dapp_16b_in_process (pgDecReadyData, 2, gDecReadyDataLen>>1);

			sDacOutIsrBuf[DACOUTISRBUF_from_DEC].src = DACOUTFROMDEC;
			sDacOutIsrBuf[DACOUTISRBUF_from_DEC].pwBuf = pgDecReadyData;
			sDacOutIsrBuf[DACOUTISRBUF_from_DEC].wSize = gDecReadyDataLen<<1;

			
		}
		else
		{
			short *src;
			src = (short*) &awOutStore.awOutSampleStore[0][0];
			/****no music ready so direct read mic or tone data******/
#if 0
			static U8 cnt = 0;
			if (cnt)
			{
				cnt = false;
				for(i=0;i<512/4;i++)
				{
					src[i] = 0x300;
				}
			}
			else
			{
				cnt = true;
				for(i=0;i<512/4;i++)
				{
					src[i] = -0x300;
				}
			}
#else
			memset(src,0,MIN_DAC_DMA_RESERVED_SIZE);
#endif
			optek_dapp_16b_in_process ((short *)src, 2, MIN_DAC_DMA_RESERVED_SIZE/4);			

			sDacOutIsrBuf[DACOUTISRBUF_from_DEC].src = DACOUTFROMDSP;
			sDacOutIsrBuf[DACOUTISRBUF_from_DEC].pwBuf = (short *)src;
			sDacOutIsrBuf[DACOUTISRBUF_from_DEC].wSize = MIN_DAC_DMA_RESERVED_SIZE;		
		}
		xthal_restore_cpregs(cpregsBackup,1);
	}
}

#endif	//OPTEK_DSP_TASK

#if 1
#include "os_config.h"

const int optek_EBass_factor_table [] = {
	0x00080000,//0dB
	0x0008F9E4,//+1dB
    0x000A1247,
	0x000B4CE0,
	0x000CADDC,
	0x000E39EA,
	0x000FF64C,
	0x0011E8E6,
	0x00141857,
	0x00168C0C,
	0x00194C58,
	0x001C6294,
	0x001FD93C,//+12dB
	0x0023BC14,
	0x00281850,
	0x002CFCC0,
	0x00327A01,
	0x0038A2BA,
	0x003F8BD7,
	0x00474CD1,
	0x00500000,
	0x0059C2F0,
	0x0064B6CA,
	0x007100C4,
	0x007ECA9C,//24
		
	0x00008E4329,
	0x00009F9EF8,
	0x0000B31902,
	0x0000C8F36F,
	0x0000E1787B,
	0x0000FCFB72,
	0x00011BD9C8,
	0x00013E7C59,
	0x00016558CC,
	0x000190F325,
	0x0001C1DF80,
	0x0001F8C410,//+36db

	0x0002365B4B,
	0x00027B766C,
	0x0002C90031,
	0x0003200000,//40dB
	0x0003819D61,
	0x0003EF23EC,
	0x00046A07B0,
	0x0004F3EA20,
	0x00058E9F9F,
	0x00063C35B8,
	0x0006FEFA16,
	0x0007D98257,//+48

	0x0008CEB4D3,
	0x0009E1D276,//50db
	0x000B1681D2,
	0x000C70DB7C,
	0x000DF577FF,
	0x000FA97F74,
	0x001192BB08,
	0x0013B7A8A4,
	0x00161F90F7,
	0x0018D2A039,
	0x001BDA01EB,
	0x001F400000,
	0x00231025CB,
	0x002757673E,
	0x002C244CE4,
	0x0031872542,
	0x0037923C3B,
	0x003E5A1938,
	0x0045F5C4E4,
	0x004E7F1769,
	0x005813103E,
	0x0062D238A5,
	0x006EE11236,
	0x007FFFFFFF,
};
void optek_dapp_set_EBass_mode(int pam1, int pam2)
{
	if (pam1 && pam2)
	{
		optek_dapp.dapp_func[DAPP_CHAN_INDEX] |= OPTEK_DAPP_EBASS_MASK_BIT;
		
	}
	else{
		optek_dapp.dapp_func[DAPP_CHAN_INDEX] &= ~OPTEK_DAPP_EBASS_MASK_BIT;
	}

	optek_dapp.ebass_factor[DAPP_CHAN_INDEX] = optek_EBass_factor_table[pam2];//0x0008000000;  //0db

	DBG_Printf("EBass Mode:0x%x,%d,0x%x\n",optek_dapp.dapp_func[DAPP_CHAN_INDEX],pam2,
		optek_dapp.ebass_factor[DAPP_CHAN_INDEX]);
}

void optek_dapp_fall_time_update(float pam)
{
	DBG_Printf("nld fall time:%d\n",(int)(pam*1000));
	optek_ebass_set(optek_dapp.ebass[DAPP_CHAN_INDEX], NLD_RISE_TIME, ((int) (2147483648*pam)));
}

int optek_dapp_EBass_Filter_update(int type,int *coefData,int *coefData2)
{
	int ret;
	TX_INTERRUPT_SAVE_AREA;
	TX_DISABLE;

	if (type)
	{
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_hpf[DAPP_CHAN_INDEX], 0, coefData);//100hz HPF
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_hpf[DAPP_CHAN_INDEX], 1, coefData+5);//100hz HPF
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_lpf[DAPP_CHAN_INDEX], 0, coefData+10);//100hz LPF
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_lpf[DAPP_CHAN_INDEX], 1, coefData+15);//100hz LPF
		

		ret = optek_bqriir_5_coeffs_updata(optek_dapp.ebass_band[DAPP_CHAN_INDEX], 0, coefData2);//250hz LPF
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.ebass_band[DAPP_CHAN_INDEX], 1, coefData2+5);//250hz LPF	
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.ebass_band[DAPP_CHAN_INDEX], 2, coefData);//100hz HPF
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.ebass_band[DAPP_CHAN_INDEX], 3, coefData+5);//100hz HPF
	}
	else
	{
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_hpf[DAPP_CHAN_INDEX], 0, &coef_drc_hpf[1]);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_hpf[DAPP_CHAN_INDEX], 1, &coef_drc_hpf[7]);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_lpf[DAPP_CHAN_INDEX], 0, &coef_drc_lpf[1]);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_lpf[DAPP_CHAN_INDEX], 1, &coef_drc_lpf[7]);		
		
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.ebass_band[DAPP_CHAN_INDEX], 0, &coef_ebass_band[1]);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.ebass_band[DAPP_CHAN_INDEX], 1, &coef_ebass_band[7]);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.ebass_band[DAPP_CHAN_INDEX], 2, &coef_ebass_band[13]);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.ebass_band[DAPP_CHAN_INDEX], 3, &coef_ebass_band[19]);		
	}
	
	
	TX_RESTORE;

	return ret;
}


const int optek_3D_factor_table [] = {

};

void optek_dapp_set_3D_mode(int pam1, int pam2)
{
	if (pam1)
	{
		optek_dapp.dapp_func[DAPP_CHAN_INDEX] |= OPTEK_DAPP_3D_CROSSOVER_MASK_BIT;
		
	}
	else{
		optek_dapp.dapp_func[DAPP_CHAN_INDEX] &= ~OPTEK_DAPP_3D_CROSSOVER_MASK_BIT;
		//optek_dapp._3D_factor[DAPP_CHAN_INDEX] = 0;//0x08000000;//0db
	}

	if (pam2 == 0)
	{
		optek_dapp._3D_factor[DAPP_CHAN_INDEX] = 0;
	}
	else{
		optek_dapp._3D_factor[DAPP_CHAN_INDEX] = optek_vol_down_rate[pam2];//0x08000000;//0db
	}

	DBG_Printf("3D Mode:0x%x,0x%x\n",optek_dapp.dapp_func[DAPP_CHAN_INDEX],optek_dapp._3D_factor[DAPP_CHAN_INDEX]);
}

int optek_dapp_3D_Filter_update(int type,int *coefData)
{
	int ret;
	TX_INTERRUPT_SAVE_AREA;
	TX_DISABLE;

	if (type)
	{
		ret = optek_bqriir_5_coeffs_updata(optek_dapp._3D_hpf[DAPP_CHAN_INDEX], 0, coefData);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp._3D_hpf[DAPP_CHAN_INDEX], 1, coefData+5);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp._3D_lpf[DAPP_CHAN_INDEX], 0, coefData+10);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp._3D_lpf[DAPP_CHAN_INDEX], 1, coefData+15);
	}
	else{
		ret = optek_bqriir_5_coeffs_updata(optek_dapp._3D_hpf[DAPP_CHAN_INDEX], 0, &coef_3d_hpf[1]);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp._3D_hpf[DAPP_CHAN_INDEX], 1, &coef_3d_hpf[7]);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp._3D_lpf[DAPP_CHAN_INDEX], 0, &coef_3d_lpf[1]);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp._3D_lpf[DAPP_CHAN_INDEX], 1, &coef_3d_lpf[7]);		
	}
	
	
	TX_RESTORE;

	return ret;
}

extern const int optek_vol_vol_table [];
int optek_dapp_10eqData_update(int *coefData)
{
	int ret;
	
	TX_INTERRUPT_SAVE_AREA;
	TX_DISABLE;
	
	ret = optek_bqriir_5_coeffs_updata_all(optek_dapp.biquad[DAPP_CHAN_INDEX], coefData);
	
	
	TX_RESTORE;

	return ret;
}
int optek_dapp_Appoint_eqData_update(int eq_num, int *coefData)
{
	int ret;
	TX_INTERRUPT_SAVE_AREA;
	TX_DISABLE;
	
	ret = optek_bqriir_5_coeffs_updata(optek_dapp.biquad[DAPP_CHAN_INDEX], eq_num, coefData);
	
	
	TX_RESTORE;

	return ret;
}

void optek_dapp_set_Master_vol(int drcNum, int value)
{
	if (drcNum == 0)
	{
		optek_drc_vol_set_master_vol(optek_dapp.drc_vol_2[DAPP_CHAN_INDEX], optek_vol_vol_table[value]);	
	}
	else{
		//optek_drc_vol_set_master_vol(optek_dapp.drc_vol[DAPP_CHAN_INDEX], value);	
		optek_drc_vol_set_master_vol(optek_dapp.drc_vol[DAPP_CHAN_INDEX], optek_vol_vol_table[value]);	
	}
}

void optek_dapp_set_vol(int drcNum, int value)
{
	if (drcNum == 0)
	{
		optek_drc_vol_set_vol(optek_dapp.drc_vol_2[DAPP_CHAN_INDEX], optek_vol_vol_table[value], optek_vol_vol_table[value]);	
	}
	else
	{
		//optek_drc_vol_set_vol(optek_dapp.drc_vol[DAPP_CHAN_INDEX], value, value);	
		optek_drc_vol_set_vol(optek_dapp.drc_vol[DAPP_CHAN_INDEX], optek_vol_vol_table[value], optek_vol_vol_table[value]);	
	}
}

int optek_dapp_set_drc(int drcNum, int attack, int release, int attack_rate, int release_rate,
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
		#if 0
		optek_drc_vol_set_drc(optek_dapp.drc_vol_2[DAPP_CHAN_INDEX], attack, release, attack_rate, release_rate, 
					OPTEK_DRC_LEV_DOWN_RATE_0P004DB, clipping,DAPP_1MS_SAMPLES_44_1kHz);
		#else
		//new the library of lib_optek_dsp_dapp.a
		optek_drc_vol_set_drc(optek_dapp.drc_vol_2[DAPP_CHAN_INDEX], attack, release, attack_rate, release_rate, 
					OPTEK_DRC_LEV_DOWN_RATE_0P004DB, clipping, DAPP_1MS_SAMPLES_44_1kHz, 600);
		#endif
	}
	else
	{
		#if 0
		optek_drc_vol_set_drc(optek_dapp.drc_vol[DAPP_CHAN_INDEX], attack, release, attack_rate, release_rate, 
					OPTEK_DRC_LEV_DOWN_RATE_0P04DB, clipping,DAPP_1MS_SAMPLES_44_1kHz);
		#else
		//new the library of lib_optek_dsp_dapp.a
		optek_drc_vol_set_drc(optek_dapp.drc_vol[DAPP_CHAN_INDEX], attack, release, attack_rate, release_rate, 
					OPTEK_DRC_LEV_DOWN_RATE_0P04DB, clipping,DAPP_1MS_SAMPLES_44_1kHz, 600);
		#endif
	}
	return 1;
}

void optek_dapp_set_DRC_mode(int pam)
{
	optek_dapp.dapp_func[DAPP_CHAN_INDEX] &= 0xFC;
	optek_dapp.dapp_func[DAPP_CHAN_INDEX] |= pam;

	DBG_Printf("optek_dapp.dapp_func[DAPP_CHAN_INDEX]:0x%x\n",optek_dapp.dapp_func[DAPP_CHAN_INDEX]);
}

int optek_dapp_DBDRC_Filter_update(int type,int *coefData)
{
	int ret;

	if (optek_dapp.dapp_func[DAPP_CHAN_INDEX] & OPTEK_DAPP_EBASS_MASK_BIT) return 0;
	
	TX_INTERRUPT_SAVE_AREA;
	TX_DISABLE;

	if (type == 0)
	{
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type->drc_filter[0]:0x%x\n",*coefData++);
		
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_hpf[DAPP_CHAN_INDEX], 0, coefData);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_hpf[DAPP_CHAN_INDEX], 1, coefData);
	}
	else{

		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		//DBG_Printf("type1->drc_filter[0]:0x%x\n",*coefData++);
		
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_lpf[DAPP_CHAN_INDEX], 0, coefData);
		ret = optek_bqriir_5_coeffs_updata(optek_dapp.drc_lpf[DAPP_CHAN_INDEX], 1, coefData);
	}
	
	
	TX_RESTORE;

	return ret;
}

#endif
#endif

