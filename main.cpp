#include <vector>
#include <iostream>
#include "AlogoEasy.h"
#include <string>
#include "Helper.h"
#include "ProblemSet.h"
#include "Structures.h"
#include "Functor.h"
#include <list>
#include <corecrt_malloc.h>
using namespace std;
using namespace Struct;

void test001(		 )
{
	cout << Alogo::CountPrimes_EulerSieve(100000) << endl;
}
void test002(		 )
{
	cout << Alogo::CountPrimes_EratosthenesSieve(100000) << endl;
}
void test003(		 )
{
	function<int(long long)> mod   = [](long long n)
	{
		int num = INT_MAX;
		int ret = 0;
		for (int i = 0; i < n; ++i)
		{
			ret += num % 8;
		}
		return ret;
	};
	function<int(long long)> minus = [](long long n)
	{
		int num = INT_MAX;
		int ret = 0;
		for (int i = 0; i < n; ++i)
		{
			ret += num - (num / 8) * 8;
		}
		return ret;
	};
	function<int(long long)> digit = [](long long n)
	{
		int num = INT_MAX;
		int ret = 0;
		for (int i = 0; i < n; ++i)
		{
			ret += num & 7;
		}
		return ret;
	};

	cout << Timer<milliseconds>(mod,   10000000000).time << "ms" << endl;
	cout << Timer<milliseconds>(minus, 10000000000).time << "ms" << endl;
	cout << Timer<milliseconds>(digit, 10000000000).time << "ms" << endl;

}
void test004(		 )
{
	class tClass
	{
	private:
		int* pVal;
	public:
		explicit tClass(int val = 0)
			:pVal(new int{val})
		{
			cout << "tClass constructor called." << endl;
		}
		~tClass()
		{
			delete pVal;
			cout << "tClass destructor called." << endl;
		}
		tClass(const tClass& rhs)
			:pVal(new int{*rhs.pVal})
		{
			cout << "tClass copy constructor called." << endl;
		}
		tClass(tClass&& rhs) noexcept
			:pVal(rhs.pVal)
		{ 
			rhs.pVal = nullptr;
			cout << "tClass move constructor called." << endl;
		}
		tClass& operator=(const tClass& rhs)
		{
			tClass tmp = rhs;//call tClass copy constructor
			swap(tmp, *this);
			cout << "tClass copy operator= called." << endl;
			return *this;
		}
		tClass& operator=(tClass&& rhs)noexcept
		{
			pVal = rhs.pVal;
			rhs.pVal = nullptr;
			cout << "tClass move operator= called." << endl;
			return *this;
		}
	};

	tClass c1;
	tClass c2; 
	c2 = move(c1);

}
void test005(uint len)
{
	vector<int> sequence(len);
	for (unsigned i = 0; i < len; ++i)
	{
		sequence[i] = rand() & 99;
	}
	auto f1 = [&]() 
	{
		cout << Problems::GetMaxSubsequence1(sequence) << " ";
	};
	auto f2 = [&]() 
	{
		cout << Problems::GetMaxSubsequence2(sequence) << " ";
	};
	auto f3 = [&]() 
	{
		cout << Problems::GetMaxSubsequence3(sequence) << " ";
	};
	//cout << Timer<milliseconds>(f1).time << "ms" << endl; // over 10000 bad alloc
	//cout << Timer<milliseconds>(f2).time << "ms" << endl; //too slow
	cout << Timer<milliseconds>(f3).time << "ms" << endl;
}
void test006(uint len)
{
// 	cout << "Adding " << len << " element(s) to a binary tree costs " << Timer<milliseconds>([&]()
// 		{
// 			Struct::BinaryTree<int> bt1;
// 			for (int i = 0; i < len; ++i)
// 			{
// 				bt1.Add(i);
// 			}
// 		}).time << " ms." << endl;

	srand(0);
	BinarySearchTree<int> bst1;
	CachedForwardList<int> cfl1;
	cout << "Adding " << len << " element(s) to a BinarySearchTree costs " << Timer<milliseconds>([&]()
		{
			for (int i = 0; i < len; ++i)
			{
				bst1.Insert(rand() % len);
			}
		}).time << " ms." << endl;

	//cout << bst1 << endl;
	srand(0);
	cout << "Removing " << len << " element(s) to a BinarySearchTree costs " << Timer<milliseconds>([&]()
		{
			for (int i = 0; i < len; ++i)
			{
				bst1.Remove(rand() % len);
			}
		}).time << " ms." << endl;

	cout << "Adding " << len << " element(s) to a CachedForwardList costs " << Timer<milliseconds>([&]()
		{
			for (int i = 0; i < len; ++i)
			{
				cfl1.PushBack(rand() % len);
			}
		}).time << " ms." << endl;

	srand(0);
	cout << "Removing " << len << " element(s) to a CachedForwardList costs " << Timer<milliseconds>([&]()
		{
			for (int i = 0; i < len; ++i)
			{
				cfl1.PopFront();
			}
		}).time << " ms." << endl;
	
}
void test007(uint len)
{

	Struct::CachedForwardList<int> l1;
	list<int> l2;
	//PushBack Elements
		cout << "PushBack  " << len << " times took " 
			<< Timer<milliseconds>([&]() {
			for (uint i = 0;i < len; ++i)
			{
				l1.PushBack(rand());
			}
			}).time 
			<< " ms in CachedForwardList" << endl;

		cout << "PushBack  " << len << " times took "
			<< Timer<milliseconds>([&]() {
			for (uint i = 0; i < len; ++i)
			{
				l2.push_back(rand());
			}
				}).time
			<< " ms in list" << endl;
	//PushFront Elements
		cout << "PushFront " << len << " times took "
			<< Timer<milliseconds>([&]() {
			for (uint i = 0; i < len; ++i)
			{
				l1.PushFront(rand());
			}
			}).time
			<< " ms in CachedForwardList" << endl;

		cout << "PushFront " << len << " times took "
			<< Timer<milliseconds>([&]() {
			for (uint i = 0; i < len; ++i)
			{
				l2.push_front(rand());
			}
			}).time
			<< " ms in list" << endl;
	//PopBack Elements
		cout << "PopBack   " << len << " times took "
			<< Timer<milliseconds>([&]() {
			for (uint i = 0; i < len; ++i)
			{
				l1.PopBack();
			}
				}).time
			<< " ms in CachedForwardList" << endl;

		cout << "PopBack   " << len << " times took "
			<< Timer<milliseconds>([&]() {
			for (uint i = 0; i < len; ++i)
			{
				l2.pop_back();
			}
				}).time
			<< " ms in list" << endl;
	//PopFront Elements
		cout << "PopFront  " << len << " times took "
			<< Timer<milliseconds>([&]() {
			for (uint i = 0; i < len; ++i)
			{
				l1.PopFront();
			}
			}).time
			<< " ms in CachedForwardList" << endl;

		cout << "PopFront  " << len << " times took "
			<< Timer<milliseconds>([&]() {
			for (uint i = 0; i < len; ++i)
			{
				l2.pop_front();
			}
			}).time
			<< " ms in list" << endl;

	//Insert Middle Elements
		cout << "Insert    " << len << " times took "
			<< Timer<milliseconds>([&]() {
			for (uint i = 0; i < len; ++i)
			{
				l1.Insert(i / 2, rand());
			}
				}).time
			<< " ms in CachedForwardList" << endl;

		cout << "Insert    " << len << " times took "
			<< Timer<milliseconds>([&]() {
			for (uint i = 0; i < len; ++i)
			{
				l2.insert(l2.begin(), rand());
			}
			}).time
			<< " ms in list" << endl;
	//Remove Middle Elements

	
	
}

template<class T>
class TestClass2 {public: T* root; };

int main()
{
	BinarySearchTree<int> bst1 = {1, 2, 3};

	BinarySearchTree<int> bst2;

	ThreadedBinaryTree<int> tbt = {10, 5, 15, 3, 8, 13, 19};

	BinaryHeap<int> bh1{21, 32, 34, 12, 32, 11, 9};



	return 0;
}

