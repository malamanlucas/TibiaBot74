#pragma once
#include "include.h"
#include "tipos.h"
#include "CreaturePos.h"
#include "ItemSearch.h"
#include "Creature.h"

class Player {
	int moduleBase;

public:
	Player(int moduleBase) {
		this->moduleBase = moduleBase;
		this->zAddress = *(int*)(moduleBase + 0xBC2DC);
		int xAddress = (zAddress + 8);
		int yAddress = (zAddress + 4);
		this->myPos = make_shared<CreaturePos>(this->zAddress, xAddress, yAddress);
		this->manaFullAddress = (moduleBase + 0x1C6828);
	}
	int zAddress;
	int manaFullAddress;
	int lastTargetId;
	
	shared_ptr<CreaturePos> myPos;

	int id();
	void refreshLastTargetId();
	void follow(int creatureId);
	void attack(int creatureId);
	int manaPercent();
	int healthPercent();
	void setTargetId(int creatureId);
	int targetId();
	bool isAttacking();
	int mana();
	int health();
	int manaFull();
	int healthFull();
	void printWhiteMessage(const char* msg);
	void say(const char*);
	void sendRune(shared_ptr<ItemSearch> itemSearch, const shared_ptr<CreaturePos> creaturePos);
	void uhSelf();
	void mfSelf();
	void useItemCreaturePos(ItemId itemId, shared_ptr<CreaturePos> creaturePos);
	void sendItemCreature(ItemId itemId, shared_ptr<Creature> creature);
	void sdTarget();
	void runeTarget(ItemId itemId);
	void explosionTarget();
	void paralyzeTarget();
	const char* toString();
	void moveItem(shared_ptr<ItemSearch> itemSearch, int xTo, int yTo, int zTo);
	void moveItemTobackpack(shared_ptr<ItemSearch> itemSearch);
};