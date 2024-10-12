#include "os_config.h"

#include "mem_reloc.h"

#include "hal_gpio.h"

#include "debug.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "codec_malloc.h"

#include "printf.h"

#ifdef DSP_EQ_SETTING_24BIT_DECODE
#include "se_link_24bit.h"

#include "console_task.h"
// #include "optek_hifi2_bqriir.h"
#define MAX_EQ_CACHE_SIZE 7000
#define MAX_EQ_CACHE_SIZE_SUB 10000
#define MAX_EQ_PARAM_LEN  656  // 552   // 544
#define NN 128   //   30ms
#define CH 1
//static int *up;
//static int *down;
enum {
	EQ_DATA_IN_TOOL = 1,
	EQ_DATA_IN_PROCESS,
};

int *DSPcache;
// int *DSPcache_sub;
void *eqfixInst;
int se_coef_update = 0;
int se_coef_update_sub = 0;
int eq_init_flag = 0;
int eq_len = 0;


unsigned int se_coef_update_buf[MAX_EQ_PARAM_LEN];

// static int se_coef[MAX_CCHIP_EQ_PARAM_LEN] = {
// 		0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
// 	    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000006, 0x00000000, 0x00000001, 0x00000000,
// 	    0x00000002, 0x00000000,

// 	    0x00000001, 0x00000000, 0x00000001, 0x000000fa, 0x00001f40, 0x000002c3, 0x00000000, 0x00000001, 0x0000012c, 0x00000bb8,
// 	    0x000002c3, 0x00000000, 0x00000001, 0x00000032, 0x00000000, 0x000002c3, 0x00000000, 0x00000001, 0x000003e8, 0x00000000,
// 	    0x000002c3, 0x00000000, 0xffffffd3, 0xfffffff1, 0x0000ac44,

// 	    0x00000006, 0xffffffff, 0x08000000, 0xf009baae, 0x07f64bf5, 0x0ff64552, 0xf809b40b, 0x08000000, 0xf01376cb, 0x07eca3b0,
// 	    0x0fec8935, 0xf8135c50, 0x08000000, 0xf01d344d, 0x07e30724, 0x0fe2cbb3, 0xf81cf8dc, 0x08000000, 0xf03a74cc, 0x07c6774b,
// 	    0x0fc58b34, 0xf83988b5, 0x08000000, 0xf0751619, 0x078e8d13, 0x0f8ae9e7, 0xf87172ed, 0x08000000, 0xf0eabe5d, 0x07236877,
// 	    0x0f1541a3, 0xf8dc9789, 0x08000000, 0xf1881f5e, 0x069dca90, 0x0e77e0a2, 0xf9623570, 0x08000000, 0xf31191c9, 0x05798839,
// 	    0x0cee6e37, 0xfa8677c7, 0x08000000, 0xf49791ba, 0x04892408, 0x0b686e46, 0xfb76dbf8, 0x08000000, 0xf903587c, 0x02a53ed4,
// 	    0x06fca784, 0xfd5ac12c, 0x08000000, 0xfa6f7beb, 0x02419ca8, 0x05908415, 0xfdbe6358, 0x08000000, 0xfbd64e7e, 0x01f6e8cd,
// 	    0x0429b182, 0xfe091733, 0x08000000, 0xfd38fd6f, 0x01c2ea09, 0x02c70291, 0xfe3d15f7, 0x08000000, 0xfe98d7be, 0x01a41bd9,
// 	    0x01672842, 0xfe5be427, 0x08000000, 0xfff73f0c, 0x01999b32, 0x0008c0f4, 0xfe6664ce,

// 	    0x00000001, 0x065ac8c3, 0x065ac8c3,

// 	    0x00000002, 0xffffffff, 0x0066a4a5, 0x07faa90f, 0x07fff41d, 0x08000000, 0x00000372, 0x00000084, 0x0066a4a5, 0x013f3df2,
// 	    0x00006851, 0x0000d0a3, 0x00006851, 0x0fad77d1, 0xf850e6e9, 0x07d7243a, 0xf051b78c, 0x07d7243a, 0x0fad77d1, 0xf850e6e9,
// 	    0x0066a4a5, 0x07eab994, 0x07ffe839, 0x08000000, 0x00000372, 0x00000084, 0x0066a4a5, 0x013f3df2, 0x0125dbab, 0x024bb756,
// 	    0x0125dbab, 0x05721418, 0xfdf67d3b, 0x03dee5b7, 0xf8423491, 0x03dee5b7, 0x05721418, 0xfdf67d3b, 0x00813856, 0x0797cfd2,
// 	    0x07ff8921, 0x08000000, 0x00000372, 0x00000084, 0x00813856, 0x00fd953a,
// };
static int se_coef[MAX_EQ_PARAM_LEN] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 
    
};


#if 0
static int se_coef_48K[MAX_CCHIP_EQ_PARAM_LEN] = {

#else
static int se_coef_48K_BT[MAX_EQ_PARAM_LEN] = {



};
static int se_coef_44_1K_BT[MAX_EQ_PARAM_LEN] = {
  

};

static const int se_coef_48K_AUX[MAX_EQ_PARAM_LEN] = {




};
static const int se_coef_44_1K_AUX[MAX_EQ_PARAM_LEN] = {
    

	
};
static const int se_coef_44_1k_test[MAX_EQ_PARAM_LEN] = {

	0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000009, 0x00000000, 0x00000001, 0x00000005, 
    0x00000000, 0x00000000, 
    
    0x00000001, 0x37fcc39b, 0x37fcc39b, 
    
    0x00000009, 0xffffffff, 0x07fca11f, 0xf006bdc1, 0x07fca11f, 0x0ff93d77, 0xf806b8f9, 0x07f7ead8, 0xf0102a4f, 0x07f7ead8, 
    0x0fefd0ec, 0xf810258a, 0x00006944, 0x0000d288, 0x00006944, 0x0fd1f4fb, 0xf82c65f4, 0x000067af, 0x0000cf5f, 0x000067af, 
    0x0f9519f0, 0xf8694753, 0x0803fc09, 0xf0055902, 0x07f6be9c, 0x0ffaa6fe, 0xf805455b, 0x07fde800, 0xf01497bd, 0x07edbbd3, 
    0x0feb6843, 0xf8145c2c, 0x07ff708b, 0xf0058d95, 0x07fb1c72, 0x0ffa726b, 0xf8057303, 0x0803586e, 0xf01a4108, 0x07e2d04b, 
    0x0fe5bef8, 0xf819d747, 0x0802ba52, 0xf0110eb5, 0x07ecea1c, 0x0feef14b, 0xf8105b91, 0x08000000, 0xf001ed3f, 0x07fe2d59, 
    0x0ffe12c1, 0xf801d2a7, 0x08000000, 0xfa6f7beb, 0x02419ca8, 0x05908415, 0xfdbe6358, 0x08000000, 0xfbd64e7e, 0x01f6e8cd, 
    0x0429b182, 0xfe091733, 0x08000000, 0xfd38fd6f, 0x01c2ea09, 0x02c70291, 0xfe3d15f7, 0x08000000, 0xfe98d7be, 0x01a41bd9, 
    0x01672842, 0xfe5be427, 0x08000000, 0xfff73f0c, 0x01999b32, 0x0008c0f4, 0xfe6664ce, 
    
    0x00000001, 0xffffffff, 0x0005e810, 0x07eab994, 0x07fffa0e, 0x08000000, 0x00000372, 0x00000161, 0x0005e810, 0x15abca24, 
    0x00006851, 0x0000d0a3, 0x00006851, 0x0fad77d1, 0xf850e6e9, 0x07d7243a, 0xf051b78c, 0x07d7243a, 0x0fad77d1, 0xf850e6e9, 
    0x0066a4a5, 0x07eab994, 0x07ffe839, 0x08000000, 0x00000372, 0x00000084, 0x0066a4a5, 0x013f3df2, 0x0125dbab, 0x024bb756, 
    0x0125dbab, 0x05721418, 0xfdf67d3b, 0x03dee5b7, 0xf8423491, 0x03dee5b7, 0x05721418, 0xfdf67d3b, 0x00813856, 0x0797cfd2, 
    0x07ff8921, 0x08000000, 0x00000372, 0x00000084, 0x00813856, 0x00fd953a, 
    
    0x00000005, 0xffffffff, 0x07f641ef, 0xf01f5fa2, 0x07efb8ab, 0x0fe0a05e, 0xf81a0566, 0x07fca11f, 0xf006bdc1, 0x07fca11f, 
    0x0ff93d77, 0xf806b8f9, 0x07f7ead8, 0xf0102a4f, 0x07f7ead8, 0x0fefd0ec, 0xf810258a, 0x00006944, 0x0000d288, 0x00006944, 
    0x0fd1f4fb, 0xf82c65f4, 0x000067af, 0x0000cf5f, 0x000067af, 0x0f9519f0, 0xf8694753, 0x08000000, 0xf0eabe5d, 0x07236877, 
    0x0f1541a3, 0xf8dc9789, 0x08000000, 0xf1881f5e, 0x069dca90, 0x0e77e0a2, 0xf9623570, 0x08000000, 0xf31191c9, 0x05798839, 
    0x0cee6e37, 0xfa8677c7, 0x08000000, 0xf49791ba, 0x04892408, 0x0b686e46, 0xfb76dbf8, 0x08000000, 0xf903587c, 0x02a53ed4, 
    0x06fca784, 0xfd5ac12c, 0x08000000, 0xfa6f7beb, 0x02419ca8, 0x05908415, 0xfdbe6358, 0x08000000, 0xfbd64e7e, 0x01f6e8cd, 
    0x0429b182, 0xfe091733, 0x08000000, 0xfd38fd6f, 0x01c2ea09, 0x02c70291, 0xfe3d15f7, 0x08000000, 0xfe98d7be, 0x01a41bd9, 
    0x01672842, 0xfe5be427, 0x08000000, 0xfff73f0c, 0x01999b32, 0x0008c0f4, 0xfe6664ce, 
    




    
	//200W_DRC
    // 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    // 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 
    // 0x00000000, 0x00000000, 
    
    // 0x00000001, 0x6fb59fc1, 0x6fb59fc1, 
    
    // 0x00000001, 0xffffffff, 0x000857d9, 0x07fcf274, 0x07ffe839, 0x08000000, 0x00000372, 0x00000161, 0x000857d9, 0x0f5788c6, 
    // 0x00006851, 0x0000d0a3, 0x00006851, 0x0fad77d1, 0xf850e6e9, 0x07d7243a, 0xf051b78c, 0x07d7243a, 0x0fad77d1, 0xf850e6e9, 
    // 0x0066a4a5, 0x07eab994, 0x07ffe839, 0x08000000, 0x00000372, 0x00000084, 0x0066a4a5, 0x013f3df2, 0x0125dbab, 0x024bb756, 
    // 0x0125dbab, 0x05721418, 0xfdf67d3b, 0x03dee5b7, 0xf8423491, 0x03dee5b7, 0x05721418, 0xfdf67d3b, 0x00813856, 0x0797cfd2, 
    // 0x07ff8921, 0x08000000, 0x00000372, 0x00000084, 0x00813856, 0x00fd953a, 

};
static const int se_coef_48k_test[MAX_EQ_PARAM_LEN] = {

    
 	0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000009, 0x00000000, 0x00000001, 0x00000005, 
    0x00000000, 0x00000000, 
    
    0x00000001, 0x37fcc39b, 0x37fcc39b, 
    
    0x00000009, 0xffffffff, 0x07fce74e, 0xf0063165, 0x07fce74e, 0x0ff9ca92, 0xf8062d5b, 0x07f89288, 0xf00edaef, 0x07f89288, 
    0x0ff1210a, 0xf80ed6e8, 0x000058ef, 0x0000b1df, 0x000058ef, 0x0fd5c85e, 0xf828d3e5, 0x000057b4, 0x0000af69, 0x000057b4, 
    0x0f9db353, 0xf860eddc, 0x0803a942, 0xf004e871, 0x07f77ee4, 0x0ffb178f, 0xf804d7da, 0x07fe135a, 0xf012e8ef, 0x07ef3604, 
    0x0fed1711, 0xf812b6a2, 0x07ff7c2f, 0xf005183d, 0x07fb8202, 0x0ffae7c3, 0xf80501cf, 0x08031341, 0xf0181a3f, 0x07e52bd0, 
    0x0fe7e5c1, 0xf817c0ef, 0x080281cc, 0xf00f9fdf, 0x07ee7598, 0x0ff06021, 0xf80f089c, 0x08000000, 0xf001c334, 0x07fe533f, 
    0x0ffe3ccc, 0xf801acc1, 0x08000000, 0xf9a11992, 0x02770be1, 0x065ee66e, 0xfd88f41f, 0x08000000, 0xfaed9eba, 0x0224c28c, 
    0x05126146, 0xfddb3d74, 0x08000000, 0xfc35fb5e, 0x01e6c789, 0x03ca04a2, 0xfe193877, 0x08000000, 0xfd7b2315, 0x01bb8f23, 
    0x0284dceb, 0xfe4470dd, 0x08000000, 0xfebe1e09, 0x01a209f7, 0x0141e1f7, 0xfe5df609, 
    
    0x00000001, 0xffffffff, 0x0005e810, 0x07ec71fb, 0x07fffa8a, 0x08000000, 0x000003c0, 0x00000180, 0x0005e810, 0x15abca24, 
    0x00005833, 0x0000b065, 0x00005833, 0x0fb42c15, 0xf84a7321, 0x07da6e3d, 0xf04b2386, 0x07da6e3d, 0x0fb42c15, 0xf84a7321, 
    0x0066a4a5, 0x07ec71fb, 0x07ffea28, 0x08000000, 0x000003c0, 0x00000090, 0x0066a4a5, 0x013f3df2, 0x0100a30f, 0x0201461d, 
    0x0100a30f, 0x063d4b0b, 0xfdc028ba, 0x041f4894, 0xf7c16ed7, 0x041f4894, 0x063d4b0b, 0xfdc028ba, 0x00813856, 0x07a01356, 
    0x07ff92c9, 0x08000000, 0x000003c0, 0x00000090, 0x00813856, 0x00fd953a, 
    
    0x00000005, 0xffffffff, 0x07f70b3d, 0xf01c70fd, 0x07f10906, 0x0fe38f03, 0xf817ebbd, 0x07fce74e, 0xf0063165, 0x07fce74e, 
    0x0ff9ca92, 0xf8062d5b, 0x07f89288, 0xf00edaef, 0x07f89288, 0x0ff1210a, 0xf80ed6e8, 0x000058ef, 0x0000b1df, 0x000058ef, 
    0x0fd5c85e, 0xf828d3e5, 0x000057b4, 0x0000af69, 0x000057b4, 0x0f9db353, 0xf860eddc, 0x08000000, 0xf0d79865, 0x07346758, 
    0x0f28679b, 0xf8cb98a8, 0x08000000, 0xf16820c1, 0x06b81bd7, 0x0e97df3f, 0xf947e429, 0x08000000, 0xf2d1c6d3, 0x05a540c8, 
    0x0d2e392d, 0xfa5abf38, 0x08000000, 0xf438fc8f, 0x04bf6d52, 0x0bc70371, 0xfb4092ae, 0x08000000, 0xf84f9a2c, 0x02dfbc1d, 
    0x07b065d4, 0xfd2043e3, 0x08000000, 0xf9a11992, 0x02770be1, 0x065ee66e, 0xfd88f41f, 0x08000000, 0xfaed9eba, 0x0224c28c, 
    0x05126146, 0xfddb3d74, 0x08000000, 0xfc35fb5e, 0x01e6c789, 0x03ca04a2, 0xfe193877, 0x08000000, 0xfd7b2315, 0x01bb8f23, 
    0x0284dceb, 0xfe4470dd, 0x08000000, 0xfebe1e09, 0x01a209f7, 0x0141e1f7, 0xfe5df609, 
    
	//200W_DRC
    // 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
    // 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 
    // 0x00000000, 0x00000000, 
    
    // 0x00000001, 0x6fb59fc1, 0x6fb59fc1, 
    
    // 0x00000001, 0xffffffff, 0x000857d9, 0x07fd31e9, 0x07ffea28, 0x08000000, 0x000003c0, 0x00000180, 0x000857d9, 0x0f5788c6, 
    // 0x00005833, 0x0000b065, 0x00005833, 0x0fb42c15, 0xf84a7321, 0x07da6e3d, 0xf04b2386, 0x07da6e3d, 0x0fb42c15, 0xf84a7321, 
    // 0x0066a4a5, 0x07ec71fb, 0x07ffea28, 0x08000000, 0x000003c0, 0x00000090, 0x0066a4a5, 0x013f3df2, 0x0100a30f, 0x0201461d, 
    // 0x0100a30f, 0x063d4b0b, 0xfdc028ba, 0x041f4894, 0xf7c16ed7, 0x041f4894, 0x063d4b0b, 0xfdc028ba, 0x00813856, 0x07a01356, 
    // 0x07ff92c9, 0x08000000, 0x000003c0, 0x00000090, 0x00813856, 0x00fd953a, 

};
#endif


int *se_link_buf_in;
int *se_link_buf_out;


int *sub_DSPcache;
void *sub_eqfixInst;
int *se_link_sub_buf_in;
int *se_link_sub_buf_out;


static const int vol_tab[31] = {
		0x00CCCCCD,//-20db//0
		0x00D8EF67,//-19.5db//1
		0x00E5CA15,//-19db//2
		0x00F367BF,//-18.5db//3
		0x0101D3F3, //-18db//4
		0x01111AEE,//-17.5db//5
		0x012149A6,//-17db//6
		0x01326DD7,//-16.5db//7
		0x0144960C,//-16db//8
		0x0157D1AE,//-15.5db//9
		0x016C310E,//-15db//10
		0x0181C576,//-14.5db//11
		0x0198A135,//-14db//12
		0x02026F31,//-12db//13
		0x0241346F, //-11db //14

		0x0287A26C, //-10db //15
		0x02D6A867, //-9db  //16
		0x0301B70B, //-8.5db  //17
		0x032F52D0, //-8db //18
		0x035FA26B, //-7.5db//19
		0x0392CED9, //-7db//20
		0x03C90387, //-6.5db//21
		0x04026E74, //-6db//22
		0x043F4058, //-5.5db//23
		0x050C335D, //-4db//24
		0x0558C4B2, //-3.5db//25
		0x05A9DF7B, //-3db//26
		0x05FFC889, //-2.5db//27
		0x065AC8C3, //-2db//28
		0x0721482C, //-1db//29
		0x08000000, //0db //30
};

int se_coef_vol_buf[3] = {
	0x00000001, 0x08000000, 0x08000000,
};
void wheq_vol_set(int vol)
{
	if(vol >= 0 && vol < sizeof(vol_tab)/sizeof(int))
	{
		se_coef_vol_buf[1] = vol_tab[vol];
		se_coef_vol_buf[2] = vol_tab[vol];
		if(eq_init_flag == 1)
		{
			extern void *sub_eqfixInst;
			leadelityInitVolume(sub_eqfixInst,se_coef_vol_buf);
		}
	}
}


void eq_init(int sample_rate, int in_channel, int out_channel)
{
	printf_info("%s\r\n", __func__);

	eqfixInst = NULL;
	DSPcache = NULL;
	se_link_buf_in = NULL;
	se_link_buf_out = NULL;

	eq_init_flag = 1;

	eqfixInst = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, eq_Create_Size(), CODEC_MEM_ALIGN_32); /// calloc(eq_Create_Size(),0);

	if (NULL == eqfixInst)
	{
		printf_info(" eqFixInst malloc fail \r\n");
	}
	memset(eqfixInst, 0, eq_Create_Size());

	DSPcache = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, MAX_EQ_CACHE_SIZE * 4, CODEC_MEM_ALIGN_32);
	if (NULL == DSPcache)
	{
		printf_info("eq malloc fail \r\n");
	}

	se_link_buf_in = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, NN*in_channel*4, CODEC_MEM_ALIGN_32);
	if (NULL == se_link_buf_in)
	{
		printf_info("se_link_buf_in malloc fail \r\n");
	}
	se_link_buf_out = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, NN*out_channel*4, CODEC_MEM_ALIGN_32);
	if (NULL == se_link_buf_out)
	{
		printf_info("eq_out malloc fail \r\n");
	}	
	#if 0 //not process
	if(audio_in_stream.sample_rate == AUDIO_IN_STREAM_SAMPLE_RATE)
	{
		if(SOURCE_AUX == global_vari.audio_source)
		{
			leadelityInitFixed2(eqfixInst, DSPcache, MAX_EQ_CACHE_SIZE, se_coef_48K_AUX, in_channel, out_channel);
		}
		else
		{
			leadelityInitFixed2(eqfixInst, DSPcache, MAX_EQ_CACHE_SIZE, se_coef, in_channel, out_channel);
		}
	}
	else
	{
		if(SOURCE_AUX == global_vari.audio_source)
		{
			leadelityInitFixed2(eqfixInst, DSPcache, MAX_EQ_CACHE_SIZE, se_coef_44_1K_AUX, in_channel, out_channel);
		}
		else
		{
			leadelityInitFixed2(eqfixInst, DSPcache, MAX_EQ_CACHE_SIZE, se_coef_44_1K_BT, in_channel, out_channel);
		}

	}
	#endif
		leadelityInitFixed2(eqfixInst, DSPcache, MAX_EQ_CACHE_SIZE, se_coef, in_channel, out_channel);


	//sub
	sub_eqfixInst = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, eq_Create_Size(), CODEC_MEM_ALIGN_32); // calloc(eq_Create_Size(),0);

	if (NULL == sub_eqfixInst)
	{
		printf_info(" sub_eqfixInst malloc fail \r\n");
	}
	memset(sub_eqfixInst, 0, eq_Create_Size());

	sub_DSPcache = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, MAX_EQ_CACHE_SIZE_SUB * 4, CODEC_MEM_ALIGN_32);
	if (NULL == sub_DSPcache)
	{
		printf_info("sub_DSPcache malloc fail \r\n");
	}


	se_link_sub_buf_in = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, NN*in_channel*4, CODEC_MEM_ALIGN_32);
	if (NULL == se_link_sub_buf_in)
	{
		printf_info("se_link_buf_in malloc fail \r\n");
	}
	se_link_sub_buf_out = codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, NN*out_channel*4, CODEC_MEM_ALIGN_32);
	if (NULL == se_link_sub_buf_out)
	{
		printf_info("eq_out malloc fail \r\n");
	}	


#if 0
	if(audio_in_stream.sample_rate == AUDIO_IN_STREAM_SAMPLE_RATE)
	{
		if(SOURCE_AUX == global_vari.audio_source)
		{
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_48K_AUX, in_channel, out_channel);		
		}
		else
		{
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_48K_BT, in_channel, out_channel);
		}
	}
	else
	{
		if(SOURCE_AUX == global_vari.audio_source)
		{
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_44_1K_AUX, in_channel, out_channel);
		}
		else
		{
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_44_1K_BT, in_channel, out_channel);
		}

	}
#else
	if(audio_in_stream.sample_rate == AUDIO_IN_STREAM_SAMPLE_RATE)
	{
		#if 0
		if(SOURCE_AUX == global_vari.audio_source)
		{
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_48k_test, in_channel, out_channel);		
		}
		else
		{
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_48k_test, in_channel, out_channel);
		}
		#else
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_48k_test, in_channel, out_channel);

		#endif
	}
	else
	{
		#if 0
		if(SOURCE_AUX == global_vari.audio_source)
		{
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_44_1k_test, in_channel, out_channel);
		}
		else
		{
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_44_1k_test, in_channel, out_channel);
		}
		#else
			leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_44_1k_test, in_channel, out_channel);

		#endif

	}

#endif
}

void eq_uninit(void)
{
	printf_info("%s\r\n", __func__);

	eq_init_flag= 0;
	
	codec_free(eqfixInst);
	codec_free(DSPcache);
	codec_free(sub_eqfixInst);
	codec_free(sub_DSPcache);
	codec_free(se_link_buf_in);
	codec_free(se_link_buf_out);
}

union converter {
    uint32_t u32;
    uint8_t u8[4];
} conv;
void convert_big_endian_to_little_endian(uint32_t *val) {
    conv.u32 = *val;
    *val = (conv.u8[0] << 24) | (conv.u8[1] << 16) | (conv.u8[2] << 8) | conv.u8[3];
}

void eq_process_16bit(int *src, int *dst, int sample_rate, int in_channel, int byte_len) __attribute__ ((section (".interal_ram_1_text")));
void eq_process_16bit(int *src, int *dst, int sample_rate, int in_channel, int byte_len)
{

	int i = 0;
	int sample_len = byte_len/2/in_channel;

	short *pSrc = (short *)src;
	short *pDest = (short *)dst;

	if(se_coef_update)
	{
		memset(se_coef_update_buf, 0, sizeof(se_coef_update_buf));
		memcpy(se_coef_update_buf, extCMD_Uart_eq_rx_buff, eq_len*4);
		for(i = 0; i < eq_len; i++)
		{
			convert_big_endian_to_little_endian(&se_coef_update_buf[i]);
		}
		leadelityInitFixed2(eqfixInst,DSPcache, 10000,se_coef_update_buf,2,2);
		se_coef_update = 0;
	}
	else
	{

		optek_hifi2_16b_to_24b((int *)se_link_buf_in, (short *)pSrc, byte_len/2);

		for(i = 0; i < sample_len*2; i++)
		{
			se_link_buf_out[i] = se_link_buf_in[i] >> 8;
		}

		leadelityRunFixed(eqfixInst, se_link_buf_out, se_link_buf_in, sample_len, sample_rate, in_channel);

		for(i = 0; i < sample_len*2; i++)
		{
			se_link_buf_out[i] = se_link_buf_in[i] << 8;
		}		

		optek_hifi2_24b_to_16b((short *)pDest, (int *)se_link_buf_out, byte_len/2);

	}



}

void eq_process(int *src, int *dst, int sample_rate, int in_channel, int byte_len) __attribute__ ((section (".interal_ram_1_text")));
void eq_process(int *src, int *dst, int sample_rate, int in_channel, int byte_len)
{
	int i = 0;
	int sample_len = byte_len/4/in_channel;

	if(se_coef_update == EQ_DATA_IN_TOOL)
	{
		memset(se_coef_update_buf, 0, sizeof(se_coef_update_buf));
		memcpy(se_coef_update_buf, extCMD_Uart_eq_rx_buff, eq_len*4);
		for(i = 0; i < eq_len; i++)
		{
			convert_big_endian_to_little_endian(&se_coef_update_buf[i]);
		}
		leadelityInitFixed2(eqfixInst, DSPcache, MAX_EQ_CACHE_SIZE, se_coef_update_buf, 2, 2);
		se_coef_update = 0;
	}
	else if(se_coef_update == EQ_DATA_IN_PROCESS)
	{
		memset(DSPcache, 0, sizeof(int) *MAX_EQ_CACHE_SIZE);
		leadelityInitFixed2(eqfixInst, DSPcache, MAX_EQ_CACHE_SIZE, se_coef_update_buf, 2, 2);
		se_coef_update = 0;
	}

		leadelityRunFixed(eqfixInst, src, dst, sample_len, sample_rate, in_channel);
}

void eq_process_sub(int *src, int *dst, int sample_rate, int in_channel, int byte_len) __attribute__ ((section (".interal_ram_1_text")));
void eq_process_sub(int *src, int *dst, int sample_rate, int in_channel, int byte_len)
{
	int i = 0;
	int sample_len = byte_len/4/in_channel;

	if(se_coef_update_sub == EQ_DATA_IN_TOOL)
	{
		memset(se_coef_update_buf, 0, sizeof(se_coef_update_buf));
		memcpy(se_coef_update_buf, extCMD_Uart_eq_rx_buff, eq_len*4);
		for(i = 0; i < eq_len; i++)
		{
			convert_big_endian_to_little_endian(&se_coef_update_buf[i]);
		}
		leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_update_buf, 2, 2);
		se_coef_update_sub = 0;
	}
	else if(se_coef_update_sub == EQ_DATA_IN_PROCESS)
	{
		memset(DSPcache, 0, sizeof(int) *MAX_EQ_CACHE_SIZE_SUB);
		leadelityInitFixed2(sub_eqfixInst, sub_DSPcache, MAX_EQ_CACHE_SIZE_SUB, se_coef_update_buf, 2, 2);
		se_coef_update = 0;
	}

		leadelityRunFixed(sub_eqfixInst, src, dst, sample_len, sample_rate, in_channel);


}
void selink_set_lr(int *coef, unsigned int len)
{
	printf_info("%s\r\n", __func__);

	eq_len = len;
	se_coef_update = 2;
}

void selink_set(int *coef, unsigned int len)
{
	printf_info("%s\r\n", __func__);
	
	eq_len = len;
	// se_coef_update = 1;
	se_coef_update_sub = 1;
}


#endif
