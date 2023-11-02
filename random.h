#pragma once
#include <fstream>
#include <random>

class random
{
public:
	random();
	random(const std::string& s);
	
	double operator()();
	
	void use_prng();
	void use_file(const std::string& s);
private:
	std::random_device m_prng;
	std::ifstream m_rfile;
};
