#pragma once
#include "include.h"

class NaviPlayerReader {
public:
	void process(string* commands);
	void doParalyze(int itemId, int creatureId);
};