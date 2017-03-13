#include <iostream>
#include "bn_boost.h"
#include "../romangol.h"

using namespace boost::multiprecision;

int main()
{
	cpp_int k("566609156469778755953447051998424988694217750998");
	cpp_int q("738825468877092119325776555204444240783934885559");


	unsigned char roma[] = {0x03, 0x23, 0x37, 0x35, 0xf8, 0x77, 0x96};
	unsigned char buf[64] = {0};
	
	std::cout << std::hex << cppint_from_uint8( roma, 7 ) << std::endl;

	cppint_to_uint8( cppint_from_uint8( roma, 7 ), buf, 32 );
	
	outputChar(buf, 7);


	std::cout << std::dec <<k<<std::endl;


	cpp_int a(15);
	cpp_int b(443);
	cpp_int c(37);

	std::cout << std::dec << pow_mod(a,b,c) << std::endl;

	return 0;
}
