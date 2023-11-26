#include "random.h"
#include <stdexcept>
#include <cmath>
#include <iostream>

Random::Random(int seed) : m_prng(seed), m_rfile() {}
Random::Random(const std::string& s) : m_prng(), m_rfile(s)
{
	if(!m_rfile.good())
	{
		throw std::runtime_error("cannot open file: " + s);
	}
}

double Random::operator()()
{
	if(m_rfile.is_open()) //usefile
	{
		if(m_rfile.eof())
		{
			throw std::runtime_error("random file is eof");
		}
		if(m_rfile.fail())
		{
			throw std::runtime_error("random file has failed, but is not eof");
		}
		double ret = 1e-6;
		m_rfile >> ret;
		return ret;
	}else{//use prng
		std::uniform_real_distribution dist(1e-10, 1.0);
		return dist(m_prng);
	}
}

void Random::use_prng(int seed)
{
	m_rfile.close();
	m_prng.seed(seed);
}

void Random::use_file(const std::string& s)
{
	m_rfile.close();
	m_rfile.open(s);
}

double Random::uniform(double a, double b)
{
	if(a >= b) throw std::invalid_argument("a must be less than b");
	double r = (*this)();
	return a + (b-a) * r;
}

long Random::equilikely(long a, long b)
{
	if(a >= b) throw std::invalid_argument("a must be less than b");
	double r = (*this)();
	return a + (long)((b-a+1) * r);
}

double Random::exponential(double mu)
{
	if(mu <= 0.0) throw std::domain_error("mu must be greater than 0");
	double r = (*this)();
	return -(mu * std::log(r));
}


long Random::geometric(double p)
{
	if(p >= 1.0 || p <= 0.0) throw std::domain_error("p must be greater than 0 and less than 1");
	double r = (*this)();
	return (long)(log(1- r)/log(p));
}

bool Random::bernouli(double p)
{
	if(p > 1.0 || p < 0.0) throw std::domain_error("p must be between 0 and 1");
	double r = (*this)();
	return r <= p ? true : false;
}
