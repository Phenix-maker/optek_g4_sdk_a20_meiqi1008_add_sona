#ifndef _OPTEK_HIFI2_3D_H
#define _OPTEK_HIFI2_3D_H


/*
	input->no input
	return->string of optek_hifi2_3D build date,time and version
*/
const char *optek_hifi2_3D_data_get(void);
const char *optek_hifi2_3D_time_get(void);
const char *optek_hifi2_3D_version_get(void);

/*
x->input
r->output buf r = x - x*b
b->b=0, disable mixer
N->samples
return->no return
*/
//MCPS: 0.34 Average: 0.34 Peak: 0.35
void optek_hifi2_3D (
                     int *x,	// [N] input
                     int *r,	// [N] output                            	  
                     int b,
                     int N) OPTEK_HIFI2_DSP_ATTIB;

#endif	//_OPTEK_HIFI2_3D_H
