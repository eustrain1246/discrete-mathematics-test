// Project05.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

using Matrix_t = vector<vector<int> >;
using Line_t = vector<int>;
/*
DirectedGraph类：
用来表示一个有向图。
成员变量：
m_AdjMat：邻接矩阵
m_ReachabilityMat：可达矩阵
成员函数：
DirectedGraph()：默认构造函数，构造一个空图。
~DirectedGraph()：析构函数
DirectedGraph(string filepath)：解析文件filepath，构造一个DirectedGraph对象。
filepath文件格式与项目四相同，但本项目的图为有向图。
DirectedGraph(const Graph & graph)：复制构造函数
operator=(const Graph & graph)：赋值运算符
ClearGraph()：清空图的邻接矩阵和可达矩阵。
OutputGraph()：输出图的邻接矩阵
operator*(const DirectedGraph & graph): 乘法运算符，用于实现可达矩阵运算中的矩阵逻辑乘
DirectedGraph Pow(int power)：邻接矩阵的整数次幂。
用法如下：
DirectedGraph a;
a = a.Pow(5);
即a的5次幂，相当于a = a * a * a * a * a;
注意要考虑0次幂的情况。
该函数适合以递归实现。
DirectedGraph MatOr(DirectedGraph graph)：矩阵逐元素的逻辑或运算。
例如：
1 0
0 1
与
0 1
1 0
运算后的结果为
1 1
1 1
void CalcReachabilityMat()：计算可达矩阵，要求该函数基于*运算符和Pow函数实现
void OutputReachabilityMat()：输出可达矩阵
IsConnected(int src, int dst)：基于可达矩阵判断从节点src与dst之间是否存在通路，如存在返回真，否则返回假

注意：请不要修改本文件中现有的任何内容，只允许添加各成员函数的定义。如需要，可自行添加任意辅助函数
*/
class DirectedGraph
{
private:
	vector<vector<int>> m_AdjMat;
	vector<vector<int>> m_ReachabilityMat;
public:
	DirectedGraph();
	~DirectedGraph();
	void ClearGraph();
	DirectedGraph(string filepath);
	DirectedGraph(const DirectedGraph & graph);
	DirectedGraph & operator=(const DirectedGraph & graph);
	void OutputGraph();
	DirectedGraph operator*(const DirectedGraph & graph);
	DirectedGraph Pow(int power);
	DirectedGraph MatOr(DirectedGraph graph);
	void CalcReachabilityMat();
	void OutputReachabilityMat();
	bool IsConnected(int src, int dst);
private:
	static DirectedGraph get_unit_matrix(int d);
};


int main()
{
	DirectedGraph dg("graph.gg");
	dg.OutputGraph();
	system("pause");
	
	DirectedGraph dg2 = dg * dg;
	cout << "dg * dg:\n";
	dg2.OutputGraph();
	system("pause");

	DirectedGraph dg3 = dg.Pow(0);
	cout << "dg^0\n作者：哈尔滨理工大学软件工程19-6班 李兴鑫 未经允许 禁止盗用";
	dg3.OutputGraph();
	system("pause");
	cout << "dg^1\n";
	dg3 = dg.Pow(1);
	dg3.OutputGraph();
	system("pause");
	cout << "dg^2\n";
	dg3 = dg.Pow(2);
	dg3.OutputGraph();
	system("pause");
	cout << "dg^3\n";
	dg3 = dg.Pow(3);
	dg3.OutputGraph();
	system("pause");
	
	dg.CalcReachabilityMat();
	dg.OutputReachabilityMat();
	system("pause");

	cout << dg.IsConnected(0, 2) << endl;
	cout << dg.IsConnected(2, 0) << endl;
	system("pause");
    return 0;
}//作者：哈尔滨理工大学软件工程19-6班 李兴鑫 未经允许 禁止盗用

DirectedGraph::DirectedGraph() {
	//do_nothing
}
DirectedGraph::~DirectedGraph() {
	//do_nothing
}
void DirectedGraph::ClearGraph() {
	this->m_AdjMat.clear();
	this->m_ReachabilityMat.clear();
}
DirectedGraph::DirectedGraph(string filepath) {
	int point_num;
	int line_num;
	FILE* file = nullptr;
	fopen_s(&file, filepath.c_str(), "r");
	if (!file)
		return;
	fscanf_s(file, "%d %d", &point_num, &line_num);
	this->m_AdjMat = Matrix_t(point_num, Line_t(point_num, 0));
	while (line_num--) {
		int o, d;
		fscanf_s(file, "%d %d", &o, &d);
		this->m_AdjMat[o][d] = 1;
	}
}
DirectedGraph::DirectedGraph(const DirectedGraph& graph) :m_AdjMat(graph.m_AdjMat), m_ReachabilityMat(graph.m_ReachabilityMat) {}
DirectedGraph& DirectedGraph::operator=(const DirectedGraph& graph) {
	this->m_AdjMat = graph.m_AdjMat;
	this->m_ReachabilityMat = graph.m_ReachabilityMat;
	return *this;
}
void DirectedGraph::OutputGraph() {
	for (const Line_t& line : this->m_AdjMat) {
		for (const int n : line)
			printf("%d\t", n);
		printf("\n");
	}//作者：哈尔滨理工大学软件工程19-6班 李兴鑫 未经允许 禁止盗用
}
DirectedGraph DirectedGraph::operator*(const DirectedGraph& graph) {
	DirectedGraph res;
	const int d = this->m_AdjMat.size();
	res.m_AdjMat = Matrix_t(d, Line_t(d, 0));
	for (int i = 0; i < d; ++i)
		for (int j = 0; j < d; ++j)
			for (int k = 0; k < d; ++k)
				res.m_AdjMat[i][j] = this->m_AdjMat[i][k] & graph.m_AdjMat[k][j] | res.m_AdjMat[i][j];
	return res;
}//作者：哈尔滨理工大学软件工程19-6班 李兴鑫 未经允许 禁止盗用
DirectedGraph DirectedGraph::Pow(int power) {
	return power == 0 ? DirectedGraph::get_unit_matrix(this->m_AdjMat.size()) : (power == 1 ? *this : *this * Pow(power - 1));
}
DirectedGraph DirectedGraph::MatOr(DirectedGraph graph) {
	DirectedGraph res;
	const int d = this->m_AdjMat.size();
	res.m_AdjMat = Matrix_t(d, Line_t(d, 0));
	for (int i = 0; i < d; ++i)
		for (int j = 0; j < d; ++j)
			res.m_AdjMat[i][j] = this->m_AdjMat[i][j] | graph.m_AdjMat[i][j];
	return res;
}
void DirectedGraph::CalcReachabilityMat() {
	DirectedGraph temp(*this);
	const int d = this->m_AdjMat.size();
	for (int i = 2; i < d; ++i)
		temp = temp.MatOr(Pow(i));
	this->m_ReachabilityMat = temp.m_AdjMat;
}
void DirectedGraph::OutputReachabilityMat() {
	for (auto& line : this->m_ReachabilityMat) {
		for (int n : line) {
			printf("%d\t", n);
		}
		printf("\n");
	}
}
bool DirectedGraph::IsConnected(int src, int dst) {
	if (this->m_ReachabilityMat.empty())
		CalcReachabilityMat();
	return this->m_ReachabilityMat[src][dst];
}
DirectedGraph DirectedGraph::get_unit_matrix(int d) {
	DirectedGraph ret;
	ret.m_AdjMat = Matrix_t(d, Line_t(d, 0));
	for (int i = 0; i < d; ++i)
		ret.m_AdjMat[i][i] = 1;
	return ret;
}//作者：哈尔滨理工大学软件工程19-6班 李兴鑫 未经允许 禁止盗用
