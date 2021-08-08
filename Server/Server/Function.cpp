#include <iostream>
#include<fstream>
#include <sstream>
#include"Function.h"

using namespace std;

Client* create1(string str) {
	Client* p = new Client;
	stringstream ss(str);
	getline(ss, p->username, ',');
	ss >> p->password;
	p->next = nullptr;
	return p;
}


void getLoginData(ClientList& l, string file_name) {
	ifstream fin(file_name, ios_base::in);
	string temp;
	while (!fin.eof()) {
		getline(fin, temp);
		//Client* g = create1(temp);
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

bool checkAvailableUsername(ClientList& l, string user) {
	Client* cur = l.head;
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

Client* create2(string username, string password) {
	Client* p = new Client;
	p->username = username;
	p->password = password;
	p->next = nullptr;
	return p;
}

void UserReg(ClientList& l, string user, string pass) {
	fstream fout("input.txt", ios_base::app);
	Client* p = create2(user, pass);
	if (l.head == nullptr) {
		l.head = p;
		l.tail = l.head;
		l.tail->next = nullptr;
	}
	else {
		l.tail->next = p;
		l.tail = l.tail->next;
		l.tail->next = nullptr;
	}
	fout << "\n" << p->username << "," << p->password;

	fout.close();
}

bool checkCorrect(ClientList& l, string user, string pass)
{
	Client* cur = l.head;
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

Country* create3(string str) {
	Country* p = new Country;
	stringstream ss(str);
	string temp;

	getline(ss, p->name, ',');

	getline(ss, temp, ',');
	p->Cases = stoi(temp);

	getline(ss, temp, ',');
	p->uTreatment = stoi(temp);

	getline(ss, temp, ',');
	p->Other = stoi(temp);

	getline(ss, temp, ',');
	p->Recovery = stoi(temp);

	getline(ss, temp);
	p->Death = stoi(temp);

	p->next = nullptr;
	return p;
}

void getCountryData(CountryList& l, string file_name) {
	ifstream fin(file_name, ios_base::in);
	string temp;
	while (!fin.eof()) {
		getline(fin, temp);
		//Client* g = create1(temp);
		if (l.head == nullptr) {
			l.head = create3(temp);
			l.tail = l.head;
			l.tail->next = nullptr;
		}
		else {
			l.tail->next = create3(temp);
			l.tail = l.tail->next;
			l.tail->next = nullptr;
		}
	}
	fin.close();
}

Country* findCountry(CountryList& l, string name) {
	if (l.head == nullptr) {
		cout << "not found" << endl;
		return nullptr;
	}

	Country* pcur = l.head;
	while (pcur) {
		if (pcur->name == name) {
			//cout << pcur->name << " " << pcur->Cases << " " << pcur->uTreatment << " " << pcur->Other << " " << pcur->Recovery << " " << pcur->Death << endl;
			return pcur;
			break;
		}
		pcur = pcur->next;
	}

	//if (pcur == nullptr) return nullptr;
}
