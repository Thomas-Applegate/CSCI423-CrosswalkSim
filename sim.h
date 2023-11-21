#pragma once
#include <set>
#include "random.h"
#include "event.h"
#include "welford.h"
#include <string>

enum class color {red, yellow, green };

class simulator
{
public:
#ifdef DEBUG
	simulator();
#else
	simulator() = delete;
#endif
	simulator(const std::string& auto_random, const std::string& ped_random,
		const std::string& button_random);
		
	void run(unsigned int N);
	
	double clock() const;
	const welford<double>& Da() const;
	const welford<double>& Dp() const;
private:
	std::set<event> m_event_list;
	Random m_auto_random;
	Random m_ped_random;
	Random m_button_random;
	
	double m_clock;
	welford<double> m_Da;
	welford<double> m_Dp;
	
	color m_color = color::green;
	bool m_walk_signal = false;
	bool m_button_pressed = false;
};
