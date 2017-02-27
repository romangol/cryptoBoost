#ifndef _BOOST_RSA_H_
#define _BOOST_RSA_H_
#include <cstdint>
#include "../bn/bn_boost.h"

using boost::multiprecision::cpp_int;

const static size_t MAX_RSA_PK_LEN = 512;

struct RSAKey
{
    size_t keyLen;
    cpp_int n;
    cpp_int e;
    cpp_int d;
    cpp_int p;
    cpp_int q;
    cpp_int dp;
    cpp_int dq;    
    cpp_int iqmp;
};

size_t		calc_rsa_key	( RSAKey & key );
bool		verify_rsa_key	( const RSAKey & key );

size_t rsa_enc_oaep( SKB_Byte * msg, size_t msgLen, SKB_Byte * cipher, const RSAKey & key, bool is_sign = false );
size_t rsa_dec_oaep( SKB_Byte * cipher, size_t textLen, SKB_Byte * msg, const RSAKey & key, bool is_sign = false );
size_t rsa_enc_pkcs15( SKB_Byte * msg, size_t msgLen, SKB_Byte * cipher, const RSAKey & key, bool is_sign = false );
size_t rsa_dec_pkcs15( SKB_Byte * cipher, size_t textLen, SKB_Byte * msg, const RSAKey & key, bool is_sign = false );

#endif