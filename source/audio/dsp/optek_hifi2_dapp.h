#ifndef OPTEK_HIFI2_DAPP_H

#define OPTEK_HIFI2_DAPP_H

#include "optek_hifi2_common.h"
#include "optek_hifi2_bqriir.h"

//#define OPTEK_EBASS_DELAY  /*delay for phase match for some freq (for example 70Hz)*/
//#define OPTEK_EBASS_CUTOFF /*cutoff for only 1/3 or 1/4 harmonics*/

//#define OPREK_DAPP_DEBUG

//#define OPREK_EBASS_HPF_FILTER_DEBUG
//#define OPREK_EBASS_NLD_PRE_FILTER_DEBUG
//#define OPREK_EBASS_NLD_POST_FILTER_DEBUG


void optek_dapp_init (void);
void optek_dapp_open (void);
void optek_dapp_close (void);
void optek_dapp_fini (void);

void optek_dapp_process (int *x, int chans, int N);
void optek_dapp_16b_in_process (short *x, int chans, int N);
void optek_dapp_ebass_delay_init (void);

void optek_dapp_fall_time_update(float pam);
void optek_dapp_set_EBass_mode(int pam1, int pam2);

int optek_dapp_EBass_Filter_update(int type,int *coefData,int *coefData2);

void optek_dapp_set_3D_mode(int pam1, int pam2);
int optek_dapp_3D_Filter_update(int type,int *coefData);

int optek_dapp_DBDRC_Filter_update(int type,int *coefData);
void optek_dapp_set_DRC_mode(int pam);
int optek_dapp_10eqData_update(int *coefData);
int optek_dapp_Appoint_eqData_update(int eq_num, int *coefData);
void optek_dapp_set_Master_vol(int drcNum, int value);
void optek_dapp_set_vol(int drcNum, int value);
int optek_dapp_set_drc(int drcNum, int attack, int release, int attack_rate, int release_rate,
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


int optek_dapp_get_volume_table_data(U8 vol_level,U8 max_level);


#endif	//OPTEK_HIFI2_DAPP_H

