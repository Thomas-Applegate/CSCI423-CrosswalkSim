#include "sim.h"

#ifdef DEBUG
simulator::simulator() : m_event_list(), m_auto_random(), m_ped_random(),
	m_button_random(), m_clock(0.0), m_Da(), m_Dp() {}
#endif

simulator::simulator(const std::string& auto_random,
	const std::string& ped_random, const std::string& button_random)
	: m_event_list(), m_auto_random(auto_random), m_ped_random(ped_random),
	m_button_random(button_random), m_clock(0.0), m_Da(), m_Dp() {}

void simulator::run(unsigned int N)
{
	//TODO
}

double simulator::clock() const { return m_clock; }
const welford<double>& simulator::Da() const { return m_Da; }
const welford<double>& simulator::Dp() const { return m_Dp; }
