#include <iostream>
#include<fstream>
#include <sstream>

using namespace std;

struct Node {
	string username;
	string password;
	Node* next;
};

struct LoginList {
	Node* head = nullptr;
	Node* tail = nullptr;
};

Node* create1(string str) {
	Node* p = new Node;
	stringstream ss(str);
	getline(ss, p->username, ',');
	ss >> p->password;
	p->next = nullptr;
	return p;
}


void getLoginData(LoginList& l, string file_name) {
	ifstream fin(file_name, ios_base::in);
	string temp;
	while (!fin.eof()) {
		getline(fin, temp);
		//Node* g = create1(temp);
		if (l.head == nullptr) {
			l.head = create1(temp);
			l.tail = l.head;
			l.tail->next = nullptr;
		}
		else {
			l.tail->next = create1(temp);
			l.tail = l.tail->next;
			l.tail->next = nullptr;
		}
	}
	fin.close();
}

bool checkAvailableUsername(LoginList& l, string user) {
	Node* cur = l.head;
	if (cur == nullptr) return true;
	while (cur) {
		if (user == cur->username) {
			return false;
			break;
		}
		cur = cur->next;
	}
	return true;
}

Node* create2(string username, string password) {
	Node* p = new Node;
	p->username = username;
	p->password = password;
	p->next = nullptr;
	return p;
}

void UserReg(LoginList& l, string user, string pass) {
	if (l.head == nullptr) {
		l.head = create2(user, pass);
		l.tail = l.head;
		l.tail->next = nullptr;
	}
	else {
		l.tail->next = create2(user, pass);
		l.tail = l.tail->next;
		l.tail->next = nullptr;
	}
}

bool checkCorrect(LoginList& l, string user, string pass)
{
	Node* cur = l.head;
	if (cur == nullptr) return false;
	while (cur) {
		if (user == cur->username && pass == cur->password) {
			return true;
			break;
		}
		cur = cur->next;
	}
	return false;
}