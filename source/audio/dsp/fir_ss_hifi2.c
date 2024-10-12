/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrit.com, info@integrit.com                    */
/*                                                                          */
/*  NatureDSP Signal Processing Library                                     */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Tensilica HiFi2 core only and must not be used for any other    */
/* processors and platforms. The license to use these sources was given to  */
/* Tensilica, Inc. under Terms and Condition of a Software License Agreement*/
/* between Tensilica, Inc. and IntegrIT, Ltd. Any third party must not use  */
/* this code in any form unless such rights are given by Tensilica, Inc.    */
/* Compiling, linking, executing or using this library in any form you      */
/* unconditionally accept these terms.                                      */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2010 IntegrIT, Limited.                           */
/*                          All Rights Reserved.                            */
/* ------------------------------------------------------------------------ */ 
/*
	NatureDSP_Signal library. FIR part
    C code with generic optimization
	Integrit, 2006-2010
*/

/*-------------------------------------------------------------------------
  Single-sample real FIR filter.
  Passes one sample via real FIR filter (direct-form) using IR stored in 
  vector h and moves delay line with samples. The real data input (delay line) 
  is stored in vector x. The filter output result is stored in vector r. 
  The filter calculates one output sample using M coefficients and requires 
  M samples on the input.
  NOTE:
  The difference with fir_bk() function is that this function returns sample 
  in Q30, not Q31 format.

  Input:
  x[M]      - input data. First in time corresponds to x[0],Q31
  h[M]      - filter coefficients in normal order,Q31
  M         - length of filter
  
  Output:
  x         - shifted in time input samples by 1 sample. 

  Returned value:
  filtered data,Q30 

  Restriction:
  x,h should not be overlapped
  M>0

  PERFORMANCE NOTE:
  for optimum performance follow rules:
  x,h - should be aligned by 8 bytes
  M - divisible by 4 and >8
  otherwise performance not guaranteed
-------------------------------------------------------------------------*/
#include "common.h"
#include "mem_reloc.h"

static int32_t fir_ss_x2 (f24 *restrict x, const f24 *restrict h, int M)
{
    ae_p24f *px0 = (      ae_p24f *)x;
    const ae_p24f *ph0 = (const ae_p24f *)(h+M);
    ae_q56s y0;
    ae_p24x2s x0, h0;
    int i;
    

    if(M&0x1) {
        // load data and prepare pointers for pre-increment
        LD_P24X2F_IU(x0, px0,  0);
        LD_P24F_IU  (h0, ph0, -1);

        // take out first iteration due to odd M
        y0 = AE_MULP24S_HH(x0, h0);

        // iterate M/2 times
        for (i = 0; i < (M-1)>>1; i++) {
            ae_p24x2s x1, del;

            // load data with pre-increment
            LD_P24X2F_IU(x1, px0,  1);
            LD_P24X2F_IU(h0, ph0, -1);

            // move delay line (a, b) (c, d) -> (b, c)
            del = AE_SELP24_LH(x0, x1);

            // MAC
            AE_MULAAP24S_HL_LH(y0, del, h0);

            // store delay
            ST_P24X2F_I (del, px0, -1);

            // advance to next iter
            x0 = x1;
        }
    } else {

        // load data and prepare pointers for pre-increment
        LD_P24X2F_IU(x0, px0,   0);
        LD_P24X2F_IU(h0, ph0,  -1);

        // take out first half-iteration to enable software pipeline
        y0 = AE_MULP24S_HL(x0, h0);

        // iterate (M/2-1) times
        for (i = 0; i < (M-1)>>1; i++) {
            ae_p24x2s x1, h1, del;

            // load h0 and next x1 with pre-increment
            LD_P24X2F_IU(x1, px0,  1);
            LD_P24X2F_IU(h1, ph0, -1);

            // load data with pre-increment
            del = AE_SELP24_LH(x0, x1);

            // MAC
            AE_MULAP24S_HH(y0, del, h0);
            AE_MULAP24S_LL(y0, del, h1);

            // store delay
            ST_P24X2F_I (del, px0, -1);

            // advance to next iter
            x0 = x1;
            h0 = h1;
        }

        // take out last half-iteration to balance loop prologue
        AE_MULAP24S_LH(y0, x0, h0);
        ST_P24F_I(x0, px0, 0);
    }

    y0 = AE_ROUNDSQ32SYM(y0);
    return AE_TRUNCA32Q48(y0);
}


f24 fir_ss (
          f24 *restrict x, // [M]
    const f24 *restrict h, // [M]
          int M
);//	__DSP_TEXT;


f24 fir_ss (
          f24 *restrict x, // [M]
    const f24 *restrict h, // [M]
          int M
)
{
    int32_t z;

    if(IS_ALIGN(x) && IS_ALIGN(h)) {
        // fastest
        z = fir_ss_x2(x, h, M);
    } else {
        ae_q56s y0 = AE_ZEROQ56();
        int i;
        // non-optimized implementation for non-aligned input
        {
            ae_p24x2s x0 = load_24f(&x[0]);
            ae_p24x2s h0 = load_24f(&h[M-1-0]);
            mac_24i(y0, x0, h0);
        }
        for (i = 1; i < M; i++) {
            ae_p24x2s x0 = load_24f(&x[i]);
            ae_p24x2s h0 = load_24f(&h[M-1-i]);
            mac_24i(y0, x0, h0);
            //store_24f(x0, &x[i-1]);
        }
        y0 = AE_ROUNDSQ32SYM(y0);
        z = AE_TRUNCA32Q48(y0);
    }
    return z;
}

