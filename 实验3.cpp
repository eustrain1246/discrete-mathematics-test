// Project03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;
/*
BinaryRelation

成员变量：
m_set: BinaryRelation是集合m_set上的二元关系
m_relation：二元关系矩阵

成员方法：

BinaryRelation()：默认构造函数

BinaryRelation(vector<int> set)：set是一个集合。该构造函数用set初始化一个二元关系。
每个BinaryRelation对象是一个定义在集合set上的二元关系。具体用法见main函数。
注意使用此构造函数时，已经知道集合中的元素，所以应该根据集合中元素的数量改变关系矩阵的大小

BinaryRelation(const BinaryRelation & relation)：复制构造函数

operator=：赋值运算符

AddElement(unsigned int first, unsigned int second)：将一个二元关系元素添加到关系
矩阵中。例如，对于(2，3)，通过调用AddElement(2, 3)将其添加到关系矩阵中。具体用法见main函数

Compound(const BinaryRelation relation)：返回与二元关系relation进行复合运算后的二元关系

Inverse()：返回二元关系的逆

Output()：输出二元关系矩阵

注意：请勿修改任何已有代码，如需要，可自行添加任意辅助函数
*/
class BinaryRelation
{
private:
	vector<int> m_set;
	vector<vector<int>> m_relation;
public:
	BinaryRelation();
	~BinaryRelation();
	BinaryRelation(vector<int> set);
	BinaryRelation(const BinaryRelation & relation);
	BinaryRelation & operator=(const BinaryRelation & relation);
	bool AddElement(unsigned int first, unsigned int second);
	BinaryRelation Compound(const BinaryRelation relation);
	BinaryRelation Inverse();
	void Output();

private://自己加的函数
	BinaryRelation(vector<int>&& set, vector<vector<int>>&& relation);
};

int main()
{
	vector<int> set = { 1, 2, 3, 4, 5, 6 };
	BinaryRelation br(set);
	br.AddElement(1, 2);
	br.AddElement(3, 2);
	br.AddElement(5, 6);
	br.AddElement(4, 4);
	br.AddElement(2, 4);
	br.AddElement(1, 6);
	br.Output();
	cout << "-----------------------------" << endl;

	BinaryRelation brinverse = br.Inverse();
	brinverse.Output();
	cout << "-----------------------------" << endl;
	BinaryRelation brcompoundinverse = br.Compound(brinverse);
	brcompoundinverse.Output();
	cout << "-----------------------------" << endl;
	
	system("pause");
    return 0;
}
BinaryRelation::BinaryRelation() {

}
BinaryRelation::BinaryRelation(vector<int>&& set, vector<vector<int>>&& relation)
	:m_set(std::move(set)), m_relation(std::move(relation)) {

}
BinaryRelation::~BinaryRelation() {

}
BinaryRelation::BinaryRelation(vector<int> set):m_set(std::move(set)),m_relation(m_set.size(),vector<int>(m_set.size(),0)) {
	
}

BinaryRelation::BinaryRelation(const BinaryRelation& relation):m_relation(relation.m_relation),m_set(relation.m_set) {

}
BinaryRelation& BinaryRelation:: operator=(const BinaryRelation& relation) {
	this->m_set = relation.m_set;
	this->m_relation = relation.m_relation;
	return *this;
}
bool BinaryRelation::AddElement(unsigned int first, unsigned int second) {
	//寻找两个数的下标
	int firstindex = -1;
	int secondindex = -1;
	//循环变量 index
	int index = 0;
	for (int n : this->m_set) {//因为无序集合，所以直接遍历
		if (n == first)
			firstindex = index;
		if (n == second)
			secondindex = index;
		if (firstindex != -1 && secondindex != -1)
			break;
		++index;
	}
	//如果两个数都在集合里就将对应的位置赋值为1，然后返回1（true），否则直接返回false
	return firstindex != -1 && secondindex != -1 ? m_relation[firstindex][secondindex] = 1, true : false;
}
BinaryRelation BinaryRelation::Compound(const BinaryRelation relation) {
	//因为是在集合内的二元关系，所以两个映射所在的集合必定相同，不考虑不同的情况,如果出状况由用户负责
	vector<vector<int> > new_relation(this->m_set.size(), vector<int>(this->m_set.size(), 0));//构建新的空白映射关系
	//好多层的嵌套，先在前面中寻找二元关系
	for (int i = 0; i < this->m_set.size(); ++i)
		for (int j = 0; j < this->m_set.size(); ++j)
			if (this->m_relation[i][j])//如果找到了上后面去找对应的所有关系，找到就加到新的关系上，没找到就算了
				for (int k = 0; k < this->m_set.size(); ++k)
					if (relation.m_relation[j][k] == 1)
						new_relation[i][k] = 1;
	return BinaryRelation(vector<int>(this->m_set), std::move(new_relation));//调用构造函数返回新的二元关系
}
BinaryRelation BinaryRelation::Inverse() {
	vector<vector<int>> new_relation(this->m_set.size(), vector<int>(this->m_set.size(), 0));//构建新的空白映射关系
	for (int i = 0; i < this->m_set.size(); ++i)
		for (int j = 0; j < this->m_set.size(); ++j)
			if (this->m_relation[i][j])//如果矩阵的ij为1
				new_relation[j][i] = 1;//那么新矩阵对称的位置设为1
	return BinaryRelation(vector<int>(this->m_set), std::move(new_relation));
}
void BinaryRelation::Output() {
	for (const auto& vec : this->m_relation) {
		for (int b : vec) {
			std::cout << b << ' ';
		}
		std::cout << endl;
	}
}
