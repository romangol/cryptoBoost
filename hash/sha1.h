#ifndef _CRYPTO_BOOST_SHA1_H_
#define _CRYPTO_BOOST_SHA1_H_

#include <cstdint>

const static size_t SHA1_DIGEST_LENGTH	= 20;
const static size_t SHA1_HMAC_SIZE		= 20;
const static size_t SHA1_BLOCK_SIZE		= 64;
static const size_t BLOCK_INTS			= 16;  /* number of 32bit integers per SHA1 block */

#ifdef __cplusplus
extern "C" {
#endif

struct SHA1_ctx
{
    uint8_t		buffer[SHA1_BLOCK_SIZE];
    uint32_t	state[6]; // 6 for alignment
    uint64_t	counter;
} ;

struct SHA1_hmac_ctx
{
	SHA1_ctx SHA1_ctx;
	uint8_t key[SHA1_DIGEST_LENGTH];
};


void sha1_init		(SHA1_ctx & ctx);
void sha1_update	(SHA1_ctx & ctx, const uint8_t data[], size_t len);
void sha1_final		(SHA1_ctx & ctx, uint8_t digest[SHA1_DIGEST_LENGTH]);
void sha1			(const uint8_t data[], size_t len, uint8_t digest[SHA1_DIGEST_LENGTH]);


void sha1_hmac_init		(SHA1_hmac_ctx & ctx, const uint8_t key[], size_t key_len);
void sha1_hmac_update	(SHA1_hmac_ctx & ctx, const uint8_t data[], size_t data_len);
void sha1_hmac_final	(SHA1_hmac_ctx & ctx, uint8_t mac[SHA1_HMAC_SIZE]);
void sha1_hmac			(const uint8_t & data, size_t data_len, const uint8_t key[], size_t key_len, uint8_t mac[SHA1_HMAC_SIZE]);

#ifdef __cplusplus
}
#endif


#endif // end of _CRYPTO_BOOST_SHA1_H_
