#ifndef Function_h_
#define Function_h
#endif // !Function_h_


#include <iostream>
#include<fstream>
#include<sstream>
using namespace std;


struct Client {
	string username;
	string password;
	Client* next;
};

struct ClientList {
	Client* head = nullptr;
	Client* tail = nullptr;
};

struct Province {
	string name;
	int Cases;
	int uTreatment;
	int Recovery;
	int Death;
	int Other;
	Province* next = nullptr;
};

struct ProvinceList {
	Province* head = nullptr;
	Province* tail = nullptr;
};


Client* create1(string str);
void getLoginData(ClientList& l, string file_name);
bool checkAvailableUsername(ClientList& l, string user);
Client* create2(string username, string password);
void UserReg(ClientList& l, string user, string pass);
bool checkCorrect(ClientList& l, string user, string pass);
Province* create3(string str);
void getProvinceData(ProvinceList& l, string file_name);
Province* findProvince(ProvinceList& l, string name);

