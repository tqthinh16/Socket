#include"Function.h"

using namespace std;

Client* create1(string str) {
	Client* p = new Client;
	stringstream ss(str);
	getline(ss, p->username, ',');
	ss >> p->password;
	p->next = nullptr;
	return p;
}


void getLoginData(ClientList& l, string file_name) {
	ifstream fin(file_name, ios_base::in);
	string temp;
	while (!fin.eof()) {
		getline(fin, temp);
		//Client* g = create1(temp);
		if (l.head == nullptr) {
			l.head = create1(temp);
			l.tail = l.head;
			l.tail->next = nullptr;
		}
		else {
			l.tail->next = create1(temp);
			l.tail = l.tail->next;
			l.tail->next = nullptr;
		}
	}
	fin.close();
}

bool checkAvailableUsername(ClientList& l, string user) {
	Client* cur = l.head;
	if (cur == nullptr) return true;
	while (cur) {
		if (user == cur->username) {
			return false;
			break;
		}
		cur = cur->next;
	}
	return true;
}

Client* create2(string username, string password) {
	Client* p = new Client;
	p->username = username;
	p->password = password;
	p->next = nullptr;
	return p;
}

void UserReg(ClientList& l, string user, string pass) {
	fstream fout("input.txt", ios_base::app);
	Client* p = create2(user, pass);
	if (l.head == nullptr) {
		l.head = p;
		l.tail = l.head;
		l.tail->next = nullptr;
	}
	else {
		l.tail->next = p;
		l.tail = l.tail->next;
		l.tail->next = nullptr;
	}
	fout << "\n" << p->username << "," << p->password;

	fout.close();
}

bool checkCorrect(ClientList& l, string user, string pass)
{
	Client* cur = l.head;
	if (cur == nullptr) return false;
	while (cur) {
		if (user == cur->username && pass == cur->password) {
			return true;
			break;
		}
		cur = cur->next;
	}
	return false;
}

Country* create3(string str) {
	Country* p = new Country;
	stringstream ss(str);
	string temp;

	getline(ss, p->name, ',');

	getline(ss, temp, ',');
	p->Cases = stoi(temp);

	getline(ss, temp, ',');
	p->today_Cases = stoi(temp);

	getline(ss, temp, ',');
	p->Death = stoi(temp);

	getline(ss, temp, ',');
	p->Today_Deaths = stoi(temp);

	getline(ss, temp, ',');
	p->uTreatment = stoi(temp);

	getline(ss, temp);
	p->Recovery = stoi(temp);


	p->next = nullptr;
	return p;
}

//void getCountryData(CountryList& l, string file_name) {
//	ifstream fin(file_name, ios_base::in);
//	string temp;
//	while (!fin.eof()) {
//		getline(fin, temp);
//		//Client* g = create1(temp);
//		if (l.head == nullptr) {
//			l.head = create3(temp);
//			l.tail = l.head;
//			l.tail->next = nullptr;
//		}
//		else {
//			l.tail->next = create3(temp);
//			l.tail = l.tail->next;
//			l.tail->next = nullptr;
//		}
//	}
//	fin.close();
//}



Country* findCountry(CountryList& l, string name) {
	if (l.head == nullptr) {
		cout << "not found" << endl;
		return nullptr;
	}

	Country* pcur = l.head;
	while (pcur) {
		if (pcur->name == name) {
			//cout << pcur->name << " " << pcur->Cases << " " << pcur->uTreatment << " " << pcur->Other << " " << pcur->Recovery << " " << pcur->Death << endl;
			return pcur;
			break;
		}
		pcur = pcur->next;
	}

	if (pcur == nullptr) return nullptr;
}


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

Json::Value decode(string text)
{
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();

    Json::Value root;
    string errors;

    bool parsingSuccessful = reader->parse(text.c_str(), text.c_str() + text.size(), &root, &errors);
    delete reader;

    return root;
}


Json::Value ConvertToJson(string web_html, string& other) {
    string simp;
    unsigned int begin = web_html.find('[');
    unsigned int end = web_html.find(']');
    unsigned i;

    for (i = begin; i <= end; i++) {
        simp = simp + web_html[i];
    }

    other = simp;
    return decode(simp);
}

void WebToFile() {
    WSADATA wsaData;
    SOCKET Socket;
    SOCKADDR_IN SockAddr;
    int lineCount = 0;
    int rowCount = 0;
    struct hostent* Host;
    locale local;
    char buffer[15000];
    int i = 0;
    int nDataLength;
    string website_HTML;

    // website url
    string url = "coronavirus-19-api.herokuapp.com";

    //HTTP GET
    string get_http = "GET /countries HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed.\n";
        system("pause");
        return;
    }

    Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    Host = gethostbyname(url.c_str());

    SockAddr.sin_port = htons(80);
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)Host->h_addr);

    if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
        cout << "Could not connect";
        system("pause");
        return;
    }

    // send GET / HTTP
    send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

    // recieve html
    while ((nDataLength = recv(Socket, buffer, 15000, 0)) > 0) {
        int i = 0;
        while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {

            website_HTML += buffer[i];
            i += 1;
        }
    }
    closesocket(Socket);
    //WSACleanup();

    string temp;
    Json::Value res = ConvertToJson(website_HTML, temp);

    ofstream fbb;
    unsigned int len = temp.length();

    fbb.open(DateTodayString() + ".txt", ios_base::out);
    if (fbb.fail()) return;
    for (unsigned int k = 0; k < len; k++) {
        if (temp[k] == '{') fbb << endl;
        fbb << temp[k];
    }
    fbb.close();

    ofstream f;
    f.open(DateTodayString() + ".json", ios_base::out);
    if (f.fail()) return;
    f << res;
    f.close();
}

void getCountryData(CountryList& root,string date ) {
    ifstream f;
    Json::Value C;
    string info;
    f.open(date + ".json", ifstream::binary);
    if (f.fail()) return;
    f >> C;

    for (int i = 0; i != C.size(); i++) {
        if (C[i].size() != 12) continue;
        info = "";

        if (C[i].isMember("country")) {
            info += (C[i]["country"].asString());
            info += ",";
        }

        if (C[i].isMember("cases")) {
            info += to_string((C[i]["cases"].asInt()));
            info += ",";
        }


        if (C[i].isMember("todayCases")) {
            info += to_string((C[i]["todayCases"].asInt()));
            info += ",";
        }

        if (C[i].isMember("deaths")) {
            info += to_string((C[i]["deaths"].asInt()));
            info += ",";
        }

        if (C[i].isMember("todayDeaths")) {
            info += to_string((C[i]["todayDeaths"].asInt()));
            info += ",";
        }

        if (C[i].isMember("active")) {
            info += to_string((C[i]["active"].asInt()));
            info += ",";
        }

        if (C[i].isMember("recovered")) {
            info += to_string((C[i]["recovered"].asInt()));
        }

        if (root.head == nullptr) {
            root.head = create3(info);
            root.tail = root.head;
            root.tail->next = nullptr;
        }
        else {
            root.tail->next = create3(info);
            root.tail = root.tail->next;
            root.tail->next = nullptr;
        }

    }
}

void deleteList(CountryList& p) {
    Country* temp;
    while (p.head != NULL) {
        temp = p.head;
        p.head = p.head->next;
        delete temp;
    }
}

time_mark getTime_mark() {
    time_t t = time(0);   // get time now
    struct tm* now = localtime(&t);
    time_mark res;
    res.hour = now->tm_hour;
    res.date = now->tm_mday;
    res.month = now->tm_mon + 1;
    res.year = now->tm_year + 1900;
    return res;
}

bool greaterTime(time_mark a, time_mark b) {
    if (a.hour <= b.hour) return 0;
    if (a.date <= b.date) return 0;
    if (a.month <= b.month) return 0;
    if (a.year <= b.year) return 0;
    return 1;
}
