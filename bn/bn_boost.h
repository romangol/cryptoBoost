#ifndef _BN_BOOST_H_
#define _BN_BOOST_H_

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

cpp_int lcm			( const cpp_int & a, const cpp_int & b );
cpp_int gcd			( const cpp_int & a, const cpp_int & b );
cpp_int inv_mod		( const cpp_int & z, const cpp_int & m );
cpp_int pow_mod		( cpp_int base, cpp_int power, const cpp_int & mod  );
cpp_int mod			( const cpp_int & a, const cpp_int & m );

cpp_int		cppint_from_uint8	( uint8_t * buffer, size_t len, bool usingLE = false );
void		cppint_to_uint8		( const cpp_int & input, uint8_t * buffer, size_t len, bool usingLE = false );

#endif