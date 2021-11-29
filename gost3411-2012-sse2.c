#if (__x86_64__ || __i386__)
#pragma GCC target ("sse2,mmx")
#pragma clang attribute push (__attribute__((target("sse2,mmx"))), apply_to=function)

#ifndef __MMX__
#define __MMX__
#endif

#ifndef __SSE__
#define __SSE__
#endif

#ifndef __SSE2__
#define __SSE2__
#endif

#include <mmintrin.h>
#include <emmintrin.h>
#include "tables.h"
#include "gost3411-2012-sse2.h"

static inline void add512(const union uint512_u* x, const union uint512_u* y, union uint512_u* r)
{
	uint_fast8_t i, CF;

	CF = 0;
	for (i=0; i<8; ++i) {
		uint64_t a   = x->QWORD[i];
		uint64_t b   = y->QWORD[i];
		uint64_t sum = a + b + CF;
		CF           = ((sum < b) ? 1 : ((sum > b) ? 0 : CF));
		r->QWORD[i]  = sum;
	}
}

#if __i386__

static inline __m128i extract0(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	uint16_t ax;
	__m64 mm0, mm1;

	ax  = _mm_extract_epi16(xmm0, 0);
	mm0 = (__m64)(Ax[0][(uint8_t)ax]);
	mm1 = (__m64)(Ax[0][(uint8_t)(ax >> 8)]);

	ax  = _mm_extract_epi16(xmm0, 4);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[1][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[1][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, 0);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[2][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[2][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, 4);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[3][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[3][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, 0);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[4][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[4][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, 4);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[5][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[5][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, 0);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[6][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[6][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, 4);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[7][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[7][(uint8_t)(ax >> 8)]));

	return _mm_set_epi64(mm1, mm0);
}

static inline __m128i extract1(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	uint16_t ax;
	__m64 mm0, mm1;

	ax  = _mm_extract_epi16(xmm0, 1);
	mm0 = (__m64)(Ax[0][(uint8_t)ax]);
	mm1 = (__m64)(Ax[0][(uint8_t)(ax >> 8)]);

	ax  = _mm_extract_epi16(xmm0, 5);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[1][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[1][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, 1);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[2][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[2][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, 5);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[3][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[3][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, 1);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[4][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[4][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, 5);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[5][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[5][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, 1);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[6][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[6][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, 5);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[7][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[7][(uint8_t)(ax >> 8)]));

	return _mm_set_epi64(mm1, mm0);
}

static inline __m128i extract2(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	uint16_t ax;
	__m64 mm0, mm1;

	ax  = _mm_extract_epi16(xmm0, 2);
	mm0 = (__m64)(Ax[0][(uint8_t)ax]);
	mm1 = (__m64)(Ax[0][(uint8_t)(ax >> 8)]);

	ax  = _mm_extract_epi16(xmm0, 6);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[1][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[1][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, 2);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[2][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[2][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, 6);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[3][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[3][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, 2);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[4][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[4][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, 6);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[5][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[5][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, 2);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[6][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[6][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, 6);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[7][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[7][(uint8_t)(ax >> 8)]));

	return _mm_set_epi64(mm1, mm0);
}

static inline __m128i extract3(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	uint16_t ax;
	__m64 mm0, mm1;

	ax  = _mm_extract_epi16(xmm0, 3);
	mm0 = (__m64)(Ax[0][(uint8_t)ax]);
	mm1 = (__m64)(Ax[0][(uint8_t)(ax >> 8)]);

	ax  = _mm_extract_epi16(xmm0, 7);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[1][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[1][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, 3);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[2][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[2][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm1, 7);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[3][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[3][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, 3);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[4][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[4][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm2, 7);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[5][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[5][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, 3);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[6][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[6][(uint8_t)(ax >> 8)]));

	ax  = _mm_extract_epi16(xmm3, 7);
	mm0 = _mm_xor_si64(mm0, (__m64)(Ax[7][(uint8_t)ax]));
	mm1 = _mm_xor_si64(mm1, (__m64)(Ax[7][(uint8_t)(ax >> 8)]));

	return _mm_set_epi64(mm1, mm0);
}

#else

static inline __m128i extract0(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	uint64_t r0, r1;
	uint16_t ax;

	ax  = _mm_extract_epi16(xmm0, 0);
	r0  = Ax[0][(uint8_t)ax];
	r1  = Ax[0][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm0, 4);
	r0 ^= Ax[1][(uint8_t)ax];
	r1 ^= Ax[1][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, 0);
	r0 ^= Ax[2][(uint8_t)ax];
	r1 ^= Ax[2][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, 4);
	r0 ^= Ax[3][(uint8_t)ax];
	r1 ^= Ax[3][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, 0);
	r0 ^= Ax[4][(uint8_t)ax];
	r1 ^= Ax[4][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, 4);
	r0 ^= Ax[5][(uint8_t)ax];
	r1 ^= Ax[5][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, 0);
	r0 ^= Ax[6][(uint8_t)ax];
	r1 ^= Ax[6][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, 4);
	r0 ^= Ax[7][(uint8_t)ax];
	r1 ^= Ax[7][(uint8_t)(ax >> 8)];

	return _mm_set_epi64x(r1, r0);
}

static inline __m128i extract1(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	uint64_t r0, r1;
	uint16_t ax;

	ax  = _mm_extract_epi16(xmm0, 1);
	r0  = Ax[0][(uint8_t)ax];
	r1  = Ax[0][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm0, 5);
	r0 ^= Ax[1][(uint8_t)ax];
	r1 ^= Ax[1][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, 1);
	r0 ^= Ax[2][(uint8_t)ax];
	r1 ^= Ax[2][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, 5);
	r0 ^= Ax[3][(uint8_t)ax];
	r1 ^= Ax[3][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, 1);
	r0 ^= Ax[4][(uint8_t)ax];
	r1 ^= Ax[4][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, 5);
	r0 ^= Ax[5][(uint8_t)ax];
	r1 ^= Ax[5][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, 1);
	r0 ^= Ax[6][(uint8_t)ax];
	r1 ^= Ax[6][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, 5);
	r0 ^= Ax[7][(uint8_t)ax];
	r1 ^= Ax[7][(uint8_t)(ax >> 8)];

	return _mm_set_epi64x(r1, r0);
}

static inline __m128i extract2(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	uint64_t r0, r1;
	uint16_t ax;

	ax  = _mm_extract_epi16(xmm0, 2);
	r0  = Ax[0][(uint8_t)ax];
	r1  = Ax[0][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm0, 6);
	r0 ^= Ax[1][(uint8_t)ax];
	r1 ^= Ax[1][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, 2);
	r0 ^= Ax[2][(uint8_t)ax];
	r1 ^= Ax[2][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, 6);
	r0 ^= Ax[3][(uint8_t)ax];
	r1 ^= Ax[3][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, 2);
	r0 ^= Ax[4][(uint8_t)ax];
	r1 ^= Ax[4][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, 6);
	r0 ^= Ax[5][(uint8_t)ax];
	r1 ^= Ax[5][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, 2);
	r0 ^= Ax[6][(uint8_t)ax];
	r1 ^= Ax[6][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, 6);
	r0 ^= Ax[7][(uint8_t)ax];
	r1 ^= Ax[7][(uint8_t)(ax >> 8)];

	return _mm_set_epi64x(r1, r0);
}

static inline __m128i extract3(__m128i xmm0, __m128i xmm1, __m128i xmm2, __m128i xmm3)
{
	uint64_t r0, r1;
	uint16_t ax;

	ax  = _mm_extract_epi16(xmm0, 3);
	r0  = Ax[0][(uint8_t)ax];
	r1  = Ax[0][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm0, 7);
	r0 ^= Ax[1][(uint8_t)ax];
	r1 ^= Ax[1][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, 3);
	r0 ^= Ax[2][(uint8_t)ax];
	r1 ^= Ax[2][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm1, 7);
	r0 ^= Ax[3][(uint8_t)ax];
	r1 ^= Ax[3][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, 3);
	r0 ^= Ax[4][(uint8_t)ax];
	r1 ^= Ax[4][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm2, 7);
	r0 ^= Ax[5][(uint8_t)ax];
	r1 ^= Ax[5][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, 3);
	r0 ^= Ax[6][(uint8_t)ax];
	r1 ^= Ax[6][(uint8_t)(ax >> 8)];

	ax  = _mm_extract_epi16(xmm3, 7);
	r0 ^= Ax[7][(uint8_t)ax];
	r1 ^= Ax[7][(uint8_t)(ax >> 8)];

	return _mm_set_epi64x(r1, r0);
}

#endif

static inline void g(union uint512_u* restrict h, const union uint512_u* restrict N, const unsigned char* restrict m)
{
	__m128i xmm0, xmm2, xmm4, xmm6;
	__m128i xmm1, xmm3, xmm5, xmm7;
	__m128i tmm0, tmm1, tmm2, tmm3;
	uint_fast8_t i;
	const __m128i *pN = (const __m128i*)N;
	const __m128i *pM = (const __m128i*)m;
	 __m128i      *pH = (__m128i*)h;
	const __m128i* p;

	xmm0 = _mm_load_si128(&pN[0]);
	xmm2 = _mm_load_si128(&pN[1]);
	xmm4 = _mm_load_si128(&pN[2]);
	xmm6 = _mm_load_si128(&pN[3]);

	/* XLPS128M(h, xmm0, xmm2, xmm4, xmm6); */
	xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&pH[0]));
	xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&pH[1]));
	xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&pH[2]));
	xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&pH[3]));

	tmm0 = extract0(xmm0, xmm2, xmm4, xmm6);
	tmm1 = extract1(xmm0, xmm2, xmm4, xmm6);
	tmm2 = extract2(xmm0, xmm2, xmm4, xmm6);
	tmm3 = extract3(xmm0, xmm2, xmm4, xmm6);

	xmm0 = tmm0;
	xmm2 = tmm1;
	xmm4 = tmm2;
	xmm6 = tmm3;
	/**/

	xmm1 = _mm_load_si128(&pM[0]);
	xmm3 = _mm_load_si128(&pM[1]);
	xmm5 = _mm_load_si128(&pM[2]);
	xmm7 = _mm_load_si128(&pM[3]);

	/* XLPS128R */
	xmm1 = _mm_xor_si128(xmm1, xmm0);
	xmm3 = _mm_xor_si128(xmm3, xmm2);
	xmm5 = _mm_xor_si128(xmm5, xmm4);
	xmm7 = _mm_xor_si128(xmm7, xmm6);

	tmm0 = extract0(xmm1, xmm3, xmm5, xmm7);
	tmm1 = extract1(xmm1, xmm3, xmm5, xmm7);
	tmm2 = extract2(xmm1, xmm3, xmm5, xmm7);
	tmm3 = extract3(xmm1, xmm3, xmm5, xmm7);

	xmm1 = tmm0;
	xmm3 = tmm1;
	xmm5 = tmm2;
	xmm7 = tmm3;
	/* end of XLPS128R */

	for (i=0; i<11; ++i) {
		/* XLPS128M(&C[i], xmm0, xmm2, xmm4, xmm6); */
		p = (const __m128i*)&C[i];
		xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&p[0]));
		xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&p[1]));
		xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&p[2]));
		xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&p[3]));

		tmm0 = extract0(xmm0, xmm2, xmm4, xmm6);
		tmm1 = extract1(xmm0, xmm2, xmm4, xmm6);
		tmm2 = extract2(xmm0, xmm2, xmm4, xmm6);
		tmm3 = extract3(xmm0, xmm2, xmm4, xmm6);

		xmm0 = tmm0;
		xmm2 = tmm1;
		xmm4 = tmm2;
		xmm6 = tmm3;
		/**/

		/* XLPS128R */
		xmm1 = _mm_xor_si128(xmm1, xmm0);
		xmm3 = _mm_xor_si128(xmm3, xmm2);
		xmm5 = _mm_xor_si128(xmm5, xmm4);
		xmm7 = _mm_xor_si128(xmm7, xmm6);

		tmm0 = extract0(xmm1, xmm3, xmm5, xmm7);
		tmm1 = extract1(xmm1, xmm3, xmm5, xmm7);
		tmm2 = extract2(xmm1, xmm3, xmm5, xmm7);
		tmm3 = extract3(xmm1, xmm3, xmm5, xmm7);

		xmm1 = tmm0;
		xmm3 = tmm1;
		xmm5 = tmm2;
		xmm7 = tmm3;
		/* end of XLPS128R */
	}

	/*XLPS128M(&C[11], xmm0, xmm2, xmm4, xmm6);*/
	p = (const __m128i*)&C[11];
	xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&p[0]));
	xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&p[1]));
	xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&p[2]));
	xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&p[3]));

	tmm0 = extract0(xmm0, xmm2, xmm4, xmm6);
	tmm1 = extract1(xmm0, xmm2, xmm4, xmm6);
	tmm2 = extract2(xmm0, xmm2, xmm4, xmm6);
	tmm3 = extract3(xmm0, xmm2, xmm4, xmm6);

	xmm0 = tmm0;
	xmm2 = tmm1;
	xmm4 = tmm2;
	xmm6 = tmm3;

	xmm0 = _mm_xor_si128(xmm0, xmm1);
	xmm2 = _mm_xor_si128(xmm2, xmm3);
	xmm4 = _mm_xor_si128(xmm4, xmm5);
	xmm6 = _mm_xor_si128(xmm6, xmm7);

	xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&pH[0]));
	xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&pH[1]));
	xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&pH[2]));
	xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&pH[3]));

	xmm0 = _mm_xor_si128(xmm0, _mm_load_si128(&pM[0]));
	xmm2 = _mm_xor_si128(xmm2, _mm_load_si128(&pM[1]));
	xmm4 = _mm_xor_si128(xmm4, _mm_load_si128(&pM[2]));
	xmm6 = _mm_xor_si128(xmm6, _mm_load_si128(&pM[3]));

	_mm_store_si128(&pH[0], xmm0);
	_mm_store_si128(&pH[1], xmm2);
	_mm_store_si128(&pH[2], xmm4);
	_mm_store_si128(&pH[3], xmm6);
}

static inline void stage2(GOST34112012Context* restrict CTX, const unsigned char* restrict data)
{
	g(&CTX->h, &CTX->N, data);

	add512(&CTX->N, &buffer512, &CTX->N);
	add512(&CTX->Sigma, (const union uint512_u*)data, &CTX->Sigma);
}

void GOST34112012Update_sse2(void* restrict ctx, const unsigned char* restrict data, size_t len)
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

void GOST34112012Final_sse2(void* restrict ctx, unsigned char* restrict digest)
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

#pragma clang attribute pop
#endif
