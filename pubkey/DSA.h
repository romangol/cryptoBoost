#ifndef _CRYPTO_BOOST_DSA_H
#define _CRYPTO_BOOST_DSA_H


#include "../romangol.h"
#include "../bn/bn_boost.h"
#include "ecc.h"

const static size_t CURVE_secp256r1_LEN = 32;

void ecdsa_sign		(const_buf data, size_t len, buf signature, const_buf priKey, const Curve & curve);
bool ecdsa_verify	(const_buf data, size_t len, buf signature, const EPoint & pubKey, const Curve & curve);

#endif