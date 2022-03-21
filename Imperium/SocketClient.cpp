#include "pch.h"
#include "include.h"
#include "SocketClient.h"
#include "variables.h";

SOCKET sock;

extern shared_ptr<Variables> v;

string ipAddress = "192.168.0.9";			// IP Address of the server
int port = 54000;

void SocketClient::close() {
	//shouldExit = false;
	cout << "Calling shutdown... " << endl;
	cout << "Disconnecting from server... " << endl;
	int iResult = shutdown(sock, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return;
	}
	else {
		closesocket(sock);
		WSACleanup();
	}
}

void SocketClient::handleMessageFromServer(char* message, int bytesReceived) {
	string msgFromServer = string(message, 0, bytesReceived);
	cout << "SERVER> " << msgFromServer << endl;

	if (msgFromServer[0] == '.') { //is a command
		msgFromServer[0] = ' ';
		istringstream iss(msgFromServer);
		string word;
		string commands[4];
		int i = 0;
		while (iss >> word) {
			ostringstream buf;
			commands[i] = word;
			i++;
		}
		v->player->naviReader->process(commands);
	}
}

thread SocketClient::initThread() {
	thread th1([]() {
		int cont = 1;
		while (v->client->shouldExit) {
			if (!v->autoTasks->isConnected) {
				cout << cont << "# Trying to connect socket: " << ipAddress << ":" << port << "...";
				if (v->client->init()) {
					v->autoTasks->isConnected = false;
					cout << "finished";
					Sleep(500);
				}
				cont++;
			}
			Sleep(1000);
		}
		v->client->close();
		});
	return th1;
}

bool SocketClient::isConnected() {
	return v->autoTasks->isConnected;
}

void SocketClient::sendMsg(const char* msg) {
	cout << "enviando msg: " << msg << endl;
	int sendResult = send(sock, msg, strlen(msg) + 1, 0);
	if (sendResult != SOCKET_ERROR) {
		cout << "Message enviada com sucesso" << endl << endl;
	}
}

bool SocketClient::init() {

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cout << "Can't start Winsock, Err #" << wsResult << endl;
		return false;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cout << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cout << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return false;
	}

	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the 
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	int iResult;
	u_long iMode = 0;

	iResult = ioctlsocket(sock, FIONBIO, &iMode);
	v->autoTasks->isConnected = true;

	// Do-while loop to send and receive data

	thread th2([]() {
		char buf[4096];
		string userInput;
		while (true) {
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				v->client->handleMessageFromServer(buf, bytesReceived);
			}
			else {
				cout << "Disconnecting from server... " << endl;
				break;
			}
			Sleep(100);
		}

		});
	th2.join();

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
	return true;
}