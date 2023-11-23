#pragma once

class simulator; //predeclare

class event
{
public:
	friend class simulator;
	enum class Type {
		auto_arrival,
		ped_arrival,
		ped_at_button,
		ped_impatient,
		green_expires,
		yellow_expires,
		red_expires,
		auto_exit,
		ped_exit,
	};
	
	event(double at, Type type, double speed = -1.0, int id = -1) noexcept;
	
	double at() const;
	Type type() const;
	double speed() const;
	
	friend bool operator<(const event& a, const event& b);
private:
	static int next_id;
	double m_at;
	Type m_type;
	int m_id;
	double m_speed;
};
