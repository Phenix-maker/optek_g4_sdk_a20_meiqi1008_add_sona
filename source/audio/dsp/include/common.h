#ifndef __COMMON_H__
#define __COMMON_H__

#include <assert.h>


#define OPTEK_DSP_DAPP_LIB


#define __RENAMING__

#include "NatureDSP_Signal.h"

#ifdef __RENAMING__
#include "__renaming__.h"
#else
#error !!!!!
#endif

// ae_p16s
#define LD_P16F_I(   x, p, idx)   x = AE_LP16F_I    (   (ae_p16s *)(p), (idx)*(int)sizeof(ae_p16s))
#define LD_P16F_IU(  x, p, idx)       AE_LP16F_IU   (x, (ae_p16s *)(p), (idx)*(int)sizeof(ae_p16s))
#define ST_P16F_I(   x, p, idx)       AE_SP16F_L_I  (x, (ae_p16s *)(p), (idx)*(int)sizeof(ae_p16s))
#define ST_P16F_IU(  x, p, idx)       AE_SP16F_L_IU (x, (ae_p16s *)(p), (idx)*(int)sizeof(ae_p16s))

// ae_p24f
#define LD_P24F_I(   x, p, idx)   x = AE_LP24F_I    (   (ae_p24f *)(p), (idx)*(int)sizeof(ae_p24f))
#define LD_P24F_IU(  x, p, idx)       AE_LP24F_IU   (x, (ae_p24f *)(p), (idx)*(int)sizeof(ae_p24f))
#define ST_P24F_I(   x, p, idx)       AE_SP24F_L_I  (x, (ae_p24f *)(p), (idx)*(int)sizeof(ae_p24f))
#define ST_P24F_IU(  x, p, idx)       AE_SP24F_L_IU (x, (ae_p24f *)(p), (idx)*(int)sizeof(ae_p24f))


// ae_p24f
#define LD_P24S_I(   x, p, idx)   x = AE_LP24_I     (   (ae_p24s *)(p), (idx)*(int)sizeof(ae_p24s))
#define LD_P24S_IU(  x, p, idx)       AE_LP24_IU    (x, (ae_p24s *)(p), (idx)*(int)sizeof(ae_p24s))
#define ST_P24S_I(   x, p, idx)       AE_SP24S_L_I  (x, (ae_p24s *)(p), (idx)*(int)sizeof(ae_p24s))
#define ST_P24S_IU(  x, p, idx)       AE_SP24S_L_IU (x, (ae_p24s *)(p), (idx)*(int)sizeof(ae_p24s))

// ae_p24x2f
#define LD_P24X2F_I( x, p, idx) x = AE_LP24X2F_I  (   (ae_p24x2f *)(p), (idx)*(int)sizeof(ae_p24x2f))
#define LD_P24X2F_IU(x, p, idx)     AE_LP24X2F_IU (x, (ae_p24x2f *)(p), (idx)*(int)sizeof(ae_p24x2f))
#define LD_P24F_I(   x, p, idx) x = AE_LP24F_I    (   (ae_p24f   *)(p), (idx)*(int)sizeof(ae_p24f))
#define LD_P24F_IU(  x, p, idx)     AE_LP24F_IU   (x, (ae_p24f   *)(p), (idx)*(int)sizeof(ae_p24f))

// ae_p16x2f
#define LD_P16X2F_I( x, p, idx) x = AE_LP16X2F_I  (   (ae_p16x2s *)(p), (idx)*(int)sizeof(ae_p16x2s))
#define LD_P16X2F_IU(x, p, idx)     AE_LP16X2F_IU (x, (ae_p16x2s *)(p), (idx)*(int)sizeof(ae_p16x2s))
#define ST_P16X2F_I(x, p, idx)		AE_SP16X2F_I  (x, (ae_p16x2s *)(p), (idx)*(int)sizeof(ae_p16x2s))
#define ST_P16X2F_IU(x, p, idx)		AE_SP16X2F_IU (x, (ae_p16x2s *)(p), (idx)*(int)sizeof(ae_p16x2s))



// ae_q32f
#define LD_Q32_I(    x, p, idx) x = AE_LQ32F_I    (   (ae_q32s   *)(p), (idx)*(int)sizeof(ae_q32s))
#define LD_Q32F_IU(  x, p, idx)     AE_LQ32F_IU   (x, (ae_q32s   *)(p), (idx)*(int)sizeof(ae_q32s))

#define ST_P24X2F_I( x, p, idx)     AE_SP24X2F_I  (x, (ae_p24x2f *)(p), (idx)*(int)sizeof(ae_p24x2f))
#define ST_P24X2F_IU(x, p, idx)     AE_SP24X2F_IU (x, (ae_p24x2f *)(p), (idx)*(int)sizeof(ae_p24x2f))

#define ST_Q32S_I(   x, p, idx)       AE_SQ32F_I(  x, (ae_q32s   *)(p), (idx)*sizeof(ae_q32s))

#define LD_Q56S_I(   x, p, idx)   x = AE_LQ56_I   (   (ae_q56s *)(p), (idx)*(int)sizeof(ae_q56s))
#define LD_Q56S_IU(  x, p, idx)       AE_LQ56_IU  (x, (ae_q56s *)(p), (idx)*(int)sizeof(ae_q56s))
#define ST_Q56S_I(   x, p, idx)       AE_SQ56S_I  (x, (ae_q56s *)(p), (idx)*(int)sizeof(ae_q56s))
#define ST_Q56S_IU(  x, p, idx)       AE_SQ56S_IU (x, (ae_q56s *)(p), (idx)*(int)sizeof(ae_q56s))

// testing stuff
#define load_24f(p)     AE_LP24F_I  (   (const ae_p24f *)(p), 0)  // load  x      -> {x, x}
#define store_24f(x, p) AE_SP24F_L_I(x, (      ae_p24f *)(p), 0)  // store {h, l} -> l

#define load_56s(p)     AE_LQ56_I   (   (const ae_q56s *)(p), 0)
#define store_56s(x, p) AE_SQ56S_I  (x, (      ae_q56s *)(p), 0)

#define mul_24i(     x, y)  AE_MULP24S_HH(x, y)
#define mul_24f(     x, y)  AE_MULFP24S_HH(x, y)
#define mac_24i(acc, x, y)  AE_MULAP24S_HH(acc, x, y)
#define mac_24f(acc, x, y)  AE_MULAFP24S_HH(acc, x, y)
#define msb_24i(acc, x, y)  AE_MULSP24S_HH(acc, x, y)
#define msb_24f(acc, x, y)  AE_MULSFP24S_HH(acc, x, y)

#define IS_ALIGN(p) ((((int)(p))&0x7) == 0) 

#ifdef _MSC_VER
    #define ALIGN(x)    _declspec(align(x)) 
#else
    #define ALIGN(x)    __attribute__((aligned(x))) 
#endif

#define INV_TBL_BITS 7
extern const int32_t tab_invQ30[128];

#if XCHAL_HAVE_NSA
  #define NSA(n) XT_NSA(n)
#else
  inline_ int32_t NSA(int32_t n)
  {
    ae_q56s t;
    if (!n) return 31;
    t = AE_CVTQ48A32S(n);
    return AE_NSAQ56S(t)-8;
  }
#endif

#endif //__COMMON_H__
