#include "event.h"
#include <stdexcept>

int event::next_id = 0;

event::event(double at, event::Type type, double speed, int id) noexcept
	: m_at(at), m_type(type), m_id(id), m_speed(speed)
{
	if(at < 0.0)
	{
		throw std::domain_error("event at must be positive");
	}
	
	if(type == Type::auto_arrival || type == Type::ped_arrival)
	{
		if(speed < 0.0)
		{
			throw std::domain_error("event speed must be positive for arrival events");
		}
		m_id = event::next_id;
		event::next_id++;
	}
}

double event::at() const { return m_at; }
event::Type event::type() const { return m_type; }
double event::speed() const { return m_speed; }

bool operator<(const event& a, const event& b)
{
	if(a.m_at == b.m_at)
	{
		return a.m_type < b.m_type;
	}else
	{
		return a.m_at < b.m_at;
	}
}

std::ostream& operator<<(std::ostream& os, const event& e)
{
	os << "event (type=";
	
	switch(e.m_type)
	{
		case event::Type::auto_arrival:
			os << "auto_arrival";
			break;
		case event::Type::ped_arrival:
			os << "ped_arrival";
			break;
		case event::Type::ped_at_button:
			os << "ped_at_button";
			break;
		case event::Type::ped_impatient:
			os << "ped_impatient";
			break;
		case event::Type::green_expires:
			os << "green_expires";
			break;
		case event::Type::yellow_expires:
			os << "yellow_expires";
			break;
		case event::Type::red_expires:
			os << "red_expires";
			break;
		case event::Type::auto_exit:
			os << "auto_exit";
			break;
		case event::Type::ped_exit:
			os << "ped_exit";
			break;
	}
	
	os << ", at=" << e.m_at << ", speed=" << e.m_speed << ", id=" << e.m_id;
	os << ")";
	return os;
}
