#include "os_config.h"

#include "mem_reloc.h"

#include "peak_level_meter.h"

U16 peak_level_L;
U16 peak_level_R;
U16 attack_factor;
U16 release_factor;


void peak_level_meter_init (U16 attack_t, U16 release_t)
{
	peak_level_L = 0;
	peak_level_R = 0;
	
	attack_factor = attack_t;
	release_factor = release_t;
}


void peak_level_meter_stereo (short *pBuf, unsigned short byte_len)
{
	short *pBufL, *pBufR;
	U16 tmp,i;
	U16 tmp_L,tmp_R;
	//Left peak meter
	
	pBufL = &pBuf[0];
	pBufR = &pBuf[1];

		
	for (i=0; i<byte_len/4; i++) 
	{
		tmp = (U16) abs (*pBufL);
		tmp_L = ((peak_level_L * attack_factor) >> 16);

		if (tmp > peak_level_L)
		{
			peak_level_L = tmp;
		}
		else
		{
			peak_level_L = tmp_L;
		}

		tmp = (U16) abs (*pBufR);
		tmp_R = ((peak_level_R * attack_factor) >> 16);

		if (tmp > peak_level_R)
		{
			peak_level_R = tmp;
		}
		else
		{
			peak_level_R = tmp_R;
		}

		pBufL += 2;
		pBufR += 2;
	}
}

void peak_level_meter_16bit_open (void *handle, U16 peak_updata_samples)
{
	peak_level_det *pObj = handle;
	pObj->peak_level = 0;
	pObj->temp_peak_level = 0;
	pObj->samples_cn= 0;
	pObj->peak_updata_samples = peak_updata_samples;
}


U16 peak_level_meter_16bit(void *handle,short *pBuf, unsigned short byte_len)
{
	peak_level_det *pObj = handle;
	U16 i;
	U16 tmp;
	byte_len >>= 1;
	for (i=0; i<byte_len; i++) 
	{
		tmp = abs(*pBuf);
		if (tmp > pObj->temp_peak_level)
		{
			pObj->temp_peak_level = tmp;
		}
		pBuf++;
	}

	pObj->samples_cn += byte_len>>1;

	if ((pObj->samples_cn > pObj->peak_updata_samples)||(pObj->peak_level < pObj->temp_peak_level))
	{
		pObj->peak_level = pObj->temp_peak_level;
		pObj->temp_peak_level >>= 1;
		pObj->samples_cn = 0;
	}

	return (U16)pObj->peak_level;
}

U32 peak_level_meter_24bit(void *handle,int *pBuf, unsigned short byte_len)
{
	peak_level_det *pObj = handle;
	U16 i;
	U32 tmp;

	byte_len >>= 2;
	for (i=0; i<byte_len; i++) 
	{
		tmp = abs(*pBuf);
		if (tmp > pObj->temp_peak_level)
		{
			pObj->temp_peak_level = tmp;
		}
		pBuf++;
	}

	pObj->samples_cn += byte_len>>1;

	if ((pObj->samples_cn > pObj->peak_updata_samples)||(pObj->peak_level < pObj->temp_peak_level))
	{
		pObj->peak_level = pObj->temp_peak_level;
		pObj->temp_peak_level >>= 1;
		pObj->samples_cn = 0;
	}

	return pObj->peak_level;
}


void peak_level_get (U32 *pLevel_L, U32 *pLevel_R)
{
	*pLevel_L = peak_level_L;
	*pLevel_R = peak_level_R;	
}



