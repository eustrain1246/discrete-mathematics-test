// P07.cpp : Defines the entry point for the console application.
//

//软件19-6 李兴鑫
//1914010612
#include "stdafx.h"
#include <string>
#include <queue>
#include <stack>
#include <map>
#include <iostream>
using namespace std;
/*
PropositionalFormular类：

成员变量：
m_strPropFmlr - 命题公式
m_strRevertPolishNotation - 命题公式对应的逆波兰表达式
m_OperatorStack - 用来存储联结词的栈
m_PriorityMap - 静态成员。这是一个<char, int>的map，用联结词作为key，
用联结词的优先级做为value。value值越大，表示优先级越高。
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
成员函数：
Convert2RPN() - 将命题公式m_strPropFmlr转换为逆波兰式，
逆波兰式存储在m_strRevertPolishNotation中。
PropositionalFormular() - 默认构造函数。
~PropositionalFormular() - 析构函数。
PropositionalFormular(string formular) - 构造函数。该构造函数将
m_strPropFmlr初始化为formular，并调用Convert2RPN()将命题转换为逆波兰式

关于命题公式和命题联结词的说明：
1、每个命题变元由一个大写字母表示；
2、为简化问题，本实验只考虑否定、合取、析取、条件四种联结词。
3、每种联结词用一个字符表示，分别是：否定（'!'）、合取（'&'）、析取（'|'）、条件（'>'）。
4、命题公式中可能存在括号。具体实例可见主函数。
注意：所有字符均为半角字符（英文输入法）。
只请不要修改现有的任何内容，只允许添加各函数的定义。如需要，可自行添加任意辅助函数。
*/

//类作用，将中缀表达式转化为后缀表达式（逆波兰表达式）
class PropositionalFormular 
{
private:
	string m_strPropFmlr;//命题公式
	string m_strRevertPolishNotation;//逆波兰表达式
	stack<char> m_OperatorStack;//联结词栈
	static map<char, int> m_PriorityMap;//优先级映射
private:
	void Convert2RPN();//将命题公式m_strPropFmlr转换为逆波兰式，这个函数被构造函数调用
public:
	PropositionalFormular();//默认构造函数
	~PropositionalFormular() {}//析构函数
	PropositionalFormular(string formular);//带参构造函数
	void PrintPriority();//打印优先级,已定义
	void PrintRPN();//打印逆波兰式，已定义

	//tools
private:
	bool IsHighLevelThan(char value, char compared_value)const;
	bool IsSymbol(char c)const;


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

void PropositionalFormular::PrintPriority()
{
	map<char, int>::iterator it;
	it = m_PriorityMap.begin();
	for (; it != m_PriorityMap.end(); it++)
		cout << it->first << '\t' << it->second << endl;
}

void PropositionalFormular::Convert2RPN()
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
			if (c != '(' && c != ')') {//如果当前读入字符为联结词，则分情况进行不同处理
				if (this->m_OperatorStack.empty())//如果当前栈为空
					this->m_OperatorStack.push(c);//直接将联结词入栈
				else {//否则进行进一步判断
					if (this->m_OperatorStack.top() == '(')//如果当前栈顶元素为"("
						this->m_OperatorStack.push(c);//则将当前联结词直接入栈
					else if (c == '(')//如果当前联结词为"("
						this->m_OperatorStack.push(c);//则将其直接入栈
					else if (c == ')') {//若当前联结词为")"
						if (this->m_OperatorStack.top() != '(')//*如果为空括号就放弃
							this->m_OperatorStack.pop();
						else
							while (true) {//则将栈中所有联结词依次
								//弹出
								char top = this->m_OperatorStack.top();
								this->m_OperatorStack.pop();
								this->m_strRevertPolishNotation.push_back(top);//并附加到逆波兰表达式末尾
								if (this->m_OperatorStack.top() == '(') {//直到栈顶元素为"("
									this->m_OperatorStack.pop();//此时把"("出栈
									break;
								}
							}
					}//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
					else {//否则
						char top = this->m_OperatorStack.top();
						if (this->IsHighLevelThan(c, top))//若当前联结词优先级大于栈顶元素优先级
							this->m_OperatorStack.push(c);//则将该联结词直接入栈
						else//否则，若当前联结词优先级小于等于栈顶元素优先级
							while (!this->m_OperatorStack.empty()) {
								char top = this->m_OperatorStack.top();//则不断地将栈顶元素出栈
								if (!this->IsHighLevelThan(c, top)) {
									this->m_strRevertPolishNotation.push_back(top);//并附加到逆波兰表达式末尾
									this->m_OperatorStack.pop();
								}
								else {//直到当前联结词优先级大于栈顶元素优先级
									this->m_OperatorStack.push(c);
									break;
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
	}
}
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
void PropositionalFormular::PrintRPN()
{
	cout << m_strRevertPolishNotation << endl;
}

map<char, int> PropositionalFormular::m_PriorityMap = map<char, int>({ { '!', 99 },{ '&', 98 },{ '|', 97 },{ '>', 96 } });
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
int main()
{
	PropositionalFormular pf1("P&(Q>!R)|S");
	pf1.PrintPriority();
	system("pause");
	pf1.PrintRPN();
	system("pause");

	PropositionalFormular pf2("P>Q|R>S");
	pf2.PrintRPN();
	system("pause");

	PropositionalFormular pf3("((P))>Q|R>S");
	pf3.PrintRPN();
	system("pause");
//本文作者：哈尔滨理工大学软件与微电子学院 李兴鑫 未经允许 禁止盗用
	PropositionalFormular pf4("P>Q|R>(S>K)");
	pf4.PrintRPN();
	system("pause");
	return 0;
}
bool PropositionalFormular::IsHighLevelThan(char value, char compared_value)const {
	int l = m_PriorityMap.find(value)->second;
	int r = m_PriorityMap.find(compared_value)->second;
	return l > r;
}
bool PropositionalFormular::IsSymbol(char c)const {
	return c == '!' || c == '&' || c == '|' || c == '>' || c == '(' || c == ')';
}
