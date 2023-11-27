#include "sim.h"
#include "event.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

entity_info::entity_info(double at, double speed) : at(at), speed(speed) {}

#ifdef DEBUG
#include <iostream>
#include <random>

int s_gen_seed()
{
	std::random_device rdev;
	return rdev();
}

simulator::simulator() : m_event_list(), m_auto_random(s_gen_seed()),
	m_ped_random(s_gen_seed()), m_button_random(s_gen_seed()), m_clock(0.0),
	m_Da(), m_Dp(), m_color(color::green), m_walk_signal(false),
	m_button_pressed(false), m_green_timer(-1.0), m_yellow_timer(-1.0),
	m_red_timer(-1.0), m_ped_cross_count(0), m_peds(), m_autos(), m_ped_queue(),
	m_delayed_autos() {}
#endif

simulator::simulator(const std::string& auto_random, const std::string& ped_random, const std::string& button_random)
	: m_event_list(), m_auto_random(auto_random), m_ped_random(ped_random),
	m_button_random(button_random), m_clock(0.0), m_Da(), m_Dp(),
	m_color(color::green), m_walk_signal(false), m_button_pressed(false),
	m_green_timer(-1.0), m_yellow_timer(-1.0), m_red_timer(-1.0),
	m_ped_cross_count(0), m_peds(), m_autos(), m_ped_queue(), m_delayed_autos() {}
	
double simulator::m_compute_ped_delay(entity_info i)
{
	return (m_clock - i.at) - (422.0/i.speed);
}

double simulator::m_compute_auto_delay(entity_info i)
{
	return std::abs((m_clock - i.at) - (2586.0/i.speed));
}

void simulator::m_start_green_timer()
{
	if(m_green_timer < 0.0)
	{
		m_button_pressed = false;
		m_walk_signal = false;
		m_color = color::green;
		m_green_timer = m_clock + 35.0;
		m_event_list.emplace(m_green_timer, event::Type::green_expires);
	}
}
void simulator::m_start_yellow_timer()
{
	if(m_yellow_timer < 0.0)
	{
		m_color = color::yellow;
		m_ped_cross_count = 0;
		m_yellow_timer = m_clock + 8.0;
		m_event_list.emplace(m_yellow_timer, event::Type::yellow_expires);
	}
}

void simulator::m_start_red_timer()
{
	if(m_red_timer < 0.0)
	{
		m_walk_signal = true;
		m_color = color::red;
		m_red_timer = m_clock + 18.0;
		m_event_list.emplace(m_red_timer, event::Type::red_expires);
	}
}

double simulator::m_button_push_prob()
{
	if(m_ped_queue.size() == 0)
	{
		return 0.9375;
	}else
	{
		return 1.0 / (double)(m_ped_queue.size()+1);
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
			m_event_list.emplace(m_clock + 376.0/current_event->m_speed,
				event::Type::ped_at_button, current_event->m_speed, current_event->m_id);
			m_peds.try_emplace(current_event->m_id, current_event->m_at, current_event->m_speed);
			break;
		case event::Type::ped_at_button:
			if(m_walk_signal)
			{
				double time_left = m_red_timer - m_clock;
				if(time_left >= 46.0/current_event->m_speed
					&& m_ped_cross_count < 20)
				{//cross
					m_ped_cross_count++;
					m_event_list.emplace(m_clock + 46.0/current_event->m_speed,
						event::Type::ped_exit, current_event->m_speed, current_event->m_id);
				}else //can't cross
				{
					m_ped_queue.insert(*current_event);
				}
			}else //can't cross
			{
				if(m_button_random.bernouli(m_button_push_prob()))
				{
					m_button_pressed = true;
					if(m_color == color::green && m_green_timer < 0.0) m_start_yellow_timer();
				}
				m_ped_queue.insert(*current_event);
				m_event_list.emplace(m_clock + 60.0, event::Type::ped_impatient,
					-1.0, current_event->m_id);
			}
			break;
		case event::Type::ped_impatient:
			if(!m_button_pressed)
			{
				auto it = std::find_if(m_ped_queue.begin(), m_ped_queue.end(),
					[current_event](const auto& e)
					{
						return e.m_id == current_event->m_id;
					});
				if(it != m_ped_queue.end())
				{
					m_button_pressed = true;
					if(m_color == color::green && m_green_timer < 0.0) m_start_yellow_timer();
				}
			}
			break;
		case event::Type::green_expires:
			m_green_timer = -1.0;
			if(m_button_pressed)
			{
				m_start_yellow_timer();
			}
			break;
		case event::Type::yellow_expires:
			m_yellow_timer = -1.0;
			m_start_red_timer();
			{
				double time_left = m_red_timer - m_clock;
				auto it = m_ped_queue.begin();
				while(m_ped_cross_count < 20)
				{
					if(it == m_ped_queue.end()) break;
					if(time_left >= 46.0/it->m_speed)
					{
						m_ped_cross_count++;
						m_event_list.emplace(m_clock + 46.0/it->m_speed,
							event::Type::ped_exit, it->m_speed, it->m_id);
						it = m_ped_queue.erase(it);
					}else
					{
						it++;
					}
				}
				for(const auto&[id, entity] : m_autos)
				{
					if(m_delayed_autos.find(id) == m_delayed_autos.end())
					{
						double dist = entity.speed * (m_clock-entity.at);
						if(dist >= 1314.0)
						{//auto can exit with no delay
							m_event_list.emplace(entity.at + (2586.0/entity.speed),
								event::Type::auto_exit, entity.speed, id);
						}else
						{
							m_delayed_autos.emplace(id, entity);
						}
					}
				}
			}
			break;
		case event::Type::red_expires:
			m_red_timer = -1.0;
			m_start_green_timer();
			if(!m_button_random.bernouli(std::pow(0.0625, m_ped_queue.size())))
			{
				m_button_pressed = true;	
			}
			for(const auto& e : m_ped_queue)
			{
				m_event_list.emplace(m_clock + 60.0, event::Type::ped_impatient,
						-1.0, e.m_id);
			}
			for(const auto&[id, entity] : m_delayed_autos)
			{
				double dist = 1479 - (entity.speed*entity.speed)/20.0;
				double time = 2*(entity.speed/10.0) + dist/entity.speed;
				m_event_list.emplace(m_clock + time, event::Type::auto_exit,
					entity.speed, id);
			}
			break;
		case event::Type::auto_exit:
		{
			auto it = m_autos.find(current_event->m_id);
			if(it != m_autos.end())
			{
				m_Da.insert_data_point(m_compute_auto_delay(it->second));
				m_delayed_autos.erase(it->first);
				m_autos.erase(it);
			}
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
	
	#ifdef DEBUG
		std::cout << "DEBUG: events left in queue:\t" << m_event_list.size() << "\n";
		std::cout << "DEBUG: peds left at crosswalk:\t" << m_ped_queue.size() << "\n";
		std::cout << "DEBUG: peds left in system:\t" << m_peds.size() << "\n";
		std::cout << "DEBUG: autos left in system:\t" << m_autos.size() << "\n";
	#endif
}

double simulator::clock() const { return m_clock; }
const welford<double>& simulator::Da() const { return m_Da; }
const welford<double>& simulator::Dp() const { return m_Dp; }
