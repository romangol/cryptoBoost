#ifndef _CRYPTO_BOOST_DSA_H
#define _CRYPTO_BOOST_DSA_H


#include "../romangol.h"
#include "ecc.h"

void ecdsa_sign		(const_buf data, size_t len, buf signature, const_buf priKey, const Curve & curve);
bool ecdsa_verify	(const_buf data, size_t len, buf signature, const EPoint & pubKey, const Curve & curve);

#endif