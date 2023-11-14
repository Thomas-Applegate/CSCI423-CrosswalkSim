#include "sim.h"
#include "event.h"

#ifdef DEBUG
#include <random>

static int s_gen_seed()
{
	static std::random_device rdev;
	return rdev();
}

simulator::simulator() : m_event_list(), m_auto_random(s_gen_seed()),
	m_ped_random(s_gen_seed()), m_button_random(s_gen_seed()), m_clock(0.0),
	m_Da(), m_Dp() {}
#endif

simulator::simulator(const std::string& auto_random,
	const std::string& ped_random, const std::string& button_random)
	: m_event_list(), m_auto_random(auto_random), m_ped_random(ped_random),
	m_button_random(button_random), m_clock(0.0), m_Da(), m_Dp() {}

void simulator::run(unsigned int N)
{
	unsigned int N_auto = N - 1;
	unsigned int N_ped = N -1;
	m_event_list.emplace(m_ped_random.exponential(6), event::Type::ped_arrival,
		m_ped_random.uniform(2.6, 4.1));
	m_event_list.emplace(m_auto_random.exponential(8), event::Type::auto_arrival,
		m_auto_random.uniform(25, 35));
	
	while(!m_event_list.empty())
	{
		auto current_event = m_event_list.begin();
		m_clock = current_event->m_at;
		
		switch(current_event->m_type)
		{
			case event::Type::auto_arrival:
				if(N_auto > 0)
				{
					m_event_list.emplace(m_clock + m_auto_random.exponential(8),
						event::Type::auto_arrival, m_auto_random.uniform(25, 35));
					N_auto--;
				}
				break;
			case event::Type::ped_arrival:
				if(N_ped > 0)
				{
					m_event_list.emplace(m_clock + m_ped_random.exponential(6),
						event::Type::ped_arrival, m_ped_random.uniform(2.6, 4.1));
					N_ped--;
				}
				break;
			case event::Type::ped_at_button:
				break;
			case event::Type::ped_impatient:
				break;
			case event::Type::green_expires:
				break;
			case event::Type::yellow_expires:
				break;
			case event::Type::red_expires:
				break;
			case event::Type::auto_exit:
				break;
			case event::Type::ped_exit:
				break;
		}
		
		m_event_list.erase(current_event);
	}
}

double simulator::clock() const { return m_clock; }
const welford<double>& simulator::Da() const { return m_Da; }
const welford<double>& simulator::Dp() const { return m_Dp; }
