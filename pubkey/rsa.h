#ifndef _BOOST_RSA_H_
#define _BOOST_RSA_H_
#include <cstdint>
#include "../bn/bn_boost.h"


struct RSAKey
{
    size_t bits;
    boost::multiprecision::cpp_int n;
    boost::multiprecision::cpp_int e;
    boost::multiprecision::cpp_int d;
    boost::multiprecision::cpp_int p;
    boost::multiprecision::cpp_int q;
    boost::multiprecision::cpp_int dp;
    boost::multiprecision::cpp_int dq;    
    boost::multiprecision::cpp_int iqmp;
};

void set_rsa_key( RSAKey & key );
boost::multiprecision::cpp_int rsa_encrypt( const boost::multiprecision::cpp_int & m, const RSAKey & key );
boost::multiprecision::cpp_int rsa_decrypt( const boost::multiprecision::cpp_int & c, const RSAKey & key );

#endif