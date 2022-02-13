#pragma once
#include "include.h"

class SocketClient {
public:
	void init();
	thread initThread();
	void sendMsg(const char* msg);
};