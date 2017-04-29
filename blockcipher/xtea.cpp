/*
  xtea.cpp - Xtea cipher
  Written by RomanGol, 2010
*/

#include <string.h>
#include "xtea.h"

const static uint32_t delta = 0x9E3779B9;

void xtea_key_setup( xtea_key & keyCtx, uint32_t key[4] )
{
	memcpy( keyCtx.key, key, KEY_LEN );
}

void xtea_encrypt( uint32_t output[2], uint32_t input[2], const xtea_key & keyCtx ) 
{
    output[0] = input[0];
    output[1] = input[1];
    uint32_t sum = 0;
    
    for ( size_t i = 0; i < NUM_ROUNDS; i++) 
    {
        output[0] += (((output[1] << 4) ^ (output[1] >> 5)) + output[1]) ^ (sum + keyCtx.key[sum & 3]);
        sum += delta;
        output[1] += (((output[0] << 4) ^ (output[0] >> 5)) + output[0]) ^ (sum + keyCtx.key[(sum>>11) & 3]);
    }
}
 
void xtea_decrypt( uint32_t output[2], uint32_t input[2], const xtea_key & keyCtx )
{
    output[0] = input[0];
    output[1] = input[1];
	uint32_t sum = 0xc6ef3720; // delta * NUM_ROUNDS;
   
    for (size_t i = 0; i < NUM_ROUNDS; i++) 
    {
        output[1] -= (((output[0] << 4) ^ (output[0] >> 5)) + output[0]) ^ (sum + keyCtx.key[(sum>>11) & 3]);
        sum -= delta;
        output[0] -= (((output[1] << 4) ^ (output[1] >> 5)) + output[1]) ^ (sum + keyCtx.key[sum & 3]);
    }
}
 