#include "os_config.h"

#include "hw_timer.h"
#include "audio_device.h"
#include "app_i2c.h"
#include "app_i2c0.h"
#include "printf.h"

#ifdef AUDIO_ACM8625
#include "amp_acm8625.h"
//#include "amp_interface.h"

//static struct device *AMP_I2C_DEV=NULL;


//#define AUDIO_ACM8625_FUNCTION_ENABLE

typedef enum{
AMP_STEREO = 0,
AMP_MONO,

}amp_type;


struct REG_TAB {
        U8 Addr;
        U8 Data;
};

typedef enum
{
    AMP_EQ_RESERVE,
    AMP_EQ_NOR = 1,
    AMP_EQ_MUSIC,
    AMP_EQ_MOVIE,			
    AMP_EQ_NEWS,	
    
    AMP_EQ_SPORT,
   AMP_EQ_NIGHT,
   AMP_EQ_CUSTOM,
   
    AMP_EQ_MAX,

}amp_eq_type_e;



typedef enum {
	AMP_AUDIO_STREAM_DEFAULT = 1,
	AMP_AUDIO_STREAM_MUSIC,
	AMP_AUDIO_STREAM_LOCAL_MUSIC,
	AMP_AUDIO_STREAM_TTS,
	AMP_AUDIO_STREAM_VOICE,
	AMP_AUDIO_STREAM_LINEIN,
	AMP_AUDIO_STREAM_LINEIN_MIX,
	AMP_AUDIO_STREAM_SUBWOOFER,
	AMP_AUDIO_STREAM_ASR,
	AMP_AUDIO_STREAM_AI,
	AMP_AUDIO_STREAM_USOUND,
	AMP_AUDIO_STREAM_USOUND_MIX,
	AMP_AUDIO_STREAM_USPEAKER,
	AMP_AUDIO_STREAM_I2SRX_IN,
	AMP_AUDIO_STREAM_SPDIF_IN,
	AMP_AUDIO_STREAM_GENERATE_IN,
	AMP_AUDIO_STREAM_GENERATE_OUT,
	AMP_AUDIO_STREAM_LOCAL_RECORD,
	AMP_AUDIO_STREAM_GMA_RECORD,
	AMP_AUDIO_STREAM_BACKGROUND_RECORD,
	AMP_AUDIO_STREAM_MIC_IN,
	AMP_AUDIO_STREAM_FM,
	AMP_AUDIO_STREAM_TWS,

	AMP_AUDIO_STREAM_ARC_IN,
	AMP_AUDIO_STREAM_HDMI0,
	AMP_AUDIO_STREAM_HDMI1,
	AMP_AUDIO_STREAM_HDMI2,

#if(CONFIG_SUPPORT_MULTI_AIN_IN_SELECT == 1)
	AMP_AUDIO_STREAM_LINEIN_1,
	AMP_AUDIO_STREAM_LINEIN_2,
#endif

}amp_stream_type_e;

typedef enum{
	
	AMP_CTRL1=0x01,
	AMP_CTRL2=0x02,
	AMP_CTRL3=0x03,
	STATE_CTRL=0x04,
	PROCESSING_CTRL1=0x05,
	PROCESSING_CTRL2=0x06,
	I2S_DATA_FORMAT1=0x07,
	I2S_DATA_FORMAT2 =0x08,
	I2S_DATA_FORMAT3 =0x09,
	GPIO2_CTRL =0x0A,
	GPIO1_CTRL =0x0B,
	GPIO1_FAULT_SEL = 0x0C,
	GPIO2_FAULT_SEL =0x0D,
	SS_CTRL =0x0E,
	VOLUME_CTRL_L = 0x0F,
	VOLUME_CTRL_R =0x10,
	MSIC_CTRL = 0x11,
	I2S_CLK_FORMAT_RPT1  = 0x12,
	I2S_CLK_FORMAT_RPT2  = 0x13,
	DIEID_RPT =0x15,
	STATE_RPT =0x16,
	FAULT_RPT1 = 0x17,
	FAULT_RPT2 = 0x18,
	FAULT_RPT3 =0x19,
	XOR_CHECKSUM =0x7E,
	CRC_CHECKSUM =0x7F,
	
}eACM8625REG;


//***********************ACM8625 is able to provide -110dB to 48dB volume, current driver only inlcudes normal 16 sets volume table***********************/

const U8 ACM8625VolTable[] =
{ 
/* 0.0db; Reg value:*/   0x00, 0x80, 0x00, 0x00, 
/*-2.5db; Reg value:*/   0x00, 0x5f, 0xfc, 0x89, 
/*-5.5db; Reg value:*/   0x00, 0x43, 0xf4, 0x05, 
/*-8.0db; Reg value:*/	 0x00, 0x32, 0xf5, 0x2d, 
/*-10.5db; Reg value:*/  0x00, 0x26, 0x36, 0x80, 
/*-13.5db; Reg value:*/  0x00, 0x1b, 0x0d, 0x7b, 
/*-16.0db; Reg value:*/  0x00, 0x14, 0x49, 0x61, 
/*-18.5db; Reg value:*/  0x00, 0x0f, 0x36, 0x7c, 
/*-21.5db; Reg value:*/  0x00, 0x0a, 0xc5, 0x15, 
/*-24.0db; Reg value:*/  0x00, 0x08, 0x13, 0x85, 
/*-26.5db; Reg value:*/  0x00, 0x06, 0x0e, 0x6c, 
/*-30.5db; Reg value:*/  0x00, 0x03, 0xd2, 0x40, 
/*-34.5db; Reg value:*/  0x00, 0x02, 0x69, 0x3c, 
/*-44.5db; Reg value:*/  0x00, 0x00, 0xc3, 0x30, 
/*-52.5db; Reg value:*/  0x00, 0x00, 0x4d, 0xb5, 
/*-58.5db; Reg value:*/  0x00, 0x00, 0x26, 0xf2, 
/*-109.5db; Reg value:*/ 0x00, 0x00, 0x00, 0x1c, 
};


//NORMAL  0db
const U8 ACM8625Volume_0db[]={

	{ 0x00, 0x04 },
	{ 0x7c, 0x00 },
	{ 0x7d, 0x80 },
	{ 0x7e, 0x00 },
	{ 0x7f, 0x00 },
	{ 0x80, 0x00 },
	{ 0x81, 0x80 },
	{ 0x82, 0x00 },
	{ 0x83, 0x00 },
};


//USE FOR USB
const U8 ACM8625Volume_9db[]={

	{ 0x00, 0x04 },
	{ 0x7c, 0x01 },
	{ 0x7d, 0x68 },
	{ 0x7e, 0xc0 },
	{ 0x7f, 0xc6 },
	{ 0x80, 0x01 },
	{ 0x81, 0x68 },
	{ 0x82, 0xc0 },
	{ 0x83, 0xc6 },
	};

const U8 ACM8625Volume_14db[]={

	  //  Pre volume	14db  sensitivity
	{ 0x00, 0x04 },
	{ 0x7c, 0x02 },
	{ 0x7d, 0x81 },
	{ 0x7e, 0x85 },
	{ 0x7f, 0x08 },
	{ 0x80, 0x02 },
	{ 0x81, 0x81 },
	{ 0x82, 0x85 },
	{ 0x83, 0x08 },

	};



const U8 ACMVolumeTable[]={     
#if 0
	0x00,	//-104db	0
	0x58,	//-60db 1
	0x5e,	//-57db 2
	0x64,	//-54db	3
	0x6a,	//-51db	4
	0x70,	//-48db	5
	0x76,	//-45db	6
	0x7c,	//-42db	7
	0x82,	//-39db	8
	0x88,	//-36db	9
	0x8e,	//-33db	10
	0x94,	//-30db	11
	0x98,	//-28db	12
	0x9c,	//-26db	13
	0xa0,	//-24db	14
	0xa4,	//-22db	15
	0xa8,	//-20db	16
	0xac,	//-18db 17
	0xb0,	//-16db	18
	0xb4,	//-14db	19
	0xb8,	//-12db	20
	0xbc,	//-10db	21
	0xc0,	//-8db	22
	0xc2,	//-7db	23
	0xc4,	//-6db	24
	0xc6,	//-5db	25
	0xc8,	//-4db	26
	0xca,	//-3db  27
	0xcc,	//-2db  28
	0xce,	//-1 db 29
	0xd0,  //0db   30
#else

	0x00, // 0  mute
	0x60, // 1 -56 dB
	0x66, // 2 -53 dB
	0x6c, // 3 -50 dB
	0x70, // 4 -48 dB
	0x74, // 5 -46 dB
	0x78, // 6 -44 dB
	0x7c, // 7 -42 dB
	0x80, // 8 -40 dB
	0x84, // 9 -38 dB
	0x88, // 10 -36 dB
	0x8c, // 11 -34 dB
	0x8e, // 12 -33 dB
	0x90, // 13 -32 dB
	0x92, // 14 -31 dB
	0x94, // 15 -30 dB
	0x96, // 16 -29 dB
	0x98, // 17 -28 dB
	0x9a, // 18 -27 dB
	0x9c, // 19 -26 dB
	0x9e, // 20 -25 dB 
	0xa0, // 21 -24 dB
	0xa2, // 22 -23 dB
	0xa4, // 23 -22 dB
	0xa6, // 24 -21 dB
	0xa8, // 25 -20 dB
	0xaa, // 26 -19 dB
	0xac, // 27 -18 dB
	0xae, // 28 -17 dB
	0xb0, // 29 -16 dB
	0xb2, // 30 -15 dB
	0xb4, // 31 -14 dB
	0xb6, // 32 -13 dB
	0xb8, // 33 -12 dB
	0xba, // 34 -11 dB
	0xbc, // 35 -10 dB
	0xbe, // 36 -9 dB
	0xc0, // 37 -8 dB
	0xc2, // 38 -7 dB
	0xc4, // 39 -6 dB
	0xc6, // 40 -5 dB
	0xc8, // 41 -4dB
	0xca, // 42 -3 dB
	0xcc, // 43 -2dB
	0xce, // 44 -1 dB
	0xd0, // 45 0 dB

#endif
};

const struct REG_TAB acm_eq_tables_init_tbd[] ={
//initialization script
{ 0x00, 0x00 },
{ 0x04, 0x00 },
{ 0xfc, 0x86 },
{ 0xfd, 0x25 },
{ 0xfe, 0x15 },
{ 0x00, 0x01 },
{ 0x02, 0x20 },
{ 0x00, 0x00 },
{ 0x00, 0x00 },
{ 0x00, 0x00 },
{ 0x00, 0x00 },
{ 0x00, 0x00 },

};

#define COUNT_GAIN 0x04//step 1==0.5dB
const struct REG_TAB m_reg_tab_init_1 [] = 
{
	{ 0x00, 0x00 },
	{ 0x04, 0x00 },
	{ 0xfc, 0x86 },
	{ 0xfd, 0x25 },
	{ 0xfe, 0x15 },
	{ 0x00, 0x01 },
	{ 0x02, 0x20 },

	{ 0x0f, 0xbc },//(0xea-COUNT_GAIN) },
	{ 0x10, 0xbc },//(0xea-COUNT_GAIN) },

	{ 0x00, 0x00 },
	{ 0x00, 0x00 },
	{ 0x00, 0x00 },
	{ 0x00, 0x00 },
	{ 0x00, 0x00 },

};


//L/R
#define COUNT_GAIN 0x04//step 1==0.5dB
const U8 m_reg_tab_init [] = 
{
	 0x00, 0x00 ,
	 0x04, 0x00 ,
	 0xfc, 0x86 ,
	 0xfd, 0x25 ,
	 0xfe, 0x15 ,
	 0x00, 0x01 ,
	 0x02, 0x20 ,

	 0x0f, 0xbc ,//(0xea-COUNT_GAIN) ,
	 0x10, 0xbc ,//(0xea-COUNT_GAIN) ,

	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,

};

//SWF
const U8 mono_reg_tab_init [] = 
{
//initialization script
	 0x00, 0x00 ,
	 0x04, 0x00 ,
	 0xfc, 0x86 ,
	 0xfd, 0x25 ,
	 0xfe, 0x15 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x01, 0x81 ,

	 0x0f, (0xe4-COUNT_GAIN) ,
	 0x10, (0xe4-COUNT_GAIN) ,

	 0x00, 0x01 ,
	 0x12, 0x00 ,
	 0x16, 0x20 ,
	 0x17, 0x80 ,
	 0x00, 0x02 ,
	 0x0b, 0x08 ,
	 0x00, 0x00 ,
	 0x04, 0x02 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x00 ,
	 0x00, 0x03 ,
	 0x13, 0x1d ,
	 0x00, 0x00 ,
	// 0x04, 0x03 ,
};


U8 ACM8625_Set_Volume_LR(U8 volume);
void volumeControl(U8 vol,amp_type type);


void transmit_registers(U8 *r, int n, U8 addr)
{
	int j = 0;
	int i = 0;
	U8 ret=0;

	//U8 *flag_test ;

   	while (i < n) 
   	{
//		printf_error("transmit_registers:len=%d\n",n);
		for(j=0;j<10;j++)
		{
			//ret = i2c_write(AMP_I2C_DEV,  (unsigned char *)&r[i], 2, addr);
			
			ret = I2C0_write_sub(addr,r[i],(unsigned char *)&r[i+1],1); 
			//ret = I2C0_write_sub(ACM8625_ADDR,r[i],(unsigned char *)&r[i+1],1); 
			if (ret != 0)
			{
				//printf_error("-----transmit_registers : Fail-----:%d,[%d],addr:%x\n",ret,i,addr);
				printf_error("%x\n",j);//("-----transmit_registers retry cnt:%x\n",j);
			}
			else
			{
				//printf_info("addr:0x%x subaddr:0x%x data:0x%x\n\r",addr,r[i],r[i+1]);
				break;
			}
		}
		//flag_test = &r[i+1];
		//I2C0_read_sub(addr,r[i],*flag_test,1);
		//DBG_Printf("0x%x,0x%x\n\r",r[i],*flag_test);
		i+=2;
	}
}


void transmit_registers_1( U8 addr ,const ACM_REG_TAB *r, int len)
{
	int j = 0;
	int i = 0;
	U8 ret=0;
	u8 *flag_test ;
	// if (!AMP_I2C_DEV) {
	// 	printf_error("cannot found I2C dev\n");
	// 	return ;
	// }
   	while (i < len) 
   	{
//		printf_error("transmit_registers:len=%d\n",n);
		for(j=0;j<10;j++)
		{
			//ret = i2c_write(AMP_I2C_DEV,  (unsigned char *)&r[i], 2, addr);
			
			ret = I2C0_write_sub(addr,r[i].Address,r[i].Data,1); 
			//ret = I2C0_write_sub(ACM8625_ADDR,r[i],(unsigned char *)&r[i+1],1); 
			if (ret != 0)
			{
				//printf_error("-----transmit_registers : Fail-----:%d,[%d],addr:%x\n",ret,i,addr);
				printf_error("%x\n\r",j);//("-----transmit_registers retry cnt:%x\n",j);
			}
			else
			{
				printf_info("addr:0x%x subaddr:0x%x data:0x%x\n\r",addr,r[i].Address,r[i].Data);
				break;
			}
		}
		flag_test = &r[i].Data;
		I2C0_read_sub(addr,r[i].Address,*flag_test,1);
		DBG_Printf("0x%x,0x%x\n\r",r[i].Address,*flag_test);
		
		i++;
	}
}



int ACM8625_WriteBytes(U8 RegAddr, U8 Regdata,U8  Address)
{
	U8 j;
    int result = 0;
    U8 tempbuf[2];
    U8 ret = 0;

    //register addr
    tempbuf[0] = RegAddr;
    tempbuf[1] = Regdata;

	for(j=0;j<10;j++)
	{
		//ret = i2c_write(AMP_I2C_DEV, tempbuf,2, Address); 
		ret = I2C0_write_sub(Address,tempbuf[0],&tempbuf[1],1);
		if (ret != 0)
		{
			printf_error("-----ACM8625_WriteBytes : Fail-----:%d\n",ret);
			printf_error("-----ACM8625_WriteBytes retry cnt:%x\n",j);
			result = -1;
		}
		else
		{
			//printf_info("Address:0x%x transmit_registers success %x \n\r",Address,RegAddr);
			break;
		}
	}
	
    return result;
}



U8 ACM8625_ReadReg(U8 Amp_addr,U8 RegAddr)
{
	U8 Data = 0;
	U8 tempbuf[2];
	U8 ret = 0;

	//ret = i2c_reg_read_byte(AMP_I2C_DEV,Amp_addr,RegAddr,tempbuf);

	ret = I2C0_read_sub(Amp_addr,RegAddr,tempbuf,1);
	Data = tempbuf[0];
//	  printf_info("ACM8625_ReadReg:0x%x,0x%x\n",RegAddr,Data);
	return Data;
}


void ACM8625_Read_ErroState(void)
{
//    U8 temp = 0;
    
//    temp = AD82128_ReadReg(eErrStat);
    
}



void volumeControl(U8 vol,amp_type type)
{
#ifdef AUDIO_ACM8625_FUNCTION_ENABLE

//	U8 ret= 0;
	U8 amp_addr = ACM8625_ADDR;
	
	if(type==AMP_STEREO)
		amp_addr = ACM8625_ADDR;
	else
		amp_addr = ACM8625_ADDR_MONO;
   
	if(vol>16)
        
		vol = 16;
	
	ACM8625_WriteBytes(0x00, 0x04,amp_addr);
	ACM8625_WriteBytes(0x7c, ACM8625VolTable[4*(16-vol)+0],amp_addr);
	ACM8625_WriteBytes(0x7d, ACM8625VolTable[4*(16-vol)+1],amp_addr);
	ACM8625_WriteBytes(0x7e, ACM8625VolTable[4*(16-vol)+2],amp_addr);
	ACM8625_WriteBytes(0x7f, ACM8625VolTable[4*(16-vol)+3],amp_addr);

    ACM8625_WriteBytes(0x00, 0x04,amp_addr);
	ACM8625_WriteBytes(0x80, ACM8625VolTable[4*(16-vol)+0],amp_addr);
	ACM8625_WriteBytes(0x81, ACM8625VolTable[4*(16-vol)+1],amp_addr);
	ACM8625_WriteBytes(0x82, ACM8625VolTable[4*(16-vol)+2],amp_addr);
	ACM8625_WriteBytes(0x83, ACM8625VolTable[4*(16-vol)+3],amp_addr);
#endif

}

U8 Audio_ACM8625_gpio_init(void)
{
    printf_info("%s\n\r",__func__);
    #if 1//def AUDIO_ACM8625_FUNCTION_ENABLE

	ACM8625_STBY_PIN_INIT;
	delayus(4);
	ACM8625_STBY_PIN_HIGH;
	delayus(4);

    ACM8625_RESET_PIN_INIT;

	delayms(1);

    ACM8625_RESET_PIN_LOW;
    delayms(10);
    ACM8625_RESET_PIN_HIGH;
    delayms(10);
    ACM8625_RESET_PIN_LOW;
    delayus(10);
    ACM8625_RESET_PIN_HIGH;
    delayms(5);
    
    #endif
}




void Audio_Dev_ACM8625_Init(void)//(amp_type type)
{
    printf_info("ACM8625_Init  enter\n");

	Audio_ACM8625_gpio_init();

//	if(type==AMP_STEREO)
	{
		transmit_registers(m_reg_tab_init,sizeof(m_reg_tab_init)/sizeof(m_reg_tab_init[0]),ACM8625_ADDR);

		//transmit_registers_1(ACM8625_ADDR,&m_reg_tab_init_1[0],sizeof(m_reg_tab_init_1)/2);

		delayms(10);
		//DBG_Printf("%d,%d,%d",sizeof(mono_reg_tab_init),sizeof(mono_reg_tab_init[0]),sizeof(mono_reg_tab_init)/sizeof(mono_reg_tab_init[0]));
		transmit_registers(mono_reg_tab_init,sizeof(mono_reg_tab_init)/sizeof(mono_reg_tab_init[0]),ACM8625_ADDR_MONO);//acm_mono_registers_battery
		
		//transmit_registers_1(ACM8625_ADDR_MONO,&acm_eq_tables_init_tbd[0],sizeof(acm_eq_tables_init_tbd)/2);
		
		
	}
//	else
//	{
//		transmit_registers(acm_mono_registers,sizeof(acm_mono_registers)/sizeof(acm_mono_registers[0]),ACM8625_ADDR_MONO);
//	}

	printf_info("ACM8625_Init exit\n");
}

void Audio_Dev_ACM8625_Open(void)
{
	 printf_info("%s\n\r",__func__);
    #ifdef AUDIO_ACM8625_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_ACM8625_Set_Channel(U8 channel)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_ACM8625_FUNCTION_ENABLE

    #endif
}

void Audio_Dev_ACM8625_Set_Volume(U8 volume)
{
    printf_info("%s\n\r",__func__);
    #if 1//def AUDIO_ACM8625_FUNCTION_ENABLE

	printf_info("AMP_SET_VOLUME:%d\n",volume);
	ACM8625_WriteBytes(0x00,0x00,ACM8625_ADDR);   

	ACM8625_WriteBytes(VOLUME_CTRL_L, ACMVolumeTable[volume],ACM8625_ADDR);
	ACM8625_WriteBytes(VOLUME_CTRL_R, ACMVolumeTable[volume],ACM8625_ADDR);

	//return 0;

    #endif
}
void Audio_Dev_ACM8625_Set_Bass_Treble(U8 bass,U8 treble)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_ACM8625_FUNCTION_ENABLE

    #endif
}
void Audio_Dev_ACM8625_Set_EQ(U8 eq_mode)
{
    printf_info("ACM8625_Set_EQ  enter:%d\n",eq_mode);
	#ifdef AUDIO_ACM8625_FUNCTION_ENABLE
//	if(type == AMP_NORMAL)
//	{
//		transmit_registers(acm_registers,sizeof(acm_registers)/sizeof(acm_registers[0]),ACM8625_ADDR);
////		transmit_registers(acm_mono_registers,sizeof(acm_mono_registers)/sizeof(acm_mono_registers[0]),ACM8625_ADDR_MONO);
//	}
//	else
//	{
//		transmit_registers(acm_registers_battery,sizeof(acm_registers_battery)/sizeof(acm_registers_battery[0]),ACM8625_ADDR);
////		transmit_registers(acm_mono_registers_battery,sizeof(acm_mono_registers_battery)/sizeof(acm_mono_registers_battery[0]),ACM8625_ADDR_MONO);
//	}

//	ACM8625_Init(AMP_MONO);

	if(eq_mode == EQ_MUSIC)
	{
		transmit_registers(acm_eq_tables_music,sizeof(acm_eq_tables_music)/sizeof(acm_eq_tables_music[0]),ACM8625_ADDR);
	}
	else if(eq_mode == EQ_MOVIE)
	{
		transmit_registers(acm_eq_tables_movie,sizeof(acm_eq_tables_movie)/sizeof(acm_eq_tables_movie[0]),ACM8625_ADDR);
	}
	else if(eq_mode == EQ_NEWS)
	{
		transmit_registers(acm_eq_tables_news,sizeof(acm_eq_tables_news)/sizeof(acm_eq_tables_news[0]),ACM8625_ADDR);
	}
	else if(eq_mode == EQ_FLAT)
	{
		transmit_registers(acm_eq_tables_flat,sizeof(acm_eq_tables_flat)/sizeof(acm_eq_tables_flat[0]),ACM8625_ADDR);
	}
	else
	{
		printf_error("unsupport EQ type,force set to music!!!!\n");
		transmit_registers(acm_eq_tables_music,sizeof(acm_eq_tables_music)/sizeof(acm_eq_tables_music[0]),ACM8625_ADDR);
	}
	
	printf_info("ACM8625_Set_EQ exit\n");

//	volumeControl(16,type);//	ACM8625_WriteBytes(0x00,0x00,ACM8625_ADDR);  


   #endif
    
 	//return 0;
}



U8 ACM8625_Set_SubVolume(U8 subvolume)
{

	return 0;
}

void Audio_Dev_ACM8625_Set_Mute(U8 mute)//,amp_type type
{
	#ifdef AUDIO_ACM8625_FUNCTION_ENABLE

	U8 amp_addr= ACM8625_ADDR;
	U8 amp_mono_addr= ACM8625_ADDR_MONO;
	U8 temp=0;
	U8 temp1=0;
//	return 0;
	
 	if(mute)
	 {
//	    ACM8625_WriteBytes(0x00,0x00,amp_addr);
//    	ACM8625_WriteBytes(0x04,0x0e,amp_addr);
		temp = ACM8625_ReadReg(amp_addr,0x04);
		temp1 = ACM8625_ReadReg(amp_mono_addr,0x04);
		temp |= 0x0c;
		temp1 |= 0x0c;

		printf_info("ACM8625_Set_Mute:%x\n\r",temp);
		ACM8625_WriteBytes(0x04,temp,amp_addr);
		delayms(5);
		ACM8625_WriteBytes(0x04,temp,amp_mono_addr);
	}
	else
	{
//	    ACM8625_WriteBytes(0x00,0x00,amp_addr);
//		ACM8625_WriteBytes(0x04,0x03,amp_addr);
		temp = ACM8625_ReadReg(amp_addr,0x04);
		temp1 = ACM8625_ReadReg(amp_mono_addr,0x04);
		temp &= ~0x0c;
		temp1 &= ~0x0c;

		printf_info("ACM8625_Set_Mute:%x\n\r",temp);
    	ACM8625_WriteBytes(0x04,0x03,amp_addr);
		delayms(5);
		ACM8625_WriteBytes(0x04,0x03,amp_mono_addr);
		
	}
	#endif
}

void Audio_Dev_ACM8625_Set_AM_Mode(U8 con, U32 kFreq)
{
    printf_info("%s\n\r",__func__);
    #ifdef AUDIO_ACM8625_FUNCTION_ENABLE

    #endif
}

#endif
