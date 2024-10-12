#ifndef __HW_SARADC__
#define __HW_SARADC__


#define AD_CH_0    0
#define AD_CH_1    2
#define AD_CH_2    1
#define BATT_CH    3

/*default detect AD channel*/
#define AD_CH_DEFAULT    AD_CH_0

#define BATT_CH_AD_SCAN_ENABLE
#ifdef BATT_CH_AD_SCAN_ENABLE
extern U8 batt_val_det_en;
extern U16 batt_val;
#endif


/*AD_USED_NUMBER support setting 0/1/2/3*/
#define AD_USED_NUMBER           1

#if (AD_USED_NUMBER == 1)      /*first ch == AD_CH_0/AD_CH_1/AD_CH_2*/
#define AD_SCAN_FIRST_CH        AD_CH_DEFAULT
#elif (AD_USED_NUMBER == 2)    /*first ch == AD_CH_0/AD_CH_1/AD_CH_2; second ch == AD_CH_0/AD_CH_1/AD_CH_2*/
#define AD_SCAN_FIRST_CH        AD_CH_0
#define AD_SCAN_SECOND_CH       AD_CH_1
#elif (AD_USED_NUMBER == 3)    /*first ch == AD_CH_0/AD_CH_1/AD_CH_2; second ch == AD_CH_0/AD_CH_1/AD_CH_2; third ch == AD_CH_0/AD_CH_1/AD_CH_2*/
#define AD_SCAN_FIRST_CH        AD_CH_0
#define AD_SCAN_SECOND_CH       AD_CH_1
#define AD_SCAN_THIRD_CH        AD_CH_2
#endif

// #define AD_SCAN_DEBUG
#ifdef AD_SCAN_DEBUG
#define AD_MAX   1//AD_USED_NUMBER
#endif


void hw_sadc_init(void);

void hw_saradc_read(U16 * buf);
void hw_saradc_oneAdCh_read(U16 * buf, U8 ad_ch);
void hw_saradc_oneAdKey_read(U16 * buf, U8 ad_ch, U8 batt_val_det_en, U16 *batt_val);
void hw_saradc_twoAdKey_read(U16 * buf, U8 ad_ch_first, U8 ad_ch_second, U8 batt_val_det_en, U16 *batt_val);
void hw_saradc_threeAdKey_read(U16 * buf, U8 ad_ch_first, U8 ad_ch_second, U8 ad_ch_third, U8 batt_val_det_en, U16 *batt_val);
void hw_saradc_multiAdCh_read( U16 ad_ch_matrix[4]);

void sadc_test(void);

#endif /*__HW_SARADC__*/
