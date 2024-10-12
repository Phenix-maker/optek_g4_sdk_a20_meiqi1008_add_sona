#ifndef _CODEC_MALLOC_H_
#define _CODEC_MALLOC_H_

#include "c_def.h"
#include "oem.h"



/*
 ****************************************************************************************
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**
 * @enum codec_memtype
 * @brief Memory type enumeration
 */
typedef enum
{
    CODEC_MEMTYPE_SLOW = 0,   /**< Slow memory type */
    CODEC_MEMTYPE_FAST,       /**< Fast memory type */
    CODEC_MEMTYPE_COUNT
} enCODEC_MEMTYPE;

/**
 * @enum codec_mem_alignment
 * @brief Memory alignment enumeration
 */
typedef enum
{
	CODEC_MEM_ALIGN_1 = 1,//byte alignment
    CODEC_MEM_ALIGN_4 = 4,
    CODEC_MEM_ALIGN_8 = 8,
    CODEC_MEM_ALIGN_16 = 16,
    CODEC_MEM_ALIGN_32 = 32,
} enCODEC_MEM_ALIGNMENT;


/*
 ****************************************************************************************
 * CONSTANT DEFINITIONS
 ****************************************************************************************
 */
/**
 * @def MEM_ALIGN(mem, align)
 * @brief align a memory address to a given byte alignment (must be power of 2)
 */
#define MEM_ALIGN(mem, align)       (uint8_t*)(((uintptr_t)mem + (uintptr_t)align-1) & (~((uintptr_t)align-1)))

#define CODEC_SLOW_MEM_SIZE         0//0x100000

#ifdef ATMOS_CODEC_CONFIG_XML_BY_OPTEK
#define CODEC_FAST_MEM_SIZE         (370*1024)
#elif defined DOLBY_DDPLUS71_DEC_ENABLE
//#define CODEC_FAST_MEM_SIZE         (0x34000)
#define CODEC_FAST_MEM_SIZE         (0x34000+1024*12)
#elif defined DTS_DMP_DEC_EANBLE
#define CODEC_FAST_MEM_SIZE         (0x20000+1024*12)
#elif defined MP3_DECODE_ENABLE
#define CODEC_FAST_MEM_SIZE         (128*1024)
#else
#ifdef DSP_SONA_MODULE_ENABLE
#define CODEC_FAST_MEM_SIZE         ((128*1024) + (100*1024) + (38*1024) - (128*1024) - (28*1024) - (46*1024))
#else
#define CODEC_FAST_MEM_SIZE         ((128*1024) + (100*1024) + (38*1024) - (128*1024) - (28*1024))
#endif
#endif


#define XA_SCRATCH_SIZE             (550*1024)
#define XA_FAST_SCRATCH_SIZE        (130*1024)

#define MAX_MAT_INPUT_BUF_SIZE      (60*1024)
#define MAX_MAT_OUTPUT_BUF_SIZE     (120*1024)

#define MAX_OAR_INPUT_BUF_SIZE      (32*1024)
#define MAX_OAR_OUTPUT_BUF_SIZE     (12*1024)

#define MAX_DAP_INPUT_BUF_SIZE      (8*1024)
#define MAX_DAP_OUTPUT_BUF_SIZE     (8*1024)

#define MAX_DTHD_INPUT_BUF_SIZE     (24*1024)
#define MAX_DTHD_OUTPUT_BUF_SIZE    (10*1024)


extern U8 codec_slow_mem[CODEC_SLOW_MEM_SIZE];
extern U8 codec_fast_mem[CODEC_FAST_MEM_SIZE];

extern char *scratch_ptr; /*slow Scratch memory shared between all nodes*/
extern char *fast_scratch_ptr; /*fast Scratch memory shared between all nodes*/

extern char *fast_mat_input_ptr;
extern char *fast_mat_output_ptr;

extern char *fast_dthd_input_ptr;
extern char *fast_dthd_output_ptr;


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void codec_malloc_init(void *fmem, U32 fsize,void *smem, U32 ssize);
void *codec_malloc_w_memtype(enCODEC_MEMTYPE memtype, U32 size, enCODEC_MEM_ALIGNMENT align);
void codec_free_w_memtype(enCODEC_MEMTYPE memtype, void *p);
void *codec_malloc(U32 size);
void codec_free(void *p);
void codec_malloc_reset(void);


#endif //_CODEC_MALLOC_H_
