#pragma once
#ifndef PriorityQueue_h__
#define PriorityQueue_h__

#include "BinaryHeap.h"

namespace Struct
{
	template<class Type,
		template<class> class CompareObject = Greater,
		template<class, template<class> class> class Basement = BinaryHeap>
	class PriorityQueue
	{
	private:

		Basement<Type, CompareObject> data;

	public:

		void Size() const
		{
			return data.Size();
		}

		void Clear()
		{
			data.Clear();
		}

		Type Top()
		{
			return data.Top();
		}

		Type RemoveRoot()
		{
			return data.RemoveRoot();
		}

		void Insert(const Type& value)
		{
			data.Insert(value);
		}

		void Insert(Type&& value)
		{
			data.Insert(move(value));
		}

	};
}

#endif // PriorityQueue_h__