#pragma once
#include "include.h"
#include "CreaturePos.h"
#include <memory>

using namespace std;

class Creature {
public: 
	Creature(int baseAddres) {
		this->baseAddres = baseAddres;
		int zAddress = this->baseAddres + 0x2C;
		this->pos = make_shared<CreaturePos>(zAddress);
	}
	int baseAddres;
	shared_ptr<CreaturePos> pos;

	char* name();
	int id();
	int speed();
	int isOnline();
	int healthPercent();
	const char* toString();
};