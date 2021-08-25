#pragma once

#include <string>
#include <array>
#include <iostream>
#include "Structures.h"
#include <map>
#include <algorithm>


namespace Problems
{
	using namespace std;

	//n(sequence) is lower than 40 billion
	int FindAInt1(vector<int> sequence)
	{
		Struct::BitMap bmap(4000000000);
		for(auto i : sequence)
		{
			bmap.Set(i, 1);
		}
		for(unsigned i = 0; i < 4000000000; ++i)
		{
			if (!bmap[i]) return i;
		}
		return -1;
	}

	int FindAInt2(vector<int> sequence)
	{
		return -1;
	}

	//O(n) solution
	string LeftRotate(string source, int step)
	{
		step %= source.size();
		for(int i = 0; i < step / 2; ++i)
		{
			swap(source[i],source[step - i - 1]);
		}
		for (int i = step; i < (source.size() - 1 + step) / 2; ++i)
		{
			swap(source[i], source[source.size() - 1 + step - i]);
		}
		for (int i = 0; i < source.size() / 2; ++i)
		{
			swap(source[i], source[source.size() - 1 - i]);
		}
		return source;
	}

	vector<vector<string>> FindAnagrams(vector<string> dictionary)
	{
		map<string, vector<int>> tb;
		for (int i = 0; i < dictionary.size(); ++i)
		{
			string s = dictionary[i];
			sort(s.begin(), s.end());
			tb[s].push_back(i);
		}
		vector<vector<string>> ret;
		for(auto && i : tb)
		{
			vector<string> bf;
			for (auto j : i.second)
			{
				bf.push_back(dictionary[j]);
			}
			ret.push_back(move(bf));
		}
		return ret;
	}


}