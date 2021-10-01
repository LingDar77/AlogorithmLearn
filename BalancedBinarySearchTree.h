#pragma once
#ifndef BalancedBinarySearchTree_h__
#define BalancedBinarySearchTree_h__
#include "Helper.h"

namespace Struct
{
	template<class Type>
	class BalancedBinarySearchTree
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
			int   height;

			Node(const Type& value, int height = 0, Node* left = nullptr, Node* right = nullptr)
				:value(value), height(height), left(left), right(right) {  }

			Node(Type&& value, int height = 0, Node* left = nullptr, Node* right = nullptr)
				:value(move(value)), height(height), left(left), right(right) {  }

		};

	private:

		Node* root;
		size_t size;

	private:

		int height(Node* node) const
		{
			return node ? node->height : -1;
		}

		//Imbalance happened by the insertion at the right of the root node's right child.
		//That means the height of right child - that of left child > 1
		void RotateLeft(Node*& root)
		{
			if (!root) return;
			Node* prev = root;
			root = prev->right;
			prev->right = root->left;
			root->left = prev;
			prev->height = max(height(prev->left), height(prev->right)) + 1;
			root->height = max(height(root->left), height(root->right)) + 1;
		}

		//Imbalance happened by the insertion at the left of the root node's left child.
		//That means the height of left child - that of right child > 1
		void RotateRight(Node*& root)
		{
			if (!root) return;
			Node* prev = root;
			root = prev->left;
			prev->left = root->right;
			root->right = prev;
			prev->height = max(height(prev->left), height(prev->right)) + 1;
			root->height = max(height(root->left), height(root->right)) + 1;

		}

		void RotateRightLeft(Node*& root)
		{
			if (!root) return;
			RotateRight(root->left);
			RotateLeft(root);

		}

		void RotateLeftRight(Node*& root)
		{
			if (!root) return;
			RotateLeft(root->left);
			RotateRight(root);

		}

		void Balance(Node*& root)
		{
			if (!root) return;
			if (height(root->left) - height(root->right) > 1)
				if (height(root->left->left) >= height(root->left->right))
					RotateRight(root);
				else
					RotateLeftRight(root);
			if (height(root->right) - height(root->left) > 1)
				if (height(root->right->right) >= height(root->right->left))
					RotateLeft(root);
				else
					RotateRightLeft(root); \

					root->height = max(height(root->left), height(root->right)) + 1;
		}

		void Insert(const Type& value, Node*& root)
		{
			if (!root) root = new Node(value);
			if (root->value < value)
				Insert(value, root->right);
			elif(value < root->value)
				Insert(value, root->left);

			Balance(root);
		}

		void Insert(Type&& value, Node*& root)
		{
			if (!root) root = new Node(move(value));
			if (root->value < value)
				Insert(move(value), root->right);
			elif(value < root->value)
				Insert(move(value), root->left);

			Balance(root);
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

			Balance(root);
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

		}

		bool Contains(const Type& value)
		{

			return false;
		}

	public:
		//Unable to construct if the given Type can not be compared.
		template<class = decltype(declval<Type>() < declval<Type>(), declval<Type>() != declval<Type>(), true)>
			BalancedBinarySearchTree()
				:root(nullptr), size(0) {   }

			BalancedBinarySearchTree(initializer_list<Type>&& il)
				:BalancedBinarySearchTree()
			{
				for (auto iter = il.begin(); iter < il.end(); ++iter)
				{
					Insert(*iter);
				}
			}

			~BalancedBinarySearchTree()
			{
				Clear(root);
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
				Remove(move(value), root);
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

	};

	template<class Type>
	using AVLTree = BalancedBinarySearchTree<Type>;
}

#endif // BalancedBinarySearchTree_h__