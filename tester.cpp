#include "./pubkey/ecc.h"
#include "./pubkey/RSA.h"
#include "./pubkey/DSA.h"
#include "./hash/sm3.h"
#include "./hash/sha256.h"
#include "./hash/sha1.h"
#include "romangol.h"
#include "./prng/random.h"

using namespace boost::multiprecision;

void outputChar(const unsigned char * const text, size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		printf("0x%02x, ", text[i]);
		if (i % 16 == 15)
			puts("");
	}
	puts("");
}

void outputDword(unsigned int * text, size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		printf("0x%08x ", text[i]);
		if (i % 4 == 3)
			puts("");
	}

	puts("");
}


/*
Test Vectors (from FIPS PUB 180-1)
"abc"
A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
A million repetitions of "a"
34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F
*/

int test_sha1()
{
	uint8_t vec1[] = "abc";
	uint8_t vec2[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
	uint8_t digest[20];

	sha1(vec2, sizeof(vec2) - 1, digest);
	for (size_t i = 0; i < 20; ++i)
		printf("%02x", digest[i]);
	puts("");

	return 0;
}

int test_sha256()
{
	puts("test_sha256()");
	unsigned char msg[] = "RomanGol";
	unsigned char dgst[SHA256_DIGEST_LENGTH] = {0};
	sha256( msg, 8, dgst );

	// checkpoint charlie
	// should be
	// 0x63, 0xd8, 0x9b, 0xca, 0x91, 0x87, 0x68, 0x54, 0x91, 0xd3, 0xe9, 0x3e, 0x11, 0x48, 0x02, 0x5f,
	// 0xdf, 0x9a, 0x19, 0xe6, 0x60, 0xa6, 0xdf, 0x8d, 0x19, 0x56, 0xa5, 0xce, 0xd0, 0x7e, 0x86, 0x6c
	outputChar(dgst, SHA256_DIGEST_LENGTH);

	return 0;
}

void test_ecdsa()
{
	uint8_t rhash[] = // SHA256 of "RomanGol"
	{
		0x63, 0xd8, 0x9b, 0xca, 0x91, 0x87, 0x68, 0x54, 0x91, 0xd3, 0xe9, 0x3e, 0x11, 0x48, 0x02, 0x5f,
		0xdf, 0x9a, 0x19, 0xe6, 0x60, 0xa6, 0xdf, 0x8d, 0x19, 0x56, 0xa5, 0xce, 0xd0, 0x7e, 0x86, 0x6c
	};

	Curve secp256r1; // y**2 = x**3 - 3 * x - b

	secp256r1.blockLen = CURVE_secp256r1_LEN;
	secp256r1.p = cpp_int("0xffffffff00000001000000000000000000000000ffffffffffffffffffffffff");
	secp256r1.a = 3;
	secp256r1.b = cpp_int("0x5ac635d8aa3a93e7b3ebbd55769886bc651d06b0cc53b0f63bce3c3e27d2604b");
	secp256r1.n = cpp_int("0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551");
	secp256r1.G.x = cpp_int("0x6b17d1f2e12c4247f8bce6e563a440f277037d812deb33a0f4a13945d898c296");
	secp256r1.G.y = cpp_int("0x4fe342e2fe1a7f9b8ee7eb4a7c0f9e162bce33576b315ececbb6406837bf51f5");

	uint8_t signature[CURVE_secp256r1_LEN * 2];
	uint8_t priKey[CURVE_secp256r1_LEN];

	random_fill(priKey, CURVE_secp256r1_LEN);
	signature[0] &= 0x7f;

	EPoint pubKey = mul(cppint_from_uint8(priKey, CURVE_secp256r1_LEN), secp256r1.G, secp256r1);

	ecdsa_sign(rhash, sizeof(rhash), signature, priKey, secp256r1);

	if ( ecdsa_verify(rhash, sizeof(rhash), signature, pubKey, secp256r1) )
		puts("bingo");
	else
		puts("what?");

	Curve sm2;

	unsigned char sm2_param_a[] =
	{
	0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
	sm2.blockLen = CURVE_secp256r1_LEN;
	sm2.p = cppint_from_uint8(sm2_param_a, sizeof(sm2_param_a));
	sm2.a = 3;
	sm2.b = cpp_int("0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93");
	sm2.b = sm2.p - sm2.b;
	sm2.n = cpp_int("0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123");
	sm2.G.x = cpp_int("0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7");
	sm2.G.y = cpp_int("0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0");

	pubKey = mul(cppint_from_uint8(priKey, CURVE_secp256r1_LEN), secp256r1.G, secp256r1);

	ecdsa_sign(rhash, sizeof(rhash), signature, priKey, secp256r1);

	if (ecdsa_verify(rhash, sizeof(rhash), signature, pubKey, secp256r1))
		puts("bingo");
	else
		puts("what?");


}

void test_int128()
{
	puts("test_int128()");
	int128_t l( "43017329585047238859772730236143539894" );
	int128_t r( "172021209466525381274003664777717609141" );
	
	// checkpoint charlie
	// should be 0xA136C268AD83FF0B260660D514E7E403
	std::cout << "0x205CD618E80ADAD75A7B150ADCD616B6 ^ 0x816A1470458925DC7C7D75DFC831F2B5 = ";
	std::cout << std::hex << (l ^ r) << std::endl;
}

void test_gcd_imod()
{
	puts("test_gcd_imod()");
	// checkpoint charlie
	// should be 7 instead of -4
	std::cout << "inv_mod(8, 11) = " << inv_mod(8,11) << std::endl;

	cpp_int k("566609156469778755953447051998424988694217750998");
	cpp_int q("738825468877092119325776555204444240783934885559");

	// checkpoint charlie
	// should be 1
	std::cout << "gcd(566609156469778755953447051998424988694217750998, 738825468877092119325776555204444240783934885559) = ";
	std::cout << gcd(k,q) << std::endl;
	
	// checkpoint charlie
	// should be 84484872257539608970510254229946568943528482405
	std::cout << "inv_mod(566609156469778755953447051998424988694217750998, 738825468877092119325776555204444240783934885559) = ";
	std::cout << inv_mod(k,q) << std::endl;
}


int test_RSA()
{
	puts("test_RSA()");
	RSAKey k;
	k.n = cpp_int
	("0xa3fe266f0b5ee57512cdf6c4c016d9ba21cba91e7c82d2b13399027520aebcc264bf2b124a9ff4a9bd9c3282b0a24ff13de2faed44c02b6e74c6ee71fdcf265ebac5c15e69f1a722e509b81687c416ef77d3ec43326a089cd6875c7eb0bcd7c3762f8db8d6ded8ecdb32bce7d1ede900d8e0da7ea797f4872e5ba97e40e220dd");
	k.d = cpp_int
	("0x02c47d3d8e819179c2517b82a283403565f09bf009c5e36c5e248ce8111f21cc5e0ae3dd312935e5cf6f5d62db3c42011e3c4063d4af318c71a811ca16b072434c0f62edbe0c38b16cd13e382039d5d246e71a6ff0272d4aa3dfbac8d4de4b9fc6250d97c17cc66941679f167ba4e5afc350a21e93660be343562ac2e2dc7af9");
	k.p = cpp_int
	("0xbcf5618f955546f2490d72523f3daacd9ff09a0e160d4230b3954f206d7360ad6b466c19dac90f3e02b364e5edc99f70e68ea14f57b0d95bf366bd1c97078eb7");
	k.q = cpp_int
	("0xde2d31b77541785812573acd705894d811e5c9dd7b013e5d0d90af437e47f6602a32cc413911a51ac038dd4e916bbbc0c9075c166cec5d108f5781337539f90b");
	k.e = 0x10001;
	k.keyLen = 256;
	
	printf("key len %zd\n", calc_rsa_key(k));

	// cpp_int cipher = rsa_encrypt( 1207, k );
	// std::cout << rsa_decrypt( cipher, k ) << std::endl;

	uint8_t rhash[] = // SHA256 of "RomanGol"
	{
		0x63, 0xd8, 0x9b, 0xca, 0x91, 0x87, 0x68, 0x54, 0x91, 0xd3, 0xe9, 0x3e, 0x11, 0x48, 0x02, 0x5f,
		0xdf, 0x9a, 0x19, 0xe6, 0x60, 0xa6, 0xdf, 0x8d, 0x19, 0x56, 0xa5, 0xce, 0xd0, 0x7e, 0x86, 0x6c
	};
	unsigned char ciph[128];
	rsa_enc_oaep( rhash, 32, ciph, k );
	
	rsa_dec_oaep( ciph, 128, rhash, k );
	outputChar(rhash, 32);
	
	return 1;
}

int test_sm2()
{
	puts("test_sm2()");
	Curve sm2;
	
	unsigned char sm2_param_a[] = 
	{
		0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFC
	};
	sm2.a = cppint_from_uint8( sm2_param_a, sizeof(sm2_param_a) );
	//sm2.a = cpp_int("0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC");
	sm2.b = cpp_int("0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93");
	sm2.n = cpp_int("0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF");
	cpp_int order = cpp_int("0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123");
	cpp_int Gx = cpp_int("0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7");
	cpp_int Gy = cpp_int("0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0");
	
	EPoint G;
	G.x = Gx;
	G.y = Gy;

	cpp_int k("0x3ec465bfe7e17fc2eea01abf1a4b5baef47262f55a99773d28aa2020ae8a9fe5");
	cpp_int d("0x75ae6de8a376874d55f487b4ac50e6de0869c2bb13b3406bdd1bcc35d444cc6f");
	
	EPoint kg = mul(k, G, sm2);
	puts("kg.x");
	std::cout << kg.x << std::endl;

	EPoint dg = mul(d, G, sm2);
	puts("dg.x");
	std::cout << dg.x << std::endl;
	
	EPoint sg1 = mul(k, dg, sm2);
	std::cout << sg1.x << std::endl;
	
	EPoint sg2 = mul(d, kg, sm2);
	std::cout << sg2.x << std::endl;

	return 0;
}

void test_hash()
{
	unsigned char dgst[SM3_DIGEST_LENGTH];
	
	unsigned char username[] = "ALICE123@YAHOO.COM";
	
	unsigned char msg[] = 
	{
		0x00, 0x90,
		0x41, 0x4C, 0x49, 0x43, 0x45, 0x31, 0x32, 0x33, 0x40, 0x59, 0x41, 0x48, 0x4F, 0x4F, 0x2E, 0x43, 0x4F, 0x4D, 
	};

	sm3( msg, sizeof(msg), msg);
	outputChar( msg, SM3_DIGEST_LENGTH );
	
	unsigned char ZA[] = 
	{
		0xF4, 0xA3, 0x84, 0x89, 0xE3, 0x2B, 0x45, 0xB6, 
		0xF8, 0x76, 0xE3, 0xAC, 0x21, 0x68, 0xCA, 0x39, 
		0x23, 0x62, 0xDC, 0x8F, 0x23, 0x45, 0x9C, 0x1D, 
		0x11, 0x46, 0xFC, 0x3D, 0xBF, 0xB7, 0xBC, 0x9A, 
		0x6D, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x20, 
		0x64, 0x69, 0x67, 0x65, 0x73, 0x74, 
	};
	
	sm3( ZA, sizeof(ZA), dgst);

	// check point charlie: should be 
	// 0xb5, 0x24, 0xf5, 0x52, 0xcd, 0x82, 0xb8, 0xb0, 0x28, 0x47, 0x6e, 0x00, 0x5c, 0x37, 0x7f, 0xb1,
	// 0x9a, 0x87, 0xe6, 0xfc, 0x68, 0x2d, 0x48, 0xbb, 0x5d, 0x42, 0xe3, 0xd9, 0xb9, 0xef, 0xfe, 0x76,
	outputChar( dgst, SM3_DIGEST_LENGTH );
}


void test_file_hash()
{
	FILE * fp = fopen("test.dat", "rb");
	
	unsigned char buffer[1024];
	size_t len = fread( buffer, 1, sizeof(buffer), fp );
	
	sm3_ctx_t ctx;
	sm3_init(ctx);
	
	while ( len != 0 )
	{
		sm3_update(ctx, buffer, len);
		len = fread( buffer, 1, sizeof(buffer), fp );
	}	

	unsigned char dgst[SM3_DIGEST_LENGTH];
	sm3_final(ctx, dgst);
	
	outputChar( dgst, SM3_DIGEST_LENGTH );
}

int test_random()
{
	unsigned char buf[1024];
	random_fill( buf, sizeof(buf) );
	
	outputChar( buf, sizeof(buf) );
	
	return 0;
}


int main()
{
	test_ecdsa();

	return 0;
	
	test_hash();
	test_file_hash();
	test_RSA();
	test_sm2();
	test_random();
	 test_sha256();
}
