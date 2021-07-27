#ifndef Function_h_
#define Function_h
#endif // !Function_h_


#include <iostream>
#include<fstream>
#include<sstream>
using namespace std;


struct Node {
	string username;
	string password;
	Node* next;
};

struct LoginList {
	Node* head;
	Node* tail;
};

Node* create1(string str);
void getLoginData(LoginList& l, string file_name);
bool checkAvailableUsername(LoginList& l, string user);
Node* create2(string username, string password);
void UserReg(LoginList& l, string user, string pass);
bool checkCorrect(LoginList& l, string user, string pass);
