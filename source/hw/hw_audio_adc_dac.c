#include "os_config.h"

#include "regmap.h"

#include "hw_audio_adc_dac.h"
#include "hw_timer.h"


//#define AUDIO_ADC_DAC_I2C_BUS

#define AUDIO_IIS_ENABLE

//#define ADC_SAMPLE_RATE_16K
#define ADC_SAMPLE_RATE_48K
//#define ADC_SAMPLE_RATE_44P1K
//#define ADC_SAMPLE_RATE_96K
//#define ADC_SAMPLE_RATE_192K
//#define ADC_SAMPLE_RATE_384K
//#define ADC_SAMPLE_RATE_768K

//#define DAC_SAMPLE_RATE_16K
#define DAC_SAMPLE_RATE_48K
//#define DAC_SAMPLE_RATE_44P1K
//#define DAC_SAMPLE_RATE_96K
//#define DAC_SAMPLE_RATE_192K
//#define DAC_SAMPLE_RATE_384K
//#define DAC_SAMPLE_RATE_768K


//#define FULL_TEST//8 channel dmic test


enum {
	enADC_DAC_DMIC_CONRTOL_SEL_AHB_BUS = 0,
	enADC_DAC_DMIC_CONTROL_SEL_I2C_BUS,
};

enum {
	enADC_FIFO_DISABLE,
	enADC_FIFO_EANBLE,
};

typedef enum {
	DEVICES_QADC,
	DEVICES_QDMIC,
	DEVICES_QDAC,
	DEVICES_QVAD
} Qdevices;

static void q_devices_reg_write(Qdevices dev, int reg, int value)
{
#ifdef AUDIO_ADC_DAC_I2C_BUS
	U32 i2c_addr;
	int rd;

	switch (dev)
	{
	case DEVICES_QADC:
		i2c_addr = QADC1_I2C_ADDRESS;
		break;

	case DEVICES_QDMIC:
		i2c_addr = QADC2_I2C_ADDRESS;
		break;

	case DEVICES_QDAC:
		i2c_addr = QADC3_I2C_ADDRESS;
		break;

	case DEVICES_QVAD:
		i2c_addr = QADC4_I2C_ADDRESS;
		break;				

	default:
		i2c_addr = QADC1_I2C_ADDRESS;
		break;
	}

	hw_i2c_write(i2c_addr,reg,value);	

#else //AUDIO_ADC_DAC_I2C_BUS

	U32 base_addr;
	int rd;

	switch (dev)
	{
	case DEVICES_QADC:
		base_addr = QADC1_BASE_ADDR;
		break;

	case DEVICES_QDMIC:
		base_addr = QADC2_BASE_ADDR;
		break;

	case DEVICES_QDAC:
		base_addr = QADC3_BASE_ADDR;
		break;

	case DEVICES_QVAD:
		base_addr = QADC4_BASE_ADDR;
		break;				

	default:
		base_addr = QADC1_BASE_ADDR;
		break;
	}

	REG(base_addr + 4*reg) = value;

	do
	{
		rd = REG(base_addr + READ_ADDR_OFFSET + 4*reg);		//checking if writing reg is finished
	} while(!(rd & 0x100));		
#endif //AUDIO_ADC_DAC_I2C_BUS
}

static int q_devices_reg_read(Qdevices dev, int reg)
{
#ifdef AUDIO_ADC_DAC_I2C_BUS
	U32 i2c_addr;
	int val;

	switch (dev)
	{
	case DEVICES_QADC:
		i2c_addr = QADC1_I2C_ADDRESS;
		break;

	case DEVICES_QDMIC:
		i2c_addr = QADC2_I2C_ADDRESS;
		break;

	case DEVICES_QDAC:
		i2c_addr = QADC3_I2C_ADDRESS;
		break;

	case DEVICES_QVAD:
		i2c_addr = QADC4_I2C_ADDRESS;
		break;				

	default:
		i2c_addr = QADC1_I2C_ADDRESS;
		break;
	}

	val = hw_i2c_read(i2c_addr);

	return val;

#else //AUDIO_ADC_DAC_I2C_BUS

	U32 base_addr;

	int val = 0;

	switch (dev)
	{
	case DEVICES_QADC:
		base_addr = QADC1_BASE_ADDR;
		break;

	case DEVICES_QDMIC:
		base_addr = QADC2_BASE_ADDR;
		break;

	case DEVICES_QDAC:
		base_addr = QADC3_BASE_ADDR;
		break;

	case DEVICES_QVAD:
		base_addr = QADC4_BASE_ADDR;
		break;				

	default:
		break;
	}

	val = REG(base_addr + 4*reg);

	do
	{
		val = REG(base_addr + READ_ADDR_OFFSET + 4*reg);		//checking if writing reg is finished
	} while(!(val & 0x100));		
	
	val &= ~0x100;

	return val;
#endif //AUDIO_ADC_DAC_I2C_BUS
}

void qadc_write(int addr,int val)
{
	int rd;

	q_devices_reg_write(DEVICES_QADC, addr, val);
	rd = q_devices_reg_read(DEVICES_QADC, addr);

	if (rd != val)
    {
        printf_error("qadc write error!\n");
    }
    else
    {
        printf_info("qadc write succeed!\n");
    }
}

static void dmic_write(int addr,int val)
{
	int rd;

	q_devices_reg_write(DEVICES_QDMIC,addr,val);
	rd = q_devices_reg_read(DEVICES_QDMIC, addr);

	if (rd != val)
    {
        printf_error("dmic write error!\n");
    }
    else
    {
        printf_info("dmic write succeed!\n");
    }

}

void dmic_gpio_pin_init(ADC_MARK_SET *adc_mark)
{
	U32 val;
	GPIO_PINs *p_gpip_pin = (GPIO_PINs *) &val;
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &val;
	
	printf_info("%s:%d,%d,%d,%d\r\r", __func__, 
	adc_mark->adc_a_dmic_en, adc_mark->adc_b_dmic_en, adc_mark->adc_c_dmic_en, adc_mark->adc_d_dmic_en);

	//dmic
    val = REG_GPIO4_FUNC;

	if (adc_mark->adc_d_dmic_en == TRUE)
	{
		p_gpip_pin->bit_9 =  !(!(1<<(GPIO0_09_INDEX-GPIO0_00_INDEX))); //DMIC CLK D
		p_gpip_pin->bit_12 = !(!(1<<(GPIO0_12_INDEX-GPIO0_00_INDEX))); //DMIC D3	
	}
	else
	{
		p_gpip_pin->bit_9 =  !(1<<(GPIO0_09_INDEX-GPIO0_00_INDEX)); //DMIC CLK D
		p_gpip_pin->bit_12 = !(1<<(GPIO0_12_INDEX-GPIO0_00_INDEX)); //DMIC D3			
	}

	if (adc_mark->adc_c_dmic_en == TRUE)
	{
		p_gpip_pin->bit_10 = !(!(1<<(GPIO0_10_INDEX-GPIO0_00_INDEX))); //DMIC CLK C
		p_gpip_pin->bit_13 = !(!(1<<(GPIO0_13_INDEX-GPIO0_00_INDEX))); //DMIC D2
	}
	else
	{
		p_gpip_pin->bit_10 = !(1<<(GPIO0_10_INDEX-GPIO0_00_INDEX)); //DMIC CLK C
		p_gpip_pin->bit_13 = !(1<<(GPIO0_13_INDEX-GPIO0_00_INDEX)); //DMIC D2		
	}

	if (adc_mark->adc_b_dmic_en == TRUE)
	{
		p_gpip_pin->bit_16 = !(!(1<<(GPIO0_16_INDEX-GPIO0_00_INDEX))); //DMIC CLK B
		p_gpip_pin->bit_17 = !(!(1<<(GPIO0_17_INDEX-GPIO0_00_INDEX))); //DMIC D1	
	}
	else
	{
		p_gpip_pin->bit_16 = !(1<<(GPIO0_16_INDEX-GPIO0_00_INDEX)); //DMIC CLK B
		p_gpip_pin->bit_17 = !(1<<(GPIO0_17_INDEX-GPIO0_00_INDEX)); //DMIC D1		
	}

	if (adc_mark->adc_a_dmic_en == TRUE)
	{
		p_gpip_pin->bit_18 = !(!(1<<(GPIO0_18_INDEX-GPIO0_00_INDEX))); //DMIC D0
		p_gpip_pin->bit_19 = !(!(1<<(GPIO0_19_INDEX-GPIO0_00_INDEX))); //DMIC CLK A	
	}
	else
	{
		p_gpip_pin->bit_18 = !(1<<(GPIO0_18_INDEX-GPIO0_00_INDEX)); //DMIC D0
		p_gpip_pin->bit_19 = !(1<<(GPIO0_19_INDEX-GPIO0_00_INDEX)); //DMIC CLK A			
	}	

	REG_GPIO4_FUNC = val;
	
	
	val = REG_MISC_CLKGATE;
	pGate->AHB_Audio_Controller_gate = 1;
	pGate->qadc_dac_apb_gate         = 1;	
	REG_MISC_CLKGATE = val;

}



void hw_dmic_init(void)
{
	dmic_write(0x2,0xe);
	dmic_write(0x26,0x80);
	dmic_write(0x27,0x80);
	
	#ifdef AUDIO_IIS_ENABLE 
	dmic_write(0x4,0xE7);
	#else
	dmic_write(0x4,0xE4);
	#endif

	#ifdef ADC_SAMPLE_RATE_48K
	dmic_write(0xa,0x08);
	#else	
	dmic_write(0xa,0x0e);
	#endif

	dmic_write(0x18,0x3);
	dmic_write(0x19,0x9);
	dmic_write(0x16,0xfc);
	dmic_write(0x5,0xe);
	dmic_write(0x26,0x0);
	dmic_write(0x27,0x0);

	#ifdef FULL_TEST

	dmic_write(0x28,0x80);
	dmic_write(0x29,0x80);

	#ifdef AUDIO_IIS_ENABLE 
	dmic_write(0x7,0xE7);
	#else
	dmic_write(0x7,0xE4);
	#endif

	#ifdef ADC_SAMPLE_RATE_48K
	dmic_write(0xb,0x08);
	#else	
	dmic_write(0xb,0x0e);
	#endif

	dmic_write(0x1c,0x3);	
	dmic_write(0x1d,0x9);
	dmic_write(0x1A,0xfc);
	dmic_write(0x8,0xe);
	dmic_write(0x28,0x0);
	dmic_write(0x29,0x0);

	qadc_write(0x2,0xe);
	qadc_write(0x26,0x80);
	qadc_write(0x27,0x80);

	#ifdef AUDIO_IIS_ENABLE 
	qadc_write(0x4,0xE7);
	#else
	qadc_write(0x4,0xE4);
	#endif

	#ifdef ADC_SAMPLE_RATE_48K
	qadc_write(0xa,0x08);
	#else	
	qadc_write(0xa,0x0e);
	#endif

	qadc_write(0x18,0x3);
	qadc_write(0x19,0x9);
	qadc_write(0x16,0xfc);
	qadc_write(0x5,0xe);
	qadc_write(0x26,0x0);
	qadc_write(0x27,0x0);

	qadc_write(0x28,0x80);
	qadc_write(0x29,0x80);

	#ifdef AUDIO_IIS_ENABLE 
	qadc_write(0x7,0xE7);
	#else
	qadc_write(0x7,0xE4);
	#endif

	#ifdef ADC_SAMPLE_RATE_48K
	qadc_write(0xb,0x08);
	#else
	qadc_write(0xb,0x0e);
	#endif

	qadc_write(0x1c,0x3);	
	qadc_write(0x1d,0x9);
	qadc_write(0x1A,0xfc);
	qadc_write(0x8,0xe);
	qadc_write(0x28,0x0);
	qadc_write(0x29,0x0);

	#endif //FULL_TEST
}

 void dac_write(int addr, int val)
{
	int rd;
	q_devices_reg_write(DEVICES_QDAC, addr,val);

    delayms(5);

	rd = q_devices_reg_read(DEVICES_QDAC, addr);	
	if (rd != val)
    {
        printf_error("dac write error!\n");
    }
    else
    {
        printf_info("dac write addr:%x ,val:%x\n",addr,val);
    }	
}


void hw_dac_init(void)
{
    printf_info("before init sr:0x%x,\n", q_devices_reg_read(DEVICES_QDAC, 0));

	#if defined DAC_VOLTAGE_SEL_1P8V
	dac_write(0x05,0x10);//Control Register for ViC //Bit4:1-LOW Voltage, Min=1.8V, Type=2.5V
	#elif defined DAC_VOLTAGE_SEL_3P3V
	// dac_write(0x05,0x00);//Control Register for ViC //Bit4:0-Normal Voltage, Min=3.0V, Type=3.3V
	 dac_write(0x05,0x04);
	#else
	DBG_Assert(FALSE);
	#endif

	dac_write(0x0f,0x80);//soft mute dac1
	dac_write(0x10,0x80);//soft mute dac2

	/*
	 * 0x00	-> -28dB
	 * 0x04	-> -20dB
	 * 0x09	-> -10dB
	 * 0x0E	-> -0dB
	 * 0x11	->  6dB
	 */
	#define GAIN_VAIL 0x0e	// 0x0e--0dB 
	// #define GAIN_VAIL 0x08	//-6dB
	dac_write(0x0C,GAIN_VAIL);//GOA1[4:0] headphone gain setting, default: 0000 = -28dB, by step of 2dB
	dac_write(0x0D,GAIN_VAIL);//GOA2[4:0] headphone gain setting, default: 0000 = -28dB, by step of 2dB
	/*
	addr: 0x0e
	0x08 //DAC A1 and A2 Control Register, Deactivation of DAC A1/A2 channel active
	0x28 //DAC A1 and A2 Control Register, Deactivation of DAC A1 channel active, Deactivation of DAC A2 channel inactive
	0x18 //DAC A1 and A2 Control Register, Deactivation of DAC A1 channel inactive, Deactivation of DAC A2 channel active
	0x38 //DAC A1 and A2 Control Register, Deactivation of DAC A1/A2 channel inactive
	*/
	dac_write(0x0e,0x08);//DAC A1 and A2 Control Register, Deactivation of DAC A1/A2 channel active


	#if ((defined DAC_SAMPLE_RATE_48K)||(defined DAC_SAMPLE_RATE_44P1K))
	dac_write(0x09,0x08);
	#elif (defined DAC_SAMPLE_RATE_16K)
	dac_write(0x09,0x03);
	#elif (defined DAC_SAMPLE_RATE_96K)
	dac_write(0x09,0x0a);
	#elif (defined DAC_SAMPLE_RATE_192K)
	dac_write(0x09,0x0c);
	#elif (defined DAC_SAMPLE_RATE_384K)
	dac_write(0x09,0x0d);
	#elif (defined DAC_SAMPLE_RATE_768K)
	dac_write(0x09,0x0e);
	#else
	DBG_Assert(FALSE);
	#endif

	#ifdef AUDIO_IIS_ENABLE
	// dac_write(0x07,0x27);
	dac_write(0x07,0xe7);
	#else
	dac_write(0x07,0x24);//parallel mode
	#endif

    dac_write(0x0B,0xA0);
	dac_write(0x11,0x40);
	
    dac_write(0x0f,0x7f);//soft unmute dac1
	dac_write(0x10,0x7f);//soft unmute dac2

	

    printf_info("after init sr:0x%x,\n", q_devices_reg_read(DEVICES_QDAC, 0));
}


void hw_adc_init(ADC_MARK_SET *adc_mark)
{
	printf_info("%s:%d,%d,%d,%d,%d,%d\r\r", __func__, 
	adc_mark->adc_a_analog_en, adc_mark->adc_a_dmic_en, adc_mark->adc_b_analog_en, 
	adc_mark->adc_b_dmic_en, adc_mark->adc_c_dmic_en, adc_mark->adc_d_dmic_en);


	if ((adc_mark->adc_a_analog_en == TRUE)&&(adc_mark->adc_a_dmic_en == TRUE))
	{
		printf_error("only adc_a_analog_enable or adc_a_dmic_enable\n\r");
		return;
	}

	if ((adc_mark->adc_b_analog_en == TRUE)&&(adc_mark->adc_b_dmic_en == TRUE))
	{
		printf_error("only adc_b_analog_enable or adc_b_dmic_enable\n\r");
		return;
	}

	if ((adc_mark->adc_a_analog_en == TRUE)||(adc_mark->adc_b_analog_en == TRUE))
	{
		#if ((defined ADC_A_ANALOG_ENABLE)||(defined ADC_B_ANALOG_ENABLE))
		#if defined ADC_VOLTAGE_SEL_1P8V
		qadc_write(0x2,0x10);//Control Register for ViC //Bit4:1-low Voltage, Min=1.8V type=2.5V
		#elif defined ADC_VOLTAGE_SEL_3P3V
		qadc_write(0x2,0x00);//Control Register for ViC //Bit4:0-normal Voltage, Min=3.0V type=3.3V
		#else
		DBG_Assert(FALSE);
		#endif
		#else
		printf_error("have not define ADC_A_ANALOG_ENABLE and ADC_B_ANALOG_ENABLE\n\r");
		#endif
	}
	
	if ((adc_mark->adc_a_dmic_en == TRUE)||(adc_mark->adc_b_dmic_en == TRUE))
	{
		#if ((defined ADC_A_DMIC_ENABLE)||(defined ADC_B_DMIC_ENABLE))
		#if defined ADC_VOLTAGE_SEL_1P8V
		qadc_write(0x2,0x1e);//adc analog in sleep mode, analog power-down, digital active;//Control Register for ViC //Bit4:1-low Voltage, Min=1.8V type=2.5V
		#elif defined ADC_VOLTAGE_SEL_3P3V
		qadc_write(0x2,0xe);//adc analog in sleep mode, analog power-down, digital active;//Control Register for ViC //Bit4:0-normal Voltage, Min=3.0V type=3.3V
		#else
		DBG_Assert(FALSE);
		#endif
		#else
		printf_error("have not define ADC_A_DMIC_ENABLE and ADC_B_DMIC_ENABLE\n\r");
		#endif			
	}

	if ((adc_mark->adc_c_dmic_en == TRUE)||(adc_mark->adc_d_dmic_en == TRUE))
	{
		#if ((defined ADC_C_DMIC_ENABLE)||(defined ADC_D_DMIC_ENABLE))
		#if defined ADC_VOLTAGE_SEL_1P8V
		dmic_write(0x2,0x1e);//adc analog in sleep mode, analog power-down, digital active;//Control Register for ViC //Bit4:1-low Voltage, Min=1.8V type=2.5V
		#elif defined ADC_VOLTAGE_SEL_3P3V
		dmic_write(0x2,0xe);//adc analog in sleep mode, analog power-down, digital active;//Control Register for ViC //Bit4:0-normal Voltage, Min=3.0V type=3.3V
		#else
		DBG_Assert(FALSE);
		#endif
		#else
		printf_error("have not define ADC_A_DMIC_ENABLE and ADC_B_DMIC_ENABLE\n\r");
		#endif	
	}


	delayms(130);

	if (adc_mark->adc_a_analog_en == TRUE)
	{
		#ifdef ADC_A_ANALOG_ENABLE
		#if 0
		qadc_write(0x1e,0x31);//
		qadc_write(0x1f,0x31);//
		#endif

		qadc_write(0x26,0x80);//ADCA1 Gain Control Register
		qadc_write(0x27,0x80);//ADCA2 Gain Control Register
		#if 0
		qadc_write(0x14,0x03); //Wind Noise Filter Control Register
		#endif
		/*
		addr: 0x24
		0x0  //ADC A1 and A2 Control Register, Deactivation of ADC A1/A2 channel active
		0x20 //ADC A1 and A2 Control Register, Deactivation of ADC A1 channel active, Deactivation of ADC A2 channel inactive
		0x10 //ADC A1 and A2 Control Register, Deactivation of ADC A1 channel inactive, Deactivation of ADC A2 channel active
		0x30 //ADC A1 and A2 Control Register, Deactivation of ADC A1/A2 channel inactive
		*/
		qadc_write(0x24,0x0);//ADC A1 and A2 Control Register, Deactivation of ADC A1/A2 channel active

		#ifdef AUDIO_IIS_ENABLE
		qadc_write(0x4,0xe3);//ADCA digital filters audio Interface Control Register
		#else
		qadc_write(0x4,0xE4);//ADCA digital filters audio Interface Control Register
		#endif

		#if ((defined ADC_SAMPLE_RATE_48K)||(defined ADC_SAMPLE_RATE_44P1K))
		qadc_write(0xa,0x08);
		#elif (defined ADC_SAMPLE_RATE_16K)
		qadc_write(0xa,0x03);
		#elif (defined ADC_SAMPLE_RATE_96K)
		qadc_write(0xa,0x0a);
		#elif (defined ADC_SAMPLE_RATE_192K)
		qadc_write(0xa,0x0c);
		#elif (defined ADC_SAMPLE_RATE_384K)
		qadc_write(0xa,0x0d);
		#elif (defined ADC_SAMPLE_RATE_768K)
		qadc_write(0xa,0x0e);
		#else
		DBG_Assert(FALSE);
		#endif

		qadc_write(0x19,0x0);//Microphone interface A1 and A2 control register

		qadc_write(0x5,0x0);//ADCA digital filters audio Interface Control Register 2
		qadc_write(0x2a,0x35);//SNR_OPT_EN

		qadc_write(0x26,0x0);//ADCA1 Gain Control Register
		qadc_write(0x27,0x0);//ADCA2 Gain Control Register
		#else
		printf_error("have not define ADC_A_ANALOG_ENABLE\n\r");
		#endif		
	}

	if (adc_mark->adc_a_dmic_en == TRUE)
	{
		#ifdef ADC_A_DMIC_ENABLE
		//a group
		qadc_write(0x26,0x80);//a1 soft mute active
		qadc_write(0x27,0x80);//a2 soft mute active

		#ifdef AUDIO_IIS_ENABLE
		qadc_write(0x4,0xE7);//ADCA digital filters audio Interface Control Register
		#else
		qadc_write(0x4,0xE4);//ADCA digital filters audio Interface Control Register
		#endif

		#if ((defined ADC_SAMPLE_RATE_48K)||(defined ADC_SAMPLE_RATE_44P1K))
		qadc_write(0xa,0x08);
		#elif (defined ADC_SAMPLE_RATE_16K)
		qadc_write(0xa,0x03);
		#elif (defined ADC_SAMPLE_RATE_96K)
		qadc_write(0xa,0x0a);
		#elif (defined ADC_SAMPLE_RATE_192K)
		qadc_write(0xa,0x0c);
		#elif (defined ADC_SAMPLE_RATE_384K)
		qadc_write(0xa,0x0d);
		#elif (defined ADC_SAMPLE_RATE_768K)
		qadc_write(0xa,0x0e);
		#else
		DBG_Assert(FALSE);
		#endif
		
		qadc_write(0x18,0x3);//div 4, dmicClk 3.072MHz

		qadc_write(0x19,0x9);//a2 no mute, no swap, digital interface; a1 no mute, no swap, digital interface

		qadc_write(0x16,0xfc);//adc a1&a2 channel active, clk active

		qadc_write(0x5,0xe);//audio interface active

		qadc_write(0x26,0x0);//a1 soft unmute
		qadc_write(0x27,0x0);//a2 soft unmute
		#else
		printf_error("have not define ADC_A_DMIC_ENABLE\n\r");		
		#endif
	}

	if (adc_mark->adc_b_analog_en == TRUE)
	{
		#ifdef ADC_B_ANALOG_ENABLE
		qadc_write(0x28,0x80);//ADCB1 Gain Control Register
		qadc_write(0x29,0x80);//ADCB2 Gain Control Register

		/*
		addr: 0x25
		0x0  //ADC A1 and B2 Control Register, Deactivation of ADC B1/A2 channel active
		0x20 //ADC B1 and B2 Control Register, Deactivation of ADC B1 channel active, Deactivation of ADC B2 channel inactive
		0x10 //ADC B1 and B2 Control Register, Deactivation of ADC B1 channel inactive, Deactivation of ADC B2 channel active
		0x30 //ADC B1 and B2 Control Register, Deactivation of ADC B1/B2 channel inactive
		*/
		qadc_write(0x25,0x0);//ADC A1 and B2 Control Register, Deactivation of ADC B1/A2 channel active

		#ifdef AUDIO_IIS_ENABLE
		qadc_write(0x7,0xE7);//ADCB digital filters audio Interface Control Register
		#else
		qadc_write(0x7,0xE4);//ADCB digital filters audio Interface Control Register
		#endif	
		
		#if ((defined ADC_SAMPLE_RATE_48K)||(defined ADC_SAMPLE_RATE_44P1K))
		qadc_write(0xb,0x08);
		#elif (defined ADC_SAMPLE_RATE_16K)
		qadc_write(0xb,0x03);
		#elif (defined ADC_SAMPLE_RATE_96K)
		qadc_write(0xb,0x0a);
		#elif (defined ADC_SAMPLE_RATE_192K)
		qadc_write(0xb,0x0c);
		#elif (defined ADC_SAMPLE_RATE_384K)
		qadc_write(0xb,0x0d);
		#elif (defined ADC_SAMPLE_RATE_768K)
		qadc_write(0xb,0x0e);
		#else
		DBG_Assert(FALSE);
		#endif

		qadc_write(0x1D,0x0);//Microphone interface B1 and B2 control register

		qadc_write(0x8,0x0);//ADCB digital filters audio Interface Control Register 2

		qadc_write(0x28,0x0);//ADCB1 Gain Control Register
		qadc_write(0x29,0x0);//ADCB2 Gain Control Register
		#else
		printf_error("have not define ADC_B_ANALOG_ENABLE\n\r");		
		#endif
	}

	if (adc_mark->adc_b_dmic_en == TRUE)
	{
		#ifdef ADC_B_DMIC_ENABLE
		qadc_write(0x28,0x80);//b1 soft mute active
		qadc_write(0x29,0x80);//b2 soft mute active

		#ifdef AUDIO_IIS_ENABLE
		qadc_write(0x7,0xE7);//ADCB digital filters audio Interface Control Register
		#else
		qadc_write(0x7,0xE4);//ADCB digital filters audio Interface Control Register
		#endif	

		#if ((defined ADC_SAMPLE_RATE_48K)||(defined ADC_SAMPLE_RATE_44P1K))
		qadc_write(0xb,0x08);
		#elif (defined ADC_SAMPLE_RATE_16K)
		qadc_write(0xb,0x03);
		#elif (defined ADC_SAMPLE_RATE_96K)
		qadc_write(0xb,0x0a);
		#elif (defined ADC_SAMPLE_RATE_192K)
		qadc_write(0xb,0x0c);
		#elif (defined ADC_SAMPLE_RATE_384K)
		qadc_write(0xb,0x0d);
		#elif (defined ADC_SAMPLE_RATE_768K)
		qadc_write(0xb,0x0e);
		#else
		DBG_Assert(FALSE);
		#endif

		qadc_write(0x1c,0x3);//dmicClk 3.072MHz

		qadc_write(0x1d,0x9);//b2 no mute, no swap, digital interface; b1 no mute, no swap, digital interface

		qadc_write(0x1A,0xfc);//adc b1&b2 channel active, clk active

		qadc_write(0x8,0xe);//audio interface active

		// #ifdef ACTIVE_HPF
		// qadc_write(0x12,0xFF);//ADCA12 digital High Pass Filter active
		// qadc_write(0x13,0xFF);//ADCA12 digital High Pass Filter active
		// #else
		// qadc_write(0x12,0x0);//ADCA12 digital High Pass Filter inactive
		// qadc_write(0x13,0x0);//ADCA12 digital High Pass Filter inactive
		// #endif

		qadc_write(0x28,0x0);//b1 soft unmute, gian 0dB
		qadc_write(0x29,0x0);//b2 soft unmute
		//qadc_write(0x28,0x40);//b1 soft unmute, gain -64dB
		//qadc_write(0x29,0x40);//b2 soft unmute, gain -64dB
		//qadc_write(0x28,0x41);//b1 soft unmute, gain -63dB
		//qadc_write(0x29,0x41);//b2 soft unmute, gain -63dB
		// qadc_write(0x28,0x20);//b1 soft unmute, gain +32dB
		// qadc_write(0x29,0x20);//b2 soft unmute, gain +32dB
		//qadc_write(0x28,0x3F);//b1 soft unmute, gain +63dB
		//qadc_write(0x29,0x3F);//b2 soft unmute, gain +63dB
		#else
		printf_error("have not define ADC_B_DMIC_ENABLE\n\r");		
		#endif			
	}
	

	if (adc_mark->adc_c_dmic_en == TRUE)
	{
		#ifdef ADC_C_DMIC_ENABLE
		//C Group
		dmic_write(0x26,0x80);//a1 soft mute active
		dmic_write(0x27,0x80);//a2 soft mute active

		#ifdef AUDIO_IIS_ENABLE
		dmic_write(0x4,0xE7);//ADCA digital filters audio Interface Control Register
		#else
		dmic_write(0x4,0xE4);//ADCA digital filters audio Interface Control Register
		#endif

		#if ((defined ADC_SAMPLE_RATE_48K)||(defined ADC_SAMPLE_RATE_44P1K))
		dmic_write(0xa,0x08);
		#elif (defined ADC_SAMPLE_RATE_16K)
		dmic_write(0xa,0x03);
		#elif (defined ADC_SAMPLE_RATE_96K)
		dmic_write(0xa,0x0a);
		#elif (defined ADC_SAMPLE_RATE_192K)
		dmic_write(0xa,0x0c);
		#elif (defined ADC_SAMPLE_RATE_384K)
		dmic_write(0xa,0x0d);
		#elif (defined ADC_SAMPLE_RATE_768K)
		dmic_write(0xa,0x0e);
		#else
		DBG_Assert(FALSE);
		#endif

		dmic_write(0x18,0x3);//div 4, dmicClk 3.072MHz

		dmic_write(0x19,0x9);//a2 no mute, no swap, digital interface; a1 no mute, no swap, digital interface

		dmic_write(0x16,0xfc);//adc a1&a2 channel active, clk active

		dmic_write(0x5,0xe);//audio interface active

		dmic_write(0x26,0x0);//a1 soft unmute
		dmic_write(0x27,0x0);//a2 soft unmute
		#else
		printf_error("have not define ADC_C_DMIC_ENABLE\n\r");	
		#endif			
	}

	if (adc_mark->adc_d_dmic_en == TRUE)
	{
		#ifdef ADC_D_DMIC_ENABLE
		//D Group
		dmic_write(0x28,0x80);//b1 soft mute active
		dmic_write(0x29,0x80);//b2 soft mute active

		#ifdef AUDIO_IIS_ENABLE
		dmic_write(0x7,0xE7);//ADCB digital filters audio Interface Control Register
		#else
		dmic_write(0x7,0xE4);//ADCB digital filters audio Interface Control Register
		#endif	

		#if ((defined ADC_SAMPLE_RATE_48K)||(defined ADC_SAMPLE_RATE_44P1K))
		dmic_write(0xb,0x08);
		#elif (defined ADC_SAMPLE_RATE_16K)
		dmic_write(0xb,0x03);
		#elif (defined ADC_SAMPLE_RATE_96K)
		dmic_write(0xb,0x0a);
		#elif (defined ADC_SAMPLE_RATE_192K)
		dmic_write(0xb,0x0c);
		#elif (defined ADC_SAMPLE_RATE_384K)
		dmic_write(0xb,0x0d);
		#elif (defined ADC_SAMPLE_RATE_768K)
		dmic_write(0xb,0x0e);
		#else
		DBG_Assert(FALSE);
		#endif

		dmic_write(0x1c,0x3);//dmicClk 3.072MHz

		dmic_write(0x1d,0x9);//b2 no mute, no swap, digital interface; b1 no mute, no swap, digital interface

		dmic_write(0x1A,0xfc);//adc b1&b2 channel active, clk active

		dmic_write(0x8,0xe);//audio interface active

		dmic_write(0x28,0x0);//b1 soft unmute
		dmic_write(0x29,0x0);//b2 soft unmute	
		#else
		printf_error("have not define ADC_D_DMIC_ENABLE\n\r");
		#endif	
	}
}


static void vad_write(int addr, int val)
{
	q_devices_reg_write(DEVICES_QVAD,addr,val);
}

void vad_init(void)
{
	vad_write(0x02,0x07);
}

void hw_audio_adc_dac_dmic_control_bus_set(void)
{
	U32 val;

	val = QADC_DMIC_CONFIG_2;
	QADC_DMIC_CONFIG2_AHB *pQadcDmicConfig2 = (QADC_DMIC_CONFIG2_AHB *) &val;
	
	#ifdef AUDIO_ADC_DAC_I2C_BUS
	pQadcDmicConfig2->qadc_dmic_i2c_en = 1;
	pQadcDmicConfig2->qadc_dmic_si2c = enADC_DAC_DMIC_CONTROL_SEL_I2C_BUS;
	#else 
	pQadcDmicConfig2->qadc_dmic_i2c_en = 0;
	pQadcDmicConfig2->qadc_dmic_si2c = enADC_DAC_DMIC_CONRTOL_SEL_AHB_BUS;
	#endif
	
	QADC_DMIC_CONFIG_2 = val;
}

void hw_audio_adc_dac_clk_init(void)
{
	U32 val;
	MISC_CLKGATE_CONTROLs *pGate = (MISC_CLKGATE_CONTROLs *) &val;

	val = REG_MISC_CLKGATE;
	pGate->AHB_Audio_Controller_gate = 1;
	pGate->qadc_dac_apb_gate         = 1;	
	REG_MISC_CLKGATE = val;	

	hw_audio_adc_dac_dmic_control_bus_set();
}

void hw_audio_adc_dac_enable(U8 enable)
{
	U32 val;
	QADC_DMIC_CONFIG2_AHB *pQadcDmicConfig2 = (QADC_DMIC_CONFIG2_AHB *) &val;

	val = QADC_DMIC_CONFIG_2;
	pQadcDmicConfig2->qadc_dac_en = enable;
	QADC_DMIC_CONFIG_2 = val;
}

U8 adc_mark_status_check(U8 adc_en_mark)
{
	U8 err_ret = FALSE;
	if (global_vari.audio_source == SOURCE_AUX)
	{
		#ifdef AUX_MODE_INPUT_FROM_ADC_A_ANALOG
		if (!(adc_en_mark & ADC_A_ANALOG_MARK))
		{
			printf_error("AUX_MODE ADC_A_ANALOG_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef AUX_MODE_INPUT_FROM_ADC_A_DMIC
		if (!(adc_en_mark & ADC_A_DMIC_MARK))
		{
			printf_error("AUX_MODE ADC_A_DMIC_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef AUX_MODE_INPUT_FROM_ADC_B_ANALOG
		if (!(adc_en_mark & ADC_B_ANALOG_MARK))
		{
			printf_error("AUX_MODE ADC_B_ANALOG_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef AUX_MODE_INPUT_FROM_ADC_B_DMIC
		if (!(adc_en_mark & ADC_B_DMIC_MARK))
		{
			printf_error("AUX_MODE ADC_B_DMIC_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef AUX_MODE_INPUT_FROM_ADC_C_DMIC
		if (!(adc_en_mark & ADC_C_DMIC_MARK))
		{
			printf_error("AUX_MODE ADC_C_DMIC_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef AUX_MODE_INPUT_FROM_ADC_D_DMIC
		if (!(adc_en_mark & ADC_D_DMIC_MARK))
		{
			printf_error("AUX_MODE ADC_D_DMIC_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif		
	}

	if (global_vari.audio_source == SOURCE_USB_SPEAKER)
	{
		#ifdef USB_DEV_MIC_INPUT_FROM_ADC_A_ANALOG
		if (!(adc_en_mark & ADC_A_ANALOG_MARK))
		{
			printf_error("USB_DEV_MIC ADC_A_ANALOG_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef USB_DEV_MIC_INPUT_FROM_ADC_A_DMIC
		if (!(adc_en_mark & ADC_A_DMIC_MARK))
		{
			printf_error("USB_DEV_MIC ADC_A_DMIC_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef USB_DEV_MIC_INPUT_FROM_ADC_B_ANALOG
		if (!(adc_en_mark & ADC_B_ANALOG_MARK))
		{
			printf_error("USB_DEV_MIC ADC_B_ANALOG_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef USB_DEV_MIC_INPUT_FROM_ADC_B_DMIC
		if (!(adc_en_mark & ADC_B_DMIC_MARK))
		{
			printf_error("USB_DEV_MIC ADC_B_DMIC_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef USB_DEV_MIC_INPUT_FROM_ADC_C_DMIC
		if (!(adc_en_mark & ADC_C_DMIC_MARK))
		{
			printf_error("USB_DEV_MIC ADC_C_DMIC_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif
		#ifdef USB_DEV_MIC_INPUT_FROM_ADC_D_DMIC
		if (!(adc_en_mark & ADC_D_DMIC_MARK))
		{
			printf_error("USB_DEV_MIC ADC_D_DMIC_MARK!\n\r");
			err_ret = TRUE;
		}
		#endif		
	}


	return err_ret;
}


void hw_audio_adc_init(U8 adc_en_mark)
{
	U32 val;
	QADC_DMIC_CONFIG_AHB *pQadcDmicConfig = (QADC_DMIC_CONFIG_AHB *) &val;
	QADC_DMIC_CONFIG2_AHB *pQadcDmicConfig2 = (QADC_DMIC_CONFIG2_AHB *) &val;

	U8 adc_en_mark_bck = adc_en_mark;
	ADC_MARK_SET *adc_mark = (ADC_MARK_SET *) &adc_en_mark_bck;

	printf_info("%s:%d,%d,%d,%d,%d,%d\r\r", __func__, 
	adc_mark->adc_a_analog_en, adc_mark->adc_a_dmic_en, adc_mark->adc_b_analog_en, 
	adc_mark->adc_b_dmic_en, adc_mark->adc_c_dmic_en, adc_mark->adc_d_dmic_en);

	U8 err_ret = adc_mark_status_check(adc_en_mark_bck);
	if (err_ret == TRUE)
	{
		return;
	}

	dmic_gpio_pin_init(adc_mark);

    val = QADC_DMIC_CONFIG_2;
	
	#ifdef AUDIO_IIS_ENABLE
	pQadcDmicConfig2->qadc_A_dmic_parallel = 0;
	pQadcDmicConfig2->qadc_B_dmic_parallel = 0;
	pQadcDmicConfig2->qadc_C_dmic_parallel = 0;
	pQadcDmicConfig2->qadc_D_dmic_parallel = 0;
	pQadcDmicConfig2->qadc_mi2s_fifo_en = 1; //i2s mode
	pQadcDmicConfig2->qadc_i2s_clk_en = 1;	
	#else
	pQadcDmicConfig2->qadc_A_dmic_parallel = 1;
	pQadcDmicConfig2->qadc_B_dmic_parallel = 1;
	pQadcDmicConfig2->qadc_C_dmic_parallel = 1;
	pQadcDmicConfig2->qadc_D_dmic_parallel = 1;
	pQadcDmicConfig2->qadc_mi2s_fifo_en = 0; //parallel mode	
	#endif

	#ifdef AUDIO_IIS_ENABLE
	pQadcDmicConfig2->qadc_dmic_lro_div = 15;//127; //96k
	#else
	pQadcDmicConfig2->qadc_dmic_lro_div = 15; //48k, for parallel
	#endif

	pQadcDmicConfig2->qadc_dmic_rstn = 1;
	pQadcDmicConfig2->qadc_dmic_vdd_ok = 1;
	pQadcDmicConfig2->qadc_dmic_avdd_ok = 1;
	pQadcDmicConfig2->qadc_dmic_force_sleep_ana = 0;
	pQadcDmicConfig2->qadc_dac_en = 1;

	if ((adc_mark->adc_c_dmic_en == TRUE)||(adc_mark->adc_d_dmic_en == TRUE))
	{
		#if ((defined ADC_C_DMIC_ENABLE)||(defined ADC_D_DMIC_ENABLE))
		pQadcDmicConfig2->qadc2_dmic_rstn = 1;
		pQadcDmicConfig2->qadc2_dmic_vdd_ok = 1;
		pQadcDmicConfig2->qadc2_dmic_avdd_ok = 1;
		pQadcDmicConfig2->qadc2_dmic_force_sleep_ana = 0;
		#else
		printf_error("have not define ADC_C_DMIC_ENABLE or ADC_D_DMIC_ENABLE\n\r");
		#endif	
	}

	pQadcDmicConfig2->qadc_clk_en = 1;
	QADC_DMIC_CONFIG_2 = val;


	pQadcDmicConfig2->qadc_dmic_vdd_ok = 1;
	pQadcDmicConfig2->qadc_dmic_avdd_ok = 1;


	if ((adc_mark->adc_c_dmic_en == TRUE)||(adc_mark->adc_d_dmic_en == TRUE))
	{
		#if ((defined ADC_C_DMIC_ENABLE)||(defined ADC_D_DMIC_ENABLE))
		pQadcDmicConfig2->qadc2_dmic_vdd_ok = 1;
		pQadcDmicConfig2->qadc2_dmic_avdd_ok = 1;
		//pQadcDmicConfig2->qadc_dmic_si2c = 1;
		#else
		printf_error("have not define ADC_C_DMIC_ENABLE or ADC_D_DMIC_ENABLE\n\r");
		#endif
	}
	QADC_DMIC_CONFIG_2 = val;


	val = QADC_DMIC_CONFIG;

	#ifdef ADC_SAMPLE_RATE_48K
	//for anc 768k
	pQadcDmicConfig->qadc_A_768k = 0;
	pQadcDmicConfig->qadc_B_768k = 0;
	pQadcDmicConfig->qadc_C_768k = 0;
	pQadcDmicConfig->qadc_D_768k = 0;
	#else
	//for anc 768k
	pQadcDmicConfig->qadc_A_768k = 1;
	pQadcDmicConfig->qadc_B_768k = 1;
	pQadcDmicConfig->qadc_C_768k = 1;
	pQadcDmicConfig->qadc_D_768k = 1;
	#endif

	#if defined ADC_DAC_DATA_WIDTH_24_BIT
	pQadcDmicConfig->qadc_fifo_24b = 1;
	#elif defined ADC_DAC_DATA_WIDTH_16_BIT
	pQadcDmicConfig->qadc_fifo_24b = 0;
	#else
	DBG_Assert(FALSE);
	#endif

	if ((adc_mark->adc_a_analog_en == TRUE)||(adc_mark->adc_a_dmic_en == TRUE))
	{
		pQadcDmicConfig->qadc_A_fifo_en = enADC_FIFO_EANBLE;		
	}
	else
	{
		pQadcDmicConfig->qadc_A_fifo_en = enADC_FIFO_DISABLE;
	}

	if ((adc_mark->adc_b_analog_en == TRUE)||(adc_mark->adc_b_dmic_en == TRUE))
	{
		pQadcDmicConfig->qadc_B_fifo_en = enADC_FIFO_EANBLE;		
	}
	else
	{
		pQadcDmicConfig->qadc_B_fifo_en = enADC_FIFO_DISABLE;
	}

	if (adc_mark->adc_c_dmic_en == TRUE)
	{
		pQadcDmicConfig->qadc_C_fifo_en = enADC_FIFO_EANBLE;		
	}
	else
	{
		pQadcDmicConfig->qadc_C_fifo_en = enADC_FIFO_DISABLE;
	}

	if (adc_mark->adc_d_dmic_en == TRUE)
	{
		pQadcDmicConfig->qadc_D_fifo_en = enADC_FIFO_EANBLE;		
	}
	else
	{
		pQadcDmicConfig->qadc_D_fifo_en = enADC_FIFO_DISABLE;
	}

	QADC_DMIC_CONFIG = val;

    hw_adc_init(adc_mark);	
}


void hw_audio_dac_init(void)
{
	U32 val;
	
	MISC_BTDM_CONTROLs *btdm_config;
	btdm_config = (MISC_BTDM_CONTROLs*) &val;
	val = REG_MISC_BTDM;
	btdm_config->audio_dac_d_sel = DAC_AUDIO_OUT_FROM_IIS_OUT_DATA_CH;
	REG_MISC_BTDM = val;
	QADC_DMIC_CONFIG_AHB *pQadcDmicConfig = (QADC_DMIC_CONFIG_AHB *) &val;

	val = QADC_DMIC_CONFIG;
	#ifdef AUDIO_IIS_ENABLE
	pQadcDmicConfig->qadc3_parallel = 0;
	#else
	pQadcDmicConfig->qadc3_parallel = 1;
	#endif

	pQadcDmicConfig->qadc3_dmic_rstn = 1;
	pQadcDmicConfig->qadc3_dmic_vdd_ok = 1;
	pQadcDmicConfig->qadc3_dmic_avdd_ok = 1;
	pQadcDmicConfig->qadc3_dmic_force_sleep_ana = 1; // if == 0, D group DMIC clk pin have not out clk

	QADC_DMIC_CONFIG = val;

	hw_dac_init();
}
