/*
 * optek_eq_tune_by_uart.h
 *
 *  Created on: 2019-11-21
 *      Author: Windowns
 */
#ifndef __OPTEK_EQ_TUNE_BY_UART_H__
#define __OPTEK_EQ_TUNE_BY_UART_H__


#define DAPP_0p1MS_SAMPLES_44_1kHz			4			/*44.1kHz*/

#define	E_VALUE      19 //Q19 [-256,256)
//#define EQ_NUMBER            10//used EQ number
//#define FC_MIN_FREQUENCY     0
//#define FC_MAX_FREQUENCY     SAMPLE_FREQUENCY/2
//#define FC_FREQ_EQUAL_GAP    1//Hz
//#define PI                   3.1415926535898


typedef struct{
	int b0;
	int b1;
	int b2;
	int a1;
	int a2;
} EQ_COEF;

typedef	struct {
	int Attack_Rate;
	int Attack;
	int Release_Rate;
	int Release;
	int clipping;
	int master_volume;
	int volume;
}DRC_PARMS_FPs;

typedef	struct {
	int magic_start;
	int len;
	EQ_COEF eq_filter[10];
	//EQ_COEF drc_filter[4];   //EBASS uses the same eq
	//EQ_COEF _3d_filter[4];
	EQ_COEF _3d_filter[4];   //EBASS uses the same eq
	EQ_COEF drc_filter[4];
	
	EQ_COEF EBASS_filter[2]; //EBASS stop frequecy
	
    int DRC_mode;
    int DRC_cutOff;
	DRC_PARMS_FPs DRC[2];
	
	int _3D_en;
	int _3D_cutOff_Freq;
	int _3D_factor;
	
	int EBASS_en;
	int EBASS_LPF_Freq; //change to fall timer factor
	int EBASS_HPF_Freq; //change to cut off freq
	int EBASS_Gain;     //virtual bass
	int magic_end;
} OPTEK_DSP_RXs;

extern OPTEK_DSP_RXs optek_dsp_rx;

extern const int dsp_setting[];

void optek_dsp_tools_tuning_init(void);

#ifdef DSP_HIFI_LIB_OLD
void app_nav_eq_set_process(OPTEK_DSP_RXs *Uart_rx_dsp_data);
#else
void app_nav_eq_set_process(void *optek_dapp, OPTEK_DSP_RXs *Uart_rx_dsp_data);
#endif

void app_nav_eq_tuning_data_receive(U8 data);

#endif //__OPTEK_EQ_TUNE_BY_UART_H__