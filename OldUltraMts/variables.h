#pragma once

#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "Player.h"
#include "Containers.h"
#include "BattleList.h"
#include "SocketClient.h"
#include "WindowsApi.h"
#include "InfoList.h"
#include "include.h"

struct AutoTask {
	bool flag = false;
	const char* delay = "500";
	const char* label;
	void (*func)();
	AutoTask(const char* label, void (*func)()) {
		this->label = label;
		this->func = func;
	}
};

class AutoTasks {
public:
	bool testGui = false;
	bool running = true;
	bool showGui = true;
	bool isConnected = false;
	int i;
	vector<thread> threads;
	vector<AutoTask*> tasks;

	void abc();
	void enableAutoTask(AutoTask *autoTask);

	void enableAllConditionally();
	void joinThreads();
};

class Variables {
public:
	Variables(int moduleBase) {
		this->player = make_shared<Player>(moduleBase);
		cout << "player done" << endl;
		this->containers = make_shared<Containers>(moduleBase);
		cout << "containers done" << endl;
		this->battleList = make_shared<BattleList>(moduleBase);
		cout << "battelList done" << endl;
		this->client = make_shared<SocketClient>();
		cout << "client done" << endl;
		this->autoTasks = make_shared<AutoTasks>();
		cout << "autoTasks done" << endl;
		this->windowsApi = make_shared<WindowsApi>();
		cout << "windowsApi done" << endl;
		this->infoList = make_shared<InfoList>();
		cout << "InfoList done" << endl;
	}
	shared_ptr<Player> player;
	shared_ptr<Containers> containers;
	shared_ptr<BattleList> battleList;
	shared_ptr<SocketClient> client;
	shared_ptr<AutoTasks> autoTasks;
	shared_ptr<WindowsApi> windowsApi;
	shared_ptr<InfoList> infoList;
};