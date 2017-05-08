#include <cstdint>
#include <string.h>
#include <stdio.h>
#include "sm4.hpp"

#include "sm4.c"

SM4cipher::SM4cipher( uint8_t key[SM4cipher::KEY_LEN] )
{
	sm4_setkey( ctx_, key );
}

void SM4cipher::enc_block( const uint8_t input[SM4cipher::BLK_LEN], uint8_t output[SM4cipher::BLK_LEN] )
{
	sm4_block( ctx_.skEnc, input, output );
}

void SM4cipher::dec_block( const uint8_t input[SM4cipher::BLK_LEN], uint8_t output[SM4cipher::BLK_LEN] )
{
	sm4_block( ctx_.skDec, input, output );
}

