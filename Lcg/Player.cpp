#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "CreaturePos.h"
#include "Player.h"
#include "Containers.h"
#include "ItemSearch.h"
#include "BattleList.h"
#include "SocketClient.h"
#include "variables.h"

extern shared_ptr<Variables> v;

void Player::runeCreatureId(ItemId itemId, int creatureId) {
	//cout << "runeCreatureId rune: " << itemId << " creatureId: " << creatureId << endl;
	this->attack(creatureId);
	if (this->healthPercent() <= 80) {
		return;
	}
	auto itemSearch = v->containers->searchItemAndOpenBp(itemId);
	if (itemSearch != nullptr) {
		//cout << "Item Found" << endl;
		//this->useItem(itemSearch, pos);
		this->sendItem(itemSearch, creatureId);
	}
	else {
		//cout << "Item Not Found" << endl;
		itemSearch = v->containers->searchItem(ItemId::EXPLOSION);
		if (itemSearch != nullptr) {
			this->sendItem(itemSearch, creatureId);
		}
		else {
			itemSearch = v->containers->searchItem(ItemId::HMM);
			if (itemSearch != nullptr) {
				this->sendItem(itemSearch, creatureId);
			}
		}
	}
}

void Player::comboTarget(ItemId itemId, int creatureId) {
	ostringstream buf;
	buf << "/" << itemId << " " << creatureId;
	v->client->sendMsg(buf.str().c_str());
}

void Player::comboEnemy(ItemId itemId) {
	auto creature = v->battleList->findEnemy();
	if (creature != nullptr) {
		ostringstream buf;
		buf << "/" << itemId << " " << creature->id();
		v->client->sendMsg(buf.str().c_str());
	}
}

void Player::printWhiteMessage(const char* msg) {
	reinterpret_cast<void(__cdecl*)(const char*)>(this->moduleBase + 0xFF500)(msg);
}

void Player::followPlayer(const char* name) {
	auto creature = v->battleList->findCreatureByName(name);
	if (creature != nullptr) {
		this->follow(creature->id());
	}
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

void Player::autoHeal() {
	int healthPercent = this->healthPercent();
	if (healthPercent < 70) {
		if (this->isDruid) {
			this->mana() > 70 ? this->sioSelf() : this->uhSelf();
		}
		else {
			this->mana() > 80 ? this->say("exura vita") : this->uhSelf();
		}
		
	}
}

void Player::sioFriend(shared_ptr<Creature> creature) {
	ostringstream buf;
	buf << "exura sio \"" << creature->name();
	this->say(buf.str().c_str());
}

void Player::sioSelf() {
	ostringstream buf;
	buf << "exura sio \"" << this->name();
	this->say(buf.str().c_str());
}

void Player::autoHealFriend() {
	auto amigo = v->battleList->findFriend(70);
	if (amigo != nullptr) {
		if (v->player->healthPercent() > 90) {
			if (this->isDruid) {
				this->mana() > 70 ? this->sioFriend(amigo) : this->sendItemCreature(ItemId::UH, amigo);
			}
			else {
				this->sendItemCreature(ItemId::UH, amigo);
			}
			
		}
	}
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

char* Player::name() {
	return v->battleList->findMe()->name();
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

bool Player::isOnline() {
	return this->manaFull() != 0;
}

int Player::healthFull() {
	return *(int*)(this->manaFullAddress + 0x1C);
}

const char* Player::toString() {
	stringstream buffer;
	buffer.str();
	buffer << " name:" << this->name();
	buffer << " pos:" << this->myPos->toString();
	buffer << " id:" << this->id();
	buffer << " lastTarget: " << this->lastTargetId;
	return buffer.str().c_str();
}

void Player::say(const char* msg) {
	reinterpret_cast<void(__cdecl*)(int, const char*)>(this->moduleBase + 0x67C0)(1, msg);
}

void Player::uhSelf() {
	this->useItemCreaturePos(ItemId::UH, this->myPos, this->id());
}

void Player::useItemCreaturePos(ItemId itemId, shared_ptr<CreaturePos> pos, int creatureId) {
	auto itemSearch = v->containers->searchItemAndOpenBp(itemId);
	if (itemSearch != nullptr) {
		//cout << "Item Found" << endl;
		//this->useItem(itemSearch, pos);
		this->sendItem(itemSearch, creatureId);
	}
}

void Player::sendItemCreature(ItemId itemId, shared_ptr<Creature> creature) {
	if (creature->isOnline()) {
		//cout << "Creautre isOnline" << endl;
		this->useItemCreaturePos(itemId, creature->pos, creature->id());
	}
	else {
		//cout << "Creautre not isOnline" << endl;
	}
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
	auto creature = v->battleList->findCreature(this->lastTargetId);
	if (creature != nullptr) {// found
		cout << "achou creature" << endl;
		this->sendItemCreature(itemId, creature);
	}
	else {
		cout << "não achou creature" << endl;
	}
}

void Player::antiPush() {
	auto itemSearch = v->containers->searchItem(ItemId::EXPLOSION);
	if (itemSearch != nullptr) {
		this->moveItemToFeet(itemSearch);
	}
	itemSearch = v->containers->searchItem(ItemId::ENERGY_BOMB);
	if (itemSearch != nullptr) {
		this->moveItemToFeet(itemSearch);
	}
}

void Player::runeEnemy(ItemId itemId) {
	if (this->isAttacking()) {
		this->runeTarget(itemId);
	}
	else {
		auto creature = v->battleList->findEnemy();
		if (creature != nullptr) {// found
			this->attack(creature->id());
			this->sendItemCreature(itemId, creature);
		}
		else {
			this->runeTarget(itemId);
		}
	}
}

void Player::paralyzeTarget() {
	this->runeTarget(ItemId::PARALYSE);
}

void Player::explosionTarget() {
	this->runeTarget(ItemId::EXPLOSION);
}


void Player::mfSelf() {
	this->useItemCreaturePos(ItemId::MF, this->myPos, this->id());
}

void Player::sendItemSelf(ItemId itemId) {
	this->useItemCreaturePos(itemId, this->myPos, this->id());
}

void Player::mfSelfOldUltra() {
	auto itemSearch = v->containers->searchItem(ItemId::MF);
	if (itemSearch == nullptr) {
		return;
	}
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
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(this->myPos->x());
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(this->myPos->y());
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(this->myPos->z());
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(99);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(2);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}

void Player::useItem(shared_ptr<ItemSearch> itemSearch, const shared_ptr<CreaturePos> creaturePos) {
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

void Player::sendItem(shared_ptr<ItemSearch> itemSearch, int creatureId) {
	//cout << "chamando sendItem" << endl;
	int fromBp = 64 + itemSearch->bpPos;

	cout << itemSearch->toString() << endl;

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0x84);
	//cout << "Enviando 0x84" << endl;
	//pos = Position(0xFFFF, 0, 0); //means that is an item in inventory
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(65535); //xPos Item means

	//64 first bp opened, 65 second bp opened, and so on...
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(fromBp);

	//0 first pos, 1 second pos
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(itemSearch->slotPos);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(itemSearch->id); //uh

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(itemSearch->slotPos);

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB880)(creatureId);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}

void Player::moveItemToBackpack(shared_ptr<ItemSearch> itemSearch) {
	cout << "moving item to backpack" << endl;
	int xTo = 65535;
	int yTo = InventoryType::Backpack;
	int zTo = 0;
	this->moveItem(itemSearch, xTo, yTo, zTo);
}

void Player::moveItemToFeet(shared_ptr<ItemSearch> itemSearch) {
	cout << "moving item to feet" << endl;
	this->moveItem(itemSearch, this->myPos->x(), this->myPos->y(), this->myPos->z());
}

void Player::moveItem(shared_ptr<ItemSearch> itemSearch, int xTo, int yTo, int zTo) {
	cout << "[moving itemSearch, bpPos:" << itemSearch->bpPos << ", slotPos:" << itemSearch->slotPos << endl;
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