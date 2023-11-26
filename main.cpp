#include "sim.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstdint>

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout << "Error: invalid number of argumnets: N not provided\n";
		return 1;
	}
	
	//c++ dosen't allow for uninitialized objects, so I have to do this
	uint8_t buff[sizeof(simulator)] = {0};
	simulator* sim;
	
	try
	{
		if(argc >= 5)
		{
			sim = new (buff) simulator(argv[2], argv[3], argv[4]);
		}else
		{
			#ifndef DEBUG
			std::cout << "Error: invalid number of arguments: trace files or N not provided\n";
			return 1;
			#else
			sim = new (buff) simulator(); //use builtin prng in debug
			#endif
		}
	}catch(const std::exception& e)
	{
		std::cout << "Exception thrown during sim construction\n" << e.what() << "\n";
		return 2;
	}
	
	unsigned int N = std::atoi(argv[1]);
	try{
		sim->run(N);
	}catch(const std::exception& e)
	{
		std::cout << "Exception thrown during sim run\n" << e.what() << "\n";
		return 2;
	}
	std::cout << "OUTPUT " << sim->Da().mean() << " ";
	std::cout << sim->Da().variance() << " ";
	std::cout << sim->Dp().mean() << "\n";
	
	#ifdef DEBUG
	std::cout << "ped sample count:\t" << sim->Dp().count() << "\n";
	std::cout << "auto sample count:\t" << sim->Da().count() << "\n";
	#endif
	sim->~simulator();
	
	return 0;
}
