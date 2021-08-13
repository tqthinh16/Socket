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

int main() {
	int sock, valread;
	struct sockaddr_in address;
	int port = 8080;
	char buffer[1024] = { 0 };
	string IP, username, password, password1, input, clientInput, option;
	string date = DateTodayString();

	//FixConsoleWindow();

	GotoXY(5, 2);
	cout << "Enter IP: ";
	getline(cin, IP);

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		exit(1);
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "Socket failed" << endl;
		exit(1);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, IP.c_str(), &address.sin_addr);

	if (connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
		cout << "Connect failed" << endl;
		exit(1);
	}

MENU:
	system("cls");
	GotoXY(0, 1);
	cout << "------MENU------" << endl << endl << "1. Login" << endl << "2. Sign up" << endl << "0. Exit" << endl << endl << "----------------" << endl << "Choose function: ";
	
	getline(cin, input);
	input = date + ":" + input;

	send(sock, input.c_str(), input.size() + 1, 0);

	if (input[11] == '1') {

	TRYAGAIN1:
		system("cls");

		GotoXY(0, 1);
		cout << "------LOGIN------" << endl << endl;
		cout << "Username: ";
		getline(cin, username);
		cout << "password: ";
		password = getpass();
		//getline(cin, password);
		send(sock, username.c_str(), username.size() + 1, 0);
		send(sock, password.c_str(), password.size() + 1, 0);

		int re = recv(sock, buffer, 1024, 0);

		if (strcmp(buffer, "failed") == 0) {
			cout << endl << "Wrong username or password, pls try again!" << endl;
			system("pause");
			goto TRYAGAIN1;
		}
		else if (re == SOCKET_ERROR) {
			cout << endl << "Server closed! Client Disconnected..." << endl;
			exit(1);
		}
	}

	else if (input[11] == '2') {
	TRYAGAIN2:
		system("cls");

		GotoXY(0, 1);
		cout << "------REGISTER------" << endl;
		cout << "Username: ";
		getline(cin, username);
		cout << "Password: ";
		getline(cin, password);
		cout << "Confirm password: ";
		getline(cin, password1);

		send(sock, username.c_str(), username.size() + 1, 0);
		send(sock, password.c_str(), password.size() + 1, 0);
		send(sock, password1.c_str(), password.size() + 1, 0);

		int re2 = recv(sock, buffer, 1024, 0);

		if (strcmp(buffer, "confirm") == 0) {
			cout << endl << "Wrong confirm password, pls try again!" << endl;
			system("pause");
			goto TRYAGAIN2;
		}

		else if (strcmp(buffer, "failed") == 0) {
			cout << endl << "Username is not available, pls try again!" << endl;
			system("pause");
			goto TRYAGAIN2;
		}
		else if (re2 == SOCKET_ERROR) {
			cout << endl << "Server closed! Client Disconnected..." << endl;
			system("pause");
			exit(1);
		}

		cout << "Register successfully, Back to MENU" << endl;
		system("pause");
		goto MENU;

	}
	else if (input[11] == '0') {
		cout << endl << "Close connection" << endl;
		exit(1);
	}

	else {
		cout << "Invalid ipnut, pls try again!" << endl;
		goto MENU;
	}

	do
	{
		system("cls");

		// Prompt the user for some text
		GotoXY(0, 1);
		cout << "Date: " << date << endl;
		cout << "Enter country to look up (or change date): ";
		getline(cin, clientInput);

		if (clientInput.size() == 10) {
			if (clientInput[2] == '-' && clientInput[5] == '-') {
				date = clientInput;
				continue;
			}
		}

		if (clientInput.size() > 0)		// Make sure the user has typed in something
		{
			// Send the text
			clientInput = date + ":" + clientInput;
			int sendResult = send(sock, clientInput.c_str(), clientInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buffer, 1024);

				int bytesReceived = recv(sock, buffer, 1024, 0);
				if (bytesReceived > 0)
				{
					// Cout response to console
					
					cout << endl << "SERVER>  " << string(buffer, 0, bytesReceived) << endl;
					cout << "1. Continue Look up" << endl << "0. Exit" << endl << "----------------" << endl << "Choose: ";

					LABEL1:
					getline(cin, option);

					if (option == "1")
						continue;
					else if (option == "0") {
						cout << "Exit" << endl;
						exit(1);
					}
					else {
						cout << "Invalid input, pls choose again: ";
						goto LABEL1;
					}				
				}
				else {
					cout << endl << "Server closed! Client Disconnected..." << endl;
					exit(1);
				}
			}
			else {
				cout << endl << "Server closed! Client Disconnected..." << endl;
				exit(1);
			}
		}
	} 
	while (clientInput.size() > 0);

	closesocket(sock);
	WSACleanup();
	return 0;

}