#pragma once
#ifndef BinarySearchTree_h__
#define BinarySearchTree_h__

namespace Struct
{
	template<class Type>
	class BinarySearchTree
	{
		template<template<class> class Tree, class Type>
		friend auto operator<<(ostream& os, const Tree<Type>& tree)->
			decltype(tree.root, declval<ostream&>() << declval<Type>(), os);

	private:

		struct Node
		{
			Type  value;
			Node* left;
			Node* right;

			Node() = default;

			Node(const Type& value, Node* left = nullptr, Node* right = nullptr)
				:value(value), left(left), right(right) {  }

			Node(Type&& value, Node* left = nullptr, Node* right = nullptr)
				:value(move(value)), left(left), right(right) {  }

		};

	private:

		Node* root;
		size_t size;

	private:

		bool Contains(Type&& value, Node* root) const
		{
			if (!root) return false;
			while (root)
			{
				if (root->value < value)
					root = root->right;
				else if (value > root->value)
					root = root->left;
				else
					return true;
			}
			return false;
		}

		bool Contains(const Type& value, Node* root) const
		{
			if (!root) return false;
			while (root)
			{
				if (root->value < value)
					root = root->right;
				else if (value > root->value)
					root = root->left;
				else
					return true;
			}
			return false;
		}

		Node* FindMax(Node* root) const
		{
			while (root)
			{
				if (!root->right)
					break;
				root = root->right;
			}
			return root;
		}

		Node* FindMin(Node* root) const
		{
			while (root)
			{
				if (!root->left)
					break;
				root = root->left;
			}
			return root;
		}

		void Insert(const Type& value, Node*& root)
		{
			if (!root) root = new Node(value);
			if (root->value < value)
				Insert(value, root->right);
			else if (value < root->value)
				Insert(value, root->left);
			else
				return;
		}

		void Insert(Type&& value, Node*& root)
		{
			if (!root) root = new Node(move(value));
			if (root->value < value)
				Insert(move(value), root->right);
			else if (value < root->value)
				Insert(move(value), root->left);
			else
				return;
		}

		void Remove(const Type& value, Node*& root)
		{
			if (!root) return;
			if (root->value < value)
				Remove(value, root->right);
			else if (value < root->value)
				Remove(value, root->left);
			else if (root->left && root->right)
			{
				root->value = FindMin(root->right)->value;
				Remove(root->value, root->right);
			}
			else
			{
				Node* prev = root;
				root = root->left ? root->left : root->right;
				delete prev;
			}
		}

		void Clear(Node* root)
		{
			if (!root) return;
			if (root->left)
				Clear(root->left);
			if (root->right)
				Clear(root->right);
			delete root;
		}

		void Clone(Node* toRoot, Node* fromRoot)
		{
			if (!fromRoot || !toRoot)
				return;
			toRoot->value = fromRoot->value;
			if (fromRoot->left)
			{
				toRoot->left = new Node();
				Clone(toRoot->left, fromRoot->left);
			}
			if (fromRoot->right)
			{
				toRoot->right = new Node();
				Clone(toRoot->right, fromRoot->right);
			}
		}

	public:

		//Unable to construct if the given Type can not be compared.
		template<class = decltype(declval<Type>() < declval<Type>(), declval<Type>() != declval<Type>(), true)>
			BinarySearchTree()
				:root(nullptr), size(0) {  }

			BinarySearchTree(initializer_list<Type>&& il)
				:BinarySearchTree()
			{
				for (auto iter = il.begin(); iter != il.end(); ++iter)
				{
					Insert(*iter, root);
					++size;
				}
			}

			BinarySearchTree(const BinarySearchTree& rhs)
				:BinarySearchTree()
			{
				root = new Node();
				Clone(root, rhs.root);
				size = rhs.size;
			}

			BinarySearchTree(BinarySearchTree&& rhs)
				:BinarySearchTree()
			{
				swap(rhs.root, root);
				swap(rhs.size, size);
			}

			~BinarySearchTree()
			{
				Clear(root);
			}

			BinarySearchTree& operator=(const BinarySearchTree& rhs)
			{
				Clear(root);
				root = new Node();
				Clone(root, rhs.root);
				size = rhs.size;
				return *this;
			}

			BinarySearchTree& operator=(BinarySearchTree&& rhs)
			{
				swap(rhs.root, root);
				swap(rhs.size, size);
				return *this;
			}

			bool IsEmpty() const
			{
				return !size;
			}

			void Clear()
			{
				Clear(root);
				root = nullptr;
			}

			void Remove(const Type& value)
			{
				Remove(value, root);
				--size;
			}

			void Remove(Type&& value)
			{
				Type v = move(value);
				Remove(v, root);
				--size;
			}

			void Insert(const Type& value)
			{
				Insert(value, root);
				++size;
			}

			void Insert(Type&& value)
			{
				Insert(move(value), root);
				++size;
			}

			bool Contains(Type&& value) const
			{
				return Contains(move(value), root);
			}

			bool Contains(const Type& value) const
			{
				return Contains(value, root);
			}

	};
}

#endif // BinarySearchTree_h__
