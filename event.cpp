#include "event.h"

event::event(double at, event::Type type, double speed) noexcept
	: m_at(at), m_type(type), m_speed(speed) {}

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
