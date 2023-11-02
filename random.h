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
	
	double uniform(double a, double b);
	long equilikely(long a, long b);
	double exponential(double mu);
	long geometric(double p);
	bool bernouli(double p);
private:
	std::random_device m_prng;
	std::ifstream m_rfile;
};
