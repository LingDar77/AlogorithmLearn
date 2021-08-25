#pragma once
#include <chrono>
using namespace std;
using namespace chrono;
template<class Duration>
class Timer
{
public:
	long long time;
	Timer():time(-1) {};
	Timer(void (*f)(void))
	{
		auto start = high_resolution_clock::now();
		f();
		auto end = high_resolution_clock::now();
		time = duration_cast<Duration>(end - start).count();
	}
};