#ifndef __SDK_CONFIG_H__
#define __SDK_CONFIG_H__


// #define LDO_VOLTAGE_SEL_3P3V
#define LDO_VOLTAGE_SEL_2P5V

/*
* ADC module
* ADC_VOLTAGE_SEL_3P3V & ADC_VOLTAGE_SEL_1P8V setting only support one
* ADC module hw power is 3.3V,open ADC_VOLTAGE_SEL_3P3V
* ADC module hw power is 2.5V,open ADC_VOLTAGE_SEL_1P8V
* */
#define ADC_VOLTAGE_SEL_3P3V
//#define ADC_VOLTAGE_SEL_1P8V
/*
* DAC module
* DAC_VOLTAGE_SEL_3P3V & DAC_VOLTAGE_SEL_1P8V setting only support one
* DAC module hw power is 3.3V,open ADC_VOLTAGE_SEL_3P3V
* DAC module hw power is 2.5V,open ADC_VOLTAGE_SEL_1P8V 
* */
#define DAC_VOLTAGE_SEL_3P3V
//#define DAC_VOLTAGE_SEL_1P8V

/***********************************************
*******             IIS set             ********
***********************************************/
/*
* if AUDIO_INPUT_FROM_IIS_TDM open and HDMI mode, need to close IIS0_MASTER_ENABLE,open MCLK_IN_AS_AUDIO_OUT_CLK
* */
//#define IIS0_MASTER_ENABLE
#define IIS1_MASTER_ENABLE
// #define MCLK_IN_AS_AUDIO_OUT_CLK

/*
* IIS0_BCLK_LRCK_FROM_ST_IIS | IIS0_BCLK_LRCK_FROM_IIS0 | IIS0_BCLK_LRCK_FROM_IIS2,only support one,and must open one
* */
#define IIS0_BCLK_LRCK_FROM_IIS0
// #define IIS0_BCLK_LRCK_FROM_ST_IIS
// #define IIS0_BCLK_LRCK_FROM_IIS2

/*
* IIS1_BCLK_LRCK_FROM_ST_IIS | IIS1_BCLK_LRCK_FROM_IIS0 | IIS1_BCLK_LRCK_FROM_IIS2,only support one,and must open one
* */
#define IIS1_BCLK_LRCK_FROM_ST_IIS
// #define IIS1_BCLK_LRCK_FROM_IIS0
// #define IIS1_BCLK_LRCK_FROM_IIS2


#define IIS_MCLK_OUTPUT_GPIO0
//#define IIS_MCLK_OUTPUT_GPIO9
// #define IIS_MCLK_INTPUT_GPIO17


/***********************************************
*******        AUDIO INPUT set          ********
***********************************************/
#define AUDIO_IN_STREAM_USED

#ifdef AUDIO_IN_STREAM_USED
//#define AUX_IN_STREAM_USED
#endif

/*
*AUDIO_OUTPUT_FROM_IIS1 & AUDIO_INPUT_FROM_ADC & (AUDIO_INPUT_FROM_IIS0 | AUDIO_INPUT_FROM_IIS_TDM) if open one DMA only support one
*AUDIO_INPUT_FROM_IIS0 & AUDIO_INPUT_FROM_IIS_TDM hardware only support one
*/
// #define AUDIO_INPUT_FROM_IIS0
// #define AUDIO_INPUT_FROM_IIS1
// #define AUDIO_INPUT_FROM_IIS_TDM
#define AUDIO_INPUT_FROM_ADC


/*
*AUX_INPUT_FROM_IIS0 & AUX_INPUT_FROM_IIS_TDM hardware only support one
*AUX_OUTPUT_FROM_IIS1 & AUX_INPUT_FROM_ADC & (AUX_INPUT_FROM_IIS0 | AUX_INPUT_FROM_IIS_TDM), if open one DMA only support one
*/
// #define AUX_INPUT_FROM_IIS0
// #define AUX_INPUT_FROM_IIS1
// #define AUX_INPUT_FROM_IIS_TDM
#define AUX_INPUT_FROM_ADC


/*
*AUDIO_OUTPUT_FROM_IIS1 & (AUDIO_OUTPUT_FROM_DAC | AUDIO_OUTPUT_FROM_IIS2 | AUDIO_OUTPUT_FROM_PWM) if open one DMA only support one
*AUDIO_OUTPUT_FROM_IIS2 & AUDIO_OUTPUT_FROM_PWM hardware only support one
*/
#define AUDIO_OUTPUT_FROM_DAC

// #define AUDIO_OUTPUT_FROM_IIS1
#define AUDIO_OUTPUT_FROM_IIS2
// #define AUDIO_OUTPUT_FROM_PWM
// #define AUDIO_OUTPUT_FROM_SPDIF

#define ADC_DAC_DATA_WIDTH_24_BIT
// #define ADC_DAC_DATA_WIDTH_16_BIT

#ifdef AUDIO_INPUT_FROM_IIS_TDM
#define I2S_TDM_DATA_WIDTH_24_BIT
// #define I2S_TDM_DATA_WIDTH_16_BIT
#endif


/*
* IIS_INPUT_CHANNEL_BIT_NUM SUPPORT
* 16
* 24
* */
#ifdef HDMI_ENABLE
//rx 16-bit data at hdmi source
#define IIS_INPUT_CHANNEL_BIT_NUM     16
#else
#if(defined (I2S_TDM_DATA_WIDTH_24_BIT) || defined ADC_DAC_DATA_WIDTH_24_BIT)
#define IIS_INPUT_CHANNEL_BIT_NUM     24
#else
#define IIS_INPUT_CHANNEL_BIT_NUM     16
#endif
#endif

/*
* IIS_OUTPUT_CHANNEL_BIT_NUM SUPPORT
* 16
* 24
* */
#if (defined (I2S_TDM_DATA_WIDTH_24_BIT) || defined ADC_DAC_DATA_WIDTH_24_BIT)
#define IIS_OUTPUT_CHANNEL_BIT_NUM    24
#else
#define IIS_OUTPUT_CHANNEL_BIT_NUM    16
#endif
#define AUDIO_IN_STREAM_SAMPLE_SPEED    1
#define AUDIO_IN_STREAM_CHANNEL    2

#if ((defined AUX_INPUT_FROM_ADC)||(defined AUDIO_INPUT_FROM_ADC))

/*
* ADC_A_ANALOG_ENABLE and ADC_A_DMIC_ENABLE only support define one
* ADC_B_ANALOG_ENABLE and ADC_B_DMIC_ENABLE only support define one
* (ADC_A_ANALOG_ENABLE or ADC_A_DMIC_ENABLE) and (ADC_B_ANALOG_ENABLE or ADC_B_DMIC_ENABLE) and ADC_C_DMIC_ENABLE and ADC_D_DMIC_ENABLE max support 4
* ADC_A_ANALOG_ENABLE
* ADC_A_DMIC_ENABLE
* ADC_B_ANALOG_ENABLE
* ADC_B_DMIC_ENABLE
* ADC_C_DMIC_ENABLE
* ADC_D_DMIC_ENABLE
* */
#define ADC_A_ANALOG_ENABLE
// #define ADC_A_DMIC_ENABLE
// #define ADC_B_ANALOG_ENABLE
// #define ADC_B_DMIC_ENABLE
// #define ADC_C_DMIC_ENABLE
// #define ADC_D_DMIC_ENABLE

/*
* AUX_MODE_INPUT_FROM_ADC_A_ANALOG and AUX_MODE_INPUT_FROM_ADC_A_DMIC only support define one
* AUX_MODE_INPUT_FROM_ADC_B_ANALOG and AUX_MODE_INPUT_FROM_ADC_B_DMIC only support define one
* (AUX_MODE_INPUT_FROM_ADC_A_ANALOG or AUX_MODE_INPUT_FROM_ADC_A_DMIC) and (AUX_MODE_INPUT_FROM_ADC_B_ANALOG or AUX_MODE_INPUT_FROM_ADC_B_DMIC) and AUX_MODE_INPUT_FROM_ADC_C_DMIC and AUX_MODE_INPUT_FROM_ADC_D_DMIC max support 4
* AUX_MODE_INPUT_FROM_ADC_A_ANALOG
* AUX_MODE_INPUT_FROM_ADC_A_DMIC
* AUX_MODE_INPUT_FROM_ADC_B_ANALOG
* AUX_MODE_INPUT_FROM_ADC_B_DMIC
* AUX_MODE_INPUT_FROM_ADC_C_DMIC
* AUX_MODE_INPUT_FROM_ADC_D_DMIC
* */
#define AUX_MODE_INPUT_FROM_ADC_A_ANALOG
// #define AUX_MODE_INPUT_FROM_ADC_A_DMIC
// #define AUX_MODE_INPUT_FROM_ADC_B_ANALOG
// #define AUX_MODE_INPUT_FROM_ADC_B_DMIC
// #define AUX_MODE_INPUT_FROM_ADC_C_DMIC
// #define AUX_MODE_INPUT_FROM_ADC_D_DMIC

/*
* USB_DEV_INPUT_FROM_ADC_A_ANALOG and USB_DEV_MIC_INPUT_FROM_ADC_A_DMIC only support define one
* USB_DEV_MIC_INPUT_FROM_ADC_B_ANALOG and USB_DEV_MIC_INPUT_FROM_ADC_B_DMIC only support define one
* (USB_DEV_MIC_INPUT_FROM_ADC_A_ANALOG or USB_DEV_MIC_INPUT_FROM_ADC_A_DMIC) and (USB_DEV_MIC_INPUT_FROM_ADC_B_ANALOG or USB_DEV_MIC_INPUT_FROM_ADC_B_DMIC) and USB_DEV_MIC_INPUT_FROM_ADC_C_DMIC and USB_DEV_MIC_INPUT_FROM_ADC_D_DMIC max support 4
* USB_DEV_MIC_INPUT_FROM_ADC_A_ANALOG
* USB_DEV_MIC_INPUT_FROM_ADC_A_DMIC
* USB_DEV_MIC_INPUT_FROM_ADC_B_ANALOG
* USB_DEV_MIC_INPUT_FROM_ADC_B_DMIC
* USB_DEV_MIC_INPUT_FROM_ADC_C_DMIC
* USB_DEV_MIC_INPUT_FROM_ADC_D_DMIC
* */
#define USB_DEV_MIC_INPUT_FROM_ADC_A_ANALOG
// #define USB_DEV_MIC_INPUT_FROM_ADC_A_DMIC
// #define USB_DEV_MIC_INPUT_FROM_ADC_B_ANALOG
// #define USB_DEV_MIC_INPUT_FROM_ADC_B_DMIC
// #define USB_DEV_MIC_INPUT_FROM_ADC_C_DMIC
// #define USB_DEV_MIC_INPUT_FROM_ADC_D_DMIC

#endif







//It is unfixed, it changes with the input source, the default below.
/*
* AUDIO_IN_STREAM_CHANNEL SUPPORT
* 2
* 4
* 6
* 8
* */
/*
* AUDIO_INTPUT_FROM_ADC support MAX 8ch input
* AUDIO_INPUT_FROM_IIS0 & AUX_INPUT_FROM_IIS0 support MAX 8ch input
* AUDIO_INPUT_FROM_IIS1 & AUX_INPUT_FROM_IIS1 support MAX 4ch input,if used input 4ch,must used hw pin GPIO_20&GPIO_23 to 2 input pin
* AUX_INPUT_FROM_IIS_TDM Hardware support MAX 4ch input
* AUDIO_OPTEK_DAPP open Hardware support MAX 2ch input
* */

#define AUDIO_IN_STREAM_CHANNEL    2


#define AUDIO_OUT_CLK_SET_FIXED

// #define AUDIO_OUT_CHANNEL_SET_FIXED


//It is fixed by hardware.
/*
* AUDIO_OUT_STREAM_CHANNEL SUPPORT
* 2
* 4
* 6
* 8
* 10
* 12
* 14
* 16
* */
/*
* DOLBY_AC3_DEC_ENABLE support MAX 6ch output
* DOLBY_DDPLUS71_DEC_ENABLE support MAX 8ch output
* AUDIO_OUTPUT_FROM_PWM support MAX 4ch output
* AUDIO_OUTPUT_FROM_IIS1 support MAX 4ch output,if used input 4ch,must used hw pin GPIO_20&GPIO_23 to 2 input pin
* AUDIO_OUTPUT_FROM_IIS2 support MAX 16ch output
* AUX_INPUT_FROM_IIS_TDM Hardware support MAX 16ch output
* USB_DEV_ENABLE & USB_DEV_UPDATA_MIC_ENABLE support MAX 2ch output??
* */
// #define AUDIO_OUT_STREAM_CHANNEL    2
// #endif

#define AUDIO_OUT_STREAM_CHANNEL            4//2
#define AUDIO_OUT_STREAM_SAMPLE_SPEED       1

/*
0(2'b00) IIS OUT data 0
1(2'b01) IIS OUT data 1
2(2'b10) IIS OUT data 2
3(2'b11) IIS OUT data 3
*/
#define DAC_AUDIO_OUT_FROM_IIS_OUT_DATA_CH          1

/*
 * 1X (32000 44100 48000)
 * 2X (32000*2 44100*2 48000*2)
 * 4X (32000*4 44100*4 48000*4)
*/
#define AUDIO_IN_STREAM_SAMPLE_RATE    48000
#define AUDIO_IN_STREAM_SAMPLE_SPEED    1



#define AUDIO_INPUT_RX_DMA_CLOSE


#define OPTICAL_SPDIF_IN_CHANNEL_SEL_0
//#define OPTICAL_SPDIF_IN_CHANNEL_SEL_1

#define COAXIAL_SPDIF_IN_CHANNEL_SEL_0
//#define COAXIAL_SPDIF_IN_CHANNEL_SEL_1

#define ARC_SPDIF_IN_CHANNEL_SEL_0
//#define ARC_SPDIF_IN_CHANNEL_SEL_1


/***********************************************
*******        KEY FUNCTION SET       ********
***********************************************/
#define KEY_ENABLE
#define KEY_GPIO_ENABLE

//#define KEY_SREG_ENABLE
#ifdef KEY_SREG_ENABLE
#define POWER_KEY_ENABLE
#endif

// #define KEY_ADC_ENABLE
#ifdef KEY_ADC_ENABLE
/*
* KEY_ADC_CHANNEL_MAX support max setting 2
* 1
* 2
* */
#define KEY_ADC_CHANNEL_MAX    2
#define HW_SARADC_ENABLE
#endif

#define KEY_GPIO_NUMBER_MAX    2
#define KEY_ADC_NUMBER_MAX     2

/*
* MAX_SIMULT_KEY mean how many button key support when Press button at the same time
* 
* */
#define MAX_SIMULT_KEY         4   


//#define SOFT_INT_6_USED
//#define SOFT_INT_7_USED

/***********************************************
*******     ROTARY KEY FUNCTION SET     ********
***********************************************/
//#define ROTARY_ENABLE
#ifdef ROTARY_ENABLE
#define ROTARY_DET_USED_GPIO
#define ROTARY_DET_USED_HW

#ifdef ROTARY_DET_USED_GPIO
//#define ROTARY_NUM_2
#endif
#endif

/***********************************************
*******     REMOTE KEY FUNCTION SET     ********
***********************************************/
//#define REMOTE_ENABLE

#ifdef REMOTE_ENABLE
/* As Sharp remoter is the same as Panasonic
 * for Sharp Audio
 * #define REMOTER_SHARP_AUDIO_FORMAT
 *  
 * As Panasonic remoter is the same as Sharp
 * #define REMOTER_PANASONIC_FORMAT
 */
#define REMOTE_NEC_FORMAT
//#define REMOTER_SHARP_AUDIO_FORMAT
//#define REMOTER_PANASONIC_FORMAT
//#define REMOTE_PHILIPS_FORMAT
//#define REMOTE_MIXED_FORMAT
//#define REMOTE_XIAOMI_FORMAT
#endif

/***********************************************
*******     ADC DETECT SET     ********
***********************************************/
#define BATT_DETECT_ENABLE
#ifdef BATT_DETECT_ENABLE
#endif

#endif //__SDK_CONFIG_H__
