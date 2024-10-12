#include "c_def.h"
#include "debug.h"
#include "oem.h"
#include "se_link_24bit.h"


#include "hw_timer.h"
#include "Audio_Device.h"


#include "app_i2c.h"
#include "app_i2c0.h"
#include "app_I2C0.h"

#ifdef AUDIO_AMP_TAS5828M
#include "Audio_TAS5828.h"
#include "Audio_TAS5828_table.h"

U8 vol_table[32] =
{
/*0  -103dB*/0xFE,
/*1  -92dB*/0xE8,
/*2  -89dB*/0xE2,
/*3  -86dB*/0xDC,
/*4  -83dB*/0xD6,
/*5  -80dB*/0xD0,
/*6  -77dB*/0xCA,
/*7  -74dB*/0xC4,
/*8  -71dB*/0xBE,
/*9  -68dB*/0xB8,
/*10  -65dB*/0xB2,
/*11  -62dB*/0xAC,
/*12  -59dB*/0xA6,
/*13  -56dB*/0xA0,
/*14  -53dB*/0x9A,
/*15  -50dB*/0x94,
/*16  -47dB*/0x8E,
/*17  -44dB*/0x88,
/*18  -41dB*/0x82,
/*19  -39dB*/0x7E,
/*20  -37dB*/0x7A,
/*21  -34dB*/0x74,
/*22  -31dB*/0x6E,
/*23  -28dB*/0x68,
/*24  -25dB*/0x62,
/*25  -23dB*/0x5E,
/*26  -21dB*/0x5A,
/*27  -19dB*/0x56,
/*28  -17dB*/0x52,
/*29  -16dB*/0x50,
/*30  -15dB*/0x4E,
/*31  -14dB*/0x4C,

};
#if 1
U8 vol_table_AUX[32] =
{
/*0  -103dB*/0xFE,
/*1  -92dB*/0xE8,
/*2  -89dB*/0xE2,
/*3  -86dB*/0xDC,
/*4  -83dB*/0xD6,
/*5  -80dB*/0xD0,
/*6  -77dB*/0xCA,
/*7  -74dB*/0xC4,
/*8  -71dB*/0xBE,
/*9  -68dB*/0xB8,
/*10  -65dB*/0xB2,
/*11  -62dB*/0xAC,
/*12  -59dB*/0xA6,
/*13  -56dB*/0xA0,
/*14  -53dB*/0x9A,
/*15  -50dB*/0x94,
/*16  -47dB*/0x8E,
/*17  -44dB*/0x88,
/*18  -41dB*/0x82,
/*19  -39dB*/0x7E,
/*20  -37dB*/0x7A,
/*21  -34dB*/0x74,
/*22  -31dB*/0x6E,
/*23  -28dB*/0x68,
/*24  -25dB*/0x62,
/*25  -23dB*/0x5E,
/*26  -21dB*/0x5A,
/*27  -19dB*/0x56,
/*28  -17dB*/0x52,
/*29  -16dB*/0x50,
/*30  -15dB*/0x4E,
/*31  -14dB*/0x4C,
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
#define AMP_FAULT_PIN               GPIO0_04_INDEX//


static I2C_RETURN TAS5828M_send_byte(U8 addr, U8 bdat)
{
    return (!(I2C0_write_sub(TAS5828M_ADDRESS, addr, &bdat, 1)) && !(I2C0_write_sub(TAS5828M_ADDRESS_1, addr, &bdat, 1))); 
}

static void TAS5828M_mute(U8 mode)
{
    return;
	u8 temp;
	I2C_RETURN ret;
	// DBG_Printf("audio_vari.mute = %d\n\r",audio_vari.mute);
	if (mode)
	{
        //audio_amp_tas5828m_set_volume(0);
		temp = 0x03|0x08;
        ret = TAS5828M_send_byte(DEVICE_CTRL_2, temp);
		DBG_Printf("TAS5828M sub MUTE ON\n");
	}
	else
	{
		DBG_Printf("TAS5828M sub MUTE OFF\n");
		temp = 0x03;
		ret = TAS5828M_send_byte(DEVICE_CTRL_2, temp);	
        //audio_amp_tas5828m_set_volume(audio_vari.volume);
	}
}

static void amp_fault_pin_init(void)//aaaaa
{
	hal_gpio_mux_func_select(AMP_FAULT_PIN, MUX_SEL_GPIO_INPUT);

    // hal_gpio_mux_func_select(GPIO0_11_INDEX,MUX_SEL_GPIO_OUTPUT); //PWR_EN
    // hal_gpio_output_level_set(GPIO0_11_INDEX, GPIO_HIGH);

    // hal_gpio_mux_func_select(GPIO0_28_INDEX,MUX_SEL_GPIO_OUTPUT); //PSU_IN_12V_EN
    // hal_gpio_output_level_set(GPIO0_28_INDEX, GPIO_HIGH);
    
    hal_gpio_mux_func_select(GPIO0_05_INDEX,MUX_SEL_GPIO_OUTPUT); //AMP_PDN
    hal_gpio_output_level_set(GPIO0_05_INDEX, GPIO_HIGH);

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
            I2C0_write_sub(TAS5828M_ADDRESS, r[i+1].command, (unsigned char *)&r[i+1].param, r[i].param - 1);
            I2C0_write_sub(TAS5828M_ADDRESS_1, r[i+1].command, (unsigned char *)&r[i+1].param, r[i].param - 1);
            i +=  (r[i].param / 2) + 1;
            break;
        default:
            I2C0_write_sub(TAS5828M_ADDRESS, r[i].command, (unsigned char *)&r[i].param,1);
            I2C0_write_sub(TAS5828M_ADDRESS_1, r[i].command, (unsigned char *)&r[i].param,1);
            break;
        }
        i++;
    }
}
void audio_amp_tas5828m_init(void)
{
	u8 i;
	I2C_RETURN ret;
    amp_fault_pin_init();//error pin init
	DBG_Printf("%s\r\n",__func__);
    transmit_registers(registers, sizeof(registers)/sizeof(registers[0]));
	return;
}


void audio_amp_tas5828m_open(void)
{
    DBG_Printf("%s\r\n",__func__);

	// audio_amp_tas5828m_set_volume(audio_vari.volume); //default
    // audio_vari.volume = 15;
    // audio_amp_tas5828m_set_mute(TRUE);
    
}

void audio_amp_tas5828m_set_channel(U8 channel)
{

}

void audio_amp_tas5828m_set_volume(U8 vol)
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
    ret = TAS5828M_send_byte(DIG_VOL_Register, temp);
	#else
    wheq_vol_set(vol);
    #endif
}

void audio_amp_tas5828m_set_bass_treble(U8 bass, U8 treble)
{
    
}

void audio_amp_tas5828m_Set_Eq_Mode(U8 mode)
{
   
}

void audio_amp_tas5828m_set_mute(U8 con)
{
	TAS5828M_mute(con);
}

#endif



