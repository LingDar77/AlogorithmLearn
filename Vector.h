#pragma once
#ifndef Vector_h__
#define Vector_h__
#include <initializer_list>
#include "Difinitions.h"
#include "Operations.hpp"
namespace Struct
{
	using namespace std;
	template<class Type>
	class Vector
	{

	private:

		Type* content;
		size_t size;
		size_t capacity;

	public:

		typedef Type* Iterator;
		typedef const Type* ConstIterator;

		explicit Vector(size_t size = 0, Type initialValue = Type())
			:size(size), capacity(MINCAPACITY), content(nullptr)
		{
			Reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				*(content + i) = initialValue;
			}
		}

		~Vector()
		{
			delete[] content;
		}

		Vector(const Vector& rhs)
		{
			Reserve(rhs.Size());
			size = rhs.Size();
			for (int i = 0; i < size; ++i)
			{
				content[i] = rhs[i];
			}
		}

		Vector(Vector&& rhs) noexcept
		{
			swap(content, rhs.content);
			swap(size, rhs.size);
			swap(capacity, rhs.capacity);
		}

		Vector(initializer_list<Type>&& data)
			:size(size), capacity(MINCAPACITY), content(nullptr)
		{
			Reserve(data.size());
			size = data.size();
			for (int i = 0; i < size; ++i)
			{
				*(content + i) = *(data.begin() + i);
			}

		}

		size_t Size() const
		{
			return size;
		}

		size_t Capacity() const
		{
			return capacity;
		}

		Iterator Begin()
		{
			return content;
		}

		ConstIterator Begin() const
		{
			return content;
		}

		Iterator End()
		{
			return content + Size();
		}

		ConstIterator End() const
		{
			return content + Size();
		}

		const Type& operator[](size_t index) const
		{
			if (index > Capacity())
				Resize(index + 1);
			return *(content + index);
		}

		Type& operator[](size_t index)
		{
			if (index > Capacity())
				Resize(index + 1);
			return *(content + index);
		}

		void Remove(Iterator It)
		{
			auto end = End() - 1;
			while (It != end)
			{
				swap(*It, *(It + 1));
				++It;
			}

			Resize(size - 1);
		}

		void Remove(size_t index)
		{
			for (int i = index; i < Size() - 1; ++i)
			{
				swap(content[i], content[i + 1]);
			}

			Resize(size - 1);
		}

		void Resize(size_t newSize)
		{
			if (newSize > Capacity() || newSize < Capacity() / 3)
				Reserve(newSize);
			size = newSize;
		}

		void Reserve(size_t newCapacity)
		{
			size_t newCap = Capacity();
			if (newCapacity > newCap)
				newCap = (newCapacity / newCap + 1) * newCap;
			elif(newCapacity < MINCAPACITY)
				newCap = MINCAPACITY;
			elif(newCapacity < newCap / 4)
				newCap = newCap / 2;
			else
				return;

			//copy the original data and adjust capacity.
			if (content)
			{
				Type* newContent = new Type[newCap + 1]();
				for (size_t i = 0; i < ::min(size, newCap + 1); ++i)
				{
					swap(*(content + i), *(newContent + i));
				}
				content = newContent;
			}
			else
			{
				content = new Type[newCap + 1]();
			}

			capacity = newCap;
		}

		void PushBack(const Type& value)
		{
			size_t size = Size();
			Resize(size + 1);
			*(content + size) = value;
		}

		void PushBack(Type&& value)
		{
			size_t size = Size();
			Resize(size + 1);
			*(content + size) = move(value);
		}

		void PushFront(const Type& value)
		{
			size_t sz = Size();
			Resize(sz + 1);
			for (int i = sz; i != 0; --i)
			{
				swap(*(content + i), *(content + i - 1));
			}
			*content = value;
		}

		void PushFront(Type&& value)
		{
			size_t sz = Size();
			Resize(sz + 1);
			for (int i = sz; i != 0; --i)
			{
				swap(*(content + i), *(content + i - 1));
			}
			*content = move(value);
		}

		Type&& PopBack()
		{
			Resize(Size() - 1);
			return move(*(content + Size()));
		}

		Type&& PopFront()
		{
			for (int i = 0; i != Size() - 1; ++i)
			{
				swap(*(content + i), *(content + i + 1));
			}
			Resize(Size() - 1);
			return move(*(content + Size()));
		}

		void Swap(size_t lhs, size_t rhs)
		{
			assert(lhs < Capacity() && rhs < Capacity());
			swap(*(content + lhs), *(content + rhs));
		}

		void Clear()
		{
			Resize(0);
		}

	};
}

#endif // Vector_h__