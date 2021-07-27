#include<iostream>
#include<string.h>
#include<WinSock2.h>
#include<winsock.h>
#include<stdlib.h>
#include<sys/types.h>
#include <WS2tcpip.h>
#include<sstream>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define _WIN32_WINNT 0x0A00

using namespace std;

int main() {
	int sock, valread;
	struct sockaddr_in address;
	int port = 8080;
	char buffer[1024] = { 0 };
	string IP;
	cout << "Enter IP: ";
	getline(cin, IP);
	string username, password;

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return -1;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "socket failed" << endl;
		exit(1);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	inet_pton(AF_INET, IP.c_str(), &address.sin_addr);


	if (connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
		cout << "connect failed" << endl;
	}


TRYAGAIN:
	cout << "Username: ";
	getline(cin, username);
	cout << "password: ";
	getline(cin, password);
	send(sock, username.c_str(), username.size() + 1, 0);
	send(sock, password.c_str(), password.size() + 1, 0);

	recv(sock, buffer, 1024, 0);
	if (strcmp(buffer, "failed") == 0) {
		cout << "Wrong username or password, pls try again!" << endl;
		goto TRYAGAIN;
	}

	string clientInput;

	/// <summary>
	/// ////////////////////////////
	/// </summary>
	/// <returns></returns>
	do
	{
		// Prompt the user for some text
		cout << "> ";
		getline(cin, clientInput);

		if (clientInput.size() > 0)		// Make sure the user has typed in something
		{
			// Send the text
			int sendResult = send(sock, clientInput.c_str(), clientInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buffer, 1024);
				int bytesReceived = recv(sock, buffer, 1024, 0);
				if (bytesReceived > 0)
				{
					// Echo response to console
					cout << "SERVER> " << string(buffer, 0, bytesReceived) << endl;
				}
			}
		}

	} while (clientInput.size() > 0);

	closesocket(sock);
	WSACleanup();
	return 0;

}