#include "sim.h"
#include <iostream>
#include <stdexcept>
#include <memory>
#include <cstdlib>

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout << "Error: invalid number of argumnets: N not provided\n";
		return -1;
	}
	
	try
	{
		//c++ dosen't allow for uninitialized objects, so I have to do this
		std::unique_ptr<simulator> sim;
		if(argc >= 5)
		{
			sim = std::make_unique<simulator>(argv[2], argv[3], argv[4]);
		}else
		{
			#ifndef DEBUG
			std::cout << "Error: invalid number of arguments: trace files not provided\n";
			return -1;
			#else
			sim = std::make_unique<simulator>(); //use builtin prng in debug
			#endif
		}
		
		unsigned int N = std::atoi(argv[1]);
		sim->run(N);
		std::cout << "OUTPUT " << sim->Da().mean() << " ";
		std::cout << sim->Da().variance() << " ";
		std::cout << sim->Dp().mean() << "\n";
	}catch(const std::exception& e)
	{
		std::cout << e.what() << "\n";
		return -1;
	}
	return 0;
}
