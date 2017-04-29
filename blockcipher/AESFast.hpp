#include "AESFast.h"

class AES256cipher
{
public:
	static const size_t BLK_LEN = 16;
	static const size_t KEY_LEN = 32;

	AES256cipher( uint8_t key[AES256cipher::KEY_LEN] );
	void transform( const uint8_t input[AES256cipher::BLK_LEN], uint8_t output[AES256cipher::BLK_LEN] );
	void transform_inv( const uint8_t input[AES256cipher::BLK_LEN], uint8_t output[AES256cipher::BLK_LEN] );
	
protected:
	AESKey encKey_;
	AESKey decKey_;
};
