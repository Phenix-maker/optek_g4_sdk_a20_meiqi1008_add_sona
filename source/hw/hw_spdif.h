#ifndef __HW_SPDIF_H__
#define __HW_SPDIF_H__


#define SPDIF_IN_CH_0_PIN    GPIO0_12_INDEX
#define SPDIF_IN_CH_1_PIN    GPIO0_29_INDEX
#define SPDIF_OUT_CH_1_PIN   GPIO0_20_INDEX


enum {
	SPDIF_IN_CH_0 = 0,
	SPDIF_IN_CH_1 = 1,
	SPIF_IN_CH_LAST
};

/*
 * SPDIF_IN_CH_0
 * SPDIF_IN_CH_1
 */
#define SPDIF_IN_CH_DEFAULT    SPDIF_IN_CH_0


void hw_spdif_dec_enable(void);
void hw_spdif_dec_disable(void);
void hw_spdif_sr_detect_enable(void);
void hw_spdif_sr_detect_disable(void);


#endif ///__HW_SPDIF_H__
