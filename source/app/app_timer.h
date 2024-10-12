#ifndef __APP_TIMER_H__
#define __APP_TIMER_H__

#include "hdmi_process.h"

#define	USB_MSC_MAX_TIMER    8

typedef struct {

	#ifdef USB_HOST_ENABLE
	U16	usbLUNtimerRun;
	U16	usbLUNtime;
	U16	usbMaxluns;
	U16	usbLUNtimer[USB_MSC_MAX_TIMER];
	U16	usbLunDetTimer;
	U8 usbOpenTimer;

	#ifdef FOR_ESD_PROTECT
	U16 esdProtectTimer;
	#endif
	
	#endif //USB_HOST_ENABLE

	#ifdef HDMI_ENABLE
	#ifdef HDMI_INT_CHECKTOUT_USED_TIMER
	U16 hdmi_int_finish_wait_timer;
	#endif
	#endif

	#ifdef ARC_ENABLE
	U16 arc_plug_timer;
	U16 arc_unplug_timer;
	#endif

#ifdef OPTEK_ARC_CEC_ENABLE
	U16 cec_init_start_timer;
	U16 cec_rejectcom_time;
	U16 cec_standby_wait_time;
	U16 cec_standby_time;
	U16 cec_unplug_delay_time;
	U16 cec_handle_enable_time;
	U16 cec_initiate_wait_time;
#endif

#ifdef ROTARY_ENABLE
	#ifdef ROTARY_DET_USED_GPIO
	U16 rotary_gpio_dir;
	U16 rotary_gpio_size;
	U16	rotary_gpio_timer;
	#endif

	#ifdef ROTARY_0_ENABLE
	U16 rotary0_dir;
	U16 rotary0_size;

	U16 rotary0_timer;
	#endif

	#ifdef ROTARY_1_ENABLE
	U16 rotary1_dir;
	U16 rotary1_size;

	U16 rotary1_timer;
	#endif
	
#endif

#ifdef SPI_DMA_RX_TEST
	U16 spi_dma_timer;
#endif

} APP_TIMER_DATA;

extern APP_TIMER_DATA app_timer_data;


void app_timer_5ms(U32 tick);



#ifdef HDMI_INT_CHECKTOUT_USED_TIMER
void app_timer_hdmi_int_finish_wait_time_set(U32 time);
void app_timer_hdmi_int_finish_wait_time_set_from_isr(U32 time);
void app_timer_hdmi_int_wait_time_clr(void);
U32 app_timer_hdmi_int_wait_time_get(void);
#endif

#ifdef ARC_PLUG_TIMER_ENABLE
void app_timer_arc_cec_plug_timer_set (U16 time);
void app_timer_arc_cec_plug_timer_clr (void);
U16 app_timer_arc_cec_plug_timer_get (void);
#endif //ARC_PLUG_TIMER_ENABLE

#ifdef ARC_UNPLUG_TIMER_ENABLE
void app_timer_arc_cec_unplug_timer_set (U16 time);
void app_timer_arc_cec_unplug_timer_clr (void);
U16 app_timer_arc_cec_unplug_timer_get (void);
#endif //ARC_UNPLUG_TIMER_ENABLE

#ifdef OPTEK_ARC_CEC_ENABLE
void app_timer_arc_cec_init_start_timer_set (U16 time);
void app_timer_arc_cec_init_start_timer_clr (void);
U16 app_timer_arc_cec_init_start_timer_get (void);

void app_timer_cec_connect_timeout_time_set (U16 time);
void app_timer_cec_connect_timeout_time_clr (void);
U16 app_timer_cec_connect_timeout_time_get(void);

void app_timer_cec_standby_wait_time_set (U16 time);
void app_timer_cec_standby_wait_time_clr (void);
U16 app_timer_cec_standby_wait_time_get (void);

void app_timer_cec_standby_time_set (U16 time);
void app_timer_cec_standby_time_clr (void);
U16 app_timer_cec_standby_time_get (void);

void app_timer_cec_unplug_delay_time_set (U16 time);
void app_timer_cec_unplug_delay_time_clr (void);
U16 app_timer_cec_unplug_delay_time_get (void);

void app_timer_cec_handle_enable_timr_set (U16 time);
void app_timer_cec_handle_enable_timr_clr (void);
U16 app_timer_cec_handle_enalbe_timr_get (void);
#endif //OPTEK_ARC_CEC_ENABLE

#ifdef ROTARY_ENABLE
void timer_rotary0_timer_set (U16 dir);
void timer_rotary0_timer_clr (void);

void timer_rotary1_timer_set (U16 dir);
void timer_rotary1_timer_clr (void);
#endif

void hal_adc_vol_convert(U32 tick);
void hal_batt_vol_detect(U32 tick);
void batt1_led(void);
void batt2_led(void);

#endif __APP_TIMER_H__
