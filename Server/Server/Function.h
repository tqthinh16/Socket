#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

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

//
#include <windows.h>
#include <vector>
#include <locale>
#include <ctime>
#include <ctype.h>
#include "json/json.h"
//

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

	Country* next = nullptr;
};

struct CountryList {
	Country* head = nullptr;
	Country* tail = nullptr;
};

struct time_mark {
	int hour;
	int date;
	int month;
	int year;
};


Client* create1(string);
void getLoginData(ClientList&, string);
bool checkAvailableUsername(ClientList&, string);
Client* create2(string, string);
void UserReg(ClientList&, string, string);
bool checkCorrect(ClientList&, string, string);
Country* create3(string);
void getCountryData(CountryList&, string);
Country* findCountry(CountryList&, string);

string DateTodayString();
Json::Value decode(string);
Json::Value ConvertToJson(string, string&);
void WebToFile();

time_mark getTime_mark();
bool greaterTime(time_mark, time_mark);

void deleteList(CountryList&);





