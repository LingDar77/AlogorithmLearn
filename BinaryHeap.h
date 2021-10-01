#pragma once
#ifndef BinaryHeap_h__
#define BinaryHeap_h__

#include "Helper.h"
namespace Struct
{
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

		Type&& RemoveRoot()
		{
			auto size = data.Size();
			assert(size, "Fault: Can Not Remove A Root From An Empty Heap!");

			swap(data[0], data[size - 1]);
			int curr = 0, child = 1;
			while (child < size - 1)
			{
				if (child != size - 2 && CompareObject<Type>()(data[child + 1], data[child]))
					++child;
				if (CompareObject<Type>()(data[child], data[curr]))
					swap(data[child], data[curr]);
				else
					break;
				curr = child;
				child = curr * 2 + 1;
			}
			data.Resize(size - 1);
			return move(data[size - 1]);
		}

		Type Top()
		{
			return data[0];
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
			Node* left;
			Node* right;
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

		Node* root;
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
			if (rhs.root)
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

		Type&& RemoveRoot()
		{
			assert(root);
			auto ret = move(root->value);
			auto newRoot = Merge(root->left, root->right);
			delete root;
			root = newRoot;
			--size;
			return move(ret);
		}

		Type Top()
		{
			return root->value;
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
}

#endif // BinaryHeap_h__