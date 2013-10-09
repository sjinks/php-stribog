#ifndef GOST3411_2012_H
#define GOST3411_2012_H

#include <stddef.h>
#include <inttypes.h>
#include <main/php_config.h>

union uint512_u {
	uint64_t QWORD[8];
};

typedef struct GOST34112012Context {
	unsigned char buffer[64];
	union uint512_u hash;
	union uint512_u h;
	union uint512_u N;
	union uint512_u Sigma;
	size_t bufsize;
	unsigned int digest_size;
} GOST34112012Context;

ZEND_DLEXPORT void GOST34112012Init(void* ctx, const unsigned int digest_size);
ZEND_DLEXPORT void GOST34112012Update(void* ctx, const unsigned char* data, size_t len);
ZEND_DLEXPORT void GOST34112012Final(void* ctx, unsigned char* digest);

#endif /* GOST3411_2012_H */
