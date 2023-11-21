#include "event.h"

int event::next_id = 0;

event::event(double at, event::Type type, int id, double speed) noexcept
	: m_at(at), m_type(type), m_id(id), m_speed(speed)
{
	if(type == Type::auto_arrival || type == Type::ped_arrival)
	{
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
