#ifndef _CRYPTO_BOOST_BLOCK_CIPHER_
#define _CRYPTO_BOOST_BLOCK_CIPHER_

#include <cstdint>
#include <cstring>

typedef const unsigned char * const 		const_buf;
typedef unsigned char  *					buf;

template<typename T>
class Blockcipher
{
public:
	Blockcipher ( uint8_t key[T::KEY_LEN], uint8_t iv[T::BLK_LEN] ) : cryptor(key)
	{
		memcpy( key_, key, T::KEY_LEN );
		memcpy( iv_, iv, T::BLK_LEN );
	}

	~Blockcipher ()
	{
		memset( key_, 0, T::KEY_LEN );
		memset( iv_, 0, T::BLK_LEN );
	}

	void encrypt_ecb( const_buf input, size_t len, buf output )
	{
		if ( len % T::BLK_LEN != 0 )
			return;

		for ( size_t i = 0; i < len; i += T::BLK_LEN )
			cryptor.enc_block( input + i, output + i );
	}


	void decrypt_ecb( const_buf input, size_t len, buf output )
	{
		if ( len % T::BLK_LEN != 0 )
			return;

		for ( size_t i = 0; i < len; i += T::BLK_LEN )
			cryptor.dec_block( input + i, output + i );
	}

	void encrypt_cbc( const_buf input, size_t len, buf output )
	{
		if ( len % T::BLK_LEN != 0 )
			return;

		uint8_t iv[T::BLK_LEN];
		memcpy(iv, iv_, T::BLK_LEN);
	
		for ( size_t i = 0; i < len; i += T::BLK_LEN )
		{
			for( size_t j = 0; j < T::BLK_LEN; ++j )
				output[i + j] = input[i + j] ^ iv[j];

			cryptor.enc_block( output + i, output + i );
			memcpy( iv, output + i, T::BLK_LEN );
		}
	}

	void decrypt_cbc( const_buf input, size_t len, buf output )
	{
		if ( len % T::BLK_LEN != 0 )
			return;

		uint8_t iv[T::BLK_LEN];
		memcpy(iv, iv_, T::BLK_LEN);

		uint8_t interim[T::BLK_LEN];

		for ( size_t i = 0; i < len; i += T::BLK_LEN )
		{
			memcpy( interim, input + i, T::BLK_LEN );
			cryptor.dec_block( input + i, output + i );

			for( size_t j = 0; j < T::BLK_LEN; ++j )
				output[i + j] = output[i + j] ^ iv[j];

			memcpy( iv, interim, T::BLK_LEN );
		}
	}


	void encrypt_cfb( const_buf input, size_t len, buf output )
	{
		uint8_t iv[T::BLK_LEN];
		memcpy(iv, iv_, T::BLK_LEN);
		
		for ( size_t i = 0; i < len; ++i )
		{
			if ( i % T::BLK_LEN == 0 )
			{
				cryptor.enc_block( iv, iv );
			}
			
			output[i] = input[i] ^ iv[i % T::BLK_LEN];
			iv[i % T::BLK_LEN] = output[i];
		}
	}

	void decrypt_cfb( const_buf input, size_t len, buf output )
	{
		uint8_t iv[T::BLK_LEN];
		uint8_t interim[T::BLK_LEN];
		memcpy(iv, iv_, T::BLK_LEN);
		memcpy(interim, iv, T::BLK_LEN);
		
		for ( size_t i = 0; i < len; ++i )
		{
			if ( i % T::BLK_LEN == 0 )
			{
				cryptor.enc_block( interim, iv );
			}
			interim[i % T::BLK_LEN] = input[i];
			output[i] = input[i] ^ iv[i % T::BLK_LEN];
		}
	}

	void encrypt_ofb( const_buf input, size_t len, buf output )
	{
		uint8_t iv[T::BLK_LEN];
		memcpy(iv, iv_, T::BLK_LEN);
		
		for ( size_t i = 0; i < len; ++i )
		{
			if ( i % T::BLK_LEN == 0 )
				cryptor.enc_block( iv, iv );
			output[i] = input[i] ^ iv[i % T::BLK_LEN];
		}
	}

	void decrypt_ofb( const_buf input, size_t len, buf output )
	{
		uint8_t iv[T::BLK_LEN];
		memcpy(iv, iv_, T::BLK_LEN);
		
		for ( size_t i = 0; i < len; ++i )
		{
			if ( i % T::BLK_LEN == 0 )
				cryptor.enc_block( iv, iv );
			output[i] = input[i] ^ iv[i % T::BLK_LEN];
		}
	}


protected:
	T cryptor;
	uint8_t key_[T::KEY_LEN];
	uint8_t iv_[T::BLK_LEN];
};

#endif