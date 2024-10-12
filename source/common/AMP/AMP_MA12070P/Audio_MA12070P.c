#include "os_config.h"
#include "regmap.h"


#ifdef AUDIO_MA12070P
#include "hw_timer.h"
#include "Audio_Device.h"
#include "Audio_MA12070P.h"
#include "Amp_Vol_Table_XiaoMi.h"

#define AUDIO_MA12070P_FUNCTION_ENABLE
//#define USED_REG_SET_ENABLE
#define DBG_Printf

U8 Audio_MA12070_read_reg(U8 i2c_addr, U8 reg_addr)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE
    U8  abData = 0x00;
    I2C1_read_sub(i2c_addr,reg_addr,&abData,1);
    printf_info("REG_ADDR: 0x%x, abData : 0x%x\r\n",reg_addr,abData);
    #endif
}

U8 Audio_MA12070P_gpio_init(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE
    app_i2c1_init();

    ENABLE_PIN_INIT;
    MUTE_PIN_INIT;
    //POWER_PIN;
    POWER_PIN_INIT;

    POWER_PIN_HIGH;
    ENABLE_PIN_HIGH;
    MUTE_PIN_HIGH;
    delayms(30);
    
    MUTE_PIN_LOW;
    delayms(30);
    
    ENABLE_PIN_LOW;
    delayms(30);

    ENABLE_PIN_HIGH;
    delayms(30);

    ENABLE_PIN_LOW;
    delayms(20);

    #endif    

}

U8 Audio_MA12070P_gpio_init_end(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE

    delayms(30);
    MUTE_PIN_HIGH;

    #endif

}



void Audio_Dev_MA12070P_Init(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE

    U8 abData = 0x00; // [bit3]  1:user VLP,  0:bypass VLP

    Audio_MA12070P_gpio_init();

    //reg addr: 0x35
    //data: 0x08
    #ifndef USED_REG_SET_ENABLE
    abData = 0x08;
    #else
    abData = 0x00;
    CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X35 *p_0x35_data = (CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X35 *)abData;
    p_0x35_data->i2s_format = RIGHT_JUSTIFIED_16BIT;
    p_0x35_data->audio_proc_enable = FALSE;
    p_0x35_data->audio_proc_release = SLOW_TIME;
    p_0x35_data->audio_proc_attack = SLOW_TIME;
    #endif
    I2C1_write_sub(DEV_PHY_ADDR,0x35, &abData, sizeof(abData));// i2s format  is i2s, Right justified 16bits


    //reg addr: 0x1D
    //data: 0x01
    #ifndef USED_REG_SET_ENABLE
    abData = 0x01;// PMP1 for THD issue
    #else
    abData = 0x00;
    CHIP_MA12070P_CUSTOM_POWER_MODE_PROFILE_SETTING p_0x1D_data = (CHIP_MA12070P_CUSTOM_POWER_MODE_PROFILE_SETTING *)abData;
    p_0x1D_data.pmprofile = POWER_MODE_PROFILE_1;
    #endif
    I2C1_write_sub(DEV_PHY_ADDR,0x1D, &abData, sizeof(abData));// Power Mode Profile 1


    //reg addr: 0x36
    //data: 0x21|0x80   /0x01|0x80  /0x09|0x80
    #ifndef USED_REG_SET_ENABLE
    //abData = 0x21|0x80;
    abData = 0x01|0x80;
    #else
    abData = 0x00;
    CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X36 p_0x36_data = (CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X36 *)abData;
    p_0x36_data.i2s_sck_pol = 1;
    p_0x36_data.i2s_ws_pol = 0;
    p_0x36_data.i2s_order = 0;
    p_0x36_data.i2s_framesize = 0;
    p_0x36_data.i2s_rightfirst = 1;

    p_0x36_data.audio_proc_limiterEnable = FALSE;
    p_0x36_data.audio_proc_mute = TRUE;
    #endif
    I2C1_write_sub(DEV_PHY_ADDR,0x36, &abData, sizeof(abData));//C use right channel,set i2s profile config

    //reg addr: 0x40
    //data: 0x18        (0x0C +12dB, 0x09 +15dB)
    #ifndef USED_REG_SET_ENABLE
    abData = 0x18;
    #else
    abData = 0x00;
    CHIP_MA12070P_MASTER_INTEGER_DB_VOLUME p_0x40_data = (CHIP_MA12070P_MASTER_INTEGER_DB_VOLUME *)abData;
    p_0x40_data.vol_db_master = 0x18;
    #endif
    I2C1_write_sub(DEV_PHY_ADDR,0x40, &abData, sizeof(abData));//master integer dB volume,[7:0] effective

    //reg addr: 0x41
    //data: 0x18        (0x0C +12dB, 0x09 +15dB)
    #ifndef USED_REG_SET_ENABLE
    abData = 0x00;
    #else
    abData = 0x00;
    CHIP_MA12070P_MASTER_FRACT_DB_VOLUME p_0x40_data = (CHIP_MA12070P_MASTER_FRACT_DB_VOLUME *)abData;
    p_0x40_data.vol_db_master = 0x00;
    #endif
    I2C1_write_sub(DEV_PHY_ADDR,0x41, &abData, sizeof(abData));//master fract  dB volume,[1:0] effective

    //reg addr: 0x36
    //data: 0x01
    #ifndef USED_REG_SET_ENABLE
    abData = 0x01;
    #else
    abData = 0x00;
    p_0x36_data = (CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X36 *)abData;
    p_0x36_data.i2s_sck_pol = 1;
    p_0x36_data.i2s_ws_pol = 0;
    p_0x36_data.i2s_order = 0;
    p_0x36_data.i2s_framesize = 0;
    p_0x36_data.i2s_rightfirst = 0;

    p_0x36_data.audio_proc_limiterEnable = FALSE;
    p_0x36_data.audio_proc_mute = FALSE;
    #endif
    I2C1_write_sub(DEV_PHY_ADDR,0x36, &abData, sizeof(abData));//C use right channel,set i2s profile config

    Audio_MA12070P_gpio_init_end();

    #endif
}

void Audio_Dev_MA12070P_Open(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE
    Audio_Dev_MA12070P_Set_Volume(audio_vari.volume);

    Audio_Dev_MA12070P_Set_Mute(FALSE);
    #endif
}

void Audio_Dev_MA12070P_Set_Channel(U8 channel)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE

    #endif
}

//#define VOLUME_MAX 40
void Audio_Dev_MA12070P_Set_Volume(U8 volume)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE
    U8 integer_db = 0x00;
    U8 fractional_db = 0x00;

    float value = 0;
    //value = table[volume];
    integer_db = xiaomi_tv_source_vol_table[volume];

    //value = (24+216) - volume * 216 / VOLUME_MAX;

    //Audio_MA12070P_VLP_gain_input_Integer_Fractional_reg_value_output(value,&integer_db,&fractional_db);

    I2C1_write_sub(DEV_PHY_ADDR,0x40, &integer_db, sizeof(integer_db));
    Audio_MA12070_read_reg(DEV_PHY_ADDR,0x40);

    I2C1_write_sub(DEV_PHY_ADDR,0x41, &fractional_db, sizeof(fractional_db));
    Audio_MA12070_read_reg(DEV_PHY_ADDR,0x41);

    #endif
}

void Audio_Dev_MA12070P_Set_Speaker(U8 speaker, U8 attenuation)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_MA12070P_Set_Bass_Treble(U8 bass, U8 treble)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_MA12070P_Set_eq_mode(U8 mode)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_MA12070P_Set_Mute(U8 con)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE
    U8 abData = 0x00;
    #ifdef USED_REG_SET_ENABLE
    CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X36 p_0x36_data = (CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X36 *)abData;
    #endif

    if (con == TRUE)
    {
        DBG_Printf("MA12070P_AMP_MUTE_ON\r\n");
        #ifndef USED_REG_SET_ENABLE
        abData = 0x01|0x80;
        #else
        p_0x36_data = (CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X36 *)abData;
        p_0x36_data.i2s_sck_pol = 1;
        p_0x36_data.i2s_ws_pol = 0;
        p_0x36_data.i2s_order = 0;
        p_0x36_data.i2s_framesize = 1;
        p_0x36_data.i2s_rightfirst = 0;

        p_0x36_data.audio_proc_limiterEnable = FALSE;
        p_0x36_data.audio_proc_mute = TRUE;             
        #endif
        I2C1_write_sub(DEV_PHY_ADDR,0x36, &abData, sizeof(abData));
        Audio_MA12070_read_reg(DEV_PHY_ADDR,0x36);
    }
    else if (con == FALSE)
    {
        DBG_Printf("MA12070P_AMP_MUTE_OFF\r\n");
        #ifndef USED_REG_SET_ENABLE
        abData = 0x01|0x00;
        #else
        p_0x36_data = (CHIP_MA12070P_I2S_FORMAT_CONFIG_AND_VOLUME_CONTROL_LIMITER_0X36 *)abData;
        p_0x36_data.i2s_sck_pol = 1;
        p_0x36_data.i2s_ws_pol = 0;
        p_0x36_data.i2s_order = 0;
        p_0x36_data.i2s_framesize = 1;
        p_0x36_data.i2s_rightfirst = 0;

        p_0x36_data.audio_proc_limiterEnable = FALSE;
        p_0x36_data.audio_proc_mute = FALSE;
        #endif
        I2C1_write_sub(DEV_PHY_ADDR,0x36, &abData, sizeof(abData));
        Audio_MA12070_read_reg(DEV_PHY_ADDR,0x36);
    }
    else
    {
        printf_error("input con error\r\n");
    }
    #endif
}

void Audio_Dev_MA12070P_Set_AM_Mode(U8 con, U32 kFreq)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE

    #endif
}

/*
  vlp_gain: 24.00dB ~ -144.00dB
  *p_integer_db: 0~255
  *p_fractional_db: 0/1/2/3
*/
void Audio_MA12070P_VLP_gain_input_Integer_Fractional_reg_value_output(float vlp_gain, U8 *p_integer_db, U8 *p_fractional_db)
{
    DBG_Printf("%s:vlp_gain:%2f\n\r",__func__,vlp_gain);
    #ifdef AUDIO_MA12070P_FUNCTION_ENABLE
    U8 vlp_gain_integer = 0x0;
    U8 vlp_gain_fractional = 0x0;
    if (vlp_gain > 0)
    {
        vlp_gain_integer 	= ((U8)(vlp_gain*100))%100;
        vlp_gain_fractional = ((U8)(vlp_gain*100))/100;
        if (vlp_gain_fractional == 0x00)
        {
            *p_integer_db = 24 - vlp_gain_integer;
            *p_fractional_db = 0x0;
        }
        else
        {
            *p_integer_db = (24 - vlp_gain_integer) + 1;
            *p_fractional_db = 3 - (vlp_gain_fractional/25);
        }

        
    }
    else if (vlp_gain > -144)
    {
        vlp_gain_integer 	= ((U8)(abs(vlp_gain)*100))%100;
        vlp_gain_fractional = ((U8)(abs(vlp_gain)*100))/100;

        *p_integer_db = vlp_gain_integer - 24;

        *p_fractional_db = vlp_gain_fractional/25;
    }
    else
    {
        //*p_integer_db > 0xA8 and *p_fractional_db = 0/1/2/3; vlp_gain = -144dB
        *p_integer_db = 0xA8;
        *p_fractional_db = 0;
    }

    #endif
    DBG_Printf("exit p_integer_db:0x%x,p_fractional_db:0x%x\n\r",__func__, *p_integer_db, *p_fractional_db);
}

#endif
