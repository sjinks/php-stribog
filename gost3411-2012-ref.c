#include "gost3411-2012-ref.h"
#include "tables.h"

#ifdef WORDS_BIGENDIAN
static inline uint64_t bswap64(uint64_t x)
{
#ifdef __GNUC__
	return __builtin_bswap64(x);
#else
	return
		((x & 0xFF00000000000000ULL) >> 56) |
		((x & 0x00FF000000000000ULL) >> 40) |
		((x & 0x0000FF0000000000ULL) >> 24) |
		((x & 0x000000FF00000000ULL) >>  8) |
		((x & 0x00000000FF000000ULL) <<  8) |
		((x & 0x0000000000FF0000ULL) << 24) |
		((x & 0x000000000000FF00ULL) << 40) |
		((x & 0x00000000000000FFULL) << 56)
	;
#endif
}
#endif

static void XLPS(const union uint512_u* x, const union uint512_u* y, union uint512_u* data)
{
	uint64_t r0, r1, r2, r3, r4, r5, r6, r7;
	int_fast8_t i;

	r0 = x->QWORD[0] ^ y->QWORD[0];
	r1 = x->QWORD[1] ^ y->QWORD[1];
	r2 = x->QWORD[2] ^ y->QWORD[2];
	r3 = x->QWORD[3] ^ y->QWORD[3];
	r4 = x->QWORD[4] ^ y->QWORD[4];
	r5 = x->QWORD[5] ^ y->QWORD[5];
	r6 = x->QWORD[6] ^ y->QWORD[6];
	r7 = x->QWORD[7] ^ y->QWORD[7];

#ifndef WORDS_BIGENDIAN
	for (i=0; i<8; ++i) {
		data->QWORD[i]  = Ax[0][(uint8_t)(r0 >> (i << 3))];
		data->QWORD[i] ^= Ax[1][(uint8_t)(r1 >> (i << 3))];
		data->QWORD[i] ^= Ax[2][(uint8_t)(r2 >> (i << 3))];
		data->QWORD[i] ^= Ax[3][(uint8_t)(r3 >> (i << 3))];
		data->QWORD[i] ^= Ax[4][(uint8_t)(r4 >> (i << 3))];
		data->QWORD[i] ^= Ax[5][(uint8_t)(r5 >> (i << 3))];
		data->QWORD[i] ^= Ax[6][(uint8_t)(r6 >> (i << 3))];
		data->QWORD[i] ^= Ax[7][(uint8_t)(r7 >> (i << 3))];
	}
#else
	for (i=7; i>=0; --i) {
		data->QWORD[7-i]  = Ax[0][(uint8_t)(r0 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[1][(uint8_t)(r1 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[2][(uint8_t)(r2 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[3][(uint8_t)(r3 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[4][(uint8_t)(r4 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[5][(uint8_t)(r5 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[6][(uint8_t)(r6 >> (i << 3))];
		data->QWORD[7-i] ^= Ax[7][(uint8_t)(r7 >> (i << 3))];
	}
#endif
}

static inline void X(const union uint512_u* x, const union uint512_u* y, union uint512_u* z)
{
	z->QWORD[0] = x->QWORD[0] ^ y->QWORD[0];
	z->QWORD[1] = x->QWORD[1] ^ y->QWORD[1];
	z->QWORD[2] = x->QWORD[2] ^ y->QWORD[2];
	z->QWORD[3] = x->QWORD[3] ^ y->QWORD[3];
	z->QWORD[4] = x->QWORD[4] ^ y->QWORD[4];
	z->QWORD[5] = x->QWORD[5] ^ y->QWORD[5];
	z->QWORD[6] = x->QWORD[6] ^ y->QWORD[6];
	z->QWORD[7] = x->QWORD[7] ^ y->QWORD[7];
}

static inline void add512(const union uint512_u* x, const union uint512_u* y, union uint512_u* r)
{
#ifndef WORDS_BIGENDIAN
	uint_fast8_t i, CF = 0;

#if defined(__SIZEOF_INT128__)
	const unsigned __int128* X = (const unsigned __int128*)x;
	const unsigned __int128* Y = (const unsigned __int128*)y;
	unsigned __int128* R       = (unsigned __int128*)r;

	for (i=0; i<4; ++i) {
		unsigned __int128 a   = X[i];
		unsigned __int128 b   = Y[i];
		unsigned __int128 sum = a + b + CF;
		CF                    = ((sum < b) ? 1 : ((sum > b) ? 0 : CF));
		R[i]  = sum;
	}
#else
	for (i=0; i<8; ++i) {
		uint64_t a   = x->QWORD[i];
		uint64_t b   = y->QWORD[i];
		uint64_t sum = a + b + CF;
		CF           = ((sum < b) ? 1 : ((sum > b) ? 0 : CF));
		r->QWORD[i]  = sum;
	}
#endif
#else
	const unsigned char *xp, *yp;
	unsigned char *rp;
	unsigned int i;
	int buf;

	xp = (const unsigned char*)&x[0];
	yp = (const unsigned char*)&y[0];
	rp = (unsigned char*)&r[0];

	buf = 0;
	for (i=0; i<64; ++i) {
		buf   = xp[i] + yp[i] + (buf >> 8);
		rp[i] = (uint8_t)buf;
	}
#endif
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


void GOST34112012Update_ref(void* restrict ctx, const unsigned char* restrict data, size_t len)
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
}

void GOST34112012Final_ref(void* restrict ctx, unsigned char* restrict digest)
{
	GOST34112012Context* CTX = (GOST34112012Context*)ctx;
	ALIGN(16) union uint512_u buf;

	memset(&CTX->buffer[CTX->bufsize], 0, sizeof(CTX->buffer) - CTX->bufsize);

#ifndef WORDS_BIGENDIAN
	buf.QWORD[0] = CTX->bufsize << 3;
#else
	buf.QWORD[0] = bswap64(CTX->bufsize << 3);
#endif
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
}
