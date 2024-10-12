#include "c_def.h"
#include "debug.h"
#include "oem.h"

#include "os_config.h"

#include "hw_timer.h"

#include "tools.h"

#ifdef DSP_HIFI_LIB_OLD
#include "optek_hifi2_dapp.h"
#else
#include "optek_hifi2_dapp_strided.h"
#endif
#include "app_media.h"

#define INVALID_CHAR	('#')

/**
 * FUNCTIONs RELATE TO UP/DOWN SAMPLE
 */

//for example sample: sarmple rate 192kHz to 48kHz
void audio_data_down_sample_from_2x_to_1x (U32 *pBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;
	
	pDataBuf = pBuf;
	
	for (i=0; i<(byte_len/4)/2; i++)
	{
		*pDataBuf++ = *pBuf;
		pBuf += 2;
	}
}

//for example sample: sarmple rate 96kHz to 48kHz
void audio_data_down_sample_from_4x_to_1x (U32 *pBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;
	
	pDataBuf = pBuf;
	
	for (i=0; i<(byte_len/4)/4; i++)
	{
		*pDataBuf++ = *pBuf;
		pBuf += 4;
	}
}



/**
 * FUNCTIONs RELATE TO CHANNEL DATA SWITCH
 */

//for expanding from 2-channel to 4-channel
void audio_data_process_2ch_in_to_4ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	#ifdef AUDIO_OPTEK_DAPP
	optek_dapp_16b_in_process ((short *)pSrcBuf, 2, byte_len/2);
	#endif

	pDataBuf = pDestBuf;

	for (i=0; i<(byte_len/4); i++)
	{
		#if 0
		*pDataBuf++ = *pSrcBuf;
		*pDataBuf++ = *pSrcBuf++;		
		#else
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		#endif
	}
}

//for expanding from 2 2-channel to 4-channel
void audio_data_process_2_2ch_in_to_4ch_out (int *pDestBuf, int *pSrcBuf_sub, int *pSrcBuf_LR, U32 byte_len)
{
	int *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	if(PHASE_KEY_STATUS)
	{
		for (i=0; i<(byte_len/4); i++)
		{
			pDataBuf[i*4+0] = pSrcBuf_sub[i*2+0];
			pDataBuf[i*4+1] = pSrcBuf_sub[i*2+1];
			pDataBuf[i*4+2] = pSrcBuf_LR[i*2+1];
			pDataBuf[i*4+3] = pSrcBuf_LR[i*2+0];
		}
	}
	else
	{
		for (i=0; i<(byte_len/4); i++)
		{
			pDataBuf[i*4+0] = -(pSrcBuf_sub[i*2+0]);
			pDataBuf[i*4+1] = -(pSrcBuf_sub[i*2+1]);
			pDataBuf[i*4+2] = pSrcBuf_LR[i*2+1];
			pDataBuf[i*4+3] = pSrcBuf_LR[i*2+0];
		}	
	}
}

//for expanding from 2-channel to 6-channel
void audio_data_process_2ch_in_to_6ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	#ifdef AUDIO_OPTEK_DAPP
	optek_dapp_16b_in_process ((short *)pSrcBuf, 2, byte_len/2);
	#endif

	pDataBuf = pDestBuf;

	for (i=0; i<(byte_len/4/2); i++)//24bit data
	{
		#if 0
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		#else
		pDataBuf[i*6+0] = pSrcBuf[i*2+0];
		pDataBuf[i*6+1] = pSrcBuf[i*2+1];
		pDataBuf[i*6+2] = pSrcBuf[i*2+0];
		pDataBuf[i*6+3] = pSrcBuf[i*2+1];
		pDataBuf[i*6+4] = pSrcBuf[i*2+0];
		pDataBuf[i*6+5] = pSrcBuf[i*2+1];
		#endif

	}
}

//for expanding from 2-channel to 8-channel
void audio_data_process_2ch_in_to_8ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	#ifdef AUDIO_OPTEK_DAPP
	optek_dapp_16b_in_process ((short *)pSrcBuf, 2, byte_len/2);
	#endif

	pDataBuf = pDestBuf;

	for (i=0; i<(byte_len/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 2-channel to 10-channel
void audio_data_process_2ch_in_to_10ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	#ifdef AUDIO_OPTEK_DAPP
	optek_dapp_16b_in_process ((short *)pSrcBuf, 2, byte_len/2);
	#endif

	pDataBuf = pDestBuf;

	for (i=0; i<(byte_len/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;		
	}
}

//for expanding from 2-channel to 12-channel
void audio_data_process_2ch_in_to_12ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	#ifdef AUDIO_OPTEK_DAPP
	optek_dapp_16b_in_process ((short *)pSrcBuf, 2, byte_len/2);
	#endif

	pDataBuf = pDestBuf;

	for (i=0; i<(byte_len/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;		
		*pDataBuf++ = 0;		
	}
}

//for expanding from 2-channel to 14-channel
void audio_data_process_2ch_in_to_14ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	#ifdef AUDIO_OPTEK_DAPP
	optek_dapp_16b_in_process ((short *)pSrcBuf, 2, byte_len/2);
	#endif

	pDataBuf = pDestBuf;

	for (i=0; i<(byte_len/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;		
		*pDataBuf++ = 0;		
		*pDataBuf++ = 0;		
	}
}

//for expanding from 2-channel to 16-channel
void audio_data_process_2ch_in_to_16ch_out (U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	#ifdef AUDIO_OPTEK_DAPP
	optek_dapp_16b_in_process ((short *)pSrcBuf, 2, byte_len/2);
	#endif

	pDataBuf = pDestBuf;

	for (i=0; i<(byte_len/4); i++)
	{
		#if 1
		*pDataBuf++ = *pSrcBuf;
		*pDataBuf++ = *pSrcBuf;
		*pDataBuf++ = *pSrcBuf;
		*pDataBuf++ = *pSrcBuf;
		*pDataBuf++ = *pSrcBuf;		
		*pDataBuf++ = *pSrcBuf;		
		*pDataBuf++ = *pSrcBuf;		
		*pDataBuf++ = *pSrcBuf++;
		#else
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;		
		*pDataBuf++ = 0;		
		*pDataBuf++ = 0;		
		*pDataBuf++ = 0;
		#endif
	}
}

//for expanding from 4-channel to 6-channel
void audio_data_process_4ch_in_to_2ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/2); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		pSrcBuf++;
	}
}

//for expanding from 4-channel to 6-channel
void audio_data_process_4ch_in_to_6ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/2); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
	}
}

//for expanding from 4-channel to 8-channel
void audio_data_process_4ch_in_to_8ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/2); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 4-channel to 10-channel
void audio_data_process_4ch_in_to_10ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/2); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 4-channel to 12-channel
void audio_data_process_4ch_in_to_12ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/2); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 4-channel to 14-channel
void audio_data_process_4ch_in_to_14ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/2); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 4-channel to 16-channel
void audio_data_process_4ch_in_to_16ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/2); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}


//for expanding from 4-channel to 6-channel
void audio_data_process_6ch_in_to_2ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/3); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		pSrcBuf++;
		pSrcBuf++;
	}
}

//for expanding from 6-channel to 4-channel
void audio_data_process_6ch_in_to_4ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/3); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		pSrcBuf++;
	}
}

//for expanding from 6-channel to 8-channel
void audio_data_process_6ch_in_to_8ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/3); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
	}
}

//for expanding from 6-channel to 10-channel
void audio_data_process_6ch_in_to_10ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/3); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 6-channel to 12-channel
void audio_data_process_6ch_in_to_12ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/3); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 6-channel to 14-channel
void audio_data_process_6ch_in_to_14ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/3); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 6-channel to 16-channel
void audio_data_process_6ch_in_to_16ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/3); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}



//for expanding from 8-channel to 2-channel
void audio_data_process_8ch_in_to_2ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;//ch0 ch1
					pSrcBuf++;//ch2 ch3
					pSrcBuf++;//ch4 ch5
					pSrcBuf++;//ch6 ch7
	}
}

//for expanding from 8-channel to 4-channel
void audio_data_process_8ch_in_to_4ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		pSrcBuf++;
		pSrcBuf++;
	}
}

//for expanding from 8-channel to 6-channel
void audio_data_process_8ch_in_to_6ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		pSrcBuf++;
	}
}

//for expanding from 8-channel to 10-channel
void audio_data_process_8ch_in_to_10ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
	}
}

//for expanding from 8-channel to 12-channel
void audio_data_process_8ch_in_to_12ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 8-channel to 14-channel
void audio_data_process_8ch_in_to_14ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

//for expanding from 8-channel to 16-channel
void audio_data_process_8ch_in_to_16ch_out(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	U32 *pDataBuf;
	U16 i;

	pDataBuf = pDestBuf;
	
	for (i=0; i<(byte_len/4/4); i++)
	{
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = *pSrcBuf++;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
		*pDataBuf++ = 0;
	}
}

#ifdef I2S_TDM_DATA_WIDTH_24_BIT
void audio_data_process_24bit_16ch_in_to_16ch_out_for_TDM(U32 *pDestBuf, U32 *pSrcBuf, U32 byte_len)
{
	int i = 0;
    for (i=0;i<byte_len/4/16;i++)
    {

        *pDestBuf++ = *(pSrcBuf+0);
        *pDestBuf++ = *(pSrcBuf+1);
        *pDestBuf++ = *(pSrcBuf+4);
        *pDestBuf++ = *(pSrcBuf+5);
        *pDestBuf++ = *(pSrcBuf+8);
        *pDestBuf++ = *(pSrcBuf+9);
        *pDestBuf++ = *(pSrcBuf+12);
        *pDestBuf++ = *(pSrcBuf+13);

        *pDestBuf++ = *(pSrcBuf+2);
        *pDestBuf++ = *(pSrcBuf+3);
        *pDestBuf++ = *(pSrcBuf+6);
        *pDestBuf++ = *(pSrcBuf+7);
        *pDestBuf++ = *(pSrcBuf+10);
        *pDestBuf++ = *(pSrcBuf+11);
        *pDestBuf++ = *(pSrcBuf+14);
        *pDestBuf++ = *(pSrcBuf+15);

    }
}
#endif //I2S_TDM_DATA_WIDTH_24_BIT 



/**
 * OTHER FUNCTIONs
 */

#define Itoa_Max    11

char aString[Itoa_Max];

//convert intgers(decimal) to strings
void itoa_dec( int val, char *string )
{
	unsigned char *p;
	char *str1= "0123456789";

	p = aString + Itoa_Max - 1;
	*p = 0;

	do
	{
		*--p = str1[val % 10];
		val = val/10;
	} while( val );

	while (*p != 0)
	{
		*string++ = *p++;
	}

	*string = 0;
}

//convert string to interger(hexadecimal)
int atoi_hex( char *string, long *val)
{
	char i;
	char *str1 = "0123456789AaBbCcDdEeFf";
	char temp;
	char temp1;
	char size;
	char *p = string;

	size = strlen(str1);

	if (string == NULL)
	{
		return 0;
	}

	if (*string == NULL)
	{
		return 0;
	}

	*val = 0;

	do
	{
		temp = *p++;

		for (i=0; i<size; i++)
		{
			if  (temp == str1[i])
			{
				if ( (temp >= '0') && (temp <= '9') )
				{
					temp1 = temp - '0';
				}
				else if ( (temp >= 'A') && (temp <= 'F') )
				{
					temp1 = temp - 'A' + 10;
				}
				else if ( (temp >= 'a') && (temp <= 'f') )
				{
					temp1 = temp - 'a' + 10;
				}

				*val <<= 4;
				*val |= temp1;
			}
		}

    } while( *p );

	return 1;
}


//convert string to interger(decimal)
int atoi_dec( char *string, long *val)
{
	char i;
	char *str1 = "0123456789AaBbCcDdEeFf";
	char temp;
	long temp1;
	char size;
	char string_len;
	char len;
	char *p = string;
	
	size = strlen(str1);
	string_len = strlen(string);

	if (string == NULL)
	{
		return 0;
	}
	
	if (*string == NULL)
	{
		return 0;
	}
	
	*val = 0;
	
	do
	{
		temp = *p++;
		len = string_len;
		
		for (i=0; i<size; i++)
		{
			if  (temp == str1[i])
			{
				if ( (temp >= '0') && (temp <= '9') )
				{
					temp1 = temp - '0';
				}
				else if ( (temp >= 'A') && (temp <= 'F') )
				{
					temp1 = temp - 'A' + 10;
				}
				else if ( (temp >= 'a') && (temp <= 'f') )
				{
					temp1 = temp - 'a' + 10;
				}

				if (string_len == 10)
				{
					temp1 *= 1000000000;
					*val += temp1;
				}
				if (string_len == 9)
				{
					temp1 *= 100000000;
					*val += temp1;
				}
				else if (string_len == 8)
				{
					temp1 *= 10000000;
					*val += temp1;
				}
				else if (string_len == 7)
				{
					temp1 *= 1000000;
					*val += temp1;
				}
				else if (string_len == 6)
				{
					temp1 *= 100000;
					*val += temp1;
				}
				else if (string_len == 5)
				{
					temp1 *= 10000;
					*val += temp1;
				}
				else if (string_len == 4)
				{
					temp1 *= 1000;
					*val += temp1;
				}
				else if (string_len == 3)
				{
					temp1 *= 100;
					*val += temp1;
				}
				else if (string_len == 2)
				{
					temp1 *= 10;
					*val += temp1;
				}
				else if (string_len == 1)
				{
					temp1 *= 1;
					*val += temp1;
				}

				if (string_len > 0)
				{
					string_len--;
				}
			}
		}
		
    } while( *p );
	
	return 1;
}

void  cpu_clock_frequency_check_out(U32 interval_time_ms)
{
    static U32 last_tick = 0;
    static U32 new_tick = 0;
    U32 temp_tick;
	U32 freq;
      
    new_tick = read_ccount();
    if (new_tick < last_tick)
    {
         temp_tick = 0x100000000 + new_tick - last_tick;
    }
	else
    {
		temp_tick = new_tick - last_tick;
    }

	freq = temp_tick/(interval_time_ms*1000);
    last_tick = new_tick;

	DBG_Printf("cpu clock %d Mhz\n", freq);
}

//the output channel number is fixed
int audio_out_data_channel_switch(U32 *pSrc, U32 *pDest, U32 in_sample_speed, U32 in_channel, 
                           U32 in_len, U32 *out_byte_len)
{
	U32 len = in_len;
	U32 out_len;

	if (in_sample_speed == SAMPLE_SPEED_4X)
	{
		audio_data_down_sample_from_4x_to_1x (pSrc, len);
		len = len/4;
	}
	else if (in_sample_speed == SAMPLE_SPEED_2X)
	{
		// audio_data_down_sample_from_2x_to_1x (pSrc, len);
		// len = len/2;
	}
	else
	{
		asm("nop");
	}

	if (AUDIO_OUT_STREAM_CHANNEL == 16)
	{
		switch (in_channel)
		{
			case 2:
			audio_data_process_2ch_in_to_16ch_out(pDest, pSrc, len);
			out_len = len*8;
			break;

			case 4:
			#ifdef I2S_TDM_DATA_WIDTH_24_BIT
			audio_data_process_24bit_16ch_in_to_16ch_out_for_TDM(pDest, pSrc, len);
			out_len = len;
			#else
			audio_data_process_4ch_in_to_16ch_out(pDest, pSrc, len);
			out_len = (len/2)*8;			
			#endif
			break;

			case 6:
			audio_data_process_6ch_in_to_16ch_out(pDest, pSrc, len);
			out_len = (len/3)*8;
			break;

			case 8:
			audio_data_process_8ch_in_to_16ch_out(pDest, pSrc, len);
			out_len = (len/4)*8;
			break;

			default:
			DBG_Assert(FALSE);
			break;
		}
	}
	else if (AUDIO_OUT_STREAM_CHANNEL == 14)
	{
		switch (in_channel)
		{
			case 2:
			audio_data_process_2ch_in_to_14ch_out(pDest, pSrc, len);
			out_len = len*7;
			break;

			case 4:
			audio_data_process_4ch_in_to_14ch_out(pDest, pSrc, len);
			out_len = (len/2)*7;
			break;

			case 6:
			audio_data_process_6ch_in_to_14ch_out(pDest, pSrc, len);
			out_len = (len/3)*7;
			break;

			case 8:
			audio_data_process_8ch_in_to_14ch_out(pDest, pSrc, len);
			out_len = (len/4)*7;
			break;

			default:
			DBG_Assert(FALSE);
			break;
		}
	}
	else if (AUDIO_OUT_STREAM_CHANNEL == 12)
	{
		switch (in_channel)
		{
			case 2:
			audio_data_process_2ch_in_to_12ch_out(pDest, pSrc, len);
			out_len = len*6;
			break;

			case 4:
			audio_data_process_4ch_in_to_12ch_out(pDest, pSrc, len);
			out_len = (len/2)*6;
			break;

			case 6:
			audio_data_process_6ch_in_to_12ch_out(pDest, pSrc, len);
			out_len = (len/3)*6;
			break;

			case 8:
			audio_data_process_8ch_in_to_12ch_out(pDest, pSrc, len);
			out_len = (len/4)*6;
			break;

			default:
			DBG_Assert(FALSE);
			break;
		}
	}
	else if (AUDIO_OUT_STREAM_CHANNEL == 10)
	{
		switch (in_channel)
		{
			case 2:
			audio_data_process_2ch_in_to_10ch_out(pDest, pSrc, len);
			out_len = len*5;
			break;

			case 4:
			audio_data_process_4ch_in_to_10ch_out(pDest, pSrc, len);
			out_len = (len/2)*5;
			break;

			case 6:
			audio_data_process_6ch_in_to_10ch_out(pDest, pSrc, len);
			out_len = (len/3)*5;
			break;

			case 8:
			audio_data_process_8ch_in_to_10ch_out(pDest, pSrc, len);
			out_len = (len/4)*5;
			break;

			default:
			DBG_Assert(FALSE);
			break;
		}
	}	
	else if (AUDIO_OUT_STREAM_CHANNEL == 8)
	{
		switch (in_channel)
		{
			case 2:
			audio_data_process_2ch_in_to_8ch_out(pDest, pSrc, len);
			out_len = len*4;
			break;

			case 4:
			audio_data_process_4ch_in_to_8ch_out(pDest, pSrc, len);
			out_len = (len/2)*4;
			break;

			case 6:
			audio_data_process_6ch_in_to_8ch_out(pDest, pSrc, len);
			out_len = (len/3)*4;
			break;

			case 8:
			memcpy((U8 *)pDest, (U8 *)pSrc, len);
			out_len = len;
			break;

			default:
			DBG_Assert(FALSE);
			break;
		}
	}
	else if (AUDIO_OUT_STREAM_CHANNEL == 6)
	{
		switch (in_channel)
		{
			case 2:
			audio_data_process_2ch_in_to_6ch_out(pDest, pSrc, len);
			out_len = len*3;
			break;

			case 4:
			audio_data_process_4ch_in_to_6ch_out(pDest, pSrc, len);
			out_len = (len/2)*3;
			break;

			case 6:
			memcpy((U8 *)pDest, (U8 *)pSrc, len);
			out_len = len;
			break;

			case 8:
			audio_data_process_8ch_in_to_6ch_out(pDest, pSrc, len);
			out_len = (len/4)*3;
			break;

			default:
			DBG_Assert(FALSE);
			break;
		}
	}
	else if (AUDIO_OUT_STREAM_CHANNEL == 4)
	{
		switch (in_channel)
		{
			case 2:
			audio_data_process_2ch_in_to_4ch_out(pDest, pSrc, len);
			out_len = len*2;
			break;

			case 4:
			memcpy((U8 *)pDest, (U8 *)pSrc, len);
			out_len = len;
			break;

			case 6:
			audio_data_process_6ch_in_to_4ch_out(pDest, pSrc, len);
			out_len = (len/3)*2;
			break;

			case 8:
			audio_data_process_8ch_in_to_4ch_out(pDest, pSrc, len);
			out_len = (len/4)*2;
			break;

			default:
			DBG_Assert(FALSE);
			break;
		}
	}
	else if (AUDIO_OUT_STREAM_CHANNEL == 2)
	{
		switch (in_channel)
		{
			case 2:
			#ifdef AUDIO_OPTEK_DAPP
			//optek_dapp_16b_in_process ((short *)pSrc, 2, len/2);
			#endif
			memcpy((U8 *)pDest, (U8 *)pSrc, len);
			out_len = len;
			break;

			case 4:
			audio_data_process_4ch_in_to_2ch_out(pDest, pSrc, len);
			out_len = (len/2)*1;
			break;

			case 6:
			audio_data_process_6ch_in_to_2ch_out(pDest, pSrc, len);
			out_len = (len/3)*1;
			break;

			case 8:
			audio_data_process_8ch_in_to_2ch_out(pDest, pSrc, len);
			out_len = (len/4)*1;
			break;

			default:
			DBG_Assert(FALSE);
			break;
		}
	}
	else
	{
		DBG_Assert(FALSE);
	}

	*out_byte_len = out_len;

	return TRUE;
}




/************************************************************/
/*					RANDOM									*/
/************************************************************/

U16 random_seed;
U16 random_base;

/*(10,3,0)*/
void random_set_seed (U16 seed)
{
	if (seed == 0) {
		seed = 0xAA;
	}

	random_seed = seed & 0x3FF;
}

void random_set_base (U16 base)
{
	if (base == 0) {
		base = 0x55;
	}

	random_base = base & 0x3FF;
}

U16 random (void)
{
	BOOL LFSR_bit;
	
	if (random_base > 0x3FF) {
		
		U16 tmp;
	    
	    tmp = (random_base >> 10);
	    
		random_base &= 0x3FF;     
		
		random_base |= tmp;
	}
	
	if (random_seed == 0) {
		random_seed = 0x45;
	}

	LFSR_bit = random_seed & 0x01;	/*bit 10*/
	LFSR_bit ^= (random_seed & 0x80) >> 7;	/*bit 3*/

	random_seed >>= 1;

	if (LFSR_bit == 1) {
		random_seed |= 0x200;
	}

	return ((random_seed ^ random_base) & 0x3FF);
}

U16 random_track (U16 max_track)
{
	U16 random_track;
	
	while (1)
	{
		random_track = random ();
		if (random_track <= max_track && random_track != 0) {
			return (random_track-1);
		}
	}
}

void random_start (U16 s)
{
	random_base = random_seed ^ s;
}


//convert intgers(decimal) to strings
void itoa( U16 val, U8 *string )
{
	unsigned char *p;
	char *str1= "0123456789";

	p = aString + Itoa_Max - 1;
	*p = 0;

	do
	{
		*--p = str1[val % 10];
		val = val/10;
	} while( val );

	while (*p != 0)
	{
		*string++ = *p++;
	}
}

void oem_to_unicode (U8 *oem_buf, U16 *unicode_buf)
{
	U16 tmp;

	while (1)
	{
		if (*oem_buf == 0x0)
		{
			break;
		}
		else if (*oem_buf < 0x80)
		{
			*unicode_buf++ = (U16) *oem_buf++;
		}
		else
		{
			if ( *(oem_buf+1) >= 0x80)
			{
				tmp = ((*oem_buf++) << 8);
				tmp |= *oem_buf++;
				*unicode_buf++ = tmp;
			}
			else
			{
				*unicode_buf++ = (U16) *oem_buf++;
			}
		}
	}

	*unicode_buf = 0;
}


void unicode_to_English_oem( U16 *lfn_name, U16 n, U8 *oem_name)
{
	U16 i;

	for (i=0; i<n; i++)
	{	
		if (*lfn_name == 0)
		{
			break;
		}

#ifdef LCD_DISP_SUPPOERT_GERMANY
		if (*lfn_name >= 0xFF)
		{
			
#if defined JP_SR80I
			*oem_name++ = (U8) '*';
#elif defined SHARP_HF108P
			*oem_name++ = (U8) '#';
#else
			*oem_name++ = (U8) '*';
#endif
			lfn_name++;
		}
		else
		{
			*oem_name++ = (U8) *lfn_name++;
		}

#else
		if (*lfn_name >= 0x80)
		{
#if defined JP_SR80I
			*oem_name++ = (U8) '*';
#elif defined BLUESEA_SDK
			*oem_name++ = (U8) '#';
#else
			*oem_name++ = (U8) '*';
#endif
			lfn_name++;
		}
		else
		{
			*oem_name++ = (U8) *lfn_name++;
		}
#endif //LCD_DISP_SUPPOERT_GERMANY
	}

	//unicode null
	*oem_name++ = 0x0;
}

void unicode_to_English_oem_n( U16 *lfn_name, U8 *oem_name, U16 n)
{
	U16 num;

	num = 0;

	while (1)
	{
		if (*lfn_name == 0)
		{
			break;
		}

#ifdef LCD_DISP_SUPPOERT_GERMANY
		if (*lfn_name >= 0xFF)
		{
#if defined JP_SR80I
			*oem_name++ = (U8) '*';
#elif defined SHARP_HF108P
			*oem_name++ = (U8) '#';
#else
			*oem_name++ = (U8) '*';
#endif
			
			lfn_name++;
		}
		else
		{
			*oem_name++ = (U8) *lfn_name++;
		}
		
		num++;
		
		if (num >= n)
		{
			break;
		}

#else
		if (*lfn_name >= 0x80)
		{
#if defined JP_SR80I
			*oem_name++ = (U8) '*';
#elif defined BLUESEA_SDK
			*oem_name++ = (U8) '#';
#else
			*oem_name++ = (U8) '*';
#endif

			lfn_name++;
		}
		else
		{
			*oem_name++ = (U8) *lfn_name++;
		}
		
		num++;

		if (num >= n)
		{
			break;
		}
	
#endif //LCD_DISP_SUPPOERT_GERMANY
	}
	
	//unicode null
	*oem_name++ = 0x0;
}


void oem_to_n_unicode (U8 *oem_buf, U16 *unicode_buf, U16 n)
{
	U16 tmp;
	U16 num = 0;

	while (1)
	{
		if (*oem_buf == 0x0)
		{
			break;
		}

		if (num >= n)
		{
			break;
		}

		if (*oem_buf < 0x80)
		{
			*unicode_buf++ = (U16) *oem_buf++;
		}
		else
		{
			if ( *(oem_buf+1) >= 0x80)
			{
				tmp = ((*oem_buf++) << 8);
				tmp |= *oem_buf++;
				*unicode_buf++ = tmp;
			}
			else
			{
				*unicode_buf++ = (U16) *oem_buf++;
			}
		}
		num++;
	}

	*unicode_buf = 0;
}

//#define JAPAN_KATAKATA
U16 unicode_to_oem_n (U16 *l_name, U8 *s_name, U16 n)
{
	U16 i, num;

	num = 0;

	s_name[0] =0x0;

	for (i=0;i<n;i++)
	{
		if (*l_name == 0)
		{
			break;
		}

		if (*l_name >= 0xFF) {
			
			*s_name = (U8) INVALID_CHAR;

#ifdef KATAKATA
			for (i=0;i<KATAKAHA_NUM;i++)
			{
				if (*l_name == JPLCD_OEM_UNICODE[i][1])
				{
					*s_name = JPLCD_OEM_UNICODE[i][0];					
				}
			}
#endif
	
			s_name++;
			l_name++;
		}
		else {
			*s_name++ = (U8) *l_name++;
		}	
	}

	*s_name++ = 0x0;

	return 0;
}


//#define UNICODE_TO_OEM

U16 unicode_char_to_oem_char (U16 uchar)
{
	return uchar;
}

void _unicode_to_oem (U16 *buf, U8 *d)
{
#ifdef UNICODE_TO_OEM
	U16 len = 0;

	while ( *buf != 0x0 )
	{
		#if 0
		if ( (*buf & 0xFF00) != 0x0 )
		{
			*d++ = (U8) (*buf >> 8);
			len++;
		}

		if ( (*buf & 0x00FF) != 0x0 )
		{
			*d++ = (U8) (*buf & 0x00FF);
			len++;
		}

		buf++;
		#endif

		if (*buf >= 0x100)
		{
			#if defined JP_SR80I
			*d++ = (U8) '*';
			#elif defined BLUESEA_SDK
			*d++ = (U8) '#';
			#else
			*d++ = (U8) '*';
			#endif
			buf++;
		}
		else
		{
			*d++ = (U8) *buf++;
		}
	}

	/*Please note, it will be need that unicode null*/
	*d++ = 0x0;
	*d++ = 0x0;

	if (len & 0x01)
	{
		*d++ = 0;
	}
#endif
}

WORD w_Little_Big_Endian (void * p)
{
	WORD tmp;
	BYTE *pByte = p;

	tmp =  (*pByte)  & 0xff;
	tmp |= ( *(pByte + 1) << 8 ) & 0xff00;
	return tmp;
}

DWORD dw_Little_Big_Endian (void *p)
{
	DWORD tmp;
	BYTE *pByte = p;

	tmp =  (*pByte) & 0x000000ff;
	tmp |= ( ( *(pByte + 1) ) << 8 ) & 0x0000ff00;
	tmp |= ( ( *(pByte + 2) ) << 16 ) & 0x00ff0000;
	tmp |= ( *(pByte + 3) << 24 ) & 0xff000000;
	return tmp;
}

U32 U32_endian_buf(U8 *buf)
{
#ifdef LITTLE_ENDIAN
	return (U32)((U32)(buf[0]<<24)|(U32)(buf[1]<<16)|(U32)(buf[2]<<8)|(U32)(buf[3]<<0));
#else
	return (U32)((U32)(buf[3]<<24)|(U32)(buf[2]<<16)|(U32)(buf[1]<<8)|(U32)(buf[0]<<0));
#endif
}

U16 U16_endian_buf(U8 *buf)
{
#ifdef LITTLE_ENDIAN
	return (U16)( (U16)(buf[0 ] << 8) | (U16)(buf[1] << 0) );
#else
	return (U16)( (U16)(buf[1] << 8) | (U16)(buf[0] << 0) );
#endif
}

void endian_convert_u16_from_big_to_little(U16 *buf, U16 len)
{
	U16 i;
	U16 *pBuf = buf;

	for (i=0; i<len; i++)
	{
		*pBuf = ( (*pBuf >> 8) | (*pBuf << 8) );
		pBuf++;
	}
}

U8 data_swtich_from_msb_to_lsb(U8 *pData, U8 byte_number)
{
	U8 bit_mask = 0x80;
	U8 tmp = 0;
	U8 i;
	U8 j;
	U8 data;
	U8 *pBuf = pData;

	for (j=0; j<byte_number; j++)
	{
		bit_mask = 0x80;
		data = *pData++;

		for (i=0; i<8; i++)
		{
			if (data & bit_mask)
			{
				tmp = (tmp >> 1) | bit_mask;
			}
			else
			{
				tmp >>= 1;
			}
			
			data <<= 1;
		}

		*pBuf++ = tmp;
	}
	
	return TRUE;
}
