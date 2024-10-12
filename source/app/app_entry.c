#include "os_config.h"

#include "hw_audio_adc_dac.h"
#include "hw_spi.h"

#include "hal_uart.h"
#include "hal_dma.h"

#include "sdk_config.h"
#include "sdk_ver.h"
#include "system_timer.h"
#include "codec.h"
#include "app_source.h"
#include "app_window.h"
#include "console_task.h"
#include "key_scan.h"
#include "hw_pwm.h"

#ifdef DISPLAY_MODULE_ENABLE
#include "DisplayModule.h"
#endif

#ifdef I2S0_ENABLE
#include "app_i2c0.h"
#endif

#ifdef I2S1_ENABLE
#include "app_i2c1.h"
#endif

#ifdef I2S2_ENABLE
#include "app_i2c2.h"
#endif

#ifdef I2S3_ENABLE
#include "app_i2c3.h"
#endif

#ifdef SPI_ENABLE
#include "hw_spi.h"
#endif

#ifdef KEY_ENABLE
#include "key_scan.h"
#endif

#ifdef AUDIO_PROCESSOR
#include "audio_device.h"
#endif

#ifdef SAVE_MODULE_ENABLE
#include "app_save.h"
#endif

#ifdef DSP_SONA_MODULE_ENABLE
#include "sona_aef.h"
#include "sona_dsp.h"
#endif

const char app_ver_date[] = __DATE__;

const char app_time_date[] = __TIME__;

// const char app_version[] = "0.0.0924_22dbu_120W";

const char app_version[] = "0.0.1011_sona_with_101102_and_updata_class_H";

const char *app_version_get(void)
{
    return app_version;
}

const char *app_date_get(void)
{
    return app_ver_date;
}

const char *app_time_get(void)
{
    return app_time_date;
}


/**
 ****************************************************************************************
 * @brief Initialize dma module and printf module before starting schedule
 *
 ****************************************************************************************
 */
void app_PreTaskSch_init(void)
{
    hal_dma_module_init();

#ifdef UART0_USED_FOR_DEBUG
	hal_uart0_open(1, 1, UART0_BAUDRATE);
	printf_init(&uart0_api);
	console_cmd_init(&uart0_api);
	console_cmd_start();
#elif defined UART1_USED_FOR_DEBUG
	hal_uart1_open(1, 1, UART1_BAUDRATE);
	printf_init(&uart1_api);
	console_cmd_init(&uart1_api);
	console_cmd_start();
#elif defined UART2_USED_FOR_DEBUG
	hal_uart2_open(1, 1, UART2_BAUDRATE);
	printf_init(&uart2_api);
	console_cmd_init(&uart2_api);
	console_cmd_start();
#endif

	printf_info ("SDK version:%s\n\r", sdk_version_get());
	printf_info ("Buid time:%s %s\n\r", sdk_time_get(), sdk_date_get());

	printf_info ("App version:%s\n\r", app_version_get());
	printf_info ("Buid time:%s %s\n\r", app_time_get(), app_date_get());
}



/**
 ****************************************************************************************
 * @brief Initialize some function modules and enter default source
 * 
 * @return will not return
 *
 ****************************************************************************************
 */
void app_PostTaskSch_init(void)
{
	printf_info("%s\n\r", __func__);

	#ifdef DEBUG_GPIO_ENABLE
    debug_pin_init();
    #endif

	system_timer_data_init();

	#ifdef I2C0_ENABLE
	app_i2c0_init();
	#endif

	#ifdef I2C1_ENABLE
	app_i2c1_init();
	#endif

	#ifdef I2C2_ENABLE
	app_i2c2_init();
	#endif

	#ifdef I2C3_ENABLE
	app_i2c3_init();
	#endif

	#ifdef SPI_ENABLE
	hw_spi_init();
	#endif



	#if (defined AUX_INPUT_FROM_ADC || defined AUDIO_OUTPUT_FROM_DAC)
	hw_audio_adc_dac_clk_init();
	hw_audio_adc_dac_enable(TRUE);
    #ifdef AUX_INPUT_FROM_ADC
	hw_audio_adc_init(CLOSE_ALL_ADC_MARK);
    #endif
    #ifdef AUDIO_OUTPUT_FROM_DAC
	hw_audio_dac_init();
	#endif
	#endif

	#ifdef HDMI_ENABLE
	hdmi_process_init();
	#endif

	#ifdef DISPLAY_MODULE_ENABLE
	app_display_module.init();
	app_display_module.open();
	#endif

	upgrade_status_get();

	#ifdef REMOTE_ENABLE
	Remote_Init();
	#endif

	#ifdef KEY_ENABLE
	KeyScan_init();
	#endif

	#ifdef BATT_DETECT_ENABLE
	hal_key_adc_init();
	// hw_pwm_init();

	#endif
	#ifdef ROTARY_ENABLE
	#ifdef ROTARY_DET_USED_GPIO
	rotary_used_gpio_init();
	#endif //ROTARY_DET_USED_GPIO
	#endif //ROTARY_ENABLE    
	
	#ifdef HDMI_ENABLE
	hdmi_start_set(TRUE);
	#endif

    app_window_init();

	system_timer_init();
	
	#ifdef BTDM52_ENABLE
	app_bt_init();
	#endif

	#ifdef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
	app_save_system_data_init();
	#endif

	#ifdef AUDIO_PROCESSOR
	#ifndef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU
	audio_vari.volume = VOLUME_DEFAULT;
	#endif
	audio_vari.volume = VOLUME_DEFAULT;

	#ifdef AUDIO_OPTEK_DAPP
	AUDIOdevice.init();
	AUDIOdevice.open();
	#endif
	#ifdef AUDIO_AMP_DEV_ENABLE
	int i;
	for (i=0;i<AMP_DEV_COUNT_MAX;i++)
	{
		AudioAmpDev[i].init();
		AudioAmpDev[i].open();
		
	}
	app_task_timer_ms_set(TIMER_MUTE_OFF,3000);//3s
	#endif
	#endif
	#ifdef DSP_SONA_MODULE_ENABLE
    dsp_sona_module_init();
	#endif	
	sona_dsp_pc_tools_by_uart_flag_switch();
	
	#ifdef ARC_ENABLE 
	hw_cec_init();
	arc_dectect_pin_init();
	arc_dectect_status_init();
	optek_cec_init();
	optek_cec_open();
	#endif
		
	#if 1//ndef SAVE_MODULE_TO_SAVE_GLOBAL_VARI_STRU	
	global_vari.audio_source = DEFAULT_APP_SOURCE;
	#endif

	APP_SOURCE_STRU* pSrcFunction;

	if (global_vari.optek_link_role == OPL_ROLE_SLAVE)
    {
	    pSrcFunction = get_source_entry_by_source_id(SOURCE_OPTEKLINK_SLAVE);
    }
	else
    {
	    pSrcFunction = get_source_entry_by_source_id(global_vari.audio_source);
    }

	#ifdef SYSTEM_POWER_ON_TO_STANDBY_MODE
	global_vari.audio_source = pSrcFunction->id;
	printf_info("audio_source : %d\n\r", global_vari.audio_source);
	app_source_convert_to_standby_enter();
	#else
    global_vari.audio_source = SOURCE_UNKNOWN;
	if (pSrcFunction != NULL)
	{
        printf_info("pSrcFunction:%s;%d\n", pSrcFunction->string, global_vari.audio_source);
		pSrcFunction->switch_to_source();
	}
	else
	{
        printf_warn("pSrcFunction:%s;%d\n", pSrcFunction->string, global_vari.audio_source);
		printf_warn("default source entry not found\n");
	}
	#endif
}

