#include <vector>
#include <iostream>
#include "AlogoEasy.h"
#include <string>
#include "Helper.h"
#include "ProblemSet.h"
#include "Structures.h"
using namespace std;
void test001()
{
	cout << Alogo::CountPrimes_EulerSieve(100000) << endl;
}
void test002()
{
	cout << Alogo::CountPrimes_EratosthenesSieve(100000) << endl;
}

int main()
{
	auto map = Alogo::GetPrefixMap("aaababababbb");
	std::string s = "bbaabbbaaabaabbbbaababbaabbaababaabbbaaaabaaabaababbbababaaaabbabbabbababbaaaaabbbbabaaabbabaabbbbaabbababbabbaaabbbbbbaaabaababaa";
	auto ret = Alogo::KmpMatch(s, "aaababababbb");


	auto p = [vowels = "aeiouAEIOU"s](char c)
	{
		return vowels.find(c) == string::npos;
	};
	
	//cout << Timer<milliseconds>(test001).time << " ms" << endl;
	//cout << Timer<milliseconds>(test002).time << " ms" << endl;
	//int i = INT_MAX & (~INT_MAX + 1);
	//cout << Problems::LeftRotate("abcdefgh", 3);
	//cout << Problems::FindAInt1({0, 1, 2, 3});
	//auto r1 = Problems::FindAnagrams({"pots","stop","tops","job","boj","obj"});


	return 0;

}

