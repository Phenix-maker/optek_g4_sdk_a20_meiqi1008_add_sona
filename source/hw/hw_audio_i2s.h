#ifndef __HW_AUDIO_I2S_H__
#define __HW_AUDIO_I2S_H__


#define AUDIO_MI2S_RX_ENABLE


enum {
 IIS_RX_DATA_16_BTT_WIDTH,
 IIS_RX_DATA_24_BTT_WIDTH,
};

enum {
	IIS_RX_SLAVE,
	IIS_RX_MASTER,
};


void hw_audio_mi2s_osr_div_set(U8 audio_type);
void hw_audio_mi2s_mclk_switch(U8 external_mclk);

void hw_audio_pllFrac_apwm_sel(U8 pllFrac_apwm_sel);
void hw_audio_d2ayn_sel(void);

void hw_audio_mi2s_rx_start(void);
void hw_audio_mi2s_rx_stop(void);

void hw_audio_mi2s_in_channel_set(U8 channel_nummber);
void hw_audio_mi2s_in_data_width_set(U8 data_width);
void hw_audio_mi2s_master_slave_switch(U8 mode);

void i2s_st_bck_lrck_sel(U8 value);
void mi2s_bck_lrck_sel_switch(U8 value);
void i2s_stereo_data_in_2_switch(U8 value, U8 input_channel, U8 output_channel);

#endif //__HW_AUDIO_I2S_H__
