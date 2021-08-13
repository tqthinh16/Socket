#ifndef Function_h_
#define Funciotn_h_
#endif // !Function_h_

#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0A00

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include<iostream>
#include<string.h>
#include<WinSock2.h>
#include<winsock.h>
#include<stdlib.h>
#include<sys/types.h>
#include <WS2tcpip.h>
#include<sstream>
#include<ctime>
#include <locale>
#include <Windows.h>

using namespace std;

string DateTodayString();
string getpass();
void GotoXY(int x, int y);
void FixConsoleWindow();

