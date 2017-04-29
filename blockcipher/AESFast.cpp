#include <iostream>
#include "AESFast.h"

void EncKey_to_DecKey(const AESKey & encKey, AESKey & decKey, size_t rounds)
{
	decKey[rounds] = encKey[0];

	for ( size_t i = 0; i < rounds - 1; ++i )
	{
		decKey[rounds - i - 1] = _mm_aesimc_si128(encKey[1 + i]);
	}
	decKey[0] = encKey[rounds];
}

static inline void AES_128_Assist (uint128 & l, uint128 & r)
{
	uint128 m;
	r = _mm_shuffle_epi32 (r ,0xff);
	m = _mm_slli_si128(l, 0x4);
	l = _mm_xor_si128(l, m);
	m = _mm_slli_si128(m, 0x4);
	l = _mm_xor_si128(l, m);
	m = _mm_slli_si128(m, 0x4);
	l = _mm_xor_si128(l, m);
	l = _mm_xor_si128(l, r);
}

void AES128_Key_Expansion(const uint8 key[KEY128_SIZE], AESKey & roundKeys)
{
	uint128 l, r;

	l = _mm_loadu_si128( (uint128 *)key );
	
	_mm_storeu_si128(roundKeys + 0 , l);
	r = _mm_aeskeygenassist_si128(l, 1);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 1 , l);
	r = _mm_aeskeygenassist_si128(l, 2);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 2 , l);
	r = _mm_aeskeygenassist_si128(l, 4);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 3 , l);
	r = _mm_aeskeygenassist_si128(l, 8);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 4 , l);
	r = _mm_aeskeygenassist_si128(l, 0x10);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 5 , l);
	r = _mm_aeskeygenassist_si128(l, 0x20);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 6 , l);
	r = _mm_aeskeygenassist_si128(l, 0x40);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 7 , l);
	r = _mm_aeskeygenassist_si128(l, 0x80);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 8 , l);
	r = _mm_aeskeygenassist_si128(l, 0x1b);
	AES_128_Assist(l, r);
	_mm_storeu_si128(roundKeys + 9 , l);
	r = _mm_aeskeygenassist_si128(l, 0x36);
	AES_128_Assist(l, r);

	_mm_storeu_si128(roundKeys + 10, l);
}
	
static inline void AES256_assistA(uint128 & x, uint128 & y )
{
	uint128 t;
	y = _mm_shuffle_epi32(y, 0xff);
	t = _mm_slli_si128(x, 0x4);
	x = _mm_xor_si128(x, t);
	t = _mm_slli_si128(t, 0x4);
	x = _mm_xor_si128(x, t);
	t = _mm_slli_si128(t, 0x4);
	x = _mm_xor_si128(x, t);
	x = _mm_xor_si128(x, y);
}

static inline void AES256_assistB(uint128 & x, uint128 & y)
{
	uint128 s, t;
	t = _mm_aeskeygenassist_si128(x, 0x0);
	s = _mm_shuffle_epi32(t, 0xaa);
	t = _mm_slli_si128(y, 0x4);
	y = _mm_xor_si128(y, t);
	t = _mm_slli_si128(t, 0x4);
	y = _mm_xor_si128(y, t);
	t = _mm_slli_si128(t, 0x4);
	y = _mm_xor_si128(y, t);
	y = _mm_xor_si128(y, s);
}

void AES256_Key_Expansion(const uint8 key[KEY256_SIZE], AESKey & roundKeys)
{
	uint128 x, y, z;

	x = _mm_loadu_si128((uint128*) key );
	z = _mm_loadu_si128((uint128*) (key + 16) );

	roundKeys[0] = x;
	roundKeys[1] = z;
	
	y = _mm_aeskeygenassist_si128(z, 0x01);
	AES256_assistA(x, y);
	roundKeys[2] = x;
	AES256_assistB(x, z);
	roundKeys[3] = z;

	y = _mm_aeskeygenassist_si128(z, 0x02);
	AES256_assistA(x, y);
	roundKeys[4] = x;
	AES256_assistB(x, z);
	roundKeys[5] = z;
	
	y = _mm_aeskeygenassist_si128(z, 0x04);
	AES256_assistA(x, y);
	roundKeys[6] = x;
	AES256_assistB(x, z);
	roundKeys[7] = z;
	
	y = _mm_aeskeygenassist_si128(z, 0x08);
	AES256_assistA(x, y);
	roundKeys[8] = x;
	AES256_assistB(x, z);
	roundKeys[9] = z;
	
	y = _mm_aeskeygenassist_si128(z, 0x10);
	AES256_assistA(x, y);
	roundKeys[10] = x;
	AES256_assistB(x, z);
	roundKeys[11] = z;
	
	y = _mm_aeskeygenassist_si128(z, 0x20);
	AES256_assistA(x, y);
	roundKeys[12] = x;
	AES256_assistB(x, z);
	roundKeys[13] = z;
	
	y = _mm_aeskeygenassist_si128(z, 0x40);
	AES256_assistA(x, y);
	roundKeys[14] = x;
}


void AES128_block_enc(const uint128 & in, uint128 & out, const AESKey & encKey)
{
	out = _mm_xor_si128(in, encKey[0]);
	out = _mm_aesenc_si128(out, encKey[1]);
	out = _mm_aesenc_si128(out, encKey[2]);
	out = _mm_aesenc_si128(out, encKey[3]);
	out = _mm_aesenc_si128(out, encKey[4]);
	out = _mm_aesenc_si128(out, encKey[5]);
	out = _mm_aesenc_si128(out, encKey[6]);
	out = _mm_aesenc_si128(out, encKey[7]);
	out = _mm_aesenc_si128(out, encKey[8]);
	out = _mm_aesenc_si128(out, encKey[9]);
	out = _mm_aesenclast_si128(out, encKey[10]);
}

void AES128_block_dec(const uint128 & in, uint128 & out, const AESKey & decKey)
{
	out = _mm_xor_si128(in, decKey[0]);
	out = _mm_aesdec_si128(out,decKey[1]);
	out = _mm_aesdec_si128(out,decKey[2]);
	out = _mm_aesdec_si128(out,decKey[3]);
	out = _mm_aesdec_si128(out,decKey[4]);
	out = _mm_aesdec_si128(out,decKey[5]);
	out = _mm_aesdec_si128(out,decKey[6]);
	out = _mm_aesdec_si128(out,decKey[7]);
	out = _mm_aesdec_si128(out,decKey[8]);
	out = _mm_aesdec_si128(out,decKey[9]);
	out = _mm_aesdeclast_si128(out,decKey[10]);
}

void AES256_block_enc(const uint128 in, uint128 & out, AESKey encKey)
{
	out = _mm_xor_si128(in, encKey[0]);
	out = _mm_aesenc_si128(out, encKey[1]);
	out = _mm_aesenc_si128(out, encKey[2]);
	out = _mm_aesenc_si128(out, encKey[3]);
	out = _mm_aesenc_si128(out, encKey[4]);
	out = _mm_aesenc_si128(out, encKey[5]);
	out = _mm_aesenc_si128(out, encKey[6]);
	out = _mm_aesenc_si128(out, encKey[7]);
	out = _mm_aesenc_si128(out, encKey[8]);
	out = _mm_aesenc_si128(out, encKey[9]);
	out = _mm_aesenc_si128(out, encKey[10]);
	out = _mm_aesenc_si128(out, encKey[11]);
	out = _mm_aesenc_si128(out, encKey[12]);
	out = _mm_aesenc_si128(out, encKey[13]);
	out = _mm_aesenclast_si128(out, encKey[14]);
}

void AES256_block_dec(const uint128 in, uint128 & out, AESKey decKey)
{
	out = _mm_xor_si128(in, decKey[0]);
	out = _mm_aesdec_si128(out, decKey[1]);
	out = _mm_aesdec_si128(out, decKey[2]);
	out = _mm_aesdec_si128(out, decKey[3]);
	out = _mm_aesdec_si128(out, decKey[4]);
	out = _mm_aesdec_si128(out, decKey[5]);
	out = _mm_aesdec_si128(out, decKey[6]);
	out = _mm_aesdec_si128(out, decKey[7]);
	out = _mm_aesdec_si128(out, decKey[8]);
	out = _mm_aesdec_si128(out, decKey[9]);
	out = _mm_aesdec_si128(out, decKey[10]);
	out = _mm_aesdec_si128(out, decKey[11]);
	out = _mm_aesdec_si128(out, decKey[12]);
	out = _mm_aesdec_si128(out, decKey[13]);
	out = _mm_aesdeclast_si128(out, decKey[14]);
}

/* public interfaces */
void AES128_ECB_enc (const uint8 * in, uint8 * out, size_t length, const uint8 key[KEY128_SIZE])
{
	if ( length % BLOCK_SIZE != 0 )
		throw std::runtime_error("Error: invalid input buffer size");
	
	AESKey encKey;
	AES128_Key_Expansion( key, encKey );	
	
	uint128 u, v;
	for ( size_t i = 0; i < length / BLOCK_SIZE; ++i )
	{
		u = _mm_loadu_si128( (uint128 *) (in + i * BLOCK_SIZE) );
		AES128_block_enc( u, v, encKey );
		_mm_storeu_si128((uint128 *)(out + i * BLOCK_SIZE), v);
	}
}

void AES128_ECB_dec (const uint8 * in, uint8 * out, size_t length, const uint8 key[KEY128_SIZE])
{
	if ( length % BLOCK_SIZE != 0 )
		throw std::runtime_error("Error: invalid input buffer size");
	
	AESKey encKey, decKey;
	AES128_Key_Expansion( key, encKey );	
	EncKey_to_DecKey( encKey, decKey, AES128_ROUNDS );
	
	uint128 u, v;
	for ( size_t i = 0; i < length / BLOCK_SIZE; ++i )
	{
		u = _mm_loadu_si128( (uint128 *) (in + i * BLOCK_SIZE) );
		AES128_block_dec( u, v, decKey );
		_mm_storeu_si128((uint128 *)(out + i * BLOCK_SIZE), v);
	}
}

void AES256_ECB_enc (const uint8 * in, uint8 * out, size_t length, const uint8 key[KEY256_SIZE])
{
	if ( length % BLOCK_SIZE != 0 )
		throw std::runtime_error("Error: invalid input buffer size");
	
	AESKey encKey;
	AES256_Key_Expansion( key, encKey );	
	
	uint128 u, v;
	for ( size_t i = 0; i < length / BLOCK_SIZE; ++i )
	{
		u = _mm_loadu_si128( (uint128 *) (in + i * BLOCK_SIZE) );
		AES256_block_enc( u, v, encKey );
		_mm_storeu_si128((uint128 *)(out + i * BLOCK_SIZE), v);
	}
}

void AES256_ECB_dec (const uint8 * in, uint8 * out, size_t length, const uint8 key[KEY256_SIZE])
{
	if ( length % BLOCK_SIZE != 0 )
		throw std::runtime_error("Error: invalid input buffer size");
	
	AESKey encKey, decKey;
	AES256_Key_Expansion( key, encKey );	
	EncKey_to_DecKey( encKey, decKey, AES256_ROUNDS );
	
	uint128 u, v;
	for ( size_t i = 0; i < length / BLOCK_SIZE; ++i )
	{
		u = _mm_loadu_si128( (uint128 *) (in + i * BLOCK_SIZE) );
		AES256_block_dec( u, v, decKey );
		_mm_storeu_si128((uint128 *)(out + i * BLOCK_SIZE), v);
	}
}