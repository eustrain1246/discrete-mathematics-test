// Project02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;


/*
BuildInjection
功能：构造集合src到集合dst的单射，将建立的映射保存在返回值injection中
例如，将src中的元素1映射为dst中的元素3，则injection[1] = 3
*/
map<int, int> BuildInjection(vector<int> src, vector<int> dst)
{
	map<int, int> injection;
	//请把你的代码写在下方，请勿修改任何已有代码
	if (src.size() > dst.size()) {
		cerr << "src.size() > dst.size()\n";
		exit(-1);
	}
	for (auto it1 = src.begin(), it2 = dst.begin(); it1 != src.end() && it2 != dst.end(); ++it1, ++it2)
		injection.insert(std::pair<int, int>(*it1, *it2));
	//
	return injection;
}
/*
ValidateInjection
功能：验证给定的两个集合src和dst之间的映射injection是否为单射。
     如果是单射则返回true，不是单射返回false
*/
bool ValidateInjection(vector<int> src, vector<int> dst, map<int, int> injection)
{
	bool bIsInjection = false;
	//请把你的代码写在下方，请勿修改任何已有代码
	if (src.size() == injection.size()) {
//		std::vector<bool> bsrc(src.size(), false);
		for (auto s : src) {
			auto it = injection.find(s);
			bool find = false;
			for(auto d : dst)
				if (d == it->second) {
					find = true;
					break;
				}
			if (find == false)
				break;
		}
		bIsInjection = true;
	}
	//
	return bIsInjection;
}
/*
BuildSurjection
功能：构造集合src到集合dst的满射，将建立的映射保存在返回值surjection中
*/
map<int, int> BuildSurjection(vector<int> src, vector<int> dst)
{
	map<int, int> surjection;
	//请把你的代码写在下方，请勿修改任何已有代码
	if (src.size() < dst.size()) {
		cerr << "src.size() < dst.size()\n";
		exit(-1);
	}
	std::vector<int>::iterator iter1 = src.begin(), iter2 = dst.begin();
	while (iter1 != src.end()) {
		surjection.insert(std::pair<int, int>(*iter1, *iter2));
		++iter1;
		if (++iter2 == dst.end())
			iter2 = dst.begin();
	}
	//
	return surjection;
}
/*
ValidateSurjection
功能：验证给定的两个集合src和dst之间的映射surjection是否为满射。
如果是满射则返回true，不是满射返回false
*/
bool ValidateSurjection(vector<int> src, vector<int> dst, map<int, int> surjection)
{
	bool bIsSurjection = false;
	//请把你的代码写在下方，请勿修改任何已有代码
	if (src.size() >= dst.size()) {
		vector<bool> bsrc(src.size(), false);
		vector<bool> bdst(dst.size(), false);
		for (const auto m : surjection) {
			for (int i = 0; i < src.size(); ++i)
				if (src[i] == m.first)
					bsrc[i] = true;
			for (int i = 0; i < dst.size(); ++i)
				if (dst[i] == m.second)
					bdst[i] = true;
		}
		for (auto b : bsrc)
			if (b == false)
				return false;
		for (auto b : bdst)
			if (b == false)
				return false;
		bIsSurjection = true;
	}
	//
	return bIsSurjection;
}
/*
BuildBijection
功能：构造集合src到集合dst的双射，将建立的映射保存在返回值bijection中
*/
map<int, int> BuildBijection(vector<int> src, vector<int> dst)
{
	map<int, int> bijection;
	//请把你的代码写在下方，请勿修改任何已有代码
	if (src.size() != dst.size()) {
		cerr << "src.size() != dst.size()\n";
		exit(-1);
	}
	auto iter1 = src.begin(), iter2 = dst.begin();
	while (iter1 != src.end()) {
		bijection.insert(pair<int, int>(*iter1, *iter2));
		++iter1;
		++iter2;
	}
	//
	return bijection;
}
/*
ValidateBijection
功能：验证给定的两个集合src和dst之间的映射bijection是否为双射。
如果是双射则返回true，不是满射返回false
*/
bool ValidateBijection(vector<int> src, vector<int> dst, map<int, int> bijection)
{
	bool bBiSurjection = false;
	//请把你的代码写在下方，请勿修改任何已有代码
	if (src.size() >= dst.size()) {
		vector<bool> bsrc(src.size(), false);
		vector<bool> bdst(dst.size(), false);
		for (const auto m : bijection) {
			for (int i = 0; i < src.size(); ++i)
				if (src[i] == m.first)
					if (bsrc[i] == false)
						bsrc[i] = true;
					else
						return false;
			for (int i = 0; i < dst.size(); ++i)
				if (dst[i] == m.second)
					if (bdst[i] == false)
						bdst[i] = true;
					else
						return false;
		}
		for (auto b : bsrc)
			if (b == false)
				return false;
		for (auto b : bdst)
			if (b == false)
				return false;
		bBiSurjection = true;
	}
	//
	return bBiSurjection;
}

int main()
{
	vector<int> setA, setB, setC;
	for (int i = 0; i < 10; i++)
		setA.push_back(i);
	for (int i = 50; i < 70; i++)
		setB.push_back(i);
	for (int i = 130; i < 140; i++)
		setC.push_back(i);

	map<int, int>::iterator iter;

	map<int, int> injection;
	injection = BuildInjection(setA, setB);
	for (iter = injection.begin(); iter != injection.end(); iter++)
		cout << iter->first << "->" << iter->second << endl;
	cout << ValidateInjection(setA, setB, injection) << endl;
	system("pause");

	map<int, int> surjection;
	surjection = BuildSurjection(setB, setA);
	for (iter = surjection.begin(); iter != surjection.end(); iter++)
		cout << iter->first << "->" << iter->second << endl;
	cout << ValidateSurjection(setB, setA, surjection) << endl;
	system("pause");

	map<int, int> bijection;
	bijection = BuildBijection(setA, setC);
	for (iter = bijection.begin(); iter != bijection.end(); iter++)
		cout << iter->first << "->" << iter->second << endl;
	cout << ValidateBijection(setA, setC, bijection) << endl;
	system("pause");

    return 0;
}

