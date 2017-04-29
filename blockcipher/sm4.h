#ifndef _SM4_H_
#define _SM4_H_


/*
* SM4 Encryption alogrithm (SMS4 algorithm)
* GM/T 0002-2012 Chinese National Standard ref:http://www.oscca.gov.cn/
*/

// Test vector 1
// plain: 01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
// key:   01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
// 	   round key and temp computing result:
// 	   rk[ 0] = f12186f9 X[ 0] = 27fad345
// 		   rk[ 1] = 41662b61 X[ 1] = a18b4cb2
// 		   rk[ 2] = 5a6ab19a X[ 2] = 11c1e22a
// 		   rk[ 3] = 7ba92077 X[ 3] = cc13e2ee
// 		   rk[ 4] = 367360f4 X[ 4] = f87c5bd5
// 		   rk[ 5] = 776a0c61 X[ 5] = 33220757
// 		   rk[ 6] = b6bb89b3 X[ 6] = 77f4c297
// 		   rk[ 7] = 24763151 X[ 7] = 7a96f2eb
// 		   rk[ 8] = a520307c X[ 8] = 27dac07f
// 		   rk[ 9] = b7584dbd X[ 9] = 42dd0f19
// 		   rk[10] = c30753ed X[10] = b8a5da02
// 		   rk[11] = 7ee55b57 X[11] = 907127fa
// 		   rk[12] = 6988608c X[12] = 8b952b83
// 		   rk[13] = 30d895b7 X[13] = d42b7c59
// 		   rk[14] = 44ba14af X[14] = 2ffc5831
// 		   rk[15] = 104495a1 X[15] = f69e6888
// 		   rk[16] = d120b428 X[16] = af2432c4
// 		   rk[17] = 73b55fa3 X[17] = ed1ec85e
// 		   rk[18] = cc874966 X[18] = 55a3ba22
// 		   rk[19] = 92244439 X[19] = 124b18aa
// 		   rk[20] = e89e641f X[20] = 6ae7725f
// 		   rk[21] = 98ca015a X[21] = f4cba1f9
// 		   rk[22] = c7159060 X[22] = 1dcdfa10
// 		   rk[23] = 99e1fd2e X[23] = 2ff60603
// 		   rk[24] = b79bd80c X[24] = eff24fdc
// 		   rk[25] = 1d2115b0 X[25] = 6fe46b75
// 		   rk[26] = 0e228aeb X[26] = 893450ad
// 		   rk[27] = f1780c81 X[27] = 7b938f4c
// 		   rk[28] = 428d3654 X[28] = 536e4246
// 		   rk[29] = 62293496 X[29] = 86b3e94f
// 		   rk[30] = 01cf72e5 X[30] = d206965e
// 		   rk[31] = 9124a012 X[31] = 681edf34
// cypher: 68 1e df 34 d2 06 96 5e 86 b3 e9 4f 53 6e 42 46
// 		
// test vector 2
// the same key and plain 1000000 times coumpting 
// plain:  01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
// key:    01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
// cypher: 59 52 98 c7 c6 fd 27 1f 04 02 f8 04 c3 3d 3f 66

typedef const unsigned char * const 		const_buf;
typedef unsigned char  *					buf;

const static size_t SM4_BLOCK_SIZE = 16;


struct sm4_ctx_t
{
    unsigned int skEnc[32];
    unsigned int skDec[32];
};

void sm4_setkey( sm4_ctx_t & ctx, uint8_t key[SM4_BLOCK_SIZE] );
void sm4_encrypt_ecb( buf output, const_buf input, size_t length, uint8_t key[SM4_BLOCK_SIZE] );
void sm4_decrypt_ecb( buf output, const_buf input, size_t length, uint8_t key[SM4_BLOCK_SIZE] );
void sm4_encrypt_cbc( buf output, const_buf input, size_t length, uint8_t key[SM4_BLOCK_SIZE], uint8_t iv[SM4_BLOCK_SIZE] );
void sm4_decrypt_cbc( buf output, const_buf input, size_t length, uint8_t key[SM4_BLOCK_SIZE], uint8_t iv[SM4_BLOCK_SIZE] );

#endif /* sm4.h */