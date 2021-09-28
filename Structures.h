#pragma once

#ifndef Structures_h__
#define Structures_h__

#include <initializer_list>
#include "Difinitions.h"
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include <map>
#include <type_traits>
#include "Helper.h"

namespace Struct
{
	using namespace std;

	/**
	 * Basic structures
	 */

	//a universal type that contains numbers and support unlimited calculation,
	//but now it costs too much, and lacks of efficiency.
	class Number
	{
	private:
		
		string* digits;

	public:

		explicit Number() noexcept
			:digits(new string("")) { }

		~Number()
		{
			delete digits;
		}

		
		//to solve the accuracy problem, I delete the float and double constructor,
		//instead, read a string as a decimal number to construct a Number,
		//however, this constructor need to support e and other formats.
		Number(string&& data)
			:Number()
		{
			//here I decided to implement with a state machine
			/** State transportation table 
			 * State		+/-			Integer			e/E			.				End
			 * Start(0)		Symbol(1)   Integer(2)		Index(4)	Decimal(3)		End(5)
			 * Symbol(1)	End(5)		Integer(2)		Index(4)	Decimal(3)		End(5)
			 * Integer(2)   End(5)		Integer(2)		Index(4)	Decimal(3)		End(5)
			 * Decimal(3)   End(5)		Decimal(3)		End(5)		End(5)			End(5)
			 * Index(4)		End(5)		Index(4)		End(5)		End(5)			End(5)
			 * End(5)		End(5)		End(5)			End(5)		End(5)			End(5)
			 */

			vector<vector<int>> stateTable = { { 1, 2, 4, 3, 5 }, //Start
											   { 5, 2, 4, 3, 5 }, //Symbol
											   { 5, 2, 4, 3, 5 }, //Integer
											   { 5, 3, 5, 5, 5 }, //Decimal
											   { 5, 4, 5, 5, 5 }, //Index
											   { 5, 5, 5, 5, 5 }  //End
											 };
			
			int currState = 0;
			string pow = "";
			for (int i = 0; i != data.size(); ++i)
			{
				char c = data[i];
				if (c == '+' || c == '-')
				{//Symbol
					currState = stateTable[currState][0];
				}
				else if (c <= '9' && c >= '0')
				{//Integer
					currState = stateTable[currState][1];
				}
				else if (c == 'e' || c == 'E')
				{//Index
					currState = stateTable[currState][2];
					
				}
				else if (c == '.')
				{//Decimal
					currState = stateTable[currState][3];
				}
				else
					currState = 5;

				switch (currState)
				{
				case 1:

					digits->push_back(c == '-' ? '-' : '+');
					break;

				case 2:
					if(digits->size() == 0 && c != '0')
						digits->push_back(c);
					else if((*(digits->end() - 1) == '-' || *(digits->end() - 1) == '+') && c == '0')
						break;
					else if(*(digits->end() - 1) != '0')
						digits->push_back(c);
					break;

				case 3:

					digits->push_back(c);
					break;

				case 4:
					
					
					break;
				default:
					break;
				}
				
				if ((i != data.size() - 1 && currState == 5) || (i == data.size() - 1 && *(digits->end() - 1) == '.'))
					assert(0);
				else if(currState == 5)
					break;
			}
											 

		}

		Number(double&&) = delete;
		Number(float&&) = delete;

		//for integers
		template<class Type>
		Number(Type&& data)
			:Number()
		{
			if (data > 0)
			{
				digits->push_back('+');
			}
			else if (data < 0)
			{
				digits->push_back('-');
				data = -1 * data;
			}
			else
			{
				digits->push_back('0');
				return;
			}

			while (data)
			{
				Type digit = data % 10;
				data /= 10;
				digits->insert(digits->begin() + 1, '0' + digit);
			}
		}

	};

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

		Type*  content ;
		size_t size	   ;
		size_t capacity;

	public:

		typedef Type*		Iterator;
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
			if(index > Capacity())
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

		Type PopBack()
		{
			Resize(Size() - 1);
			return *(content + Size());
		}

		Type PopFront()
		{
			for (int i = 0; i != Size() - 1; ++i)
			{
				swap(*(content + i), *(content + i + 1));
			}
			Resize(Size() - 1);
			return *(content + Size());
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
				:value(NULL), prev(nullptr), next(nullptr) { }

			Node(const Type& value, Node* prev = nullptr, Node* next = nullptr)
				:value(value), prev(prev), next(next)
			{
				if (next)
					next->prev = this;
				if (prev)
					prev->next = this;
			}

			Node(Type&& value, Node* prev = nullptr, Node* next = nullptr)
				:value(move(value)), prev(prev), next(next)
			{
				if (next)
					next->prev = this;
				if (prev)
					prev->next = this;
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

			explicit ConstIterator(Node* p) noexcept
				:cur(p) { }

		public:

			ConstIterator() noexcept
				:cur(nullptr) { }

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
				auto old = *this;
				++(*this);
				return old;
			}

			ConstIterator& operator--()
			{
				cur = cur->prev;
				return *this;
			}

			ConstIterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}

			ConstIterator operator+(size_t step)
			{
				auto it = *this;
				while (step)
				{
					++it;
					--step;
				}
				return it;
			}

			ConstIterator operator-(size_t step)
			{
				auto it = *this;
				while (step)
				{
					--it;
					--step;
				}
				return it;
			}

			bool operator==(const ConstIterator& rhs) const
			{
				return rhs.cur == cur;
			}

			bool operator!=(const ConstIterator& rhs) const
			{
				return !(rhs.cur == cur);
			}

		};

		struct Iterator : ConstIterator
		{
			friend class LinkedList<Type>;
		protected:

			Iterator(Node* p)
				:ConstIterator(p) { }

		public:

			Iterator& operator++()
			{
				this->cur = this->cur->next;
				return *this;
			}

			Iterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}

			Iterator& operator--()
			{
				this->cur = this->cur->prev;
				return *this;
			}

			Iterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}

			Iterator operator+(size_t step)
			{
				auto it = *this;
				while (step)
				{
					++it;
					--step;
				}
				return it;
			}

			Iterator operator-(size_t step)
			{
				auto it = *this;
				while (step)
				{
					--it;
					--step;
				}
				return it;
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

		Node* head = nullptr;
		Node* tail = nullptr;
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

		Type DeleteNode(Node* node)
		{
			auto prevNode = node->prev;
			auto nextNode = node->next;
			prevNode->next = nextNode;
			nextNode->prev = prevNode;
			--size;
			Type ret = node->value;
			delete node;
			return ret;
		}

	public:

		explicit LinkedList() { Init(); }

		LinkedList(LinkedList&& rhs) noexcept
		{
			Init();
			swap(rhs.size, size);
			swap(rhs.head, head);
			swap(rhs.tail, tail);
		}

		LinkedList(const LinkedList& rhs)
		{
			Init();
			size = rhs.size;
			auto it = rhs.Begin();
			auto ed = rhs.End();
			while (it != ed)
			{
				new Node(*it, tail->prev, tail);
				++it;
			}

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

		size_t Size()const
		{
			return size;
		}

		Iterator Remove(Iterator it)
		{
			auto prev = it - 1;
			auto next = it + 1;

			prev.cur->next = next.cur;
			next.cur->prev = prev.cur;
			delete it.cur;
			--size;

			return next;
		}

		void Insert(size_t index, const Type& value)
		{
			size_t step = Size() - index - 1;
			if (step >= index)
			{
				return Insert(Begin() + index, value);
			}
			else
			{
				return Insert(End() - 1 - step, value);
			}
		}

		void Insert(size_t index, Type&& value)
		{
			size_t step = Size() - index - 1;
			if (step >= index)
			{
				return Insert(Begin() + index, move(value));
			}
			else
			{
				return Insert(End() - 1 - step, move(value));
			}
		}

		void Insert(Iterator it, const Type& value)
		{
			new Node(value, it.cur->prev, it.cur);
			++size;
		}

		void Insert(Iterator it, Type&& value)
		{
			new Node(value, it.cur->prev, it.cur);
			++size;
		}

		const ConstIterator Begin() const
		{
			return ConstIterator(head->next);
		}

		Iterator Begin()
		{
			return Iterator(head->next);
		}

		const ConstIterator Head() const
		{
			return ConstIterator(head);
		}

		Iterator Head()
		{
			return Iterator(head);
		}

		const ConstIterator End() const
		{
			return ConstIterator(tail);
		}

		Iterator End()
		{
			return Iterator(tail);
		}

		Type& operator[](size_t index)
		{
			size_t back = size - index;
			if (index < back)
			{
				auto It = Begin();
				It = It + index;
				return *It;
			}
			else
			{
				auto It = End();
				It = It - back;
				return *It;
			}
		}

		const Type& operator[](size_t index) const
		{
			size_t back = size - index;
			if (index < back)
			{
				auto It = Begin();
				It = It + index;
				return *It;
			}
			else
			{
				auto It = End();
				It = It - back;
				return *It;
			}
		}

		void PushBack(const Type& value)
		{
			new Node(value, tail->prev, tail);
			++size;
		}

		void PushBack(Type&& value)
		{
			new Node(value, tail->prev, tail);
			++size;
		}

		void PushFront(const Type& value)
		{
			new Node(value, head, head->next);
			++size;
		}

		void PushFront(Type&& value)
		{
			new Node(value, head, head->next);
			++size;
		}

		Type PopBack()
		{
			return DeleteNode(tail->prev);
		}

		Type PopFront()
		{
			return DeleteNode(head->next);
		}

	};

	template<class Type>
	class ForwardList
	{

	private:

		struct Node
		{
			Type  value;
			Node* next;

			explicit Node() noexcept
				:value(NULL), next(nullptr) { }

			Node(const Type& value, Node* next = nullptr) noexcept
				:value(value), next(next) { }

			Node(Type&& value, Node* next = nullptr)
				:value(move(value)), next(next) { }

		};

		struct ConstIterator
		{
			friend class ForwardList<Type>;
		protected:

			const ForwardList* list;
			Node*		 curr;

		protected:

			Type& Retrieve() const
			{
				return this->curr->value;
			}

			ConstIterator Backward(const ConstIterator& start, size_t step)
			{//Imagine that the list is a loop
				auto sz = start.list->Size() + 1 - step;
				auto node = start.curr;

				for (int i = sz; i >= 0; --i)
				{
					node = node->next;
				}

				return ConstIterator(start.list, node);
			}

			explicit ConstIterator(const ForwardList<Type>* list, Node* curr) noexcept
				:list(list), curr(curr) { }

		public:

			ConstIterator() noexcept
				:list(nullptr), curr(nullptr) { }

			const Type& operator*() const
			{
				return Retrieve();
			} 

			ConstIterator& operator++()
			{
				this->curr = this->curr->next;
				return *this;
			}

			ConstIterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}

			ConstIterator& operator--()
			{
				auto h = this->list->head;
				while (h->next != this->curr)
				{
					h = h->next;
				}
				this->curr = h;
				return *this;
			}

			ConstIterator operator--(int)
			{
				auto old = *this;
				--(*this);
				return old;
			}

			ConstIterator operator-(size_t step)
			{
				return Backward(*this, step);
				
			}

			ConstIterator operator+(size_t step)
			{
				auto cur = *this;
				while (step)
				{
					++cur;
					--step;
				}
				return cur;
			}

			bool operator==(const ConstIterator& rhs) const
			{
				return this->list == rhs.list && this->curr == rhs.curr;
			}

			bool operator==(ConstIterator&& rhs) const
			{
				return this->list == rhs.list && this->curr == rhs.curr;
			}

			bool operator!=(const ConstIterator& rhs) const
			{
				return this->list != rhs.list || this->curr != rhs.curr;
			}

			bool operator!=(ConstIterator&& rhs) const
			{
				return this->list != rhs.list || this->curr != rhs.curr;
			}

		};

		struct Iterator : ConstIterator
		{
			friend class ForwardList<Type>;
		protected:

			explicit Iterator(ForwardList<Type>* list, Node* curr) noexcept
				:ConstIterator(list, curr) { }
			
			Iterator Backward(Iterator& start, size_t step)
			{//Imagine that the list is a loop
				auto sz = start.list->Size() + 1 - step;
				auto node = start.curr;

				for (int i = sz; i >= 0; --i)
				{
					node = node->next;
				}

				return Iterator(start.list, node);
			}

		public:

			Type& operator*()
			{
				return this->Retrieve();
			}

			const Type& operator*() const
			{
				return ConstIterator::operator*();
			}

			Iterator& operator++()
			{
				this->curr = this->curr->next;
				return *this;
			}

			Iterator operator++(int)
			{
				auto old = *this;
				++(*this);
				return old;
			}

			Iterator& operator--()
			{
				return ConstIterator::operator--();
			}

			Iterator operator--(int)
			{
				return ConstIterator::operator--(1);
			}

			Iterator operator+(size_t step)
			{
				auto cur = *this;
				while (step)
				{
					++cur;
					--step;
				}
				return cur;
			}

			Iterator operator-(size_t step)
			{
				return Backward(*this, step);
			}

			bool operator==(const Iterator& rhs)
			{
				return this->list == rhs.list && this->curr == rhs.curr;
			}

			bool operator==(Iterator&& rhs)
			{
				return this->list == rhs.list && this->curr == rhs.curr;
			}

			bool operator!=(const Iterator& rhs)
			{
				return this->list != rhs.list || this->curr != rhs.curr;
			}

			bool operator!=(Iterator&& rhs)
			{
				return this->list != rhs.list || this->curr != rhs.curr;
			}
		};

	private:

		Node*  head;
		Node*  curr;
		size_t size;

	private:

		void Init()
		{
			head = new Node();
			curr = head;
			head->next = curr;
			size = 0;
		}
	
public:

		explicit ForwardList()
		{
			Init();
		}

		~ForwardList()
		{
			Clear();
			delete head;
		}

		ForwardList(const initializer_list<Type>& data)
		{
			Init();
			size_t sz = data.size();
			for (int i = 0; i< sz; ++i)
			{
				curr->next = new Node(*(data.begin() + i), head);
				curr = curr->next;
				++size;
			}
		}

		ForwardList(ForwardList&& rhs) noexcept
		{
			Init();
			swap(this->size, rhs.size);
			swap(this->head, rhs.head);
			swap(this->curr, rhs.curr);
		}

		ForwardList(const ForwardList& rhs)
		{
			Init();
			size_t sz = rhs.Size();
			auto it = rhs.Begin();
			auto ed = rhs.End();
			while(it != ed)
			{
				curr->next = new Node(*it, head);
				curr = curr->next;
				++size;
				++it;
			}
		}

		size_t Size() const
		{
			return size;
		}

		Iterator Begin()
		{
			return Iterator(this, head->next);
		}

		const ConstIterator Begin() const
		{
			return ConstIterator(this, head->next);
		}

		Iterator Head()
		{
			return Iterator(this, head);
		}

		const ConstIterator Head() const
		{
			return ConstIterator(this, head);
		}

		Iterator End()
		{
			return Iterator(this, head);
		}

		const ConstIterator End() const
		{
			return ConstIterator(this, head);
		}

		void Clear()
		{
			auto node = head->next;
			while (node != head)
			{
				auto next = node->next;
				delete node;
				node = next;
			}
			curr = head;
			size = 0;
		}

		bool IsEmpty() const
		{
			return head == curr;
		}

		Type PopFront()
		{
			--size;
			auto node = head;
			head = head->next;
			curr->next = head;
			delete node;
			return head->value;
		}

		Type PopBack()
		{
			auto node = curr;
			size_t step = Size();
			while (step)
			{
				node = node->next;
				--step;
			}
			curr->next = head->next;
			delete head;
			head = curr;
			curr = node;
			--size;
			if (size == 0) curr = head;
			return head->value;
		}

		void PushBack(const Type& value)
		{
			curr->next = new Node(value, head);
			curr = curr->next;
			++size;
		}

		void PushBack(Type&& value)
		{
			curr->next = new Node(move(value), head);
			curr = curr->next;
			++size;
		}

		void PushFront(const Type& value)
		{
			auto node = new Node(value, head->next);
			head->next = node;
			++size;
			if (head == curr) curr = head->next;
		}

		void PushFront(Type&& value)
		{
			auto node = new Node(move(value), head->next);
			head->next = node;
			++size;
			if (head == curr) curr = head->next;
		}
	
		void Insert(Iterator it, const Type& value)
		{
			auto val = *it;
			*it = value;
			it.curr->next = new Node(val, it.curr->next);
			++size;
		}

		void Insert(Iterator it, Type&& value)
		{
			auto val = *it;
			*it = move(value);
			it.curr->next = new Node(val, it.curr->next);
			++size;
		}

		void Insert(size_t index, const Type& value)
		{
			return Insert(Begin() + index, value);
		}

		void Insert(size_t index, Type&& value)
		{
			return Insert(Begin() + index, value);
		}

		void Remove(Iterator it)
		{
			auto next = it + 1;
			*it = *next;
			it.curr->next = next.curr->next;
			delete next.curr;
			--size;
			
		}

		void Remove(size_t index)
		{
			Remove(Begin() + index);
		}

};

	//Another version of Forward List, 
	//using a head node to store removed nodes in case of frequently allocation and deallocation.
	//TODO
	//By this design, this list might be able to revoke remove operations(removed nodes are reversely stored in another list)
	//TODO
	//These removed nodes should be deleted when necessary.
	template<class Type>
	class CachedForwardList
	{
	private:

		struct Node
		{
			Type value;
			Node* next;

			Node(const Type& value, Node* next = nullptr)
				:value(value), next(next) {  }

			Node(Type&& value, Node* next = nullptr)
				:value(move(value)), next(next) {  }

			Node(Node* next = nullptr)
				:value(Type()), next(next) {  }
		};

		struct HeadNode : Node
		{//A special node in the head to keep removed nodes as another list.
			
			Node* Cache;

			HeadNode(Node* cache = nullptr, Node* next = nullptr)
				: Node(next), Cache(cache){  }
		};
	
		class ConstIterator
		{

			friend class CachedForwardList<Type>;

		private:

			Node*		  curr;
			const size_t& size;
		
		protected:

			ConstIterator(const CachedForwardList<Type>& par, Node* node)
				:size(par.size), curr(node) {  }

		public:

			const Type& operator*()
			{
				return this->curr->value;
			}

			ConstIterator& operator++()
			{
				this->curr = this->curr->next;
				return *this;
			}

			ConstIterator operator++(int)
			{
				ConstIterator cpy = *this;
				++(*this);
				return cpy;
			}

			ConstIterator& operator--()
			{
				auto s = size;
				while (s)
				{
					this->curr = this->curr->next;
					--s;
				}
				return *this;
			}

			ConstIterator operator--(int)
			{
				ConstIterator cpy = *this;
				--(*this);
				return cpy;
			}

			ConstIterator operator+(int step)
			{
				auto cpy = *this;
				while (step)
				{
					cpy.curr = cpy.curr->next;
					--step;
				}
				return cpy;
			}

			ConstIterator operator-(int step)
			{
				auto cpy = *this;
				auto s = this->size - step + 1;
				while (s)
				{
					cpy.curr = cpy.curr->next;
					--s;
				}
				return cpy;
			}

			bool operator==(const ConstIterator& rhs)
			{
				return &this->size == &rhs.size && this->curr == rhs.curr;
			}

			bool operator!=(const ConstIterator& rhs)
			{
				return &this->size != &rhs.size || this->curr != rhs.curr;
			}
		};

		class Iterator : ConstIterator
		{
			friend class CachedForwardList<Type>;

		protected:

			Iterator(const CachedForwardList<Type>& par, Node* node)
				:ConstIterator(par, node) {  }

		public:

			Type& operator*()
			{
				return this->curr->value;
			}

			Iterator& operator++()
			{
				this->curr = this->curr->next;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator cpy = *this;
				++(*this);
				return cpy;
			}

			Iterator& operator--()
			{
				auto s = this->size;
				while (s)
				{
					this->curr = this->curr->next;
					--s;
				}
				return *this;
			}

			Iterator operator--(int)
			{
				Iterator cpy = *this;
				--(*this);
				return cpy;
			}

			Iterator operator+(int step)
			{
				auto cpy = *this;
				while (step)
				{
					cpy.curr = cpy.curr->next;
					--step;
				}
				return cpy;
			}

			Iterator operator-(int step)
			{
				auto cpy = *this;
				auto s = this->size - step + 1;
				while (s)
				{
					cpy.curr = cpy.curr->next;
					--s;
				}
				return cpy;
			}
			
			bool operator==(const Iterator& rhs)
			{
				return &this->size == &rhs.size && this->curr == rhs.curr;
			}

			bool operator!=(const Iterator& rhs)
			{
				return &this->size != &rhs.size || this->curr != rhs.curr;
			}
		};
			
	private:

		HeadNode* head;
		//for faster PushBacks, 
		//using a tail node to monitor the last available node.
		//This node will not participate in memory alloc, only monitor.
		Node*     tail;
		size_t	  size;

	private:

		//Delete all content, called when destructing.
		void  DoClear()
		{
			auto curr = head->next;
			while (curr != head)
			{
				auto next = curr->next;
				delete curr;
				curr = next;
			}
		}

		void  ClearCache()
		{ 
			Node* curr = head->Cache;
			while (curr)
			{
				auto next = curr->next;
				delete curr;
				curr = next;
			}
			head->next = nullptr;
		}

		Node* AllocNode(const Type& value , Node* next = nullptr)
		{
			if (head->Cache)
			{
				auto cache = head->Cache;
				head->Cache = cache->next;
				cache->value = value;
				cache->next = next;
				return cache;
			}
			else
			{
				return new Node(value, next);
			}

		}

		Node* AllocNode(Type&& value, Node* next = nullptr)
		{
			if (head->Cache)
			{
				auto cache = head->Cache;
				head->Cache = cache->next;
				cache->value = move(value);
				cache->next = next;
				return cache;
			}
			else
			{
				return new Node(move(value), next);
			}

		}

		void  CacheNode(Node* node)
		{
			node->next = head->Cache;
			head->Cache = node;
		}

	public:

		CachedForwardList()
			:head(new HeadNode()), size(0), tail(head)
		{
			head->next = head;
		}
		
		CachedForwardList(initializer_list<Type>&& il)
			:CachedForwardList()
		{
			for (auto iter = il.begin(); iter < il.end(); ++iter)
			{
				tail->next = new Node(*iter, head);
				tail = tail->next;
				++size;
			}
		}

		CachedForwardList(CachedForwardList& rhs)
			:CachedForwardList() 
		{//Trying to avoid allocation

			for (auto iter = rhs.Begin(); iter != rhs.End(); ++iter)
			{
				this->tail->next = rhs.AllocNode(*iter, head);
				this->tail = this->tail->next;
				++size;
			}
		}

		CachedForwardList(CachedForwardList&& rhs)
			:CachedForwardList() 
		{
			swap(this->size, rhs.size);
			swap(this->head, rhs.head);
			swap(this->tail, rhs.tail);
		}

		template<template<class> class Container, class OtherType, 
			class = enable_if_t<is_convertible_v<Type, OtherType>, typename Container<OtherType>::Iterator>>
		CachedForwardList(Container<OtherType>& rhs)
			:CachedForwardList()
		{
			for (auto iter = rhs.Begin(); iter != rhs.End(); ++iter)
			{
				this->tail->next = this->AllocNode(Type(*iter), head);
				this->tail = this->tail->next;
				++size;
			}
		}

		~CachedForwardList()
		{
			DoClear();
			ClearCache();
			delete head;
		}
	
		CachedForwardList& operator=(CachedForwardList& rhs)
		{//Trying to avoid allocation
			
			Clear();
			for (auto iter = rhs.Begin(); iter != rhs.End(); ++iter)
			{
				this->tail->next = this->head->Cache ? this->AllocNode(*iter, head) : rhs.AllocNode(*iter, head);
				this->tail = this->tail->next;
				++size;
			}
			return *this;
		}

		CachedForwardList& operator=(CachedForwardList&& rhs)
		{
			swap(this->size, rhs.size);
			swap(this->head, rhs.head);
			swap(this->tail, rhs.tail);
			return *this;
		}

		template<template<class> class Container, class OtherType, 
			class = enable_if_t<is_convertible_v<Type, OtherType>, typename Container<OtherType>::Iterator>>
		CachedForwardList& operator=(Container<OtherType>& rhs)
		{
			Clear();
			for (auto iter = rhs.Begin(); iter != rhs.End(); ++iter)
			{
				this->tail->next = this->AllocNode(*iter, head);
				this->tail = this->tail->next;
				++size;
			}
			return *this;
		}

		//Delete all content.
		void Clear()
		{//but not really delete them, instead, throw them into the caches.

			Node* curr = head->next;
			while (curr != head)
			{
				auto next = curr->next;
				curr->next = head->Cache;
				head->Cache = curr;
				curr = next;
			}
			head->next = head;
			tail = head;
			size = 0;
		}

		size_t Size() const
		{
			return size;
		}

		bool IsEmpty() const
		{
			return !size;
		}

		ConstIterator Begin() const
		{
			return ConstIterator(*this, head->next);
		}

		Iterator	  Begin()
		{
			return Iterator(*this, head->next);
		}

		ConstIterator End() const
		{
			return ConstIterator(*this, head);
		}

		Iterator	  End()
		{
			return Iterator(*this, head);
		}

		ConstIterator Back() const
		{
			return ConstIterator(*this, tail);
		}

		Iterator	  Back() 
		{
			return Iterator(*this, tail);
		}

		void PushBack(const Type& value)
		{
			tail->next = AllocNode(value, head);
			tail = tail->next;
			++size;
		}

		void PushBack(Type&& value)
		{
			tail->next = AllocNode(move(value), head);
			tail = tail->next;
			++size;
		}

		void PushFront(const Type& value)
		{
			head->next = AllocNode(value, head->next);
			++size;
		}

		void PushFront(Type&& value)
		{
			head->next = AllocNode(move(value), head->next);
			++size;
		}

		//A terrible O(n) algorithm
		Type PopBack()
		{
			assert(size);
			Iterator it(*this, tail);
			tail = (--it).curr;
			CacheNode(tail->next);
			tail->next = head;
			--size;
			return head->Cache->value;
		}

		Type PopFront()
		{
			assert(size);
			if(size != 1)
			{
				auto node = head->next;
				head->next = node->next;
				CacheNode(node);
				--size;
				return head->Cache->value;
			}
			else
			{
				CacheNode(head->next);
				--size;
				head->next = head;
				tail = head;
				return head->Cache->value;
			}
		}

		//Remove an element by the iterator,
		//after the operator, 
		//the original iterator will point at the next element.
		void Remove(Iterator& iter)
		{
			assert(iter.curr != head);
			if (iter.curr != tail)
			{
				auto next = iter.curr->next;
				iter.curr->value = next->value;
				iter.curr->next = next->next;
				--size;
				CacheNode(next);
			}
			else
			{//Removing the last element costs much.
				Iterator prev = iter - 1;
				tail = prev.curr;
				tail->next = head;
				CacheNode(iter.curr);
				iter.curr = head->next;
				--size;
			}
		}

		void Remove(Iterator&& iter)
		{
			Iterator it = move(iter);
			Remove(it);
		}

		void Remove(size_t index)
		{
			Remove(Begin() + index);
		}

		void Insert(Iterator& iter, const Type& value)
		{
			iter.curr->next =  AllocNode(value, iter.curr->next);
			++size;
			if (iter.curr == tail)
				tail = tail->next;
		}

		void Insert(Iterator& iter, Type&& value)
		{
			iter.curr->next = AllocNode(move(value), iter.curr->next);
			++size;
			if (iter.curr == tail)
				tail = tail->next;
		}
	
		void Insert(Iterator&& iter, const Type& value)
		{
			Iterator it = iter;
			Insert(it, value);
		}

		void Insert(Iterator&& iter, Type&& value)
		{
			Iterator it = iter;
			Insert(it, move(value));
		}
	
		void Insert(size_t index, const Type& value)
		{
			Insert(Begin() + index, value);
		}

		void Insert(size_t index, Type&& value)
		{
			Insert(Begin() + index, move(value));
		}
		
		
	};

	/**
	 * Advanced Structures
	 */

	template<class Type, template<class> class Container = CachedForwardList>
	class Stack
	{
	private:

		Container<Type>* content;

	public:

		explicit Stack() noexcept
			:content(new Container<Type>()) { }

		~Stack()
		{
			delete content;
		}

		Stack(const initializer_list<Type>& data)
			:Stack()
		{
			for(int i = 0; i < data.size(); ++i)
				content->PushFront(*(data.begin() + i));
		}

		Stack(const Stack& rhs)
			:Stack()
		{
			auto newContent = new Container<Type>(*rhs.content);
			swap(content, newContent);
		}

		Stack(Stack&& rhs)
			:Stack()
		{
			swap(content, rhs.content);
		}

		const Type& Top() const
		{
			return *content->Begin();
		}

		Type& Top()
		{
			return *content->Begin();
		}

		Type Pop()
		{
			return content->PopFront();
		}

		void Push(Type&& value)
		{
			content->PushFront(value);
		}

		void Push(const Type& value)
		{
			content->PushFront(value);
		}

		size_t Size() const
		{
			return content->Size();
		}

		bool IsEmpty() const
		{
			return content->IsEmpty();
		}

		void Clear()
		{
			return content->Clear();
		}
	};

	template<class Type, template<class> class Container = CachedForwardList>
	class Queue
	{
	private:

		Container<Type>* content;

	public:

		Queue()
			:content(new Container<Type>()) { }

		~Queue()
		{
			delete content;
		}

		Queue(initializer_list<Type> data)
			:Queue()
		{
			for (auto beg = data.begin(); beg != data.end(); ++beg)
			{
				content->PushBack(*beg);
			}
		}

		Queue(const Queue& rhs)
			:Queue()
		{
			auto newContent = new Container<Type>(*rhs.content);
			swap(newContent, content);
		}

		Queue(Queue&& rhs)
			:Queue()
		{
			swap(content, rhs.content);
		}

		Type Dequeue()
		{
			return content->PopFront();
		}

		void Enqueue(const Type& value)
		{
			return content->PushBack(value);
		}

		void Enqueue(Type&& value)
		{
			return content->PushBack(value);
		}

		const Type& Front() const
		{
			return *content->Begin();
		}

		size_t Size() const
		{
			return content->Size();
		}

		bool IsEmpty() const
		{
			return content->IsEmpty();
		}

	};

	template<class Type, template<class> class Container = LinkedList>
	class Deque
	{
	private:

		Container<Type>* content;

	public:

		Deque()
			:content(new Container<Type>()) { }

		~Deque()
		{
			delete content;
		}

		Deque(const initializer_list<Type>& data)
			:Deque()
		{
			for (auto beg = data.begin(); beg != data.end(); ++beg)
			{
				content->PushBack(*beg);
			}
		}

		Deque(const Deque& rhs)
			:Deque()
		{
			auto newContent = new Container<Type>(*rhs.content);
			swap(content, newContent);
		}

		Deque(Deque&& rhs)
			:Deque()
		{
			swap(rhs.content, content);
		}

		Type Pop()
		{
			return content->PopFront();
		}

		void Push(const Type& val)
		{
			return content->PushFront(val);
		}

		void Push(Type&& val)
		{
			return content->PushFront(val);
		}

		void Inject(const Type& val)
		{
			return content->PushBack(val);
		}

		void Inject(Type&& val)
		{
			return content->PushBack(val);
		}

		Type Eject()
		{
			return content->PopBack();
		}



	};

	template<class Type>
	class BinaryTree
	{
		template<template<class> class Tree, class Type>
		friend auto operator<<(ostream& os, const Tree<Type>& tree)->
			decltype(tree.root, declval<ostream&>() << declval<Type>(), os);
	private:

		struct Node
		{

			Type  value;
			Node* left ;
			Node* right;

			Node() = default;
			explicit Node(Type value, Node* left = nullptr, Node* right = nullptr) noexcept
				:value(value), left(left), right(right) { }

		};

	private:

		static void DoClear(Node* root)
		{
			if (root)
			{
				if (root->left)
					DoClear(root->left);
				if (root->right)
					DoClear(root->right);
				delete root;
			}
		}

		//Copy a tree from a root node to another root node
		static Node* CpyTree(Node* destinationRoot, Node* resourceRoot)
		{
			destinationRoot->value = resourceRoot->value;
			if(resourceRoot->left)
			{
				destinationRoot->left = CpyTree(new Node(), resourceRoot->left);
			}
			if (resourceRoot->right)
			{
				destinationRoot->right = CpyTree(new Node(), resourceRoot->right);
			}
				return destinationRoot;
		}
		
		static Node* DeepthFirstSearch(Node* root, const Type& value)
		{
			if (!root) return nullptr;
			if (root->value == value) return root;

			if (root->left)
			{
				auto leftRet = DeepthFirstSearch(root->left, value);
				if (leftRet) return leftRet;
			}
			if (root->right)
			{
				auto rightRet = DeepthFirstSearch(root->right, value);
				if (rightRet) return rightRet;
			}
			return nullptr;

		}

		static Node* BreadthFirstSearch(Node* root, const Type& value)
		{
			assert(root);
			Queue<Node*> queue;
			queue.Enqueue(root);
			while (!queue.IsEmpty())
			{
				Node* curr = queue.Dequeue();
				if (curr->left) queue.Enqueue(curr->left);
				if (curr->right) queue.Enqueue(curr->right);
				if (curr->value == value) return curr;
			}
			return nullptr;

		}

		//make sure that the result node is not complete
		static Node* FindHighestLeaf(Node* root)
		{
			Queue<Node*> queue;
			queue.Enqueue(root);
			while (!queue.IsEmpty())
			{
				auto node = queue.Dequeue();
				if (!node->left || !node->right) return node;
				else
				{
					if(node->left)
						queue.Enqueue(node->left);
					if (node->right)
						queue.Enqueue(node->right);
				}
			}
			return nullptr;
		}


	private:

		Node*  root;
		size_t size;

	public:

		explicit BinaryTree() noexcept
			:root(new Node()), size(0)
		{ }

		~BinaryTree()
		{
			DoClear(root);
		}

		BinaryTree(const initializer_list<Type>& data)
			:BinaryTree()
		{
			assert(data.size());
			Queue<Node*> roots;
			roots.Enqueue(root);
			root->value = *(data.begin());
			++size;
			for (int i = 1; i < data.size(); ++i)
			{
				Node* front = roots.Front();
				if (front->left && front->right)
					roots.Dequeue();
				front = roots.Front();

				Node* node = new Node(*(data.begin() + i));
				roots.Enqueue(node);
				if (!front->left)
				{
					front->left = node;
				}
				else if (!front->right)
				{
					front->right = node;
				}
			}
			size = data.size();
			

		}

		BinaryTree(const BinaryTree& rhs)
			:BinaryTree()
		{
			size = rhs.size;
			CpyTree(root, rhs.root);
		}

		BinaryTree(BinaryTree&& rhs)
			:BinaryTree()
		{
			swap(rhs.root, root);
			swap(rhs.size, size);
		}

		void Clear()
		{
			DoClear(root->left);
			DoClear(root->right);
			size = 0;
		}
		
		size_t Size() const noexcept
		{
			return size;
		}

		const Node* DFS(Type value) const
		{
			return DeepthFirstSearch(root, value);
		}

		const Node* BFS(Type value) const
		{
			return BreadthFirstSearch(root, value);
		}

		void Add(const Type& value)
		{
			if (!size)
			{
				root->value = value;
				++size;
				return;
			}
			auto node = FindHighestLeaf(root);
			if (!node->left)
			{
				node->left = new Node(value);
			}
			else
			{
				node->right = new Node(value);
			}
			++size;
		}

		void Add(Type&& value)
		{
			if (!size)
			{
				root->value = move(value);
				++size;
				return;
			}
			auto node = FindHighestLeaf(root);
			if (!node->left)
			{
				node->left = new Node(move(value));
			}
			else
			{
				node->right = new Node(move(value));
			}
			++size;
		}

		void Remove(const Type& value)
		{
			Queue<Node*> nodes;
			Node* mark		   = nullptr;
			Node* markParent   = nullptr;//used when remove the root node
			Node* targetParent = nullptr;
			Node* target	   = nullptr;
			nodes.Enqueue(root);
			while (!nodes.IsEmpty())
			{
				auto curr = nodes.Dequeue();
				if (curr->left && curr->left->value != value && !curr->left->left && !curr->left->right)
					markParent = curr, mark = curr->left;
				if (curr->right && curr->right->value != value && !curr->right->left && !curr->right->right)
					markParent = curr, mark = curr->right;
				if (curr->left)
					nodes.Enqueue(curr->left);
				if (curr->right)
					nodes.Enqueue(curr->right);
				if (curr->left && curr->left->value == value)
					target = curr->left, targetParent = curr;
				if(curr->right && curr->right->value == value)
					target = curr->right, targetParent = curr;
				if (targetParent && mark) break;
			}
			if (mark && target)
			{
				mark->left = target->left;
				mark->right = target->right;
				if (target == targetParent->left)
					targetParent->left = nullptr;
				else
					targetParent->right = nullptr;
				delete target;
			}
			else if (mark && root->value == value)
			{
				mark->left = root->left;
				mark->right = root->right;
				delete root;
				root = mark;
				if (mark == markParent->left)
					markParent->left = nullptr;
				else
					markParent->right = nullptr;
				//root is the element to remove
			}
			else if(size != 1)
			{
				//do not find such an element, do nothing.
				return;
			}
			--size;
		}

		void Remove(Type&& value)
		{
			Type v = move(value);
			Remove(v);
		}

		Node* Contains(const Type& value)
		{
			Queue<Node*> nodes;
			nodes.Enqueue(root);
			while (!nodes.IsEmpty())
			{
				auto curr = nodes.Dequeue();
				if (curr->left)
					nodes.Enqueue(curr->left);
				if (curr->right)
					nodes.Enqueue(curr->right);
				if (curr->value == value) return curr;
			}
			return nullptr;
		}

		Node* Contains(Type&& value)
		{
			Type v = move(value);
			return Contains(v);
		}

		const Node* Contains(const Type& value) const
		{
			Queue<Node*> nodes;
			nodes.Enqueue(root);
			while (!nodes.IsEmpty())
			{
				auto curr = nodes.Dequeue();
				if (curr->left)
					nodes.Enqueue(curr->left);
				if (curr->right)
					nodes.Enqueue(curr->right);
				if (curr->value == value) return curr;
			}
			return nullptr;
		}

		const Node* Contains(Type&& value) const
		{
			Type v = move(value);
			return Contains(v);
		}
	};

	template<class Type>
	class BinarySearchTree
	{
		template<template<class> class Tree, class Type>
		friend auto operator<<(ostream& os, const Tree<Type>& tree)->
			decltype(tree.root, declval<ostream&>() << declval<Type>(), os);
	
	private:

		struct Node
		{
			Type  value;
			Node* left ;
			Node* right;

			Node() = default;

			Node(const Type& value,Node* left = nullptr, Node* right = nullptr)
				:value(value),left(left), right(right){  }

			Node(Type&& value, Node* left = nullptr, Node* right = nullptr)
				:value(move(value)), left(left), right(right) {  }
		
		};
	
	private:

		Node*  root;
		size_t size;
	
	private:

		bool Contains(Type&& value, Node* root) const
		{
			if (!root) return false;
			while (root)
			{
				if (root->value < value)
					root = root->right;
				else if (value > root->value)
					root = root->left;
				else
					return true;
			}
			return false;
		}

		bool Contains(const Type& value, Node* root) const
		{
			if (!root) return false;
			while (root)
			{
				if (root->value < value)
					root = root->right;
				else if (value > root->value)
					root = root->left;
				else
					return true;
			}
			return false;
		}

		Node* FindMax(Node* root) const
		{
			while (root)
			{
				if (!root->right)
					break;
				root = root->right;
			}
			return root;
		}

		Node* FindMin(Node* root) const
		{
			while (root)
			{
				if (!root->left)
					break;
				root = root->left;
			}
			return root;
		}

		void Insert(const Type& value, Node*& root)
		{
			if (!root) root = new Node(value);
			if (root->value < value)
				Insert(value, root->right);
			else if (value < root->value)
				Insert(value, root->left);
			else
				return;
		}

		void Insert(Type&& value, Node*& root)
		{
			if (!root) root = new Node(move(value));
			if (root->value < value)
				Insert(move(value), root->right);
			else if (value < root->value)
				Insert(move(value), root->left);
			else
				return;
		}

		void Remove(const Type& value, Node*& root)
		{
			if (!root) return;
			if (root->value < value)
				Remove(value, root->right);
			else if (value < root->value)
				Remove(value, root->left);
			else if (root->left && root->right)
			{
				root->value = FindMin(root->right)->value;
				Remove(root->value, root->right);
			}
			else
			{
				Node* prev = root;
				root = root->left ? root->left : root->right;
				delete prev;
			}
		}

		void Clear(Node* root)
		{
			if (!root) return;
			if (root->left)
				Clear(root->left);
			if (root->right)
				Clear(root->right);
			delete root;
		}

		void Clone(Node* toRoot, Node* fromRoot)
		{
			if (!fromRoot || !toRoot)
				return;
			toRoot->value = fromRoot->value;
			if (fromRoot->left)
			{
				toRoot->left = new Node();
				Clone(toRoot->left, fromRoot->left);
			}
			if (fromRoot->right)
			{
				toRoot->right = new Node();
				Clone(toRoot->right, fromRoot->right);
			}
		}

	public:

		//Unable to construct if the given Type can not be compared.
		template<class = decltype(declval<Type>() < declval<Type>(), declval<Type>() != declval<Type>(), true)>
		BinarySearchTree()
			:root(nullptr), size(0) {  }
		
		BinarySearchTree(initializer_list<Type>&& il)
			:BinarySearchTree()
		{
			for (auto iter = il.begin(); iter != il.end(); ++iter)
			{
				Insert(*iter, root);
				++size;
			}
		}

		BinarySearchTree(const BinarySearchTree& rhs)
			:BinarySearchTree()
		{
			root = new Node();
			Clone(root, rhs.root);
			size = rhs.size;
		}

		BinarySearchTree(BinarySearchTree&& rhs)
			:BinarySearchTree()
		{
			swap(rhs.root, root);
			swap(rhs.size, size);
		}

		~BinarySearchTree()
		{
			Clear(root);
		}

		BinarySearchTree& operator=(const BinarySearchTree& rhs)
		{
			Clear(root);
			root = new Node();
			Clone(root, rhs.root);
			size = rhs.size;
			return *this;
		}

		BinarySearchTree& operator=(BinarySearchTree&& rhs)
		{
			swap(rhs.root, root);
			swap(rhs.size, size);
			return *this;
		}

		bool IsEmpty() const 
		{
			return !size;
		}
		
		void Clear()
		{
			Clear(root);
			root = nullptr;
		}

		void Remove(const Type& value)
		{
			Remove(value, root);
			--size;
		}

		void Remove(Type&& value)
		{
			Type v = move(value);
			Remove(v, root);
			--size;
		}

		void Insert(const Type& value)
		{
			Insert(value, root);
			++size;
		}

		void Insert(Type&& value)
		{
			Insert(move(value), root);
			++size;
		}

		bool Contains(Type&& value) const
		{
			return Contains(move(value), root);
		}

		bool Contains(const Type& value) const
		{
			return Contains(value, root);
		}

	};

	template<class Type>
	class BalancedBinarySearchTree
	{
		template<template<class> class Tree, class Type>
		friend auto operator<<(ostream& os, const Tree<Type>& tree)->
			decltype(tree.root, declval<ostream&>() << declval<Type>(), os);
	
	private:

		struct Node
		{
			Type  value ;
			Node* left  ;
			Node* right ;
			int   height;

			Node(const Type& value, int height = 0, Node* left = nullptr, Node* right = nullptr)
				:value(value), height(height), left(left), right(right) {  }
			
			Node(Type&& value, int height = 0, Node* left = nullptr, Node* right = nullptr)
				:value(move(value)), height(height), left(left), right(right) {  }
		
		};
	
	private:

		Node*  root;
		size_t size;
	
	private:

		int height(Node* node) const
		{
			return node ? node->height : -1;
		}

		//Imbalance happened by the insertion at the right of the root node's right child.
		//That means the height of right child - that of left child > 1
		void RotateLeft(Node*& root)
		{
			if (!root) return;
			Node* prev = root;
			root = prev->right;
			prev->right = root->left;
			root->left = prev;
			prev->height = max(height(prev->left), height(prev->right)) + 1;
			root->height = max(height(root->left), height(root->right)) + 1;
		}

		//Imbalance happened by the insertion at the left of the root node's left child.
		//That means the height of left child - that of right child > 1
		void RotateRight(Node*& root)
		{
			if (!root) return;
			Node* prev = root;
			root = prev->left;
			prev->left = root->right;
			root->right = prev;
			prev->height = max(height(prev->left), height(prev->right)) + 1;
			root->height = max(height(root->left), height(root->right)) + 1;

		}

		void RotateRightLeft(Node*& root)
		{
			if (!root) return;
			RotateRight(root->left);
			RotateLeft(root);

		}

		void RotateLeftRight(Node*& root)
		{
			if (!root) return;
			RotateLeft(root->left);
			RotateRight(root);

		}

		void Balance(Node*& root)
		{
			if (!root) return;
			if (height(root->left) - height(root->right) > 1)
				if (height(root->left->left) >= height(root->left->right))
					RotateRight(root);
				else
					RotateLeftRight(root);
			if (height(root->right) - height(root->left) > 1)
				if (height(root->right->right) >= height(root->right->left))
					RotateLeft(root);
				else
					RotateRightLeft(root);\

			root->height = max(height(root->left), height(root->right)) + 1;
		}
		
		void Insert(const Type& value, Node*& root)
		{
			if (!root) root = new Node(value);
			if (root->value < value)
				Insert(value, root->right);
			elif (value < root->value)
				Insert(value, root->left);

			Balance(root);
		}

		void Insert(Type&& value, Node*& root)
		{
			if (!root) root = new Node(move(value));
			if (root->value < value)
				Insert(move(value), root->right);
			elif (value < root->value)
				Insert(move(value), root->left);

			Balance(root);
		}

		void Remove(const Type& value, Node*& root)
		{
			if (!root) return;
			if (root->value < value)
				Remove(value, root->right);
			else if (value < root->value)
				Remove(value, root->left);
			else if (root->left && root->right)
			{
				root->value = FindMin(root->right)->value;
				Remove(root->value, root->right);
			}
			else
			{
				Node* prev = root;
				root = root->left ? root->left : root->right;
				delete prev;
			}

			Balance(root);
		}

		void Clear(Node* root)
		{
			if (!root) return;
			if (root->left)
				Clear(root->left);
			if (root->right)
				Clear(root->right);
			delete root;
		}

		void Clone(Node* toRoot, Node* fromRoot)
		{

		}

		bool Contains(const Type& value)
		{

			return false;
		}

	public:
		//Unable to construct if the given Type can not be compared.
		template<class = decltype(declval<Type>() < declval<Type>(), declval<Type>() != declval<Type>(), true)>
		BalancedBinarySearchTree()
			:root(nullptr), size(0) {   }
	
		BalancedBinarySearchTree(initializer_list<Type>&& il)
			:BalancedBinarySearchTree()
		{
			for (auto iter = il.begin(); iter < il.end(); ++iter)
			{
				Insert(*iter);
			}
		}

		~BalancedBinarySearchTree()
		{
			Clear(root);
		}

		bool IsEmpty() const
		{
			return !size;
		}

		void Clear()
		{
			Clear(root);
			root = nullptr;
		}

		void Remove(const Type& value)
		{
			Remove(value, root);
			--size;
		}

		void Remove(Type&& value)
		{
			Remove(move(value), root);
			--size;
		}

		void Insert(const Type& value)
		{
			Insert(value, root);
			++size;
		}

		void Insert(Type&& value)
		{
			Insert(move(value), root);
			++size;
		}

	};

	template<class Type>
	using AVLTree = BalancedBinarySearchTree<Type>;

	template<class Type>
	class SPTree
	{

	};

	template<class Type>
	class ThreadedBinaryTree
	{
	private:

		struct Node
		{
			Type  value ;
			Node* left  ;
			bool  lChild;
			Node* right ;
			bool  rChild;

			Node(const Type& value, Node* left = nullptr, Node* right = nullptr, bool lChild = 1, bool rChild = 1)
				:value(value), left(left), right(right), lChild(lChild), rChild(rChild){  }

			Node(Type&& value, Node* left = nullptr, Node* right = nullptr, bool lChild = 1, bool rChild = 1)
				:value(value), left(left), right(right), lChild(lChild), rChild(rChild) {  }

		};

	private:

		Node*  head;
		Node*  root;
		size_t size;
	
	private:

		void Insert(const Type& value, Node*& root)
		{
			if (!size)
			{
				this->root = new Node(value, head, head, 0, 0);
				head->right = this->root;
				head->left = this->root;
				++size;
			}
			elif(!root)
			{
				root = new Node(value);
				++size;
			}
			elif(value < root->value)
			{
				if (!root->lChild)
				{
					Node* prev = root->left;
					root->left = nullptr;
					Insert(value, root->left);
					root->lChild = 1;
					root->left->left = prev;
					root->left->lChild = 0;
					root->left->right = root;
					root->left->rChild = 0;
					if (prev == head)
					{
						head->right = root->left;
					}
				}
				else
				{
					Insert(value, root->left);
					root->lChild = 1;
				}
			}
			elif(root->value < value)
			{
				if (!root->rChild)
				{
					Node* next = root->right;
					root->right = nullptr;
					Insert(value, root->right);
					root->rChild = 1;
					root->right->right = next;
					root->right->rChild = 0;
					root->right->left = root;
					root->right->lChild = 0;
					if (next == head)
					{
						head->left = root->right;
					}
				}
				else
				{
					Insert(value, root->right);
					root->rChild = 1;
				}
			}
		}

		void Insert(Type&& value, Node*& root)
		{
			if (!size)
			{
				this->root = new Node(move(value), head, head, 0, 0);
				head->right = this->root;
				head->left = this->root;
				++size;
			}
			elif(!root)
			{
				root = new Node(move(value));
				++size;
			}
			elif(value < root->value)
			{
				if (!root->lChild)
				{
					Node* prev = root->left;
					root->left = nullptr;
					Insert(value, root->left);
					root->lChild = 1;
					root->left->left = prev;
					root->left->lChild = 0;
					root->left->right = root;
					root->left->rChild = 0;
					if (prev == head)
					{
						head->right = root->left;
					}
				}
				else
				{
					Insert(value, root->left);
					root->lChild = 1;
				}
			}
			elif(root->value < value)
			{
				if (!root->rChild)
				{
					Node* next = root->right;
					root->right = nullptr;
					Insert(value, root->right);
					root->rChild = 1;
					root->right->right = next;
					root->right->rChild = 0;
					root->right->left = root;
					root->right->lChild = 0;
					if (next == head)
					{
						head->left = root->right;
					}
				}
				else
				{
					Insert(value, root->right);
					root->rChild = 1;
				}
			}
		}

		void Clear(Node* root)
		{
			if (root)
			{
				if (root->lChild && root->lChild)
					Clear(root->left);
				if (root->rChild && root->right)
					Clear(root->right);
				delete root;
			}
		}

	public:

		ThreadedBinaryTree()
			:head(new Node(Type(), nullptr, nullptr, 0, 0)), root(nullptr), size(0) {  }

		ThreadedBinaryTree(initializer_list<Type>&& il)
			:ThreadedBinaryTree()
		{
			for (auto iter = il.begin(); iter != il.end(); ++iter)
			{
				Insert(*iter, root);
			}
		}

		~ThreadedBinaryTree()
		{
			Clear(root);
			delete head;
		}
	};

	template<class Type>
	class ThreadedBalancedBinarySearchTree
	{
	private:

		struct Node
		{
			Type  value;
			Node* left;
			bool  lChild;
			Node* right;
			bool  rChild;

			Node(const Type& value, Node* left = nullptr, Node* right = nullptr, bool lChild = 1, bool rChild = 1)
				:value(value), left(left), right(right), lChild(lChild), rChild(rChild) {  }

			Node(Type&& value, Node* left = nullptr, Node* right = nullptr, bool lChild = 1, bool rChild = 1)
				:value(value), left(left), right(right), lChild(lChild), rChild(rChild) {  }

		};

	private:

		Node* head;
		Node* root;
		size_t size;

	private:

		void Insert(const Type& value, Node*& root)
		{
			if (!size)
			{
				this->root = new Node(value, head, head, 0, 0);
				head->right = this->root;
				head->left = this->root;
				++size;
			}
			elif(!root)
			{
				root = new Node(value);
				++size;
			}
			elif(value < root->value)
			{
				if (!root->lChild)
				{
					Node* prev = root->left;
					root->left = nullptr;
					Insert(value, root->left);
					root->lChild = 1;
					root->left->left = prev;
					root->left->lChild = 0;
					root->left->right = root;
					root->left->rChild = 0;
					if (prev == head)
					{
						head->right = root->left;
					}
				}
				else
				{
					Insert(value, root->left);
					root->lChild = 1;
				}
			}
			elif(root->value < value)
			{
				if (!root->rChild)
				{
					Node* next = root->right;
					root->right = nullptr;
					Insert(value, root->right);
					root->rChild = 1;
					root->right->right = next;
					root->right->rChild = 0;
					root->right->left = root;
					root->right->lChild = 0;
					if (next == head)
					{
						head->left = root->right;
					}
				}
				else
				{
					Insert(value, root->right);
					root->rChild = 1;
				}
			}
		}

		void Insert(Type&& value, Node*& root)
		{
			if (!size)
			{
				this->root = new Node(move(value), head, head, 0, 0);
				head->right = this->root;
				head->left = this->root;
				++size;
			}
			elif(!root)
			{
				root = new Node(move(value));
				++size;
			}
			elif(value < root->value)
			{
				if (!root->lChild)
				{
					Node* prev = root->left;
					root->left = nullptr;
					Insert(value, root->left);
					root->lChild = 1;
					root->left->left = prev;
					root->left->lChild = 0;
					root->left->right = root;
					root->left->rChild = 0;
					if (prev == head)
					{
						head->right = root->left;
					}
				}
				else
				{
					Insert(value, root->left);
					root->lChild = 1;
				}
			}
			elif(root->value < value)
			{
				if (!root->rChild)
				{
					Node* next = root->right;
					root->right = nullptr;
					Insert(value, root->right);
					root->rChild = 1;
					root->right->right = next;
					root->right->rChild = 0;
					root->right->left = root;
					root->right->lChild = 0;
					if (next == head)
					{
						head->left = root->right;
					}
				}
				else
				{
					Insert(value, root->right);
					root->rChild = 1;
				}
			}
		}

		void Clear(Node* root)
		{
			if (root)
			{
				if (root->lChild && root->lChild)
					Clear(root->left);
				if (root->rChild && root->right)
					Clear(root->right);
				delete root;
			}
		}

	public:

		ThreadedBalancedBinarySearchTree()
			:head(new Node(Type(), nullptr, nullptr, 0, 0)), root(nullptr), size(0) {  }

		ThreadedBalancedBinarySearchTree(initializer_list<Type>&& il)
			:ThreadedBalancedBinarySearchTree()
		{
			for (auto iter = il.begin(); iter != il.end(); ++iter)
			{
				Insert(*iter, root);
			}
		}

		~ThreadedBalancedBinarySearchTree()
		{
			Clear(root);
			delete head;
		}
	};

	template<class Type, template<class>class CompareObject = Less>
	class BinaryHeap
	{

	private:

		Vector<Type> data;

	public:

		BinaryHeap() = default;

		BinaryHeap(initializer_list<Type>&& il)
		{
			for (auto iter = il.begin(); iter != il.end(); ++iter)
			{
				Insert(*iter);
			}
		}

		void Insert(const Type& value)
		{
			int location = data.Size();
			data.PushBack(value);
			int parLoc = (location - 1) / 2;
			while (parLoc >= 0 && CompareObject<Type>()(data[location], data[parLoc]))
			{
				swap(data[parLoc], data[location]);
				location = parLoc;
				parLoc = (location - 1) / 2;
			}

		}
			
		Type RemoveRoot()
		{
			auto size = data.Size();
			assert(size, "Fault: Can Not Remove A Root From An Empty Heap!");

			swap(data[0], data[size - 1]);
			int curr = 0, child = 1;
			while (child < size - 1)
			{
				if (child != size - 2 && CompareObject<Type>()(data[child + 1], data[child]))
					++child;
				if(CompareObject<Type>()(data[child], data[curr]))
					swap(data[child], data[curr]);
				else
					break;
				curr = child;
				child = curr * 2 + 1;
			}
			data.Resize(size - 1);
			return data[size - 1];
		}

		bool IsEmpty() const
		{
			return data.IsEmpty();
		}

		size_t Size() const
		{
			return data.Size();
		}
		
		void Check(int root = 0)
		{
			int left = root * 2 + 1, right = left + 1;
			if (left < Size())
			{
				if (CompareObject<Type>()(data[left], data[root]))
					cout << "Invalid Node Detected At Root: " << root << endl;
			}
			elif(right < Size())
			{
				if (CompareObject<Type>()(data[right], data[root]))
					cout << "Invalid Node Detected At Root: " << root << endl;
			}
			else
				return;
			Check(left);
			Check(right);
		}
	
	};

	template<class Type, template<class>class CompareObject = Less>
	class LeftistHeap
	{
		template<template<class, template<class> class> class Tree, class Type, template<class> class CompareObject>
		friend inline auto operator<<(ostream& os, const Tree<Type, CompareObject>& tree)->
			decltype(tree.root, declval<ostream&>() << declval<Type>(), os);
	private:

		struct Node
		{
			Type   value;
			Node*  left;
			Node*  right;
			size_t npl;  //Null Pointer Path Length

			Node(const Type& value, Node* left = nullptr, Node* right = nullptr)
				:value(value), left(left), right(right) 
			{
				if (left && right)
					npl = ::min(left->npl, right->npl) + 1; 
				elif(left && !right)
					npl = left->npl + 1;
				elif(right && !left)
					npl = right->npl + 1;
				else
					npl = 0;
			}
		
			Node(Type&& value, Node* left = nullptr, Node* right = nullptr)
				:value(move(value)), left(left), right(right)
			{
				if (left && right)
					npl = ::min(left->npl, right->npl) + 1;
				elif(left && !right)
					npl = left->npl + 1;
				elif(right && !left)
					npl = right->npl + 1;
				else
					npl = 0;
			}
		
		};

	private:

		Node*  root;
		size_t size;

	private:

		void Clear(Node* root)
		{
			if (!root) return;
			if (root->left) Clear(root->left);
			if (root->right) Clear(root->right);
			delete root;
		}
		
		Node* MergeTo(Node* smaller, Node* bigger)
		{
			if (!smaller->left)
				smaller->left = bigger;
			else
			{
				smaller->right = Merge(smaller->right, bigger);
				if (smaller->left->npl < smaller->right->npl)
				{
					auto prev = smaller->left;
					smaller->left = smaller->right;
					smaller->right = prev;
				}
				smaller->npl = smaller->right->npl + 1;
			}
			return smaller;
		}

		Node* Merge(Node* lhs, Node* rhs)
		{
			if (!lhs)
				return rhs;
			if (!rhs)
				return lhs;
			if (CompareObject<Type>()(lhs->value, rhs->value))
				return MergeTo(lhs, rhs);
			else
				return MergeTo(rhs, lhs);
		}

		void Check(Node* root)
		{
			auto left = root->left, right = root->right;
			if (!left)
			{
				if (CompareObject<Type>()(left->value, root->value))
					cout << "Invalid Node Detected At Root: " << root << endl;
			}
			elif(!right)
			{
				if (CompareObject<Type>()(right->value, root->value))
					cout << "Invalid Node Detected At Root: " << root << endl;
			}
			else
				return;
			Check(left);
			Check(right);
		}

	public:

		LeftistHeap()
			:root(nullptr), size(0) {  }

		~LeftistHeap()
		{
			Clear(root);
		}

		LeftistHeap(initializer_list<Type>&& il)
		{
			for (auto iter = il.begin(); iter != il.end(); ++iter)
			{
				Insert(*iter);
			}
		}

		void Merge(LeftistHeap& rhs)
		{
			if (&rhs == this || !rhs.root) return;
			if(rhs.root)
				this->size += rhs.size;
			root = Merge(this->root, rhs.root);
			rhs.root = nullptr;
			rhs.size = 0;
		}

		void Insert(const Type& value)
		{
			root = Merge(root, new Node(value));
			++size;
		}

		void Insert(Type&& value)
		{
			root = Merge(new Node(move(value)), root);
			++size;
		}

		Type RemoveRoot()
		{
			assert(root);
			auto ret = move(root->value);
			auto newRoot = Merge(root->left, root->right);
			delete root;
			root = newRoot;
			--size;
			return ret;
		}

		void Check()
		{
			Check(root);
		}

	};

	template<class Type, template<class>class CompareObject = Less>
	class SkewHeap
	{

	};

	template<class Type, template<class>class CompareObject = Less>
	class BinomialQueue
	{
	private:

		struct Node
		{
			Type  value;
			Node* child;
			Node* next;

			Node(const Type& value, Node* child = nullptr, Node* next = nullptr)
				:value(value), child(child), next(next) {  }

			Node(Type&& value, Node* child = nullptr, Node* next = nullptr)
				:value(move(value)), child(child), next(next) {  }

		};
	
	private:
		
		Vector<Node*> roots;

	private:

		Node* MergeRoots(Node* lhs, Node* rhs)
		{
			if (!lhs)
				return rhs;
			if (!rhs)
				return lhs;
			if (CompareObject<Type>()(rhs->value, lhs->value))
			{
				return MergeRoots(rhs, lhs);
			}
			rhs->next = lhs->child;
			lhs->child = rhs;
			return lhs;
		}

		void Print(Node* root)
		{
			if (!root)
				return;
			cout << root->value << " ";
			Print(root->child);
			Print(root->next);

		}

		void Clear(Node*& root)
		{
			if (!root)
				return;
			Clear(root->child);
			Clear(root->next);
			delete root;
			root = nullptr;
		}

	public:

		BinomialQueue() {}

		~BinomialQueue()
		{
			for (auto iter = this->roots.Begin(); iter != this->roots.End(); ++iter)
			{
				Clear(*iter);
			}
		}

		BinomialQueue(initializer_list<Type>&& il)
			:BinomialQueue()
		{
			for (auto iter = il.begin(); iter < il.end(); ++iter)
			{
				Insert(*iter);
			}
		}

		void Clear()
		{
			for (auto iter = this->roots.Begin(); iter != this->roots.End(); ++iter)
			{
				Clear(*iter);
			}
			this->roots.Resize(0);
		}

		size_t Size() const
		{
			return this->roots.Size();
		}

		void Merge(BinomialQueue& rhs)
		{
			if (&rhs == this) return;
			size_t cap = ::max(rhs.Size(), this->Size());
			Node* prev = nullptr;
			for (size_t i = 0; i < cap; ++i)
			{
				auto ret = MergeRoots(this->roots[i], rhs.roots[i]);
				if (!this->roots[i] && !rhs.roots[i])
				{//ret ranks 0
					ret = MergeRoots(ret, prev);
					//ret ranks 0 or i.
					this->roots[i] = ret;
					prev = nullptr;
				}
				elif(!this->roots[i] || !rhs.roots[i])
				{//ret ranks i
					if (prev)
					{
						prev = MergeRoots(ret, prev);
						//prev ranks i + 1;
						this->roots[i] = nullptr;
						
					}
					else
					{
						this->roots[i] = ret;
					}
				}
				else
				{//ret ranks i + 1
					this->roots[i] = prev;
					prev = ret;
				}
				rhs.roots[i] = nullptr;

			}
			if (prev)
			{
				this->roots[cap] = prev;
				this->roots.Resize(cap + 1);
			}
			else
				this->roots.Resize(cap);
		}

		void Insert(const Type& value)
		{
			BinomialQueue<Type, CompareObject> que;
			que.roots.PushBack(new Node(value));
			Merge(que);
		}

		void Insert(Type&& value)
		{
			BinomialQueue<Type, CompareObject> que;
			que.roots.PushBack(new Node(move(value)));
			Merge(que);
		}

		Type Top()
		{
			for (size_t i = 0; i < this->roots.Size(); ++i)
				if (this->roots[i])
				{
					auto ret = this->roots[i]->value;
					for (size_t j = i + 1; j < this->roots.Size(); ++j)
					{
						if (this->roots[j])
							if (CompareObject<Type>()(this->roots[j]->value, ret))
							{
								ret = this->roots[j]->value;
							}
					}
					return ret;
				}
			assert(0);
			return Type();
			
		}

		Type RemoveRoot()
		{
			assert(this->roots.Size());
			for (size_t i = 0; i < this->roots.Size(); ++i)
				if (this->roots[i])
				{
					size_t index = i;
					auto top = this->roots[index]->value;
					for (size_t j = index + 1; j < this->roots.Size(); ++j)
					{
						if (this->roots[j])
							if (CompareObject<Type>()(this->roots[j]->value, top))
							{
								top = this->roots[j]->value;
								index = j;
							}
					}

					Node* child = this->roots[index]->child;
					Node* next = this->roots[index]->next;
					this->roots[index] = nullptr;
					size_t rank = !index ? index : index - 1;
					BinomialQueue<Type, CompareObject> b1;
					BinomialQueue<Type, CompareObject> b2;
					b1.roots[rank] = child;
					b2.roots[rank] = next;

					Merge(b1);
					Merge(b2);

					return top;
				}
			assert(0);
			return Type();
		}

		void Print()
		{
			for (auto iter = this->roots.Begin(); iter != this->roots.End(); ++iter)
			{
				Print(*iter);
				cout << endl;
			}
		}
	};

	template<class Type, 
		template<class> class CompareObject = Greater, 
		template<class, template<class> class> class Basement = BinaryHeap>
	class PriorityQueue
	{
	private:

		Basement<Type, CompareObject> data;

	};



	//Operation Methods

	//For LinearLists Or Types With Iterators
	template<template<class> class LinearList, class Type>
	inline auto operator<<(ostream& os, const LinearList<Type>& list)->
		decltype(list.Begin(), list.End(), declval<ostream&>() << declval<Type>(), os)
	{
		for(auto iter = list.Begin(); iter != list.End(); ++iter)
			os << *iter << " ";
		return os;
	}

	//For Trees
	//In order to control the element length that printed on screen,
	//you can define the macro MAXELEMENTLENGTH to control the probable length in your btree,
	//this macro will be defined as 3 if you ignore it,
	//but this value should better be a odd number 
	template<template<class> class Tree, class Type>
	inline auto operator<<(ostream& os, const Tree<Type>& tree)->
		decltype(tree.root, declval<ostream&>() << declval<Type>(), os)
	{
		/**
		 *  Overall, we need to know which level the current element belongs to,
		 *	so that we can know how to set the align.
		 *	first, let the height of the tree be variable h,
		 *  absolutely, the nth level has 2^(n - 1) elements(containing null),
		 *	we have to know the max length of each element, 
		 *  now let it be variable l,
		 */
		//Stored the nodes in every level, containing nullptr
		if (!tree.size) return os << "NOE";
#ifndef MAXELEMENTLENGTH
#define MAXELEMENTLENGTH 3
#endif
		int l = MAXELEMENTLENGTH;
		map<int, vector<typename Tree<Type>::Node*>> nodes;

		//Initializing the node map
		{
		Queue<typename Tree<Type>::Node*> queue;
				queue.Enqueue(tree.root);
				int nodeCnt   = 0;
				int maxNodes  = 1;
				int currLevel = 0;
				int nullCnt   = 0;
				while (1)
				{
					auto curr = queue.Dequeue();
					nodes[currLevel].push_back(curr);
					if (curr)
					{
						queue.Enqueue(curr->left);
						queue.Enqueue(curr->right);
					}
					else
					{
						queue.Enqueue(nullptr);
						queue.Enqueue(nullptr);
						nullCnt += 2;
					}
		
					++nodeCnt;
					if (nullCnt == maxNodes * 2)
					{
						break;
					}
					if (nodeCnt == maxNodes)
					{
						++currLevel;
						maxNodes *= 2;
						nodeCnt = 0;
						nullCnt = 0;
					}
				}
		}

		int h = nodes.size();
		//before doing this, we'd better keep l a odd number;
		int metaL = (l + 1) / 2;

		//Stored the number of space bar of every level should have
		vector<int> SpaceBarMap(h, 1);
		//Stored the length of alignment of every level
		vector<int> AlginmentMap(h, 0);
		//Initializing the two maps
		for (int i = h - 2; i >= 0; --i)
		{
			AlginmentMap[i] = 2 * AlginmentMap[i + 1] + 1;
			SpaceBarMap[i] = (SpaceBarMap[i + 1]) * 2 + l;
			AlginmentMap[i + 1] *= metaL;
		}
		AlginmentMap[0] *= metaL;


		//Finally print the whole tree with the help of these maps
		for (int level = 0; level < h; ++ level)
		{
			for (int al = 0; al < AlginmentMap[level]; ++al)
			{
				os << " ";
			}
			for (int ele = 0; ele < nodes[level].size(); ++ele)
			{
				if (nodes[level][ele])
				{//if I can know the length of printing element,
				 //I can justify the length to make every element the same length.
					os << nodes[level][ele]->value;
					auto lEle = GetLenOfElement(nodes[level][ele]->value);
					if (lEle < l || lEle != 0)
					{
						for (int i = 0; i < l - lEle; ++i)
						{
							os << " ";
						}
					}
				}
				else
				{
					os << "NOE";
				}
				for (int sp = 0; sp < SpaceBarMap[level]; ++sp)
				{
					os << " ";
				}
			}
			os << endl;
		}



		return os;
	}

	template<template<class, template<class> class> class Tree, class Type, template<class> class CompareObject>
	inline auto operator<<(ostream& os, const Tree<Type, CompareObject>& tree)->
		decltype(tree.root, declval<ostream&>() << declval<Type>(), os)
	{
		if (!tree.size) return os << "NOE";
#ifndef MAXELEMENTLENGTH
#define MAXELEMENTLENGTH 3
#endif
		int l = MAXELEMENTLENGTH;
		map<int, vector<typename Tree<Type, CompareObject>::Node*>> nodes;

		//Initializing the node map
		{
			Queue<typename Tree<Type, CompareObject>::Node*> queue;
			queue.Enqueue(tree.root);
			int nodeCnt = 0;
			int maxNodes = 1;
			int currLevel = 0;
			int nullCnt = 0;
			while (1)
			{
				auto curr = queue.Dequeue();
				nodes[currLevel].push_back(curr);
				if (curr)
				{
					queue.Enqueue(curr->left);
					queue.Enqueue(curr->right);
				}
				else
				{
					queue.Enqueue(nullptr);
					queue.Enqueue(nullptr);
					nullCnt += 2;
				}

				++nodeCnt;
				if (nullCnt == maxNodes * 2)
				{
					break;
				}
				if (nodeCnt == maxNodes)
				{
					++currLevel;
					maxNodes *= 2;
					nodeCnt = 0;
					nullCnt = 0;
				}
			}
		}

		int h = nodes.size();
		//before doing this, we'd better keep l a odd number;
		int metaL = (l + 1) / 2;

		//Stored the number of space bar of every level should have
		vector<int> SpaceBarMap(h, 1);
		//Stored the length of alignment of every level
		vector<int> AlginmentMap(h, 0);
		//Initializing the two maps
		for (int i = h - 2; i >= 0; --i)
		{
			AlginmentMap[i] = 2 * AlginmentMap[i + 1] + 1;
			SpaceBarMap[i] = (SpaceBarMap[i + 1]) * 2 + l;
			AlginmentMap[i + 1] *= metaL;
		}
		AlginmentMap[0] *= metaL;


		//Finally print the whole tree with the help of these maps
		for (int level = 0; level < h; ++level)
		{
			for (int al = 0; al < AlginmentMap[level]; ++al)
			{
				os << " ";
			}
			for (int ele = 0; ele < nodes[level].size(); ++ele)
			{
				if (nodes[level][ele])
				{//if I can know the length of printing element,
				 //I can justify the length to make every element the same length.
					os << nodes[level][ele]->value;
					auto lEle = GetLenOfElement(nodes[level][ele]->value);
					if (lEle < l || lEle != 0)
					{
						for (int i = 0; i < l - lEle; ++i)
						{
							os << " ";
						}
					}
				}
				else
				{
					os << "NOE";
				}
				for (int sp = 0; sp < SpaceBarMap[level]; ++sp)
				{
					os << " ";
				}
			}
			os << endl;
		}



		return os;
	}



	template<class Iterator, class Type, class = decltype(declval<Type>() == declval<Type>())>
	inline Iterator Find(Iterator begin, Iterator end, const Type& target)
	{
		while (begin != end)
		{
			if (*begin == target) return begin;
			++begin;
		}
		return begin;
	}

	template<class Iterator, class Type, class = decltype(declval<Type>() == declval<Type>())>
	inline Iterator Find(Iterator begin, Iterator end, Type&& target)
	{
		while (begin != end)
		{
			if (*begin == target) return begin;
			++begin;
		}
		return begin;
	}

}
#endif // Structures_h__