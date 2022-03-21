#pragma once
#include "Creature.h"
#include "include.h"

class BattleList {
public:
	BattleList(int moduleBase) {
		this->moduleBase = moduleBase;
		this->creatureOffset = 0x9C;
		this->creatureStartAddress = *(int*)(this->moduleBase + 0x4C84C);
		this->init();
	}
	int moduleBase;
	int creatureOffset;
	int creatureStartAddress;

	void init();
	void loadEnemies();
	void loadFriends();
	bool isEnemyOnScreen();
	bool isEnemy(shared_ptr<Creature> creature);
	bool isFriend(shared_ptr<Creature> creature);
	shared_ptr<Creature> findCreature(int creatureId);
	shared_ptr<Creature> findCreatureByName(const char* name);
	shared_ptr<Creature> findEnemy();
	shared_ptr<Creature> findFriend(int lowerHpPercent);
	shared_ptr<Creature> findMe();
	void readCreaturesVisible();
	bool isVisible(shared_ptr<Creature> creature);

};