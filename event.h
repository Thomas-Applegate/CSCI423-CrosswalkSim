#pragma once

class event
{
public:
	event(double at) noexcept;
	
	double at() const;
	
	friend bool operator<(const event& a, const event& b);
private:
	double m_at;
};
