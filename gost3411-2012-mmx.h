#ifndef GOST3411_2012_MMX_H
#define GOST3411_2012_MMX_H

#include "gost3411-2012.h"

#if (__x86_64__ || __i386__)
void GOST34112012Update_mmx(void* restrict ctx, const unsigned char* restrict data, size_t len);
void GOST34112012Final_mmx(void* restrict ctx, unsigned char* restrict digest);
#endif

#endif /* GOST3411_2012_MMX_H */
