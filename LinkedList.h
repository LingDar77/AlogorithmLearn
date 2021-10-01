#pragma once

#ifndef LinkedList_h__
#define LinkedList_h__

namespace Struct
{

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

			size_t operator-(ConstIterator rhs)
			{
				size_t ret = 0;
				while (rhs != *this)
				{
					++rhs;
					++ret;
				}
				return ret;
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

			size_t operator-(Iterator rhs)
			{
				size_t ret = 0;
				while (rhs != *this)
				{
					++rhs;
					++ret;
				}
				return ret;
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

}


#endif // LinkedList_h__