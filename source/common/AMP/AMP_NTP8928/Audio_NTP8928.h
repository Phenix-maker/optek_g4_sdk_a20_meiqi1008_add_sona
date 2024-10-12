#ifndef __AUDIO_NTP8928__
#define __AUDIO_NTP8928__


//GPIO setting for NTP8928 chip pins
#define AMP_RESET_PIN          GPIO0_25_INDEX
//init gpio pin to output mode
#define AMP_RESET_PIN_INIT     hal_gpio_mux_func_select(AMP_RESET_PIN,MUX_SEL_GPIO_OUTPUT)
//set gpio pin to high level
#define AMP_RESET_PIN_HIGH     hal_gpio_output_level_set(AMP_RESET_PIN,GPIO_HIGH)
//set gpio pin to low level
#define AMP_RESET_PIN_LOW      hal_gpio_output_level_set(AMP_RESET_PIN,GPIO_LOW)




#define DEV_PHY_ADDR 0x56



void Audio_Dev_NTP8928_Init(void);
void Audio_Dev_NTP8928_Open(void);
void Audio_Dev_NTP8928_Set_Channel(U8 channel);
void Audio_Dev_NTP8928_Set_Volume(U8 volume);
void Audio_Dev_NTP8928_Set_Speaker(U8 speaker, U8 attenuation);
void Audio_Dev_NTP8928_Set_Bass_Treble(U8 bass, U8 treble);
void Audio_Dev_NTP8928_Set_eq_mode(U8 mode);
void Audio_Dev_NTP8928_Set_Mute(U8 con);
void Audio_Dev_NTP8928_Set_AM_Mode(U8 con, U32 kFreq);


#define AUDIO_DEV_NTP8928       Audio_Dev_NTP8928_Init, \
								Audio_Dev_NTP8928_Open,	\
								Audio_Dev_NTP8928_Set_Channel, \
								Audio_Dev_NTP8928_Set_Volume, \
								Audio_Dev_NTP8928_Set_Bass_Treble, \
								Audio_Dev_NTP8928_Set_eq_mode, \
								Audio_Dev_NTP8928_Set_Mute, \
								Audio_Dev_NTP8928_Set_AM_Mode  //,\
								//Audio_Dev_NTP8928_Set_Speaker

#endif /* __AUDIO_NTP8928__ */
