#pragma once
#include "include.h"

class InfoList {
public:
	InfoList() {
		this->set("autoFollow", "OFF");
		this->set("lastBpPosMf", -2);
		this->set("lastBpPosSD", -2);
		this->set("autoMf", "OFF");
		this->set("othersAuto", "OFF");
		this->set("pid", GetCurrentProcessId());
	}
	map<string, string> infos;
	void add(string key);
	void set(string key, const char* value);
	void set(string key, int value);
};