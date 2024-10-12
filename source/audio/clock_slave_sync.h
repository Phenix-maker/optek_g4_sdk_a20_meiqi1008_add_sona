#ifndef __CLOCK_SLAVE_SYNC__
#define __CLOCK_SLAVE_SYNC__


//host ms counter
extern volatile U32 hcountms;
#define host_samples hcountms

extern volatile U32 devfirst_ccount;
extern volatile U32 devcur_ccount;
extern volatile U32 clock_sync_cnt;
extern volatile U8 clk_fast_adjust_flag;

extern volatile U32 spdif_frame_cnt;


void clock_slave_sync_first_count_init(void);
void clock_slave_sync_init(U32 pcm_block_size, U32 saple_rate);

int host_dev_clock_get(U32 *host_clk, U32 *dev_clk);
int clock_slave_sync_polling_for_spdif(U32 sample_rate, U32 pcm_block_size);

int spdif_clk_adaption_process(U32 pcm_block_size);


#endif //__CLOCK_SLAVE_SYNC__
