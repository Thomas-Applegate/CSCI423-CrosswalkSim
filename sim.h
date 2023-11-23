#pragma once
#include <set>
#include "random.h"
#include "event.h"
#include "welford.h"
#include <string>
#include <unordered_map>

enum class color {red, yellow, green };

struct entity_info
{
	entity_info(double at, double speed);
	double at;
	double speed;
};

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
	
	color m_color;
	bool m_walk_signal;
	bool m_button_pressed;
	double m_green_timer;
	double m_yellow_timer;
	double m_red_timer;
	int m_ped_cross_count;
	
	std::unordered_map<int, entity_info> m_peds;
	std::unordered_map<int, entity_info> m_autos;
	std::set<event> m_ped_queue;
	
	double m_compute_ped_delay(entity_info i);
	double m_compute_auto_delay(entity_info i);
	
	void m_start_green_timer();
	void m_start_yellow_timer();
	void m_start_red_timer();
	
	double m_button_push_prob();
};
