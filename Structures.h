#pragma once
#include <initializer_list>
#include "Difinitions.h"
#include <assert.h>


namespace Struct
{
	using namespace std;

	/**
	 * Basic structures
	 */

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
				:value(NULL), prev(nullptr), next(nullptr){ }

			Node(const Type& value, Node* prev = nullptr, Node* next = nullptr)
				:value(value), prev(prev), next(next)
			{
				if(next)
					next->prev = this;
				if (prev)
					prev->next = this;
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

			explicit ConstIterator(Node* p) noexcept
				:cur(p){ }

		public:

			ConstIterator() noexcept
				:cur(nullptr){ }

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
				:ConstIterator(p){ }
		
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
		
	 	explicit LinkedList(){ }

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
			size_t back  = size - index;
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

			Type* operator*()
			{
				return this->Retrieve();
			}

			const Type* operator*() const
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
			curr->Node = new Node(value, head);
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
			auto beg = Begin();
			while (beg + 1 != it)
			{
				++beg;
			}
			beg.curr->next = new Node(value, it.curr);
			++size;
		}

		void Insert(Iterator it, Type&& value)
		{
			auto beg = Begin();
			while (beg + 1 != it)
			{
				++beg;
			}
			beg.curr->next = new Node(move(value), it.curr);
			++size;
		}

		void Insert(size_t index, const Type& value)
		{
			auto beg = head;
			for (int i = 0; i < index; ++i)
			{
				beg = beg->next;
			}
			beg->next = new Node(value, beg->next);
			++size;
		}

		void Insert(size_t index, Type&& value)
		{
			auto beg = head;
			for (int i = 0; i < index; ++i)
			{
				beg = beg->next;
			}
			beg->next = new Node(move(value), beg->next);
			++size;
		}

		void Erase(Iterator it)
		{
			auto beg = Begin();
			while (beg + 1 != it)
			{
				++beg;
			}
			--size;
			beg.curr->next = it.curr->next;
			delete it.curr;
		}

		void Erase(size_t index)
		{
			auto beg = head;
			for (int i = 0; i < index; ++i)
			{
				beg = beg->next;
			}
			--size;
			auto n = beg->next;
			beg->next = n->next;
			delete n;
		}

};

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

	/**
	 * Collections based on basic structures
	 */

	template<class Type, template<class Type> class Container = ForwardList>
	class Stack
	{

	};

}