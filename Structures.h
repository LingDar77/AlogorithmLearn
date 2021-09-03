#pragma once

namespace Struct
{
	using namespace std;


	class BitMap
	{
		char* bits;
	public:
		BitMap() = delete;

		BitMap(long long length)
		{
			bits = new char[(length >> 3) + 1];
			memset(bits, 0, (length >> 3) + 1);
		}

		~BitMap()
		{
			delete[] bits;
		}

		bool Get(long long index)
		{
			int i   = index >> 3;
			int pos = index & 7;
			return (bits[i] & (1 << pos)) == (1 << pos);
		}

		bool operator[](long long index)
		{
			return Get(index);
		}

		void Set(long long index, bool value)
		{
			int i   = index >> 3;
			int pos = index & 7;
			char val = (1 << pos);
			bits[i] = value ? bits[i] | val : bits[i] & ~val;
		}
	};

	


}