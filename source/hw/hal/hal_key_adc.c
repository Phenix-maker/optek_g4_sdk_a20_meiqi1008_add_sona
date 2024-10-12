#include "os_config.h"

#include "regmap.h"

#include "hw_saradc.h"

#include "hal_key_adc.h"

#include "app_key.h"
#include "key_scan.h"
#include "audio_device.h"
#include "hw_pwm.h"

#ifdef BATT_DETECT_ENABLE
static U8 cur_r_1 = FALSE,cur_g_1 = FALSE;
static U8 cur_r_2 = FALSE,cur_g_2 = FALSE;


#define Val_TOLERANCE 	0x09//0X0D
#define BATT_TOLERANCE 	0x09
#define BATT_MAX		100
U16 batt_buf,val;
U16 buf[2];
static U8 flag_charge = FALSE;
static U8 flag_charge1 = FALSE;
/*
* */
static const U16  adc_vol_table[][2] = {
	{(0x0				 ), (0x0+ Val_TOLERANCE + Val_TOLERANCE)},
	{(0x21 				 ), (0x21 + Val_TOLERANCE )}, 
	{(0x42 - Val_TOLERANCE ), (0x42  + Val_TOLERANCE )}, 
	{(0x63 - Val_TOLERANCE ), (0x63 + Val_TOLERANCE )}, 
	{(0x84 - Val_TOLERANCE ), (0x84 + Val_TOLERANCE )}, 
	{(0xA6 - Val_TOLERANCE ), (0xA6 + Val_TOLERANCE )}, 
	{(0xC7 - Val_TOLERANCE ), (0xC7 + Val_TOLERANCE )}, 
	{(0xE8 - Val_TOLERANCE ), (0xE8 + Val_TOLERANCE )}, 
	{(0x109 - Val_TOLERANCE ), (0x109 + Val_TOLERANCE )}, 
	{(0x12A - Val_TOLERANCE ), (0x12A + Val_TOLERANCE )}, 
	{(0x14C - Val_TOLERANCE ), (0x14C + Val_TOLERANCE )}, 
	{(0x16D - Val_TOLERANCE ), (0x16D + Val_TOLERANCE )}, 
	{(0x18E - Val_TOLERANCE ), (0x18E + Val_TOLERANCE )}, 
	{(0x1AF - Val_TOLERANCE ), (0x1AF + Val_TOLERANCE )}, 
	{(0x1D0 - Val_TOLERANCE ), (0x1D0 + Val_TOLERANCE )}, 
	{(0x1F2 - Val_TOLERANCE ), (0x1F2 + Val_TOLERANCE )}, 
	{(0x213 - Val_TOLERANCE ), (0x213 + Val_TOLERANCE )}, 
	{(0x234 - Val_TOLERANCE ), (0x234 + Val_TOLERANCE )}, 
	{(0x255 - Val_TOLERANCE ), (0x255 + Val_TOLERANCE )}, 
	{(0x276 - Val_TOLERANCE ), (0x276 + Val_TOLERANCE )}, 
	{(0x298 - Val_TOLERANCE ), (0x298 + Val_TOLERANCE )}, 
	{(0x2B9 - Val_TOLERANCE ), (0x2B9 + Val_TOLERANCE )}, 
	{(0x2DA - Val_TOLERANCE ), (0x2DA + Val_TOLERANCE )}, 
	{(0x2FB - Val_TOLERANCE ), (0x2FB + Val_TOLERANCE )}, 
	{(0x31C - Val_TOLERANCE ), (0x31C + Val_TOLERANCE )}, 
	{(0x33E - Val_TOLERANCE ), (0x33E + Val_TOLERANCE )}, 
	{(0x35F - Val_TOLERANCE ), (0x35F + Val_TOLERANCE )}, 
	{(0x380 - Val_TOLERANCE ), (0x380 + Val_TOLERANCE )}, 
	{(0x3A1 - Val_TOLERANCE ), (0x3A1 + Val_TOLERANCE )}, 
	{(0x3C2 - Val_TOLERANCE ), (0x3C2 + Val_TOLERANCE )}, 
	{(0x3E4 - Val_TOLERANCE ), (0x3FF )}, 
	// {(0x3C1 - Val_TOLERANCE ), (0x3FF )}, 

};
static const U16 adc_batt_table[][2] ={

	{(0x0), 					(0x24B - BATT_TOLERANCE )}, //< 12.4V == 5%
	{(0x24B - BATT_TOLERANCE ), (0x24B + BATT_TOLERANCE )}, //12V - 13.64V < 5%
	{(0x286 - BATT_TOLERANCE ), (0x297 - BATT_TOLERANCE )}, //13.64V - 14V < 20%
	{(0x297	- BATT_TOLERANCE ), (0x3FF )},  //> 14V  20% - 100%
	
};
enum
{
 enBATT_NORMAL 	= 3,
 enBATT_CHARGE 	= 2,
 enBATT_ALARM  	= 1,
 enBATT_OFF		= 0,

};


void batt1_led(void)
{
	static U8 bak = TRUE,count = 0;
	U8 tmp;
	if(hal_gpio_input_level_read(AC_INT_DET) == GPIO_LOW && audio_vari.batt_vol != enBATT_OFF )
	{
		#if 0
		if(audio_vari.batt_vol  != enBATT_NORMAL && audio_vari.volume <= 20) 
		{
			hal_gpio_output_level_set(BAT1_LED_G,GPIO_HIGH);
			hal_gpio_output_level_set(BAT1_LED_R,state);
			state =!state;
			DBG_Printf("charging_1\r\n");

		}
		else if(audio_vari.batt_vol == enBATT_NORMAL && hal_gpio_input_level_read(BAT1_LED) == GPIO_LOW )
		{
			hal_gpio_output_level_set(BAT1_LED_R,GPIO_HIGH);
			hal_gpio_output_level_set(BAT1_LED_G,GPIO_LOW);
			DBG_Printf("charge1_finish\r\n");

		}

		#else
		tmp = hal_gpio_input_level_read(BAT1_LED);
		if(tmp != bak && audio_vari.volume <= 20)
		{
			hal_gpio_output_level_set(BAT1_LED_G,tmp);
			hal_gpio_output_level_set(BAT1_LED_R,tmp);
			bak = tmp;
			// DBG_Printf("charging_1\r\n");
			count = 0;

		}
		else if(tmp == bak && tmp == GPIO_LOW)
		{
			count++;
			if(count > 10)
			{
				hal_gpio_output_level_set(BAT1_LED_R,GPIO_HIGH);
				hal_gpio_output_level_set(BAT1_LED_G,GPIO_LOW);
				// DBG_Printf("charge1_finish\r\n");
				count = 0;
			}
			// DBG_Printf("filter 1\r\n");
		}
		else
		{
			// DBG_Printf("batt1_err\r\n");
			count = 0;

		}
		
		#endif
	}
	else if(hal_gpio_input_level_read(AC_INT_DET) == GPIO_HIGH)
	{
		switch(audio_vari.batt_vol)
		{
			case enBATT_OFF:
				hal_gpio_output_level_set(BAT1_LED_G,GPIO_HIGH);
				hal_gpio_output_level_set(BAT1_LED_R,GPIO_HIGH);		
				// DBG_Printf("1_ac_off\r\n");
				break;
			case enBATT_ALARM:
				hal_gpio_output_level_set(BAT1_LED_G,GPIO_HIGH);
				hal_gpio_output_level_set(BAT1_LED_R,GPIO_LOW);
				// DBG_Printf("1_ac_<%5\r\n");	
				break;
			case enBATT_CHARGE:
				hal_gpio_output_level_set(BAT1_LED_G,GPIO_LOW);
				hal_gpio_output_level_set(BAT1_LED_R,GPIO_LOW);
				// DBG_Printf("1_ac_%5-%20\r\n");	
				break;
			case enBATT_NORMAL:
				hal_gpio_output_level_set(BAT1_LED_G,GPIO_LOW);
				hal_gpio_output_level_set(BAT1_LED_R,GPIO_HIGH);
				// DBG_Printf("1_ac_>%20\r\n");	
				break;
		}
	}
	else
	{
		hal_gpio_output_level_set(BAT1_LED_G,GPIO_HIGH);
		hal_gpio_output_level_set(BAT1_LED_R,GPIO_HIGH);		
		// DBG_Printf("1_err\r\n");
	}
}

void batt2_led(void)
{
	static U8 bak2 = TRUE,count1 = 0;
	U8 tmp;
	if(hal_gpio_input_level_read(AC_INT_DET) == GPIO_LOW && audio_vari.batt_vol1 != enBATT_OFF)
	{
		#if 0
		if(audio_vari.batt_vol1  != enBATT_NORMAL && audio_vari.volume <= 20) 
		{
			hal_gpio_output_level_set(BAT2_LED_G,GPIO_HIGH);
			hal_gpio_output_level_set(BAT2_LED_R,state_2);
			state_2 = !state_2;
			DBG_Printf("charging_2\r\n");
		}
		else if(audio_vari.batt_vol1 == enBATT_NORMAL && hal_gpio_input_level_read(BAT2_LED) == GPIO_LOW )
		{
			hal_gpio_output_level_set(BAT2_LED_R,GPIO_HIGH);
			hal_gpio_output_level_set(BAT2_LED_G,GPIO_LOW);
			DBG_Printf("charge2_finish\r\n");
		}

		#else
		tmp = hal_gpio_input_level_read(BAT2_LED);
		if(tmp != bak2 && audio_vari.volume <= 20)
		{
			hal_gpio_output_level_set(BAT2_LED_G,tmp);
			hal_gpio_output_level_set(BAT2_LED_R,tmp);
			bak2 = tmp;
			// DBG_Printf("charging_2\r\n");
			count1 = 0;
			
		}
		else if(tmp == bak2 && tmp == GPIO_LOW)
		{
			count1++;
			if(count1 > 10)
			{
				hal_gpio_output_level_set(BAT2_LED_R,GPIO_HIGH);
				hal_gpio_output_level_set(BAT2_LED_G,GPIO_LOW);
				// DBG_Printf("charge2_finish\r\n");
				count1 = 0;
			}
			// DBG_Printf("filter 1\r\n");

		}
		else
		{
			// DBG_Printf("batt2_err\r\n");
			count1 = 0;

		}
		#endif
	}
	else if(hal_gpio_input_level_read(AC_INT_DET) == GPIO_HIGH )
	{
		switch(audio_vari.batt_vol1)
		{
			case enBATT_OFF:
				hal_gpio_output_level_set(BAT2_LED_G,GPIO_HIGH);
				hal_gpio_output_level_set(BAT2_LED_R,GPIO_HIGH);	
				// DBG_Printf("2_ac_off\r\n");	
				break;
			case enBATT_ALARM:
				hal_gpio_output_level_set(BAT2_LED_G,GPIO_HIGH);
				hal_gpio_output_level_set(BAT2_LED_R,GPIO_LOW);
				// DBG_Printf("2_ac_<%5\r\n");	
				break;
			case enBATT_CHARGE:
				hal_gpio_output_level_set(BAT2_LED_G,GPIO_LOW);
				hal_gpio_output_level_set(BAT2_LED_R,GPIO_LOW);
				// DBG_Printf("2_ac_%5-%20\r\n");	
				break;
			case enBATT_NORMAL:
				hal_gpio_output_level_set(BAT2_LED_G,GPIO_LOW);
				hal_gpio_output_level_set(BAT2_LED_R,GPIO_HIGH);
				// DBG_Printf("2_ac_>%20\r\n");	
				break;
		}
	}
	else
	{
		hal_gpio_output_level_set(BAT2_LED_G,GPIO_HIGH);
		hal_gpio_output_level_set(BAT2_LED_R,GPIO_HIGH);	
		// DBG_Printf("2_err\r\n");	
	}
}



static int8_t switch_flag = TRUE;

void hal_adc_vol_convert(U32 tick)
{ 
	
		// DBG_Printf("%s\r\n",__func__);
		static U8 vol = 0x00;
		U32 ret_fault;
		// static U16 val_bak = 0,vol_bak = 0;
		val = 0;
		hw_saradc_twoAdKey_read(buf,AD_CH_0,AD_CH_1,FALSE,NULL);
		val = buf[0];

		// ret_fault = hal_gpio_input_level_read(GPIO0_04_INDEX);

		#if 0
		if((val_bak -Val_TOLERANCE ) < val  && val  < (val_bak + Val_TOLERANCE ))
		{
			return;
		}
		#endif
		// val_bak = val;

		#if 0
			vol = (U8)((val*VOLUME_MAX)/0x3FF);
		#else
			for(U8 i = 0;i<31;i++)
			{
				if( val > adc_vol_table[i][0] && val <= adc_vol_table[i][1] )
				{
					vol = i;
					// DBG_Printf("val_adc = %x\r\n",val);
				}
			}
		#endif

		if(audio_vari.volume != vol )
		{
			// DBG_Printf("vol_bak = %d,vol = %d\r\n",vol_bak,vol);
			#ifdef AUDIO_AMP_DEV_ENABLE
			func_w_one_param_msgSend(AudioAmpDev[0].Set_Volume,vol);
			audio_vari.volume = vol;
			#endif
			if(audio_vari.volume <= 10)
			{
				hal_gpio_output_level_set(I_CTL,GPIO_LOW);
				// func_w_one_param_msgSend(hw_pwm_set,0xfff);
				// hw_pwm_set(0xfff); //0v
			}
			else if(audio_vari.volume  <= 20)
			{
				hal_gpio_output_level_set(I_CTL,GPIO_LOW);
				// hw_pwm_set(0x6ff); //1.8v
				// func_w_one_param_msgSend(hw_pwm_set,0x6ff);
			
			}
			else if(audio_vari.volume <= 31)
			{
				hal_gpio_output_level_set(I_CTL,GPIO_HIGH);
				// hw_pwm_set(0x1); //3.3v
				// func_w_one_param_msgSend(hw_pwm_set,0x1);

			}

		}
}

void hal_batt_vol_detect(U32 tick)
{
		U16 batt_val = 0;
	 	U8 batt_vol = 0;
		static int8_t batt1_vol_bak = 0,batt2_vol_bak = 0;
		batt_val = buf[1];
		static U8 cnt = 0;

		// DBG_Printf("batt_adc = 0x%x\r\n",batt_val);
		// batt_val = (batt_val - BATT_VOL_MIN) ? (batt_val - BATT_VOL_MIN) :(BATT_VOL_MIN - batt_val);
		#if 1
			for(U8 i = 0;i<4;i++)
			{
				if( batt_val > adc_batt_table[i][0] && batt_val <= adc_batt_table[i][1] )
				{
					batt_vol = i;
					// DBG_Printf("val_adc = %x\r\n",val);
				}
			}

		if(switch_flag)
		{
			if(batt1_vol_bak != batt_vol)
			{
				audio_vari.batt_vol = batt_vol;
				batt1_vol_bak = batt_vol;
			}
			// DBG_Printf("batt1_vol = %d\r\n",batt_vol);

			#if 1
			if(cnt > 3)
			{
				switch_flag = FALSE;
				cnt = 0;
				hal_gpio_output_level_set(SW_BATT,GPIO_HIGH);
				// DBG_printf("batt1 detecting\r\n");				
			}
			else
			{
				cnt++;
			}
			#endif
		}
		else
		{

			if(batt2_vol_bak != batt_vol)
			{
				audio_vari.batt_vol1 = batt_vol;
				batt2_vol_bak = batt_vol;
			}
			// DBG_Printf("batt2_vol = %d\r\n",batt_vol);		

			#if 1
			if(cnt > 3)
			{
				switch_flag = TRUE;
				cnt = 0;
				hal_gpio_output_level_set(SW_BATT,GPIO_LOW);
				// DBG_printf("batt2 detecting\r\n");				
			}
			else
			{
				cnt++;
			}
			#endif
		}
		#endif

}
#endif


void hal_key_adc_init(void)
{
	#ifdef HW_SARADC_ENABLE
	hw_sadc_init();
	#endif

	#ifdef BATT_DETECT_ENABLE
	hw_sadc_init();

	//BATT management setting
	hal_gpio_mux_func_select(SW_BATT,MUX_SEL_GPIO_OUTPUT);
	hal_gpio_mux_func_select(I_CTL,MUX_SEL_GPIO_OUTPUT);
	//BATT led1 control setting
	hal_gpio_mux_func_select(BAT1_LED_R,MUX_SEL_GPIO_OUTPUT);
	hal_gpio_mux_func_select(BAT1_LED_G,MUX_SEL_GPIO_OUTPUT);
	//BATT led2 control setting
	hal_gpio_mux_func_select(BAT2_LED_R,MUX_SEL_GPIO_OUTPUT);
	hal_gpio_mux_func_select(BAT2_LED_G,MUX_SEL_GPIO_OUTPUT);
	//BATT status detect setting
	hal_gpio_mux_func_select(BAT1_LED,MUX_SEL_GPIO_INPUT);
	hal_gpio_mux_func_select(BAT2_LED,MUX_SEL_GPIO_INPUT);
	// bt pair led setting
	hal_gpio_mux_func_select(GPIO0_10_INDEX,MUX_SEL_GPIO_OUTPUT); // bt_led

	hal_gpio_mux_func_select(AC_INT_DET,MUX_SEL_GPIO_INPUT);
	hal_gpio_mux_func_select(PVDD_AC_EN,MUX_SEL_GPIO_OUTPUT);
	hal_gpio_mux_func_select(MUTE_CLT,MUX_SEL_GPIO_OUTPUT);



	//sw for setting
	hal_gpio_mux_func_select(HPM_SW,MUX_SEL_GPIO_INPUT);
	hal_gpio_mux_func_select(PHASE_SW,MUX_SEL_GPIO_INPUT);
	hal_gpio_mux_func_select(F_SW,MUX_SEL_GPIO_INPUT);
	hal_gpio_mux_func_select(FRO_SW,MUX_SEL_GPIO_INPUT);



	hal_gpio_mux_func_select(F_LED_CTL,MUX_SEL_GPIO_OUTPUT);

	DBG_Printf("default ch\r\n");

	hal_gpio_output_level_set(GPIO0_10_INDEX,GPIO_HIGH); // bt_led
	hal_gpio_output_level_set(SW_BATT,GPIO_LOW); //TEST

	hal_gpio_output_level_set(I_CTL,GPIO_HIGH);

	hal_gpio_output_level_set(BAT1_LED_R,GPIO_HIGH);
	hal_gpio_output_level_set(BAT1_LED_G,GPIO_HIGH);

	hal_gpio_output_level_set(BAT2_LED_R,GPIO_HIGH);
	hal_gpio_output_level_set(BAT2_LED_G,GPIO_HIGH);

	hal_gpio_output_level_set(F_LED_CTL,GPIO_LOW);

	hal_gpio_output_level_set(PVDD_AC_EN,GPIO_HIGH);
	hal_gpio_output_level_set(MUTE_CLT,GPIO_HIGH);


    hal_gpio_mux_func_select(BATT_EN,MUX_SEL_GPIO_OUTPUT); 
    hal_gpio_output_level_set(BATT_EN, GPIO_LOW);

    // hal_gpio_mux_func_select(PSU_IN_12V_EN,MUX_SEL_GPIO_OUTPUT); //PSU_IN_12V_EN
    // hal_gpio_output_level_set(PSU_IN_12V_EN, GPIO_HIGH);



	#endif
}

U8 hal_key_adc_detect(APP_KEY *app_keys, U8 max_simult)
{
	APP_KEY ad_key_id = APP_KEY_INVALID;

	if (max_simult <= 0)
	{
		DBG_Assert(FALSE);
		printf_warn("hal_key_adc_detect max_simult == %d\n\r", MAX_SIMULT_KEY);
		return 0;
	}

	U8 key_adc_channel_cnt = 0;

    #if   (AD_USED_NUMBER == 1)
    	U16 kdata[1];
    #elif (AD_USED_NUMBER == 2)
       	U16 kdata[2];
    #elif (AD_USED_NUMBER == 3)
    	U16 kdata[3];
    #else

    #endif

	#ifdef BATT_CH_AD_SCAN_ENABLE
	batt_val_det_en = TRUE;
	U16 *pbatt_val = batt_val;
	#else
	U8 batt_val_det_en = FALSE;
	U16 *pbatt_val = NULL;	
	#endif

	#if (AD_USED_NUMBER > 0)

	if (key_adc_channel_cnt < max_simult)
	{
		#if (AD_USED_NUMBER == 1)
		#ifdef HW_SARADC_ENABLE
		hw_saradc_oneAdKey_read(kdata, AD_SCAN_FIRST_CH, batt_val_det_en, pbatt_val);
		#else
		kdata[0] = 0x3ff;
		#endif

		if (kdata[0] < ADC_INVALID_VAL2)
		{
			ad_key_id = vAdMapMainKey(0, kdata[0]);
		}
		if (APP_KEY_INVALID != ad_key_id)
		{
			app_keys[key_adc_channel_cnt++] = ad_key_id;
		}

	    #elif (AD_USED_NUMBER == 2)
		#ifdef HW_SARADC_ENABLE
		hw_saradc_twoAdKey_read(kdata, AD_SCAN_FIRST_CH, AD_SCAN_SECOND_CH, batt_val_det_en, pbatt_val);
		#else
		kdata[0] = 0x3ff;
		kdata[1] = 0x3ff;
		#endif

       	if (kdata[0] < ADC_INVALID_VAL2)
		{
			ad_key_id = vAdMapMainKey(0, kdata[0]);
			if (APP_KEY_INVALID != ad_key_id)
			{
				app_keys[key_adc_channel_cnt++] = ad_key_id;//		
			}
		}

		if (key_adc_channel_cnt < max_simult)
		{
			if (kdata[1] < ADC_INVALID_VAL2)
			{
				ad_key_id = vAdMapMainKey(1, kdata[1]);
				if (APP_KEY_INVALID != ad_key_id)
				{
					app_keys[key_adc_channel_cnt++] = ad_key_id;//		
				}
			}			
		}

    	#elif (AD_USED_NUMBER == 3)
		#ifdef HW_SARADC_ENABLE
		hw_saradc_twoAdKey_read(kdata, AD_SCAN_FIRST_CH, AD_SCAN_SECOND_CH, AD_SCAN_THIRD_CH, batt_val_det_en, pbatt_val);
		#else
		kdata[0] = 0x3ff;
		kdata[1] = 0x3ff;
		kdata[2] = 0x3ff;
		#endif

       	if (kdata[0] < ADC_INVALID_VAL2)
		{
			ad_key_id = vAdMapMainKey(0, kdata[0]);
			if (APP_KEY_INVALID != ad_key_id)
			{
				app_keys[key_adc_channel_cnt++] = ad_key_id;
			}
		}

		if (key_adc_channel_cnt < max_simult)
		{
			if (kdata[1] < ADC_INVALID_VAL2)
			{
				ad_key_id = vAdMapMainKey(1, kdata[1]);
				if (APP_KEY_INVALID != ad_key_id)
				{
					app_keys[key_adc_channel_cnt++] = ad_key_id;
				}
			}			
		}

		if (key_adc_channel_cnt < max_simult)
		{
			if (kdata[2] < ADC_INVALID_VAL2)
			{
				ad_key_id = vAdMapMainKey(2, kdata[2]);
				if (APP_KEY_INVALID != ad_key_id)
				{
					app_keys[key_adc_channel_cnt++] = ad_key_id;
				}
			}			
		}		
		#endif
	}

	#else

	{
		#ifdef BATT_CH_AD_SCAN_ENABLE
		batt_val_det_en = TRUE;
		U16 *pbatt_val = batt_val;
		app_saradc_oneAdCh_read(pbatt_val, BATT_CH);
		#endif		
	}

	#endif

	return key_adc_channel_cnt;	
}


