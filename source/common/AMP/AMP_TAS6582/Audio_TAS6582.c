
#include "c_def.h"
#include "debug.h"
#include "oem.h"
#include "se_link_24bit.h"


#include "hw_timer.h"
#include "Audio_Device.h"

#ifdef AUDIO_AMP_TAS6582

#include "app_i2c.h"
#include "app_i2c0.h"
#include "Audio_TAS6582.h"
#include "Audio_TAS6582_table.h"

#ifdef DSP_SONA_MODULE_ENABLE
float vol_table[31] =
{
-20,    //0
-19,    //1
-18,    //2
-17.2,  //3
-16.6,  //4
-16,    //5
-15.4,  //6
-14.8,  //7
-14.2,  //8
-13.6,  //9
-13,    //10
-12.4,  //11
-11.8,  //12
-11.2,  //13
-10.6,  //14
-10,    //15
-8.4,   //16
-7.8,   //17
-7.2    //18
-6.6,   //19
-6,     //20
-5.4,   //21
-4.8,   //22
-4.2,   //23
-3.6,   //24
-3,     //25
-2.4,   //26
-1.8,   //27
-1.2,   //28
-0.6,   //29
0,      //30
};
#endif
#if 1
U8 vol_table_AUX[32] =
{
// /*0  -103dB*/0xFE,
// /*1  -92dB*/0xE8,
// /*2  -89dB*/0xE2,
// /*3  -86dB*/0xDC,
// /*4  -83dB*/0xD6,
// /*5  -80dB*/0xD0,
// /*6  -77dB*/0xCA,
// /*7  -74dB*/0xC4,
// /*8  -71dB*/0xBE,
// /*9  -68dB*/0xB8,
// /*10  -65dB*/0xB2,
// /*11  -62dB*/0xAC,
// /*12  -59dB*/0xA6,
// /*13  -56dB*/0xA0,
// /*14  -53dB*/0x9A,
// /*15  -50dB*/0x94,
// /*16  -47dB*/0x8E,
// /*17  -44dB*/0x88,
// /*18  -41dB*/0x82,
// /*19  -39dB*/0x7E,
// /*20  -37dB*/0x7A,
// /*21  -34dB*/0x74,
// /*22  -31dB*/0x6E,
// /*23  -28dB*/0x68,
// /*24  -25dB*/0x62,
// /*25  -23dB*/0x5E,
// /*26  -21dB*/0x5A,
// /*27  -19dB*/0x56,
// /*28  -17dB*/0x52,
// /*29  -16dB*/0x50,
// /*30  -15dB*/0x4E,
// /*31  -14dB*/0x4C,
};
#else
U8 vol_table_AUX[32] =
{
/*0  -103dB*/0xFE,
/*1  -74dB*/0xC4,
/*2  -71dB*/0xBE,
/*3  -68dB*/0xB8,
/*4  -65dB*/0xB2,
/*5  -62dB*/0xAC,
/*6  -59dB*/0xA6,
/*7  -56dB*/0xA0,
/*8  -53dB*/0x9A,
/*9  -50dB*/0x94,
/*10  -47dB*/0x8E,
/*11  -44dB*/0x88,
/*12  -41dB*/0x82,
/*13  -38dB*/0x7C,
/*14  -35dB*/0x76,
/*15  -32dB*/0x70,
/*16  -29dB*/0x6A,
/*17  -26dB*/0x64,
/*18  -23dB*/0x5E,
/*19  -21dB*/0x5A,
/*20  -19dB*/0x56,
/*21  -16dB*/0x50,
/*22  -13dB*/0x4A,
/*23  -10dB*/0x44,
/*24  -7dB*/0x3E,
/*25  -5dB*/0x3A,
/*26  -3dB*/0x36,
/*27  -1dB*/0x32,
/*28  1dB*/0x2E,
/*29  2dB*/0x2C,
/*30  3dB*/0x2A,
/*31  4dB*/0x28,
};


#endif
#define AMP_FAULT_PIN               GPIO0_18_INDEX//
#define Boost_PGOOD                 GPIO0_00_INDEX

static I2C_RETURN TAS6582_send_byte(U8 addr, U8 bdat)
{
    // if((I2C0_write_sub(TAS6582_ADDRESS, addr, &bdat, 1) == I2C_SUCCESS) && (I2C0_write_sub(TAS6582_ADDRESS_1, addr, &bdat, 1)== I2C_SUCCESS))
    if((I2C0_write_sub(TAS6582_ADDRESS, addr, &bdat, 1) == I2C_SUCCESS))
    {
        return I2C_SUCCESS;
    }
    else
    {
        return I2C_ERROR;
    }
}

static void TAS6582_mute(U8 mode)
{
    // return;
	u8 temp;
	I2C_RETURN ret;
	// DBG_Printf("audio_vari.mute = %d\n\r",audio_vari.mute);
	if (mode)
	{
        //audio_amp_tas6582_set_volume(0);
		temp = 0x04|0x08;
        ret = TAS6582_send_byte(TAS6582_DEVICE_CTRL_1, temp);
		temp = 0x40|0x80;
        ret = TAS6582_send_byte(TAS6582_DEVICE_CTRL_2, temp);
        audio_vari.mute = TRUE;
		DBG_Printf("TAS6582 sub MUTE ON\n");
	}
	else
	{
		DBG_Printf("TAS6582 sub MUTE OFF\n");
		temp = 0x04;
		ret = TAS6582_send_byte(TAS6582_DEVICE_CTRL_1, temp);	
		temp = 0x40;
		ret = TAS6582_send_byte(TAS6582_DEVICE_CTRL_2, temp);	
        audio_vari.mute = FALSE;

        //audio_amp_tas6582_set_volume(audio_vari.volume);
	}
}

static void amp_fault_pin_init(void)//aaaaa
{
	hal_gpio_mux_func_select(AMP_FAULT_PIN, MUX_SEL_GPIO_INPUT);

    // hal_gpio_mux_func_select(GPIO0_11_INDEX,MUX_SEL_GPIO_OUTPUT); //PWR_EN
    // hal_gpio_output_level_set(GPIO0_11_INDEX, GPIO_HIGH);

    // hal_gpio_mux_func_select(GPIO0_28_INDEX,MUX_SEL_GPIO_OUTPUT); //PSU_IN_12V_EN
    // hal_gpio_output_level_set(GPIO0_28_INDEX, GPIO_HIGH);
    
    hal_gpio_mux_func_select(GPIO0_04_INDEX,MUX_SEL_GPIO_OUTPUT); //AMP_PDN
    hal_gpio_mux_func_select(GPIO0_05_INDEX,MUX_SEL_GPIO_OUTPUT); //AMP_STANDY
    hal_gpio_mux_func_select(Boost_PGOOD,MUX_SEL_GPIO_INPUT); //AMP_Boost

    hal_gpio_output_level_set(GPIO0_04_INDEX, GPIO_LOW);
    hal_gpio_output_level_set(GPIO0_05_INDEX, GPIO_LOW);
    // hal_gpio_output_level_set(Boost_PGOOD,GPIO_LOW);
}

static void transmit_registers(cfg_reg *r, int n)
{
    int i = 0;
    while (i < n) {
        switch (r[i].command) {
        case CFG_META_SWITCH:
            // Used in legacy applications.  Ignored here.
            break;
        case CFG_META_DELAY:
            delayms(r[i].param);
            break;
        case CFG_META_BURST:
            //i2c_write((unsigned char *)&r[i+1], r[i].param);
            if(r[i].param < 1)
            {
                // I2C0_write_sub(TAS6582_ADDRESS, r[i+1].command, (unsigned char *)&r[i+1].param, 0);
            } 
            else
            {
                I2C0_write_sub(TAS6582_ADDRESS, r[i+1].command, (unsigned char *)&r[i+1].param, r[i].param - 1);

            }
            // I2C0_write_sub(TAS6582_ADDRESS, r[i+1].command, (unsigned char *)&r[i+1].param, r[i].param - 1);
            DBG_printf("burst_len:%d\r\n",r[i].param);
            // I2C0_write_sub(TAS6582_ADDRESS_1, r[i+1].command, (unsigned char *)&r[i+1].param, r[i].param - 1);
            i +=  (r[i].param / 2) + 1;
            break;
        default:
            I2C0_write_sub(TAS6582_ADDRESS, r[i].command, (unsigned char *)&r[i].param,1);
            // I2C0_write_sub(TAS6582_ADDRESS_1, r[i].command, (unsigned char *)&r[i].param,1);
            break;
        }
        i++;
    }
}
void audio_amp_tas6582_init(void)
{
	u8 i;
	I2C_RETURN ret;
    amp_fault_pin_init();//error pin init

    hal_gpio_output_level_set(GPIO0_04_INDEX,GPIO_HIGH);//PD
    // delayms(500);
    hal_gpio_output_level_set(GPIO0_05_INDEX,GPIO_HIGH);//STANDBY
    // delayms(500);    
	DBG_Printf("%s\r\n",__func__);
    transmit_registers(registers, sizeof(registers)/sizeof(registers[0]));
	return;
}


void audio_amp_tas6582_open(void)
{
    DBG_Printf("%s\r\n",__func__);

	// audio_amp_tas6582set_volume(audio_vari.volume); //default
    // audio_vari.volume = 15;
    audio_amp_tas6582_set_mute(TRUE);
    
}

void audio_amp_tas6582_set_channel(U8 channel)
{

}

void audio_amp_tas6582_set_volume(U8 vol)
{
    DBG_Printf("vol = %d\n\r",vol);
    if(vol > 30 )
    {
        vol = 30;
    }
    u8 temp;
	I2C_RETURN ret;
    #if 0
    if(SOURCE_AUX == global_vari.audio_source)
    {

    	temp = vol_table_AUX[vol];
    }
    else
    {
	temp = vol_table[vol];
    }

    // DBG_Printf("temp = 0x%x\r\n",temp);
    ret = TAS6582_send_byte(DIG_VOL_Register, temp);
	#else
    #ifdef DSP_EQ_SETTING_24BIT_DECODE
    wheq_vol_set(vol);
    #endif
    #ifdef DSP_SONA_MODULE_ENABLE
    audio_vari.woofer_volume_dB = vol_table[vol];
    #endif   
    // AUDIOdevice.Set_Volume(vol);
    #endif
}

void audio_amp_tas6582_set_bass_treble(U8 bass, U8 treble)
{
    
}

void audio_amp_tas6582_Set_Eq_Mode(U8 mode)
{
   
}

void audio_amp_tas6582_set_mute(U8 con)
{
	TAS6582_mute(con);
}


void audio_amp_tas6582_class_H(U8 enable)
{
    #if 0
    int val = 0x00; //class_H disable
    if(enable)
    {
        val = 0x20;
        I2C0_write_sub(TAS6582_ADDRESS, 0x3a, &val, 1);
        DBG_Printf("class_H enable\r\n");
    }
    else
    {
        val = 0x00;
        I2C0_write_sub(TAS6582_ADDRESS, 0x3a, &val, 1);
        DBG_Printf("class_H disable\r\n");

    }

    #endif

}
#endif



