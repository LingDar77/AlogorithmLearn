#pragma once
#include <chrono>
#include "TypeTraits.h"
#include <iostream>
#include <functional>
using namespace std;
using namespace chrono;

#define max(lhs, rhs) (lhs) > (rhs) ? (lhs) : (rhs)
#define min(lhs, rhs) (lhs) < (rhs) ? (lhs) : (rhs)

typedef long long LL;

template<class Duration>
class Timer
{
public:
	long long time;
	Timer():time(-1) {};
	
	Timer(function<void(void)> f)
	{
		auto start = high_resolution_clock::now();
		f();
		auto end = high_resolution_clock::now();
		time = duration_cast<Duration>(end - start).count();
	}

	Timer(function<void(long long)> f, long long para1)
	{
		auto start = high_resolution_clock::now();
		f(para1);
		auto end = high_resolution_clock::now();
		time = duration_cast<Duration>(end - start).count();
	}

	Timer(function<int(long long)> f, long long para1)
	{
		auto start = high_resolution_clock::now();
		cout << f(para1) << " ";
		auto end = high_resolution_clock::now();
		time = duration_cast<Duration>(end - start).count();
	}
};