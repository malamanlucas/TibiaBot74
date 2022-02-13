
#pragma once
#include "include.h"
#include "tipos.h"
#include "CreaturePos.h"
#include "ItemSearch.h"
#include "Creature.h"
#include "Inventory.h"

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
		this->inventory = make_shared<Inventory>(moduleBase);
	}
	char* nameSingleton;
	int zAddress;
	int manaFullAddress;
	int lastTargetId;
	shared_ptr<Inventory> inventory;
	
	shared_ptr<CreaturePos> myPos;

	int id();
	void autoHealFriend();
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
	void useItem(shared_ptr<ItemSearch> itemSearch, const shared_ptr<CreaturePos> creaturePos);
	void uhSelf();
	void mfSelf();
	void mfSelfOldUltra();
	void useItemCreaturePos(ItemId itemId, shared_ptr<CreaturePos> creaturePos, int creatureId);
	void sendItemCreature(ItemId itemId, shared_ptr<Creature> creature);
	void sdTarget();
	void runeTarget(ItemId itemId);
	void runeEnemy(ItemId itemId);
	void explosionTarget();
	void paralyzeTarget();
	const char* toString();
	void moveItem(shared_ptr<ItemSearch> itemSearch, int xTo, int yTo, int zTo);
	void moveItemToBackpack(shared_ptr<ItemSearch> itemSearch);
	void moveItemToFeet(shared_ptr<ItemSearch> itemSearch);
	void sendItem(shared_ptr<ItemSearch> itemSearch, int creatureId);
	void sendItemSelf(ItemId itemId);
	void autoHeal();
	char* name();
	void antiPush();
	void pushItensTarget();
};