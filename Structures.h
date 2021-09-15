#pragma once
#include <initializer_list>
#include "Difinitions.h"
#include <assert.h>
#include <vector>
#include <string>
#include <map>

namespace Struct
{
	using namespace std;

	/**
	 * Basic structures
	 */

	//a universal type that contains numbers and support unlimited calculation.
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

		Type*  content  = nullptr	 ;
		size_t size     = 0			 ;
		size_t capacity = MINCAPACITY;

	public:

		typedef Type*		Iterator;
		typedef const Type* ConstIterator;

		explicit Vector(size_t size = MINCAPACITY)
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
			return *(content + index);
		}

		Type& operator[](size_t index)
		{
			return *(content + index);
		}

		void Erase(Iterator It)
		{
			auto end = End() - 1;
			while (It != end)
			{
				swap(*It, *(It + 1));
				++It;
			}
			--size;
		}

		void Erase(size_t index)
		{
			for (int i = index; i < Size() - 1; ++i)
			{
				swap(content[i], content[i + 1]);
			}
			--size;
		}

		void Resize(size_t newSize)
		{
			if (newSize > Capacity())
				Reserve(newSize);
			size = newSize;
			
		}

		void Reserve(size_t newCapacity)
		{
			if (newCapacity <= Capacity())
				return;
			size_t newCap = (newCapacity / Capacity() + 1) * Capacity();
			auto newContent = new Type[newCap + 1];
			for (int i = 0; i != Size(); ++i)
			{
				swap(newContent[i], content[i]);
			}
			delete[] content;
			content = newContent;
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
			size_t sz = Size() - 1;
			Resize(sz);
			return *(content + sz);
		}

		Type PopFront()
		{
			size_t sz = Size() - 1;
			Resize(sz);
			for (int i = 0; i != sz; ++i)
			{
				swap(*(content + i), *(content + i + 1));
			}
			return *(content + sz);
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

		Iterator Erase(Iterator it)
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

		void Erase(Iterator it)
		{
			auto next = it + 1;
			*it = *next;
			it.curr->next = next.curr->next;
			delete next.curr;
			--size;
			
		}

		void Erase(size_t index)
		{
			Erase(Begin() + index);
		}

};

	/**
	 * Collections based on basic structures
	 */

	template<class Type, template<class> class Container = ForwardList>
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

	template<class Type, template<class> class Container = ForwardList>
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
		template<class Type> friend ostream& operator<<(ostream& os, const BinaryTree<Type>& tree);
	private:

		struct TreeNode
		{

			Type	  value;
			TreeNode* left;
			TreeNode* right;

			explicit TreeNode(Type value = NULL, TreeNode* left = nullptr, TreeNode* right = nullptr) noexcept
				:value(value), left(left), right(right) { }

		};

	private:

		static void DoClear(TreeNode* root)
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
		static TreeNode* CpyTree(TreeNode* destinationRoot, TreeNode* resourceRoot)
		{
			destinationRoot->value = resourceRoot->value;
			if(resourceRoot->left)
			{
				destinationRoot->left = CpyTree(new TreeNode(), resourceRoot->left);
			}
			if (resourceRoot->right)
			{
				destinationRoot->right = CpyTree(new TreeNode(), resourceRoot->right);
			}
				return destinationRoot;
		}
		
		static TreeNode* DeepthFirstSearch(TreeNode* root, const Type& value)
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

		static TreeNode* BreadthFirstSearch(TreeNode* root, const Type& value)
		{
			assert(root);
			Queue<TreeNode*> queue;
			queue.Enqueue(root);
			while (!queue.IsEmpty())
			{
				TreeNode* curr = queue.Dequeue();
				if (curr->left) queue.Enqueue(curr->left);
				if (curr->right) queue.Enqueue(curr->right);
				if (curr->value == value) return curr;
			}
			return nullptr;

		}

		//make sure that the result node is not complete
		static TreeNode* FindHighestLeaf(TreeNode* root)
		{
			Queue<TreeNode*> queue;
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

		TreeNode* root;
		size_t	  size;

	public:

		explicit BinaryTree() noexcept
			:root(new TreeNode()), size(0)
		{ }

		~BinaryTree()
		{
			DoClear(root);
		}

		BinaryTree(const initializer_list<Type>& data)
			:BinaryTree()
		{
			assert(data.size());
			Queue<TreeNode*> roots;
			roots.Enqueue(root);
			root->value = *(data.begin());
			++size;
			for (int i = 1; i < data.size(); ++i)
			{
				TreeNode* front = roots.Front();
				if (front->left && front->right)
					roots.Dequeue();
				front = roots.Front();

				TreeNode* node = new TreeNode(*(data.begin() + i));
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

		const TreeNode* DFS(Type value) const
		{
			return DeepthFirstSearch(root, value);
		}

		const TreeNode* BFS(Type value) const
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
				node->left = new TreeNode(value);
			}
			else
			{
				node->right = new TreeNode(value);
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
				node->left = new TreeNode(move(value));
			}
			else
			{
				node->right = new TreeNode(move(value));
			}
			++size;
		}

		void Remove(const Type& value)
		{

		}

		void Remove(Type&& value)
		{
			
			auto ret = WidthFirstSearch(root, value);
			if (ret)
			{
				--size;

			}
			
			
		}


	};



	//Operation Methods

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
	ostream& operator<<(ostream& os, const LinkedList<Type>& list)
	{
		auto it = list.Begin();
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
	ostream& operator<<(ostream& os, const ForwardList<Type>& list)
	{
		if (list.IsEmpty()) return os;
		auto It = list.Begin();
		auto Ed = list.End();
		os << *It;
		++It;
		while (It != Ed)
		{
			os << "," << *It;
			++It;
		}
		return os;
	}

	//In order to control the element length that printed on screen,
	//you can define the macro MAXELEMENTLENGTH to control the probable length in your btree,
	//this macro will be defined as 3 if you ignore it,
	//but this value better be a odd number 
	template<class Type>
	ostream& operator<<(ostream& os, const BinaryTree<Type>& tree)
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

#ifndef MAXELEMENTLENGTH
#define MAXELEMENTLENGTH 3
#endif
		int l = MAXELEMENTLENGTH;
		map<int, vector<typename BinaryTree<Type>::TreeNode*>> nodes;

		//Initializing the node map
		{
		Queue<typename BinaryTree<Type>::TreeNode*> queue;
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

	template<class Iterator, class Type>
	Iterator Find(Iterator begin, Iterator end, const Type& target)
	{
		while (begin != end)
		{
			if (*begin == target) return begin;
			++begin;
		}
		return begin;
	}

	template<class Iterator, class Type>
	Iterator Find(Iterator begin, Iterator end, Type&& target)
	{
		while (begin != end)
		{
			if (*begin == target) return begin;
			++begin;
		}
		return begin;
	}

}