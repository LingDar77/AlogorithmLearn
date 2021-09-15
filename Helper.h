#pragma once
#include <chrono>
#include "TypeTraits.h"
#include <iostream>
#include <functional>
using namespace std;
using namespace chrono;
typedef long long LL;


#define min(lhs, rhs) (lhs) < (rhs) ? (lhs) : (rhs)
#define max(lhs, rhs) (lhs) > (rhs) ? (lhs) : (rhs)

template<class Type>
Type Max(Type val)
{
	return val;
}

template<class Type, class... Types>
Type Max(Type lhs, Types... others)
{
	return max(lhs, Max(others...));
}

template<class Type>
Type Min(Type val)
{
	return val;
}

template<class Type, class... Types>
Type Min(Type lhs, Types... others)
{
	return min(lhs, Min(others...));
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

