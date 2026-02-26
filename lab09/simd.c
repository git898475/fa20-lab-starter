#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "simd.h"

long long int sum(int vals[NUM_ELEMS]) {
	clock_t start = clock();

	long long int sum = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS; i++) {
			if(vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	long long int sum = 0;

	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			if(vals[i] >= 128) sum += vals[i];
			if(vals[i + 1] >= 128) sum += vals[i + 1];
			if(vals[i + 2] >= 128) sum += vals[i + 2];
			if(vals[i + 3] >= 128) sum += vals[i + 3];
		}

		//This is what we call the TAIL CASE
		//For when NUM_ELEMS isn't a multiple of 4
		//NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
		for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);		// This is a vector with 127s in it... Why might you need this?
	long long int result = 0;				   // This is where you should put your final result!
	/* DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE. */
	
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* YOUR CODE GOES HERE */
		__m128i sum_1 = _mm_setzero_si128();
		for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4)
		{
			__m128i vect = _mm_loadu_si128((__m128i *)(vals + i));
			__m128i cmp = _mm_cmpgt_epi32(vect, _127);
			vect = _mm_and_si128(vect, cmp);
			sum_1 = _mm_add_epi32(vect, sum_1);
		}
		int sum[4];
		_mm_storeu_si128((__m128i *)sum, sum_1);
		for (unsigned int i = 0; i < 4;i++){
			result += sum[i];
		}
		for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS;i++){
			if (vals[i] > 127)
			{
				result += vals[i];
			}
		}
		/* You'll need a tail case. */
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

//loop unrolling 用于超标量执行
long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);
	long long int result = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* COPY AND PASTE YOUR sum_simd() HERE */
		/* MODIFY IT BY UNROLLING IT */
		__m128i sum_1 = _mm_setzero_si128();
		__m128i sum_2 = _mm_setzero_si128();
		__m128i sum_3 = _mm_setzero_si128();
		__m128i sum_4 = _mm_setzero_si128();
		for (unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i += 16)
		{
			__m128i vect_1 = _mm_loadu_si128((__m128i *)(vals + i));
			__m128i vect_2 = _mm_loadu_si128((__m128i *)(vals + i + 4));
			__m128i vect_3 = _mm_loadu_si128((__m128i *)(vals + i + 8));
			__m128i vect_4 = _mm_loadu_si128((__m128i *)(vals + i + 12));
			__m128i cmp_1 = _mm_cmpgt_epi32(vect_1, _127);
			__m128i cmp_2 = _mm_cmpgt_epi32(vect_2, _127);
			__m128i cmp_3 = _mm_cmpgt_epi32(vect_3, _127);
			__m128i cmp_4 = _mm_cmpgt_epi32(vect_4, _127);
			vect_1 = _mm_and_si128(vect_1, cmp_1);
			vect_2 = _mm_and_si128(vect_2, cmp_2);
			vect_3 = _mm_and_si128(vect_3, cmp_3);
			vect_4 = _mm_and_si128(vect_4, cmp_4);
			sum_1 = _mm_add_epi32(vect_1, sum_1);
			sum_2 = _mm_add_epi32(vect_2, sum_2);
			sum_3 = _mm_add_epi32(vect_3, sum_3);
			sum_4 = _mm_add_epi32(vect_4, sum_4);
		}
		sum_1 = _mm_add_epi32(sum_1, sum_2);
		sum_3 = _mm_add_epi32(sum_3, sum_4);
		sum_1 = _mm_add_epi32(sum_1, sum_3);
		int sum_arr[4];
		_mm_storeu_si128((__m128i *)sum_arr, sum_1);
		result += sum_arr[0];
		result += sum_arr[1];
		result += sum_arr[2];
		result += sum_arr[3];
		for (unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS;i++){
			if (vals[i] > 127)
			{
				result += vals[i];
			}
		}
		/* You'll need 1 or maybe 2 tail cases here. */

	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}