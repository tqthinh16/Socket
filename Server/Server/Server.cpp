#include<iostream>
#include<string.h>
#include<WinSock2.h>
#include<winsock.h>
#include<stdlib.h>
#include<sys/types.h>
#include<time.h>
#include"Function.h"

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace std;

int main() {
	int sockfd, new_sock, valread;
	struct sockaddr_in address;
	int opt = 1;
	int port = 8080;
	int client_socket[30], max_clients = 30, activity, sd;
	char buffer[1024] = { 0 };
	char error[7] = "failed";
    char correct[8] = "correct";
	fd_set readfds;
	int max_sd;
	string username, password;
    char message[10] = "connected";
    bool checkLogin[30];
	ClientList l = {nullptr, nullptr};
    CountryList p = { nullptr, nullptr };


	for (int i = 0; i < max_clients; i++) {
		client_socket[i] = 0;
        checkLogin[i] = false;
	}
	
    getLoginData(l, "input.txt");
    getCountryData(p, "data.txt");


	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET) {
		cout << "fail socket" << endl;
		exit(1);
	}
	cout << "Socket created" << endl;

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
    address.sin_addr.S_un.S_addr = INADDR_ANY;

	int len = sizeof(address);

	if (bind(sockfd, (struct sockaddr*)&address, len) < 0) {
		cout << "failed bind" << endl;
		return -1;
	}
	cout << "Binded" << endl;

	listen(sockfd, SOMAXCONN);
	cout << "Waiting for connection....." << endl;

    while (TRUE)
    {
        //clear the socket set 
        FD_ZERO(&readfds);

        //add master socket to set 
        FD_SET(sockfd, &readfds);
        max_sd = sockfd;

        //add child sockets to set 
        for (int i = 0; i < max_clients; i++)
        {
            //socket descriptor 
            sd = client_socket[i];

            //if valid socket descriptor then add to read list 
            if (sd > 0)
                FD_SET(sd, &readfds);

            //highest file descriptor number, need it for the select function 
            if (sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(sockfd, &readfds))
        {
            if ((new_sock = accept(sockfd, (struct sockaddr*)&address, &len)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands 
            //printf("New connection , socket fd is %d , ip is : %s , port : %d n" , new_sock , inet_ntoa(address.sin_addr) , ntohs_ address.sin_port));

            //send new connection greeting message 
            /*if (send(new_sock, message, strlen(message), 0) != strlen(message))
            {
                perror("send");
            }

            puts("Welcome message sent successfully");
            */
            //add new socket to array of sockets 
            for (int i = 0; i < max_clients; i++)
            {
                //if position is empty 
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_sock;
                    cout << "Adding to list of sockets as client no." << i << endl;
                    break;
                }
            }
        }
    
        //else its some IO operation on some other socket
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            
            if (FD_ISSET(sd, &readfds))
            {

                BACK:
                //Check if it was for closing , and also read the 
                //incoming message 
                valread = recv(sd, buffer, 1024, 0);
                
                string input = buffer;

                if (valread == SOCKET_ERROR || valread == 0 || input == "0")
                {
                    //Somebody disconnected , get his details and print 
                    getpeername(sd, (struct sockaddr*)&address, &len);
                    cout << "CLient No." << i << " disconnected! " << endl;
                    closesocket(sd);
                    client_socket[i] = 0;
                    break;
                }

                if (input == "1") {
                    if (checkLogin[i] == false) {
                    TRY1:
                        ZeroMemory(buffer, 1024);
                        valread = recv(sd, buffer, 1024, 0);
                        username = buffer;
                        valread = recv(sd, buffer, 1024, 0);
                        password = buffer;

                        if (checkCorrect(l, username, password) == false) {
                            send(sd, error, sizeof(error), 0);
                            cout << username << " " << password << endl;
                            cout << "wrong username or password" << endl;
                            goto TRY1;
                        }
                        else
                        {
                            cout << username << " " << password << endl;
                            send(sd, correct, sizeof(correct), 0);
                            checkLogin[i] = true;
                            ZeroMemory(buffer, 1024);
                        }
                    }
                }

                else if (input == "2") {
                TRY2:
                    ZeroMemory(buffer, 1024);
                    valread = recv(sd, buffer, 1024, 0);
                    username = buffer;
                    valread = recv(sd, buffer, 1024, 0);
                    password = buffer;
                    
                    if (checkAvailableUsername(l, username) == false) {
                        send(sd, error, sizeof(error), 0);
                        cout << username << " " << password << endl;
                        cout << "wrong username or password" << endl;
                        goto TRY2;
                    }
                    else {
                        UserReg(l, username, password);
                        send(sd, correct, sizeof(correct), 0);
                        cout << "register successfully" << endl;
                        goto BACK;
                    }

                }

                else {
                    Country* cur = findCountry(p, input);
                    if (cur) {
                        string cases = to_string(cur->Cases);
                        string uTreat = to_string(cur->uTreatment);
                        string other = to_string(cur->Other);
                        string recovery = to_string(cur->Recovery);
                        string death = to_string(cur->Death);


                        string info = cur->name + "," + cases + "," + uTreat + "," + other + "," + recovery + "," + death + "\n";
                        //Echo back the message that came in 
                        //else
                        //{
                            //set the string terminating NULL byte on the end 
                            //of the data read 
                            //buffer[valread] = '\0';
                        send(sd, info.c_str(), info.size() + 1, 0);
                    }
                }
            }
        }
    }
	WSACleanup();
	return 0;
}
