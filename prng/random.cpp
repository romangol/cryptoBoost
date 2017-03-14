#include "random.h"
#include <random>

void random_fill( uint8_t * buffer, size_t len )
{
	std::random_device rd;

	for (size_t i = 0; i < len; ++i )
	{
		do
		{
			buffer[i] = rd() & 0xff;
		}
		while (buffer[i] == 0);
	}
}
