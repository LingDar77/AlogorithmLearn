#pragma once
#ifndef ThreadedTree_h__
#define ThreadedTree_h__

namespace Struct
{
	template<class Type>
	class ThreadedBinaryTree
	{
	private:

		struct Node
		{
			Type  value;
			Node* left;
			bool  lChild;
			Node* right;
			bool  rChild;

			Node(const Type& value, Node* left = nullptr, Node* right = nullptr, bool lChild = 1, bool rChild = 1)
				:value(value), left(left), right(right), lChild(lChild), rChild(rChild) {  }

			Node(Type&& value, Node* left = nullptr, Node* right = nullptr, bool lChild = 1, bool rChild = 1)
				:value(value), left(left), right(right), lChild(lChild), rChild(rChild) {  }

		};

	private:

		Node* head;
		Node* root;
		size_t size;

	private:

		void Insert(const Type& value, Node*& root)
		{
			if (!size)
			{
				this->root = new Node(value, head, head, 0, 0);
				head->right = this->root;
				head->left = this->root;
				++size;
			}
			elif(!root)
			{
				root = new Node(value);
				++size;
			}
			elif(value < root->value)
			{
				if (!root->lChild)
				{
					Node* prev = root->left;
					root->left = nullptr;
					Insert(value, root->left);
					root->lChild = 1;
					root->left->left = prev;
					root->left->lChild = 0;
					root->left->right = root;
					root->left->rChild = 0;
					if (prev == head)
					{
						head->right = root->left;
					}
				}
				else
				{
					Insert(value, root->left);
					root->lChild = 1;
				}
			}
			elif(root->value < value)
			{
				if (!root->rChild)
				{
					Node* next = root->right;
					root->right = nullptr;
					Insert(value, root->right);
					root->rChild = 1;
					root->right->right = next;
					root->right->rChild = 0;
					root->right->left = root;
					root->right->lChild = 0;
					if (next == head)
					{
						head->left = root->right;
					}
				}
				else
				{
					Insert(value, root->right);
					root->rChild = 1;
				}
			}
		}

		void Insert(Type&& value, Node*& root)
		{
			if (!size)
			{
				this->root = new Node(move(value), head, head, 0, 0);
				head->right = this->root;
				head->left = this->root;
				++size;
			}
			elif(!root)
			{
				root = new Node(move(value));
				++size;
			}
			elif(value < root->value)
			{
				if (!root->lChild)
				{
					Node* prev = root->left;
					root->left = nullptr;
					Insert(value, root->left);
					root->lChild = 1;
					root->left->left = prev;
					root->left->lChild = 0;
					root->left->right = root;
					root->left->rChild = 0;
					if (prev == head)
					{
						head->right = root->left;
					}
				}
				else
				{
					Insert(value, root->left);
					root->lChild = 1;
				}
			}
			elif(root->value < value)
			{
				if (!root->rChild)
				{
					Node* next = root->right;
					root->right = nullptr;
					Insert(value, root->right);
					root->rChild = 1;
					root->right->right = next;
					root->right->rChild = 0;
					root->right->left = root;
					root->right->lChild = 0;
					if (next == head)
					{
						head->left = root->right;
					}
				}
				else
				{
					Insert(value, root->right);
					root->rChild = 1;
				}
			}
		}

		void Clear(Node* root)
		{
			if (root)
			{
				if (root->lChild && root->lChild)
					Clear(root->left);
				if (root->rChild && root->right)
					Clear(root->right);
				delete root;
			}
		}

	public:

		ThreadedBinaryTree()
			:head(new Node(Type(), nullptr, nullptr, 0, 0)), root(nullptr), size(0) {  }

		ThreadedBinaryTree(initializer_list<Type>&& il)
			:ThreadedBinaryTree()
		{
			for (auto iter = il.begin(); iter != il.end(); ++iter)
			{
				Insert(*iter, root);
			}
		}

		~ThreadedBinaryTree()
		{
			Clear(root);
			delete head;
		}
	};

	template<class Type>
	class ThreadedBalancedBinarySearchTree
	{
	private:

		struct Node
		{
			Type  value;
			Node* left;
			bool  lChild;
			Node* right;
			bool  rChild;

			Node(const Type& value, Node* left = nullptr, Node* right = nullptr, bool lChild = 1, bool rChild = 1)
				:value(value), left(left), right(right), lChild(lChild), rChild(rChild) {  }

			Node(Type&& value, Node* left = nullptr, Node* right = nullptr, bool lChild = 1, bool rChild = 1)
				:value(value), left(left), right(right), lChild(lChild), rChild(rChild) {  }

		};

	private:

		Node* head;
		Node* root;
		size_t size;

	private:

		void Insert(const Type& value, Node*& root)
		{
			if (!size)
			{
				this->root = new Node(value, head, head, 0, 0);
				head->right = this->root;
				head->left = this->root;
				++size;
			}
			elif(!root)
			{
				root = new Node(value);
				++size;
			}
			elif(value < root->value)
			{
				if (!root->lChild)
				{
					Node* prev = root->left;
					root->left = nullptr;
					Insert(value, root->left);
					root->lChild = 1;
					root->left->left = prev;
					root->left->lChild = 0;
					root->left->right = root;
					root->left->rChild = 0;
					if (prev == head)
					{
						head->right = root->left;
					}
				}
				else
				{
					Insert(value, root->left);
					root->lChild = 1;
				}
			}
			elif(root->value < value)
			{
				if (!root->rChild)
				{
					Node* next = root->right;
					root->right = nullptr;
					Insert(value, root->right);
					root->rChild = 1;
					root->right->right = next;
					root->right->rChild = 0;
					root->right->left = root;
					root->right->lChild = 0;
					if (next == head)
					{
						head->left = root->right;
					}
				}
				else
				{
					Insert(value, root->right);
					root->rChild = 1;
				}
			}
		}

		void Insert(Type&& value, Node*& root)
		{
			if (!size)
			{
				this->root = new Node(move(value), head, head, 0, 0);
				head->right = this->root;
				head->left = this->root;
				++size;
			}
			elif(!root)
			{
				root = new Node(move(value));
				++size;
			}
			elif(value < root->value)
			{
				if (!root->lChild)
				{
					Node* prev = root->left;
					root->left = nullptr;
					Insert(value, root->left);
					root->lChild = 1;
					root->left->left = prev;
					root->left->lChild = 0;
					root->left->right = root;
					root->left->rChild = 0;
					if (prev == head)
					{
						head->right = root->left;
					}
				}
				else
				{
					Insert(value, root->left);
					root->lChild = 1;
				}
			}
			elif(root->value < value)
			{
				if (!root->rChild)
				{
					Node* next = root->right;
					root->right = nullptr;
					Insert(value, root->right);
					root->rChild = 1;
					root->right->right = next;
					root->right->rChild = 0;
					root->right->left = root;
					root->right->lChild = 0;
					if (next == head)
					{
						head->left = root->right;
					}
				}
				else
				{
					Insert(value, root->right);
					root->rChild = 1;
				}
			}
		}

		void Clear(Node* root)
		{
			if (root)
			{
				if (root->lChild && root->lChild)
					Clear(root->left);
				if (root->rChild && root->right)
					Clear(root->right);
				delete root;
			}
		}

	public:

		ThreadedBalancedBinarySearchTree()
			:head(new Node(Type(), nullptr, nullptr, 0, 0)), root(nullptr), size(0) {  }

		ThreadedBalancedBinarySearchTree(initializer_list<Type>&& il)
			:ThreadedBalancedBinarySearchTree()
		{
			for (auto iter = il.begin(); iter != il.end(); ++iter)
			{
				Insert(*iter, root);
			}
		}

		~ThreadedBalancedBinarySearchTree()
		{
			Clear(root);
			delete head;
		}
	};
}

#endif // ThreadedTree_h__