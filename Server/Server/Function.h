#ifndef Function_h_
#define Function_h
#endif // !Function_h_

#include<iostream>
#include<string.h>
#include<WinSock2.h>
#include<winsock.h>
#include<stdlib.h>
#include<sys/types.h>
#include<time.h>
#include<fstream>
#include<sstream>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

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

struct Country {
	string name;
	int Cases;
	int today_Cases;
	int uTreatment;
	int Recovery;
	int Death;
	int Today_Deaths;
	int Other;

	Country* next = nullptr;
};

struct CountryList {
	Country* head = nullptr;
	Country* tail = nullptr;
};


Client* create1(string str);
void getLoginData(ClientList& l, string file_name);
bool checkAvailableUsername(ClientList& l, string user);
Client* create2(string username, string password);
void UserReg(ClientList& l, string user, string pass);
bool checkCorrect(ClientList& l, string user, string pass);
Country* create3(string str);
void getCountryData(CountryList& l, string file_name);
Country* findCountry(CountryList& l, string name);

