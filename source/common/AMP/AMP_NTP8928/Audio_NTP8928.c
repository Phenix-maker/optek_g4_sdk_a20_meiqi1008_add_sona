#include "os_config.h"

#ifdef AUDIO_NTP8928
#include "hw_timer.h"
#include "Audio_Device.h"
#include "Audio_NTP8928.h"
#include "app_i2c.h"
#include "app_i2c0.h"

#define AUDIO_NTP8928_FUNCTION_ENABLE
//#define USED_REG_SET_ENABLE
#define DBG_Printf

const U8 section0[] = {
	0x33, 0x03, // Soft Mute On Control
	0x34, 0x03, // PWM switching Off
	0x35, 0x06, // PWM Mask Reset
	0x0B, 0x01, // Soft Reset contorl
	0x02, 0x00, // MCLK 3.072(48)/2.8224(44.1) MHz
	//0x7E, 0x03, // Enable coefficient write for ch1/ch2
};
const U8 section0_1[] = {
	0x7E, 0x03 // Enable coefficient write for ch1/ch2
};
const U32 section1[] = { //DRC Coefficients
	0x32, 0x0D298285, 
	0x33, 0x0D298285, 
	0x34, 0x20000000, 
	0x35, 0x10559F5F, 
	0x36, 0x20000000, 
	0x37, 0x0972EC61, 
	0x38, 0x0A72EC61, 
	0x39, 0x0972EC61, 
	0x3A, 0x11672C34, 
	0x3B, 0x10D5EFCC, 
	0x3C, 0x106ACFAF, 
	0x3D, 0x10EACFAF, 
	0x3E, 0x20000000, 
	0x3F, 0x10559F5F, 
	0x40, 0x20000000, 
	0x41, 0x1069120D, 
	0x42, 0x11E9120D, 
	0x43, 0x1069120D, 
	0x44, 0x11672C34, 
	0x45, 0x10D5EFCC, 
	0x46, 0x09551314, 
	0x47, 0x09551314, 
	0x48, 0x20000000, 
	0x49, 0x107CABB4, 
	0x4A, 0x20000000, 
	0x4B, 0x11000000, 
	0x4C, 0x20000000, 
	0x4D, 0x20000000, 
	0x4E, 0x20000000, 
	0x4F, 0x20000000, 
};
const U8 section1_1[] = {
	0x7E, 0x00 // Disable coefficient write for ch1/ch2
};
const U8 section2[] = {
	0x7E, 0x00, // Disable coefficient write for ch1/ch2
	0x33, 0x00, // Soft Mute Off Control/ch2
	0x20, 0xA0, // Low band DRC threshold = 0.8 dB
	0x21, 0x11, // DRC Low band At/Rt
	0x22, 0xA0, // High band DRC threshold = 0.8 dB
	0x23, 0x00, // DRC High band At/Re
	0x26, 0xA0, // Post band DRC threshold = 0.8 dB
	0x27, 0x02, // DRC Post band At/Re
	0x2A, 0xEA, // Sub band DRC threshold = -9.0 dB
	0x2B, 0x12, // DRC Sub band At/Re
	0x29, 0x11, // 2Band DRC mode enable
	0x2C, 0x01, // Sub band mode enable
	0x17, 0xD1, // channel 1 vol
	0x18, 0xD1, // channel 2 vol
	0x43, 0x02, // PWM Output mode (0x02 = DBTL mode / 0x00 = AD mode)
	0x3C, 0x7F, // Prescaler Value CH12
	0x33, 0x03, // Soft Mute On Control/ch2
	//0x7E, 0x03, // Enable coefficient write for ch1/ch2
};
const U8 section2_1[] = {
	0x7E, 0x03 // Enable coefficient write for ch1/ch2
};
const U32 section3[] = { // BQ Coefficients
	0x00, 0x107ECE59, 
	0x01, 0x11FECE59, 
	0x02, 0x107ECE59, 
	0x03, 0x117ECDA7, 
	0x04, 0x10FD9E18, 
	0x05, 0x107ECE59, 
	0x06, 0x11FECE59, 
	0x07, 0x107ECE59, 
	0x08, 0x117ECDA7, 
	0x09, 0x10FD9E18, 
	0x0A, 0x107ECE59, 
	0x0B, 0x11FECE59, 
	0x0C, 0x107ECE59, 
	0x0D, 0x117ECDA7, 
	0x0E, 0x10FD9E18, 
	0x0F, 0x11001DD8, 
	0x10, 0x11FF97EE, 
	0x11, 0x107EF834, 
	0x12, 0x117F97EE, 
	0x13, 0x10FF33E5, 
	0x14, 0x11017550, 
	0x15, 0x11F6109B, 
	0x16, 0x106B982D, 
	0x17, 0x1176109B, 
	0x18, 0x10EE82CC, 
	0x19, 0x11469016, 
	0x1A, 0x12B69A2D, 
	0x1B, 0x1130D689, 
	0x1C, 0x11648C09, 
	0x1D, 0x10DD7C9B, 
	0x1E, 0x106535A3, 
	0x1F, 0x1185614B, 
	0x20, 0x102F5F9D, 
	0x21, 0x1105614B, 
	0x22, 0x10949540, 
	0x23, 0x11000000, 
	0x24, 0x20000000, 
	0x25, 0x20000000, 
	0x26, 0x20000000, 
	0x27, 0x20000000, 
	0x28, 0x11000000, 
	0x29, 0x20000000, 
	0x2A, 0x20000000, 
	0x2B, 0x20000000, 
	0x2C, 0x20000000, 
	0x2D, 0x11000000, 
	0x2E, 0x20000000, 
	0x2F, 0x20000000, 
	0x30, 0x20000000, 
	0x31, 0x20000000, 
	0x5C, 0x11000000, 
	0x5D, 0x20000000, 
	0x5E, 0x20000000, 
	0x5F, 0x20000000, 
	0x60, 0x20000000, 
	0x61, 0x11000000, 
	0x62, 0x20000000, 
	0x63, 0x20000000, 
	0x64, 0x20000000, 
	0x65, 0x20000000, 
	0x66, 0x11000000, 
	0x67, 0x20000000, 
	0x68, 0x20000000, 
	0x69, 0x20000000, 
	0x6A, 0x20000000, 
	0x6B, 0x11000000, 
	0x6C, 0x20000000, 
	0x6D, 0x20000000, 
	0x6E, 0x20000000, 
	0x6F, 0x20000000, 
	0x70, 0x11000000, 
	0x71, 0x20000000, 
	0x72, 0x20000000, 
	0x73, 0x20000000, 
	0x74, 0x20000000, 
	0x75, 0x11000000, 
	0x76, 0x118D8C80, 
	0x77, 0x0F49AC71, 
	0x78, 0x110D8C80, 
	0x79, 0x0FC9AC71, 
};
const U8 section4[] = {
	0x7E, 0x08 // Enable APEQ settings
};
const U32 section5[] = { // APEQ Coefficients
	0x00, 0x10000000, 
	0x01, 0x10000000, 
	0x02, 0x10000000, 
	0x03, 0x10000000, 
	0x04, 0x10000000, 
	0x05, 0x10000000, 
	0x06, 0x11000000, 
	0x07, 0x11000000, 
	0x08, 0x11000000, 
	0x09, 0x11000000, 
	0x0A, 0x11000000, 
	0x0B, 0x11000000, 
	0x0C, 0x11000000, 
	0x0D, 0x11000000, 
	0x0E, 0x11000000, 
	0x0F, 0x11000000, 
	0x10, 0x11000000, 
	0x11, 0x11000000, 
	0x12, 0x20000000, 
	0x13, 0x20000000, 
	0x14, 0x20000000, 
	0x15, 0x20000000, 
	0x16, 0x20000000, 
	0x17, 0x20000000, 
	0x1F, 0x11000000, 
	0x20, 0x11000000, 
	0x21, 0x11000000, 
	0x22, 0x11000000, 
	0x23, 0x11000000, 
	0x24, 0x11000000, 
};
const U8 section5_1[] = {
	0x7E, 0x00 // Enable APEQ settings
};
const U8 section6[] = {
	//0x7E, 0x00, // Disable coefficient write for ch1/ch2
	0x33, 0x00, // Soft Mute Off Control/ch2
	0x19, 0x04, // APEQ Path Option
	0x1A, 0x13, // APEQ1 At/Re time
	0x1B, 0x00, // APEQ2 At/Re time
	0x1C, 0x00, // APEQ3 At/Re time
	0x1D, 0x13, // APEQ4 At/Re time
	0x1E, 0x02, // APEQ5 At/Re time
	0x1F, 0x01, // APEQ6 At/Re time
	0x0E, 0x15, // CH1 BQ1 ~ BQ3 Enable
	0x0F, 0x15, // CH2 BQ1 ~ BQ3 Enable
	0x10, 0x07, // CH1 BQ4 ~ BQ6 Enable
	0x11, 0x07, // CH2 BQ4 ~ BQ6 Enable
	0x12, 0x55, // CH1 BQ7 ~ BQ10 Enable
	0x13, 0x55, // CH2 BQ7 ~ BQ10 Enable
	0x14, 0xF5, // CH1 BQ11 ~ BQ12 Enable
	0x15, 0xF5, // CH2 BQ11 ~ BQ12 Enable
	0x68, 0x00, // Phase Control
	0x24, 0x80, // RS DRC Enable 
	0x25, 0x0B, // Vrms Period for Low-Band 
	0x4D, 0x0A, // Vrms Period for High-Band 
	0x4E, 0x14, // Vrms Period for Sub-Band 
	0x4F, 0x14, // Vrms Period for Post-Band 
	0x2D, 0x0A, // Threshold Table Mapping 
	0x07, 0xF1, // Vrms DRC Release Filter On 
};
const U8 section7[] = {
	0x00, 0x00, // Input Format
	0x01, 0x00, // General Serial Input Format
	0x38, 0x0B, // PWM Protection Control
	0x34, 0x00, // PWM On-Off Switching
	0x35, 0x00, // PWM Mask
	0x41, 0x1A, // MPW
	0x44, 0x0E, // D-BTL Control
	0x76, 0x70, // Monitor1 & Mornitor2
	0x03, 0x4E, // Mixer 00 = 0.0 dB
	0x04, 0x00, // Mixer 01 = -150 dB
	0x05, 0x00, // Mixer 10 = -150 dB
	0x06, 0x4E, // Mixer 11 = 0.0 dB
	0x3E, 0x08, // PWM Mapping 1A to 1A/1B 
	0x3F, 0x1A, // PWM Mapping 1B to 1A/1B 
};
const U8 section8[] = {
	0x3D, 0x00, // WCK Synchronizing Control 
	0x0C, 0xEB, // Master volume Control 
	0x16, 0x00, // Master Fine Volume : 0 dB 
	0x35, 0x00, // PWM Mask = High
	0x34, 0x00, // PWM output switching = CH1/2 ON
	0x33, 0x00, // Soft Mute Off
};


U8 Audio_NTP8928_read_reg(U8 i2c_addr, U8 reg_addr)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE
	U32 ret;
    U8  abData = 0x00;
    ret = I2C0_read_sub(i2c_addr,reg_addr,&abData,1);
	DBG_Assert(ret == I2C_SUCCESS);
	if (ret == I2C_SUCCESS)
	{
        DBG_Printf("REG_ADDR: 0x%x, abData : 0x%x\r\n",reg_addr,abData);
		return TRUE;
	}
    return FALSE;
    #else
	return FALSE;
	#endif
}

U8 Audio_NTP8928_write_reg(U8 reg_addr,U8 *pbuf, U16 len)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE
		
	U32 ret;
	U8 *wbuf;

	wbuf = pbuf;
	//DBG_Printf("NTP8928_i2c_WritePage reg addr: 0x%x\n\r",reg_addr);
	ret = I2C0_write_sub (DEV_PHY_ADDR, reg_addr, wbuf, len);
	DBG_Assert(ret == I2C_SUCCESS);
	delayms(4); // min delay 4ms

	if (ret == I2C_SUCCESS)
	{
        //DBG_Printf("\n\r \n\r NTP8928_i2c_WritePage reg addr: 0x%x\n\r",reg_addr);
		return TRUE;
	}
	
	return FALSE;
	#else
	return FALSE;
	#endif
}


U8 Audio_NTP8928_write()
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    U16 temp;
	BYTE i;
    BYTE buf[20];

    temp = sizeof(section0);
	DBG_Printf("temp0 = %d \n\r",temp);
    for(i=0;i<temp;i+=2)
    {
        buf[0] = section0[i];
        buf[1] = section0[i+1];
        Audio_NTP8928_write_reg(buf[0],&buf[1],1);
    }

    // temp = sizeof(section1)/4;
	// DBG_Printf("temp1 = %d \n\r",temp);
    // for(i=0;i<temp;i+=2)
    // {
    //     buf[0] = section1[i]& 0xff;
    //     buf[1] = (section1[i+1] >> 24) & 0xff;
    //     buf[2] = (section1[i+1] >> 16) & 0xff;
    //     buf[3] = (section1[i+1] >> 8) & 0xff;
    //     buf[4] = section1[i+1] & 0xff;
    //     Audio_NTP8928_write_reg(buf[0],&buf[1],4);
    // }


    // temp = sizeof(section2);
	// DBG_Printf("temp2 = %d \n\r",temp);
    // for(i=0;i<temp;i+=2)
    // {
    //     buf[0] = section2[i];
    //     buf[1] = section2[i+1];
    //     Audio_NTP8928_write_reg(buf[0],&buf[1],1);
    // }

    // temp = sizeof(section3)/4;
	// DBG_Printf("temp3 = %d \n\r",temp);
    // for(i=0;i<temp;i+=2)
    // {
    //     buf[0] = section3[i]& 0xff;
    //     buf[1] = (section3[i+1] >> 24) & 0xff;
    //     buf[2] = (section3[i+1] >> 16) & 0xff;
    //     buf[3] = (section3[i+1] >> 8) & 0xff;
    //     buf[4] = section3[i+1] & 0xff;
    //     Audio_NTP8928_write_reg(buf[0],&buf[1],4);
    // }
	
    // temp = sizeof(section4);
	// DBG_Printf("temp4 = %d \n\r",temp);
    // for(i=0;i<temp;i+=2)
    // {
    //     buf[0] = section4[i];
    //     buf[1] = section4[i+1];
    //     Audio_NTP8928_write_reg(buf[0],&buf[1],1);
    // }

    // temp = sizeof(section5)/4;
	// DBG_Printf("temp5 = %d \n\r",temp);
    // for(i=0;i<temp;i+=2)
    // {
    //     buf[0] = section5[i]& 0xff;
    //     buf[1] = (section5[i+1] >> 24) & 0xff;
    //     buf[2] = (section5[i+1] >> 16) & 0xff;
    //     buf[3] = (section5[i+1] >> 8) & 0xff;
    //     buf[4] = section5[i+1] & 0xff;
    //     Audio_NTP8928_write_reg(buf[0],&buf[1],4);
    // }

	#if 1
    // temp = sizeof(section6);
	// DBG_Printf("temp6 = %d \n\r",temp);
    // for(i=0;i<temp;i+=2)
    // {
    //     buf[0] = section6[i];
    //     buf[1] = section6[i+1];
    //     Audio_NTP8928_write_reg(buf[0],&buf[1],1);
    // }
	
    temp = sizeof(section7);
	DBG_Printf("temp7 = %d \n\r",temp);
    for(i=0;i<temp;i+=2)
    {
        buf[0] = section7[i];
        buf[1] = section7[i+1];
        Audio_NTP8928_write_reg(buf[0],&buf[1],1);
    }

    temp = sizeof(section8);
	DBG_Printf("temp8 = %d \n\r",temp);
    for(i=0;i<temp;i+=2)
    {
        buf[0] = section8[i];
        buf[1] = section8[i+1];
        Audio_NTP8928_write_reg(buf[0],&buf[1],1);
    }
	#endif 
	#endif
}


U8 Audio_NTP8928_gpio_init(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    AMP_RESET_PIN_INIT;
    AMP_RESET_PIN_LOW;
    delayms(10);
    AMP_RESET_PIN_HIGH;
    delayms(10);
    AMP_RESET_PIN_LOW;
    delayus(10);
    AMP_RESET_PIN_HIGH;
    delayms(1);
    

    #endif    

}


void Audio_Dev_NTP8928_Init(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE
    Audio_NTP8928_gpio_init();
   
    Audio_NTP8928_write();


    #endif
}

void Audio_Dev_NTP8928_Open(void)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_NTP8928_Set_Channel(U8 channel)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_NTP8928_Set_Volume(U8 volume)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_NTP8928_Set_Speaker(U8 speaker, U8 attenuation)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_NTP8928_Set_Bass_Treble(U8 bass, U8 treble)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_NTP8928_Set_eq_mode(U8 mode)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    #endif	
}

void Audio_Dev_NTP8928_Set_Mute(U8 con)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_NTP8928_Set_AM_Mode(U8 con, U32 kFreq)
{
    DBG_Printf("%s\n\r",__func__);
    #ifdef AUDIO_NTP8928_FUNCTION_ENABLE

    #endif
}




#endif
