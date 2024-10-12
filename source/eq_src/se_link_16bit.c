/**
 **************************************************************************************
 * @file    se_link.c
 * @brief   CCHIP EQ
 *
 * @author  CCHIP WuHan EQ
 * @version V8.3
 *
 **************************************************************************************
 */
#include "os_config.h"

#include "regmap.h"

#include "se_link_16bit.h"

extern int eq_Create_Size();
extern void leadelityInitFixed2(void *eqInst,int *dspCache, int length,int *dbuff,int chin,int chout);
extern int leadelityRunFixed(void *eqInst,short *in, short *out, int FrameSize, int sr, int ch);

#ifdef DSP_EQ_SETTING_16BIT_DECODE
#include "codec_malloc.h"
#include "printf.h"
#include "hal_gpio.h"
#include "debug.h"
#define MAX_EQ_CACHE_SIZE 30000
#define MAX_EQ_PARAM_LEN  656  // 552   // 544
#define NN 128   //   30ms
#define CH 2

int  *dspCache;
void *eqFixInst;
int eqCoefUpdateFlag = 0;
int eqLen = 0;

union converter
{
	uint32_t u32;
	uint8_t u8[4];
} conv;

static unsigned int eqCoefUpdateBuffer[MAX_CCHIP_EQ_PARAM_LEN];
static int eqCoefDefault[MAX_CCHIP_EQ_PARAM_LEN] =
{
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x0000000e, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x0000000e, 0xffffffff, 0x0808a5fb, 0xf0942cbf, 0x076998bb, 0x0f6bd341, 0xf88dc149, 0x08000000,
	0xf01376cb, 0x07eca3b0, 0x0fec8935, 0xf8135c50, 0x08000000, 0xf01d344d, 0x07e30724, 0x0fe2cbb3,
	0xf81cf8dc, 0x08000000, 0xf03a74cc, 0x07c6774b, 0x0fc58b34, 0xf83988b5, 0x08000000, 0xf0751619,
	0x078e8d13, 0x0f8ae9e7, 0xf87172ed, 0x08000000, 0xf0eabe5d, 0x07236877, 0x0f1541a3, 0xf8dc9789,
	0x08000000, 0xf1881f5e, 0x069dca90, 0x0e77e0a2, 0xf9623570, 0x08000000, 0xf31191c9, 0x05798839,
	0x0cee6e37, 0xfa8677c7, 0x08000000, 0xf49791ba, 0x04892408, 0x0b686e46, 0xfb76dbf8, 0x08000000,
	0xf903587c, 0x02a53ed4, 0x06fca784, 0xfd5ac12c, 0x08000000, 0xfa6f7beb, 0x02419ca8, 0x05908415,
	0xfdbe6358, 0x08000000, 0xfbd64e7e, 0x01f6e8cd, 0x0429b182, 0xfe091733, 0x08000000, 0xfd38fd6f,
	0x01c2ea09, 0x02c70291, 0xfe3d15f7, 0x08000000, 0xfe98d7be, 0x01a41bd9, 0x01672842, 0xfe5be427,
	0x08000000, 0xfff73f0c, 0x01999b32, 0x0008c0f4, 0xfe6664ce,
};

void eq_init(int sample_rate, int in_channel, int out_channel)
{
	eqFixInst = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, eq_Create_Size(), CODEC_MEM_ALIGN_32); /// calloc(eq_Create_Size(),0);

	if (NULL == eqFixInst)
	{
		printf_info(" eqFixInst malloc fail \r\n");
	}
	memset(eqFixInst, 0, eq_Create_Size());
	dspCache = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, MAX_EQ_CACHE_SIZE * 4, CODEC_MEM_ALIGN_32);
	if (NULL == dspCache)
	{
		printf_info("eq malloc fail \r\n");
	}

	leadelityInitFixed2(eqFixInst, dspCache, MAX_EQ_CACHE_SIZE, eqCoefDefault, in_channel, out_channel);
}

void eq_uninit(void)
{
	codec_free(eqFixInst);
	codec_free(dspCache);
}

void convert_big_endian_to_little_endian(uint32_t *val)
{
	conv.u32 = *val;
	*val = (conv.u8[0] << 24) | (conv.u8[1] << 16) | (conv.u8[2] << 8) | conv.u8[3];
}

void eq_process(short *src, short *dst, int sample_rate, int in_channel, int byte_len) __attribute__ ((section (".interal_ram_1_text")));
void eq_process(short *src, short *dst, int sample_rate, int in_channel, int byte_len)
{
	
	if (eqCoefUpdateFlag)
	{
		int i = 0;
		extern U8 extCMD_Uart_eq_rx_buff[MAX_EQ_PARAM_LEN * 4];
		memset(eqCoefUpdateBuffer, 0, sizeof(eqCoefUpdateBuffer));
		memcpy(eqCoefUpdateBuffer, extCMD_Uart_eq_rx_buff, eqLen * 4);
		for (i = 0; i < eqLen; i++)
		{
			convert_big_endian_to_little_endian(&eqCoefUpdateBuffer[i]);
		}

		leadelityInitFixed2(eqFixInst, dspCache, MAX_EQ_CACHE_SIZE, eqCoefUpdateBuffer, in_channel, in_channel);
		eqCoefUpdateFlag = 0;
		printf_info("EQ update success!\r\n");
	}
	else
	{
		int sample_len;
		
		sample_len = byte_len/2/in_channel;

		leadelityRunFixed(eqFixInst, src, dst, sample_len, sample_rate, in_channel);
	}

}

void selink_set(int *coef, unsigned int len)
{
	memset(eqCoefUpdateBuffer, 0, sizeof(eqCoefUpdateBuffer));
	memcpy(eqCoefUpdateBuffer, coef, len*4);

	eqLen = len;
	eqCoefUpdateFlag = 1;
}

#endif
