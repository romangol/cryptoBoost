#include "sm4.h"

#include <cstdint>

class SM4cipher
{
public:
	static const size_t BLK_LEN = 16;
	static const size_t KEY_LEN = 16;

	SM4cipher( uint8_t key[SM4cipher::KEY_LEN] );
	void enc_block( const uint8_t input[SM4cipher::BLK_LEN], uint8_t output[SM4cipher::BLK_LEN] );
	void dec_block( const uint8_t input[SM4cipher::BLK_LEN], uint8_t output[SM4cipher::BLK_LEN] );
	
protected:
	sm4_ctx_t ctx_;
};
