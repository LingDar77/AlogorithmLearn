#pragma once
#ifndef CachedForwardList_h__
#define CachedForwardList_h__

#include <initializer_list>
#include "Helper.h"

namespace Struct
{
	using namespace std;
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
				: Node(next), Cache(cache) {  }
		};

		class ConstIterator
		{

			friend class CachedForwardList<Type>;

		private:

			Node* curr;
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
		Node* tail;
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

		Node* AllocNode(const Type& value, Node* next = nullptr)
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
			CachedForwardList & operator=(Container<OtherType>& rhs)
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
			if (size != 1)
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
			iter.curr->next = AllocNode(value, iter.curr->next);
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
}

#endif // CachedForwardList_h__