#ifndef OPTEK_HIFI2_COMMON_H

#define OPTEK_HIFI2_COMMON_H

#include <xtensa/tie/xt_core.h>
#include "NatureDSP_Signal.h"


#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif


typedef struct
{
    uint32_t magic;				//check if the structure is valid
	#ifdef DSP_HIFI_LIB_OLD
	uint32_t enable_mask;		//32 enable mask
	#endif
    int M;						//how many bqriir
	#ifndef DSP_HIFI_LIB_OLD		
    int M_MAX;					//ALLOWED BUFFER
	#endif			
    f32 * coef;					// [M*6] aligned 
    f32 * d;					// [2*(M+1)] aligned
    f32 * d_R;					// [2*(M+1)] aligned		//Stereo, Channel R
    f32 * xd;					// [2*(M+1)] aligned		//Form I
    f32 * xd_R;					// [2*(M+1)] aligned		//Form I, Stereo Channel R
} optek_bqriir;


/*
	input->no input
	return->string of optek_bgriir build date,time and version
*/

#define ID1_NO   0x3af1b

#define OPTEK_HIFI2_DSP_ATTIB	__attribute__ ((section (".dsp_critical_sec_text")))

typedef signed char   int8_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

//#define DAPP_PRE_POST_SCALE           2 /*+12db margin for EQ*/
//#define DAPP_PRE_POST_SCALE             3 /*+18db margin for EQ*/
#define DAPP_PRE_POST_SCALE             4 /*+24db margin for EQ*/

#define DAPP_1MS_SAMPLES_44_1kHz        44 /*44.1kHz*/
#define DAPP_1MS_SAMPLES_88_2kHz        88 /*88.2kHz*/

#define DAPP_1MS_SAMPLES_48kHz          48 /*48kHz*/
#define DAPP_1MS_SAMPLES_96kHz          96 /*96kHz*/

#define VOL_EXP				4				/*max gain is +24db*/
#define EBASS_VOL_EXP		12				/*max gain is +72db, 0x00080000->0db*/



#define OPTEK_VOL_X1                    (0x80000000 >> VOL_EXP)
#define OPTEK_VOL_X0P5                  (0x80000000 >> (VOL_EXP+1))

//#define OPTEK_EBASS_DELAY    /*delay for phase match for some freq (for example 70Hz)*/
//#define OPTEK_EBASS_CUTOFF   /*cutoff for only 1/3 or 1/4 harmonics*/

//#define OPREK_DAPP_DEBUG

//#define OPREK_EBASS_HPF_FILTER_DEBUG
//#define OPREK_EBASS_NLD_PRE_FILTER_DEBUG
//#define OPREK_EBASS_NLD_POST_FILTER_DEBUG

//#define DSP_DEBUG


#undef OPTEK_CHIP_CHECK
#define OPTEK_CHIP_CHECK

#undef OPTEK_DOBLY_CHIP_CHECK
//#define OPTEK_DOBLY_CHIP_CHECK


const char *optek_dsp_dapp_data_get(void);
const char *optek_dsp_dapp_time_get(void);
const char *optek_dsp_dapp_version_get(void);


#endif	//OPTEK_HIFI2_DAPP_H

