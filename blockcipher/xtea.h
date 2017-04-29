#ifndef _CRYPTO_BOOST_XTEA_H_
#define _CRYPTO_BOOST_XTEA_H_

#include <stdint.h>

const static size_t NUM_ROUNDS = 32;
const static size_t KEY_LEN = 16;
const static size_t BLK_LEN = 8;

struct xtea_key
{
	uint32_t key[4];
};

void xtea_key_setup( xtea_key & keyCtx, uint32_t key[4] );
void xtea_encrypt( uint32_t output[2], uint32_t input[2], const xtea_key & keyCtx );
void xtea_decrypt( uint32_t output[2], uint32_t input[2], const xtea_key & keyCtx );

#endif