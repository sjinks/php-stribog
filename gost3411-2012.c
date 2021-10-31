#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CPUID_H
#	include <cpuid.h>
#endif

#include "gost3411-2012.h"
#include "gost3411-2012-ref.h"

#if (__x86_64__ || __i386__)
#	include "gost3411-2012-mmx.h"
#	include "gost3411-2012-sse2.h"
#	include "gost3411-2012-sse41.h"
#endif

#ifndef __GNUC_PREREQ__
#ifdef __GNUC__
#define	__GNUC_PREREQ__(x, y)						\
	((__GNUC__ == (x) && __GNUC_MINOR__ >= (y)) ||			\
	 (__GNUC__ > (x)))
#else
#define	__GNUC_PREREQ__(x, y)	0
#endif
#endif

#include "tables.h"

void GOST34112012Init(void* ctx, const unsigned int digest_size)
{
	GOST34112012Context* CTX = (GOST34112012Context*)ctx;

	memset(CTX, 0, sizeof(GOST34112012Context));
	CTX->digest_size = digest_size;

	if (256 == digest_size) {
		CTX->h.QWORD[0] = 0x0101010101010101ULL;
		CTX->h.QWORD[1] = 0x0101010101010101ULL;
		CTX->h.QWORD[2] = 0x0101010101010101ULL;
		CTX->h.QWORD[3] = 0x0101010101010101ULL;
		CTX->h.QWORD[4] = 0x0101010101010101ULL;
		CTX->h.QWORD[5] = 0x0101010101010101ULL;
		CTX->h.QWORD[6] = 0x0101010101010101ULL;
		CTX->h.QWORD[7] = 0x0101010101010101ULL;
	}
}

#if defined(HAVE_CPUID_H) && (__x86_64__ || __i386__) && defined(__GNUC__) && __GNUC_PREREQ__(4, 6)
typedef void (*func_t)(void);

#ifndef bit_MMX
#define bit_MMX     (1 << 23)
#endif

#ifndef bit_SSE2
#define bit_SSE2    (1 << 26)
#endif

#ifndef bit_SSE4_1
#define bit_SSE4_1  (1 << 19)
#endif

static void (*resolve_GOST34112012Update(void))(void* restrict, const unsigned char* restrict, size_t)
{
	uint32_t eax, ebx, ecx, edx;

	if (__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
		if (ecx & bit_SSE4_1) {
			return GOST34112012Update_sse41;
		}

		if (edx & bit_SSE2) {
			return GOST34112012Update_sse2;
		}

		if (edx & bit_MMX) {
			return GOST34112012Update_mmx;
		}
	}

	return GOST34112012Update_ref;
}

static void (*resolve_GOST34112012Final(void))(void* restrict, unsigned char* restrict)
{
	uint32_t eax, ebx, ecx, edx;

	if (__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
		if (ecx & bit_SSE4_1) {
			return GOST34112012Final_sse41;
		}

		if (edx & bit_SSE2) {
			return GOST34112012Final_sse2;
		}

		if (edx & bit_MMX) {
			return GOST34112012Final_mmx;
		}
	}

	return GOST34112012Final_ref;
}

extern void GOST34112012Update(void* restrict ctx, const unsigned char* restrict data, size_t len) __attribute__((ifunc("resolve_GOST34112012Update")));
extern void GOST34112012Final(void* restrict ctx, unsigned char* restrict digest) __attribute__((ifunc("resolve_GOST34112012Final")));

#else

void GOST34112012Update(void* restrict ctx, const unsigned char* restrict data, size_t len)
{
#if __SSE4_1__
	GOST34112012Update_sse41(ctx, data, len);
#elif __SSE2__
	GOST34112012Update_sse2(ctx, data, len);
#elif __MMX__
	GOST34112012Update_mmx(ctx, data, len);
#else
	GOST34112012Update_ref(ctx, data, len);
#endif
}

extern void GOST34112012Final(void* restrict ctx, unsigned char* restrict digest)
{
#if __SSE4_1__
	GOST34112012Final_sse41(ctx, digest);
#elif __SSE2__
	GOST34112012Final_sse2(ctx, digest);
#elif __MMX__
	GOST34112012Final_mmx(ctx, digest);
#else
	GOST34112012Final_ref(ctx, digest);
#endif
}

#endif /* defined(__GNUC__) */
