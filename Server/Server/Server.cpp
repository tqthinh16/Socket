#pragma warning( disable : 4996)
#include<iostream>
#include<string.h>
#include<WinSock2.h>
#include<winsock.h>
#include<stdlib.h>
#include <WS2tcpip.h>
#include<sys/types.h>
#include<time.h>

#include"Function.h"

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


using namespace std;

int main() {
    int sockfd, new_sock, valread, max_sd;
	struct sockaddr_in address;
	int opt = 1;
	int port = 8080;
	int client_socket[30], max_clients = 30, activity, sd;
	char buffer[1024] = { 0 };
	char error[7] = "failed";
    char correct[8] = "correct";
    char confirm[8] = "confirm";
	fd_set readfds;
    string username, password, password1;
    char message[10] = "connected";
    bool checkLogin[30];

	ClientList l = {nullptr, nullptr};
    CountryList p = { nullptr, nullptr };

	for (int i = 0; i < max_clients; i++) {
		client_socket[i] = 0;
        checkLogin[i] = false;
	}
	
    getLoginData(l, "input.txt");

    WebToFile();
    time_mark hold = getTime_mark();

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
		cout << "Sokcet Failed" << endl;
		exit(1);
	}
	cout << "Socket created" << endl;

    // Forcefully attaching socket to the port 8080
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(1);
    }

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
    address.sin_addr.S_un.S_addr = INADDR_ANY;

	int len = sizeof(address);

	if (bind(sockfd, (struct sockaddr*)&address, len) < 0) {
		cout << endl << "Bind Failed" << endl;
        exit(1);
	}
	cout << endl << "Binded" << endl;

	listen(sockfd, SOMAXCONN);
	cout << endl << "Waiting for connection....." << endl;

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

        //wait for an activity on one of the sockets, timeout is NULL, 
        //so wait indefinitely 
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        //if ((activity < 0) && (errno != EINTR))
        //{
        //    printf("select error");
        //}

    //BACK2:
        //If something happened on the master socket, 
        //then its an incoming connection 
        if (FD_ISSET(sockfd, &readfds))
        {
            if ((new_sock = accept(sockfd, (struct sockaddr*)&address, &len)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            cout << endl << "New connection from " << inet_ntoa(address.sin_addr) << endl;

            //add new socket to array of sockets 
            for (int i = 0; i < max_clients; i++)
            {
                //if position is empty 
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_sock;
                    cout << endl << "Adding to list of sockets as client no." << i << endl;
                    break;
                }
            }
        }
    BACK2:
        //else its some IO operation on some other socket
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds))
            {
            
                ZeroMemory(buffer, 1024);

                //Check if it was for closing , and also read the incoming message 
                valread = recv(sd, buffer, 1024, 0);

                string mes = buffer;
                string input = "";
                string date_file = "";

                int pos = mes.find(":") + 1;
                for (int cou = pos; cou < mes.length(); cou++) {
                    input += mes[cou];
                }

                for (int cou = 0; cou < pos-1; cou++) {
                    date_file += mes[cou];
                }


                if (valread == SOCKET_ERROR || valread == 0 || input == "0")
                {
                    //Somebody disconnected , get his details and print 
                    getpeername(sd, (struct sockaddr*)&address, &len);
                    cout << endl << "CLient No." << i << " disconnected! " << endl;
                    closesocket(sd);
                    client_socket[i] = 0;
                    checkLogin[i] = false;
                    break;
                }

                // login process
                if (input == "1") {
                    if (checkLogin[i] == false) {
                    TRY1:

                        ZeroMemory(buffer, 1024);
                        valread = recv(sd, buffer, 1024, 0);
                        username = buffer;
                        valread = recv(sd, buffer, 1024, 0);
                        password = buffer;

                        if (valread == SOCKET_ERROR || valread == 0)
                        {
                            //Somebody disconnected , get his details and print 
                            getpeername(sd, (struct sockaddr*)&address, &len);
                            cout << endl << "CLient No." << i << " disconnected! " << endl;

                            closesocket(sd);
                            FD_CLR(sd, &readfds);
                            client_socket[i] = 0;
                            checkLogin[i] = false;
                            break;
                        }

                        if (checkCorrect(l, username, password) == false) {
                            send(sd, error, sizeof(error), 0);
                            cout << endl << "CLient No." << i << " login unsuccessfully! " << endl;
                            goto TRY1;
                        }
                        else
                        {
                            cout << endl << "CLient No." << i << " login successfully! " << endl;
                            send(sd, correct, sizeof(correct), 0);
                            checkLogin[i] = true;
                            ZeroMemory(buffer, 1024);
                        }
                    }
                }

                // if user want to register
                else if (input == "2") {
                TRY2:

                    ZeroMemory(buffer, 1024);
                    valread = recv(sd, buffer, 1024, 0);
                    username = buffer;
                    valread = recv(sd, buffer, 1024, 0);
                    password = buffer;
                    valread = recv(sd, buffer, 1024, 0);
                    password1 = buffer;

                    if (valread == SOCKET_ERROR || valread == 0)
                    {
                        //Somebody disconnected , get his details and print 
                        getpeername(sd, (struct sockaddr*)&address, &len);
                        cout << "CLient No." << i << " disconnected! " << endl;
                        closesocket(sd);
                        FD_CLR(sd, &readfds);
                        checkLogin[i] = false;
                        client_socket[i] = 0;
                        break;
                    }

                    if (password != password1)
                    {
                        send(sd, confirm, sizeof(confirm) + 1, 0);
                        cout << endl << "Wrong confirm password" << endl;
                        goto TRY2;
                    }

                    else if (checkAvailableUsername(l, username) == false) {
                        send(sd, error, sizeof(error) + 1, 0);
                        goto TRY2;
                    }

                    else {
                        UserReg(l, username, password);
                        send(sd, correct, sizeof(correct) + 1, 0);
                        cout << endl << "CLient No." << i << " register successfully! " << endl;
                    }
                }

                // Secret command to disconnect all client connection
                else if (input == "-closeAll") {
                    string confirm;
                    int sdk;
                BACK3:
                    cout << endl << "Disconnect all connected clients? (Y/N): ";
                    getline(cin, confirm);

                    if (confirm == "Y") {
                        for (int i = 0; i < max_clients; i++) {
                            sdk = client_socket[i];
                            checkLogin[i] = false;
                            //FD_CLR(sdk, &readfds);
                            closesocket(sdk);
                        }
                        cout << endl << "Done!" << endl;
                        goto BACK2;
                    }
                    else if (confirm == "N") {
                        string denied = "Server denied";
                        send(sd, denied.c_str(), denied.size() + 1, 0);
                        continue;
                    }
                    else {
                        cout << endl << "Invalid input" << endl;
                        goto BACK3;
                    }
                }

                else {
                    if (date_file == DateTodayString()) {
                        time_mark moment = getTime_mark();
                        if (greaterTime(moment, hold)) {
                            hold = moment;
                            WebToFile();
                        }
                    }

                    deleteList(p);
                    getCountryData(p, date_file);

                    cout << endl << "CLient No." << i << " search for " << input << endl;
                    Country* cur = findCountry(p, input);

                    if (cur) {

                        string cases = to_string(cur->Cases);
                        string uTreat = to_string(cur->uTreatment);
                        string recovery = to_string(cur->Recovery);
                        string death = to_string(cur->Death);
                        string todaydeath = to_string(cur->Today_Deaths);
                        string todaycases = to_string(cur->today_Cases);

                        string info = cur->name + "  Cases: " + cases + "  Today_Cases: " + todaycases + "  Deaths: " + death + "  Today_Deaths: " + todaydeath + "  Under_treatment: " + uTreat + "  Recovered: " + recovery + "\n";

                        cout << endl <<  "SERVER> " << info;

                        //set the string terminating NULL byte on the end 
                        //of the data read 
                        buffer[valread] = '\0';
                        send(sd, info.c_str(), info.size() + 1, 0);
                    }

                    else {
                        string info = "Invalid input or Country not found";
                        send(sd, info.c_str(), info.size() + 1, 0);
                    }
                }
            }
        }
    }

	WSACleanup();
	return 0;
}
