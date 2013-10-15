#ifndef GOST3411_2012_REF_H
#define GOST3411_2012_REF_H

#include "gost3411-2012.h"

void GOST34112012Update_ref(void* restrict ctx, const unsigned char* restrict data, size_t len);
void GOST34112012Final_ref(void* restrict ctx, unsigned char* restrict digest);

#endif /* GOST3411_2012_REF_H */
