#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "CreaturePos.h"
#include "Player.h"
#include "Containers.h"
#include "ItemSearch.h"
#include "BattleList.h"

extern shared_ptr<Containers> containers;
extern shared_ptr<BattleList> battleList;

void Player::printWhiteMessage(const char* msg) {
	reinterpret_cast<void(__cdecl*)(const char*)>(this->moduleBase + 0xFF500)(msg);
}

void Player::follow(int creatureId) {
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0xA2);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB880)(creatureId);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}

void Player::attack(int creatureId) {
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0xA1);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB880)(creatureId);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
	this->setTargetId(creatureId);
}

bool Player::isAttacking() {
	return this->targetId() != 0;
}

void Player::setTargetId(int creatureId) {
	*(int*)(this->moduleBase + 0x1C681C) = creatureId;
}

int Player::targetId() {
	return *(int*)(this->moduleBase + 0x1C681C);
}

int Player::manaPercent() {
	return this->mana() * 100 / this->manaFull();
}

int Player::healthPercent() {
	if (this->healthFull() > 0) {
		return this->health() * 100 / this->healthFull();
	}
	return 0;
}

int Player::id() {
	return *(int*)(this->zAddress - 0x8);
}

int Player::mana() {
	return *(int*)(this->zAddress - 0x10);
}

int Player::health() {
	return *(int*)(this->zAddress - 0xC);
}

int Player::manaFull() {
	return *(int*)(this->manaFullAddress);
}

int Player::healthFull() {
	return *(int*)(this->manaFullAddress + 0x1C);
}

const char* Player::toString() {
	stringstream buffer;
	buffer.str();
	buffer << "x: " << this->myPos->x();
	buffer << " y: " << this->myPos->y();
	buffer << " z: " << this->myPos->z();
	return buffer.str().c_str();
}

void Player::say(const char* msg) {
	reinterpret_cast<void(__cdecl*)(int, const char*)>(this->moduleBase + 0x67C0)(1, msg);
}

void Player::uhSelf() {
	this->useItemCreaturePos(ItemId::UH, this->myPos);
}

void Player::useItemCreaturePos(ItemId itemId, shared_ptr<CreaturePos> pos) {
	auto itemSearch = containers->searchItem(itemId);
	if (itemSearch != nullptr) {
		this->sendRune(itemSearch, pos);
	}
}

void Player::sendItemCreature(ItemId itemId, shared_ptr<Creature> creature) {
	this->useItemCreaturePos(itemId, creature->pos);
}

void Player::refreshLastTargetId() {
	if (this->isAttacking()) {
		this->lastTargetId = this->targetId();
	}
}

void Player::sdTarget() {
	this->runeTarget(ItemId::SD);
}

void Player::runeTarget(ItemId itemId) {
	auto creature = battleList->findCreature(this->lastTargetId);
	if (creature != nullptr) {// found
		this->sendItemCreature(itemId, creature);
	}
}

void Player::paralyzeTarget() {
	this->runeTarget(ItemId::PARALYSE);
}

void Player::explosionTarget() {
	this->runeTarget(ItemId::EXPLOSION);
}


void Player::mfSelf() {
	this->useItemCreaturePos(ItemId::MF, this->myPos);
}

void Player::sendRune(shared_ptr<ItemSearch> itemSearch, const shared_ptr<CreaturePos> creaturePos) {
	int fromBp = 64 + itemSearch->bpPos;

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0x83);
	//pos = Position(0xFFFF, 0, 0); //means that is an item in inventory
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(65535); //xPos Item means

	//64 first bp opened, 65 second bp opened, and so on...
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(fromBp);

	//0 first pos, 1 second pos
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(itemSearch->slotPos);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(itemSearch->id); //uh

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(0);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(creaturePos->x());
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(creaturePos->y());
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(creaturePos->z());
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(99);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(1);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}

void Player::moveItemTobackpack(shared_ptr<ItemSearch> itemSearch) {
	int xTo = 65535;
	int yTo = InventoryType::Backpack;
	int zTo = 0;
	this->moveItem(itemSearch, xTo, yTo, zTo);
}

void Player::moveItem(shared_ptr<ItemSearch> itemSearch, int xTo, int yTo, int zTo) {
	int fromBp = 64 + itemSearch->bpPos;

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0x78);
	//pos = Position(0xFFFF, 0, 0); //means that is an item in inventory
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(65535); //xPos Item means

	//64 first bp opened, 65 second bp opened, and so on...
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(fromBp);

	//0 first pos, 1 second pos
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(itemSearch->slotPos);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(itemSearch->id); //uh

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(itemSearch->slotPos);

	//65535 X (inventory) to
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(xTo);

	//64 Y , bpsPos or typeInventory to
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(yTo);

	//2 Z (slotPos) to
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(zTo);

	//50 (amountItems)
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(itemSearch->amount);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}

//10B40