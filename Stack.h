#pragma once
#ifndef Stack_h__
#define Stack_h__

#include "CachedForwardList.h"

namespace Struct
{
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
			for (int i = 0; i < data.size(); ++i)
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
}

#endif // Stack_h__