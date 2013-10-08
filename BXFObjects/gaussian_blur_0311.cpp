/*
Copyright (c) 2010, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that 
the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
	  disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors may be used to endorse or promote products 
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
#include <immintrin.h>
#include <omp.h>  


typedef unsigned int uint;

#define MAX_THREAD	12   // Maximum HW threads

int num_thread;

// coefficients for gaussian filter
float a0 = 0, a1 = 0, a2 = 0, a3 = 0, b1 = 0, b2 = 0, cprev = 0, cnext = 0;

int avx_support()
{
	__asm {
		   ; result in eax
			   mov eax, 1
			   cpuid
			   and ecx, 0x018000000
			   cmp ecx, 0x018000000  ; check OSXSAVE and AVX feature flag
			   jne  NOT_SUPPORTED
			   mov ecx, 0
			   XGETBV
			   and eax, 0x06
			   cmp eax, 0x06
			   jne  NOT_SUPPORTED
			   mov eax, 1
			   jmp done
			   NOT_SUPPORTED:
			   mov eax, 0
		done:
	}
}
/* SSE Implementation: gaussianVertical_sse
 *		oTemp - Temporary small buffer used between left to right pass
 *		id    - input image 
 *		od    - output image from this filter
 *		height - image height
 *		width - image width
 * 		a0, a1, a2, a3, b1, b2, cprev, cnext: Gaussian coefficients
 */
void gaussianVertical_sse(float *oTemp, float* id,  unsigned int *od, int height, int width, float *a0, float *a1, float *a2, float *a3, float *b1, float *b2, float *cprev, float *cnext )
{
	__m128 prevIn, currIn,  prevOut, prev2Out, prevInN, currInN,  prevOutN, prev2OutN, coeft, coefa0, coefa1, coefb1, coefb2;
	
	// Edge calculations
	coeft = _mm_load_ss((float *) cprev);
	coeft = _mm_shuffle_ps(coeft, coeft, 0x00);
	prevIn  = _mm_loadu_ps((float *) id);
	prev2Out = _mm_mul_ps(prevIn,  coeft);
	prevOut = prev2Out;
	prevInN  = _mm_loadu_ps((float *) (id+4*height));	
	prev2OutN = _mm_mul_ps(prevInN, coeft);
	prevOutN = prev2OutN;
	// load the coefficients
	coefa0 = _mm_load_ss((float *) a0);
	coefa0 = _mm_shuffle_ps(coefa0, coefa0, 0x00);
	coefa1 = _mm_load_ss((float *) a1);
	coefa1 = _mm_shuffle_ps(coefa1, coefa1, 0x00);
	coefb1 = _mm_load_ss((float *) b1);
	coefb1 = _mm_shuffle_ps(coefb1, coefb1, 0x00);
	coefb2 = _mm_load_ss((float *) b2);
	coefb2 = _mm_shuffle_ps(coefb2, coefb2, 0x00);

	// it is widthwise as input is transposed.
    for (int y = 0; y < height; y++) {
		// Unpack 2 input pixels
		currIn  = _mm_loadu_ps((float *) id);			
		currInN  =_mm_loadu_ps((float *) (id+4*height));

		__m128 currComp = _mm_mul_ps(currIn, coefa0);
	  	__m128 temp1 = _mm_mul_ps(prevIn, coefa1);
	  	__m128 temp2 = _mm_mul_ps(prevOut, coefb1);
	 	__m128 temp3 = _mm_mul_ps(prev2Out, coefb2);
	  	currComp = _mm_add_ps(currComp, temp1);
	  	temp2 = _mm_add_ps(temp2, temp3);
	  	prev2Out = prevOut;
	 	prevOut= _mm_sub_ps(currComp, temp2);
	  	prevIn = currIn;
		// Store output in temp buffer
	    _mm_storeu_ps((float *)(oTemp), prevOut);

		//Process next  pixel
	    currComp = _mm_mul_ps(currInN, coefa0);
	  	temp1 = _mm_mul_ps(prevInN, coefa1);
	  	temp2 = _mm_mul_ps(prevOutN, coefb1);
	  	temp3 = _mm_mul_ps(prev2OutN, coefb2);
	    currComp = _mm_add_ps(currComp, temp1);
	  	temp2 = _mm_add_ps(temp2, temp3);
	  	prev2OutN = prevOutN;
	  	prevOutN= _mm_sub_ps(currComp, temp2);
	 	prevInN = currInN;
		_mm_storeu_ps((float *)(oTemp+4), prevOutN);
	
		id += 4; oTemp += 8;    // move to next row
    } //height = j loop
  
	// start right-to-left pass
	id -= 4;
	oTemp -=8;
	od += width*(height-1); //set to last row.
   
	//edge caculations
	coeft = _mm_load_ss((float *) cnext);
	coeft = _mm_shuffle_ps(coeft, coeft, 0x00);
	prevIn  = _mm_loadu_ps((float *) id);
	currIn = prevIn;
	prev2Out = _mm_mul_ps(prevIn, coeft);
	prevOut = prev2Out;

	prevInN  = _mm_loadu_ps((float *) (id+4*height));
	currInN = prevInN;
	prev2OutN = _mm_mul_ps(prevInN, coeft);
	prevOutN = prev2OutN;

	// load the coefficients
	coefa0 = _mm_load_ss((float *) a2);
	coefa0 = _mm_shuffle_ps(coefa0, coefa0, 0x00);
	coefa1 = _mm_load_ss((float *) a3);
	coefa1 = _mm_shuffle_ps(coefa1, coefa1, 0x00);

    for (int y = height-1; y >= 0; y--) {
		// Unpack 2 input pixel
		__m128 inNext  = _mm_loadu_ps((float *) id);	
		__m128 inNextN  = _mm_loadu_ps((float *) (id+4*height));

		//get output from left-to-right pass
		__m128 output = _mm_load_ps((float *) (oTemp)); 
		  	
		__m128 currComp = _mm_mul_ps(currIn, coefa0);
	  	__m128 temp1 = _mm_mul_ps(prevIn, coefa1);
	  	__m128 temp2 = _mm_mul_ps(prevOut, coefb1);
	  	__m128 temp3 = _mm_mul_ps(prev2Out, coefb2);
	  	currComp = _mm_add_ps(currComp, temp1);
	  	temp2 = _mm_add_ps(temp2, temp3);
	  	prev2Out = prevOut;
	  	prevOut= _mm_sub_ps(currComp, temp2);
	  	prevIn = currIn;
	  	currIn = inNext;
	
		//add currently computed output left-to-right pass output
	  	output = _mm_add_ps(output, prevOut);
	    __m128i  output1 = _mm_cvttps_epi32(output);

	  	 //get the left-to-right pass output
		__m128 outputN = _mm_loadu_ps((float *) (oTemp+4));

	   	currComp = _mm_mul_ps(currInN, coefa0);
	   	temp1 = _mm_mul_ps(prevInN, coefa1);
	   	temp2 = _mm_mul_ps(prevOutN, coefb1);
	   	temp3 = _mm_mul_ps(prev2OutN, coefb2);
	  	currComp = _mm_add_ps(currComp, temp1);
	  	temp2 = _mm_add_ps(temp2, temp3);
	  	prev2OutN = prevOutN;
	  	prevOutN= _mm_sub_ps(currComp, temp2);
	  	prevInN = currInN;
	  	currInN = inNextN;
	  	outputN = _mm_add_ps(outputN, prevOutN);

		__m128i  output2 = _mm_cvttps_epi32(outputN);
		   
	  output1 = _mm_packus_epi32(output1, output2);
	  output1 = _mm_packus_epi16(output1, output1);
	  _mm_storel_epi64((__m128i *)od, output1);
	   id -= 4; od -= width; oTemp -=8; // move to previous row
    } //height = j loop 
	_mm256_zeroupper();
} // gaussianvertical SSE

/* SSE Implementation: gaussianHorizontal_sse
 *		oTemp - Temporary small buffer used between left to right pass
 *		id    - input image 
 *		od    - output image from this filter
 *		height - image height
 *		width - image original width
 *		Nwidth - Padded width
 * 		a0, a1, a2, a3, b1, b2, cprev, cnext: Gaussian coefficients
 */
void gaussianHorizontal_sse(float *oTemp,  unsigned int* id, float *od, int width, int height, int Nwidth, float *a0, float *a1, float *a2, float *a3, float *b1, float *b2, float *cprev, float *cnext )
{
	__m128 prevIn, currIn, prevOut, prev2Out, prevInN, currInN,  prevOutN, prev2OutN, coeft, coefa0, coefa1, coefb1, coefb2;
	
   // edge calculations
	coeft = _mm_load_ss((float *) cprev);
	coeft = _mm_shuffle_ps(coeft, coeft, 0x00);
	prevIn  = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(*(__m128i *) (id)));
	prev2Out = _mm_mul_ps(prevIn,  coeft);
	prevOut = prev2Out;
	
	prevInN  =  _mm_cvtepi32_ps(_mm_cvtepu8_epi32(*(__m128i *) (id+Nwidth)));
	prev2OutN = _mm_mul_ps(prevInN, coeft);
	prevOutN = prev2OutN;

	// load the coefficients
	coefa0 = _mm_load_ss((float *) a0);
	coefa0 = _mm_shuffle_ps(coefa0, coefa0, 0x00);
	coefa1 = _mm_load_ss((float *) a1);
	coefa1 = _mm_shuffle_ps(coefa1, coefa1, 0x00);
	coefb1 = _mm_load_ss((float *) b1);
	coefb1 = _mm_shuffle_ps(coefb1, coefb1, 0x00);
	coefb2 = _mm_load_ss((float *) b2);
	coefb2 = _mm_shuffle_ps(coefb2, coefb2, 0x00);

    for (int y = 0; y < width; y++) {
		// Load 2 input pixels from adjacent rows
	  __m128 currIn  =_mm_cvtepi32_ps(_mm_cvtepu8_epi32(*(__m128i *) (id)));
	  __m128 currInN  =_mm_cvtepi32_ps(_mm_cvtepu8_epi32(*(__m128i *) (id+Nwidth)));

	  __m128 currComp = _mm_mul_ps(currIn, coefa0);
	  __m128 currCompN = _mm_mul_ps(currInN, coefa0); 
	  __m128 temp1 = _mm_mul_ps(prevIn, coefa1);
	  __m128 temp1N = _mm_mul_ps(prevInN, coefa1);
	  __m128 temp2 = _mm_mul_ps(prevOut,coefb1);
	  __m128 temp2N = _mm_mul_ps(prevOutN, coefb1);
	  __m128 temp3 = _mm_mul_ps(prev2Out, coefb2);
	  __m128 temp3N = _mm_mul_ps(prev2OutN, coefb2);
	  currComp = _mm_add_ps(currComp, temp1);
	  currCompN = _mm_add_ps(currCompN, temp1N);
	  temp2 = _mm_add_ps(temp2, temp3);
	  temp2N = _mm_add_ps(temp2N, temp3N);
	  prev2Out = prevOut;
	  prevOut= _mm_sub_ps(currComp, temp2);
	  prev2OutN = prevOutN;
	  prevOutN= _mm_sub_ps(currCompN, temp2N);
	  prevIn = currIn;
	  prevInN = currInN;
	   //store output data
	  _mm_store_ps((float *)(oTemp), prevOut);
	  _mm_store_ps((float *)(oTemp+4), prevOutN);
	  	  	 
	   id += 1 ; oTemp += 8;    // move to next element
    }

    // right-to-left horizontal pass
	// reset pointers
    id -= 1;
 
	od += 4*height*(width-1); //set to last row.
	oTemp -=8;

	// edge calculations
	coeft = _mm_load_ss((float *) cnext);
	coeft = _mm_shuffle_ps(coeft, coeft, 0x00);
    prevIn  = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(*(__m128i *)(id)));
	prev2Out = _mm_mul_ps(prevIn, coeft);
	prevOut = prev2Out;
    currIn = prevIn;

	prevInN  =  _mm_cvtepi32_ps(_mm_cvtepu8_epi32(*(__m128i *)  (id+Nwidth)));
	prev2OutN = _mm_mul_ps(prevInN, coeft);
	prevOutN = prev2OutN;
    currInN = prevInN;

	// load the coefficients
	coefa0 = _mm_load_ss((float *) a2);
	coefa0 = _mm_shuffle_ps(coefa0, coefa0, 0x00);
	coefa1 = _mm_load_ss((float *) a3);
	coefa1 = _mm_shuffle_ps(coefa1, coefa1, 0x00);

    for (int y = width-1; y >= 0; y--) {
		// load inputs 
	   __m128 inNext  = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(*(__m128i *) (id)));
	   __m128 inNextN  = _mm_cvtepi32_ps(_mm_cvtepu8_epi32(*(__m128i *) (id+Nwidth)));

	  __m128 output = _mm_load_ps((float *) (oTemp)); // Current output
	  __m128 outputN = _mm_load_ps((float *) (oTemp+4)); // Current output 

	  __m128 currComp = _mm_mul_ps(currIn, coefa0);
	  __m128 temp1 = _mm_mul_ps(prevIn, coefa1);
	  __m128 temp2 = _mm_mul_ps(prevOut, coefb1);
	  __m128 temp3 = _mm_mul_ps(prev2Out, coefb2);
	  currComp = _mm_add_ps(currComp, temp1);
	  temp2 = _mm_add_ps(temp2, temp3);
	  prev2Out = prevOut;	 
	  prevOut= _mm_sub_ps(currComp, temp2);
	  prevIn = currIn;
	  currIn = inNext;
	   // add previous pass output
	  output = _mm_add_ps(output, prevOut);	
	
	  //next set
      currComp = _mm_mul_ps(currInN, coefa0);
	  temp1 = _mm_mul_ps(prevInN, coefa1);
	  temp2 = _mm_mul_ps(prevOutN, coefb1);
	  temp3 = _mm_mul_ps(prev2OutN, coefb2);
	  currComp = _mm_add_ps(currComp, temp1);
	  temp2 = _mm_add_ps(temp2, temp3);
	  prev2OutN = prevOutN;	 
	  prevOutN = _mm_sub_ps(currComp, temp2);
	  prevInN = currInN;
	  currInN = inNextN;
	  // add previous pass output
	  outputN = _mm_add_ps(outputN, prevOutN);
	  _mm_store_ps((float *)(od), output);
	  _mm_store_ps((float *)(od+4), outputN);

	   id -= 1; od -= 4*height; 
	   oTemp -=8;
	} 
	_mm256_zeroupper(); 
}


/* AVX Implementation: gaussianHorizontal_avx
 *		oTemp - Temporary small buffer used between left to right pass
 *		id    - input image 
 *		od    - output image from this filter
 *		height - image height
 *		width - image width
 * 		a0, a1, a2, a3, b1, b2, cprev, cnext: Gaussian coefficients
 */
void gaussianVertical_avx(float *oTemp, float* id,  unsigned int *od, int height,int width, float *a0, float *a1, float *a2, float *a3, float *b1, float *b2, float *cprev, float *cnext )
{
	__m256 prevIn, currIn,  prevOut, prev2Out, prevInN, currInN,  prevOutN, prev2OutN;
	// Edge calculations
 	prevIn  = _mm256_castps128_ps256(_mm_loadu_ps((float *) id));
	prevIn = _mm256_insertf128_ps(prevIn, _mm_loadu_ps((float *)(id+4*height)), 0x1);
	prev2Out = _mm256_mul_ps(prevIn, _mm256_broadcast_ss((float *) cprev));
	prevOut = prev2Out;
	prevInN  = _mm256_castps128_ps256(_mm_loadu_ps((float *) (id+8*height)));
	prevInN = _mm256_insertf128_ps(prevInN, _mm_loadu_ps((float *)(id+12*height)), 0x1);
	
	prev2OutN = _mm256_mul_ps(prevInN, _mm256_broadcast_ss((float *) cprev));
	prevOutN = prev2OutN;
	
	// it is widthwise as input is transposed.
    for (int y = 0; y < height; y++) {
		// Unpack 2 input pixels
		currIn  = _mm256_castps128_ps256(_mm_loadu_ps((float *) id));
		currIn = _mm256_insertf128_ps(currIn, _mm_loadu_ps((float *)(id+4*height)), 0x1);
	 			
		// Unpack 2 more input pixels
		currInN  = _mm256_castps128_ps256(_mm_loadu_ps((float *) (id+8*height)));
		currInN = _mm256_insertf128_ps(currInN, _mm_loadu_ps((float *)(id+12*height)), 0x1);
		//Process first 2 pixels
	 	__m256 currComp = _mm256_mul_ps(currIn, _mm256_broadcast_ss((float *)a0));
	  	__m256 temp1 = _mm256_mul_ps(prevIn, _mm256_broadcast_ss((float *)a1));
	  	__m256 temp2 = _mm256_mul_ps(prevOut, _mm256_broadcast_ss((float *)b1));
	 	__m256 temp3 = _mm256_mul_ps(prev2Out, _mm256_broadcast_ss((float *)b2));
	  	currComp = _mm256_add_ps(currComp, temp1);
	  	temp2 = _mm256_add_ps(temp2, temp3);
	  	prev2Out = prevOut;
	 	prevOut= _mm256_sub_ps(currComp, temp2);
	  	prevIn = currIn;
		// Store output in temp buffer
	    _mm256_store_ps((float *)(oTemp), prevOut);

	  	//Process next 2 pixels
	    currComp = _mm256_mul_ps(currInN, _mm256_broadcast_ss((float *)a0));
	  	temp1 = _mm256_mul_ps(prevInN, _mm256_broadcast_ss((float *)a1));
	  	temp2 = _mm256_mul_ps(prevOutN, _mm256_broadcast_ss((float *)b1));
	  	temp3 = _mm256_mul_ps(prev2OutN, _mm256_broadcast_ss((float *)b2));
	    currComp = _mm256_add_ps(currComp, temp1);
	  	temp2 = _mm256_add_ps(temp2, temp3);
	  	prev2OutN = prevOutN;
	  	prevOutN= _mm256_sub_ps(currComp, temp2);
	 	prevInN = currInN;
		_mm256_store_ps((float *)(oTemp+8), prevOutN);
	    
		id += 4; oTemp += 16;    // move to next row
   } //height = j loop
  
	// start right-to-left pass
	id -= 4;
	oTemp -=16;
	od += width*(height-1); //set to last row.
	
	//edge caculations
	prevIn  = _mm256_castps128_ps256(_mm_loadu_ps((float *) id));
	prevIn = _mm256_insertf128_ps(prevIn, _mm_loadu_ps((float *)(id+4*height)), 0x1);
	currIn = prevIn;
	prev2Out = _mm256_mul_ps(prevIn, _mm256_broadcast_ss((float *) cnext));
	prevOut = prev2Out;
	prevInN  = _mm256_castps128_ps256(_mm_loadu_ps((float *) (id+8*height)));
	prevInN = _mm256_insertf128_ps(prevInN, _mm_loadu_ps((float *)(id+12*height)), 0x1);
	currInN = prevInN;
	prev2OutN = _mm256_mul_ps(prevInN, _mm256_broadcast_ss((float *) cnext));
	prevOutN = prev2OutN;

    for (int y = height-1; y >= 0; y--) {
		// load 2 input pixel
		__m256 inNext  = _mm256_castps128_ps256(_mm_loadu_ps((float *) id));
		inNext = _mm256_insertf128_ps(inNext, _mm_loadu_ps((float *)(id+4*height)), 0x1);
		
		// load 2 more input pixel
		__m256 inNextN  = _mm256_castps128_ps256(_mm_loadu_ps((float *) (id+8*height)));
		inNextN = _mm256_insertf128_ps(inNextN, _mm_loadu_ps((float *)(id+12*height)), 0x1);

		//get output from left-to-right pass
		__m256 output = _mm256_load_ps((float *) (oTemp)); 
		  	
		__m256 currComp = _mm256_mul_ps(currIn, _mm256_broadcast_ss((float *)a2));
	  	__m256 temp1 = _mm256_mul_ps(prevIn, _mm256_broadcast_ss((float *)a3));
	  	__m256 temp2 = _mm256_mul_ps(prevOut, _mm256_broadcast_ss((float *)b1));
	  	__m256 temp3 = _mm256_mul_ps(prev2Out, _mm256_broadcast_ss((float *)b2));
	  	currComp = _mm256_add_ps(currComp, temp1);
	  	temp2 = _mm256_add_ps(temp2, temp3);
	  	prev2Out = prevOut;
	  	prevOut= _mm256_sub_ps(currComp, temp2);
	  	prevIn = currIn;
	  	currIn = inNext;
	
		//add currently computed output left-to-right pass output
	  	output = _mm256_add_ps(output, prevOut);

		output  = _mm256_castsi256_ps(_mm256_cvttps_epi32(output));

		__m128i output2 =  _mm_castps_si128(_mm256_extractf128_ps(output, 1));
	    __m128i  output1 = _mm_castps_si128(_mm256_castps256_ps128(output));

	  	 //get the left-to-right pass output
		__m256 outputN = _mm256_load_ps((float *) (oTemp+8));

	   	currComp = _mm256_mul_ps(currInN, _mm256_broadcast_ss((float *)a2));
	   	temp1 = _mm256_mul_ps(prevInN, _mm256_broadcast_ss((float *)a3));
	   	temp2 = _mm256_mul_ps(prevOutN, _mm256_broadcast_ss((float *)b1));
	   	temp3 = _mm256_mul_ps(prev2OutN, _mm256_broadcast_ss((float *)b2));
	  	currComp = _mm256_add_ps(currComp, temp1);
	  	temp2 = _mm256_add_ps(temp2, temp3);
	  	prev2OutN = prevOutN;
	  	prevOutN= _mm256_sub_ps(currComp, temp2);
	  	prevInN = currInN;
	  	currInN = inNextN;
	  	outputN = _mm256_add_ps(outputN, prevOutN);
		outputN  = _mm256_castsi256_ps(_mm256_cvttps_epi32(outputN));  
	
	  //convert to RGBA
	  __m128i  output4 = _mm_castps_si128(_mm256_extractf128_ps(outputN, 1));
	  __m128i  output3 = _mm_castps_si128(_mm256_castps256_ps128(outputN));
	   
	  output1 = _mm_packus_epi32(output1, output2);
	  output3 = _mm_packus_epi32(output3, output4);
	  output1 = _mm_packus_epi16(output1, output3);

	  _mm_store_si128((__m128i *)(od), output1);
	 
	   id -= 4; od -= width; oTemp -=16; // move to previous row
    } //height = j loop 
	_mm256_zeroupper();
} // gaussianvertical AVX


/* AVX Implementation: gaussianHorizontal_avx
 *		oTemp - Temporary small buffer used between left to right pass
 *		id    - input image 
 *		od    - output image from this filter
 *		height - image height
 *		width - image original width
 *		Nwidth - Padded width
 * 		a0, a1, a2, a3, b1, b2, cprev, cnext: Gaussian coefficients
 */
void gaussianHorizontal_avx(float *oTemp,  unsigned int* id, float *od, int width, int height, int Nwidth, float *a0, float *a1, float *a2, float *a3, float *b1, float *b2, float *cprev, float *cnext )
{
	__m256 prevIn, currIn, prevOut, prev2Out, prevInN, currInN,  prevOutN, prev2OutN;
	
   // edge calculations
	prevIn  = _mm256_castsi256_ps(_mm256_castsi128_si256(_mm_cvtepu8_epi32(*(__m128i *) (id))));
	prevIn = _mm256_insertf128_ps(prevIn, _mm_castsi128_ps(_mm_cvtepu8_epi32((*(__m128i *)(id+Nwidth)))), 0x1);
	prevIn = _mm256_cvtepi32_ps(_mm256_castps_si256(prevIn));
	prev2Out = _mm256_mul_ps(prevIn, _mm256_broadcast_ss((float *) cprev));
	prevOut = prev2Out;
	
	prevInN  =  _mm256_castsi256_ps(_mm256_castsi128_si256(_mm_cvtepu8_epi32(*(__m128i *) (id+2*Nwidth))));
	prevInN = _mm256_insertf128_ps(prevInN, _mm_castsi128_ps(_mm_cvtepu8_epi32((*(__m128i *)(id+3*Nwidth)))), 0x01);
	prevInN = _mm256_cvtepi32_ps(_mm256_castps_si256(prevInN));
	prev2OutN = _mm256_mul_ps(prevInN, _mm256_broadcast_ss((float *) cprev));
	prevOutN = prev2OutN;

    for (int y = 0; y < width; y++) {
		// Load 2 input pixels from adjacent rows
	  __m256 currIn  = _mm256_castsi256_ps(_mm256_castsi128_si256(_mm_cvtepu8_epi32(*(__m128i *) (id))));
	  currIn = _mm256_insertf128_ps(currIn, _mm_castsi128_ps(_mm_cvtepu8_epi32((*(__m128i *)(id+Nwidth)))), 0x1);
	  currIn = _mm256_cvtepi32_ps(_mm256_castps_si256(currIn));
	  // Load 2 more input pixels from next adjacent rows
	  __m256 currInN  =_mm256_castsi256_ps(_mm256_castsi128_si256(_mm_cvtepu8_epi32(*(__m128i *) (id+2*Nwidth))));
	  currInN = _mm256_insertf128_ps(currInN, _mm_castsi128_ps(_mm_cvtepu8_epi32((*(__m128i *)(id+3*Nwidth)))), 0x1);
	  currInN = _mm256_cvtepi32_ps(_mm256_castps_si256(currInN));

	  __m256 currComp = _mm256_mul_ps(currIn, _mm256_broadcast_ss((float *)a0));
	  __m256 currCompN = _mm256_mul_ps(currInN, _mm256_broadcast_ss((float *)a0)); 
	  __m256 temp1 = _mm256_mul_ps(prevIn, _mm256_broadcast_ss((float *)a1));
	   __m256 temp1N = _mm256_mul_ps(prevInN, _mm256_broadcast_ss((float *)a1));
	  __m256 temp2 = _mm256_mul_ps(prevOut, _mm256_broadcast_ss((float *)b1));
	    __m256 temp2N = _mm256_mul_ps(prevOutN, _mm256_broadcast_ss((float *)b1));
	  __m256 temp3 = _mm256_mul_ps(prev2Out, _mm256_broadcast_ss((float *)b2));
	   __m256 temp3N = _mm256_mul_ps(prev2OutN, _mm256_broadcast_ss((float *)b2));
	  currComp = _mm256_add_ps(currComp, temp1);
	  currCompN = _mm256_add_ps(currCompN, temp1N);
	  temp2 = _mm256_add_ps(temp2, temp3);
	  temp2N = _mm256_add_ps(temp2N, temp3N);
	  prev2Out = prevOut;
	  prevOut= _mm256_sub_ps(currComp, temp2);
	  prev2OutN = prevOutN;
	  prevOutN= _mm256_sub_ps(currCompN, temp2N);
	  prevIn = currIn;
	  prevInN = currInN;
	   //store output data
	  _mm256_store_ps((float *)(oTemp), prevOut);
	  _mm256_store_ps((float *)(oTemp+8), prevOutN);
	   id += 1 ; oTemp += 16;    // move to next element
    }

    // right-to-left horizontal pass
	// reset pointers
    id -= 1;
	od += 4*height*(width-1); //set to last row.
	oTemp -=16;

	// edge calculations
    prevIn  = _mm256_castsi256_ps(_mm256_castsi128_si256(_mm_cvtepu8_epi32(*(__m128i *) (id))));
	prevIn = _mm256_insertf128_ps(prevIn, _mm_castsi128_ps(_mm_cvtepu8_epi32((*(__m128i *)(id+Nwidth)))), 0x1);
	prevIn = _mm256_cvtepi32_ps(_mm256_castps_si256(prevIn));
	prev2Out = _mm256_mul_ps(prevIn, _mm256_broadcast_ss((float *) cnext));
	prevOut = prev2Out;
    currIn = prevIn;

	prevInN  =  _mm256_castsi256_ps(_mm256_castsi128_si256(_mm_cvtepu8_epi32(*(__m128i *) (id+2*Nwidth))));
	prevInN = _mm256_insertf128_ps(prevInN, _mm_castsi128_ps(_mm_cvtepu8_epi32((*(__m128i *)(id+3*Nwidth)))), 0x01);
	prevInN = _mm256_cvtepi32_ps(_mm256_castps_si256(prevInN));
	prev2OutN = _mm256_mul_ps(prevInN, _mm256_broadcast_ss((float *) cnext));
	prevOutN = prev2OutN;
    currInN = prevInN;

    for (int y = width-1; y >= 0; y--) {
		// load inputs 
	   __m256 inNext  = _mm256_castsi256_ps(_mm256_castsi128_si256(_mm_cvtepu8_epi32(*(__m128i *) (id))));
	   inNext = _mm256_insertf128_ps(inNext, _mm_castsi128_ps(_mm_cvtepu8_epi32((*(__m128i *)(id+Nwidth)))), 0x1);
	   inNext = _mm256_cvtepi32_ps(_mm256_castps_si256(inNext));

	   __m256 inNextN  = _mm256_castsi256_ps(_mm256_castsi128_si256(_mm_cvtepu8_epi32(*(__m128i *) (id+2*Nwidth))));
	   inNextN = _mm256_insertf128_ps(inNextN, _mm_castsi128_ps(_mm_cvtepu8_epi32((*(__m128i *)(id+3*Nwidth)))), 0x01);
	   inNextN = _mm256_cvtepi32_ps(_mm256_castps_si256(inNextN));
	 	//get the current output from temp buffer
	   __m256 output = _mm256_load_ps((float *) (oTemp)); // Current output
	   __m256 outputN = _mm256_load_ps((float *) (oTemp+8)); // Current output 

	  __m256 currComp = _mm256_mul_ps(currIn, _mm256_broadcast_ss((float *)a2));
	  __m256 temp1 = _mm256_mul_ps(prevIn, _mm256_broadcast_ss((float *)a3));
	  __m256 temp2 = _mm256_mul_ps(prevOut, _mm256_broadcast_ss((float *)b1));
	  __m256 temp3 = _mm256_mul_ps(prev2Out, _mm256_broadcast_ss((float *)b2));
	  currComp = _mm256_add_ps(currComp, temp1);
	  temp2 = _mm256_add_ps(temp2, temp3);
	  prev2Out = prevOut;	 
	  prevOut= _mm256_sub_ps(currComp, temp2);
	  prevIn = currIn;
	  currIn = inNext;
	   // add previous pass output
	  output = _mm256_add_ps(output, prevOut);	

	  //next set
      currComp = _mm256_mul_ps(currInN, _mm256_broadcast_ss((float *)a2));
	  temp1 = _mm256_mul_ps(prevInN, _mm256_broadcast_ss((float *)a3));
	  temp2 = _mm256_mul_ps(prevOutN, _mm256_broadcast_ss((float *)b1));
	  temp3 = _mm256_mul_ps(prev2OutN, _mm256_broadcast_ss((float *)b2));
	  currComp = _mm256_add_ps(currComp, temp1);
	  temp2 = _mm256_add_ps(temp2, temp3);
	  prev2OutN = prevOutN;	 
	  prevOutN = _mm256_sub_ps(currComp, temp2);
	  prevInN = currInN;
	  currInN = inNextN;
	  // add previous pass output
	  outputN = _mm256_add_ps(outputN, prevOutN);
	
	  _mm256_stream_ps((float *)(od), output);
	  _mm256_stream_ps((float *)(od+8), outputN);

	   id -= 1; od -= 4*height; 
	   oTemp -=16;
	} 
	_mm256_zeroupper(); 
}
/* Calcualte Gaussian Blur Filter Coefficiens
 *  alpha -> smooting gradient depends on sigma
 *  k = ((1-e^-alpha)^2)/(1+2*alpha*e^-alpha - e^-2alpha)
 *  a0 = k; a1 = k*(alpha-1)*e^-alpha; a2 = k*(alpha+1)*e^-alpha; a3 = -k*e^(-2*alpha)
 *  b1 = -2*e^-alpha; b2 = e^(-2*alpha)
 */
void calGaussianCoeff( float sigma,  float *a0, float *a1, float *a2, float *a3, float *b1, float *b2, float *cprev, float *cnext)
{
  float alpha, lamma,  k; 
  // defensive check
  if (sigma < 0.5f)
	  sigma = 0.5f;

  alpha = (float) exp((0.726)*(0.726)) / sigma;
  lamma = (float)exp(-alpha);
  *b2 = (float)exp(-2*alpha);
  k = (1-lamma)*(1-lamma)/(1+2*alpha*lamma- (*b2));
  *a0 = k;
  *a1 = k*(alpha-1)*lamma;
  *a2 = k*(alpha+1)*lamma;
  *a3 = -k* (*b2);
  *b1 = -2*lamma;
  *cprev = (*a0 + *a1)/(1+ *b1 + *b2);
  *cnext = (*a2 + *a3)/(1+ *b1 + *b2);
}

void writeBMP( unsigned char* image, unsigned char* header, int height, int width,int  Nwidth, const char* name ) {

  //RGB
 int numPixels = height*Nwidth;
  unsigned char* tmp = (unsigned char*) malloc( numPixels * 3);
  int i =0;
  int k = 0;
 
  for( int i = 0; i < height; i++ ) {
	  int j = i*Nwidth;
	   for(j; j < (i*Nwidth+width); j++, k++) {
		   tmp[k*3] = image[j*4];
   		   tmp[k*3+1] = image[j*4+1];
    	   tmp[k*3+2] = image[j*4+2];
	   }
  }
  FILE* out = fopen(name, "wb");
  if( out == NULL ) {
    fprintf(stderr, "Can't open output image %s", name);
  }
  if (fwrite(header, 1, 54, out) != 54) {
    fprintf(stderr, "Can't write header %s", name);
  }

  if (fwrite(tmp, 1, numPixels * 3, out) != numPixels * 3) {
    fprintf(stderr, "Write to the output image %s failed", name);
  }
  fclose( out);
  free( tmp );
}
// Main IIR Gaussian Blur Filter
int main( int argc, char* argv[] ) {

  FILE * in;
  unsigned int  width = 0;
  unsigned int  height = 0;
  unsigned int  numPixels = 0;
  unsigned char header[54];
  float sigma; 
  int order = 0;
  unsigned int *img0m;
  float *img1SSEm;
  unsigned int *img2SSEm;
  float *img1AVXm;
  unsigned int *img2AVXm;

  //aligned pointers
  unsigned int *img0;
  float *img1SSE;
  unsigned int *img2SSE;
  float *img1AVX;
  unsigned int *img2AVX;

  int test = 1;
  int status = 0;


  if( argc != 2 ) {
    perror("Usage: gaussian_blur <image_name>");
    exit(1);
  }

  in = fopen(argv[1], "rb"); 
  if (in == NULL) {
    perror("Could not open image");
    return 1;
  }
  
  if (fread(header, 1, 54, in) != 54) {
    perror("Could not read image header");
    return 2;
  }
  
  width = abs(*(short *)&header[18]);
  height = abs(*(short *)&header[22]);

  
  if((width & 7) || (height & 7)) {
    printf("Only images with width and height a multiple of 8 are supported\n");
    return 3;
  }
  //Increase image/intermediate image width to avoid cache thrash by atlease one cacheline
  // New image width -Nwidth
  int Nwidth = width+32;
  int buffWidth = Nwidth*height;
  numPixels = width * height;

  img0m = (unsigned int*) malloc( (buffWidth * 4) + (height*64));
  img1SSEm = (float*) malloc((buffWidth *4*sizeof(float)) + (height*64));
  img2SSEm = (unsigned int*) malloc((buffWidth*4) + (height*64));
  img1AVXm = (float*) malloc( (buffWidth * 4*sizeof(float)) + (height*64));
  img2AVXm = (unsigned int*) malloc( (buffWidth * 4) + (height*64));
 
  img0 = (unsigned int*) (((long long) img0m + 64) + ~(63));
  img1SSE = (float*)(((long long) img1SSEm + 64) + ~(63));
  img2SSE = (unsigned int*) (((long long) img2SSEm + 64) + ~(63));
  img1AVX = (float *)(((long long) img1AVXm + 64) + ~(63));
  img2AVX = (unsigned int*)(((long long) img2AVXm + 64) + ~(63)); 
 
  // check AVX support

  if (avx_support() == 0)
  {
	  printf("\n The OS or HW does not support AVX\n");
	  return 0;
  }
  else {
	  printf("\n The computer supports AVX\n");
   }
   SYSTEM_INFO sysInfo;
  GetSystemInfo( &sysInfo );
  int nCPU = sysInfo.dwNumberOfProcessors;
  printf("number CPU in the system = %d \n",nCPU); 
  num_thread = nCPU;
  if(num_thread > MAX_THREAD)
		num_thread = MAX_THREAD;

 // Temp buffer initializtion, used inbetween passes.
  int buffer_size_each_thread = width*4*4; // as image is symmetric width=height, this calculation works, otherwise
											 // for each pass there should be separate temp buffer.
  float *oTempm = (float*) malloc(buffer_size_each_thread*num_thread*sizeof(float) +64); // temp buffer to store data between passes - 16- 4 threads
  float *oTemp = (float *)(((long long) oTempm + 64) + ~(63));

 
  unsigned char* tmp = (unsigned char*) malloc( numPixels * 3);
  if (fread(tmp, 1, numPixels * 3, in) != numPixels * 3) {
    perror("can't read input image");
    return 4;
  }
  fclose(in); 

  
   // copy the RGB values from the image, pad the 4th byte with zero 
  // so we get RGBA
  unsigned char* img0byte = (unsigned char*) img0;
  // copy image to the buffer
  unsigned int i = 0;
  unsigned int k =0;
  for(unsigned int j = 0; j < height; j++ ) {		
	k = j*(width);
	for( i; i < (j*(Nwidth)+ width); i++, k++ ) {
   		img0byte[i*4] = tmp[k*3];
    	img0byte[i*4+1] = tmp[k*3+1];
    	img0byte[i*4+2] = tmp[k*3+2];
    	img0byte[i*4+3] = 0;
	}		 
	for(; i <( j*(Nwidth)+ Nwidth); i++ ) {
		img0byte[i*4] = 0;
    	img0byte[i*4+1] =0;
    	img0byte[i*4+2] = 0;
    	img0byte[i*4+3] = 0;
	}

  } // for loop copy image to the buffer
  
  header[18] = (short) width;
  writeBMP( (unsigned char*) img0, header,  height, width, Nwidth, "original Image.bmp");
  free(tmp);

  // Calculate IIR Gaussian Filter coefficeints
  sigma = 20.0f;
  calGaussianCoeff(sigma, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext);
  printf("\n IIR Gaussian Filter Coefficients are: \n");
  printf("a0 = %f, a1 = %f, a2 = %f, a3 = %f, b1 = %f, b2 = %f, cprev = %f, cnext = %f\n", a0, a1, a2, a3, b1, b2, cprev, cnext);  
  
  printf("\nimage width = %d, height = %d\n", width, height );

 
 
 // Functional Test
 // SSE implementation
  printf("\nRunning multi threaded SSE code\n");
#pragma omp parallel for
for(int i = 0; i < height; i+=2) {
 		int tidx = omp_get_thread_num();
		gaussianHorizontal_sse( oTemp + tidx*buffer_size_each_thread, &img0[i*Nwidth], &img1SSE[4*i], width, height, Nwidth, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext );
	}


#pragma omp parallel for 
 for (int i = 0; i < width; i+=2) {
	 int tidx = omp_get_thread_num();         
     gaussianVertical_sse( oTemp + tidx*buffer_size_each_thread, &img1SSE[4*i*height], &img2SSE[i],  height, Nwidth, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext );
 }
  
  printf("\nRunning multi threaded AVX code\n");
 // AVX implementation
 _mm256_zeroupper();
#pragma omp parallel for
for(int i = 0; i < height; i+=4) {
 		int tidx = omp_get_thread_num();
		gaussianHorizontal_avx( oTemp + tidx*buffer_size_each_thread, &img0[i*Nwidth], &img1AVX[4*i], width, height, Nwidth, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext ); 
}

#pragma omp parallel for 
 for (int i = 0; i < width; i+=4) {
	 int tidx = omp_get_thread_num();         
     gaussianVertical_avx( oTemp + tidx*buffer_size_each_thread, &img1AVX[4*i*height], &img2AVX[i], height, Nwidth, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext );
 }

 if (test ==1) {
	    // check Error in original and AVX implementation
   int status = 0;
   unsigned char *origbyte = (unsigned char *)img2SSE;
   unsigned char *avxbyte = (unsigned char *)img2AVX;
   
   for( int i = 0; i < (height*Nwidth); i++ ) {
	   //R Plane
	  if (abs((float) (origbyte[i*4] - avxbyte[i*4])) >0)
		  status = 1;		  
	  //G Plane
	  if (abs((float) (origbyte[i*4+1] - avxbyte[i*4+1])))
		   status = 1;
	  //B Plane
	  if (abs((float) (origbyte[i*4+2] - avxbyte[i*4+2])))
	   status = 1;
	}
    if (status ==1)
 	 	printf(" \nSSE and AVX Implementation does not match\n" );
	else
 		printf(" \nSSE and AVX Implementation matches\n" );
  } // test


  writeBMP( (unsigned char*) img2AVX, header,  height, width, Nwidth, "blur_AVX.bmp");
  writeBMP( (unsigned char*) img2SSE, header,  height, width, Nwidth, "blured_sse.bmp");
  
  // Measure Performance
  printf("\n\nPerformance Measurement: \n");
  int numIter = 20000;
  LARGE_INTEGER proc_freq;
  QueryPerformanceFrequency(&proc_freq);
  LARGE_INTEGER startTime;
  QueryPerformanceCounter( &startTime );
  double time;
  LARGE_INTEGER stopTime;

  float times[20000];
  __int64 starttime;
  __int64 endtime;
  
   // SSE Timing
    //horizontal Pass Performance 
 for( int iter = 0; iter < numIter; iter++ ) { 
	QueryPerformanceCounter( (LARGE_INTEGER*)&starttime); 
#pragma omp parallel for schedule(static, 8)
	for(int i = 0; i < height; i+=2) {
 		int tidx = omp_get_thread_num();
		gaussianHorizontal_sse( oTemp + tidx*buffer_size_each_thread, &img0[i*Nwidth], &img1SSE[2*i], width, height, Nwidth, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext );
	}	
	QueryPerformanceCounter( (LARGE_INTEGER*)&endtime);
	endtime -= starttime;
	times[iter] = (float) endtime / (float) proc_freq.QuadPart;
  }  
  
  float minval = 100000000;
  float maxval = 0;
  float average = 0;
  for (int iter=0; iter<numIter; iter++)
  {
	  if (times[iter] > maxval) maxval = times[iter];
	  if (times[iter] < minval) minval = times[iter];

	  average += times[iter];
  }

  average /= numIter;

   printf("\n SSE horizontal Pass min: %g max: %g avg: %g\n", minval*1000, maxval*1000, average*1000);
   
 // Vertical Pass

  for( int iter = 0; iter < numIter; iter++ ) {
  QueryPerformanceCounter( (LARGE_INTEGER*)&starttime); 
#pragma omp parallel for schedule(static, 8)
 for (int i = 0; i < width; i+=2) {
	 int tidx = omp_get_thread_num();         
     gaussianVertical_sse( oTemp + tidx*buffer_size_each_thread, &img1SSE[4*i*height], &img2SSE[i],  height, Nwidth, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext );
  
    }
	QueryPerformanceCounter( (LARGE_INTEGER*)&endtime);
	endtime -= starttime;

	times[iter] = (float) endtime / (float) proc_freq.QuadPart;

  }   
  minval = 100000000;
  maxval = 0;
  average = 0;
  for (int iter=0; iter<numIter; iter++)
  {
	  if (times[iter] > maxval) maxval = times[iter];
	  if (times[iter] < minval) minval = times[iter];

	  average += times[iter];
  }

  average /= numIter;

  printf(" SSE vertical Pass min: %g max: %g avg: %g\n", minval*1000, maxval*1000, average*1000);
  

  //AVX Timing
  _mm256_zeroupper();
  // horitzontal Pass Performance 
 for( int iter = 0; iter < numIter; iter++ ) { 
	QueryPerformanceCounter( (LARGE_INTEGER*)&starttime); 
#pragma omp parallel for 
for(int i = 0; i < height; i+=4) {
  		int tidx = omp_get_thread_num();
		gaussianHorizontal_avx( oTemp + tidx*buffer_size_each_thread, &img0[i*Nwidth], &img1AVX[4*i], width, height, Nwidth, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext );
   }	
	QueryPerformanceCounter( (LARGE_INTEGER*)&endtime);
	endtime -= starttime;
	times[iter] = (float) endtime / (float) proc_freq.QuadPart;
  }  
  
  minval = 100000000;
  maxval = 0;
  average = 0;
  for (int iter=0; iter<numIter; iter++)
  {
	  if (times[iter] > maxval) maxval = times[iter];
	  if (times[iter] < minval) minval = times[iter];

	  average += times[iter];
  }

  average /= numIter;

   printf("\n AVX horizontal Pass  min: %g max: %g avg: %g\n", minval*1000, maxval*1000, average*1000);

 // Vertical Pass
 
 for( int iter = 0; iter < numIter; iter++ ) {
  QueryPerformanceCounter( (LARGE_INTEGER*)&starttime); 
 #pragma omp parallel for
 for (int i = 0; i < width; i+=4) {
	 int tidx = omp_get_thread_num();         
     gaussianVertical_avx( oTemp + tidx*buffer_size_each_thread, &img1AVX[4*i*height], &img2AVX[i], height, Nwidth, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext );
  }
	QueryPerformanceCounter( (LARGE_INTEGER*)&endtime);
	endtime -= starttime;

	times[iter] = (float) endtime / (float) proc_freq.QuadPart;

  }   
  minval = 1000;
  maxval = 0;
  average = 0;
  for (int iter=0; iter<numIter; iter++)
  {
	  if (times[iter] > maxval) maxval = times[iter];
	  if (times[iter] < minval) minval = times[iter];

	  average += times[iter];
  }

  average /= numIter;

  printf("\n AVX vertical Pass min:   %g max: %g avg: %g\n", minval*1000, maxval*1000, average*1000);
 	
  free(oTempm);
  free(img0m);
  free(img1SSEm);
  free(img2SSEm);
  free(img1AVXm);
  free(img2AVXm);
  fflush(stdout);

}


