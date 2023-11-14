#pragma once
#include <fstream>
#include <random>

class Random
{
public:
	Random(int seed = 0);
	Random(const std::string& s);
	
	double operator()();
	
	void use_prng(int seed = 0);
	void use_file(const std::string& s);
	
	double uniform(double a, double b);
	long equilikely(long a, long b);
	double exponential(double mu);
	long geometric(double p);
	bool bernouli(double p);
private:
	std::default_random_engine m_prng;
	std::ifstream m_rfile;
};
