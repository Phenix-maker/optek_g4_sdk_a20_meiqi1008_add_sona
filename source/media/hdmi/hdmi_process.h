#ifndef __HDMI_PROCESS_H__
#define __HDMI_PROCESS_H__


//#define HDMI_INT_CHECKTOUT_USED_TIMER


#define HDMI_INIT_PIN                   GPIO0_13_INDEX

#define HDMI_INT_PIN_MASK               (1<<(HDMI_INIT_PIN-GPIO0_00_INDEX))

#define	HDMI_INT_CLEAR                  (REG_GPIO0_INTST |= HDMI_INT_PIN_MASK)

#define	HDMI_INT_ENABLE                 REG_GPIO0_INTEN |= HDMI_INT_PIN_MASK
#define	HDMI_INT_DISABLE                REG_GPIO0_INTEN &= ~HDMI_INT_PIN_MASK


typedef enum {
	HDMI_INIT_PROCESS_UNKNOWN,
	HDMI_INIT_PROCESS_START,
	HDMI_INIT_PROCESS_FAIL,
	HDMI_INIT_PROCESS_END, 
} enHDMI_INIT_PROCESS_STATUS;

typedef enum {
	HDMI_INT_PROCESS_UNKNOWN,
	HDMI_INT_PROCESS_WAIT_START,
	HDMI_INT_PROCESS_START,
	HDMI_INT_PROCESS_WAIT_FINISH,	
	HDMI_INT_PROCESS_FINISH,
} enHDMI_INT_PROCESS_STATUS;


enum {
	AudType_Unkown,
	AudType_LPCM,
	AudType_NLPCM,
	AudType_HBR,
	AudType_DSD,
};

enum{
	AudSel_Unkown,
	AudSel_I2S,
	AudSel_SPDIF,
};

enum{
	AudCH_Unkown,
	AudCH_0,
	AudCH_2 = 2,
	AudCH_3,
	AudCH_4,
	AudCH_5,
	AudCH_6,
	AudCH_7,
	AudCH_8,
	AudCH_10 = 10,
	AudCH_12 = 12,
	AudCH_16 = 16,
	AudCH_32 = 32,
};

enum{
	AudSample_Freq_Unkown,
	AudSample_Freq_err,
	AudSample_Freq_32K   = 32000,
	AudSample_Freq_44K   = 44100,
	AudSample_Freq_48K   = 48000,
	AudSample_Freq_64K   = 64000,
	AudSample_Freq_88K   = 88200,
	AudSample_Freq_96K   = 96000,
	AudSample_Freq_128K  = 128000,
	AudSample_Freq_176K  = 176000,
	AudSample_Freq_192K  = 192000,
	AudSample_Freq_256K  = 256000,
	AudSample_Freq_352K  = 352000,
	AudSample_Freq_324K  = 384000,
	AudSample_Freq_512K  = 512000,
	AudSample_Freq_705K  = 705000,
	AudSample_Freq_768K  = 768000,
	AudSample_Freq_1024K = 1024000, 
	AudSample_Freq_1411K = 1411000,
	AudSample_Freq_1536K = 1536000,
};


typedef struct 
{
	U8  init_enable;
	U8  init_status;
	U8  int_count;          //use for the number of interrupts
	U8  int_process_status; //use for interrupt pocessing status

	U8  tx_mode;
	U8  vol;
	U8  stream_type;
	U8  channel;
	U32 sample_rate;

	U8  pre_stream_type;
	U8  pre_channel;
	U32 pre_sample_rate;

	U8 eArcLatency;
} HDMI_INFOR_STRU;

extern HDMI_INFOR_STRU hdmi_infor;



void hdmi_hw_isr_init(void);
void hdmi_hw_isr(void);

void hdmi_process_init(void);
void hdmi_start_set(int con_flag);
void hdmi_channel_set(int channel_index);

void hdmi_int_process_finish_set(HDMI_INFOR_STRU *parm);

#endif //__HDMI_PROCESS_H__
