#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>
using namespace std;
#define IDMAX 20
/*
学生类
要求：
1、学生有ID，ID为无符号整型，取值范围为 1 ~ IDMAX
2、完成该类的三个构造函数。默认构造函数可随机生成一个ID
3、重载学生类的赋值操作符
4、重载学生类的==操作符，要求：当两个学生对象的ID相同时，认为这两个对象相等
*/
class Student
{
private:
	int m_uiId;
public:
	Student() :Student(0) {}
	Student(int id) : m_uiId(id) {}
	Student(const Student& student) = default;
	Student& operator=(const Student& student) = default;
	bool operator==(const Student& student) const {
		return m_uiId == student.m_uiId;
	}
	bool operator!=(const Student& student) const = default;
	int GetID() const { return m_uiId; }
};


/*
学生集合类
要求：
1、集合中的元素为Student对象，用vector存储；
2、实现默认构造函数、复制构造函数
3、重载赋值操作符和==操作符
4、实现向集合中插入（Insert）和删除（Remove）操作
5、实现集合的并(Union)、交(InterSect)、差(Difference)、补(Complement)和对称差(SymDifference)
，其中补集运算以{1 ... IDMAX}为全集。并、交、差、补、对称差运算函数均不改变对象本身的数据，而是返回一个
新的对象。
6、实现空集的判断(IsEmpty)
7、实现集合的清空操作(Clear)
8、实现打印集合所有学生学号的操作(Output)。格式如下：
id1
id2
...
idn
9、不允许直接使用STL的set类
*/
class StudentSet
{
private:
	vector<Student> m_vecData;
public:
	StudentSet() = default;
	StudentSet(const StudentSet& studentset)
		:m_vecData(studentset.m_vecData) {}
	StudentSet& operator=(const StudentSet& studentset) = default;
	bool Insert(const Student student) {
		if (student.GetID() <= 0 || student.GetID() > IDMAX)
			return false;
		if (m_vecData.empty()) {
			m_vecData.push_back(student);
			return true;
		}
		size_t l = 0;
		size_t r = m_vecData.size() - 1;
		while (l < r) {
			size_t mid = (l + r) / 2;
			if (student.GetID() < m_vecData[mid].GetID())
				r = mid;
			else if (student.GetID() > m_vecData[mid].GetID())
				l = mid + 1;
			else
				return false;
		}
		if (m_vecData.size() == m_vecData.capacity())
			m_vecData.reserve(static_cast<long long>(m_vecData.capacity() * 1.5) + 1L);
		m_vecData.push_back(m_vecData[m_vecData.size() - 1]);
		for (size_t rindex = m_vecData.size() - 2; rindex > l + 1; --rindex) {
			m_vecData[rindex] = m_vecData[rindex - 1];
		}
		m_vecData[l + 1] = student;
		return true;
	}
	bool Remove(const int id) {
		size_t l = 0;
		size_t r = m_vecData.size() - 1;
		while (l < r) {
			size_t mid = l + (r - l) / 2;
			if (id < m_vecData[mid].GetID())
				r = mid;
			else if (id > m_vecData[mid].GetID())
				l = mid;
			else {
				while (mid < m_vecData.size() - 1) {
					m_vecData[mid] = m_vecData[mid + 1];
					++mid;
				}
				m_vecData.pop_back();
				return true;
			}
		}
		return false;
	}
	StudentSet Union(const StudentSet& studentset) const {	//并
		size_t id = 0;
		StudentSet ret;
		int index1 = 0, index2 = 0;
		vector<Student>::const_iterator iter1 = this->m_vecData.cbegin();
		vector<Student>::const_iterator iter2 = studentset.m_vecData.cbegin();
		while (iter1 != this->m_vecData.cend() && iter2 != studentset.m_vecData.cend()) {
			if (iter1->GetID() < iter2->GetID()) {
				id = iter1->GetID();
				++iter1;
			}
			else if (iter1->GetID() > iter2->GetID()) {
				id = iter2->GetID();
				++iter2;
			}
			else {
				id = iter1->GetID();
				++iter1;
				++iter2;
			}
			ret.m_vecData.emplace_back(id);
		}
		if (iter1 != this->m_vecData.cend()) {
			ret.m_vecData.insert(ret.m_vecData.end(), iter1, this->m_vecData.cend());
		}
		if (iter2 != studentset.m_vecData.cend()) {
			ret.m_vecData.insert(ret.m_vecData.end(), iter2, studentset.m_vecData.cend());
		}
		return ret;
	}
	StudentSet InterSect(const StudentSet& studentset) const {	//交
		size_t id = 0;
		StudentSet ret;
		int index1 = 0, index2 = 0;
		vector<Student>::const_iterator iter1 = this->m_vecData.cbegin();
		vector<Student>::const_iterator iter2 = studentset.m_vecData.cbegin();
		while (iter1 != this->m_vecData.cend() && iter2 != studentset.m_vecData.cend()) {
			if (iter1->GetID() < iter2->GetID()) {
				++iter1;
			}
			else if (iter1->GetID() > iter2->GetID()) {
				++iter2;
			}
			else {
				id = iter1->GetID();
				++iter1;
				++iter2;
				ret.m_vecData.emplace_back(id);
			}
		}
		return ret;
	}
	StudentSet Difference(const StudentSet& studentset) const {	
 		size_t id = 0;
		StudentSet ret;
		int index1 = 0, index2 = 0;
		vector<Student>::const_iterator iter1 = this->m_vecData.cbegin();
		vector<Student>::const_iterator iter2 = studentset.m_vecData.cbegin();
		while (iter1 != this->m_vecData.cend() && iter2 != studentset.m_vecData.cend()) {
			if (iter1->GetID() < iter2->GetID()) {
				id = iter1->GetID();
				++iter1;
				ret.m_vecData.emplace_back(id);
			}
			else if (iter1->GetID() > iter2->GetID()) {
				id = iter2->GetID();
				++iter2;
			}
			else {
				id = iter1->GetID();
				++iter1;
				++iter2;
			}
		}
		if(iter1 != this->m_vecData.cend()) 
			ret.m_vecData.insert(ret.m_vecData.end(), iter1, this->m_vecData.cend());
		return ret;
	}
	StudentSet Complement() const {
		size_t id = 1;
		StudentSet ret;
		vector<Student>::const_iterator iter = m_vecData.begin();
		while (id <= IDMAX) {
			if (iter!= m_vecData.cend() && id == iter->GetID())
				++iter;
			else
				ret.m_vecData.emplace_back(id);
			++id;
		}
		return ret;
	}
	StudentSet SymDifference(const StudentSet& studentset) const {
		size_t id = 0;
		StudentSet ret;
		int index1 = 0, index2 = 0;
		vector<Student>::const_iterator iter1 = this->m_vecData.cbegin();
		vector<Student>::const_iterator iter2 = studentset.m_vecData.cbegin();
		while (iter1 != this->m_vecData.cend() && iter2 != studentset.m_vecData.cend()) {
			if (iter1->GetID() < iter2->GetID()) {
				id = iter1->GetID();
				++iter1;
				ret.m_vecData.emplace_back(id);
			}
			else if (iter1->GetID() > iter2->GetID()) {
				id = iter2->GetID();
				++iter2;
				ret.m_vecData.emplace_back(id);
			}
			else {
				id = iter1->GetID();
				++iter1;
				++iter2;
			}
		}
		if (iter1 != this->m_vecData.cend()) {
			ret.m_vecData.insert(ret.m_vecData.end(), iter1, this->m_vecData.cend());
		}
		if (iter2 != studentset.m_vecData.cend()) {
			ret.m_vecData.insert(ret.m_vecData.end(), iter2, studentset.m_vecData.cend());
		}
		return ret;
	}
	bool IsEmpty() const{
		return m_vecData.empty();
	}
	void Clear() {
		m_vecData.clear();
	}
	void Output() const {
		for (auto& stu : m_vecData)
			cout << stu.GetID() << std::endl;
	}
};

int main()
{
	srand((unsigned int)time(NULL));
	StudentSet set1, set2, set3;
	set1.Clear();
	set2.Clear();
	set3.Clear();
	for (unsigned int i = 0; i < 5; i++)
	{
		Student tmpstudent(i);
		set1.Insert(tmpstudent);
	}
	set1.Output();
	system("pause");
	Student tmpstudent1(3);
	cout << set1.Insert(tmpstudent1) << endl;
	system("pause");
	Student tmpstudent2(10);
	cout << set1.Insert(tmpstudent2) << endl;
	system("pause");
	for (unsigned int i = 7; i < 12; i++)
	{
		Student tmpstudent(i);
		set2.Insert(tmpstudent);
	}
	cout << "set1:" << endl;
	set1.Output();
	cout << "set2:" << endl;
	set2.Output();
	system("pause");
	set3 = set1.Union(set2);
	set3.Output();
	system("pause");
	set3 = set1.InterSect(set2);
	set3.Output();
	system("pause");
	set3 = set1.Difference(set2);
	set3.Output();
	system("pause");
	set3 = set1.Complement();
	set3.Output();
	system("pause");
	set3 = set1.SymDifference(set2);
	set3.Output();
	system("pause");

	return 0;
}
