#pragma once
#ifndef BinaryTree_h__
#define BinaryTree_h__

namespace Struct
{
	template<class Type>
	class BinaryTree
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
			explicit Node(Type value, Node* left = nullptr, Node* right = nullptr) noexcept
				:value(value), left(left), right(right) { }

		};

	private:

		static void DoClear(Node* root)
		{
			if (root)
			{
				if (root->left)
					DoClear(root->left);
				if (root->right)
					DoClear(root->right);
				delete root;
			}
		}

		//Copy a tree from a root node to another root node
		static Node* CpyTree(Node* destinationRoot, Node* resourceRoot)
		{
			destinationRoot->value = resourceRoot->value;
			if (resourceRoot->left)
			{
				destinationRoot->left = CpyTree(new Node(), resourceRoot->left);
			}
			if (resourceRoot->right)
			{
				destinationRoot->right = CpyTree(new Node(), resourceRoot->right);
			}
			return destinationRoot;
		}

		static Node* DeepthFirstSearch(Node* root, const Type& value)
		{
			if (!root) return nullptr;
			if (root->value == value) return root;

			if (root->left)
			{
				auto leftRet = DeepthFirstSearch(root->left, value);
				if (leftRet) return leftRet;
			}
			if (root->right)
			{
				auto rightRet = DeepthFirstSearch(root->right, value);
				if (rightRet) return rightRet;
			}
			return nullptr;

		}

		static Node* BreadthFirstSearch(Node* root, const Type& value)
		{
			assert(root);
			Queue<Node*> queue;
			queue.Enqueue(root);
			while (!queue.IsEmpty())
			{
				Node* curr = queue.Dequeue();
				if (curr->left) queue.Enqueue(curr->left);
				if (curr->right) queue.Enqueue(curr->right);
				if (curr->value == value) return curr;
			}
			return nullptr;

		}

		//make sure that the result node is not complete
		static Node* FindHighestLeaf(Node* root)
		{
			Queue<Node*> queue;
			queue.Enqueue(root);
			while (!queue.IsEmpty())
			{
				auto node = queue.Dequeue();
				if (!node->left || !node->right) return node;
				else
				{
					if (node->left)
						queue.Enqueue(node->left);
					if (node->right)
						queue.Enqueue(node->right);
				}
			}
			return nullptr;
		}


	private:

		Node* root;
		size_t size;

	public:

		explicit BinaryTree() noexcept
			:root(new Node()), size(0)
		{ }

		~BinaryTree()
		{
			DoClear(root);
		}

		BinaryTree(const initializer_list<Type>& data)
			:BinaryTree()
		{
			assert(data.size());
			Queue<Node*> roots;
			roots.Enqueue(root);
			root->value = *(data.begin());
			++size;
			for (int i = 1; i < data.size(); ++i)
			{
				Node* front = roots.Front();
				if (front->left && front->right)
					roots.Dequeue();
				front = roots.Front();

				Node* node = new Node(*(data.begin() + i));
				roots.Enqueue(node);
				if (!front->left)
				{
					front->left = node;
				}
				else if (!front->right)
				{
					front->right = node;
				}
			}
			size = data.size();


		}

		BinaryTree(const BinaryTree& rhs)
			:BinaryTree()
		{
			size = rhs.size;
			CpyTree(root, rhs.root);
		}

		BinaryTree(BinaryTree&& rhs)
			:BinaryTree()
		{
			swap(rhs.root, root);
			swap(rhs.size, size);
		}

		void Clear()
		{
			DoClear(root->left);
			DoClear(root->right);
			size = 0;
		}

		size_t Size() const noexcept
		{
			return size;
		}

		const Node* DFS(Type value) const
		{
			return DeepthFirstSearch(root, value);
		}

		const Node* BFS(Type value) const
		{
			return BreadthFirstSearch(root, value);
		}

		void Add(const Type& value)
		{
			if (!size)
			{
				root->value = value;
				++size;
				return;
			}
			auto node = FindHighestLeaf(root);
			if (!node->left)
			{
				node->left = new Node(value);
			}
			else
			{
				node->right = new Node(value);
			}
			++size;
		}

		void Add(Type&& value)
		{
			if (!size)
			{
				root->value = move(value);
				++size;
				return;
			}
			auto node = FindHighestLeaf(root);
			if (!node->left)
			{
				node->left = new Node(move(value));
			}
			else
			{
				node->right = new Node(move(value));
			}
			++size;
		}

		void Remove(const Type& value)
		{
			Queue<Node*> nodes;
			Node* mark = nullptr;
			Node* markParent = nullptr;//used when remove the root node
			Node* targetParent = nullptr;
			Node* target = nullptr;
			nodes.Enqueue(root);
			while (!nodes.IsEmpty())
			{
				auto curr = nodes.Dequeue();
				if (curr->left && curr->left->value != value && !curr->left->left && !curr->left->right)
					markParent = curr, mark = curr->left;
				if (curr->right && curr->right->value != value && !curr->right->left && !curr->right->right)
					markParent = curr, mark = curr->right;
				if (curr->left)
					nodes.Enqueue(curr->left);
				if (curr->right)
					nodes.Enqueue(curr->right);
				if (curr->left && curr->left->value == value)
					target = curr->left, targetParent = curr;
				if (curr->right && curr->right->value == value)
					target = curr->right, targetParent = curr;
				if (targetParent && mark) break;
			}
			if (mark && target)
			{
				mark->left = target->left;
				mark->right = target->right;
				if (target == targetParent->left)
					targetParent->left = nullptr;
				else
					targetParent->right = nullptr;
				delete target;
			}
			else if (mark && root->value == value)
			{
				mark->left = root->left;
				mark->right = root->right;
				delete root;
				root = mark;
				if (mark == markParent->left)
					markParent->left = nullptr;
				else
					markParent->right = nullptr;
				//root is the element to remove
			}
			else if (size != 1)
			{
				//do not find such an element, do nothing.
				return;
			}
			--size;
		}

		void Remove(Type&& value)
		{
			Type v = move(value);
			Remove(v);
		}

		Node* Contains(const Type& value)
		{
			Queue<Node*> nodes;
			nodes.Enqueue(root);
			while (!nodes.IsEmpty())
			{
				auto curr = nodes.Dequeue();
				if (curr->left)
					nodes.Enqueue(curr->left);
				if (curr->right)
					nodes.Enqueue(curr->right);
				if (curr->value == value) return curr;
			}
			return nullptr;
		}

		Node* Contains(Type&& value)
		{
			Type v = move(value);
			return Contains(v);
		}

		const Node* Contains(const Type& value) const
		{
			Queue<Node*> nodes;
			nodes.Enqueue(root);
			while (!nodes.IsEmpty())
			{
				auto curr = nodes.Dequeue();
				if (curr->left)
					nodes.Enqueue(curr->left);
				if (curr->right)
					nodes.Enqueue(curr->right);
				if (curr->value == value) return curr;
			}
			return nullptr;
		}

		const Node* Contains(Type&& value) const
		{
			Type v = move(value);
			return Contains(v);
		}
	};
}

#endif // BinaryTree_h__