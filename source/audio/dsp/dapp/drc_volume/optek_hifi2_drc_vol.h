/*
	Optek DRC & VOL
		Copyright by Optek in June 9, 2016
*/

#ifndef _OPTEK_HIFI2_DRC_VOL_H
#define _OPTEK_HIFI2_DRC_VOL_H



enum {
	OPTEK_DRC_IDLE = 0,
	OPTEK_DRC_ATTACK,
	OPTEK_DRC_RELEASE
};

#define OPTEK_DRC_MAX_VOL		(0x80000000 >> VOL_EXP)			/*exp -4, x1*/
#define M100DB					(0x0000053e)					/*-100db*/
#define M80DB					(0x0000346e)					/*-80db*/


typedef struct
{
    uint32_t magic;				//check if the structure is valid	
	f24 attack_threshold;		//attack_threshold - release_threshold should be several steps of change rate  
	f24 release_threshold;
	f24 attack_rate;			//should < OPTEK_DRC_MAX_VOL, drc_vol = drc_vol * attack_rate 
	f24 release_rate;			//should > OPTEK_DRC_MAX_VOL, drc_vol = drc_vol * attack_rate
	
	f24 lev_detect_release_time;
	uint32_t  plev;					//peak or rms detect level
	f24	drc_vol;				


	/*vol*/
	f24 master_vol;
	f24 vol_curr_L;
	f24 vol_req_L;
	f24 vol_curr_R;
	f24 vol_req_R;
	f24 up_rate;
	f24 down_rate;
	f24 L_last_sample;			//for block data zero detect
	f24 R_last_sample;			//for block data zero detect
	
	f24 vol_mute_L;
	f24 vol_mute_R;

	//uint16_t attack_to_release;
	
	uint8_t mute;

	uint8_t post_scale;
	uint8_t L_R_same_vol;

	/*drc*/
	uint8_t clipping_enable;
	uint8_t state;


	uint8_t update_samples;
	uint8_t update_vol_samples;
	uint8_t sample_cn;
	uint8_t sample_vol_cn;

	uint32_t attack_hold_samples;

} optek_drc_vol __attribute__((packed));

/*
	input->no input
	return->string of optek_drc_vol build date,time and version
*/
const char *optek_drc_vol_data_get(void);
const char *optek_drc_vol_time_get(void);
const char *optek_drc_vol_version_get(void);

/*
	input no
	return->how many bytes needed for optek_drc
*/
int optek_drc_vol_alloc(void);

void optek_drc_vol_init (void);



/*
	objmem->mem allocated for optek_drc_vol
	attack_threshold->DRC attack level
	release_threshold->DRC release level, should be < attack_threshold, 
		it is better = attack_threshold * down_rate (for example 1db) and it is better several times (for example 4 or 6) of attack_rate
	attack_rate->in attack phase, drc_vol = drc_vol * attack_rate, it should be < 0db
	release_rate->in release phase, drc_vol = drc_vol * release_rate, it should be > 0db
	lev_detect_release_time->level detect release time, it should be < 0db, 
		it depends on input signal frequency, for example 1kHz signal, 0.04db down is ok, for 100Hz, 0.004db down is ok 
	clipping_enable-> 0 means no clipping, 1 means clipping
	update_samples->DRC update time, about 1ms samples
	return->return optek_drc_vol handle
*/
void * optek_drc_vol_open (void * objmem, int attack_threshold, int release_threshold, int attack_rate, int release_rate, int lev_detect_release_time,
						   uint8_t clipping_enable, uint8_t update_samples);


/*
	handle->optek_drc_vol handle
	master_vol->real vol = master_vol * vol, for different applications
	vol_init_L->int vol for chan L
	vol_init_R->int vol for chan R
	up_rate->soft vol up step, should > 0db, vol* up_rate
	down_rate->soft vol down step, should <0db, vol * down_rate
	post_scale->0, not support post scale; 1, support post scale
	L_R_same_vol->for soft vol, 0 means L/R vol is not same, 1 means L/R vol is same
	return->no return
*/
void optek_drc_vol_open_vol (void * handle, int master_vol, int vol_init_L, int vol_init_R, int up_rate,
                            int down_rate, uint8_t post_scale, uint8_t L_R_same_vol);


/*
	handle->optek_drc_vol handle
	attack_threshold->DRC attack level
	release_threshold->DRC release level, should be < attack_threshold, 
	it is better = attack_threshold * down_rate (for example 1db) and it is better several times (for example 4 or 6) of attack_rate
	attack_rate->in attack phase, drc_vol = drc_vol * attack_rate, it should be < 0db
	release_rate->in release phase, drc_vol = drc_vol * release_rate, it should be > 0db
	lev_detect_release_time->level detect release time, it should be < 0db, 
	it depends on input signal frequency, for example 1kHz signal, 0.04db down is ok, for 100Hz, 0.004db down is ok 
	clipping_enable-> 0 means no clipping, 1 means clipping
	update_samples->DRC update time, about 1ms samples
	attack_hold_samples -> attack hold time, unit is samples
	return->no return
*/
#if 0
void optek_drc_vol_set_drc (void *handle,  int attack_threshold, int release_threshold, int attack_rate, int release_rate,
                            int lev_detect_release_time,
                            uint8_t clipping_enable, uint8_t update_samples);
#else
//new the library of lib_optek_dsp_dapp.a
void optek_drc_vol_set_drc (void *handle,  int attack_threshold, int release_threshold, int attack_rate, int release_rate,
                            int lev_detect_release_time,
                            uint8_t clipping_enable, uint8_t update_samples, uint32_t attack_hold_samples);
#endif


/*
	handle->optek_drc_vol handle
	return->no return
*/
void optek_drc_vol_set_mute_on (void *handle);

/*
	handle->optek_drc_vol handle
	return->no return
*/
void optek_drc_vol_set_mute_off (void *handle);


/*
	handle->optek_drc_vol handle
	vol_L->L vol
	vol_R->R vol
	return->no return
*/
void optek_drc_vol_set_vol (void *handle, int vol_L, int vol_R);


/*
	handle->optek_drc_vol handle
	master_vol->master vol
	return->no return
*/
void optek_drc_vol_set_master_vol (void *handle, int master_vol);


/*
	handle->optek_drc_vol handle
	update_samples->update time
	return->no return
*/
void optek_drc_vol_set_update_samples (void *handle, int update_samples);

/*
	when opening, vol update time is same with drc update time
	handle->optek_drc_vol handle
	update_samples->update vol_time
	return->no return
*/
void optek_drc_vol_set_vol_update_samples (void *handle, int update_samples);


/*
	handle->optek_drc_vol handle
	up_rate->vol up change rate
	down_rate->vol down change rate
	return->no return
*/
void optek_drc_vol_set_rate (void *handle, int up_rate, int down_rate);

/*
	handle->optek_drc_vol
	x->input
	r->output
	N->samples (L sampes + R samples for stereo)
	return void
*/
//MCPS:3.69 Average: 3.73 Peak: 4.09
void optek_drc_zdt_vol_stereo_24b_process (void *handle, const int *x, int *r, int N) OPTEK_HIFI2_DSP_ATTIB;


/*
	handle->optek_drc_vol
	x->input
	r->output
	N->samples (L sampes + R samples for stereo)
	return void
*/
//MCPS: 2.95 Average: 2.98 Peak: 3.24
void optek_drc_soft_vol_stereo_24b_process (void *handle, const int *x, int *r, int N) OPTEK_HIFI2_DSP_ATTIB;


/*
 * Dynamic Range Compression (DRC) Soft Volume Processing Function (Mono, 24-bit), For Multichannel
 *
 * Parameters:
 *   handle: Pointer to the DRC handle (optek_drc_vol).
 *   x: Pointer to the input buffer containing 24-bit samples.
 *   r: Pointer to the output buffer for processed samples.
 *   N: Number of samples (each sample represented by 24 bits).
 *   in_stride: Stride for the input buffer. It specifies the distance between consecutive input samples.
 *   out_stride: Stride for the output buffer. It specifies the distance between consecutive output samples.
 *
 * Notes:
 *   - The first sample to be processed must be in input[0], and the first output sample will be stored in output[0].
 *   - If the output buffer is the same as the input buffer, the input samples will be overwritten by the filtered output.
 *   - The input and output buffers can have strides to process non-contiguous samples.
 */

void optek_drc_soft_vol_mono_24b_process_strided (void *handle, const int *x, int *r, int N, int in_stride, int out_stride)OPTEK_HIFI2_DSP_ATTIB;

/*
 * Dynamic Range Compression (DRC) Soft Volume Processing Function (Stereo, 24-bit), For Multichannel
 *
 * Parameters:
 *   handle: Pointer to the DRC handle (optek_drc_vol).
 *   x: Pointer to the input buffer containing 24-bit samples.
 *   r: Pointer to the output buffer for processed samples.
 *   N: Number of samples (each sample represented by 24 bits).
 *   in_stride: Stride for the input buffer. It specifies the distance between consecutive input samples.
 *   out_stride: Stride for the output buffer. It specifies the distance between consecutive output samples.
 *
 * Notes:
 *   - The first sample to be processed must be in input[0], and the first output sample will be stored in output[0].
 *   - If the output buffer is the same as the input buffer, the input samples will be overwritten by the filtered output.
 *   - The input and output buffers can have strides to process non-contiguous samples.
 */
void optek_drc_soft_vol_stereo_24b_process_strided (void *handle, const int *x, int *r, int N, int in_stride, int out_stride);

#endif	//_OPTEK_HIFI2_DRC_VOL_H
