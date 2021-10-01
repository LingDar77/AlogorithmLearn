#pragma once
#ifndef ForwardList_h__
#define ForwardList_h__

namespace Struct
{
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
			Node* curr;

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
				auto h = this->list->head;
				while (h->next != this->curr)
				{
					h = h->next;
				}
				this->curr = h;
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

		Node* head;
		Node* curr;
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
			for (int i = 0; i < sz; ++i)
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
			while (it != ed)
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
}

#endif // ForwardList_h__