#include <iostream>
#include "bn_boost.h"

using namespace boost::multiprecision;

void test_int128()
{
	int128_t l( "43017329585047238859772730236143539894" );
	int128_t r( "172021209466525381274003664777717609141" );
	
	// checkpoint charlie
	// should be 0xA136C268AD83FF0B260660D514E7E403
	std::cout << "0x205CD618E80ADAD75A7B150ADCD616B6 ^ 0x816A1470458925DC7C7D75DFC831F2B5 = ";
	std::cout << std::hex << (l ^ r) << std::endl;
}

void test_gcd_imod()
{
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

int main()
{
	test_int128();
	// test_gcd_imod();
	return 0;
}