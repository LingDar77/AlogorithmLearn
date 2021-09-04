#pragma once

#include <string>
#include <array>
#include <iostream>
#include "Structures.h"
#include <map>
#include <algorithm>
#include <initializer_list>
#include "assert.h"


namespace Problems
{
	using namespace std;
	namespace Structures
	{
		template<class Type>
		class Collection
		{
		private:
			unsigned size;
			Type*    content;

		public:
			Collection()
				:size(0), content(nullptr)
			{

			}

			Collection(int size)
				:size(size), content(new Type[size])
			{
				
			}

			Collection(initializer_list<Type>&& list)
				:size(list.size()), content(new Type[size])
			{
				for (size_t i = 0; i < size; ++i)
				{
					content[i] = *(list.begin() + i);
				}
			}

			Type& operator[](int index)
			{
				assert(index < Size() && index >= 0);
				return *(content + index);
			}

			const Type& operator[](int index) const
			{
				assert(index < Size() && index >= 0);
				return *(content + index);
			}

			~Collection()
			{
				delete[] content;
			}

			size_t Size() const
			{
				return size;
			}

			bool IsEmpty() const
			{
				return Size();
			}

			void Empty()
			{
				size = 0;
				delete[] content;
				content = nullptr;
			}

			void Insert(int index, Type&& item)
			{
				assert(index < Size() && index >= 0);
				Type* bf = new Type[Size() + 1];
				for (int i = 0; i < index; ++i)
				{
					bf[i] = content[i];
				}
				bf[index] = item;
				for (int i = index + 1; i < Size() + 1; ++i)
				{
					bf[i] = content[i - 1];
				}

				++size;
				delete[] content;
				content = bf;
			}

			Type Remove(int index)
			{
				assert(index < Size() && index >= 0);
				auto bf  = content[index];
				auto ctt = new Type[Size() - 1];
				for (int i = 0; i < index; ++i)
				{
					ctt[i] = content[i];
				}
				for (int i = index + 1; i < Size(); ++i)
				{
					ctt[i - 1] = content[i];
				}
				delete[] content;
				content = ctt;
				--size;
				return bf;
			}

			int Contains(Type&& item)
			{
				for (int i = 0; i < Size(); ++i)
				{
					if (content[i] == item) return i;
				}
				return -1;
			}

		};

		template<class Type>
		class OrderedCollection
		{

		};
	};
	using namespace Structures;

	namespace tests
	{
		void test001()
		{
			Collection<int> a({1, 2, 3});
			a.Insert(0, 9);
			a.Remove(1);
			cout << a.Contains(10) << endl;
			for (int i = 0; i < a.Size(); ++i)
			{
				cout << a[i] << endl;
			}
		}
	}

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

	vector<string> FindAnagramsPlus1(string pattern, vector<string> dictionary)
	{
		vector<string> ret;
		sort(pattern.begin(), pattern.end());
		for (auto i : dictionary)
		{
			if(i.size() != pattern.size()) continue;
			string s = i;
			sort(i.begin(), i.end());
			if (i == pattern)
				ret.push_back(move(s));
		}
		return ret;
	}

	vector<string> FindAnagramsPlus2(string pattern, vector<string> dictionary)
	{
		vector<string> ret;
		
		return ret;
	}

	bool ExistMaxSubset(vector<int> set, int maxSum, int size)
	{
		sort(set.begin(), set.end());
		for (int i = 0; i < size; ++i)
		{
			maxSum -= set[i];
		}
		return maxSum >= 0;
	}

	void EvenNums(int i)
	{
		while (i != 1)
		{
			if (i % 2 == 0)
			{
				i /= 2;
			}
			else
			{
				i = i * 3 + 1;
			}
			cout << i << endl;
		}
	}

	int NumberOfOne(unsigned num)
	{
		if (num == 1)
		{
			return 1;
		}
		else if (num == 0)
		{
			return 0;
		}
		else if((num & 1) == 1)
		{
			return NumberOfOne(num / 2) + 1;
		}
		else
		{
			int ret  = 0;
			int mask = 2;
			while (num)
			{
				if (num & mask)
				{
					++ret;
					num ^= mask;
				}
				else
					num &= ~mask;
				mask <<= 1;
			}
			return ret;
		}

	}

	//O(n2) solution
	unsigned GetMaxSubsequence1(const vector<int>& sequence) 
	{
		if (sequence.size() == 0) return 0;
		int ret = 0;
		vector<vector<int>> bp(sequence.size(), vector<int>(sequence.size()));
		//bp[i][j] expresses the sum from index i to index j, it's not necessary to use a array.
		for (int i = 0; i < sequence.size(); ++i)
		{
			bp[i][i] = sequence[i];
		}
		for (int i = 0; i < sequence.size(); ++i)
		{
			for (int j = i + 1; j < sequence.size(); ++j)
			{
				bp[i][j] = bp[i][j - 1] + sequence[j];
				ret = max(ret, bp[i][j]);
			}
		}
		return ret;
	}

	//O(nlogn) solution
	unsigned GetMaxSubsequence2(const vector<int>& sequence)
	{
		class GetMaxSubsequence
		{
		public:
			unsigned operator()(const vector<int>& sequence, size_t begin, size_t end)
			{
				if (begin > end || (begin == end && sequence[begin] <= 0)) return 0;
				if (begin == end && sequence[begin] > 0) return sequence[begin];
				size_t middle = (begin + end) >> 1;
				size_t left  = GetMaxSubsequence()(sequence, begin, middle);
				size_t right = GetMaxSubsequence()(sequence, middle + 1, end);
				int leftSum = 0, maxLeftSum = 0;
				for (int i = (int)middle; i >= (int)begin; --i)
				{
					leftSum += sequence[i];
					maxLeftSum = max(maxLeftSum, leftSum);
				}
				int rightSum = 0, maxRightSum = 0;
				for (int i = int(middle + 1); i <= (int)end; ++i)
				{
					rightSum += sequence[i];
					maxRightSum = max(maxRightSum, rightSum);
				}
				//cout << begin << " " << end << " " << left << " " << maxLeftSum << " " << maxRightSum << " " << right << endl;
				return max(max(left, right), maxLeftSum + maxRightSum);
			}
		};
		return GetMaxSubsequence()(sequence, 0, sequence.size() - 1);
	}

	//O(n) solution
	unsigned GetMaxSubsequence3(const vector<int>& sequence)
	{
		int maxSum = 0, thiSum = 0;
		for (int i = 0; i < sequence.size(); ++i)
		{
			thiSum += sequence[i];
			maxSum = max(thiSum, maxSum);
			thiSum = max(0, thiSum);
		}
		return maxSum;
	}

	LL gcd(LL m, LL n)
	{
		while (n)
		{
			LL rem = m % n;

			m = n;
			n = rem;
		}
		return m;
	}

	int GetMinSumOfSubsequence(const vector<int>& sequence)
	{
		int minSum = INT_MAX, thiSum = 0;
		for (int i = 0; i < sequence.size(); ++i)
		{
			thiSum += sequence[i];
			minSum = min(thiSum, minSum);
			thiSum = min(thiSum, 0);
		}
		return minSum;
	}

	int GetMaxProductOfSubsequence(const vector<int>& sequence)
	{
		int maxProduct = sequence[0], thisProduct = sequence[0];
		for (int  i = 1; i < sequence.size(); ++i)
		{
			thisProduct *= sequence[i];
			maxProduct = max(maxProduct, thisProduct);
			thisProduct = max(thisProduct, sequence[i]);
		}
		return maxProduct;
	}

	//required no recursion version
	LL qpow_norec(LL num, LL index)
	{
		LL ret = 0LL;
		

		
		
		return 0;
	}

	LL qpow(LL num, LL index)
	{
		if (index == 1) return num;//base case
		else if (index % 2 == 1)   //even case
		{
			return qpow(num * num, index / 2) * num;
		}
		else					   //odd case
		{
			return qpow(num * num, index / 2);
		}
	}

	//O(n) O(n)
	int FindMajorityElement1(const vector<int>& sequence)
	{
		map<int, int> table;
		for (const auto& i : sequence)
		{
			++table[i];
			if (table[i] > sequence.size() / 2) return i;
		}
		assert(0);
		return -1;
	}

	//Recursion version
	int FindMajorityElement2(const vector<int>& sequence)
	{	
		vector<int> bf;
		return 0;
	}

	bool FindXInSortedMatrix(const vector<vector<int>>& matrix, int X)
	{
		int N = matrix.size();
		//base case, if X is lower than the min or the X is greater than max,
		//then there is no way to exist X
		if (X > matrix[N - 1][N - 1] || X < matrix[0][0]) return false;
		
		int x = 0, y = N - 1;
		//Scan the matrix from [y][y] to [0][0] in search of the first place of its diagonal which is lower than X,
		//then X might exist in the 2x2 matrix
		while (1)
		{
			if (matrix[y][y] > X)
			{
				--y;
			}
			else if (matrix[y][y] < X)
			{
				x = y;
				++y;
				break;
			}
			else
			{
				cout << "(" << y << "," << y << ")" << endl;
				return 1;
			}
		}

		if (matrix[x][x + 1] == X)
		{
			cout << "(" << x << "," << x + 1 << ")" << endl;
			return 1;
		}
		if (matrix[y][y - 1] == X)
		{
			cout << "(" << y << "," << y - 1 << ")" << endl;
			return 1;
		}
		return 0;

	}

	//Find the max sum/dif/pro/quo of a[j] and a[i] j >= i
	
	int PositiveASum(const vector<int>& a)
	{
		//Can I find tow max value in one loop ? 
		int first  = 0;
		int second = 0;
		for (auto i : a)
		{
			if (i > first)
			{
				second = first;
				first = i;
			}
			else if (i < first && i > second)
			{
				second = i;
			}

		}
		return first + second;
	}

	int PositiveADif(const vector<int>& a)
	{
		//Can I find the max value and min value with right indexes in one loop ?
		int maxValue = INT_MIN;
		int minValue = INT_MAX;
		for (int i = a.size() - 1, cnt = 0, maxIndex = a.size() - 1; i >= 0 ; --i)
		{
			if(maxIndex == cnt)
				break;
			if (a[i] > maxValue)
			{
				maxValue = a[i];
				maxIndex = i;
			}
			else
			{
				if (a[cnt] < minValue)
				{
					minValue = a[cnt];
				}
				++cnt;
			}
		}
		return maxValue - minValue;
	}

	int PositiveAPro(const vector<int>& a)
	{
		int first = 0;
		int second = 0;
		for (auto i : a)
		{
			if (i > first)
			{
				second = first;
				first = i;
			}
			else if (i < first && i > second)
			{
				second = i;
			}

		}
		return first * second;
	}

	int PositiveAQuo(const vector<int>& a)
	{
		int maxValue = INT_MIN;
		int minValue = INT_MAX;
		for (int i = a.size() - 1, cnt = 0, maxIndex = a.size() - 1; i >= 0; --i)
		{
			if (maxIndex == cnt)
				break;
			if (a[i] > maxValue)
			{
				maxValue = a[i];
				maxIndex = i;
			}
			else
			{
				if (a[cnt] < minValue)
				{
					minValue = a[cnt];
				}
				++cnt;
			}
		}
		return maxValue / minValue;
	}


}
