#include "event.h"

event::event(double at) noexcept : m_at(at) {}
double event::at() const { return m_at; }

bool operator<(const event& a, const event& b)
{
	return a.m_at < b.m_at;
}
