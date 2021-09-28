#pragma once

#ifndef Helper_h__
#define Helper_h__

#include <chrono>
#include "Limits.h"
#include <iostream>
#include <functional>
using namespace std;
using namespace chrono;

template<class Type, class... Types>
constexpr auto max(Type lhs, Types... others)
{
	if constexpr (sizeof...(others) == 0)
	{
		return lhs;
	}
	else
	{
		auto m = max(others...);
		return lhs > m ? lhs : m;
	}
}

template<class Type, class... Types>
constexpr auto min(Type lhs, Types... others)
{
	if constexpr (sizeof...(others) == 0)
	{
		return lhs;
	}
	else
	{
		auto m = min(others...);
		return lhs < m ? lhs : m;
	}
}


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


template<class Type>
size_t GetLenOfElement(Type ele)
{
	return 0;
}

size_t GetLenOfElement(int ele)
{
	int cnt = 0;
	if (ele > 0)
	{
		while (ele)
		{
			++cnt;
			ele /= 10;
		}
		return cnt;
	}
	else if (ele < 0)
	{
		ele *= -1;
		while (ele)
		{
			++cnt;
			ele /= 10;
		}
		return cnt + 1;
	}
	else
		return 1;
}


template<class Type>
struct Less
{
	bool operator()(const Type& lhs, const Type& rhs)
	{
		return lhs < rhs;
	}
};

template<class Type>
struct Greater
{
	bool operator()(const Type& lhs, const Type& rhs)
	{
		return lhs > rhs;
	}
};

#endif // Helper_h__
