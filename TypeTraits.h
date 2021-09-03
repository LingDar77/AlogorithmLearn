#pragma once

namespace Traits
{
	template<class Type>
	struct IsInteger
	{
		bool Result = false;
	};
	template<>
	struct IsInteger<int>
	{
		bool Result = true;
	};
	template<>
	struct IsInteger<unsigned int>
	{
		bool Result = true;
	};
	template<>
	struct IsInteger<short>
	{
		bool Result = true;
	};
	template<>
	struct IsInteger<unsigned short>
	{
		bool Result = true;
	};
	template<>
	struct IsInteger<long>
	{
		bool Result = true;
	};
	template<>
	struct IsInteger<unsigned long>
	{
		bool Result = true;
	};
	template<>
	struct IsInteger<long long>
	{
		bool Result = true;
	};
	template<>
	struct IsInteger<unsigned long long>
	{
		bool Result = true;
	};
	template<>
	struct IsInteger<char>
	{
		bool Result = true;
	};
	template<>
	struct IsInteger<unsigned char>
	{
		bool Result = true;
	};

}
