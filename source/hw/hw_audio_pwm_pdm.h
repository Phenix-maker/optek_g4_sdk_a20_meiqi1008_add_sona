#ifndef __HW_AUDIO_PWM_PDM_H__
#define __HW_AUDIO_PWM_PDM_H__

//#define PDM_SDM128x						//for HD audio (can play at most 40k audio)
//#define PDM_SDM64x                        //(can play at most 20k audio)

#define OPTEK_D2ASYN		0				//optek patented Dynamic Digital Audio Sync*/

#if !(defined PDM_SDM64x || defined PDM_SDM128x)
//#define PWM_4096x_SDM16x					/*apwm clk is 196.608Mhz for sampling rate 48kHz x N (N = 1/2/4)*/
//#define PWM_4096x_SDM8x					/*apwm clk is 196.608Mhz for sampling rate 48kHz x N (N = 1/2/4)*/
//#define PWM_2048x_SDM16x					/*apwm clk is  96Mhz, PLLess  HDAUDIO (can play at most 40k audio) */
#define PWM_2048x_SDM8x					    /*apwm clk is  96Mhz, PLLess  (can play at most 20k audio)*/

#define SDM_O5								/*sigma delta order 5 or order 4*/
#endif


#endif //__HW_AUDIO_PWM_PDM_H__
