#ifndef _CRYPTO_BOOST_SM2_H
#define _CRYPTO_BOOST_SM2_H

#include "../romangol.h"
#include "ecc.h"

void sm2_sign		(const_buf data, size_t len, buf signature, const_buf priKey, const Curve & curve);
bool sm2_verify	(const_buf data, size_t len, buf signature, const EPoint & pubKey, const Curve & curve);

#endif