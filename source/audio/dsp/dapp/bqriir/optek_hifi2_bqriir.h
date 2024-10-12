/*
 * Optek BiQuad - Biquad IIR Filter Implementation
 * Copyright by Optek on February 20, 2024
 */

#ifndef _OPTEK_HIFI2_BQRIIR_H
#define _OPTEK_HIFI2_BQRIIR_H

#include "NatureDSP_Signal.h"


/*
	input->no input
	return->string of optek_bgriir build date,time and version
*/


#define MAGIC 0x64758903


//#define SHIFT_VALUE    4 //Q28, coefs [8,-8)
//#define SHIFT_VALUE    3 //Q29, coefs [4,-4)
#define SHIFT_VALUE    2 //Q30, coefs [2,-2)


#if (SHIFT_VALUE == 2)
#define X1_VALUE 0x40000000
#elif (SHIFT_VALUE == 3)
#define X1_VALUE 0x20000000
#elif (SHIFT_VALUE == 4)
#define X1_VALUE 0x10000000
#else
#error "Must define SHIFT_VALUE!"
#endif



/*
 * Calculate the memory size needed for Biquad IIR filters
 *
 * Parameters:
 *   M: Number of Biquad IIR filters
 *
 * Returns:
 *   Total bytes required for memory allocation
 */
int optek_bqriir_alloc(int M);

/*
 * Initialization function for the Optek Biquad IIR module
 */
void optek_bqriir_init(void);

/*
 * Update all Biquad IIR coefficients
 *
 * Parameters:
 *   handle: Pointer to the Optek Biquad IIR filter structure
 *   coef: Array containing all coefficients for multiple filters
 *
 * Returns:
 *   true if coefficients were updated successfully
 *
 * Notes:
 *   - Must disable interrupt if caller task priority level is less than this handle process task priority
 *   - Caller task priority must not be higher than this handle process task priority
 *   - Recommended to use the same task as this handle process task
 */
int optek_bqriir_coeffs_update_all(void *handle, const int *coef);

/*
 * Update specific Biquad IIR coefficients
 *
 * Parameters:
 *   handle: Pointer to the Optek Biquad IIR filter structure
 *   k: Index of the Biquad IIR filter to update
 *   coef: Array containing coefficients for the specified filter
 *
 * Returns:
 *   true if coefficients were updated successfully
 *
 * Notes:
 *   - Must disable interrupt if caller task priority level is less than this handle process task priority
 *   - Caller task priority must not be higher than this handle process task priority
 *   - Recommended to use the same task as this handle process task
 */
int optek_bqriir_coeffs_update(void *handle, int k, const int *coef);


/*
 * Open a Biquad IIR filter
 *
 * Parameters:
 *   objmem: Memory allocated for Biquad IIR filter
 *   M: Number of Biquad IIR filters
 *   coef: Array containing all coefficients for multiple filters
 *
 * Returns:
 *   Handle to the Biquad IIR filter structure
 */
void *optek_bqriir_open(void *objmem, int M, const int *coef);

/*
	handle->optek_bqriir,
	r->output
	x->input
	N->samples
	return void
*/
//MCPS: 0.31 Average: 0.31 Peak: 0.31
void optek_bqdir_mono_24b_process (
		  void *handle, 
          int * r,     // [N] output
		  int * x,     // [N] input
          int N
) OPTEK_HIFI2_DSP_ATTIB;

/*
	handle->optek_bqriir,
	r->output
	x->input
	N->samples
	return void
*/
//MCPS: 0.31 Average: 0.31 Peak: 0.31
void optek_bqdiir_mono_16b_process (
                            		void *handle, 
                            		short * r,     // [N] output
                            		short * x,     // [N] input
                            		int N
                            		) OPTEK_HIFI2_DSP_ATTIB;

/*
	handle->optek_bqriir,
	r->output
	x->input
	N->samples (L sampes + R samples for stereo)
	return void
*/
//MCPS: 0.58 Average: 0.58 Peak: 0.58
void optek_bqdiir_stereo_24b_process (
                            		void *handle, 
                            		int * r,     // [N] output
                            		int * x,     // [N] input
                            		int N
                            		) OPTEK_HIFI2_DSP_ATTIB;

/*
	handle->optek_bqriir,
	r->output
	x->input
	N->samples (L sampes + R samples for stereo)
	return void
*/
//MCPS: 0.58 Average: 0.58 Peak: 0.58
void optek_bqdiir_stereo_16b_process (
                            		void *handle, 
                            		short * r, // [N] output
                            		short * x, // [N] input
                            		int N
                            		) OPTEK_HIFI2_DSP_ATTIB;


/*
 * Biquad IIR Mono Filter Processing Function with Strides, For Multichannel
 *
 * Parameters:
 *   handle: Pointer to the filter handle (optek_bqriir).
 *   input: Pointer to the input buffer. The input samples are stored with a specified input stride.
 *   output: Pointer to the output buffer. The output samples are stored with a specified output stride.
 *   samples: Number of samples.
 *   in_stride: Stride for the input buffer. It specifies the distance between consecutive input samples.
 *   out_stride: Stride for the output buffer. It specifies the distance between consecutive output samples.
 *
 * Notes:
 *   - The first sample to be processed must be in input[0], and the first output sample will be stored in output[0].
 *   - If the output buffer is the same as the input buffer, the input samples will be overwritten by the filtered output.
 *   - The output and input buffers can have strides to process non-contiguous samples.
 *   - The biquad IIR filter is configured by the handle, including filter coefficients and state variables.
 *
 * Biquad IIR Filter Equation:
 *   y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] - a1 * y[n-1] - a2 * y[n-2]
 *
 *   where:
 *   - y[n] is the current output sample.
 *   - x[n] is the current input sample.
 *   - x[n-1], x[n-2] are the previous input samples.
 *   - y[n-1], y[n-2] are the previous output samples.
 *   - b0, b1, b2 are the feedforward (input) coefficients.
 *   - a1, a2 are the feedback (output) coefficients.
 */

void optek_bqdiir_mono_32b_process_strided(
    void *handle,
    const int *input,  // [samples * in_stride] input
    int *output,       // [samples * out_stride] output
    int samples,
    int in_stride,
    int out_stride
);

/*
 * Biquad IIR Stereo Filter Processing Function with Strides, For Multichannel
 *
 * Parameters:
 *   handle: Pointer to the filter handle (optek_bqriir).
 *   input: Pointer to the input buffer. The input samples are stored with a specified input stride.
 *   output: Pointer to the output buffer. The output samples are stored with a specified output stride.
 *          Note: output buffer can be the same as the input buffer.
 *   samples: Number of samples (L samples + R samples for stereo).
 *   in_stride: Stride for the input buffer. It specifies the distance between consecutive input samples.
 *   out_stride: Stride for the output buffer. It specifies the distance between consecutive output samples.
 *
 * Notes:
 *   - The first sample to be processed must be in input[0], and the first output sample will be stored in output[0].
 *   - If the output buffer is the same as the input buffer, the input samples will be overwritten by the filtered output.
 *   - The output and input buffers can have strides to process non-contiguous samples.
 *   - The biquad IIR filter is configured by the handle, including filter coefficients and state variables.
 *
 * Biquad IIR Filter Equation:
 *   y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] - a1 * y[n-1] - a2 * y[n-2]
 *
 *   where:
 *   - y[n] is the current output sample.
 *   - x[n] is the current input sample.
 *   - x[n-1], x[n-2] are the previous input samples.
 *   - y[n-1], y[n-2] are the previous output samples.
 *   - b0, b1, b2 are the feedforward (input) coefficients.
 *   - a1, a2 are the feedback (output) coefficients.
 */

void optek_bqdiir_stereo_32b_process_strided(
    void *handle,
    const int *input,  // [samples * in_stride] input
    int *output,  // [samples * out_stride] output
    int samples,
    int in_stride,
    int out_stride
);

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


/*pre scale and convert*/

/*
	r->output, if r = x, r size should be double of x, aligned with 8 bytes
	x->input
	N->samples
	return void
*/
void optek_hifi2_prescale_16b (
                            	int * r,   // [N] output
                            	short * x, // [N] input
                            	int N
                            ) OPTEK_HIFI2_DSP_ATTIB;



/*
	r->output, if r = x, r size should be double of x, aligned with 8 bytes
	x->input
	N->samples
	return void
*/
void optek_hifi2_16b_to_24b (
                            int * r,   // [N] output
                            short * x, // [N] input
                            int N
                            ) OPTEK_HIFI2_DSP_ATTIB;



/*
	r->output
	x->input
	N->samples
	return void
*/
void optek_hifi2_prescale_24b (
                            int * r, //[N] output
                            int * x, //[N] input
                            int N
                            ) OPTEK_HIFI2_DSP_ATTIB;

/*
	r->output
	x->input
	N->samples
	return void
*/
void optek_hifi2_24b_to_16b (
                             short * r, // [N] output
                             int * x,   // [N] input
                             int N
                             ) OPTEK_HIFI2_DSP_ATTIB;

/*
	r->output		r = (x_L + x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_stereo_24b_to_mono (
                                     int * r, // [N/2] output
                            		int * x, // [N] input
                            		int N);

/*
	r->output		r.L = r.R = (x_L + x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_stereo_24b_to_2mono (
                                int * r, // [N] output
                            	int * x, // [N] input
                            	int N);

/*
	r->output		r.L = (x_L + x_R)/2 r.R = -(x_L + x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_stereo_24b_to_difference (
                                int * r, // [N] output
                            	int * x, // [N] input
                            	int N);

/*
	r->output		r.L = (x_L - x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_difference_in_24b_to_mono (
                                 int * r, // [N/2] output
                            	int * x,  // [N] input
                            	int N);

/*
	r->output		r.L = r.R = (x_L - x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_difference_in_24b_to_2mono (
                                int * r, // [N] output
                            	int * x, // [N] input
                            	int N);

/*
	r->output		r = (x_L + x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_stereo_16b_to_mono (
                                short * r, // [N/2] output
                            	short * x, // [N] input
                            	int N);

/*
	r->output		r.L = r.R = (x_L + x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_stereo_16b_to_2mono (
                                short * r, // [N] output
                            	short * x, // [N] input
                            	int N);

/*
	r->output		r.L = (x_L + x_R)/2 r.R = -(x_L + x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_stereo_16b_to_difference (
                                short * r, // [N] output
                            	short * x, // [N] input
                            	int N);

/*
	r->output		r.L = (x_L - x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_difference_in_16b_to_mono (
                                short * r, // [N/2] output
                            	short * x, // [N] input
                            	int N);


/*
	r->output		r.L = r.R = (x_L - x_R)/2
	x->input
	N->samples
	return void
*/
void optek_hifi2_difference_in_16b_to_2mono (
                                short * r, // [N] output
                            	short * x, // [N] input
                            	int N);


const char *optek_bqriir_data_get(void);
const char *optek_bqriir_time_get(void);
const char *optek_bqriir_version_get(void);

#endif	//_OPTEK_HIFI2_BQRIIR_H
