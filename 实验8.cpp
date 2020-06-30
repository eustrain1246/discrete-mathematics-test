// Project08.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <iostream>
#include <bitset>
using namespace std;
/*
PropositionalFormular类：

成员变量：
m_strPropFmlr - 命题公式
m_strRevertPolishNotation - 命题公式对应的逆波兰表达式
m_OperatorStack - 用来存储联结词的栈
m_PriorityMap - 静态成员。这是一个<char, int>的map，用联结词作为key，
用联结词的优先级做为value。value值越大，表示优先级越高。
m_OperandStack - 用于存储操作数的栈。该栈在解析逆波兰式时暂存操作数（即命题变元和常元）
m_vecTableTitle - 真值表表头。假设向量中有n个元素，前n-1个为命题变元，
第n个元素为命题公式。例如，对于公式P&Q>R，该向量有4个元素，依次为
P、Q、R、P&Q>R。注意：命题公式中可能存在命题常元T和F，注意特殊处理。
另外，应该意识到：m_vecTableTitle.size() - 1 = 公式中命题变元的数量
这里规定命题变元的数量不超过16个
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
成员函数：
Convert2RPN() - 将命题公式m_strPropFmlr转换为逆波兰式，
逆波兰式存储在m_strRevertPolishNotation中。
PropositionalFormular() - 默认构造函数。
~PropositionalFormular() - 析构函数。
PropositionalFormular(string formular) - 构造函数。该构造函数将
m_strPropFmlr初始化为formular，并调用Convert2RPN()将命题转换为逆波兰式
FillTableTitle() - 解析命题公式m_strPropFmlr，生成真值表表头m_vecTableTitle
例如，对于命题公式 P&Q>T，由于T是命题常元，所以该命题公式中有两个变元，所以解析后
m_vecTableTitle中应该有三个元素：P、Q、P&Q>T
EvalRPN(map<char, bool> varvals) - 计算并返回逆波兰表达式的值。其中varvals存储的是
命题公式的一种指派。例如，对于命题公式 P&Q>T，其中有两个命题变元，当P = false，
Q = true，则varvals['P'] = false，varvals['Q'] = true
GenerateTrueValueTable() - 生成并输出命题公式的真值表。以P&Q>T为例，要求输出格式如下：
P	Q	P&Q>T
0	0	1
1	0	1
0	1	1
1	1	1
注意：各列之间以制表符（'\t'）分隔。
提示：真值表的每一行的前n-1列实际是该公式的一种指派。而每一行的最后一列对应
一个逆波兰式的值，这个值是用EvalRPN(map<char, bool> varvals)来求。参数varvals中
存储的就是这个指派。
ClearOperatorStack() - 清空运算符栈。在把中缀表达式转为逆波兰式之前调用。
ClearOperandStack() - 清空操作数栈。在对逆波兰表达式求值之前调用。
CalcConjunction(bool left, bool right) - 返回left与right的合取
CalcCompatileDisjunction(bool left, bool right) - 返回left与right的析取
CalcNegation(bool right) - 返回right的否定
CalcImplication(bool left, bool right) - 返回left->right的真值


关于命题公式和命题联结词的说明：
1、每个命题变元由一个大写字母表示；
2、为简化问题，本实验只考虑否定、合取、析取、条件四种联结词。
3、每种联结词用一个字符表示，分别是：否定（'!'）、合取（'&'）、析取（'|'）、条件（'>'）。
4、命题公式中可能存在括号。具体实例可见主函数。
5、命题公式中可能存在命题常元：T和F，注意特殊处理。
6、这里规定命题变元的数量不超过16个。
注意：所有字符均为半角字符（英文输入法）。
只请不要修改现有的任何内容，只允许添加各函数的定义。如需要，可自行添加任意辅助函数。
*/

class PropositionalFormular
{
private:
	string m_strPropFmlr;					//中缀表达式
	string m_strRevertPolishNotation;		//逆波兰表达式
	stack<char> m_OperatorStack;			//运算符栈
	static map<char, int> m_PriorityMap;	//优先级映射
	stack<char> m_OperandStack;				//操作数栈
	vector<string> m_vecTableTitle;			//真值表
private:
	void Convert2RPN();						//将中缀表达式转化为逆波兰式
	void FillTableTitle();					//填充表头
	bool EvalRPN(map<char, bool> varvals);	//根据指派计算真值
public:
	PropositionalFormular();
	~PropositionalFormular() {}
	PropositionalFormular(string formular);
	void PrintPriority();									//打印优先级
	void PrintRPN();										//打印逆波兰表达式
	void ClearOperatorStack();								//清空运算符栈
	void ClearOperandStack();								//清空操作数栈
	bool CalcConjunction(bool left, bool right);			//计算合取
	bool CalcCompatileDisjunction(bool left, bool right);	//计算析取
	bool CalcNegation(bool right);							//计算否定
	bool CalcImplication(bool left, bool right);			//计算->
	void GenerateTrueValueTable();							//构造真值表,并输出

	//tools
private:
	static bool IsHighLevelThan(char value, char compared_value);
	static bool IsSymbol(char c);
	void call(const string& myset, size_t index, size_t len, map<char, bool>& varvals);
	static bool toBool(char c);
};
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
PropositionalFormular::PropositionalFormular()
{
	m_strPropFmlr = "";
	Convert2RPN();
}

PropositionalFormular::PropositionalFormular(string formular)
{
	m_strPropFmlr = formular;
	Convert2RPN();
}
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
void PropositionalFormular::PrintPriority()
{
	map<char, int>::iterator it;
	it = m_PriorityMap.begin();
	for (; it != m_PriorityMap.end(); it++)
		cout << it->first << '\t' << it->second << endl;
}



void PropositionalFormular::Convert2RPN()//此处直接复制了实验7中的Convert2RPN函数
{
	//以下依据实验报告中的内容写，* 表示是eustrain个人附加的
	this->m_strRevertPolishNotation = std::string(this->m_strPropFmlr);//*先申请足够大的空间，避免性能损失
	this->m_strRevertPolishNotation.clear();//将存储逆波兰表达式的字符串初始化为空字符串
	this->m_OperatorStack = std::stack<char>();//初始化一个空栈，用于暂存联结词（下文简称为“栈”）
	for (char c : this->m_strPropFmlr) {//读入中缀表达式，对中缀表达式进行逐字符处理
		if (c == ' ')continue;// *忽略空格
		if (!IsSymbol(c))	//如果是命题标识符
			this->m_strRevertPolishNotation.push_back(c);//直接将标识符附加到逆波兰表达式的末尾
		else {//否则
			if (this->m_OperatorStack.empty())//如果当前栈为空
				this->m_OperatorStack.push(c);//直接将联结词入栈
			else {//否则进行进一步判断
				//*此处将对")"的判断改在了开头，否则可能会把成对的空括号压入栈
				if (c == ')') {//如果当前联结词为")"
					while (this->m_OperatorStack.top() != '(') {//则将栈中所有联结词依次
						//弹出
						char top = this->m_OperatorStack.top();
						this->m_OperatorStack.pop();
						this->m_strRevertPolishNotation.push_back(top);//并附加到逆波兰表达式末尾
					}
					this->m_OperatorStack.pop();
				}//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
				else if (c == '(')//如果当前联结词为"("
					this->m_OperatorStack.push(c);//则将其直接入栈
				else if (this->m_OperatorStack.top() == '(')//如果当前栈顶元素为"("
					this->m_OperatorStack.push(c);//则将当前联结词直接入栈
				else {//否则
					char top = this->m_OperatorStack.top();
					if (this->IsHighLevelThan(c, top))//若当前联结词优先级大于栈顶元素优先级
						this->m_OperatorStack.push(c);//则将该联结词直接入栈
					else {//否则，若当前联结词优先级小于等于栈顶元素优先级
						while (!this->m_OperatorStack.empty()) {
							char top = this->m_OperatorStack.top();//则不断地将栈顶元素出栈
							if (!this->IsHighLevelThan(c, top)) {
								this->m_strRevertPolishNotation.push_back(top);//并附加到逆波兰表达式末尾
								this->m_OperatorStack.pop();
							}
							else break;//直到当前联结词优先级大于栈顶元素优先级
						}
						this->m_OperatorStack.push(c);//此时将当前联结词入栈
					}
				}
			}


		}
	}
	while (!this->m_OperatorStack.empty()) {//把栈中剩余的联结词依次
		char top = this->m_OperatorStack.top();
		this->m_OperatorStack.pop();//出栈
		this->m_strRevertPolishNotation.push_back(top);//并附加到波兰表达式末尾
	}
}

void PropositionalFormular::PrintRPN()
{
	cout << m_strRevertPolishNotation << endl;
}

void PropositionalFormular::ClearOperatorStack() {
	this->m_OperatorStack = std::stack<char>();
}
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
void PropositionalFormular::ClearOperandStack() {
	this->m_OperandStack = std::stack<char>();
}
bool PropositionalFormular::CalcConjunction(bool left, bool right) {
	return left && right;
}
bool PropositionalFormular::CalcCompatileDisjunction(bool left, bool right) {
	return left || right;
}
bool PropositionalFormular::CalcNegation(bool right) {
	return !right;
}
bool PropositionalFormular::CalcImplication(bool left, bool right) {
	return !left || right;
}
void PropositionalFormular::GenerateTrueValueTable() {
	FillTableTitle();
	int len = 0;//获取命题变元个数
	string myset = "";//因为明天变元个数不会超过16个，所以直接用字符串来表示命题变元
	auto find = [](const string& myset, char c)->bool {
		for (char member : myset) 
			if (member == c)
				return true;
		return false;
	};
	//计算命题变元个数
	for (char c : this->m_strRevertPolishNotation) {
		if (c == '!' || c == '|' || c == '&' || c == '>' || c == 'T' || c == 'F')continue;//忽略非操作数
		if (!find(myset, c)) { myset.push_back(c); ++len; }//如果命题变元集合中没有该字符，就添加进去。同时增加计数
	}
	std::map<char, bool> varvals;
	//对真值指派赋初值
	for (char c : myset)
		varvals.insert(make_pair(c, 0));
	this->call(myset, 0, len, varvals);
	//EvalRPN(varvals);

}
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
void PropositionalFormular::call(const string& myset, size_t index, size_t len, map<char, bool>& varvals) {
	if (index == len) {
		for (char c : myset) {
			auto iter = varvals.find(c);
			cout << iter->second << '\t';
		}
		cout << EvalRPN(varvals) << endl;
	}
	else {
		varvals.find(myset[index])->second = false;
		call(myset, index + 1, len, varvals);
		varvals.find(myset[index])->second = true;
		call(myset, index + 1, len, varvals);
	}
}

void PropositionalFormular::FillTableTitle() {
	char title[40];//此处因为命题变元数量不超过16个，加上'\t'后不会超过32个，但是后面要加上'\0'，所以需要至少33个字节，为了向8补齐所以申请了40字节
	int len = 0;
	auto find = [len](const char const str[], char c)->bool {
		for (int i = 0; i < len; ++i)
			if (str[i] == c)return true;
		return false;
	};
	for (char c : this->m_strPropFmlr) {
		if (this->IsSymbol(c)  || c == 'T' || c == 'F')continue;
		if (!find(title, c)) {
			title[len++] = c;
			title[len++] = '\t';
		}
	}
	title[len] = '\0';
	cout << title << this->m_strPropFmlr << endl;
}
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
bool PropositionalFormular::toBool(char c) {
	return c == 'T';
}

bool PropositionalFormular::EvalRPN(map<char, bool> varvals) {
	this->ClearOperandStack();//清空操作数栈
	auto toBool = [&varvals](char c)->bool {
		if (c == 'T')return true;
		if (c == 'F')return false;
		return varvals.find(c)->second;
	};
	auto toChar = [](bool b)->char {
		return b ? 'T' : 'F';
	};
	for (char c : this->m_strRevertPolishNotation) {
		switch (c) {
		case '!': 
		{
			char Op = this->m_OperandStack.top();
			this->m_OperandStack.pop();
			this->m_OperandStack.push(toChar(this->CalcNegation(toBool(Op))));
		}
				break;
		case '|': 
		{
			char Op1 = this->m_OperandStack.top();
			this->m_OperandStack.pop();
			char Op2 = this->m_OperandStack.top();
			this->m_OperandStack.pop();
			this->m_OperandStack.push(toChar(this->CalcCompatileDisjunction(toBool(Op1), toBool(Op2))));
		}
				break;
		case '&':
		{
			char Op1 = this->m_OperandStack.top();
			this->m_OperandStack.pop();
			char Op2 = this->m_OperandStack.top();
			this->m_OperandStack.pop();
			this->m_OperandStack.push(toChar(this->CalcConjunction(toBool(Op1), toBool(Op2))));
		}
				break;
		case '>':
		{
			char Op1 = this->m_OperandStack.top();
			this->m_OperandStack.pop();
			char Op2 = this->m_OperandStack.top();
			this->m_OperandStack.pop();
			this->m_OperandStack.push(toChar(this->CalcImplication(toBool(Op2), toBool(Op1))));
		}//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
				break;
		default:
				this->m_OperandStack.push(toChar(toBool(c)));
				break;
		}
	}
//	cout << "操作数栈大小：" << this->m_OperandStack.size() << "\t";
	return toBool(this->m_OperandStack.top());
}

map<char, int> PropositionalFormular::m_PriorityMap = map<char, int>({ { '!', 99 },{ '&', 98 },{ '|', 97 },{ '>', 96 } });

int main()
{
	PropositionalFormular pf1("P&(Q>!R)|S");
	pf1.PrintRPN();
	pf1.GenerateTrueValueTable();
	system("pause");

	PropositionalFormular pf2("P>Q|R>S");
	pf2.PrintRPN();
	pf2.GenerateTrueValueTable();
	system("pause");
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
	PropositionalFormular pf3("((P))>Q|R>S");
	pf3.PrintRPN();
	system("pause");

	PropositionalFormular pf4("P>Q|R>(S>K)");
	pf4.PrintRPN();
	system("pause");

	PropositionalFormular pf5("P&T>F");
	pf5.PrintRPN();
	pf5.GenerateTrueValueTable();
	system("pause");
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
	return 0;
}
bool PropositionalFormular::IsHighLevelThan(char value, char compared_value) {
	int l = m_PriorityMap.find(value)->second;
	int r = m_PriorityMap.find(compared_value)->second;
	return l > r;
}
bool PropositionalFormular::IsSymbol(char c) {
	return c == '!' || c == '&' || c == '|' || c == '>' || c == '(' || c == ')';
}
