#ifndef __PEAK_LEVEL_METER_H__
#define __PEAK_LEVEL_METER_H__


extern U16 peak_level_L;
extern U16 peak_level_R;

extern U16 peak_level;

typedef struct
{
	U32 peak_level;
	U32 temp_peak_level;
	U16 samples_cn;
	U16 peak_updata_samples;
} peak_level_det;


#define PEAK_UPDATA_SAMPLES_44P1k  	735
#define PEAK_UPDATA_SAMPLES_8K  	80


void peak_level_meter_init (U16 attack_t, U16 release_t);

void peak_level_meter_stereo (short *pBuf, unsigned short byte_len);
void peak_level_get (U32 *pLevel_L, U32 *pLevel_R);

void peak_level_meter_16bit_open (void *handle, U16 peak_updata_samples);

U16 peak_level_meter_16bit(void *handle,short *pBuf, unsigned short byte_len);
U32 peak_level_meter_24bit(void *handle,int *pBuf, unsigned short byte_len);


#endif //__PEAK_LEVEL_METER_H__
