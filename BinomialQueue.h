#pragma once
#ifndef BinomialQueue_h__
#define BinomialQueue_h__

#include "Helper.h"
namespace Struct
{
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
			rhs.Clear();
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

		Type&& RemoveRoot()
		{
			assert(this->roots.Size());
			for (size_t i = 0; i < this->roots.Size(); ++i)
				if (this->roots[i])
				{
					size_t index = i;
					auto top = this->roots[index];
					for (size_t j = index + 1; j < this->roots.Size(); ++j)
					{
						if (this->roots[j])
							if (CompareObject<Type>()(this->roots[j]->value, top->value))
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

					return move(top->value);
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

}

#endif // BinomialQueue_h__