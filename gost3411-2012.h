#ifndef GOST3411_2012_H
#define GOST3411_2012_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include <inttypes.h>
#include <main/php_config.h>
#include <Zend/zend.h>

#if defined _MSC_VER
#	define ALIGN(x) __declspec(align(x))
#else
#	define ALIGN(x) __attribute__((__aligned__(x)))
#endif

union uint512_u {
	uint64_t QWORD[8];
} ALIGN(16);

ALIGN(16) typedef struct GOST34112012Context {
	ALIGN(16) unsigned char buffer[64];
	ALIGN(16) union uint512_u hash;
	ALIGN(16) union uint512_u h;
	ALIGN(16) union uint512_u N;
	ALIGN(16) union uint512_u Sigma;
	size_t bufsize;
	unsigned int digest_size;
} GOST34112012Context;

ZEND_DLEXPORT void GOST34112012Init(void* restrict ctx, const unsigned int digest_size);
ZEND_DLEXPORT extern void GOST34112012Update(void* restrict ctx, const unsigned char* restrict data, size_t len);
ZEND_DLEXPORT extern void GOST34112012Final(void* restrict ctx, unsigned char* restrict digest);

#endif /* GOST3411_2012_H */
