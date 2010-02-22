/*
 ================================================================
 
 Amplifier Filters
 The Myelin Project
 
 Copyright (C) 2010 Saumitro Dasgupta.
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>
 
 ================================================================
*/

#ifndef __AMP_FILTERS_H__
#define __AMP_FILTERS_H__

#define BANDPASS_FILTER_COUNT 202
#define NOTCH_FILTER_COUNT 14

typedef struct tagFilterSpec {
	float hf; // Highpass frequency
	float lf; // Lowpass frequency
	int sf; // Sampling frequency
	int type;
	int order;
	unsigned char details[0x90];
} FilterSpec;

#endif
