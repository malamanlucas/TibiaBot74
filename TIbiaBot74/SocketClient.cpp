#include "pch.h"
#include "include.h"
#include "SocketClient.h"

SOCKET sock;

thread SocketClient::initThread() {
	thread th1([]() {
		SocketClient client;
		client.init();
	});
	return th1;
}

void SocketClient::sendMsg(const char* msg) {
	int sendResult = send(sock, msg, strlen(msg) + 1, 0);
	if (sendResult != SOCKET_ERROR) {
		cout << "Message enviada com sucesso" << endl << endl;
	}
}


void SocketClient::init() {
	string ipAddress = "52.55.88.254";			// IP Address of the server
	int port = 5000;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
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
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;

	while (true) {
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(sock, buf, 4096, 0);
		if (bytesReceived > 0)
		{
			// Echo response to console
			cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
		}
	}

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}