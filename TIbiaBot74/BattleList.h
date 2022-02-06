#pragma once
#include "Creature.h"
#include "include.h"

class BattleList {
public:
	BattleList(int moduleBase) {
		this->moduleBase = moduleBase;
		this->creatureOffset = 0x9C;
		this->creatureStartAddress = *(int*)(this->moduleBase + 0x4C84C);
	}
	int moduleBase;
	int creatureOffset;
	int creatureStartAddress;

	shared_ptr<Creature> findCreature(int creatureId);
};