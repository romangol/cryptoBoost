#include "bn_boost.h"
#include <string>
#include <iostream>

using std::string;

cpp_int cppint_from_uint8( const uint8_t * buffer, size_t len, bool usingLE )
{
	string s;

	char tmpBuf[3] = {0, 0, 0};

	for ( size_t i = 0; i < len; ++i )
	{
		sprintf(tmpBuf, "%02x", buffer[i]);
		if ( usingLE )
			s = string(tmpBuf) + s;
		else
			s += string(tmpBuf);
	}

	return cpp_int( string("0x") + s );
}


void cppint_to_uint8( const cpp_int & input, uint8_t * buffer, size_t len, bool usingLE )
{
	std::ostringstream ost;
	ost << std::hex << input; 
	string str = ost.str();

	if ( len < str.size() / 2 )
		return;

	size_t padLen = len * 2 - str.size();
	for ( size_t i = 0; i < padLen; ++i )
		str = string("0") + str;

	for ( size_t i = 0; i < len; ++i )
	{
		if ( usingLE )
		{
			buffer[len - i - 1] = std::stoi( str.substr(i * 2, 2), 0, 16 );
		}
		else
		{
			buffer[i] = std::stoi( str.substr(i * 2, 2), 0, 16 );
		}
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

cpp_int lcm( const cpp_int & a, const cpp_int & b )
{
	return a / gcd(a, b) * b;
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


