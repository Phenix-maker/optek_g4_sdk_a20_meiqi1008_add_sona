#include "os_config.h"

#include "hw_pll.h"

#include "fifo.h"
#include "sdk_com.h"
#include "audio_com.h"
#include "audio_codec.h"
#include "audio_task.h"
#include "tools.h"
#include "audio_input_receive_task.h"

#ifdef SPDIF_ENABLE
#include "audio_optical_input.h"
#include "spdif_process.h"
#endif

#include "clock_slave_sync.h"


#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
#undef DMA_TX_INT_INTERVAL_MS_TIME

#define DMA_TX_INT_INTERVAL_US_TIME(tx_byte_each_time, sample_rate)   (((tx_byte_each_time/4)*1000000)/sample_rate) //us


U8 SPDIF_STREAM_START[];
U8 SPDIF_STREAM_END[];

volatile U32 hcountms;
volatile U32 devfirst_ccount;
volatile U32 devcur_ccount;
volatile U32 clock_sync_cnt;
volatile U8 clk_fast_adjust_flag;

extern volatile U32 spdif_frame_cnt;


int spdif_clk_adaption_process(U32 pcm_block_size);


void clock_slave_sync_first_count_init(void)
{
	devfirst_ccount = 0;
}

void clock_slave_sync_init(U32 pcm_block_size, U32 sample_rate)
{
#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	clock_sync_cnt = (10000*100)/DMA_TX_INT_INTERVAL_US_TIME(pcm_block_size, sample_rate);
	clk_fast_adjust_flag = FALSE;
	DBG_Printf("clk_sync_cnt init\n\r");
#endif
}

void clock_slave_sync_adjust_flag_int(void)
{
	clk_fast_adjust_flag = FALSE;
}

int clock_host_dev_clock_get(U32 *host_clk, U32 *dev_clk)
{
#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	TX_INTERRUPT_SAVE_AREA;
	U32 sample_rate = audio_in_stream.sample_rate;

	if ( (devfirst_ccount == 0) || (hcountms == 0) )
	{
		//clk not present
		*host_clk = 1;
		*dev_clk = 1;
		return 0;
	}

	U32 first_cc,cur_cc,host_1ms_num,host_sample_num;

	TX_DISABLE;
	first_cc = devfirst_ccount;
	cur_cc = devcur_ccount;
	host_1ms_num = hcountms;
	host_sample_num = host_samples;
	TX_RESTORE;	

	DBG_Printf("host sample num:%d\n\r", host_sample_num);
	
	//host sample counts
	*host_clk = (U64)((U64)host_sample_num*SYS_CLK_FREQ_DEFAULT)/(U64)sample_rate;
	
	if (cur_cc >= first_cc)
	{
		//slave clock counts
		*dev_clk = (cur_cc - first_cc);
	}
	else
	{
		//slave clock counts
		*dev_clk = (0x100000000L + (cur_cc - first_cc));
	}
	
	return 1;
#endif //CLOCK_SYNC_WITH_MASTER_ENABLE
}

int clock_slave_sync_polling_for_spdif(U32 sample_rate, U32 pcm_block_size)
{
#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
    U32 data_len = AUDIO_FIFO_STREAM_DATA_LEN();
	U32 tmp_cnt1;
	U32 tmp_cnt2;

	if (clock_sync_cnt)
	{
		//clock_sync_cnt init value: 2500
		--clock_sync_cnt;
		tmp_cnt1=5000;

		//tmp_cnt2 = 5000
		tmp_cnt2 = (CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(pcm_block_size, sample_rate);

		if (pcm_block_size != (192*4) )
		{
			DBG_Assert(FALSE);
		}

		if (sample_rate != 48000 )
		{
			DBG_Assert(FALSE);
		}

		if (clock_sync_cnt < tmp_cnt1)
		{
			if (clock_sync_cnt % (2500*1000/DMA_TX_INT_INTERVAL_US_TIME(pcm_block_size, sample_rate)) == 0)
			{
				if (clock_sync_cnt == 0)
				{
					clock_sync_cnt = ((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(pcm_block_size, sample_rate))+
									((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(pcm_block_size, sample_rate));
				}

				spdif_clk_adaption_process(pcm_block_size);
				clock_slave_sync_first_count_init();
				clock_slave_sync_init(pcm_block_size, sample_rate);
			}
		}
		else if(clock_sync_cnt == tmp_cnt2)
		{
			clock_sync_cnt = ((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(pcm_block_size, sample_rate))+
							((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(pcm_block_size, sample_rate));

			spdif_clk_adaption_process(pcm_block_size);
			clock_slave_sync_first_count_init();
			clock_slave_sync_init(pcm_block_size, sample_rate);
		}
	}

	return FALSE;
#endif //CLOCK_SYNC_WITH_MASTER_ENABLE
}


int spdif_clk_adaption_process(U32 pcm_block_size)
{
#ifdef CLOCK_SYNC_WITH_MASTER_ENABLE
	U32 hclk,dclk;
	i32 diff;
	double drift;
	U32 sample_rate = audio_in_stream.sample_rate;
	U32 dlen;

	dlen = AUDIO_FIFO_STREAM_DATA_LEN();


	int ret = clock_host_dev_clock_get(&hclk, &dclk);
	DBG_Printf("G len:%d,dclk:%d,hclk:%d\n\r", dlen, dclk, hclk);

	if (ret)
	{
		DBG_Printf("G len:%d,dclk:%d,hclk:%d\n\r", dlen, dclk, hclk);

		#define CLK_ADJUST_OFFSET_TIMES    10

		if (clock_sync_cnt < ((CLK_SYC_TOTAL_TIME_MS*1000)/DMA_TX_INT_INTERVAL_US_TIME(pcm_block_size, sample_rate)))
		{
			if (dlen < EX_AUDIO_STREAM_THRESHOLD)
			{
				//adjust clk slower
				dclk += CLK_ADJUST_OFFSET_TIMES*(SYS_CLK_FREQ_DEFAULT/1000000);
			}
			else if (dlen > EX_AUDIO_STREAM_THRESHOLD)
			{
				//adjust clk faster
				dclk -= CLK_ADJUST_OFFSET_TIMES*(SYS_CLK_FREQ_DEFAULT/1000000);
			}
		}

		diff = dclk - hclk;
		drift = (double)diff/(double)dclk;

		hw_audio_pll_clk_adj_by_drift(drift); //adjust local clock
	}

	return 1;
#endif //CLOCK_SYNC_WITH_MASTER_ENABLE
}


#endif //CLOCK_SYNC_WITH_MASTER_ENABLE
