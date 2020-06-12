// Project06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

#define MOD 11

#define ID_NONE 0
#define ID_ADD 1
#define ID_MULT 2

class BinaryOp //基类：二元运算
{
public:
	BinaryOp(){}
	virtual int operator() (int a, int b) = 0;
};

class Add : public BinaryOp //对二元运算的实现 加法
{
public:
	Add() {}
	virtual int operator() (int a, int b) { return (a + b) % MOD; }
};

class Mult : public BinaryOp //对二元运算的实现 乘法
{
public:
	Mult(){}
	virtual int operator() (int a, int b) { return (a * b) % MOD; }
};

/*
AlgSys类：用来表示一个代数系统（A, *）

成员变量
m_Set：代数系统中的集合
m_pOp：指向运算符的指针，运算符是一个functor。这里使用基类指针以便实现多态。
m_iOpID：运算符ID。该成员变量的作用是标识运算符类型，以便在复制构造函数和
赋值运算符中正确完成深拷贝
m_iIdentity：单位元。有的代数系统没有单位元，因此在各种构造函数中，单位元
被初始化为MOD（因为本项目中的两种运算分别是模K加法和模K乘法，所以模数是不可
能在运算过程中得到的值）

成员函数：
AlgSys()：构造函数。要求把m_iOpID和m_iIdentity分别初始化为ID_NONE和MOD
~AlgSys()：析构函数。注意完成内存的回收。
AlgSys(vector<int> aset, int opid)：构造函数。要求用aset初始化代数系统中
的集合，并根据opid实例化相应的运算符对象，并使m_pOp指向该对象：
当opid为ID_ADD时，实例化Add对象；当opid为ID_MULT时，实例化Mult对象。
AlgSys(const AlgSys & as)：复制构造函数。注意根据m_iOpID完成深拷贝。
const AlgSys & operator=(const AlgSys & as)：赋值运算符。注意根据m_iOpID
完成深拷贝。
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
int PerformOp(int left, int right)：该函数根据实际的运算符对象以left和right
作为操作数，完成二元运算。
要求：后面的IsClosed()、IsAssociative()、HasIdentity()、EachEleHasInverse()
四个函数都利用该函数完成运算。
bool IsClosed()：判断运算符m_pOp在m_Set上是否封闭。如封闭返回真，否则返回假。
要求在验证过程中调用PerformOp函数完成二元运算。
bool IsAssociative()：判断运算符m_pOp在m_Set上是否满足结合律。如满足返回真，
否则返回假。要求在验证过程中调用PerformOp函数完成二元运算。
bool HasIdentity()：判断该代数系统是否存在单位元。如存在，返回真，并用成员变量
m_iIdentity存储单位元。如不存在，返回否（此时m_iIdentity的值仍应为MOD）。
要求在验证过程中调用PerformOp函数完成二元运算。
void OutputIdentity()：输出单位元。
bool EachEleHasInverse()：判断m_Set中是否每个元素都有逆元。如是，返回真，否则
返回假。要求在验证过程中调用PerformOp函数完成二元运算。
bool IsGroup()：判断该代数系统是否为群。如是，返回真，否则返回假。
*/

class AlgSys
{
private:
	vector<int> m_Set;
	BinaryOp * m_pOp;
	int m_iOpID;
	int m_iIdentity;
private:
	int PerformOp(int left, int right) { return (*m_pOp)(left, right); }
public:
	AlgSys();
	~AlgSys();
	AlgSys(vector<int> aset, int opid);
	AlgSys(const AlgSys & as);
	const AlgSys & operator=(const AlgSys & as);
	bool IsClosed();
	bool IsAssociative();
	bool HasIdentity();
	void OutputIdentity();
	bool EachEleHasInverse();
	bool IsGroup();

private:
	BinaryOp* getpOp(const int opid);
	bool inSet(int);
};



int main()
{
	vector<int> a = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
	AlgSys as1(a, ID_ADD);
	cout << "IsClosed: " << as1.IsClosed() << endl;
	cout << "IsAssociative: " << as1.IsAssociative() << endl;
	cout << "HasIdentity: " << as1.HasIdentity() << endl;
	cout << "Identity: ";
	as1.OutputIdentity();
	cout << "EachEleHasInverse: " << as1.EachEleHasInverse() << endl;
	cout << "IsGroup: " << as1.IsGroup() << endl;
	system("pause");
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
	AlgSys as2(a, ID_MULT);
	cout << "IsClosed: " << as2.IsClosed() << endl;
	cout << "IsAssociative: " << as2.IsAssociative() << endl;
	cout << "HasIdentity: " << as2.HasIdentity() << endl;
	cout << "Identity: ";
	as2.OutputIdentity();
	cout << "EachEleHasInverse: " << as2.EachEleHasInverse() << endl;
	cout << "IsGroup: " << as2.IsGroup() << endl;
	system("pause");
	return 0;
}
AlgSys::AlgSys() {
	this->m_iOpID = ID_NONE;
	this->m_iIdentity = MOD;
	this->m_pOp = nullptr;
}
AlgSys::~AlgSys() {
	delete this->m_pOp;
}
AlgSys::AlgSys(vector<int> aset, int opid) {
	this->m_pOp = this->getpOp(opid);
	this->m_Set = std::move(aset);
	this->m_iOpID = opid;
}
AlgSys::AlgSys(const AlgSys& as)
	:AlgSys(as.m_Set, as.m_iOpID) {
	this->m_iIdentity = as.m_iIdentity;
}
const AlgSys& AlgSys::operator=(const AlgSys& as) {
	this->m_iIdentity = as.m_iIdentity;
	this->m_iOpID = as.m_iOpID;
	this->m_Set = as.m_Set;
	this->m_pOp = getpOp(as.m_iOpID);
	return *this;
}
bool AlgSys::IsClosed() {
	for (int i : this->m_Set)
		for (int j : this->m_Set)
			if (!this->inSet(this->PerformOp(i, j)))
				return false;
	return true;
}
bool AlgSys::IsAssociative() {
	for (size_t i = 0; i < this->m_Set.size() - 1; ++i)
		for (size_t j = i + 1; j < this->m_Set.size(); ++j)
			if (this->PerformOp(this->m_Set[i], this->m_Set[j]) != this->PerformOp(this->m_Set[j], this->m_Set[i]))
				return false;
	return true;
}
bool AlgSys::HasIdentity() {//是否存在单位元
	for (int i : this->m_Set) {
		bool hasdiff = false;
		for (int j : this->m_Set)
			if (j != this->PerformOp(i, j) || j != this->PerformOp(j, i)) {
				hasdiff = true;
				break;
			}
		if (!hasdiff) {
			this->m_iIdentity = i;
			return true;
		}
	}
	return false;
}
void AlgSys::OutputIdentity() {
	if (this->m_iIdentity == MOD)
		this->HasIdentity();
	std::cout << this->m_iIdentity << std::endl;
}//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
bool AlgSys::EachEleHasInverse() {
	if (this->m_iIdentity == MOD)
		if (!this->HasIdentity())
			return false;
	int iden = this->m_iIdentity;
	for (int i : this->m_Set) {
		bool hasInverse = false;
		for (int j : this->m_Set) {
			if (iden == this->PerformOp(i, j) && iden == this->PerformOp(j, i)) {
				hasInverse = true;
				break;
			}
		}
		if (!hasInverse)
			return false;
	}
	return true;
}
bool AlgSys::IsGroup() {
	return this->IsClosed() && this->EachEleHasInverse();
}

BinaryOp* AlgSys::getpOp(const int opid) {
	switch (opid)
	{
	case ID_ADD:
  //本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
		return new Add();
	case ID_MULT:
		return new Mult();
	default:
		return nullptr;
	}
}

bool AlgSys::inSet(int value) {
	for (int num : this->m_Set)
		if (num == value)
			return true;
	return false;
}
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
