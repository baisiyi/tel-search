#include<iostream>
#include<string>
#include<fstream>
#define max 20
#define END '\0'
#define na 2//�����ַ�ʽ����ָʾ��Ϊ2
#define t 1//�Ե绰��ʽ����ָʾ��Ϊ1
using namespace std;
typedef struct telephoneDirectory {
	bool is_head;
	string tel;//�绰����
	string name;//����
	string addr;//��ַ
	struct telephoneDirectory *next;
}teldir;
teldir*table_name[max];//������Ϊ�ؼ����hash��
teldir*table_tel[max];//�Ե绰����Ϊ�ؼ����hash��
teldir table1_tel[max];//��ɢ�е��Ե绰����Ϊ�ؼ����hash��
teldir table1_name[max];//��ɢ�е�������Ϊ�ؼ����hash��
teldir h;//��������ṹ��
void add() {//�������븨���ṹ��
	cout << "������������";
	cin >> h.name;
	cout << "�������ֻ����룺";
	cin >> h.tel;
	cout << "�������ַ��";
	cin >> h.addr;
}
int hash_t(teldir h, int n) {//�Ե绰����Ϊ�ؼ��룬��λ��ӵĺ�%9
	if (n == 1) {//n=1ʱΪ�绰Ϊ�ؼ���
		int sum = 0, i = 0;
		string num = h.tel;
		while (num[i] != END) {
			sum = sum + (num[i] - '0');
			i++;
		}
		sum = sum % 19;
		return sum;//����ֵΪ0-18
	}
	else if (n == 2) {//n=2ʱΪ����Ϊ�ؼ���
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
teldir*find(int key, teldir h, int n) {//����key����λ�ò�����ָ���λ�õ�ָ��
	if (n == 1) {//n=1ʱΪ�绰Ϊ�ؼ���
		int i = 1;
		teldir*node = table_tel[key];//ͷ��㲻Ϊ��,��ʵ������
		for (; node->next->is_head != true; node = node->next, i++) {
			if (node->tel == h.tel) return node;
		}
		return node;
	}
	else if (n == 2) {//n=2ʱΪ����Ϊ�ؼ���
		teldir*node = table_name[key];//ͷ��㲻Ϊ��,��ʵ������
		for (; node->next->is_head != true; node = node->next) {
			if (node->name == h.name) return node;
		}
		return node;
	}
}
void find1(int key, teldir h, int n) {
	if (n == t) {//n=1ʱΪ�绰Ϊ�ؼ���
		while (key <= max) {
			if (key == max)key = 0;
			else if (table1_tel[key].tel == h.tel) {
				cout << "�ڱ�ɢ�б���key=" << key << "�����ҵ�����ϢΪ��" << endl;
				cout << table1_tel[key].name << endl;
				cout << table1_tel[key].tel << endl;
				cout << table1_tel[key].addr << endl;
				return;
			}
			else key++;
		}
	}
	else if (n == na) {//n=2ʱΪ����Ϊ�ؼ���
		while (key <= max) {
			if (key == max)key = 0;
			else if (table1_name[key].name == h.name) {
				cout << "�ڱ�ɢ�б���key=" << key << "���ҵ�����ϢΪ��" << endl;
				cout << table1_name[key].name << endl;
				cout << table1_name[key].tel << endl;
				cout << table1_name[key].addr << endl;
				return;
			}
			else key++;
		}
	}
	cout << "û���ҵ��ó�Ա" << endl;
}
void index(int key, teldir h, int n) {//����ؼ���λ�õ�ĩβ
	teldir*newnode = new teldir;
	teldir*tilenode = NULL;
	newnode->addr = h.addr;
	newnode->name = h.name;
	newnode->tel = h.tel;
	newnode->is_head = false;
	if (n == t) {
		tilenode = find(key, h, t);
		newnode->next = table_tel[key];//�γ�ѭ��
	}//n=1ʱΪ�绰Ϊ�ؼ���
	else if (n == na) {
		tilenode = find(key, h, na);
		newnode->next = table_name[key];//�γ�ѭ��
	}//n=2ʱΪ����Ϊ�ؼ���
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
				table1_name[key].is_head = false;//���Ϊ���
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
	for (int i = 0; i < max; i++) { //��ʼ��,�γ�ѭ������
		table_tel[i] = new teldir;
		table_tel[i]->is_head = true;
		table_tel[i]->next = table_tel[i];
	}
	for (int i = 0; i < max; i++) { //��ʼ��,�γ�ѭ������
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
	cout << "		���Ʋ˵�" << endl;
	cout << "����1������Ϣ" << endl;
	cout << "����2����������" << endl;
	cout << "����3���绰�������" << endl;
	cout << "����4����" << endl;
	cout << "�������֣�";
	while (cin >> n) {
		switch (n) {
		case(1): {//������Ϣ
			add();
			int key1 = hash_t(h, t);
			int key2 = hash_t(h, na);
			index(key1, h, t);
			index1(key1, h, t);
			index(key2, h, na);
			index1(key2, h, na);
			break;
		}
		case(2): {//����������
			cout << "������ҵ����֣�";
			cin >> h.name;
			int key = hash_t(h, na);
			teldir*H = find(key, h, na);//�ڿ�ɢ�б��в���
			if (H != NULL) {
				cout << "�ڿ�ɢ�б���key=" << key << "���ҵ�����ϢΪ��" << endl;
				cout << H->name << endl;
				cout << H->tel << endl;
				cout << H->addr << endl;
			}
			else cout << "�ڿ�ɢ�б���û�в��ҵ��ó�Ա" << endl;
			find1(key, h, na);//�ڱ�ɢ�б��в���
			break;
		}
		case(3): {
			cout << "������ҵĺ��룺";
			cin >> h.tel;
			int key = hash_t(h, t);
			teldir*H = find(key, h, t);
			if (H != NULL) {
				cout << "�ڿ�ɢ�б���key=" << key << "���ҵ�����ϢΪ��" << endl;
				cout << H->name << endl;
				cout << H->tel << endl;
				cout << H->addr << endl;
			}
			else cout << "�ڿ�ɢ�б���û�в��ҵ��ó�Ա" << endl;
			find1(key, h, t);
			break;
		}
		case(4): return 0;
		default: cout << "�������" << endl;
		}
		cout << "�������֣�";
	}
	system("pause");
	return 0;
}