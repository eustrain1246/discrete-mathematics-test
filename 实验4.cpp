// Project04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include<fstream>//自己添加的头文件
using namespace std;
/*
Node结构：
该结构体是Graph类的辅助结构体，用于存储图的邻接表中的各个节点
m_iNodeID：节点ID
m_pNextNode：下一个节点
*/
struct Node
{
	unsigned int m_iNodeID;
	Node * m_pNextNode;
};
/*
Graph类：
用来表示一个无向图。
成员变量：
m_AdjMat：邻接矩阵
m_AdjList：邻接表
成员函数：
Graph()：默认构造函数，构造一个空图。注意应同时初始化邻接矩阵和邻接表
~Graph()：析构函数
Graph(string filepath)：解析文件filepath，构造一个Graph对象。注意要根据输入同时构造出邻接矩阵和邻接表
filepath文件格式如下：
3
2
0 1
1 2
文件格式说明：
3：表示该图有3个顶点（顶点ID从0开始编号）
2：表示该图有两条边
0 1：表示第一条边关联的两个顶点的ID分别是0和1
1 2：表示第一条边关联的两个顶点的ID分别是1和2
注意：Graph为无向图，故应根据文件中每条边的数据在邻接表和邻接矩阵中添加相应的两个方向的边。
例如对于文件中给出的边 0 1，
在邻接矩阵中，应该体现为
0 1 0
1 0 0
0 0 0
而不是
0 1 0
0 0 0
0 0 0
Graph(const Graph & graph)：复制构造函数
operator=(const Graph & graph)：赋值运算符
OutputGraph()：输出图的邻接矩阵和邻接表
输出示例：
Adjacent Matrix:
0       1       1       1
1       0       1       1
1       1       0       1
1       1       1       0
Ajdcent List:
0: 3    2       1
1: 2    3       0
2: 1    3       0
3: 2    1       0
ClearGraph()：清空图的邻接表和邻接矩阵，注意回收内存
NodeNeighbours(int nodeid)：输出给定结点的所有邻接结点
格式如下：
邻接结点1	邻接结点2	邻接结点3 ... 邻接结点n
IsEuler()：利用定理5.7判断该图是否为欧拉图，如果是，返回真，否则返回假
bool IsConnected()：根据连通图的定义判定是否为连通图
int NodeDegree(int nodeid)：输出顶点nodeid的度数

注意：请不要修改本文件中现有的任何内容，只允许添加各成员函数的定义。如需要，可自行添加任意辅助函数
*/
class Graph
{
private:
	vector<vector<int>> m_AdjMat;
	vector<Node *> m_AdjList;
public:
	Graph();
	~Graph();
	Graph(string filepath);
	Graph(const Graph & graph);
	Graph & operator=(const Graph & graph);
	void OutputGraph();
	void ClearGraph();
	void NodeNeighbours(int nodeid);
	bool IsEuler();
	bool IsConnected();
	int NodeDegree(int nodeid);
private:
	void addToList(int l, int r); //添加到链表
};

int main()
{
	Graph g("graph.gg");//此处"graph.gg"为文件路径，该路径可以根据输入文件的实际位置修改。

	g.OutputGraph();
	system("pause");

	Graph g1 = g;
	g.OutputGraph();
	system("pause");

	g.NodeNeighbours(0);
	system("pause");
	g.NodeNeighbours(1);
	system("pause");

	cout << g.IsConnected() << endl;
	system("pause");

	cout << g.IsEuler() << endl;
	system("pause");


	return 0;
}

Graph::Graph() {
	//do_nothing
}
Graph::~Graph() {
	using pNode = Node*;
	for (pNode pnode : this->m_AdjList) {
		pNode p1 = pnode, p2;
		if (p1) {
			while (p1) {
				p2 = p1;
				p1 = p1->m_pNextNode;
				delete p2;
			}
			delete p1;
		}
	}
	this->m_AdjMat.clear();
}
Graph::Graph(string filepath) {
	using pNode = Node*;
	ifstream ifs(filepath);
	int point, line;
	ifs >> point >> line;
	//初始化vector
	this->m_AdjMat = decltype(this->m_AdjMat)(point,vector<int>(point,0));//懒得写类型了，直接用decltype代替
	this->m_AdjList = decltype(this->m_AdjList)(point, nullptr);//这个也是
	//赋值
	int p1, p2;
	while (ifs >> p1 >> p2) {
		this->m_AdjMat[p1][p2] = 1;
		this->m_AdjMat[p2][p1] = 1;
		addToList(p1, p2);
		addToList(p2, p1);
	}
}
Graph::Graph(const Graph& graph)//老师可算记得加const了
:m_AdjMat(graph.m_AdjMat),m_AdjList(graph.m_AdjList.size(),nullptr){//拷贝矩阵，初始化链表
	using pNode = Node*;
	//对链表进行初始化
	auto l = begin(this->m_AdjList);
	auto r = begin(graph.m_AdjList);
	while (r != end(graph.m_AdjList)) {//两条链表一起走
		pNode rp = *r;
		pNode newlisthead = nullptr, newlist = nullptr;
		while (rp) {
			if (newlist) {//从下面拷贝过来的代码,再改一改
				newlist->m_pNextNode = new Node();//添加节点
				newlist->m_pNextNode->m_iNodeID = rp->m_iNodeID;
				newlist->m_pNextNode->m_pNextNode = nullptr;//实际上new之后这里会被自动初始化为nullptr
				newlist = newlist->m_pNextNode;
			}
			else {
				newlisthead = newlist = new Node();
				newlist->m_iNodeID = rp->m_iNodeID;
				newlist->m_pNextNode = nullptr;//实际上new之后这里也会被自动初始化为nullptr
			}
			rp = rp->m_pNextNode;
		}
		*l = newlisthead;
		++l;
		++r;
	}
}
Graph& Graph::operator=(const Graph& graph) {
	//通过已经写好的构造函数和析构函数提高代码的复用率
	this->~Graph();
	Graph temp(graph);
	this->m_AdjMat = std::move(temp.m_AdjMat);
	this->m_AdjList = std::move(temp.m_AdjList);
	return *this;
}
void Graph::OutputGraph() {
	std::cout << "Adjacent Matrix :" << std::endl;
	for (auto& v : this->m_AdjMat) {
		for (auto i : v)
			std::cout << i << '\t';
		std::cout << std::endl;
	}
	std::cout << "Ajdcent List :" << std::endl;
	for (int i = 0; i < this->m_AdjList.size(); ++i) {
		cout << i << " : ";
		using pNode = Node*;
		pNode pnode = this->m_AdjList[i];
		while (pnode) {
			std::cout << pnode->m_iNodeID << '\t';
			pnode = pnode->m_pNextNode;
		}
		std::cout << endl;
	}

	
}
void Graph::ClearGraph() {
	this->~Graph();//直接调用析构函数
}
void Graph::NodeNeighbours(int nodeid) {
	using pNode = Node*;
	pNode pnode = this->m_AdjList[nodeid];
	while (pnode) {
		std::cout << pnode->m_iNodeID << '\t';
		pnode = pnode->m_pNextNode;
	}
	std::cout << endl;
}
bool Graph::IsEuler() {
	if (this->m_AdjMat.empty())return false;
	for (auto& v : this->m_AdjMat) {
		int sum = 0;
		for (auto i : v)
			sum += i;
		if (sum % 2 == 1)
			return false;
	}
	return true;
}
bool Graph::IsConnected() {
	using Matrix = decltype(this->m_AdjMat);
	const int n = this->m_AdjMat.size();
	std::vector<Matrix>MatArr(n, this->m_AdjMat);//先搞一堆矩阵出来
	MatArr[0] = this->m_AdjMat;
	const int l = 0;
	for (int r = 0; r < n; r++) {
		auto& matl = MatArr[l];
		auto& matr = MatArr[r];
		Matrix res(n, std::vector<int>(n, 0));
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j)
				if (matr[i][j])
					for (int k = 0; k < n; ++k)
						if (matl[j][k])
							res[i][k] = 1;
		}
	}
	for (int i = 0; i < n; ++i)//将指向自身的设为1
		MatArr[n - 1][i][i] = 1;
	for (auto& v : MatArr[n - 1]) {
		for (int i : v)
			if (i == 0)
				return false;
  //作者：哈尔滨理工大学软件19-6班 李兴鑫 未经允许 禁止盗用
	}
	return true;
	
}
int Graph::NodeDegree(int nodeid) {
	int sum = 0;//计数
	for (auto i : this->m_AdjMat[nodeid])
		sum += i;
	return sum;
}

void Graph::addToList(int l, int r) {//添加到链表
	using pNode = Node*;
	pNode pnode = this->m_AdjList[l];
	if (pnode) {
		while (pnode->m_pNextNode)pnode = pnode->m_pNextNode;//到达链表尾
		pnode->m_pNextNode = new Node();//添加节点
		pnode->m_pNextNode->m_iNodeID = r;
		pnode->m_pNextNode->m_pNextNode = nullptr;//实际上new之后这里会被自动初始化为nullptr
	}
	else {
		this->m_AdjList[l] = pnode = new Node();
		pnode->m_iNodeID = r;
		pnode->m_pNextNode = nullptr;//实际上new之后这里也会被自动初始化为nullptr
	}
	//作者：哈尔滨理工大学软件19-6班 李兴鑫 未经允许 禁止盗用
}
