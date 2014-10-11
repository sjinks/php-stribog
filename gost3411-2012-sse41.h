#ifndef GOST3411_2012_SSE41_H_
#define GOST3411_2012_SSE41_H_

#include "gost3411-2012.h"

void GOST34112012Update_sse41(void* restrict ctx, const unsigned char* restrict data, size_t len);
void GOST34112012Final_sse41(void* restrict ctx, unsigned char* restrict digest);

#endif /* GOST3411_2012_SSE41_H_ */
