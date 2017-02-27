#include "random.h"
#include <random>

#define WINDOWS 1

#if WINDOWS
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
#endif

#if LINUX
void random_fill( uint8_t buffer, size_t len )
{
	int32_t fd = open("/dev/urandom", O_RDONLY);
	
	if(fd == -1)
	{
		puts("unable to open random file");
		return;
	}
	
	size_t readBytes = read(fd, buffer, len);

	if(bytes != len)
		puts("inadequate entropy");

	close(fd);
}
#endif