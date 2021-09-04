#pragma once
#include <initializer_list>
#include "Difinitions.h"
#include <assert.h>


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

	template<class Type>
	class Vector
	{

	private:

		Type*  content  = nullptr;
		size_t size     = 0;
		size_t capacity = MINCAPACITY;

	public:

		typedef Type*		Iterator;
		typedef const Type* ConstIterator;

		explicit Vector(size_t size)
		{
			if (size <= Capacity())
			{
				content = new Type[Capacity() + 1];
			}
			else
			{
				Reserve(size);
			}
		}

		~Vector()
		{
			delete[] content;
		}

		Vector(const Vector& rhs)
		{

		}

		Vector(Vector&& rhs)
		{

		}

		Vector(initializer_list<Type>&& data)
		{
			if (data.size() <= Capacity())
			{
				content = new Type[Capacity() + 1];
			}
			else
			{
				Reserve(data.size());
			}
			
			for (int i = 0; i < data.size(); ++i)
			{
				assert(content + i);
				*(content + i) = *(data.begin() + i);
			}
			size = data.size();
		}

		size_t Size() const
		{
			return size;
		}
		
		size_t Capacity() const
		{
			return capacity;
		}

		const Type& operator[](size_t index) const
		{
			return *(content + index);
		}

		Type& operator[](size_t index)
		{
			return *(content + index);
		}

		void Resize(size_t newSize)
		{
			if (newSize <= Capacity())
			{//cut size but keep the capacity
				size = newSize;
			}
			else
			{//keep size but get a huger capacity
				Reserve(newSize);
			}
		}

		void Reserve(size_t newCapacity)
		{
			if (newCapacity <= Capacity())
				return;
			size_t newCap = (newCapacity / Capacity() + 1) * Capacity();
			auto newContent = new Type[newCap + 1];
			for (int i = 0; i < Size(); ++i)
			{
				swap(newContent[i], content[i]);
			}
			delete[] content;
			content = newContent;
			capacity = newCap;
		}
	};

	template<class Type>
	ostream& operator<<(ostream& os, const Vector<Type>& target)
	{
		if (target.Size() == 0) return os;
		os << target.operator[](0);
		for (int i = 1; i < target.Size(); ++i)
		{
			os << "," << target.operator[](i);
		}
		return os << endl;
	}

	template<class Type>
	class LinkedList
	{

	private:

		struct Node
		{
			Type  value;
			Node* prev;
			Node* next;

			explicit Node()
				:value(NULL), prev(nullptr), next(nullptr)
			{

			}

			Node(const Type& value, Node* prev = nullptr, Node* next = nullptr)
				:value(value), prev(prev), next(next)
			{
				if(prev)
					prev->next = this;
				if(next)
					next->prev = this;
			}

			Node(Type&& value, Node* prev = nullptr, Node* next = nullptr)
				:value(move(value)), prev(prev), next(next)
			{
				if (prev)
					prev->next = this;
				if (next)
					next->prev = this;
			}
		};

		struct ConstIterator
		{
			friend class LinkedList<Type>;
		protected:

			Node* cur;

		protected:

			Type& Retrieve() const
			{
				return cur->value;
			}

			ConstIterator(Node* p)
				:cur(p)
			{

			}

		public:

			ConstIterator()
				:cur(nullptr)
			{

			}

			const Type& operator*() const
			{
				return Retrieve();
			}

			ConstIterator& operator++()
			{
				cur = cur->next;
				return *this;
			}

			ConstIterator operator++(int)
			{
				ConstIterator old = *this;
				++(*this);
				return old;
			}

			bool operator==(const ConstIterator& rhs) const
			{
				return rhs.cur == cur;
			}

			bool operator!=(const ConstIterator& rhs) const
			{
				return !operator==(rhs);
			}

		};

		struct Iterator : ConstIterator
		{
			friend class LinkedList<Type>;
		protected:
			Iterator(Node* p)
				:ConstIterator(p)
			{

			}
		public:

			Iterator& operator++()
			{
				this->cur = this->cur->next;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator old = *this;
				this->cur = this->cur->next;
				return old;
			}

			Type& operator*()
			{
				return this->Retrieve();
			}

			const Type& operator*() const
			{
				return ConstIterator::operator*();
			}
		};

	private:
		
		Node*  head = nullptr;
		Node*  tail = nullptr;
		size_t size = 0;

	private:

		//init head and tail
		void Init()
		{
			head = new Node();
			tail = new Node();
			head->next = tail;
			tail->prev = head;
			size = 0;
		}

	public:
		
	 	explicit LinkedList()
		{

		}

		LinkedList(LinkedList&& rhs)
		{

		}

		LinkedList(const LinkedList& rhs)
		{

		}

		LinkedList(initializer_list<Type> data)
		{
			Init();
			for (int i = 0; i < data.size(); ++i)
			{
				new Node(*(data.begin() + i), tail->prev, tail);
				++size;
			}
		}

		~LinkedList()
		{
			Clear();
			delete head;
			delete tail;
		}

		//delete all available nodes
		void Clear()
		{
			Iterator i = head->next;
			Iterator t = tail;
			while (i != t)
			{
				Node* cur = i.cur;
				++i;
				delete cur;
			}
		}

		Iterator Erase(Iterator it)
		{

		}

		ConstIterator Begin() const
		{
			return ConstIterator(head->next);
		}

		Iterator Begin()
		{
			return Iterator(head->next);
		}

		ConstIterator End() const
		{
			return ConstIterator(tail);
		}

		Iterator End()
		{
			return Iterator(tail);
		}

	};

	template<class Type>
	ostream& operator<<(ostream& os, const LinkedList<Type>& list)
	{
		auto it  = list.Begin();
		auto end = list.End();

		if (it == end) return os;

		os << *it;
		++it;

		while (it != end)
		{
			os << "," << *it;
			++it;
		}

		return os << endl;
	}

	template<class Type>
	class ForwardList
	{

	};

	template<class Type>
	class BackwardList
	{

	};

}