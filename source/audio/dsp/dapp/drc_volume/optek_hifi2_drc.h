/*
	Optek DRC
		Copyright by Optek in May 24, 2016
*/

#ifndef _OPTEK_HIFI2_DRC_H
#define _OPTEK_HIFI2_DRC_H

/*
	input->no input
	return->string of optek_drc version
*/
const char *optek_drc_get_version (void);

/*
	input no
	return->how many bytes needed for optek_drc
*/
int optek_drc_alloc(void);

void optek_drc_init (void);

/*
	handle->optek_drc
	other input, please referenced the function optek_drc_open() 
	return->no return
*/

void optek_drc_set (void *handle,  int attack_threshold, int release_threshold, int attack_rate, int release_rate,int lev_detect_release_time,
					uint8_t clipping_enable, uint8_t rms_enable, uint8_t update_samples);

/*
	objmem->mem allocated for optek_drc
	attack_threshold->DRC attack level
	release_threshold->DRC release level, should be < attack_threshold, 
		it is better = attack_threshold * down_rate (for example 1db) and it is better several times (for example 4 or 6) of attack_rate
	attack_rate->in attack phase, drc_vol = drc_vol * attack_rate, it should be < 0db
	release_rate->in release phase, drc_vol = drc_vol * release_rate, it should be > 0db
	lev_detect_release_time->level detect release time, it should be < 0db, 
		it depends on input signal frequency, for example 1kHz signal, 0.04db down is ok, for 100Hz, 0.004db down is ok 
	clipping_enable-> 0 means no clipping, 1 means clipping
	rms_enable->level detect based on peak or rms, should be set 0
	update_samples->DRC update time, about 1ms samples
	return->no return
*/
void * optek_drc_open (void * objmem, int attack_threshold, int release_threshold, int attack_rate, int release_rate, int lev_detect_release_time,
					 uint8_t clipping_enable, uint8_t rms_enable, uint8_t update_samples);


/*
	handle->optek_drc
	x->input/outpuf buf
	N->samples (L sampes + R samples for stereo)
	return void
*/
//MCPS: 2.97 Average: 3.00 Peak: 3.03
void optek_drc_stereo_24b_process (void *handle, int *x, int N) OPTEK_HIFI2_DSP_ATTIB;


#endif	//_OPTEK_HIFI2_DRC_H
