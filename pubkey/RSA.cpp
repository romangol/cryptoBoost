#include <iostream>
#include <string>
#include "RSA.h"
#include "../hash/sha256.h"
#include "../prng/random.h"

using boost::multiprecision::cpp_int;
using std::string;

size_t calc_rsa_key( RSAKey & key )
{
	key.n = key.p * key.q;
	
	key.d = inv_mod( key.e, lcm( key.p - 1, key.q - 1) );

	/*
	 * Reduce the exponent mod phi(p) and phi(q), to save time when
	 * exponentiating mod p and mod q respectively. Of course, since p
	 * and q are prime, phi(p) == p-1 and similarly for q.
	 */
	key.dp = key.d % (key.p - 1);
	key.dq = key.d % (key.q - 1);
	
	key.iqmp = inv_mod( key.q, key.p );
	
	std::ostringstream ost;
	ost << std::hex << key.n; 
	string str = ost.str();
	key.keyLen = str.size() / 2;
	if ( str.size() % 2 == 1 )
		key.keyLen += 1;

	return key.keyLen;
}

bool verify_rsa_key	( const RSAKey & key )
{
	if (key.keyLen != 1024 / 8 && key.keyLen != 2048 / 8)
		return false;

	if (key.n != key.p * key.q)
		return false;
	
	if ( key.d != inv_mod( key.e, lcm( key.p - 1, key.q - 1) ) )
		return false;
	
	if ( key.dp != key.d % (key.p - 1) )
		return false;

	if ( key.dq != key.d % (key.q - 1) )
		return false;
	
	if ( key.iqmp != inv_mod( key.q, key.p ) )
		return false;

	return true;
}
	

static cpp_int crt_modpow( const cpp_int & ciphertext, const RSAKey & key )
{
	/*
	 * Do the two modpows.
	 */
	cpp_int presult = powm(ciphertext, key.dp, key.p);
	cpp_int qresult = powm(ciphertext, key.dq, key.q);

	/*
	 * Recombine the results. We want a value which is congruent to
	 * qresult mod q, and to presult mod p.
	 *
	 * We know that iqmp * q is congruent to 1 * mod p (by definition
	 * of iqmp) and to 0 mod q (obviously). So we start with qresult
	 * (which is congruent to qresult mod both primes), and add on
	 * (presult-qresult) * (iqmp * q) which adjusts it to be congruent
	 * to presult mod p without affecting its value mod q.
	 */
	if ( presult < qresult )
	{
		/*
		 * Can't subtract presult from qresult without first adding on p.
		 */
		presult += key.p;
	}

	return ( key.iqmp * key.q * (presult - qresult) + qresult) % key.n;
}

cpp_int rsa_encrypt( const cpp_int & m, const RSAKey & key )
{
	return powm( m, key.e, key.n );
}

cpp_int rsa_decrypt( const cpp_int & c, const RSAKey & key )
{
	// return powm( c, key.d, key.n );
	return crt_modpow( c, key );
}

static void MGF1_mask( uint8_t * seed, size_t seedLen, uint8_t * toBeMasked, size_t maskLen )
{
	SHA256_ctx ctx;
	uint8_t hashBuf[SHA256_DIGEST_LENGTH];
	uint8_t tail[4];

	size_t counter = 0;
	while ( maskLen > SHA256_DIGEST_LENGTH )
	{
		memcpy( tail, &counter, 4 );
	    sha256_init(ctx);
		sha256_update(ctx, seed, seedLen);
		sha256_update(ctx, tail, 4);
		sha256_final(ctx, hashBuf);
		for ( size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i )
			toBeMasked[counter * SHA256_DIGEST_LENGTH + i] ^= hashBuf[i];

		maskLen -= SHA256_DIGEST_LENGTH;
		++counter;
	}

	memcpy( tail, &counter, 4 );
    sha256_init(ctx);
	sha256_update(ctx, seed, seedLen);
	sha256_update(ctx, tail, 4);
	sha256_final(ctx, hashBuf);
	for ( size_t i = 0; i < maskLen; ++i )
		toBeMasked[counter * SHA256_DIGEST_LENGTH + i] ^= hashBuf[i];
}


size_t rsa_enc_oaep( uint8_t * msg, size_t msgLen, uint8_t * cipher, const RSAKey & key, bool is_sign )
{
	// rsa key 长度应该最少为msgLen + 2 * SHA256_DIGEST_LENGTH + 2
	// i.e. key 长度至少应该比消息长度长(SHA256_DIGEST_LENGTH + 2)字节
	if ( msgLen + 2 * SHA256_DIGEST_LENGTH + 2 > key.keyLen )
	{
		// throw rsaException???
		return 0;
	}

	static uint8_t buffer[1024];
	memset( buffer, 0, sizeof(buffer) );
	
	const uint8_t rhash[] = // SHA256 of "RomanGol"
	{
		0x63, 0xd8, 0x9b, 0xca, 0x91, 0x87, 0x68, 0x54, 0x91, 0xd3, 0xe9, 0x3e, 0x11, 0x48, 0x02, 0x5f,
		0xdf, 0x9a, 0x19, 0xe6, 0x60, 0xa6, 0xdf, 0x8d, 0x19, 0x56, 0xa5, 0xce, 0xd0, 0x7e, 0x86, 0x6c
	};

	uint8_t seed[SHA256_DIGEST_LENGTH];

	random_fill( seed, SHA256_DIGEST_LENGTH );
	memcpy( buffer + 1, seed, SHA256_DIGEST_LENGTH );
	memcpy( buffer + 1 +  SHA256_DIGEST_LENGTH, rhash, SHA256_DIGEST_LENGTH );

	size_t psLen = key.keyLen - 2 * SHA256_DIGEST_LENGTH - 2 - msgLen;
	buffer[2 * SHA256_DIGEST_LENGTH + 1 + psLen] = 0x01;
	memcpy( buffer + 2 * SHA256_DIGEST_LENGTH + 2 + psLen, msg, msgLen );
	
	size_t dbLen = SHA256_DIGEST_LENGTH + psLen + 1 + msgLen;
	MGF1_mask( seed, SHA256_DIGEST_LENGTH, buffer + SHA256_DIGEST_LENGTH + 1, dbLen );
	MGF1_mask( buffer + SHA256_DIGEST_LENGTH + 1, dbLen, buffer + 1, SHA256_DIGEST_LENGTH );

	if ( is_sign )
		cppint_to_uint8( rsa_decrypt( cppint_from_uint8(buffer, key.keyLen), key ), cipher, key.keyLen );
	else
		cppint_to_uint8( rsa_encrypt( cppint_from_uint8(buffer, key.keyLen), key ), cipher, key.keyLen );
	
	return msgLen;
}

size_t rsa_dec_oaep( uint8_t * cipher, size_t textLen, uint8_t * msg, const RSAKey & key, bool is_sign )
{
	if ( textLen != key.keyLen )
		return 0;
	
	static uint8_t buffer[1024];
	memset( buffer, 0, sizeof(buffer) );

	if ( is_sign )
		cppint_to_uint8( rsa_encrypt( cppint_from_uint8(cipher, textLen), key ), buffer, key.keyLen );
	else
		cppint_to_uint8( rsa_decrypt( cppint_from_uint8(cipher, textLen), key ), buffer, key.keyLen );

	size_t dbLen = textLen - SHA256_DIGEST_LENGTH - 1;
	MGF1_mask( buffer + SHA256_DIGEST_LENGTH + 1, dbLen, buffer + 1, SHA256_DIGEST_LENGTH );
	MGF1_mask( buffer + 1, SHA256_DIGEST_LENGTH, buffer + SHA256_DIGEST_LENGTH + 1, dbLen );

	size_t pos = 2 * SHA256_DIGEST_LENGTH + 2;
	while ( buffer[pos++] != 0x1 )
		;

	memcpy( msg, buffer + pos, key.keyLen - pos );
	
	return key.keyLen - pos;
}

size_t rsa_enc_pkcs15( uint8_t * msg, size_t msgLen, uint8_t * cipher, const RSAKey & key, bool is_sign )
{
	if ( msgLen + 11 > key.keyLen )
	{
		return 0;
	}

	static uint8_t buffer[1024];
	memset( buffer, 0, sizeof(buffer) );
	
	buffer[1] = 0x2; // ??

	if ( buffer[1] == 0x2 )
		random_fill( buffer + 2, key.keyLen - 3 - msgLen );
	else if ( buffer[1] == 0x1 )
		memset( buffer + 2, 0xff, key.keyLen - 3 - msgLen );
	else if ( buffer[1] == 0x0 )
		memset( buffer + 2, 0, key.keyLen - 3 - msgLen );

	buffer[key.keyLen - msgLen - 1] = 0x0;
	memcpy( buffer + key.keyLen - msgLen, msg, msgLen );

	if ( is_sign )
		cppint_to_uint8( rsa_decrypt( cppint_from_uint8(buffer, key.keyLen), key ), cipher, key.keyLen );
	else
		cppint_to_uint8( rsa_encrypt( cppint_from_uint8(buffer, key.keyLen), key ), cipher, key.keyLen );
	
	return msgLen;
}

size_t rsa_dec_pkcs15( uint8_t * cipher, size_t textLen, uint8_t * msg, const RSAKey & key, bool is_sign )
{
	if ( textLen != key.keyLen )
		return 0;
	
	static uint8_t buffer[1024];
	memset( buffer, 0, sizeof(buffer) );

	if ( is_sign )
		cppint_to_uint8( rsa_encrypt( cppint_from_uint8(cipher, textLen), key ), buffer, key.keyLen );
	else
		cppint_to_uint8( rsa_decrypt( cppint_from_uint8(cipher, textLen), key ), buffer, key.keyLen );

	if ( buffer[0] != 0x0 )
		return 0;
	if ( buffer[1] != 0x2 ) // ?
		return 0;

	size_t pos = 2;
	while ( buffer[pos++] != 0x0 )
		;

	memcpy( msg, buffer + pos, key.keyLen - pos );

	return key.keyLen - pos;
}
