#include "bn_boost.h"

using namespace boost::multiprecision;

namespace boost
{
	void throw_exception(std::exception const & e)
	{
		throw e;
	}
}

cpp_int mod( const cpp_int & a, const cpp_int & m )
{
	if ( a < 0 )
	{
		return m + a % m;
	}
	return a % m;
}

cpp_int gcd( const cpp_int & a, const cpp_int & b )
{
	if ( a == b )
		return a;

	cpp_int x, y;
	
	// assignment
	if ( a > b )
	{
		x = a; y = b;
	}
	else
	{
		x = b; y = a;
	}
	
	// calculation
	cpp_int remainder = 1;
	while ( true )
	{
		remainder = x % y;

		if ( remainder ==  0 )
		{
			return y;
		}
		if ( remainder ==  1 )
		{
			return remainder;
		}

		x = y;
		y = remainder;
	}
}

// calc z^-1 % m
cpp_int inv_mod( const cpp_int & z, const cpp_int & m )
{
	if ( m < 0 ) throw 0x19841207;
	
	if ( z % m == 0 ) return 0;

	cpp_int x = 0;
	cpp_int yy = 0;
	cpp_int y = 1;
	cpp_int xx = 1;
	cpp_int b = m;
	cpp_int a = mod(z, m);
	cpp_int tmp;

	while ( b != 0 )
	{
		cpp_int q = a / b;
		tmp = b; b = a % b; a = tmp;
		tmp = x; x = xx - q * x; xx = tmp;
		tmp = y; y = yy - q * y ; yy = tmp;
	}

	return (xx % m) + (xx < 0 ? m : 0);
}
