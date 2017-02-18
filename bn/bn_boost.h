#ifndef _BN_BOOST_H_
#define _BN_BOOST_H_

#include <boost/multiprecision/cpp_int.hpp>

boost::multiprecision::cpp_int gcd		( const boost::multiprecision::cpp_int & a, const boost::multiprecision::cpp_int & b );
boost::multiprecision::cpp_int inv_mod	( const boost::multiprecision::cpp_int & z, const boost::multiprecision::cpp_int & m );
boost::multiprecision::cpp_int pow_mod	( boost::multiprecision::cpp_int base, boost::multiprecision::cpp_int power, const boost::multiprecision::cpp_int & mod  );
boost::multiprecision::cpp_int mod		( const boost::multiprecision::cpp_int & a, const boost::multiprecision::cpp_int & m );

#endif