#include "os_config.h"


#if ((defined HW_SARADC_ENABLE)||(defined OTK_HW_VERSION_ENABLE) || (defined BATT_DETECT_ENABLE))
#include "regmap.h"

#include "hw_timer.h"
#include "hw_hardware_aon.h"
#include "hw_saradc.h"


#ifdef BATT_CH_AD_SCAN_ENABLE
U8 batt_val_det_en;
U16 batt_val;
#endif


#ifdef AD_SCAN_DEBUG
U32 ValIndex_back[4];
U16 ad_data[AD_MAX];
#endif

void hw_sadc_init(void)
{
	U32 val = 0;
    MISC_SADC_CONTROLs *adc_read = (MISC_SADC_CONTROLs *) &val;

    val = REG_MISC_ADC_CTL;
    adc_read->sadc_en = 1;
    adc_read->sadc_clkDiv = 0x3ff;
    adc_read->sadc_addr = AD_SCAN_FIRST_CH;
    REG_MISC_ADC_CTL = val;

    #ifdef BATT_CH_AD_SCAN_ENABLE
    MISC_AON_CONTROLs *misc_aon_controls = (MISC_AON_CONTROLs *) &val;
    val = REG_MISC_RTC;
    misc_aon_controls->batt_lev_det_en = 1;
    REG_MISC_RTC = val;

    val = REG_MISC_RTC;
    printf_info ("batt_lev_det_en:%d\n", misc_aon_controls->batt_lev_det_en);
    #endif

    #ifdef AD_SCAN_DEBUG
    memset(ValIndex_back, 0xFFF, 4);
    #endif
}

void hw_saradc_read(U16 * buf)
{
	U32 val = 0x00;

	MISC_SADC_CONTROLs *adc_read = (MISC_SADC_CONTROLs *)&val;
    
#if (AD_USED_NUMBER == 1)

	#ifndef AD_SCAN_DEBUG
	buf[0] = REG_MISC_ADC_DATA;
	#else
	buf[0] = 0x3ff;
	ad_data[0] = REG_MISC_ADC_DATA;
	if ((ad_data[0] != ValIndex_back[AD_CH_0]))//||(adc_read->sadc_addr == AD_CH_0))
	{
		ValIndex_back[AD_CH_0] = ad_data[0];
		printf_info ("app_saradc_read ch[0] = 0x%x\n",ad_data[0]);
	}
	#endif

	val = REG_MISC_ADC_CTL;     
    if (adc_read->sadc_addr != AD_CH_DEFAULT)
    {
        adc_read->sadc_addr = AD_CH_DEFAULT;
    }
    REG_MISC_ADC_CTL = val;

#elif (AD_USED_NUMBER == 2)
    #ifndef AD_SCAN_DEBUG
	if (adc_read->sadc_addr == AD_CH_0)
	{
		buf[0] = REG_MISC_ADC_DATA;
	}
	else //if (adc_read->sadc_addr == AD_CH_1)
	{
		buf[1] = REG_MISC_ADC_DATA;
	}
	// printf_info("AD value:\n[0] = 0x%x; [1] = 0x%x\n\r",buf[0],buf[1]);
    #else
	buf[0] = 0x3ff;
    buf[1] = 0x3ff;
	if (adc_read->sadc_addr == AD_CH_0)
	{
		ad_data[0] = REG_MISC_ADC_DATA;
	}
	else //if (adc_read->sadc_addr == AD_CH_1)
	{
		ad_data[1] = REG_MISC_ADC_DATA;
	}
	if ((ad_data[0] != ValIndex_back[AD_CH_0])||(ad_data[1] != ValIndex_back[AD_CH_1]))
	{
		ValIndex_back[AD_CH_0] = ad_data[0];
        ValIndex_back[AD_CH_1] = ad_data[1];
		printf_info ("app_saradc_read ch[0] = 0x%x,ch[1] = 0x%x\n",ad_data[0],ad_data[1]);
	}
    #endif

	val = REG_MISC_ADC_CTL;     
    if (adc_read->sadc_addr == AD_CH_0)
    {
        adc_read->sadc_addr = AD_CH_1;
    }
    else
    {
    	adc_read->sadc_addr = AD_CH_0;
    }
    REG_MISC_ADC_CTL = val;

#elif (AD_USED_NUMBER == 3)
    #ifndef AD_SCAN_DEBUG
	if (adc_read->sadc_addr == AD_CH_0)
	{
		buf[0] = REG_MISC_ADC_DATA;
	}
	else if (adc_read->sadc_addr == AD_CH_1)
	{
		buf[1] = REG_MISC_ADC_DATA;
	}	
	else if (adc_read->sadc_addr == AD_CH_2)
	{
		buf[2] = REG_MISC_ADC_DATA;
	}

    #else
	if (adc_read->sadc_addr == AD_CH_0)
	{
		ad_data[0] = REG_MISC_ADC_DATA;
	}
	else if (adc_read->sadc_addr == AD_CH_1)
	{
		ad_data[1] = REG_MISC_ADC_DATA;
	}	
	else if (adc_read->sadc_addr == AD_CH_2)
	{
		ad_data[2] = REG_MISC_ADC_DATA;
	}	

	if ((ad_data[0] != ValIndex_back[AD_CH_0])||(ad_data[1] != ValIndex_back[AD_CH_1])||(ad_data[2] != ValIndex_back[AD_CH_2]))
	{
		ValIndex_back[AD_CH_0] = ad_data[0];
        ValIndex_back[AD_CH_1] = ad_data[1];
		printf_info ("app_saradc_read ch[0] = 0x%x,ch[1] = 0x%x,ch[2] = 0x%x\n",ad_data[0],ad_data[1],ad_data[1]);
	}
    #endif

	val = REG_MISC_ADC_CTL;     
    if (adc_read->sadc_addr == AD_CH_0)
    {
        adc_read->sadc_addr = AD_CH_1;
    }
    else if (adc_read->sadc_addr == AD_CH_1)
    {
        adc_read->sadc_addr = AD_CH_2;
    }
    else
    {
    	adc_read->sadc_addr = AD_CH_0;
    }
    REG_MISC_ADC_CTL = val;

#endif
}

void hw_saradc_multiAdCh_read( U16 ad_ch_matrix[4])
{
	U32 val = 0x00;

	MISC_SADC_CONTROLs *adc_read = (MISC_SADC_CONTROLs *)&val;
	val = REG_MISC_ADC_CTL;     

    if(adc_read->sadc_addr == BATT_CH)
    {
        ad_ch_matrix[BATT_CH] = REG_MISC_ADC_DATA;
        adc_read->sadc_addr = AD_CH_0;
    }
    else if( adc_read->sadc_addr == AD_CH_0)
    {
        ad_ch_matrix[AD_CH_0] = REG_MISC_ADC_DATA;
        adc_read->sadc_addr = AD_CH_1;
    }
    else if( adc_read->sadc_addr == AD_CH_1)
    {
        ad_ch_matrix[AD_CH_1] = REG_MISC_ADC_DATA;
        adc_read->sadc_addr = AD_CH_2;

    }
    else if( adc_read->sadc_addr == AD_CH_2)
    {
        ad_ch_matrix[AD_CH_2] = REG_MISC_ADC_DATA;
        adc_read->sadc_addr = BATT_CH;
    }

    REG_MISC_ADC_CTL = val;
}


void hw_saradc_oneAdCh_read(U16 * buf, U8 ad_ch)
{
	U32 val = 0x00;

	MISC_SADC_CONTROLs *adc_read = (MISC_SADC_CONTROLs *)&val;
	val = REG_MISC_ADC_CTL;     
    
    if (adc_read->sadc_addr != ad_ch)
    {
        adc_read->sadc_addr = ad_ch;
    }
    REG_MISC_ADC_CTL = val;

	#ifndef AD_SCAN_DEBUG
	*buf = REG_MISC_ADC_DATA;
	#else
	*buf = 0x3ff;
	ad_data[0] = REG_MISC_ADC_DATA;
	if ((ad_data[0] != ValIndex_back[AD_CH_0]))//||(adc_read->sadc_addr == AD_CH_0))
	{
		ValIndex_back[AD_CH_0] = ad_data[0];
		printf_info ("app_saradc_read ch[0] = 0x%x\n",ad_data[0]);
	}
	#endif
}

void hw_saradc_oneAdKey_read(U16 * buf, U8 ad_ch, U8 batt_val_det_en, U16 *batt_val)
{
	U32 val = 0x00;
    static U16 buf_bck = 0xfff;

	MISC_SADC_CONTROLs *adc_read = (MISC_SADC_CONTROLs *)&val;
    val = REG_MISC_ADC_CTL; 

    if (batt_val_det_en == TRUE)
    {
        if ((adc_read->sadc_addr != ad_ch) && (adc_read->sadc_addr != BATT_CH))
        {
            buf[0] = 0x3ff;
            *batt_val = 0xfff;
            return;
        }
    }
    else
    {
        if (adc_read->sadc_addr != ad_ch)
        {
            buf[0] = 0x3ff;
            return;
        }        
    }


	#ifndef AD_SCAN_DEBUG
    if (adc_read->sadc_addr == ad_ch)
    {
        buf[0] = REG_MISC_ADC_DATA;
        buf_bck = buf[0];
    }
    else
    {
        *batt_val = REG_MISC_ADC_DATA;

        buf[0] = buf_bck;
    }
	
	#else
	buf[0] = 0x3ff;
    *batt_val = 0xfff;
    if (adc_read->sadc_addr == ad_ch)
    {
        ad_data[ad_ch] = REG_MISC_ADC_DATA;
    }
    else
    {
        ad_data[BATT_CH] = REG_MISC_ADC_DATA;
    }
	if ((ad_data[ad_ch] != ValIndex_back[ad_ch])||(ad_data[BATT_CH] != ValIndex_back[BATT_CH]))
	{
		ValIndex_back[ad_ch]    = ad_data[ad_ch];
        ValIndex_back[BATT_CH]  = ad_data[BATT_CH];
        printf_info ("%s\n\r",__func__);
		printf_info ("ch[%d] = 0x%x\nch[BATT_CH] = 0x%x\n\r", 
                    ad_ch,      ad_data[ad_ch], 
                    ad_data[BATT_CH]);
	}
	#endif    

    if (adc_read->sadc_addr != ad_ch)
    {
        adc_read->sadc_addr = ad_ch;
    }
    else
    {
        if (batt_val_det_en == TRUE)
        {
            adc_read->sadc_addr = BATT_CH;
        }
    }

    REG_MISC_ADC_CTL = val;


}

void hw_saradc_twoAdKey_read(U16 * buf, U8 ad_ch_first, U8 ad_ch_second, U8 batt_val_det_en, U16 *batt_val)
{
	U32 val = 0x00;
    static U16 buf_bck[2] = {0xfff,0xfff};

	MISC_SADC_CONTROLs *adc_read = (MISC_SADC_CONTROLs *)&val;
    val = REG_MISC_ADC_CTL; 

    if (batt_val_det_en == TRUE)
    {
        if ((adc_read->sadc_addr != ad_ch_first) && (adc_read->sadc_addr != ad_ch_second) && (adc_read->sadc_addr != BATT_CH))
        {
            buf[0] = 0x3ff;
            buf[1] = 0x3ff;
            *batt_val = 0xfff;
            return;
        }
    }
    else
    {
        if ((adc_read->sadc_addr != ad_ch_first) && (adc_read->sadc_addr != ad_ch_second))
        {
            buf[0] = 0x3ff;
            buf[1] = 0x3ff;
            return;
        }        
    }

	#ifndef AD_SCAN_DEBUG
    if (adc_read->sadc_addr == ad_ch_first)
    {
        buf[0] = REG_MISC_ADC_DATA;
        buf_bck[0] = buf[0];

        buf[1] = buf_bck[1];
    }
    else if (adc_read->sadc_addr == ad_ch_second)
    {
        buf[1] = REG_MISC_ADC_DATA;
        buf_bck[1] = buf[1];

        buf[0] = buf_bck[0];
    }
    else
    {
        *batt_val = REG_MISC_ADC_DATA;

        buf[0] = buf_bck[0];
        buf[1] = buf_bck[1];
    }
	
	#else
	buf[0] = 0x3ff;
    buf[1] = 0x3ff;
    *batt_val = 0xfff;
    if (adc_read->sadc_addr == ad_ch_first)
    {
        ad_data[ad_ch_first] = REG_MISC_ADC_DATA;
    }
    else if (adc_read->sadc_addr == ad_ch_second)
    {
        ad_data[ad_ch_second] = REG_MISC_ADC_DATA;
    }    
    else
    {
        ad_data[BATT_CH] = REG_MISC_ADC_DATA;
    }
	if ((ad_data[ad_ch_first] != ValIndex_back[ad_ch_first])||(ad_data[ad_ch_second] != ValIndex_back[ad_ch_second])||(ad_data[BATT_CH] != ValIndex_back[BATT_CH]))
	{
		ValIndex_back[ad_ch_first]  = ad_data[ad_ch_first];
        ValIndex_back[ad_ch_second] = ad_data[ad_ch_second];
        ValIndex_back[BATT_CH]      = ad_data[BATT_CH];
        printf_info ("%s\n\r",__func__);
		printf_info ("ch[%d] = 0x%x\nch[%d] = 0x%x\nch[BATT_CH] = 0x%x\n\r", 
                    ad_ch_first,    ad_data[ad_ch_first], 
                    ad_ch_second,   ad_data[ad_ch_second], 
                    ad_data[BATT_CH]);
	}
	#endif   

	if (adc_read->sadc_addr == ad_ch_first)
	{
		adc_read->sadc_addr = ad_ch_second;
	}
	else if (adc_read->sadc_addr == ad_ch_second)
	{
		if (batt_val_det_en)
		{
			adc_read->sadc_addr = BATT_CH;
		}
		else
		{
			adc_read->sadc_addr = ad_ch_first;
		}
	}
	else if (adc_read->sadc_addr == BATT_CH)
	{
		adc_read->sadc_addr = ad_ch_first;
	}
	else
	{
		adc_read->sadc_addr = ad_ch_first;
	}

    REG_MISC_ADC_CTL = val;

}

void hw_saradc_threeAdKey_read(U16 * buf, U8 ad_ch_first, U8 ad_ch_second, U8 ad_ch_third, U8 batt_val_det_en, U16 *batt_val)
{
	U32 val = 0x00;
    static U16 buf_bck[3] = {0xfff,0xfff,0xfff};

	MISC_SADC_CONTROLs *adc_read = (MISC_SADC_CONTROLs *)&val;
    val = REG_MISC_ADC_CTL; 

    if (batt_val_det_en == TRUE)
    {
        if ((adc_read->sadc_addr != ad_ch_first) && (adc_read->sadc_addr != ad_ch_second) && (adc_read->sadc_addr != ad_ch_third) && (adc_read->sadc_addr != BATT_CH))
        {
            buf[0] = 0x3ff;
            buf[1] = 0x3ff;
            buf[2] = 0x3ff;
            *batt_val = 0xfff;
            return;
        }
    }
    else
    {
        if ((adc_read->sadc_addr != ad_ch_first) && (adc_read->sadc_addr != ad_ch_second) && (adc_read->sadc_addr != ad_ch_third))
        {
            buf[0] = 0x3ff;
            buf[1] = 0x3ff;
            buf[2] = 0x3ff;
            return;
        }        
    }

	#ifndef AD_SCAN_DEBUG
    if (adc_read->sadc_addr == ad_ch_first)
    {
        buf[0] = REG_MISC_ADC_DATA;
        buf_bck[0] = buf[0];

        buf[1] = buf_bck[1];
        buf[2] = buf_bck[2];
    }
    else if (adc_read->sadc_addr == ad_ch_second)
    {
        buf[1] = REG_MISC_ADC_DATA;
        buf_bck[1] = buf[1];

        buf[0] = buf_bck[0];
        buf[2] = buf_bck[2];        
    }
    else if (adc_read->sadc_addr == ad_ch_third)
    {
        buf[2] = REG_MISC_ADC_DATA;
        buf_bck[2] = buf[2];

        buf[0] = buf_bck[0];
        buf[1] = buf_bck[1];
    }
    else
    {
        *batt_val = REG_MISC_ADC_DATA;

        buf[0] = buf_bck[0];
        buf[1] = buf_bck[1];
        buf[2] = buf_bck[2];
    }
	
	#else
	buf[0] = 0x3ff;
    buf[1] = 0x3ff;
    buf[2] = 0x3ff;
    *batt_val = 0xfff;
    if (adc_read->sadc_addr == ad_ch_first)
    {
        ad_data[ad_ch_first] = REG_MISC_ADC_DATA;
    }
    else if (adc_read->sadc_addr == ad_ch_second)
    {
        ad_data[ad_ch_second] = REG_MISC_ADC_DATA;
    }    
    else if (adc_read->sadc_addr == ad_ch_third)
    {
        ad_data[ad_ch_third] = REG_MISC_ADC_DATA;
    }      
    else
    {
        ad_data[BATT_CH] = REG_MISC_ADC_DATA;
    }
	if ((ad_data[ad_ch_first] != ValIndex_back[ad_ch_first])||(ad_data[ad_ch_second] != ValIndex_back[ad_ch_second])||(ad_data[ad_ch_third] != ValIndex_back[ad_ch_third])||(ad_data[BATT_CH] != ValIndex_back[BATT_CH]))
	{
		ValIndex_back[ad_ch_first]  = ad_data[ad_ch_first];
        ValIndex_back[ad_ch_second] = ad_data[ad_ch_second];
        ValIndex_back[ad_ch_third]  = ad_data[ad_ch_third];
        ValIndex_back[BATT_CH]      = ad_data[BATT_CH];
        printf_info ("%s\n\r",__func__);
		printf_info ("ch[%d] = 0x%x\nch[%d] = 0x%x\nch[%d] = 0x%x\nch[BATT_CH] = 0x%x\n\r", 
                    ad_ch_first,    ad_data[ad_ch_first], 
                    ad_ch_second,   ad_data[ad_ch_second], 
                    ad_ch_third,    ad_data[ad_ch_third], 
                    ad_data[BATT_CH]);
	}
	#endif   

	if (adc_read->sadc_addr == ad_ch_first)
	{
		adc_read->sadc_addr = ad_ch_second;
	}
	else if (adc_read->sadc_addr == ad_ch_second)
	{
		adc_read->sadc_addr = ad_ch_third;
	}
	else if (adc_read->sadc_addr == ad_ch_third)
	{
		if (batt_val_det_en)
		{
			adc_read->sadc_addr = BATT_CH;
		}
		else
		{
			adc_read->sadc_addr = ad_ch_first;
		}
	}
	else if (adc_read->sadc_addr == BATT_CH)
	{
		adc_read->sadc_addr = ad_ch_first;
	}
	else
	{
		adc_read->sadc_addr = ad_ch_first;
	}

    REG_MISC_ADC_CTL = val;


}
#endif

