#include <iostream>
#include "RSA.h"

using namespace boost::multiprecision;

void set_rsa_key( RSAKey & key )
{
	key.n = key.p * key.q;
	
	key.d = inv_mod( key.e, (key.p - 1) * (key.q - 1) );

	/*
	 * Reduce the exponent mod phi(p) and phi(q), to save time when
	 * exponentiating mod p and mod q respectively. Of course, since p
	 * and q are prime, phi(p) == p-1 and similarly for q.
	 */
	key.dp = key.d % (key.p - 1);
	key.dq = key.d % (key.q - 1);
	
	key.iqmp = inv_mod( key.q, key.p );
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
