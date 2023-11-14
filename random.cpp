#include "random.h"
#include <stdexcept>
#include <cmath>

Random::Random(int seed) : m_prng(seed), m_rfile()
{
	m_rfile.exceptions(m_rfile.failbit | m_rfile.badbit | m_rfile.eofbit);
}
Random::Random(const std::string& s) : m_prng(), m_rfile(s)
{
	m_rfile.exceptions(m_rfile.failbit | m_rfile.badbit | m_rfile.eofbit);
}

double Random::operator()()
{
	if(m_rfile.is_open()) //usefile
	{
		double ret;
		m_rfile >> ret;
		return ret;
	}else{//use prng
		static std::uniform_real_distribution
			dist(std::numeric_limits<double>::lowest(), 1.0);
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
	return a + (b-1) * (*this)();
}

long Random::equilikely(long a, long b)
{
	if(a >= b) throw std::invalid_argument("a must be less than b");
	return a + (long)((b-a+1) * (*this)());
}

double Random::exponential(double mu)
{
	if(mu <= 0.0) throw std::domain_error("mu must be greater than 0");
	return -mu * std::log(1.0 - (*this)());
}


long Random::geometric(double p)
{
	if(p >= 1.0 || p <= 0.0) throw std::domain_error("p must be greater than 0 and less than 1");
	return (long)(log(1- (*this)())/log(p));
}

bool Random::bernouli(double p)
{
	if(p > 1.0 || p < 0.0) throw std::domain_error("p must be between 0 and 1");
	return (*this)() <= p ? true : false;
}
