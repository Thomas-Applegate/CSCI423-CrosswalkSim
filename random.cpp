#include "random.h"

random::random() : m_prng(), m_rfile()
{
	m_rfile.exceptions(m_rfile.failbit | m_rfile.badbit | m_rfile.eofbit);
}
random::random(const std::string& s) : m_prng(), m_rfile(s)
{
	m_rfile.exceptions(m_rfile.failbit | m_rfile.badbit | m_rfile.eofbit);
}

double random::operator()()
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

void random::use_prng()
{
	m_rfile.close();
}

void random::use_file(const std::string& s)
{
	m_rfile.close();
	m_rfile.open(s);
}
