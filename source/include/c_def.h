#ifndef _C_DEF_H_
#define _C_DEF_H_

/* Standard C Included Files */
#include <stddef.h>
#include <stdint.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif

#define VOID                     void
typedef char                     CHAR;
typedef unsigned char            UCHAR;
typedef int                      INT;
typedef unsigned int             UINT;
typedef long                     LONG;
typedef unsigned long            ULONG;
typedef short                    SHORT;
typedef unsigned short           USHORT;

typedef	UCHAR	U8;
typedef	USHORT	U16;
typedef	UINT	U32;
typedef	unsigned long long	U64;

typedef	UCHAR	u8;
typedef	USHORT	u16;
typedef	UINT	u32;
typedef	unsigned long long	u64;

typedef	CHAR	S8;
typedef	SHORT	S16;
typedef	INT		S32;

#define I64    long long
#define I32    int
#define i32    int
#define I16    short int
#define i16    short int
#define I8     signed char
#define i8     signed char


typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed char    INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed short   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed int     INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

typedef INT32U         OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef INT32U         OS_CPU_SR;                /* Define size of CPU status register (PSW = 32 bits) */

typedef INT8S          BYTE;
typedef INT8U          UBYTE;
typedef INT16S         WORD;
typedef INT16U         UWORD;

typedef INT8S          WORD8;
typedef INT8U          UWORD8;

typedef INT16S         WORD16;
typedef INT16U         UWORD16;

typedef signed char         *   pWORD8;
typedef unsigned char       *   pUWORD8;

typedef signed short        *   pWORD16 ;/* ps      pWORD16  ps_count   2   */
typedef unsigned short      *   pUWORD16;/* pus     pUWORD16 pus_count  2   */

typedef signed int              WORD32  ;/* i       WORD32   i_count    4   */
typedef signed int          *   pWORD32 ;/* pi      pWORD32  pi_count   4   */
typedef unsigned int            UWORD32 ;/* ui      UWORD32  ui_count   4   */
typedef unsigned int        *   pUWORD32;/* pui     pUWORD32 pui_count  4   */

typedef void                *   pVOID   ;/* pv      pVOID    pv_flag    4   */

/* variable size types: platform optimized implementation */
typedef unsigned int            UBOOL   ;/* ubool   BOOL     ubool_true     */
typedef unsigned int            BOOL    ;/* ubool   BOOL     ubool_true     */
typedef signed int              FLAG    ;/* flag    FLAG     flag_false     */
typedef unsigned int            UFLAG   ;/* uflag   FLAG     uflag_false    */
typedef signed int              LOOPIDX ;/* lp      LOOPIDX  lp_index       */
typedef unsigned int            ULOOPIDX;/* ulp     SLOOPIDX ulp_index      */

typedef LOOPIDX                 LOOPINDEX; /* lp    LOOPIDX  lp_index       */
typedef ULOOPIDX                ULOOPINDEX;/* ulp   SLOOPIDX ulp_index      */

/****************************************************************************/
/*     types               type define    prefix        examples      bytes */
/************************  ***********    ******    ****************  ***** */

typedef signed int              WORD24  ;/* k       WORD24   k_count    3   */
typedef signed int          *   pWORD24 ;/* pk      pWORD24  pk_count   3   */
typedef unsigned int            UWORD24 ;/* uk      UWORD24  uk_count   3   */
typedef unsigned int        *   pUWORD24;/* puk     pUWORD24 puk_count  3   */

typedef signed long long        WORD40  ;/* m       WORD40   m_count    5   */
typedef signed long long    *   pWORD40 ;/* pm      pWORD40  pm_count   5   */
typedef unsigned long long      UWORD40 ;/* um      UWORD40  um_count   5   */
typedef unsigned long long  *   pUWORD40;/* pum     pUWORD40 pum_count  5   */

typedef signed long long        WORD64  ;/* h       WORD64   h_count    8   */
typedef signed long long    *   pWORD64 ;/* ph      pWORD64  ph_count   8   */
typedef unsigned long long      UWORD64 ;/* uh      UWORD64  uh_count   8   */
typedef unsigned long long  *   pUWORD64;/* puh     pUWORD64 puh_count  8   */

typedef float                   FLOAT32 ;/* f       FLOAT32  f_count    4   */
typedef float               *   pFLOAT32;/* pf      pFLOAT32 pf_count   4   */
typedef double                  FLOAT64 ;/* d       UFLOAT64 d_count    8   */
typedef double              *   pFlOAT64;/* pd      pFLOAT64 pd_count   8   */


#define guint unsigned int
#define gboolean int
#define gpointer void *
#define gsize size_t
#define guint8 unsigned char
#define guint16  unsigned short
typedef long off_t;

#define WORD    unsigned short
#define DWORD   unsigned long
#define BYTE    unsigned char
#define BOOL    unsigned char


typedef void (*FUNCTION) (void);
typedef void (*FUNCTION_PARM1) (U32 PARM1);
typedef void (*FUNCTION_PARM2) (U32 PARM1, U32 PARM2);
typedef void (*FUNCTION_PARM3) (U32 PARM1, U32 PARM2, U32 PARM3);


#define bit0		(1<<0)
#define bit1		(1<<1)
#define bit2		(1<<2)
#define bit3		(1<<3)
#define bit4		(1<<4)
#define bit5		(1<<5)
#define bit6		(1<<6)
#define bit7		(1<<7)

#define bit8		(1<<8)
#define bit9		(1<<9)
#define bit10		(1<<10)
#define bit11		(1<<11)
#define bit12		(1<<12)
#define bit13		(1<<13)
#define bit14		(1<<14)
#define bit15		(1<<15)

#define bit16		(1<<16)
#define bit17		(1<<17)
#define bit18		(1<<18)
#define bit19		(1<<19)
#define bit20		(1<<20)
#define bit21		(1<<21)
#define bit22		(1<<22)
#define bit23		(1<<23)

#define bit24		(1<<24)
#define bit25		(1<<25)
#define bit26		(1<<26)
#define bit27		(1<<27)
#define bit28		(1<<28)
#define bit29		(1<<29)
#define bit30		(1<<30)
#define bit31		(1<<31)

/*USE FOR REGISTR BITS*/
typedef struct {
    unsigned int   b0    :1;
    unsigned int   b1    :1;
    unsigned int   b2    :1;
    unsigned int   b3    :1;
    unsigned int   b4    :1;
    unsigned int   b5    :1;
    unsigned int   b6    :1;
    unsigned int   b7    :1;
	
    unsigned int   b8    :1;
    unsigned int   b9    :1;
    unsigned int   b10   :1;
    unsigned int   b11   :1;
    unsigned int   b12   :1;
    unsigned int   b13   :1;
    unsigned int   b14   :1;
    unsigned int   b15   :1;
	
    unsigned int   b16   :1;
    unsigned int   b17   :1;
    unsigned int   b18   :1;
    unsigned int   b19   :1;
    unsigned int   b20   :1;
    unsigned int   b21   :1;
    unsigned int   b22   :1;
    unsigned int   b23   :1;
	
    unsigned int   b24   :1;
    unsigned int   b25   :1;
    unsigned int   b26   :1;
    unsigned int   b27   :1;
    unsigned int   b28   :1;
    unsigned int   b29   :1;
    unsigned int   b30   :1;
    unsigned int   b31   :1;
} BITS;

#endif //_C_DEF_H_
