#pragma once
#ifndef AlogoEasy_h__
#define AlogoEasy_h__


#include <vector>
#include <string>
#include <iostream>
#include "Difinitions.h"

namespace Alogo {
	using namespace std;

	vector<int> GetPrefixMap(string Pattern)
	{
		if (Pattern.size() == 0) return {};
		vector<int> map = {-1, 0};
		for (int i = 1; i < Pattern.size() - 1; ++i)
		{
			int cnt = 0;
			if (Pattern[i] == Pattern[map[i]])
			{
				map.push_back(map[i] + 1);
				continue;
			}
			//this loop has no need to run so many times which causes O(n) = n square 
			for (int prefixLen = 1; prefixLen <= i; ++prefixLen)
			{
				string prefix = string(Pattern.begin(), Pattern.begin() + prefixLen);
				string suffix = string(Pattern.begin() + i + 1 - prefixLen, Pattern.begin() + i + 1);
				if (prefix == suffix && prefixLen > cnt) cnt = prefixLen;
				//cout << prefix << " " << suffix << endl;
			}
			//cout << endl;
			map.push_back(cnt);
		}
		return map;
	}
	vector<int> KmpMatch(string Source, string Target)
	{
		if (Source.size() == 0 && Target.size() == 0) return { 0 };
		else if (Target.size() == 0 && Source.size() != 0) return {(int) Source.size() + 1};
		else if (Source.size() == 0 || Target.size() > Source.size()) return {};
		auto map = GetPrefixMap(Target);
		vector<int> ret = { };
		int index = 0;
		while (index <= Source.size() - Target.size())
		{
			int i = 0;
			while (index <= Source.size() - Target.size() && i < Target.size())
			{
				//cout << i << " " << index << endl;
				if (Source[size_t(index) + i] != Target[i])
				{
					if (map[i] == -1)
					{
						++index;
						continue;
					}
					index = index + i - map[i];
					i = map[i];
				}
				else
					++i;
			}
			if(index <= Source.size() - Target.size())
			{
				ret.push_back(index);
				index += Target.size();
			}
		}
		return ret;
	}
	int			CountPrimes_EulerSieve(const int n)
	{
		bool* Composites = new bool[n + 1];
		for (int i = 0; i < n + 1; ++i) Composites[i] = 0;
		vector<int> Primes;
		for (int i = 2; i < n; ++i)
		{
			if (!Composites[i])
			{
				Primes.push_back(i);
			}
			for (auto prime : Primes)
			{
				if (prime * i > n)
					break;
				Composites[prime * i] = 1;
				if (i % prime == 0)
					break;
			}
			
		}
		delete[](Composites);
// 		for(auto i : Primes) cout << i << " ";

		return Primes.size();
	}
	int			CountPrimes_EratosthenesSieve(const int n)
	{
		vector<bool> prime(n + 1, 1);
		int cnt = 0;
		for (int i = 2; i < n + 1; ++i)
		{
			if (prime[i])
			{
				++cnt;
				prime[i] = 0;
				for(int j = i * 2; j <= n; j += i)
				{
					prime[j] = 0;
				}
			}
		}
		return cnt;
	}
	uint		FibNumber(uint n)
	{
		uint *dp = new uint[n + 1]();

		dp[0] = 0;
		dp[1] = 1;

		for (int i = 2; i <= n; ++i)
		{
			dp[i] = dp[i - 1] + dp[i - 2];
		}

		auto ret = dp[n];
		delete[] dp;
		return ret;
	}



}
#endif // AlogoEasy_h__