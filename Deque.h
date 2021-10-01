#pragma once
#ifndef Deque_h__
#define Deque_h__

#include "LinkedList.h"


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


#endif // Deque_h__