#ifndef _CRYPTO_BOOST_SHA256_H_
#define _CRYPTO_BOOST_SHA256_H_

#include <cstdint>

const static size_t SHA256_DIGEST_LENGTH	= 32;
const static size_t SHA256_BLOCK_SIZE		= 64;
const static size_t SHA256_HMAC_SIZE		= 32;


#ifdef __cplusplus
extern "C" {
#endif

struct SHA256_ctx
{
	uint8_t data[64];
	size_t datalen;
	unsigned long long bitlen;
	uint32_t state[8];
} ;

struct SHA256_hmac_ctx
{
	SHA256_ctx SHA256_ctx;
	uint8_t key[SHA256_DIGEST_LENGTH];
};


void sha256_init		(SHA256_ctx & ctx);
void sha256_update		(SHA256_ctx & ctx, const uint8_t * data, size_t data_len);
void sha256_final		(SHA256_ctx & ctx, uint8_t digest[SHA256_DIGEST_LENGTH]);
void sha256				(const uint8_t * data, size_t datalen, uint8_t digest[SHA256_DIGEST_LENGTH]);


void sha256_hmac_init		(SHA256_hmac_ctx & ctx, const uint8_t *key, size_t key_len);
void sha256_hmac_update		(SHA256_hmac_ctx & ctx, const uint8_t *data, size_t data_len);
void sha256_hmac_final		(SHA256_hmac_ctx & ctx, uint8_t mac[SHA256_HMAC_SIZE]);
void sha256_hmac			(const uint8_t & data, size_t data_len, const uint8_t *key, size_t key_len, uint8_t mac[SHA256_HMAC_SIZE]);

#ifdef __cplusplus
}
#endif


#endif // end of _CRYPTO_BOOST_SHA256_H_
