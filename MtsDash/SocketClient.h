#pragma once
#include "include.h"

class SocketClient {
public:
	bool shouldExit = true;
	bool init();
	thread initThread();
	void sendMsg(const char* msg);
	void handleMessageFromServer(char* message, int bytesReceived);
	void close();
};