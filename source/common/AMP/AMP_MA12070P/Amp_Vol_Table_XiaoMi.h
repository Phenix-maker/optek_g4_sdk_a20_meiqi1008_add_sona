#ifndef _AMP_VOL_TABLE_XIAOMI_H_
#define _AMP_VOL_TABLE_XIAOMI_H_


extern const int amp_vol_table [];

extern const int xiaomi_tv_source_vol_table [];

typedef struct 
{
    int MainGainVol;
    int MainGainStepNum;
}Amp_Vol_Parm;

typedef struct _Amp_Vol_Cfg
{
    Amp_Vol_Parm *vol_table;
}Amp_Vol_Cfg;


extern Amp_Vol_Cfg amp_vol;


#endif /* _AMP_VOL_TABLE_H_ */
