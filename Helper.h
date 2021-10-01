#pragma once

#ifndef Helper_h__
#define Helper_h__

#include <chrono>
#include "Limits.h"
#include <iostream>
#include <functional>
#include <assert.h>

using namespace std;
using namespace chrono;

template<class Type, class... Types>
constexpr const Type& max(const Type& lhs, const Types&... others)
{
	if constexpr (sizeof...(others) == 0)
	{
		return lhs;
	}
	else
	{
		const Type& m = ::max(others...);
		return lhs > m ? lhs : m;
	}
}

template<class Type, class... Types>
constexpr const Type& min(const Type& lhs, const Types&... others)
{
	if constexpr (sizeof...(others) == 0)
	{
		return lhs;
	}
	else
	{
		const Type& m = ::min(others...);
		return lhs < m ? lhs : m;
	}
}

template<class Type, class... Types>
constexpr Type max(Type&& lhs, Types&&... others)
{
	if constexpr (sizeof...(others) == 0)
	{
		return lhs;
	}
	else
	{
		auto m = ::max(others...);
		return lhs > m ? lhs : m;
	}
}

template<class Type, class... Types>
constexpr Type min(Type&& lhs, Types&&... others)
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
