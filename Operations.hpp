#pragma once
#ifndef Operations_h__
#define Operations_h__

#include "Helper.h"
#include "Queue.h"
#include <map>

namespace Struct
{

	using namespace std;
	//For LinearLists Or Types With Iterators
	template<template<class> class LinearList, class Type>
	inline auto operator<<(ostream& os, const LinearList<Type>& list)->
		decltype(list.Begin(), list.End(), declval<ostream&>() << declval<Type>(), os)
	{
		for (auto iter = list.Begin(); iter != list.End(); ++iter)
			os << *iter << " ";
		return os;
	}

	//For Trees
	//In order to control the element length that printed on screen,
	//you can define the macro MAXELEMENTLENGTH to control the probable length in your btree,
	//this macro will be defined as 3 if you ignore it,
	//but this value should better be a odd number 
	template<template<class> class Tree, class Type>
	inline auto operator<<(ostream& os, const Tree<Type>& tree)->
		decltype(tree.root, declval<ostream&>() << declval<Type>(), os)
	{
		/**
		 *  Overall, we need to know which level the current element belongs to,
		 *	so that we can know how to set the align.
		 *	first, let the height of the tree be variable h,
		 *  absolutely, the nth level has 2^(n - 1) elements(containing null),
		 *	we have to know the max length of each element,
		 *  now let it be variable l,
		 */
		 //Stored the nodes in every level, containing nullptr
		if (!tree.size) return os << "NOE";
#ifndef MAXELEMENTLENGTH
#define MAXELEMENTLENGTH 3
#endif
		int l = MAXELEMENTLENGTH;
		map<int, vector<typename Tree<Type>::Node*>> nodes;

		//Initializing the node map
		{
			Queue<typename Tree<Type>::Node*> queue;
			queue.Enqueue(tree.root);
			int nodeCnt = 0;
			int maxNodes = 1;
			int currLevel = 0;
			int nullCnt = 0;
			while (1)
			{
				auto curr = queue.Dequeue();
				nodes[currLevel].push_back(curr);
				if (curr)
				{
					queue.Enqueue(curr->left);
					queue.Enqueue(curr->right);
				}
				else
				{
					queue.Enqueue(nullptr);
					queue.Enqueue(nullptr);
					nullCnt += 2;
				}

				++nodeCnt;
				if (nullCnt == maxNodes * 2)
				{
					break;
				}
				if (nodeCnt == maxNodes)
				{
					++currLevel;
					maxNodes *= 2;
					nodeCnt = 0;
					nullCnt = 0;
				}
			}
		}

		int h = nodes.size();
		//before doing this, we'd better keep l a odd number;
		int metaL = (l + 1) / 2;

		//Stored the number of space bar of every level should have
		vector<int> SpaceBarMap(h, 1);
		//Stored the length of alignment of every level
		vector<int> AlginmentMap(h, 0);
		//Initializing the two maps
		for (int i = h - 2; i >= 0; --i)
		{
			AlginmentMap[i] = 2 * AlginmentMap[i + 1] + 1;
			SpaceBarMap[i] = (SpaceBarMap[i + 1]) * 2 + l;
			AlginmentMap[i + 1] *= metaL;
		}
		AlginmentMap[0] *= metaL;


		//Finally print the whole tree with the help of these maps
		for (int level = 0; level < h; ++level)
		{
			for (int al = 0; al < AlginmentMap[level]; ++al)
			{
				os << " ";
			}
			for (int ele = 0; ele < nodes[level].size(); ++ele)
			{
				if (nodes[level][ele])
				{//if I can know the length of printing element,
				 //I can justify the length to make every element the same length.
					os << nodes[level][ele]->value;
// 					auto lEle = GetLenOfElement(nodes[level][ele]->value);
// 					if (lEle < l || lEle != 0)
// 					{
// 						for (int i = 0; i < l - lEle; ++i)
// 						{
// 							os << " ";
// 						}
// 					}
				}
				else
				{
					os << "NOE";
				}
				for (int sp = 0; sp < SpaceBarMap[level]; ++sp)
				{
					os << " ";
				}
			}
			os << endl;
		}



		return os;
	}

	template<template<class, template<class> class> class Tree, class Type, template<class> class CompareObject>
	inline auto operator<<(ostream& os, const Tree<Type, CompareObject>& tree)->
		decltype(tree.root, declval<ostream&>() << declval<Type>(), os)
	{
		if (!tree.size) return os << "NOE";
#ifndef MAXELEMENTLENGTH
#define MAXELEMENTLENGTH 3
#endif
		int l = MAXELEMENTLENGTH;
		map<int, vector<typename Tree<Type, CompareObject>::Node*>> nodes;

		//Initializing the node map
		{
			Queue<typename Tree<Type, CompareObject>::Node*> queue;
			queue.Enqueue(tree.root);
			int nodeCnt = 0;
			int maxNodes = 1;
			int currLevel = 0;
			int nullCnt = 0;
			while (1)
			{
				auto curr = queue.Dequeue();
				nodes[currLevel].push_back(curr);
				if (curr)
				{
					queue.Enqueue(curr->left);
					queue.Enqueue(curr->right);
				}
				else
				{
					queue.Enqueue(nullptr);
					queue.Enqueue(nullptr);
					nullCnt += 2;
				}

				++nodeCnt;
				if (nullCnt == maxNodes * 2)
				{
					break;
				}
				if (nodeCnt == maxNodes)
				{
					++currLevel;
					maxNodes *= 2;
					nodeCnt = 0;
					nullCnt = 0;
				}
			}
		}

		int h = nodes.size();
		//before doing this, we'd better keep l a odd number;
		int metaL = (l + 1) / 2;

		//Stored the number of space bar of every level should have
		vector<int> SpaceBarMap(h, 1);
		//Stored the length of alignment of every level
		vector<int> AlginmentMap(h, 0);
		//Initializing the two maps
		for (int i = h - 2; i >= 0; --i)
		{
			AlginmentMap[i] = 2 * AlginmentMap[i + 1] + 1;
			SpaceBarMap[i] = (SpaceBarMap[i + 1]) * 2 + l;
			AlginmentMap[i + 1] *= metaL;
		}
		AlginmentMap[0] *= metaL;


		//Finally print the whole tree with the help of these maps
		for (int level = 0; level < h; ++level)
		{
			for (int al = 0; al < AlginmentMap[level]; ++al)
			{
				os << " ";
			}
			for (int ele = 0; ele < nodes[level].size(); ++ele)
			{
				if (nodes[level][ele])
				{//if I can know the length of printing element,
				 //I can justify the length to make every element the same length.
					os << nodes[level][ele]->value;
					auto lEle = GetLenOfElement(nodes[level][ele]->value);
					if (lEle < l || lEle != 0)
					{
						for (int i = 0; i < l - lEle; ++i)
						{
							os << " ";
						}
					}
				}
				else
				{
					os << "NOE";
				}
				for (int sp = 0; sp < SpaceBarMap[level]; ++sp)
				{
					os << " ";
				}
			}
			os << endl;
		}



		return os;
	}



	template<class Iterator, class Type, class = decltype(declval<Type>() == declval<Type>())>
	inline Iterator Find(Iterator begin, Iterator end, const Type& target)
	{
		while (begin != end)
		{
			if (*begin == target) return begin;
			++begin;
		}
		return begin;
	}

	template<class Iterator, class Type, class = decltype(declval<Type>() == declval<Type>())>
	inline Iterator Find(Iterator begin, Iterator end, Type&& target)
	{
		while (begin != end)
		{
			if (*begin == target) return begin;
			++begin;
		}
		return begin;
	}

}


#endif // Operations_h__