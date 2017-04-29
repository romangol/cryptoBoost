#include <iostream>
#include "sm2.h"
#include "../hash/sha256.h"
#include "../prng/random.h"

using namespace boost::multiprecision;


void sm2_sign(const_buf data, size_t len, buf signature, const_buf priKey, const Curve & curve)
{
	// 1. hash the message
	sha256(data, len, signature);
	cpp_int hash = cppint_from_uint8(signature, SHA256_DIGEST_LENGTH);

	// 2. produce random k
	uint8_t tmp[CURVE_LEN];
	random_fill(tmp, sizeof(tmp));
	tmp[0] &= 0x7f;
	cpp_int k = cppint_from_uint8(tmp, sizeof(tmp));
	memset(tmp, 0, sizeof(tmp));

	// 3. sign
	cpp_int dA = cppint_from_uint8(priKey, curve.blockLen);
	EPoint kG = mul(k, curve.G, curve);
	cpp_int r = mod( hash + kG.x, curve.n);
	cpp_int s = mod( inv_mod(1 + dA, curve.n) * (k - r * dA), curve.n );

	std::cout << s << std::endl;

	// 4. export
	cppint_to_uint8(r, signature, curve.blockLen);
	cppint_to_uint8(s, signature + curve.blockLen, curve.blockLen);
}


bool sm2_verify(const_buf data, size_t len, buf signature, const EPoint & pubKey, const Curve & curve)
{
	// 1. recover parameters
	uint8_t hash[SHA256_DIGEST_LENGTH];
	sha256(data, len, hash);
	cpp_int sign = cppint_from_uint8(hash, SHA256_DIGEST_LENGTH);
	cpp_int r = cppint_from_uint8(signature, CURVE_LEN);
	cpp_int s = cppint_from_uint8(signature + CURVE_LEN, CURVE_LEN);

	// 2. verify sign
	cpp_int t = mod(r + s, curve.n);
	EPoint A = mul( s, curve.G, curve);
	EPoint B = mul( t, pubKey, curve);
	EPoint X = add(A, B, curve);

	return mod(sign + X.x, curve.n) == r;
}

