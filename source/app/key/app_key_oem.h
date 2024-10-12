#ifndef _APP_KEY_OEM_
#define _APP_KEY_OEM_



#define V_TOLERANCE 0x16  // it is about 0.08v

static const KEY_AD_ENTRY oem_main_kbd_entry0[] = {

	{(0x000 - 0x000), (0x000 + V_TOLERANCE ), APP_KEY_SOURCE},  		//Source
	{(0x03D - V_TOLERANCE ), (0x03E + V_TOLERANCE ), APP_KEY_PLAY},    	//Play/Pause
	{(0x09F - V_TOLERANCE ), (0x0A0 + V_TOLERANCE ), APP_KEY_STOP},    	//STOP
	{(0x100 - V_TOLERANCE ), (0x102 + V_TOLERANCE ), APP_KEY_SKIPF},   	//skipF
	{(0x16C - V_TOLERANCE ), (0x16E + V_TOLERANCE ), APP_KEY_SKIPB},   	//skipB
	{(0x1D6 - V_TOLERANCE ), (0x1D7 + V_TOLERANCE ), APP_KEY_PLUS},    	//Vol +
	{(0x248 - V_TOLERANCE ), (0x249 + V_TOLERANCE ), APP_KEY_MINUS},   	//Vol -
	{(0x2C1 - V_TOLERANCE ), (0x2C2 + V_TOLERANCE ), APP_KEY_EQ},      	//EQ0

	{(0x33A - V_TOLERANCE ), (0x33C + V_TOLERANCE ), APP_KEY_EQ},      	//EQ1
	{(0x3AE - V_TOLERANCE ), (0x3AF + V_TOLERANCE ), APP_KEY_EQ},      	//EQ2
    /*
    * {(0x341 - V_TOLERANCE ), (0x348 + V_TOLERANCE ), APP_KEY_EQ},      	//EQ3
    * {(0x3AC - V_TOLERANCE ), (0x3B2 + V_TOLERANCE ), APP_KEY_TIMER},   	//timer
    * * */
	
	{ADC_INVALID_VAL, 0xFD0,           APP_KEY_INVALID}, //End Flag	
};

static const KEY_AD_ENTRY oem_main_kbd_entry1[] = {
	{ADC_INVALID_VAL, 0xffff,APP_KEY_INVALID},		//End Flag
};

static const KEY_AD_ENTRY oem_main_kbd_entry2[] = {
	{ADC_INVALID_VAL, 0xffff,APP_KEY_INVALID},		//end glag				
};

static const KEY_GPIO_ENTRY oem_main_gpio_kbd_entry[] = {
	/*
	 * example
	 * {GPIO_UNDEFINE_INDEX, GPIO_LOW, APP_KEY_INVALID},
	 */
	/*
    {GPIO0_11_INDEX, GPIO_LOW, APP_KEY_SOURCE},
	{GPIO0_10_INDEX, GPIO_LOW, APP_KEY_PLAY},
	*/
	// {GPIO0_26_INDEX, GPIO_LOW, APP_KEY_PLUS},

	// {GPIO0_25_INDEX, GPIO_LOW, APP_KEY_MINUS},

	// {GPIO0_20_INDEX, GPIO_LOW, APP_KEY_TVOUT},//F_SW
	{GPIO0_27_INDEX, GPIO_LOW, APP_KEY_SOURCE}, //pair_key


	// {GPIO0_26_INDEX, GPIO_LOW, APP_KEY_FOLDER},//HPM_SW
	// {GPIO0_20_INDEX, GPIO_LOW, APP_KEY_ALARM1},//F_SW
	// {GPIO0_25_INDEX, GPIO_LOW, APP_KEY_ALARM2},//PHASE_SW
	// {GPIO0_24_INDEX, GPIO_LOW, APP_KEY_DISC_CHANGE},//FRO_Sw

};

#ifdef POWER_KEY_ENABLE
static const KEY_GPIO_ENTRY oem_main_sreg_gpio_kbd_entry[] = {
	/*
	 * example
	 * {SREG_PWR_INDEX, GPIO_LOW, APP_KEY_INVALID},
	 */
	
    {SREG_PWR_INDEX, GPIO_LOW, APP_KEY_POWER},
};
#endif

/*
Examples:
	1, dual key mix:
		{APP_KEY_PLAY, APP_KEY_SOURCE, APP_KEY_MIX_KEY_PS},
	2, multiple key mix:
		{APP_KEY_PLAY, APP_KEY_SOURCE, APP_KEY_MIX_KEY_PS},	
		{APP_KEY_MIX_KEY_PS, APP_KEY_STOP , APP_KEY_MIX_KEY_PSS},
		{APP_KEY_MIX_KEY_PSS, APP_KEY_POWER , APP_KEY_MIX_KEY_PSSP},

PLEASE NOTE: Two mixing keys cannot be mixed. For examples:	following multiple key mix is forbidden
		{APP_KEY_PLAY, APP_KEY_SOURCE, APP_KEY_MIX_KEY_PS},	
		{APP_KEY_STOP, APP_KEY_POWER, APP_KEY_MIX_KEY_SP},
		{APP_KEY_MIX_KEY_PS, APP_KEY_MIX_KEY_SP , APP_KEY_MIX_KEY_PSSP},
*/

static const KEY_MIX_ENTRY oem_main_mix_entry[] = {
	{APP_KEY_PLAY, APP_KEY_SOURCE, APP_KEY_MODE},	
	{APP_KEY_MODE, APP_KEY_STOP ,APP_KEY_MENU},
};


#define REMOTE_NEC_FORMAT

const KEY_REMOTE_ENTRY oem_remote_entry[] = {
#ifdef REMOTE_NEC_FORMAT
	{APP_KEY_POWER,         0x807f807f},  //Power
	{APP_KEY_PLAY,          0x807fe01f},  //Play
	{APP_KEY_SOURCE,        0x807f00ff},  //SOURCE
	{APP_KEY_SKIPF,         0x807f609f},  //Skipf
	{APP_KEY_SKIPB,         0x807f00ff},  //Skipb
	{APP_KEY_PLUS,          0x807fc03f},  //Vol+
	{APP_KEY_MINUS,         0x807fd02f},  //Vol-	
	{APP_KEY_BASS_TREBLE,   0x807f20df},  //BASS+
	{APP_KEY_BASS_TREBLE,   0x807f906f},  //BASS-
	{APP_KEY_BASS_TREBLE,   0x807f40bf},  //TREBLE+
	{APP_KEY_BASS_TREBLE,   0x807f10ef},  //TREBLE-

#elif defined REMOTER_SHARP_AUDIO_FORMAT
	{APP_KEY_POWER,         0xf1ae0885},  //Power
	{APP_KEY_PLAY,          0xf1ae6289},  //Play
	{APP_KEY_SOURCE,        0xFFFFFFFF},  //SOURCE
	{APP_KEY_SKIPF,         0xf1ae3688},  //Skipf
	{APP_KEY_SKIPB,         0xf1aeb680},  //Skipb
	{APP_KEY_PLUS,          0xf1ae0489},  //Vol+
	{APP_KEY_MINUS,         0xf1ae8481},  //Vol-	
	{APP_KEY_BASS_TREBLE,   0xf1ae248b},  //BASS+
	{APP_KEY_BASS_TREBLE,   0xFFFFFFFF},  //BASS-
	{APP_KEY_BASS_TREBLE,   0xf1ae248b},  //TREBLE+
	{APP_KEY_BASS_TREBLE,   0x807f10ef},  //TREBLE-

#elif defined REMOTE_PHILIPS_FORMAT////PHILIPS soundba remoter
	{APP_KEY_POWER,         0x0000110C},  //Power
	{APP_KEY_PLAY,          0x00001135},  //Play
	{APP_KEY_SOURCE,        0xFFFFFFFF},  //SOURCE
	{APP_KEY_SKIPF,         0x00001120},  //Skipf
	{APP_KEY_SKIPB,         0x00001121},  //Skipb
	{APP_KEY_PLUS,          0x00001010},  //Vol+
	{APP_KEY_MINUS,         0x00001011},  //Vol-	
	{APP_KEY_BASS_TREBLE,   0x00001016},  //BASS+
	{APP_KEY_BASS_TREBLE,   0x00001017},  //BASS-
	{APP_KEY_BASS_TREBLE,   0x00001018},  //TREBLE+
	{APP_KEY_BASS_TREBLE,   0x00001019},  //TREBLE-

#endif
};

#endif// _APP_KEY_OEM_
