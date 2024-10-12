/*
	Optek Vol
	Copyright by Optek in May 24, 2016
*/

#ifndef _OPTEK_HIFI2_VOL_H
#define _OPTEK_HIFI2_VOL_H


#define MAGIC 0x64758903

typedef struct
{
    uint32_t magic;				//check if the structure is valid	
	f24 master_vol;
	f24 vol_curr_L;
	f24 vol_req_L;
	f24 vol_curr_R;
	f24 vol_req_R;
	f24 L_last_sample;			//for block data zero detect
	f24 R_last_sample;			//for block data zero detect
	uint8_t L_R_same_vol;

} optek_vol __attribute__((packed));


/*
	input->no input
	return->string of optek_vol build date,time and version
*/

/*
	input no
	return->how many bytes needed for optek_vol
*/
int optek_vol_alloc(void);

void optek_vol_init (void);

/*
	handle->optek_vol
	master_vol->real vol = master_vol * vol, for different applications
	return->no return
*/
void optek_vol_set_master_vol (void *handle, int master_vol);

/*
	handle->optek_vol
	vol_L->request vol for chan L
	vol_R->request vol for chan R
	return->no return
*/
void optek_vol_set_vol (void *handle, int vol_L, int vol_R);


/*
	objmem->mem allocated for optek_vol
	master_vol->real vol = master_vol * vol, for different applications
	vol_curr_L->current vol for chan L
	vol_req_L->request vol for chan L
	vol_curr_R->current vol for chan R
	vol_req_R->request vol for chan R
	up_rate->soft vol up step, should > 0db, vol* up_rate
	down_rate->soft vol down step, should <0db, vol * down_rate
	update_samples->soft vol update time, about 1ms
	post_scale->0, not support post scale; 1, support post scale
	L_R_same_vol->for soft vol, 0 means L/R vol is not same, 1 means L/R vol is same
	return->no return
*/
void * optek_vol_open (void * objmem, int master_vol, int vol_curr_L, int vol_req_L, int vol_curr_R, int vol_req_R, uint8_t L_R_same_vol);


/*
	handle->optek_vol
	x->input buf
	r->output buf
	N->samples (L sampes + R samples for stereo, samples for mono)
	return->no return
*/
//MCPS: 0.78 Average: 0.78 Peak: 0.85
void optek_vol_stereo_24b_w_zdt_process (void *handle, const int *x, int *r, int N) OPTEK_HIFI2_DSP_ATTIB;

/*
x->input buf
r->output buf
gain->gain(VOL_EXP)
N->samples (L sampes + R samples for stereo, samples for mono)
	return->no return
*/
//MCPS: 0.34 Average: 0.34 Peak: 0.34
void optek_vol_24b_process (const int *x, int *r, int gain, int N) OPTEK_HIFI2_DSP_ATTIB;

/*
x->input buf
r->output buf
gain->gain(EBASS_VOL_EXP)
N->samples (L sampes + R samples for stereo, samples for mono)
	return->no return
*/
void optek_vol_ebass_24b_process (const int *x, int *r, int gain, int N) OPTEK_HIFI2_DSP_ATTIB;


/*
x->input buf
r->output buf
gain->gain(VOL_EXP)
N->samples (L sampes + R samples for stereo, samples for mono)
	return->no return
*/
//MCPS: 0.34 Average: 0.34 Peak: 0.34
void optek_vol_16b_process (const short *x, short *r, int gain, int N) OPTEK_HIFI2_DSP_ATTIB;


const char *optek_vol_data_get(void);
const char *optek_vol_time_get(void);
const char *optek_vol_version_get(void);


#endif	//_OPTEK_HIFI2_VOL_H
