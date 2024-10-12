#ifndef _ACM8625_ACM8625_H
#define _ACM8625_ACM8625_H

//#include "ext_ACM8625_inner.h"


typedef struct
{
	U8 Address;
	U8 Data;
}ACM_REG_TAB;


typedef struct {
        U8 offset;
        U8 value;
} cfg_reg;




//0x58:4.7k pull-up and DEV_NUM bit default 1,use 4 address
//0x5c:15k pull-up and DEV_NUM bit default 1,use 4 address
#define ACM8625_ADDR	(0x58) //(0x31)  //4.7k pull-up and DEV_NUM bit default 1,use 4 address
//#define ACM8625_ADDR_MONO	(0x5a>>1) //(0x31)  //15k pull-up and DEV_NUM bit default 1,use 4 address
#define ACM8625_ADDR_MONO	(0x5c) //(0x31)  //

#define MAX_EQ_MODE                             (0x04)




enum
{
	ACM8625_EQ_MUSIC,
	ACM8625_EQ_MOVIE,
	ACM8625_EQ_NEWS,
	ACM8625_EQ_FLAT
};


#define ACM8625_STBY_PIN		  GPIO0_15_INDEX
#define ACM8625_STBY_PIN_INIT     hal_gpio_mux_func_select(ACM8625_STBY_PIN,MUX_SEL_GPIO_OUTPUT)
//set gpio pin to high level
#define ACM8625_STBY_PIN_HIGH     hal_gpio_output_level_set(ACM8625_STBY_PIN,GPIO_HIGH)
//set gpio pin to low level
#define ACM8625_STBY_PIN_LOW      hal_gpio_output_level_set(ACM8625_STBY_PIN,GPIO_LOW)

//GPIO setting for ACM8625 chip pins
#define ACM8625_RESET_PIN          GPIO0_26_INDEX
//init gpio pin to output mode
#define ACM8625_RESET_PIN_INIT     hal_gpio_mux_func_select(ACM8625_RESET_PIN,MUX_SEL_GPIO_OUTPUT)
//set gpio pin to high level
#define ACM8625_RESET_PIN_HIGH     hal_gpio_output_level_set(ACM8625_RESET_PIN,GPIO_HIGH)
//set gpio pin to low level
#define ACM8625_RESET_PIN_LOW      hal_gpio_output_level_set(ACM8625_RESET_PIN,GPIO_LOW)



void Audio_Dev_ACM8625_Init(void);
void Audio_Dev_ACM8625_Open(void);
void Audio_Dev_ACM8625_Set_Channel(U8 channel);
void Audio_Dev_ACM8625_Set_Volume(U8 volume);
//void Audio_Dev_ACM8625_Set_Speaker(U8 speaker, U8 attenuation);
void Audio_Dev_ACM8625_Set_Bass_Treble(U8 bass, U8 treble);
void Audio_Dev_ACM8625_Set_EQ(U8 eq_mode);
void Audio_Dev_ACM8625_Set_Mute(U8 con);
void Audio_Dev_ACM8625_Set_AM_Mode(U8 con, U32 kFreq);



#define AUDIO_DEV_ACM8625       Audio_Dev_ACM8625_Init, \
								Audio_Dev_ACM8625_Open,	\
								Audio_Dev_ACM8625_Set_Channel, \
								Audio_Dev_ACM8625_Set_Volume, \
								Audio_Dev_ACM8625_Set_Bass_Treble, \
								Audio_Dev_ACM8625_Set_EQ, \
								Audio_Dev_ACM8625_Set_Mute,\
								Audio_Dev_ACM8625_Set_AM_Mode  //,\
								//Audio_Dev_NTP8928_Set_Speaker

#endif
