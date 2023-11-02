#include "sim.h"
#include <iostream>

int main(int argc, char** argv)
{
	try
	{
	#ifdef DEBUG

	#else

	#endif
	}catch(const std::exception& e)
	{
		std::cout << e.what() << "\n";
		return -1;
	}
	return 0;
}
