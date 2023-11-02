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
		
	}else{//use prng
		
	}
	return -1;
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
