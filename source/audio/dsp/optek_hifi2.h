#ifndef __OPTEK_HIFI2_H__
#define __OPTEK_HIFI2_H__

#include "NatureDSP_Signal.h"


#define restrict


bqriir_handle_t bqriir_init(void * objmem, int M, const f24 * restrict coef);

void bqriir_24bit_mono_process( bqriir_handle_t handle, f24 * restrict r,const f24 * x, int N);
void bqriir_24bit_stereo_process( bqriir_handle_t handle, f24 * restrict r,const f24 * x, int N);
void bqriir_16bit_mono_process( bqriir_handle_t handle, short * restrict r,const short * x, int N);
void bqriir_16bit_stereo_process( bqriir_handle_t handle, short * restrict r,const short * x, int N);



/*
16 bit and 24 bit digital volume,
gain:
0x080000 << 8		x1
0x100000 << 8		x2	+6db
0x200000 << 8		x4	+12db
0x400000 << 8		x8	+18db
0x7fffff << 8		x16	+24db		
*/ 

void digital_vol_16bit_process (const short *x, short *r, int gain, int N);
void digital_vol_24bit_process (const int *x, int *r, int gain, int N);

void optek_hifi2_stereo_16b_to_mono_diff (
                                          short * r, // [N/2] output
                                          short * x, // [N] input
                                          int N);

#endif //__OPTEK_HIFI2_H__
