#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hw_timer.h"

#include "fifo.h"
#include "audio_task.h"
#include "audio_com.h"
#include "audio_output.h"
#include "codec_malloc.h"
#include "CFasm_memcpy.h"


#define USE_FREE_RTOS_HEAP


U8 codec_slow_mem[CODEC_SLOW_MEM_SIZE] __attribute__ ((section (".codec_slow_memory_bss")));
U8 codec_fast_mem[CODEC_FAST_MEM_SIZE];

typedef struct {
	U8 *codec_fmem_start;
	U8 *codec_fmem_end;
	U8 *codec_fmem_ptr;

	U8 *codec_smem_start;
	U8 *codec_smem_end;
	U8 *codec_smem_ptr;	
} CODEC_MALLOC_ENV_STRU;

static CODEC_MALLOC_ENV_STRU codec_malloc_env = {0};

void codec_malloc_init(void *fmem, U32 fsize,void *smem, U32 ssize)
{
    uint8_t *byte_mem;

    DBG_Assert(NULL != fmem);

    #ifdef USE_FREE_RTOS_HEAP
    prvHeapInit_codec();
    #else
	byte_mem = (uint8_t *) fmem;
    codec_malloc_env.codec_fmem_start = byte_mem;
    codec_malloc_env.codec_fmem_end = byte_mem + fsize;
    codec_malloc_env.codec_fmem_ptr = codec_malloc_env.codec_fmem_start;
    #endif

    DBG_Assert(NULL != smem);
	byte_mem = (uint8_t *) smem;
    codec_malloc_env.codec_smem_start = byte_mem;
    codec_malloc_env.codec_smem_end = byte_mem + ssize;
    codec_malloc_env.codec_smem_ptr = codec_malloc_env.codec_smem_start;

}


void codec_malloc_reset(void)
{
	//slow memory reset
    codec_malloc_env.codec_smem_ptr = codec_malloc_env.codec_smem_start;
    if (codec_malloc_env.codec_smem_ptr)
    {
        memset(codec_malloc_env.codec_smem_ptr, '\0', codec_malloc_env.codec_smem_end - codec_malloc_env.codec_smem_start);
    }

	//fast memory reset
    #ifdef USE_FREE_RTOS_HEAP
    prvHeapInit_codec();
    #else
    codec_malloc_env.codec_fmem_ptr = codec_malloc_env.codec_fmem_start;
    if (codec_malloc_env.codec_fmem_ptr)
    {
        memset(codec_malloc_env.codec_fmem_ptr, '\0', codec_malloc_env.codec_fmem_end - codec_malloc_env.codec_fmem_start);
    }
    #endif	
}

void *codec_malloc_w_memtype(enCODEC_MEMTYPE memtype, U32 size, enCODEC_MEM_ALIGNMENT align)
{
	void *mem = NULL;

	if (memtype == CODEC_MEMTYPE_FAST)
	{
        #ifdef USE_FREE_RTOS_HEAP
        //notes :fixed alignment to 64
        mem = pvPortMalloc_codec(size);
        if (xPortGetFreeHeapSize_codec() < 0x10000)
		{
            printf_info("codec malloc size:%d, remain size:%d\n", size, xPortGetFreeHeapSize_codec());
		}
        #else
		{
			codec_malloc_env.codec_fmem_ptr = MEM_ALIGN(codec_malloc_env.codec_fmem_ptr, align);
		}

        if (codec_malloc_env.codec_fmem_ptr + size <= codec_malloc_env.codec_fmem_end)
        {
            mem = codec_malloc_env.codec_fmem_ptr;
            codec_malloc_env.codec_fmem_ptr += size;
			printf_info("size:%d, free:%d\n", size, codec_malloc_env.codec_fmem_end - codec_malloc_env.codec_fmem_ptr);
        }
        else
        {
            printf_info("## codec malloc fast mem overflow ##\n\r");

            DBG_Assert(FALSE);
        }
        #endif
	}
	else
	{
		codec_malloc_env.codec_smem_ptr = MEM_ALIGN(codec_malloc_env.codec_smem_ptr, (int)align);

        if (codec_malloc_env.codec_smem_ptr + size <= codec_malloc_env.codec_smem_end)
        {
            mem = codec_malloc_env.codec_smem_ptr;
            codec_malloc_env.codec_smem_ptr += size;
            printf_info("slow codec malloc size:%d, remain size:%d\n", size, codec_malloc_env.codec_smem_end - codec_malloc_env.codec_smem_ptr);
        }
        else
        {
            printf_fatal("##### codec malloc slow mem overflow #####\n");
            delayms(1000);
			
            DBG_Assert(FALSE);
        }
	}
	
	return mem;
}

void codec_free_w_memtype(enCODEC_MEMTYPE memtype, void *p)
{
	if (memtype == CODEC_MEMTYPE_FAST)
	{
        #ifdef USE_FREE_RTOS_HEAP
        vPortFree_codec(p);
        #else
        //no need free
        #endif
	}
	else
	{
        //no need free
	}
}

void *codec_malloc(U32 size)
{
	return codec_malloc_w_memtype(CODEC_MEMTYPE_FAST, size, CODEC_MEM_ALIGN_16);
}

void codec_free(void *p)
{
	codec_free_w_memtype(CODEC_MEMTYPE_FAST, p);
}
