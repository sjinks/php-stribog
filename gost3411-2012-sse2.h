#ifndef GOST3411_2012_SSE2_H_
#define GOST3411_2012_SSE2_H_

#include "gost3411-2012.h"

#if (__x86_64__ || __i386__)
void GOST34112012Update_sse2(void* restrict ctx, const unsigned char* restrict data, size_t len);
void GOST34112012Final_sse2(void* restrict ctx, unsigned char* restrict digest);
#endif

#endif /* GOST3411_2012_SSE2_H_ */
