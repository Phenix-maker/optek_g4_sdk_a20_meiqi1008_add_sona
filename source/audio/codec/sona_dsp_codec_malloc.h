#ifndef __SONA_DSP_CODEC_MALLOC_H__
#define __SONA_DSP_CODEC_MALLOC_H__

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

#ifdef SONA_DSP_MALLOC_USED_CODE_MALLOC
#define SONA_DSP_CODEC_SLOW_MEM_SIZE         0
#define SONA_DSP_CODEC_FAST_MEM_SIZE         0
#else
#define SONA_DSP_CODEC_SLOW_MEM_SIZE         0//0x100000
#define SONA_DSP_CODEC_FAST_MEM_SIZE         64//min size, current no used.
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


extern U8 sona_dsp_codec_slow_mem[SONA_DSP_CODEC_SLOW_MEM_SIZE];
extern U8 sona_dsp_codec_fast_mem[SONA_DSP_CODEC_FAST_MEM_SIZE];

extern char *sona_dsp_scratch_ptr; /*slow Scratch memory shared between all nodes*/
extern char *sona_dsp_fast_scratch_ptr; /*fast Scratch memory shared between all nodes*/

extern char *sona_dsp_fast_mat_input_ptr;
extern char *sona_dsp_fast_mat_output_ptr;

extern char *sona_dsp_fast_dthd_input_ptr;
extern char *sona_dsp_fast_dthd_output_ptr;


/*
 ****************************************************************************************
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void sona_dsp_codec_malloc_init(void *fmem, U32 fsize,void *smem, U32 ssize);
void *sona_dsp_codec_malloc_w_memtype(enCODEC_MEMTYPE memtype, U32 size, enCODEC_MEM_ALIGNMENT align);
void sona_dsp_codec_free_w_memtype(enCODEC_MEMTYPE memtype, void *p);
void *sona_dsp_codec_malloc(U32 size);
void sona_dsp_codec_free(void *p);
void sona_dsp_codec_malloc_reset(void);

#ifdef SONA_DSP_ENABLE
#ifdef SONA_DSP_MALLOC_PRINTF
extern U32 sona_dsp_malloc_print_size;
extern U32 sona_dsp_L2_malloc_print_size;
#endif

void * sona_dsp_malloc ( unsigned long long size );
void sona_dsp_free(void *p);
void *sona_dsp_pc_tools_malloc(U32 size);
void sona_dsp_pc_tools_free(void *p);
#endif

#endif //_CODEC_MALLOC_H_
