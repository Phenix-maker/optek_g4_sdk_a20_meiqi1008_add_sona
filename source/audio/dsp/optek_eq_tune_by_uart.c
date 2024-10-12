/*
 * optek_eq_tune_by_uart.c
 *
 *  Created on: 2019-11-21
 *      Author: Windowns
 */
#include "os_config.h"

#include "common.h"


#ifdef DSPEQ_TUNING_ENABLE
#include "fifo.h"
#include "app_main.h"

#ifdef DSP_HIFI_LIB_OLD
#include "optek_hifi2_dapp.h"
#else
#include "optek_hifi2_dapp_strided.h"
#endif
#include "optek_eq_tune_by_uart.h"

#define DSPEQ_TUNING_FUNC_ENABLE
//#define DBG_Printf


OPTEK_DSP_RXs optek_dsp_rx __attribute__ ((section (".d_dsp_bss")));

U32 _2power(U32 exp)// output = 2^exp;
{
	U32 ret=1;

	while(exp--)
		ret *= 2;

	return ret;
}

#ifndef DSP_HIFI_LIB_OLD
void optek_dsp_setting_updata(void *optek_dapp, OPTEK_DSP_RXs *eqSetData)
{
	#ifdef DSPEQ_TUNING_FUNC_ENABLE
	int i;
	float xl;
	int attack,release,attack_rate,release_rate;

	if (eqSetData->magic_start == 0xAAAAAAAA && eqSetData->magic_end == 0xF0F0F0F0)
	{
		DBG_Printf("eq data valid\n");

		#if 0
		//_3D
		xl = ((float)eqSetData->_3D_factor)/_2power(E_VALUE);//Fixed Point Numbe -> float
		optek_dapp_set_3D_mode(eqSetData->_3D_en, (int)((float)(xl * 100.0)+0.5));
		optek_dapp_3D_Filter_update(eqSetData->_3D_en, (int *)&eqSetData->_3d_filter[0]);

		//Ebass
		if (eqSetData->DRC_mode != 0) eqSetData->EBASS_en = 0;//only normal drc enable
				
		xl = (eqSetData->EBASS_Gain)/_2power(E_VALUE);//Fixed Point Numbe -> float
		optek_dapp_set_EBass_mode(eqSetData->EBASS_en, (int)xl);
		if (eqSetData->EBASS_en && (int)xl)
		{
			xl = ((float)eqSetData->EBASS_LPF_Freq/_2power(E_VALUE));
			optek_dapp_fall_time_update(xl+0.00001);
					
			optek_dapp_EBass_Filter_update(eqSetData->EBASS_en, (int *)&eqSetData->drc_filter[0], 
				(int *)&eqSetData->EBASS_filter[0]);
		}
		#endif
		//EQ
		optek_dapp_10eqData_update(optek_dapp, (int *)eqSetData->eq_filter);

		//EQ DRC or DB DRC
		optek_dapp_set_DRC_mode(optek_dapp, eqSetData->DRC_mode);
		if (eqSetData->DRC_mode == 0x2 || eqSetData->DRC_mode == 0x1)
		{
			optek_dapp_DBDRC_Filter_update(optek_dapp, 0, (int *)&eqSetData->drc_filter[0]);
			optek_dapp_DBDRC_Filter_update(optek_dapp, 1, (int *)&eqSetData->drc_filter[2]);
		}
				
		for (i=0; i<2; i++)
		{
			if ((i == 0) && (eqSetData->DRC_mode != 0x2)) continue;

			//Master volume
			xl = 240 - (((float)eqSetData->DRC[i].master_volume)/_2power(E_VALUE) * 10);//optek_vol_vol_table[240]->0Db
			optek_dapp_set_Master_vol(optek_dapp, i, xl);

			//volume
			xl = 240 - (((float)eqSetData->DRC[i].volume)/_2power(E_VALUE) * 10);
			optek_dapp_set_vol(optek_dapp, i, xl);

			//attack
			xl = ((float)eqSetData->DRC[i].Attack)/_2power(E_VALUE);//Fixed Point Numbe -> float
			attack = (int)((float)((-xl) * 10.0) + 0.5);// *10,and rounding

			//release
			xl = ((float)eqSetData->DRC[i].Release)/_2power(E_VALUE);
			release = (int)((float)((-xl) * 10.0) + 0.5);

			//attack rate
			xl = ((float)eqSetData->DRC[i].Attack_Rate)/_2power(E_VALUE);
			attack_rate = (int)((float)(xl * 100.0) + 0.5);//for according to the talble data
			/*
			* if (xl <= 0.09)
			* {//for according to the talble data
			* 	attack_rate = (int)((float)(xl * 100.0) + 0.5);
			* }
			* else{
			* 	attack_rate = (int)((float)(xl * 10.0) + 0.5);
			* 	attack_rate *= 10;
			* }
			* */

			//release rate
			xl = ((float)eqSetData->DRC[i].Release_Rate)/_2power(E_VALUE);
			release_rate = (int)((float)(xl * 100.0) + 0.5);//for according to the talble data
			/*
			* if (xl <= 0.09)
			* {//for according to the talble data
			* 	release_rate = (int)((float)(xl * 100.0) + 0.5);
			* }
			* else{
			* 	release_rate = (int)((float)(xl * 10.0) + 0.5);
			* 	release_rate *= 10;
			* }
			* */

			optek_dapp_set_drc(optek_dapp, i, attack, release, attack_rate, release_rate, eqSetData->DRC[i].clipping, DAPP_0p1MS_SAMPLES_44_1kHz);
		}
	}
	#endif
}


void app_nav_eq_set_process(void *optek_dapp, OPTEK_DSP_RXs *Uart_rx_dsp_data)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DSPEQ_TUNING_FUNC_ENABLE
	memcpy(&optek_dsp_rx, Uart_rx_dsp_data, sizeof(optek_dsp_rx));
	
	optek_dsp_setting_updata(optek_dapp, &optek_dsp_rx);
	#endif
}

void optek_dsp_tools_tuning_init(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DSPEQ_TUNING_FUNC_ENABLE

	#endif
}

void app_nav_eq_tuning_data_receive(U8 data)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DSPEQ_TUNING_FUNC_ENABLE

	#endif
}


const int dsp_setting[] =
{
	0xAAAAAAAA,
	0x0,	

	/*************************************************************
	Sound configurations,it generate by pc tool--"OptekDspTools.exe"
	*************************************************************/
	//EQ Filter
	#if 1//bypass
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
	#else
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	#endif
	
	//3D Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	
	//DRC & EBass Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,

	//EBass Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,


	//DRC Setting
	//DRC Mode
	0, //OPTEK_DRC_NORMAL:0

	//DRC Cutoff Freqency
	0,

	//DRC 1
	10485,0,10485,0,0,0,0,
	//DRC 2
	10485,0,10485,0,0,0,0,


	//3D Setting
	//Enable, Cutoff Freqency, Factor
	0,127,0,


	//EBASS Setting
	//Enable, LPF Freqency, HPF Freqency, Gain
	0,2097,100,0,

	/*********************Sound configurations end**********************/

	0xF0F0F0F0
};





const int subwoofer_dsp_setting[] =
{
	0xAAAAAAAA,
	0x0,	

	/*************************************************************
	Sound configurations,it generate by pc tool--"OptekDspTools.exe"
	*************************************************************/
	//EQ Filter

	//Biquad 1
	0x00018eb5,
	0x00031d6b,
	0x00018eb5,
	0x81c6ec2f,
	0x3e3f4ea9,

	//Biquad 2
	0x3fd29167,
	0x805add31,
	0x3fd29167,
	0x805afd72,
	0x3fa5430f,

	//Biquad 3
	0x4013b454,
	0x8099fd62,
	0x3f54181b,
	0x8099fd62,
	0x3f67cc6f,

//	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
//	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
//	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	//3D Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	//DRC & EBass Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,

	//EBass Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,


	//DRC Setting
	//DRC Mode
	0,

	//DRC Cutoff Freqency
	0,

	//DRC 1
	10485,0,10485,0,0,0,0,
	//DRC 2
	10485,0,10485,0,0,3145728,0,


	//3D Setting
	//Enable, Cutoff Freqency, Factor
	0,127,0,


	//EBASS Setting
	//Enable, LPF Freqency, HPF Freqency, Gain
	0,2097,100,0,

	/*********************Sound configurations end**********************/

	0xF0F0F0F0
};

OPTEK_DSP_RXs optek_dsp_rx __attribute__ ((section (".d_dsp_bss")));

void optek_dsp_setting_init(void *optek_dapp, OPTEK_DSP_RXs *Uart_rx_dsp_data)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DSPEQ_TUNING_FUNC_ENABLE
	memcpy(&optek_dsp_rx, Uart_rx_dsp_data, sizeof(optek_dsp_rx));
	
	optek_dsp_setting_updata(optek_dapp, &optek_dsp_rx);
	#endif
}

#else


void optek_dsp_setting_updata(OPTEK_DSP_RXs *eqSetData)
{
	#ifdef DSPEQ_TUNING_FUNC_ENABLE
	int i;
	float xl;
	int attack,release,attack_rate,release_rate;

	if (eqSetData->magic_start == 0xAAAAAAAA && eqSetData->magic_end == 0xF0F0F0F0)
	{
		DBG_Printf("eq data valid\n");

		//_3D
		xl = ((float)eqSetData->_3D_factor)/_2power(E_VALUE);//Fixed Point Numbe -> float
		optek_dapp_set_3D_mode(eqSetData->_3D_en, (int)((float)(xl * 100.0)+0.5));
		optek_dapp_3D_Filter_update(eqSetData->_3D_en, (int *)&eqSetData->_3d_filter[0]);

		//Ebass
		if (eqSetData->DRC_mode != 0) eqSetData->EBASS_en = 0;//only normal drc enable
				
		xl = (eqSetData->EBASS_Gain)/_2power(E_VALUE);//Fixed Point Numbe -> float
		optek_dapp_set_EBass_mode(eqSetData->EBASS_en, (int)xl);
		if (eqSetData->EBASS_en && (int)xl)
		{
			xl = ((float)eqSetData->EBASS_LPF_Freq/_2power(E_VALUE));
			optek_dapp_fall_time_update(xl+0.00001);
					
			optek_dapp_EBass_Filter_update(eqSetData->EBASS_en, (int *)&eqSetData->drc_filter[0], 
				(int *)&eqSetData->EBASS_filter[0]);
		}

		//EQ
		optek_dapp_10eqData_update((int *)eqSetData->eq_filter);

		//EQ DRC or DB DRC
		optek_dapp_set_DRC_mode(eqSetData->DRC_mode);
		if (eqSetData->DRC_mode == 0x2 || eqSetData->DRC_mode == 0x1)
		{
			optek_dapp_DBDRC_Filter_update(0, (int *)&eqSetData->drc_filter[0]);
			optek_dapp_DBDRC_Filter_update(1, (int *)&eqSetData->drc_filter[2]);
		}
				
		for (i=0; i<2; i++)
		{
			if ((i == 0) && (eqSetData->DRC_mode != 0x2)) continue;

			//Master volume
			xl = 240 - (((float)eqSetData->DRC[i].master_volume)/_2power(E_VALUE) * 10);//optek_vol_vol_table[240]->0Db
			optek_dapp_set_Master_vol(i, xl);

			//volume
			xl = 240 - (((float)eqSetData->DRC[i].volume)/_2power(E_VALUE) * 10);
			optek_dapp_set_vol(i, xl);

			//attack
			xl = ((float)eqSetData->DRC[i].Attack)/_2power(E_VALUE);//Fixed Point Numbe -> float
			attack = (int)((float)((-xl) * 10.0) + 0.5);// *10,and rounding

			//release
			xl = ((float)eqSetData->DRC[i].Release)/_2power(E_VALUE);
			release = (int)((float)((-xl) * 10.0) + 0.5);

			//attack rate
			xl = ((float)eqSetData->DRC[i].Attack_Rate)/_2power(E_VALUE);
			attack_rate = (int)((float)(xl * 100.0) + 0.5);//for according to the talble data
			/*
			* if (xl <= 0.09)
			* {//for according to the talble data
			* 	attack_rate = (int)((float)(xl * 100.0) + 0.5);
			* }
			* else{
			* 	attack_rate = (int)((float)(xl * 10.0) + 0.5);
			* 	attack_rate *= 10;
			* }
			* */

			//release rate
			xl = ((float)eqSetData->DRC[i].Release_Rate)/_2power(E_VALUE);
			release_rate = (int)((float)(xl * 100.0) + 0.5);//for according to the talble data
			/*
			* if (xl <= 0.09)
			* {//for according to the talble data
			* 	release_rate = (int)((float)(xl * 100.0) + 0.5);
			* }
			* else{
			* 	release_rate = (int)((float)(xl * 10.0) + 0.5);
			* 	release_rate *= 10;
			* }
			* */

			optek_dapp_set_drc(i, attack, release, attack_rate, release_rate, eqSetData->DRC[i].clipping, DAPP_0p1MS_SAMPLES_44_1kHz);
		}
	}
	#endif
}

#ifdef DSPEQ_SETTING_ENABLE
void app_nav_eq_set_process(OPTEK_DSP_RXs *Uart_rx_dsp_data)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DSPEQ_TUNING_FUNC_ENABLE
	memcpy(&optek_dsp_rx, Uart_rx_dsp_data, sizeof(optek_dsp_rx));
	
	optek_dsp_setting_updata(&optek_dsp_rx);
	#endif
}

void optek_dsp_tools_tuning_init(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DSPEQ_TUNING_FUNC_ENABLE

	#endif
}

void app_nav_eq_tuning_data_receive(U8 data)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DSPEQ_TUNING_FUNC_ENABLE

	#endif
}


const int dsp_setting[] =
{
	0xAAAAAAAA,
	0x0,	

	/*************************************************************
	Sound configurations,it generate by pc tool--"OptekDspTools.exe"
	*************************************************************/
	//EQ Filter
	#if 1//bypass
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
	#else
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	0x26729d25,0xb31ac5b5,0x26729d25,0xbdbfc142,0x178a35d8,
	#endif
	
	//3D Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	
	//DRC & EBass Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,

	//EBass Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,


	//DRC Setting
	//DRC Mode
	0, //OPTEK_DRC_NORMAL:0

	//DRC Cutoff Freqency
	0,

	//DRC 1
	10485,0,10485,0,0,0,0,
	//DRC 2
	10485,0,10485,0,0,0,0,


	//3D Setting
	//Enable, Cutoff Freqency, Factor
	0,127,0,


	//EBASS Setting
	//Enable, LPF Freqency, HPF Freqency, Gain
	0,2097,100,0,

	/*********************Sound configurations end**********************/

	0xF0F0F0F0
};





const int subwoofer_dsp_setting[] =
{
	0xAAAAAAAA,
	0x0,	

	/*************************************************************
	Sound configurations,it generate by pc tool--"OptekDspTools.exe"
	*************************************************************/
	//EQ Filter

	//Biquad 1
	0x00018eb5,
	0x00031d6b,
	0x00018eb5,
	0x81c6ec2f,
	0x3e3f4ea9,

	//Biquad 2
	0x3fd29167,
	0x805add31,
	0x3fd29167,
	0x805afd72,
	0x3fa5430f,

	//Biquad 3
	0x4013b454,
	0x8099fd62,
	0x3f54181b,
	0x8099fd62,
	0x3f67cc6f,

//	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
//	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
//	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	//3D Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	//DRC & EBass Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,

	//EBass Filter
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x40000000,0x00000000,0x00000000,0x00000000,0x00000000,


	//DRC Setting
	//DRC Mode
	0,

	//DRC Cutoff Freqency
	0,

	//DRC 1
	10485,0,10485,0,0,0,0,
	//DRC 2
	10485,0,10485,0,0,3145728,0,


	//3D Setting
	//Enable, Cutoff Freqency, Factor
	0,127,0,


	//EBASS Setting
	//Enable, LPF Freqency, HPF Freqency, Gain
	0,2097,100,0,

	/*********************Sound configurations end**********************/

	0xF0F0F0F0
};

OPTEK_DSP_RXs optek_dsp_rx __attribute__ ((section (".d_dsp_bss")));

void optek_dsp_setting_init(void)
{
	DBG_Printf("%s\n\r",__func__);
	#ifdef DSPEQ_TUNING_FUNC_ENABLE
	memcpy(&optek_dsp_rx, dsp_setting, sizeof(optek_dsp_rx));
	
	optek_dsp_setting_updata(&optek_dsp_rx);
	#endif
}
#endif
#endif
#endif //DSPEQ_TUNING_ENABLE

