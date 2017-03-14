#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "sha1.h"


static uint32_t rol(const uint32_t value, const size_t bits)
{
    return (value << bits) | (value >> (32 - bits));
}


static uint32_t blk(const uint32_t block[BLOCK_INTS], const size_t i)
{
    return rol(block[(i+13)&15] ^ block[(i+8)&15] ^ block[(i+2)&15] ^ block[i], 1);
}


/*
 * (R0+R1), R2, R3, R4 are the different operations used in SHA1
 */

static void R0(const uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    z += ((w&(x^y))^y) + block[i] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}


static void R1(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    block[i] = blk(block, i);
    z += ((w&(x^y))^y) + block[i] + 0x5a827999 + rol(v, 5);
    w = rol(w, 30);
}


static void R2(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    block[i] = blk(block, i);
    z += (w^x^y) + block[i] + 0x6ed9eba1 + rol(v, 5);
    w = rol(w, 30);
}


static void R3(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    block[i] = blk(block, i);
    z += (((w|x)&y)|(w&x)) + block[i] + 0x8f1bbcdc + rol(v, 5);
    w = rol(w, 30);
}


static void R4(uint32_t block[BLOCK_INTS], const uint32_t v, uint32_t &w, const uint32_t x, const uint32_t y, uint32_t &z, const size_t i)
{
    block[i] = blk(block, i);
    z += (w^x^y) + block[i] + 0xca62c1d6 + rol(v, 5);
    w = rol(w, 30);
}


/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

static void transform(uint32_t state[5], uint32_t block[BLOCK_INTS])
{
    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t e = state[4];

    /* 4 rounds of 20 operations each. Loop unrolled. */
    R0(block, a, b, c, d, e,  0);
    R0(block, e, a, b, c, d,  1);
    R0(block, d, e, a, b, c,  2);
    R0(block, c, d, e, a, b,  3);
    R0(block, b, c, d, e, a,  4);
    R0(block, a, b, c, d, e,  5);
    R0(block, e, a, b, c, d,  6);
    R0(block, d, e, a, b, c,  7);
    R0(block, c, d, e, a, b,  8);
    R0(block, b, c, d, e, a,  9);
    R0(block, a, b, c, d, e, 10);
    R0(block, e, a, b, c, d, 11);
    R0(block, d, e, a, b, c, 12);
    R0(block, c, d, e, a, b, 13);
    R0(block, b, c, d, e, a, 14);
    R0(block, a, b, c, d, e, 15);
    R1(block, e, a, b, c, d,  0);
    R1(block, d, e, a, b, c,  1);
    R1(block, c, d, e, a, b,  2);
    R1(block, b, c, d, e, a,  3);
    R2(block, a, b, c, d, e,  4);
    R2(block, e, a, b, c, d,  5);
    R2(block, d, e, a, b, c,  6);
    R2(block, c, d, e, a, b,  7);
    R2(block, b, c, d, e, a,  8);
    R2(block, a, b, c, d, e,  9);
    R2(block, e, a, b, c, d, 10);
    R2(block, d, e, a, b, c, 11);
    R2(block, c, d, e, a, b, 12);
    R2(block, b, c, d, e, a, 13);
    R2(block, a, b, c, d, e, 14);
    R2(block, e, a, b, c, d, 15);
    R2(block, d, e, a, b, c,  0);
    R2(block, c, d, e, a, b,  1);
    R2(block, b, c, d, e, a,  2);
    R2(block, a, b, c, d, e,  3);
    R2(block, e, a, b, c, d,  4);
    R2(block, d, e, a, b, c,  5);
    R2(block, c, d, e, a, b,  6);
    R2(block, b, c, d, e, a,  7);
    R3(block, a, b, c, d, e,  8);
    R3(block, e, a, b, c, d,  9);
    R3(block, d, e, a, b, c, 10);
    R3(block, c, d, e, a, b, 11);
    R3(block, b, c, d, e, a, 12);
    R3(block, a, b, c, d, e, 13);
    R3(block, e, a, b, c, d, 14);
    R3(block, d, e, a, b, c, 15);
    R3(block, c, d, e, a, b,  0);
    R3(block, b, c, d, e, a,  1);
    R3(block, a, b, c, d, e,  2);
    R3(block, e, a, b, c, d,  3);
    R3(block, d, e, a, b, c,  4);
    R3(block, c, d, e, a, b,  5);
    R3(block, b, c, d, e, a,  6);
    R3(block, a, b, c, d, e,  7);
    R3(block, e, a, b, c, d,  8);
    R3(block, d, e, a, b, c,  9);
    R3(block, c, d, e, a, b, 10);
    R3(block, b, c, d, e, a, 11);
    R4(block, a, b, c, d, e, 12);
    R4(block, e, a, b, c, d, 13);
    R4(block, d, e, a, b, c, 14);
    R4(block, c, d, e, a, b, 15);
    R4(block, b, c, d, e, a,  0);
    R4(block, a, b, c, d, e,  1);
    R4(block, e, a, b, c, d,  2);
    R4(block, d, e, a, b, c,  3);
    R4(block, c, d, e, a, b,  4);
    R4(block, b, c, d, e, a,  5);
    R4(block, a, b, c, d, e,  6);
    R4(block, e, a, b, c, d,  7);
    R4(block, d, e, a, b, c,  8);
    R4(block, c, d, e, a, b,  9);
    R4(block, b, c, d, e, a, 10);
    R4(block, a, b, c, d, e, 11);
    R4(block, e, a, b, c, d, 12);
    R4(block, d, e, a, b, c, 13);
    R4(block, c, d, e, a, b, 14);
    R4(block, b, c, d, e, a, 15);

    /* Add the working vars back into state[] */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

static void sha1_transform(SHA1_ctx & ctx)
{
	uint32_t block[BLOCK_INTS];
    
	for (size_t i = 0; i < BLOCK_INTS; ++i)
    {
        block[i] = (ctx.buffer[4 *i + 3] & 0xff)
                   | (ctx.buffer[4 *i + 2] & 0xff) << 8
                   | (ctx.buffer[4 *i + 1] & 0xff) << 16
                   | (ctx.buffer[4 *i + 0] & 0xff) << 24;
    }

	for( size_t i = 0; i < 16; ++i )
		printf("%08x", block[i] );
	puts("");

	transform( ctx.state, block );
}

/* Initialize the SHA values */
void sha1_init(SHA1_ctx & ctx)
{
    /* SHA1 initialization constants */
    ctx.state[0] = 0x67452301;
    ctx.state[1] = 0xefcdab89;
    ctx.state[2] = 0x98badcfe;
    ctx.state[3] = 0x10325476;
    ctx.state[4] = 0xc3d2e1f0;

	ctx.counter = 0;
}


void sha1_update(SHA1_ctx & ctx, const uint8_t data[], size_t len)
{
	for (size_t i = 0; i < len; ++i) 
	{
		ctx.buffer[(ctx.counter++) % SHA1_BLOCK_SIZE] = data[i];
		if (ctx.counter % SHA1_BLOCK_SIZE == 0 )
		{
			sha1_transform(ctx);
		}
	}
}

void sha1_final(SHA1_ctx & ctx, uint8_t digest[SHA1_DIGEST_LENGTH])
{
    /* Total number of hashed bits */
    uint64_t total_bits = ctx.counter * 8;

	ctx.buffer[(ctx.counter++) % SHA1_BLOCK_SIZE] = 0x80;
	while (ctx.counter % SHA1_BLOCK_SIZE != (SHA1_BLOCK_SIZE - 8) )
	{
		if (ctx.counter % SHA1_BLOCK_SIZE == 0 )
		{
			sha1_transform(ctx);
		}
		ctx.buffer[(ctx.counter++) % SHA1_BLOCK_SIZE] = 0;
	}

	for ( size_t i = 1; i <= 8; ++i )
	{
		ctx.buffer[SHA1_BLOCK_SIZE - i] = (total_bits & 0xff);
		total_bits >>= 8;
	}

	sha1_transform(ctx);

	memcpy( digest, ctx.state, SHA1_DIGEST_LENGTH );
	for (size_t i = 0; i < SHA1_DIGEST_LENGTH / 4; ++i)
    {
        ctx.state[i] = (digest[4 *i + 3] & 0xff)
                   | (digest[4 *i + 2] & 0xff) << 8
                   | (digest[4 *i + 1] & 0xff) << 16
                   | (digest[4 *i + 0] & 0xff) << 24;
    }
    memcpy( digest, ctx.state, SHA1_DIGEST_LENGTH );
    
	memset( ctx.state, 0, 20 );
	memset( ctx.buffer, 0, 64 );
	ctx.counter = 0;
	
}

void sha1(const uint8_t data[], size_t len, uint8_t digest[SHA1_DIGEST_LENGTH])
{
	SHA1_ctx s;
    sha1_init(s);
	sha1_update(s, data, len);
    sha1_final(s, digest);
}

