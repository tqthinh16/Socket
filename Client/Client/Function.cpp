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
#include"Function.h"

using namespace std;

string DateTodayString() {
	time_t t = time(0);   // get time now
	struct tm* now = localtime(&t);
	string res;
	if (now->tm_mday < 10) res += "0";
	res += to_string(now->tm_mday);
	res += "-";

	if (now->tm_mon < 9) res += "0";
	res += to_string(now->tm_mon + 1);

	res += "-";

	res += to_string(now->tm_year + 1900);
	return res;
}

string getpass()
{
	const char BACKSPACE = 8;
	const char RETURN = 13;

	string password;
	unsigned char ch = 0;


	DWORD con_mode = 0;
	DWORD dwRead;
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

	GetConsoleMode(hIn, &con_mode);
	SetConsoleMode(hIn, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

	while (ReadConsoleA(hIn, &ch, 1, &dwRead, NULL) && ch != RETURN)
	{
		if (ch == BACKSPACE)
		{
			if (password.length() != 0)
			{

				cout << "\b \b";
				password.resize(password.length() - 1);
			}
		}
		else
		{
			password += ch;
			cout << '*';
		}
	}
	cout << endl;
	SetConsoleMode(hIn, con_mode);
	return password;
}

void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
