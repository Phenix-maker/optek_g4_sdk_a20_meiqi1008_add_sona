#ifndef OPTEK_HIFI2_DAPP_H

#define OPTEK_HIFI2_DAPP_H

#include "optek_hifi2_common.h"
#include "optek_hifi2_bqriir.h"


#define OPTEK_DSP_LR_MAX_EQ  		10
#define OPTEK_DSP_CENTER_MAX_EQ  	10
#define OPTEK_DSP_SUB_MAX_EQ  		10

#define OPTEK_DSP_LR_DC  			0
#define OPTEK_DSP_LR_DRC  			1
#define OPTEK_DSP_LR_DBDRC  		0
#define OPTEK_DSP_LR_3D  			0
#define OPTEK_DSP_LR_EBASS  		0

#define OPTEK_DSP_CENTER_DC  		0
#define OPTEK_DSP_CENTER_DRC  		1
#define OPTEK_DSP_CENTER_DBDRC  	0
#define OPTEK_DSP_CENTER_3D  		0
#define OPTEK_DSP_CENTER_EBASS  	0

#define OPTEK_DSP_SUB_DC  			0
#define OPTEK_DSP_SUB_DRC  			1
#define OPTEK_DSP_SUB_DBDRC  		0
#define OPTEK_DSP_SUB_3D  			0
#define OPTEK_DSP_SUB_EBASS  		0


//#define OPTEK_EBASS_DELAY  /*delay for phase match for some freq (for example 70Hz)*/
//#define OPTEK_EBASS_CUTOFF /*cutoff for only 1/3 or 1/4 harmonics*/

//#define OPREK_DAPP_DEBUG

//#define OPREK_EBASS_HPF_FILTER_DEBUG
//#define OPREK_EBASS_NLD_PRE_FILTER_DEBUG
//#define OPREK_EBASS_NLD_POST_FILTER_DEBUG


typedef struct
{
	void *biquad;
	//DRC
	void *drc_vol;
	void *drc_vol_2;			/*multi band drc*/
	void *drc_hpf;
	void *drc_lpf;
	//3D
	uint32_t _3D_factor;
	void *_3D_hpf;
	void *_3D_lpf;

	//VBASS
	void *ebass;
	uint32_t ebass_factor;

#ifdef OPTEK_EBASS_CUTOFF
	void *ebass_cutoff;
#endif
	void *ebass_band;

	//mixer
	uint32_t mx1_factor;
	uint32_t mx2_factor;

	void *mx1_biquad;

	void *dc_block_biquad;

	//setting
	uint8_t dapp_func_enable_mask;
	uint8_t dapp_func;
} OPTEK_DAPPs __attribute__((packed));


void optek_dapp_init (void);
int optek_dapp_alloc(uint32_t main_eq_cnt, uint32_t dc_block_enable, uint32_t drc_enable, uint32_t multiband_drc_enable, uint32_t _3d_enable, uint32_t ebass_enable);
void *optek_dapp_open (uint8_t *buffer, uint32_t main_eq_cnt, const int *main_eq_coef, uint32_t dc_block_enable, uint32_t drc_enable, uint32_t multiband_drc_enable, uint32_t _3d_enable, uint32_t ebass_enable);
void optek_dapp_stereo_process (void *optek_dapp, int *x, int *r, int N, int in_stride, int out_stride);
void optek_dapp_mono_process (void *optek_dapp, int *x, int *r, int N, int in_stride, int out_stride);

int optek_dapp_DBDRC_Filter_update(void *optek_dapp, int type, int *coefData);
void optek_dapp_set_DRC_mode(void *optek_dapp, int pam);
int optek_dapp_10eqData_update(void *optek_dapp, int *coefData);
int optek_dapp_Appoint_eqData_update(void *optek_dapp, int eq_num, int *coefData);
void optek_dapp_set_Master_vol(void *optek_dapp, int drcNum, int value);
void optek_dapp_set_vol(void *optek_dapp, int drcNum, int value);
int optek_dapp_set_drc(void *optek_dapp, int drcNum, int attack, int release, int attack_rate, int release_rate,
					 uint8_t clipping, uint8_t update_samples);
extern const int optek_vol_vol_table [];
extern const int optek_vol_up_rate [];
extern const int optek_vol_down_rate [];


#define MSBC_MAX_BIQUAD    2
extern const int msbc_dsp_coef[MSBC_MAX_BIQUAD*6];

#define SR_24K_MAX_BIQUAD	2
extern const int sr_24k_dsp_coef[SR_24K_MAX_BIQUAD*6]; 

#ifdef BT_HFP_CVSD_FUNC_ENABLE
#define CVSD_MAX_BIQUAD    8
extern const int cvsd_dsp_coef[CVSD_MAX_BIQUAD*6];
#endif

#ifndef DSPEQ_SETTING_ENABLE
extern U8 dsp_set_buffer[1024];
extern OPTEK_DAPPs *optek_dapp_strero;
#else
extern U8 dsp_set_buffer[1024*3];
extern OPTEK_DAPPs *optek_dsp_lr;
extern OPTEK_DAPPs *optek_dsp_center;
extern OPTEK_DAPPs *optek_dsp_sub;
#endif

int optek_dapp_get_volume_table_data(U8 vol_level,U8 max_level);

void optek_dapp_16b_in_process (short *x, int chans, int N);

#endif	//OPTEK_HIFI2_DAPP_H

