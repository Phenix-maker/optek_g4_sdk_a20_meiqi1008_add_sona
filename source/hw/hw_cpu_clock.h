#ifndef __HW_CPU_CLOCK__
#define __HW_CPU_CLOCK__


enum {
	CPU_CLK_SRC_SEL_PLL = 1,
	CPU_CLK_SRC_SEL_XTAL = 2,
};

enum {
	CPU_CLK_FREQ_SEL_48MHZ = 0,
	CPU_CLK_FREQ_SEL_96MHZ = 1,
};


#define CLK_MHZ    (1000*1000)
#define CLK_KHZ    (1000)

#define CRYSTAL_FREQ            (48*CLK_MHZ) //Note that this value is a hardware decision, so don't change it

#define CPU_FREQ_24MHz          (24*CLK_MHZ)
#define CPU_FREQ_32MHz          (32*CLK_MHZ)
#define CPU_FREQ_48MHz          (48*CLK_MHZ)
#define CPU_FREQ_48MHz_2X       (96*CLK_MHZ)


//#define CPU_FREQ_BASE           (11289600)
#define CPU_FREQ_BASE           (12288*CLK_KHZ)

#define CPU_FREQ_BASE_16X       (CPU_FREQ_BASE * 16) //196.608 Mhz
#define CPU_FREQ_BASE_32X       (CPU_FREQ_BASE * 32) //393.216 Mhz
#define CPU_FREQ_BASE_64X       (CPU_FREQ_BASE * 64) //786.432 Mhz

/*
 * CPU_FREQ_48MHz
 * CPU_FREQ_48MHz_2X
 * CPU_FREQ_BASE_16X
 * CPU_FREQ_BASE_32X
 * CPU_FREQ_BASE_64X
 */

#define SYS_CLK_FREQ_DEFAULT    CPU_FREQ_BASE_32X//CPU_FREQ_48MHz_2X

#define SYSTEM_CLK_FREQ         SYS_CLK_FREQ_DEFAULT


 //must less than CRYSTAL_FREQ/2. Note that this affects uarts, spi, pwm, i2c, timers and os tick etc.
#define PERI_CLK_HZ             (CRYSTAL_FREQ / 2)

#define PERI_CLK_DIVISOR        (CRYSTAL_FREQ / PERI_CLK_HZ)


extern U32 SystemClock;


void hw_cpu_clock_init(void);
void hw_cpu_clk_set(U32 freq);
void hw_clk_pdm_ds_set(U8 ds);

void hw_cpu_clock_test(void);

#endif //__HW_CPU_CLOCK__
