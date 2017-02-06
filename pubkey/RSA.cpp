#include <iostream>
#include "bn/bn_boost.h"

using namespace boost::multiprecision;

struct RSAKey
{
	cpp_int e;
	cpp_int n;
	cpp_int d;
	cpp_int p;
	cpp_int q;
	
};

static cpp_int rsa_encrypt( const cpp_int & m, const RSAKey & key )
{
	return pow_mod( m, key.e, key.n );
}

static cpp_int rsa_decrypt( const cpp_int & c, const RSAKey & key )
{
	return pow_mod( c, key.d, key.n );
}

int main()
{
	RSAKey k;
	k.n = cpp_int
	("0xa3fe266f0b5ee57512cdf6c4c016d9ba21cba91e7c82d2b13399027520aebcc264bf2b124a9ff4a9bd9c3282b0a24ff13de2faed44c02b6e74c6ee71fdcf265ebac5c15e69f1a722e509b81687c416ef77d3ec43326a089cd6875c7eb0bcd7c3762f8db8d6ded8ecdb32bce7d1ede900d8e0da7ea797f4872e5ba97e40e220dd");
	k.d = cpp_int
	("0x02c47d3d8e819179c2517b82a283403565f09bf009c5e36c5e248ce8111f21cc5e0ae3dd312935e5cf6f5d62db3c42011e3c4063d4af318c71a811ca16b072434c0f62edbe0c38b16cd13e382039d5d246e71a6ff0272d4aa3dfbac8d4de4b9fc6250d97c17cc66941679f167ba4e5afc350a21e93660be343562ac2e2dc7af9");
	k.p = cpp_int
	("0xbcf5618f955546f2490d72523f3daacd9ff09a0e160d4230b3954f206d7360ad6b466c19dac90f3e02b364e5edc99f70e68ea14f57b0d95bf366bd1c97078eb7");
	k.q = cpp_int
	("0xde2d31b77541785812573acd705894d811e5c9dd7b013e5d0d90af437e47f6602a32cc413911a51ac038dd4e916bbbc0c9075c166cec5d108f5781337539f90b");
	k.e = 0x10001;

	cpp_int cipher = rsa_encrypt( 1207, k );
	std::cout << rsa_decrypt( cipher, k ) << std::endl;
}