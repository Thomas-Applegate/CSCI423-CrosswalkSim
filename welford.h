#pragma once
#include <cmath>
#include <initializer_list>

template<typename T>
class welford
{
public:
	welford() : i(0), xi(0), vi(0) {}
	welford(const std::initializer_list<T>& list) : welford()
	{
		for(const T& x : list) insert_data_point(x);
	}
	
	void insert_data_point(const T& x)
	{
		i++;
		T xlast = xi;
		T vlast = vi;
		xi = xlast + (1/(T)i)*(x-xlast);
		vi = vlast + ((i-1)/(T)i)*(x-xlast)*(x-xlast);
	}
	
	T mean() const { return xi; }
	
	T variance() const { return vi / i; }
	
	T stddev() const { return std::sqrt(variance()); }
	
	size_t count() const { return i; }
private:
	size_t i;
	T xi;
	T vi;
};
