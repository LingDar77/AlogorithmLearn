#pragma once
#ifndef Queue_h__
#define Queue_h__
#include "ForwardList.h"

namespace Struct
{
	template<class Type, 
		template<class> class Container = ForwardList>
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
}

#endif // Queue_h__