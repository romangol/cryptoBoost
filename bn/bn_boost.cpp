#include "bn_boost.h"

using namespace boost::multiprecision;

namespace boost
{
	void throw_exception(std::exception const & e)
	{
		throw e;
	}
}

cpp_int pow_mod( cpp_int base, cpp_int power, const cpp_int & mod  )
{
	if (power == 0)
        return 1;

	cpp_int r = 1;
	cpp_int blind = 1;
	while ( power != 0 )
	{
		if ( power % 2  == 1 )
		{
			r = r * base % mod;
		}
		else
		{
			blind = r * base % mod; // remove time differential
		}
		base =  base * base % mod;
		power /= 2;
	}
	
	return r;
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
			return y;
		if ( remainder ==  1 )
			return remainder;

		x = y;
		y = remainder;
	}
}

// calc z^-1 % m
cpp_int inv_mod( const cpp_int & z, const cpp_int & m )
{
	if ( z == m )
		return 0;
	
	cpp_int a, b;
	cpp_int r, q;
	cpp_int s = 1;
	cpp_int t = 0;
	
	if ( z > 0 && m > 0 )
	{
		a = m;
		if ( z < m )
			b = z;
		else
			b = z % m;

		while ( b > 0 ) // until j = 0
		{
			q = t - s * (a / b);
			r = a % b;

			// update!
			a = b; b = r;
			t = s; s = q;
		}
		if (a == 1) // always return positive value
		{
			q = t % m;
			if ( q < 0 )
				return q + m;
			return q;
		}
	}

	return 0;
}
