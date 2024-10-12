#ifndef _OPTEK_HIFI2_MIXER_H
#define _OPTEK_HIFI2_MIXER_H

/*
	input->no input
	return->string of optek_mixer build date,time and version
*/
const char *optek_mixer_data_get(void);
const char *optek_mixer_time_get(void);
const char *optek_mixer_version_get(void);


/*
	r->x - y
	x->input
	y->input
	N->samples
	return void
*/
//MCPS: 0.20 Average: 0.20 Peak: 0.20
void optek_hifi2_minus_24b (
                            int * r, // [N] output
                            int * x, // [N] input
                            int * y, // [N] input
                            int N
                            ) OPTEK_HIFI2_DSP_ATTIB;

/*
	r->x + y
	x->input
	y->input
	N->samples
	return void
*/
//MCPS: 0.20 Average: 0.20 Peak: 0.20
void optek_hifi2_plus_24b (
                           int * r, // [N] output
                           int * x, // [N] input
                           int * y, // [N] input
                           int N
                          ) OPTEK_HIFI2_DSP_ATTIB;



/*
	r->x - y
	x->input
	y->input
	N->samples
	return void
*/
//MCPS: 0.20 Average: 0.20 Peak: 0.20
void optek_hifi2_minus_16b (
                            short * r, // [N] output
                            short * x, // [N] input
                            short * y, // [N] input
                            int N
                           ) OPTEK_HIFI2_DSP_ATTIB;

/*
	r->x + y
	x->input
	y->input
	N->samples
	return void
*/
//MCPS: 0.20 Average: 0.20 Peak: 0.20
void optek_hifi2_plus_16b (
                           short * r, // [N] output
                           short * x, // [N] input
                           short * y, // [N] input
                           int N
                          ) OPTEK_HIFI2_DSP_ATTIB;

/*
mx1->mix input1 24b stereo, mx1==NULL--> dsiable mixer
mx1b->mx1b*mx1
mx2->mix input2 16b mono, mx2==NULL--> dsiable mixer
mx2b->mx2b*mx2
r->output 24b stereo, r = mx1*mx1b + mx2*mx2b,r==NULL-->dsiable mixer
N->samples
return->no return
*/

void optek_hifi2_24bstereo_16bmono_mixer(
                            		int *mx1,   // [N] input, 24b stereo                              
                            		int mx1b,
                            		short *mx2, // [N] input, 16b mono
                            		int mx2b,
                            		int *r,     // [N] output, 24b stereo                            	   
                            		int N
                            		) OPTEK_HIFI2_DSP_ATTIB;

/*
mx1->mix input1 24b stereo, mx1==NULL--> dsiable mixer
mx1b->mx1b*mx1
mx2->mix input2 16b stereo, mx2==NULL--> dsiable mixer
mx2b->mx2b*mx2
r->output 24b stereo, r = mx1*mx1b + mx2*mx2b,r==NULL-->dsiable mixer
N->samples
return->no return
*/

void optek_hifi2_24bstereo_16bstereo_mixer(
                            		int *mx1,   // [N] input,24b stereo                              
                            		int mx1b,
                            		short *mx2, // [N] input, 16b stereo
                            		int mx2b,
                            		int *r,     // [N] output,	24b stereo                            	   
                            		int N
                            		) OPTEK_HIFI2_DSP_ATTIB;


/*
mx1->mix input1 16b stereo, mx1==NULL--> dsiable mixer
mx1b->mx1b*mx1
mx2->mix input2 16b mono, mx2==NULL--> dsiable mixer
mx2b->mx2b*mx2
r->output 16b stereo, r = mx1*mx1b + mx2*mx2b,r==NULL-->dsiable mixer
N->samples
return->no return
*/

void optek_hifi2_16bstereo_16bmono_mixer(
                            		short *mx1,  // [N] input,16b stereo                              
                            		int mx1b,
                            		short *mx2,  // [N] input, 16b mono                            	  
                            		int mx2b,
                            		short *r,    // [N] output,	16b stereo                            	   
                            		int N
                            		) OPTEK_HIFI2_DSP_ATTIB;

/*
mx1->mix input1 16b Xchannel, mx1==NULL--> dsiable mixer
mx1b->mx1b*mx1
mx2->mix input2 16b Xchannel, mx2==NULL--> dsiable mixer
mx2b->mx2b*mx2
r->output 16b Xchannel, r = mx1*mx1b + mx2*mx2b,r==NULL-->dsiable mixer
N->samples
return->no return
*/

void optek_hifi2_16bXchannel_mixer(
                            		short *mx1, // [N] input,16b Xchannel						  
                            		int mx1b,
                            		short *mx2, // [N] input,16b Xchannel                            	  
                            		int mx2b,
                            		short *r,   // [N] output,16b Xchannel                               
                            		int N
                            		) OPTEK_HIFI2_DSP_ATTIB;

/*
mx1->mix input1 24b Xchannel, mx1==NULL--> dsiable mixer
mx1b->mx1b*mx1
mx2->mix input2 24b Xchannel, mx2==NULL--> dsiable mixer
mx2b->mx2b*mx2
r->output 24b Xchannel, r = mx1*mx1b + mx2*mx2b,r==NULL-->dsiable mixer
N->samples
return->no return
*/

void optek_hifi2_24bXchannel_mixer(
                            		int *mx1, // [N] input,24b Xchannel						  
                            		int mx1b,
                            		int *mx2, // [N] input,24b Xchannel                            	  
                            		int mx2b,
                            		int *r,   // [N] output,24b Xchannel                               
                            		int N
                            		) OPTEK_HIFI2_DSP_ATTIB;
#endif	//_OPTEK_HIFI2_MIXER_H
