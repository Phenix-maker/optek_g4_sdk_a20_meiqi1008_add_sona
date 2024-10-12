#ifndef __HW_AUDIO_ADC_DAC_H__
#define __HW_AUDIO_ADC_DAC_H__
#include "regmap.h"


#define ADC_A_ANALOG_MARK               (U8)(1<<0)
#define ADC_A_DMIC_MARK                 (U8)(1<<1)
#define ADC_B_ANALOG_MARK               (U8)(1<<2)
#define ADC_B_DMIC_MARK                 (U8)(1<<3)
#define ADC_C_DMIC_MARK                 (U8)(1<<4)
#define ADC_D_DMIC_MARK                 (U8)(1<<5)

#define CLOSE_ALL_ADC_MARK              (U8)(0<<0)

/*
* if defined AUX_MODE_INPUT_FROM_ADC_A_ANALOG   support ADC_A_ANALOG_MARK
* if defined AUX_MODE_INPUT_FROM_ADC_A_DMIC     support ADC_A_DMIC_MARK
* if defined AUX_MODE_INPUT_FROM_ADC_B_ANALOG   support ADC_B_ANALOG_MARK
* if defined AUX_MODE_INPUT_FROM_ADC_B_DMIC     support ADC_B_DMIC_MARK
* if defined AUX_MODE_INPUT_FROM_ADC_C_DMIC     support ADC_C_DMIC_MARK
* if defined AUX_MODE_INPUT_FROM_ADC_D_DMIC     support ADC_D_DMIC_MARK
* */
#define AUX_MODE_INPUT_ADC_MARK         (ADC_A_ANALOG_MARK)
/*
* AUX_MODE_INPUT_ADC_MARK support (1/2/3/4) mark ,ch support (2/4/6/8) channel
* */
#define AUX_MODE_INPUT_ADC_CH           2

/*
* if defined USB_DEV_MIC_INPUT_FROM_ADC_A_ANALOG   support ADC_A_ANALOG_MARK
* if defined USB_DEV_MIC_INPUT_FROM_ADC_A_DMIC     support ADC_A_DMIC_MARK
* if defined USB_DEV_MIC_INPUT_FROM_ADC_B_ANALOG   support ADC_B_ANALOG_MARK
* if defined USB_DEV_MIC_INPUT_FROM_ADC_B_DMIC     support ADC_B_DMIC_MARK
* if defined USB_DEV_MIC_INPUT_FROM_ADC_C_DMIC     support ADC_C_DMIC_MARK
* if defined USB_DEV_MIC_INPUT_FROM_ADC_D_DMIC     support ADC_D_DMIC_MARK
* */
#define USB_DEV_MIC_INPUT_ADC_MARK      (ADC_A_ANALOG_MARK)
/*
* USB_DEV_MIC_INPUT_ADC_MARK support (1/2/3/4) mark ,ch support (2/4/6/8) channel
* */
#define USB_DEV_MIC_INPUT_ADC_CH        2

typedef volatile struct {
	U8	adc_a_analog_en 	: 1;
    U8	adc_a_dmic_en 		: 1;
    U8	adc_b_analog_en 	: 1;
    U8	adc_b_dmic_en 		: 1;
    U8	adc_c_dmic_en 		: 1;
    U8	adc_d_dmic_en 		: 1;
} ADC_MARK_SET;

void hw_audio_adc_dac_clk_init(void);
void hw_audio_adc_dac_enable(U8 enable);

void hw_audio_adc_init(U8 adc_en_mark);

void hw_audio_dac_init(void);
void hw_audio_adc_dac_enable(u8 enable);
void dac_write(int addr, int val);

#endif /* __HW_AUDIO_ADC_DAC_H__ */
