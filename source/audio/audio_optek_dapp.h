#ifndef __AUDIO_OPTEK_DAPP_H__
#define __AUDIO_OPTEK_DAPP_H__


void audio_optek_dapp_init(void);
void audio_optek_dapp_open(void);
void audio_optek_dapp_set_channel(U8 channel);
void audio_optek_dapp_set_volume(U8 volume);
void audio_optek_dapp_set_speaker(U8 speaker, U8 attenuation);
void audio_optek_dapp_set_bass_treble(U8 bass, U8 treble);
void audio_optek_dapp_set_eq_mode(U8 mode);
void audio_optek_dapp_set_mute(U8 con);
void audio_optek_dapp_set_AM_mode(U8 con, U32 kFreq);


#define AUDIO_DEV_OPTEK_DAPP      	audio_optek_dapp_init, \
									audio_optek_dapp_open,	\
									audio_optek_dapp_set_channel, \
									audio_optek_dapp_set_volume, \
									audio_optek_dapp_set_bass_treble, \
									audio_optek_dapp_set_eq_mode, \
									audio_optek_dapp_set_mute, \
									audio_optek_dapp_set_AM_mode  //,\
									//audio_optek_dapp_set_speaker

#endif /* __AUDIO_OPTEK_DAPP_H__ */
