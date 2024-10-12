#ifndef _OPTEK_HIFI3_MIXER_H
#define _OPTEK_HIFI3_MIXER_H

/*
	Optek digital Volume demo
	Copyright by Optek in Aug 22, 2020
*/

#define VOL_EXP    4 /*max gain is +24db*/

/*
 * Mixer function for processing interleaved input signal and producing interleaved output signal.
 *
 * Parameters:
 *   input_buffer: Pointer to the interleaved input signal buffer (32-bit per channel).
 *   coefficients: An array of coefficients for each output channel, describing how to mix input channels into each output channel.
 *   num_input_channels: Number of input channels.
 *   num_output_channels: Number of output channels.
 *   output_buffer: Pointer to the interleaved output signal array (32-bit per channel). Note:It's ok to be the same as the input_buffer 
 *   num_samples: Number of sets of interleaved channels to process. Example: in stereo, one sample includes L and R channel.
 * 
 * Notes:
 *   - The input and output channels are interleaved.
 *   - Coefficients are organized with column order determined by input channels and row order determined by output channels.
 *   - Each set of interleaved channels corresponds to one sample.
 *   - Coefficient Scaling: Coefficients are in Q5.27 format.
 *     0x08000000 = 1.0, 0x10000000 = 2.0, 0x04000000 = 0.5.
 *   - Shifter Value: Shifter value is set up using WAE_SAR(VOL_EXP), where VOL_EXP is the maximum gain (e.g., +24dB).
 */


void optek_hifi3_mixer(int *input_buffer, int *coefficients, int num_input_channels, int num_output_channels, int *output_buffer, int num_samples) OPTEK_HIFI2_DSP_ATTIB;

#endif	//_OPTEK_HIFI3_MIXER_H
