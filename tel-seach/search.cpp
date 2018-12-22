#include<iostream>
#include<string>
#include<fstream>
#define max 20
#define END '\0'
#define na 2//以名字方式操作指示码为2
#define t 1//以电话方式操作指示码为1
using namespace std;
typedef struct telephoneDirectory {
	bool is_head;
	string tel;//电话号码
	string name;//姓名
	string addr;//地址
	struct telephoneDirectory *next;
}teldir;
teldir*table_name[max];//以姓名为关键码的hash表
teldir*table_tel[max];//以电话号码为关键码的hash表
teldir table1_tel[max];//闭散列的以电话号码为关键码的hash表
teldir table1_name[max];//闭散列的以姓名为关键码的hash表
teldir h;//辅助输入结构体
void add() {//数据填入辅助结构体
	cout << "请输入姓名：";
	cin >> h.name;
	cout << "请输入手机号码：";
	cin >> h.tel;
	cout << "请输入地址：";
	cin >> h.addr;
}
int hash_t(teldir h, int n) {//以电话号码为关键码，各位相加的和%9
	if (n == 1) {//n=1时为电话为关键码
		int sum = 0, i = 0;
		string num = h.tel;
		while (num[i] != END) {
			sum = sum + (num[i] - '0');
			i++;
		}
		sum = sum % 19;
		return sum;//返回值为0-18
	}
	else if (n == 2) {//n=2时为姓名为关键码
		int i = 1;
		int sum = 0;
		int seed = 133;
		while ((h.name[i]) != END)
		{
			sum = sum * seed + (h.name[i++]);
		}
		sum = (sum < 0) ? (!sum) : sum;
		sum = sum % 19;
		return sum;
	}
}
teldir*find(int key, teldir h, int n) {//查找key所在位置并返回指向该位置的指针
	if (n == 1) {//n=1时为电话为关键码
		int i = 1;
		teldir*node = table_tel[key];//头结点不为空,无实际意义
		for (; node->next->is_head != true; node = node->next, i++) {
			if (node->tel == h.tel) return node;
		}
		return node;
	}
	else if (n == 2) {//n=2时为姓名为关键码
		teldir*node = table_name[key];//头结点不为空,无实际意义
		for (; node->next->is_head != true; node = node->next) {
			if (node->name == h.name) return node;
		}
		return node;
	}
}
void find1(int key, teldir h, int n) {
	if (n == t) {//n=1时为电话为关键码
		while (key <= max) {
			if (key == max)key = 0;
			else if (table1_tel[key].tel == h.tel) {
				cout << "在闭散列表中key=" << key << "处查找到的信息为：" << endl;
				cout << table1_tel[key].name << endl;
				cout << table1_tel[key].tel << endl;
				cout << table1_tel[key].addr << endl;
				return;
			}
			else key++;
		}
	}
	else if (n == na) {//n=2时为姓名为关键码
		while (key <= max) {
			if (key == max)key = 0;
			else if (table1_name[key].name == h.name) {
				cout << "在闭散列表中key=" << key << "查找到的信息为：" << endl;
				cout << table1_name[key].name << endl;
				cout << table1_name[key].tel << endl;
				cout << table1_name[key].addr << endl;
				return;
			}
			else key++;
		}
	}
	cout << "没有找到该成员" << endl;
}
void index(int key, teldir h, int n) {//插入关键码位置的末尾
	teldir*newnode = new teldir;
	teldir*tilenode = NULL;
	newnode->addr = h.addr;
	newnode->name = h.name;
	newnode->tel = h.tel;
	newnode->is_head = false;
	if (n == t) {
		tilenode = find(key, h, t);
		newnode->next = table_tel[key];//形成循环
	}//n=1时为电话为关键码
	else if (n == na) {
		tilenode = find(key, h, na);
		newnode->next = table_name[key];//形成循环
	}//n=2时为姓名为关键码
	tilenode->next = newnode;
}
void index1(int key, teldir h, int n) {
	if (n == t) {
		while (key <= max) {
			if (key == max) key = 0;
			else if (table1_tel[key].is_head == true) {
				table1_tel[key].name = h.name;
				table1_tel[key].tel = h.tel;
				table1_tel[key].addr = h.addr;
				table1_tel[key].is_head = false;
				break;
			}
			else  key++;
		}
	}
	else if (n == na) {
		while (key <= max) {
			if (key == max)key = 0;
			else if (table1_name[key].is_head == true) {
				table1_name[key].name = h.name;
				table1_name[key].tel = h.tel;
				table1_name[key].addr = h.addr;
				table1_name[key].is_head = false;//标记为填过
				break;
			}
			else  key++;
		}
	}
}
void fileread() {
	ifstream r("table.txt");
	if (r.is_open()) {
		while (!r.eof()) {
			r >> h.name >> h.tel >> h.addr;
			int key1 = hash_t(h, t);
			int key2 = hash_t(h, na);
			index(key1, h, t);
			index(key2, h, na);
			index1(key1, h, t);
			index1(key2, h, na);
		}
	}
	r.close();
}
void firstset() {
	for (int i = 0; i < max; i++) { //初始化,形成循环链表。
		table_tel[i] = new teldir;
		table_tel[i]->is_head = true;
		table_tel[i]->next = table_tel[i];
	}
	for (int i = 0; i < max; i++) { //初始化,形成循环链表。
		table_name[i] = new teldir;
		table_name[i]->is_head = true;
		table_name[i]->next = table_name[i];
	}
	for (int i = 0; i < max; i++) table1_tel[i].is_head = true;
	for (int i = 0; i < max; i++) table1_name[i].is_head = true;
	fileread();
}
int main() {
	firstset();
	int n;
	cout << "		控制菜单" << endl;
	cout << "输入1增添信息" << endl;
	cout << "输入2按姓名查找" << endl;
	cout << "输入3按电话号码查找" << endl;
	cout << "输入4结束" << endl;
	cout << "输入数字：";
	while (cin >> n) {
		switch (n) {
		case(1): {//增加信息
			add();
			int key1 = hash_t(h, t);
			int key2 = hash_t(h, na);
			index(key1, h, t);
			index1(key1, h, t);
			index(key2, h, na);
			index1(key2, h, na);
			break;
		}
		case(2): {//按姓名查找
			cout << "输入查找的名字：";
			cin >> h.name;
			int key = hash_t(h, na);
			teldir*H = find(key, h, na);//在开散列表中查找
			if (H != NULL) {
				cout << "在开散列表中key=" << key << "查找到的信息为：" << endl;
				cout << H->name << endl;
				cout << H->tel << endl;
				cout << H->addr << endl;
			}
			else cout << "在开散列表中没有查找到该成员" << endl;
			find1(key, h, na);//在闭散列表中查找
			break;
		}
		case(3): {
			cout << "输入查找的号码：";
			cin >> h.tel;
			int key = hash_t(h, t);
			teldir*H = find(key, h, t);
			if (H != NULL) {
				cout << "在开散列表中key=" << key << "查找到的信息为：" << endl;
				cout << H->name << endl;
				cout << H->tel << endl;
				cout << H->addr << endl;
			}
			else cout << "在开散列表中没有查找到该成员" << endl;
			find1(key, h, t);
			break;
		}
		case(4): return 0;
		default: cout << "输入错误" << endl;
		}
		cout << "输入数字：";
	}
	system("pause");
	return 0;
}