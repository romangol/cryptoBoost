#ifndef _CRYPTO_BOOST_AES256_H_
#define _CRYPTO_BOOST_AES256_H_

#include <cstring>
#include <cstdint>

const static size_t AES256_BLK_SIZE = 16;
const static size_t AES256_KEY_SIZE = 32;
static const size_t AES256_ROUNDS = 14;
const static size_t SUBKEY_LEN = AES256_ROUNDS * AES256_BLK_SIZE;

union AESBlock
{
	uint32_t l[AES256_BLK_SIZE / sizeof(uint32_t)];
	uint8_t c[AES256_BLK_SIZE];
};

void aes256_set_key ( uint8_t key[AES256_KEY_SIZE], uint32_t subkeyE[SUBKEY_LEN / sizeof(uint32_t)], uint32_t subkeyD[SUBKEY_LEN / sizeof(uint32_t)] );
void aes256_enc_block( AESBlock & block, uint32_t subKey[SUBKEY_LEN / sizeof(uint32_t)] );
void aes256_dec_block( AESBlock & block, uint32_t subKey[SUBKEY_LEN / sizeof(uint32_t)] );

void aes256_enc_ecb_w( uint8_t input[AES256_BLK_SIZE], size_t len, uint8_t output[AES256_BLK_SIZE] );
void aes256_dec_ecb_w( uint8_t input[AES256_BLK_SIZE], size_t len, uint8_t output[AES256_BLK_SIZE] );


class AES256cipher
{
public:
	static const size_t BLK_LEN = 16;
	static const size_t KEY_LEN = 32;

	AES256cipher( uint8_t key[AES256cipher::KEY_LEN] );
	void enc_block( const uint8_t input[AES256cipher::BLK_LEN], uint8_t output[AES256cipher::BLK_LEN] );
	void dec_block( const uint8_t input[AES256cipher::BLK_LEN], uint8_t output[AES256cipher::BLK_LEN] );
	
protected:
	uint32_t subKeyE_[SUBKEY_LEN / sizeof(uint32_t)];
	uint32_t subKeyD_[SUBKEY_LEN / sizeof(uint32_t)];
};

#endif