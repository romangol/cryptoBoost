#ifndef _SM3_H_
#define _SM3_H_

#include <stdint.h>
#include <string.h>

const static size_t SM3_DIGEST_LENGTH	= 32;
const static size_t SM3_BLOCK_SIZE		= 64;
const static size_t SM3_HMAC_SIZE		= 32;


#ifdef __cplusplus
extern "C" {
#endif


struct sm3_ctx_t
{
	uint32_t digest[8];
	size_t nblocks;
	uint8_t block[64];
	size_t num;
};

void sm3_init		(sm3_ctx_t & ctx);
void sm3_update		(sm3_ctx_t & ctx, const uint8_t * data, size_t data_len);
void sm3_final		(sm3_ctx_t & ctx, uint8_t digest[SM3_DIGEST_LENGTH]);
void sm3_compress	(uint32_t digest[8], const uint8_t block[SM3_BLOCK_SIZE]);
void sm3			(const uint8_t * data, size_t datalen, uint8_t digest[SM3_DIGEST_LENGTH]);


struct sm3_hmac_ctx_t
{
	sm3_ctx_t sm3_ctx;
	uint8_t key[SM3_DIGEST_LENGTH];
};

void sm3_hmac_init		(sm3_hmac_ctx_t & ctx, const uint8_t *key, size_t key_len);
void sm3_hmac_update	(sm3_hmac_ctx_t & ctx, const uint8_t *data, size_t data_len);
void sm3_hmac_final		(sm3_hmac_ctx_t & ctx, uint8_t mac[SM3_HMAC_SIZE]);
void sm3_hmac			(const uint8_t & data, size_t data_len, const uint8_t *key, size_t key_len, uint8_t mac[SM3_HMAC_SIZE]);

#ifdef __cplusplus
}
#endif


#endif // end of sm3.h
