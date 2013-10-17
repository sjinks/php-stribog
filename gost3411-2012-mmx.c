#pragma GCC target ("mmx")

#ifndef __MMX__
#define __MMX__
#endif

#include <mmintrin.h>
#include "tables.h"
#include "gost3411-2012-mmx.h"

static inline __m64 i64_to_m64(uint64_t x)
{
#ifndef __x86_64__
	return _mm_setr_pi32((uint32_t)x, (uint32_t)(x >> 32));
#else
	return _mm_cvtsi64_m64(x);
#endif
}

static inline uint64_t m64_to_i64(__m64 x)
{
#ifndef __x86_64__
	return (uint64_t)x;
#else
	return _mm_cvtm64_si64(x);
#endif
}

static inline void xtranspose(const union uint512_u* x, const union uint512_u* y, union uint512_u* z)
{
	__m64 mm0, mm1, mm2, mm3, mm4, mm5, mm6, mm7;
	__m64 tm0, tm1, tm2, tm3, tm4, tm5, tm6, tm7;

	/* load and XOR */
	mm0 = _mm_xor_si64(i64_to_m64(x->QWORD[0]), i64_to_m64(y->QWORD[0]));
	mm1 = _mm_xor_si64(i64_to_m64(x->QWORD[1]), i64_to_m64(y->QWORD[1]));
	mm2 = _mm_xor_si64(i64_to_m64(x->QWORD[2]), i64_to_m64(y->QWORD[2]));
	mm3 = _mm_xor_si64(i64_to_m64(x->QWORD[3]), i64_to_m64(y->QWORD[3]));
	mm4 = _mm_xor_si64(i64_to_m64(x->QWORD[4]), i64_to_m64(y->QWORD[4]));
	mm5 = _mm_xor_si64(i64_to_m64(x->QWORD[5]), i64_to_m64(y->QWORD[5]));
	mm6 = _mm_xor_si64(i64_to_m64(x->QWORD[6]), i64_to_m64(y->QWORD[6]));
	mm7 = _mm_xor_si64(i64_to_m64(x->QWORD[7]), i64_to_m64(y->QWORD[7]));

	/* transpose */
	tm0 = _mm_unpacklo_pi8(mm0, mm2);
	tm1 = _mm_unpackhi_pi8(mm0, mm2);
	tm2 = _mm_unpacklo_pi8(mm1, mm3);
	tm3 = _mm_unpackhi_pi8(mm1, mm3);
	tm4 = _mm_unpacklo_pi8(mm4, mm6);
	tm5 = _mm_unpackhi_pi8(mm4, mm6);
	tm6 = _mm_unpacklo_pi8(mm5, mm7);
	tm7 = _mm_unpackhi_pi8(mm5, mm7);

	mm0 = _mm_unpacklo_pi8(tm0, tm2);
	mm1 = _mm_unpackhi_pi8(tm0, tm2);
	mm2 = _mm_unpacklo_pi8(tm1, tm3);
	mm3 = _mm_unpackhi_pi8(tm1, tm3);
	mm4 = _mm_unpacklo_pi8(tm4, tm6);
	mm5 = _mm_unpackhi_pi8(tm4, tm6);
	mm6 = _mm_unpacklo_pi8(tm5, tm7);
	mm7 = _mm_unpackhi_pi8(tm5, tm7);

	tm2 = _mm_unpacklo_pi32(mm1, mm5);
	tm3 = _mm_unpackhi_pi32(mm1, mm5);
	tm0 = _mm_unpacklo_pi32(mm0, mm4);
	tm1 = _mm_unpackhi_pi32(mm0, mm4);
	mm4 = _mm_unpacklo_pi32(mm2, mm6);
	mm5 = _mm_unpackhi_pi32(mm2, mm6);
	mm6 = _mm_unpacklo_pi32(mm3, mm7);
	mm7 = _mm_unpackhi_pi32(mm3, mm7);
	mm0 = tm0;
	mm1 = tm1;
	mm2 = tm2;
	mm3 = tm3;

	/* unload */
	z->QWORD[0] = m64_to_i64(mm0);
	z->QWORD[1] = m64_to_i64(mm1);
	z->QWORD[2] = m64_to_i64(mm2);
	z->QWORD[3] = m64_to_i64(mm3);
	z->QWORD[4] = m64_to_i64(mm4);
	z->QWORD[5] = m64_to_i64(mm5);
	z->QWORD[6] = m64_to_i64(mm6);
	z->QWORD[7] = m64_to_i64(mm7);
}

static void XLPS(const union uint512_u* x, const union uint512_u* y, union uint512_u* data)
{
	uint_fast8_t i;
	uint8_t* p;
	ALIGN(16) union uint512_u buf;

	xtranspose(x, y, &buf);
	p = (uint8_t*)&buf;

	for (i=0; i<8; ++i) {
		__m64 mm0;
		mm0 =                   i64_to_m64(Ax[0][*p]);  ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[1][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[2][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[3][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[4][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[5][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[6][*p])); ++p;
		mm0 = _mm_xor_si64(mm0, i64_to_m64(Ax[7][*p])); ++p;
		data->QWORD[i] = m64_to_i64(mm0);
	}
}

static inline void X(const union uint512_u* x, const union uint512_u* y, union uint512_u* z)
{
	z->QWORD[0] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[0]), i64_to_m64(y->QWORD[0])));
	z->QWORD[1] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[1]), i64_to_m64(y->QWORD[1])));
	z->QWORD[2] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[2]), i64_to_m64(y->QWORD[2])));
	z->QWORD[3] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[3]), i64_to_m64(y->QWORD[3])));
	z->QWORD[4] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[4]), i64_to_m64(y->QWORD[4])));
	z->QWORD[5] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[5]), i64_to_m64(y->QWORD[5])));
	z->QWORD[6] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[6]), i64_to_m64(y->QWORD[6])));
	z->QWORD[7] = m64_to_i64(_mm_xor_si64(i64_to_m64(x->QWORD[7]), i64_to_m64(y->QWORD[7])));
}

static inline void add512(const union uint512_u* x, const union uint512_u* y, union uint512_u* r)
{
	uint_fast8_t i, CF, OF;

	CF = 0;
	for (i=0; i<8; ++i) {
		r->QWORD[i]  = x->QWORD[i] + y->QWORD[i];
		OF           = (r->QWORD[i] < y->QWORD[i]) ? 1 : 0;
		r->QWORD[i] += CF;
		CF           = OF;
	}
}

static void g(union uint512_u* restrict h, const union uint512_u* restrict N, const unsigned char* restrict m)
{
	ALIGN(16) union uint512_u Ki, data;
	uint_fast8_t i;

	XLPS(h, N, &data);

	/* Starting E() */
	Ki = data;
	XLPS(&Ki, (const union uint512_u*)&m[0], &data);

	for (i=0; i<11; ++i) {
		XLPS(&Ki, &C[i], &Ki);
		XLPS(&Ki, &data, &data);
	}

	XLPS(&Ki, &C[11], &Ki);
	X(&Ki, &data, &data);
	/* E() done */

	X(&data, h, &data);
	X(&data, (const union uint512_u*)&m[0], h);
}

static inline void stage2(GOST34112012Context* restrict CTX, const unsigned char* restrict data)
{
	g(&CTX->h, &CTX->N, data);

	add512(&CTX->N, &buffer512, &CTX->N);
	add512(&CTX->Sigma, (const union uint512_u*)data, &CTX->Sigma);
}


void GOST34112012Update_mmx(void* restrict ctx, const unsigned char* restrict data, size_t len)
{
	size_t chunksize;
	GOST34112012Context* CTX = (GOST34112012Context*)ctx;

	while (len > 63 && CTX->bufsize == 0) {
		stage2(CTX, data);

		data += 64;
		len  -= 64;
	}

	while (len) {
		chunksize = 64 - CTX->bufsize;
		if (chunksize > len) {
			chunksize = len;
		}

		memcpy(&CTX->buffer[CTX->bufsize], data, chunksize);

		CTX->bufsize += chunksize;
		len          -= chunksize;
		data         += chunksize;

		if (CTX->bufsize == 64) {
			stage2(CTX, CTX->buffer);
			CTX->bufsize = 0;
		}
	}

	_mm_empty();
}

void GOST34112012Final_mmx(void* restrict ctx, unsigned char* restrict digest)
{
	GOST34112012Context* CTX = (GOST34112012Context*)ctx;
	ALIGN(16) union uint512_u buf;

	memset(&CTX->buffer[CTX->bufsize], 0, sizeof(CTX->buffer) - CTX->bufsize);

	buf.QWORD[0] = CTX->bufsize << 3;
	memset(((char*)&buf) + sizeof(buf.QWORD[0]), 0, sizeof(buf) - sizeof(buf.QWORD[0]));

	if (CTX->bufsize <= 63) {
		CTX->buffer[CTX->bufsize] = 1;
		memset(CTX->buffer + CTX->bufsize + 1, 0, sizeof(CTX->buffer) - CTX->bufsize + 1);
	}

	g(&CTX->h, &CTX->N, (const unsigned char*)&CTX->buffer);

	add512(&CTX->N, &buf, &CTX->N);
	add512(&CTX->Sigma, (const union uint512_u*)&CTX->buffer[0], &CTX->Sigma);

	g(&CTX->h, &buffer0, (const unsigned char*)&CTX->N);
	g(&CTX->h, &buffer0, (const unsigned char*)&CTX->Sigma);

	memcpy(&CTX->hash, &CTX->h, sizeof(CTX->hash));

	if (CTX->digest_size == 256) {
		memcpy(digest, &(CTX->hash.QWORD[4]), 32);
	}
	else {
		memcpy(digest, &(CTX->hash.QWORD[0]), 64);
	}

	memset(CTX, 0, sizeof(GOST34112012Context));
	_mm_empty();
}
