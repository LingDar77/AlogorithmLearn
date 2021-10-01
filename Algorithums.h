#pragma once
#ifndef Algorithums_h__
#define Algorithums_h__

#include "Helper.h"
#include "Vector.h"
#include "Queue.h"

namespace Algo
{
	using namespace Struct;
	template<template<class> class CompareObject = Less,class Iterator>
	void InsertionSort(Iterator begin, Iterator end)
	{
		for (auto iter = begin + 1; iter != end; ++iter)
		{
			for (auto i = iter; 
				i != begin && CompareObject<decltype(*i)>()(*i, *(i - 1)) 
				; --i)
			{//This operation costs much when dealing with a forward list (operator-)
				swap(*i, *(i - 1));
			}
		}
	}
	
	//TODO
	template<template<class> class CompareObject = Less, 
		class Iterator,
		class IncrementSequence>
	void ShellSort(Iterator begin, Iterator end)
	{
		auto Increment = IncrementSequence(end - begin);

		for (auto gap = Increment.Begin(); gap != Increment.End(); ++gap)
		{
			for (auto iter = begin; iter != end - gap; ++iter)
			{
				//
			}
		}
	}

	template<template<class> class CompareObject = Less,
		template<class> class Container,
		class Type>
	void Heapify(Container<Type>& sequence)
	{
		auto sz = sequence.Size();
		for (int i = (sz - 1) / 2; i >= 0; --i)
		{
			int par = i;
			while (par * 2 + 1 < sz)
			{
				int child = par * 2 + 1;

				if (child + 1 < sz && CompareObject<Type>()(sequence[child + 1], sequence[child]))
				{
					++child;
				}
				if (CompareObject<Type>()(sequence[child], sequence[par]))
				{
					swap(sequence[child], sequence[par]);
				}
				par = child;
			}
		}
	}

	template<template<class> class CompareObject = Greater, class Iterator>
	void Heapify(Iterator begin, Iterator end)
	{
		int sz = end - begin;
		for (int i = (sz - 1) / 2; i >= 0; --i)
		{
			using Type = remove_reference_t<decltype(*begin)>;
			int par = i;
			while (par * 2 + 1 < sz)
			{
				int child = par * 2 + 1;

				if (child + 1 < sz && CompareObject<Type>()(*(begin + child + 1), *(begin + child)))
				{
					++child;
				}
				if (CompareObject<Type>()(*(begin + child), *(begin + par)))
				{
					swap(*(begin + child), *(begin + par));
				}
				par = child;
			}
		}

	}

	template<template<class> class CompareObject = Greater, class Iterator>
	void PercolatingDown(Iterator root, Iterator end)
	{
		using Type = remove_reference_t<decltype(*root)>;
		int sz = end - root;
		int par = 0;
		int child = par * 2 + 1;
		while (child < sz)
		{
			if (child + 1 < sz && CompareObject<Type>()(*(root + child + 1), *(root + child)))
				++child;
			if (CompareObject<Type>()(*(root + child), *(root + par)))
				swap(*(root + child), *(root + par));
			par = child;
			child = par * 2 + 1;
		}
	}

	template<template<class> class CompareObject = Greater, class Iterator>
	void HeapSort(Iterator begin, Iterator end)
	{
		using Type = remove_reference_t<decltype(*begin)>;
		Heapify<CompareObject, decltype(begin)>(begin, end);
		while (begin != end)
		{
			PercolatingDown<CompareObject, decltype(begin)>(begin, end);
			swap(*begin, *(--end));
		}

	}

	template<template<class> class CompareObject = Less, class Iterator>
	void Merge(Iterator begin, Iterator mid, Iterator back,
		Struct::Vector<remove_reference_t<decltype(*begin)>>& temp)
	{
		using Type = remove_reference_t<decltype(*begin)>;
		size_t cnt = 0;
		auto left = begin;
		auto right = mid + 1;
		
		while (left != mid + 1 || right != back + 1)
		{
			if (left != mid + 1 && right != back + 1)
			{
				if (CompareObject<Type>()(*left, *right))
				{
					temp[cnt] = move(*left);
					++left;
				}
				else
				{
					temp[cnt] = move(*right);
					++right;
				}
			}
			elif(left == mid + 1 && right != back + 1)
			{
				temp[cnt] = move(*right);
				++right;
			}
			else
			{
				temp[cnt] = move(*left);
				++left;
			}
			++cnt;
		}

		for (size_t i = 0; i < cnt; ++i)
		{
			*begin = move(temp[i]);
			++begin;
		}
	}

	template<template<class> class CompareObject = Less, class Iterator>
	void MergeSort(Iterator begin, Iterator back,
		Struct::Vector<remove_reference_t<decltype(*begin)>>& temp)
	{
		if (begin == back) return;
		auto mid = begin + ((back - begin) >> 1);

		MergeSort<CompareObject, Iterator>(begin, mid, temp);
		MergeSort<CompareObject, Iterator>(mid + 1, back, temp);

		Merge<CompareObject, Iterator>(begin, mid, back, temp);
	}
	
	template<template<class> class CompareObject = Less, class Iterator>
	void MergeSort(Iterator begin, Iterator end)
	{
		using Type = remove_reference_t<decltype(*begin)>;
		auto sz = end - begin;
		Struct::Vector<Type> temp(sz, Type());
		//Pretty awful when operator- works not well
		MergeSort<CompareObject, Iterator>(begin, --end, temp);
	}

	template<template<class> class CompareObject = Less,
		template<class> class Container,
		class Type>
	void MergeSort(Container<Type>& sequence, 
		Struct::Vector<Type>& temp, 
		size_t begin, size_t back)
	{
		if (begin >= back) return;
		auto mid = (begin + back) >> 1;
		MergeSort<CompareObject, Container, Type>(sequence, temp, begin, mid);
		MergeSort<CompareObject, Container, Type>(sequence, temp, mid + 1, back);
		Merge<CompareObject, decltype(sequence.Begin())>(
			sequence.Begin() + begin, sequence.Begin() + mid, sequence.Begin() + back, temp);
	}

	template<template<class> class CompareObject = Less,
		template<class> class Container,
		class Type>
	void MergeSort(Container<Type>& sequence)
	{
		auto sz = sequence.Size();
		Struct::Vector<Type> temp(sz, Type());
		MergeSort< CompareObject, Container, Type>(sequence, temp, 0, sz - 1);
	} 

	template<template<class> class CompareObject = Less,
		template<class> class Container, class Type>
	void QuickSort(Container<Type>& sequence, size_t begin, size_t back)
	{
		if (begin >= back) return;
		auto mid = ((back + begin) >> 1);
		
		if (CompareObject<Type>()(sequence[mid], sequence[begin]))
			swap(sequence[mid], sequence[begin]);
		if (CompareObject<Type>()(sequence[back], sequence[begin]))
			swap(sequence[back], sequence[begin]);
		if (CompareObject<Type>()(sequence[back], sequence[mid]))
			swap(sequence[back], sequence[mid]);
		swap(sequence[mid], sequence[back - 1]);

		if (mid == begin) return;

		const Type& pivot = sequence[back - 1];
		auto left = begin + 1;
		auto right = back - 2;

		while (1)
		{
			while (CompareObject<Type>()(sequence[left], pivot)) ++left;
			while (CompareObject<Type>()(pivot, sequence[right])) --right;
			if (left < right)
				swap(sequence[left], sequence[right]);
			else
				break;
		}

		swap(sequence[back - 1], sequence[left]);
		QuickSort<CompareObject>(sequence, begin, left - 1);
		QuickSort<CompareObject>(sequence, left + 1, back);
	}

	template<template<class> class CompareObject = Less, class Iterator>
	void QuickSort(Iterator begin, Iterator back, bool)
	{
		if (begin == end) return;
		auto sz = back - begin;

	}

	template<template<class> class CompareObject = Less, class Iterator>
	void QuickSort(Iterator begin, Iterator end) = delete;
// 	{
// 		//Pretty awful when operator- works not well
// 		QuickSort<CompareObject>(begin, end - 1, true);
// 	}

	template<template<class> class CompareObject = Less,
		template<class> class Container, class Type>
	void QuickSort(Container<Type>& sequence)
	{
		QuickSort<CompareObject>(sequence, 0, sequence.Size() - 1);
	}
	
};
#endif // Algorithums_h__