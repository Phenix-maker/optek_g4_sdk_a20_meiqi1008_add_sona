#ifndef __APP_LC3_ENCODE_H__
#define __APP_LC3_ENCODE_H__


BOOL app_mp3_encode_init (void);
BOOL app_ad_pcm_lc3_encode_frame (I16 *ppBuf, I32 len) __INTERNAL_RAM_TEXT;

extern U16 lc3_enc_header[9];
	
#endif //__APP_LC3_ENCODE_H__

