#include <iostream>
#include <boost/multiprecision/miller_rabin.hpp>
#include "bn_boost.h"

using namespace boost::multiprecision;

int main()
{
	cpp_int k("0xaa84979232aa54b0474c17acc50eceada5205b402445aa9e0c2eabd3ba65f6db9c2b5feb05d6904cdc112b5274cb9d81c0c0eb2955c360517e4f45e5cc93ca8a");

	
	if ( miller_rabin_test(k, 32) )
		std::cout << "yes" << std::endl;

	cpp_int a(15);
	cpp_int b(443);
	cpp_int c(37);

	std::cout << std::dec << powm(a,b,c) << std::endl;

	return 0;
}
