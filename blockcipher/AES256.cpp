#include "AES256.h"
#include "AESConst.h"

#define mulby2(x) ( ((x & 0x7F) << 1) ^ (x & 0x80 ? 0x1B : 0) )

void aes256_set_key ( uint8_t key[32], uint32_t subKeyE[60], uint32_t subKeyD[60] )
{
	const size_t Nk = AES256_KEY_SIZE / 4;
	AESBlock interim;
	size_t rconst = 1;

	memcpy( subKeyE, key, AES256_KEY_SIZE );

	for ( size_t i = AES256_KEY_SIZE / 4; i < (AES256_ROUNDS + 1) * 4; ++i )
	{
		interim.l[0] = subKeyE[i - 1];

		if (i % Nk == 0)
		{
			interim.l[0] = 
				(Sbox[interim.c[1]] ^ rconst) | 
				(Sbox[interim.c[2]] << 8) | 
				(Sbox[interim.c[3]] << 16) | 
				(Sbox[interim.c[0]] << 24);
			rconst = mulby2(rconst);
		}
		else if (i % Nk == 4 && Nk > 6)
		{
			for ( size_t i = 0; i < 4; ++i )
				interim.c[i] = Sbox[ interim.c[i] ];
		}
		
		subKeyE[i] = subKeyE[i - Nk] ^ interim.l[0];
	}
	
	/* subkey for decryption. */
	memcpy( subKeyD + 56, subKeyE, 16 );
	memcpy( subKeyD, subKeyE + 56, 16 );
	
	for (size_t i = 1; i < AES256_ROUNDS; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			interim.l[0] = subKeyE[(AES256_ROUNDS - i) * 4 + j];
			interim.l[0] = 
				D0[Sbox[interim.c[0]]] ^ D1[Sbox[interim.c[1]]] ^ 
				D2[Sbox[interim.c[2]]] ^ D3[Sbox[interim.c[3]]];
			subKeyD[i * 4 + j] = interim.l[0];
		}
	}
}

struct AESTable
{
	const size_t * pm;
	const uint32_t * subKey;
	const uint32_t * T0;
	const uint32_t * T1;
	const uint32_t * T2;
	const uint32_t * T3;
	const uint8_t * sbox;
};

static void aes256_block( AESBlock & blk, const AESTable & t )
{
	AESBlock interim;
	
	for ( size_t i = 0; i < 4; ++i )
		blk.l[i] ^= t.subKey[i];

	for (size_t i = 1; i < AES256_ROUNDS; ++i)
	{
		memcpy( interim.l, t.subKey + i * 4, AES256_BLK_SIZE );
		for ( size_t j = 0; j < 4; ++j )
		{
			interim.l[j] ^= t.T0[blk.c[t.pm[j * 4 + 0]]];
			interim.l[j] ^= t.T1[blk.c[t.pm[j * 4 + 1]]];
			interim.l[j] ^= t.T2[blk.c[t.pm[j * 4 + 2]]];
			interim.l[j] ^= t.T3[blk.c[t.pm[j * 4 + 3]]];
		}
		memcpy( blk.l, interim.l, AES256_BLK_SIZE );
	}
	
	for ( size_t i = 0; i < AES256_BLK_SIZE; ++i )
		interim.c[i] = t.sbox[blk.c[ t.pm[i] ]];

	for ( size_t i = 0; i < 4; ++i )
		blk.l[i] = interim.l[i] ^ t.subKey[56 + i];
}


void aes256_enc_block( AESBlock & block, uint32_t subKey[60] )
{
	const static size_t pm[AES256_BLK_SIZE] = 
	{
		0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12, 1, 6, 11
	};

	AESTable t;
	t.subKey = subKey;
	t.pm = pm;
	t.sbox = Sbox;
	t.T0 = E0; 	t.T1 = E1;	t.T2 = E2;	t.T3 = E3;

	aes256_block( block, t );
}

	
void aes256_dec_block( AESBlock & block, uint32_t subKey[60] )
{
	const static size_t pm[AES256_BLK_SIZE] = 
	{
		0, 13, 10, 7, 4, 1, 14, 11, 8, 5, 2, 15, 12, 9, 6, 3
	};
	
	AESTable t;
	t.subKey = subKey;
	t.pm = pm;
	t.sbox = Sboxinv;
	t.T0 = D0;	t.T1 = D1;	t.T2 = D2;	t.T3 = D3;

	aes256_block( block, t );
}


AES256cipher::AES256cipher( uint8_t key[AES256cipher::KEY_LEN] )
{
	aes256_set_key( key, subKeyE_, subKeyD_ );
}

void AES256cipher::enc_block( const uint8_t input[AES256cipher::BLK_LEN], uint8_t output[AES256cipher::BLK_LEN] )
{
	static AESBlock block;
	memcpy(block.c, input, AES256cipher::BLK_LEN );
	aes256_enc_block( block, subKeyE_);
	memcpy(output, block.c, AES256cipher::BLK_LEN );
}

void AES256cipher::dec_block( const uint8_t input[AES256cipher::BLK_LEN], uint8_t output[AES256cipher::BLK_LEN] )
{
	static AESBlock block;
	memcpy(block.c, input, AES256cipher::BLK_LEN );
	aes256_dec_block( block, subKeyD_);
	memcpy(output, block.c, AES256cipher::BLK_LEN );
}
