/*
	Optek EBASS
		Copyright by Optek in May 24, 2016
*/
//#include "optek_vol_table.h"

#ifndef _OPTEK_HIFI2_EBASS_H
#define _OPTEK_HIFI2_EBASS_H


/*
	input->no input
	return->string of optek_ebass build date,time and version
*/
const char *optek_ebass_data_get(void);
const char *optek_ebass_time_get(void);
const char *optek_ebass_version_get(void);

//#define OPTEK_NLD_INTEGRATOR

#define OPTEK_EBASS_ATTACK_THESHOLD				0x7ffc3a50		//-0.999db			
#define OPTEK_EBASS_RELEASE_THESHOLD			0x7100c4d8		//-1.0db

#define OPTEK_EBASS_ATTACK_RATE					OPTEK_DRC_ATTACK_RATE_0P1DB
#define OPTEK_EBASS_RELEASE_RATE				OPTEK_DRC_RELEASE_RATE_0P1DB


//The following Integrator Factor: input signal >= -62db NLD works, <-62db->zero out
#define		INTEGRATOR_FACTOR_44100		0x0144960c		//-40db //0x0066a4a5;				//-50db, not overflow for 70Hz/-3db sine wave
#define		INTEGRATOR_FACTOR_48000		0x0144960c		//-40db		//-50db, not overflow for 70Hz/-3db sine wave

#define		INTEGRATOR_FACTOR_88200		(INTEGRATOR_FACTOR_44100/2)			//-56db, not overflow for 70Hz sine wave
#define		INTEGRATOR_FACTOR_96000		(INTEGRATOR_FACTOR_48000/2)			//-56db, not overflow for 70Hz sine wave

#define OPTEK_EBASS_MIN_FREQ_SAMPLES	3000					/*48 * 16/1000, 16Hz*/


#define		NLD_RISE_TIME				((int) (2147483648*0.1))
//#define		NLD_FALL_TIME				((int) (2147483648*0.002))
//#define		NLD_FALL_TIME				((int) (2147483648*0.003))
#define		NLD_FALL_TIME				((int) (2147483648*0.004))
//#define		NLD_FALL_TIME				((int) (2147483648*0.005))


/*
	input->no input
	return->string of optek_drc version
*/
const char *optek_ebass_get_version (void);

/*
	input no
	return->how many bytes needed for optek_drc
*/
int optek_ebass_alloc(void);

void optek_ebass_init (void);


#ifdef OPTEK_NLD_INTEGRATOR

/*
	handle->optek_ebass handle
	integrator_factor->integrator factor
	attack_threshold->DRC attack level
	release_threshold->DRC release level, should be < attack_threshold, 
	it is better = attack_threshold * down_rate (for example 1db) and it is better several times (for example 4 or 6) of attack_rate
	attack_rate->in attack phase, drc_vol = drc_vol * attack_rate, it should be < 0db
	release_rate->in release phase, drc_vol = drc_vol * release_rate, it should be > 0db
	lev_detect_release_time->level detect release time, it should be < 0db, 
	it depends on input signal frequency, for example 1kHz signal, 0.04db down is ok, for 100Hz, 0.004db down is ok 
	update_samples->DRC update time, about 1ms samples
	return->no return
*/
void optek_ebass_set (void *handle,  int integrator_factor, uint32_t  min_freq_cn, int attack_threshold, int release_threshold, int attack_rate, int release_rate,
					  int lev_detect_release_time,
					 uint8_t update_samples);


/*
	objmem->mem allocated for optek_drc
	integrator_factor->integrator factor
	attack_threshold->DRC attack level
	release_threshold->DRC release level, should be < attack_threshold, 
		it is better = attack_threshold * down_rate (for example 1db) and it is better several times (for example 4 or 6) of attack_rate
	attack_rate->in attack phase, drc_vol = drc_vol * attack_rate, it should be < 0db
	release_rate->in release phase, drc_vol = drc_vol * release_rate, it should be > 0db
	lev_detect_release_time->level detect release time, it should be < 0db, 
		it depends on input signal frequency, for example 1kHz signal, 0.04db down is ok, for 100Hz, 0.004db down is ok 
	update_samples->DRC update time, about 1ms samples

	return->optek_ebass ptr
*/
void * optek_ebass_open (void * objmem, int integrator_factor, uint32_t  min_freq_cn,
						 int attack_threshold, int release_threshold, int attack_rate, int release_rate, int lev_detect_release_time,
					     uint8_t update_samples);



/*
	handle->optek_ebass ptr
	integrator_factor->integrator factor
	return->no return
*/
void optek_ebass_reinit (void *handle,int integrator_factor);


#else		//OPTEK_NLD_INTEGRATOR

/*
	objmem->mem allocated for optek_drc
	rise_t->NLD rise time
	fall_t->NLD fall time

	return->optek_ebass ptr
*/
void * optek_ebass_open (void * objmem, int rise_t, int fall_t);

/*
	handle->optek_ebass handle
	rise_t->NLD rise time
	fall_t->NLD fall time
	return->no return
*/
void optek_ebass_set (void *handle,  int rise_t, int fall_t);

#endif		//OPTEK_NLD_INTEGRATOR


/*
	handle->optek_ebass ptr
	r->output
	x->input
	hpf->hpf filter ptr
	lpf->lpf filter ptr
	N->samples (L sampes + R samples for stereo)
	return void
*/
#if 0
void optek_ebass_NLD (void * handle, int *r, int *x, void *ebass_band, int N);
#else
void optek_ebass_NLD (void * handle, int *r, int *x, void *ebass_band, int N) OPTEK_HIFI2_DSP_ATTIB;
#endif

#endif	//_OPTEK_HIFI2_EBASS_H
