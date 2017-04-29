#ifndef _AES_FAST_
#define _AES_FAST_

#include <wmmintrin.h>
#include <cstdint>

typedef unsigned char	uint8;
typedef __m128i			uint128;


enum
{
	BLOCK_SIZE = 16,
	KEY128_SIZE = 16,
	KEY192_SIZE = 24,
	KEY256_SIZE = 32,
	AES128_ROUNDS = 10,
	AES192_ROUNDS = 12,
	AES256_ROUNDS = 14,
};

typedef uint128 AESKey[AES256_ROUNDS + 1];

void AES128_CBC_enc (const uint8 * in, uint8 * out, uint8 iv[BLOCK_SIZE], size_t length, const uint8 key[KEY128_SIZE]);
void AES128_CBC_dec (const uint8 * in, uint8 * out, uint8 iv[BLOCK_SIZE], size_t length, const uint8 key[KEY128_SIZE]);

void AES128_ECB_enc (const uint8 * in, uint8 * out, size_t length, const uint8 key[KEY128_SIZE]);
void AES128_ECB_dec (const uint8 * in, uint8 * out, size_t length, const uint8 key[KEY128_SIZE]);

void AES256_CBC_enc (const uint8 * in, uint8 * out, uint8 iv[BLOCK_SIZE], size_t length, const uint8 key[KEY256_SIZE]);
void AES256_CBC_dec (const uint8 * in, uint8 * out, uint8 iv[BLOCK_SIZE], size_t length, const uint8 key[KEY256_SIZE]);

void AES256_ECB_enc (const uint8 * in, uint8 * out, size_t length, const uint8 key[KEY256_SIZE]);
void AES256_ECB_dec (const uint8 * in, uint8 * out, size_t length, const uint8 key[KEY256_SIZE]);

#endif
