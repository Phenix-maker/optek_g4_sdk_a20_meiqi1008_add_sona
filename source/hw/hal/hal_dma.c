#include "os_config.h"

#include "regmap.h"

#include "mem_reloc.h"

#include "hal_dma.h"
#include "hal_sflash.h"

#define SIMPLE_DMA

#ifdef SIMPLE_DMA

dma_done_callback dma_callbacks[DMA_COUNT];

void hal_dma_module_init(void)
{
	memset(dma_callbacks, 0, sizeof(dma_callbacks));
}

void hal_dma_transfer_init(enumDMA_ID dma_id, U32 *dst, U32 *src, enumDMA_TRIG dma_trig, enumDMA_TRANS_BYTEs size, 
		enumDMA_TRANS_BURST_SIZE b_size, BOOL src_inc, BOOL dst_inc, BOOL int_enb, dma_done_callback callback)
{
	DMA_CNTRs *dma_cntr = (DMA_CNTRs *) (DMA_CONFIG_BASE_ADDR + (int)dma_id*DMA_CNTRs_SIZE);

	//sanity
	DBG_Assert((int)dma_id < DMA_ID_MAX);
	DBG_Assert(dma_trig < SOURCE_DMA_MAX);
	DBG_Assert(size < DMA_TRANS_BYTEs_MAX);
	DBG_Assert(b_size <= DMA_TRANS_BURST_MAX);	

	if ((int)dma_id < DMA_ID_MAX)
	{
		if (dma_callbacks[dma_id] != callback)
		{
			if (dma_callbacks[dma_id] != NULL)
			{
				//permit DMA to reuse
				asm("nop");
			}

			dma_callbacks[dma_id] = callback;
		}

		dma_cntr->sub_cntr.channel_enb = 0;

		dma_cntr->count = 0;
		dma_cntr->source = (U32)src;
		dma_cntr->dest = (U32)dst;
		dma_cntr->sub_cntr.size = size;
		dma_cntr->sub_cntr.burst_size = b_size;
		dma_cntr->sub_cntr.saddr_inc = src_inc;
		dma_cntr->sub_cntr.daddr_inc = dst_inc;
		dma_cntr->sub_cntr.source = dma_trig;
		dma_cntr->sub_cntr.start = 0;
		dma_cntr->sub_cntr.int_enb = int_enb;
		dma_cntr->sub_cntr.channel_enb = 1;
	}
}

void hal_dma_close(enumDMA_ID dma_id)
{
	TX_INTERRUPT_SAVE_AREA;

	DBG_Printf("%s\n\r", __func__);
	DBG_Assert((int)dma_id <  DMA_ID_MAX);

	TX_DISABLE;

	if ((int)dma_id < DMA_ID_MAX)
	{
		if (dma_id != 1)
		{
			DMA_CNTRs *dma_cntr = (DMA_CNTRs *) (DMA_CONFIG_BASE_ADDR + (int)dma_id*DMA_CNTRs_SIZE);

			dma_cntr->sub_cntr.int_enb = 0;
			dma_cntr->sub_cntr.channel_enb = 0;
			dma_cntr->count = 0;
		}
		else
		{
			DMA_1_CNTR = 0;
			DMA_1_COUNT = 0;
		}

		if (dma_callbacks[dma_id] != NULL)
		{
			dma_callbacks[dma_id] = NULL;
		}
		else
		{
			//DMA of dma_id is idle
		}
	}

	TX_RESTORE;
}


void L3_int9_extlevel(void *param) __INTERNAL_RAM_TEXT;
void L3_int9_extlevel(void *param)
{
	U32 dma_channel;
	int i;

	#ifdef SAVE_MODULE_FROM_FLASH
	sflash_in_programming_check();
	#endif


	dma_channel = DMA_GCNTR;
	
	for (i = 0; i < DMA_COUNT; i++)
	{
		if ((dma_channel & DMA_CHANNEL(i)) == DMA_CHANNEL(i))
		{
			//Clear dma channel
			DMA_GCNTR = (0x00001000 | DMA_CHANNEL(i));

			if (i == SPI_TX_DMA_ID)
			{				
				//asm("break 1,1");
			}

			if (i == SPI_RX_DMA_ID)
			{				
				//asm("break 1,1");
			}

			if (dma_callbacks[i] != NULL)
			{
				dma_callbacks[i]();
			}
		}	
	}
}

#endif



