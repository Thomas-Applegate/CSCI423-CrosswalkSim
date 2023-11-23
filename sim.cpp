#include "sim.h"
#include "event.h"
#include <stdexcept>

entity_info::entity_info(double at, double speed) : at(at), speed(speed) {}

#ifdef DEBUG
#include <random>

int s_gen_seed()
{
	std::random_device rdev;
	return rdev();
}

simulator::simulator() : m_event_list(), m_auto_random(s_gen_seed()),
	m_ped_random(s_gen_seed()), m_button_random(s_gen_seed()), m_clock(0.0),
	m_Da(), m_Dp(), m_color(color::green), m_walk_signal(false),
	m_button_pressed(false) {}
#endif

simulator::simulator(const std::string& auto_random,
	const std::string& ped_random, const std::string& button_random)
	: m_event_list(), m_auto_random(auto_random), m_ped_random(ped_random),
	m_button_random(button_random), m_clock(0.0), m_Da(), m_Dp(),
	m_color(color::green), m_walk_signal(false), m_button_pressed(false) {}
	
double simulator::m_compute_ped_delay(entity_info i)
{
	return (m_clock - i.at) - (422.0/i.speed);
}

double simulator::m_compute_auto_delay(entity_info i)
{
	//TODO
	return -1.0;
}

void simulator::m_start_green_timer()
{
	if(m_green_timer < 0)
	{
		m_green_timer = m_clock + 35.0;
		m_event_list.emplace(m_green_timer, event::Type::green_expires);
	}
}
void simulator::m_start_yellow_timer()
{
	if(m_yellow_timer < 0)
	{
		m_yellow_timer = m_clock + 8.0;
		m_event_list.emplace(m_yellow_timer, event::Type::yellow_expires);
	}
}

void simulator::m_start_red_timer()
{
	if(m_red_timer < 0)
	{
		m_red_timer = m_clock + 18.0;
		m_event_list.emplace(m_red_timer, event::Type::red_expires);
	}
}

void simulator::run(unsigned int N)
{
	unsigned int N_auto = N - 1;
	unsigned int N_ped = N -1;
	m_event_list.emplace(m_ped_random.exponential(10.0), event::Type::ped_arrival,
		m_ped_random.uniform(2.6, 4.1));
	m_event_list.emplace(m_auto_random.exponential(7.5), event::Type::auto_arrival,
		m_auto_random.uniform(36.67, 51.33));
	
	while(!m_event_list.empty())
	{
		auto current_event = m_event_list.begin();
		m_clock = current_event->m_at;
		
		switch(current_event->m_type)
		{
		case event::Type::auto_arrival:
			if(N_auto > 0)
			{
				m_event_list.emplace(m_clock + m_auto_random.exponential(7.5),
					event::Type::auto_arrival, m_auto_random.uniform(36.67, 51.33));
				N_auto--;
			}
			m_autos.try_emplace(current_event->m_id, current_event->m_at, current_event->m_speed);
			break;
		case event::Type::ped_arrival:
			if(N_ped > 0)
			{
				m_event_list.emplace(m_clock + m_ped_random.exponential(10.0),
					event::Type::ped_arrival, m_ped_random.uniform(2.6, 4.1));
				N_ped--;
			}
			m_peds.try_emplace(current_event->m_id, current_event->m_at, current_event->m_speed);
			break;
		case event::Type::ped_at_button:
			break;
		case event::Type::ped_impatient:
			break;
		case event::Type::green_expires:
			//still need to handle case when green timer has expired
			//but the button has not yet been pressed
			m_green_timer = -1.0;
			if(m_button_pressed)
			{
				m_color = color::yellow;
				m_start_yellow_timer();
			}
			break;
		case event::Type::yellow_expires:
			m_yellow_timer = -1.0;
			m_walk_signal = true;
			m_color = color::red;
			m_start_red_timer();
			break;
		case event::Type::red_expires:
			m_red_timer = -1.0;
			m_button_pressed = false;
			m_walk_signal = false;
			m_color = color::green;
			m_start_green_timer();
			break;
		case event::Type::auto_exit:
		{
			auto it = m_autos.find(current_event->m_id);
			if(it == m_autos.end())
			{
				throw std::logic_error("auto not found in map, entity_info may have gotten lost");
			}
			m_Da.insert_data_point(m_compute_auto_delay(it->second));
			m_autos.erase(it);
		}
			break;
		case event::Type::ped_exit:
		{
			auto it = m_peds.find(current_event->m_id);
			if(it == m_peds.end())
			{
				throw std::logic_error("ped not found in map, entity_info may have gotten lost");
			}
			m_Dp.insert_data_point(m_compute_ped_delay(it->second));
			m_peds.erase(it);
		}
			break;
		}
		
		m_event_list.erase(current_event);
	}
}

double simulator::clock() const { return m_clock; }
const welford<double>& simulator::Da() const { return m_Da; }
const welford<double>& simulator::Dp() const { return m_Dp; }
