#include "pch.h"
#include "BattleList.h"
#include "Creature.h"
#include "include.h"
#include "Player.h"
#include "variables.h"

extern shared_ptr<Variables> v;

shared_ptr<set<string>> enemies;
shared_ptr<set<string>> friends;

void BattleList::init() {
	this->loadEnemies();
	this->loadFriends();
}

shared_ptr<Creature> BattleList::findAllExpectFriend() {
	int i = 0;
	int value = 0;
	while (true) {
		int actualAddress = this->creatureStartAddress + (this->creatureOffset * i);
		value = *(int*)(actualAddress);
		if (value == 0) { // there is no more creature
			break;
		}
		auto creature = make_shared<Creature>(actualAddress);
		if (!this->isFriend(creature) && isVisible(creature)) {
			return creature;
		}
		i++;
	}
	return nullptr;
}

bool BattleList::isEnemyOnScreen() {
	int i = 0;
	int value = 0;
	while (true) {
		int actualAddress = this->creatureStartAddress + (this->creatureOffset * i);
		value = *(int*)(actualAddress);
		if (value == 0) { // there is no more creature
			break;
		}
		auto creature = make_shared<Creature>(actualAddress);
		if (this->isEnemy(creature) && isVisible(creature)) {
			return true;
		}
		i++;
	}
	return false;
}

void BattleList::loadEnemies() {
	if (enemies == nullptr) {
		enemies = make_shared<set<string>>();
	}
	enemies->clear();
	ifstream input;

	input.open("C:\\ferramentas\\enemies.txt");

	for (string enemy; getline(input, enemy); )
	{
		enemies->insert(enemy);
	}
	input.close();
}

void BattleList::loadFriends() {
	if (friends == nullptr) {
		friends = make_shared<set<string>>();
	}
	friends->clear();
	ifstream input;

	input.open("C:\\ferramentas\\friends.txt");

	for (string amigo; getline(input, amigo); )
	{
		friends->insert(amigo);
	}
	input.close();
}

shared_ptr<Creature> BattleList::findCreatureByName(const char* name) {
	int i = 0;
	int value = 0;

	while (true) {
		int actualAddress = this->creatureStartAddress + (this->creatureOffset * i);
		value = *(int*)(actualAddress);
		if (value == 0) { // there is no more creature
			break;
		}
		auto creature = make_shared<Creature>(actualAddress);
		if (strcmp(creature->name(), name) == 0 && isVisible(creature)) {
			//cout << "Encontrei: " << creature->name() << endl;
			return creature;
		}
		i++;
	}
	return nullptr;
}

bool BattleList::isParalyzed(int creatureId) {
	return false;
}

shared_ptr<Creature> BattleList::findEnemy() {
	int i = 0;
	int value = 0;
	while (true) {
		int actualAddress = this->creatureStartAddress + (this->creatureOffset * i);
		value = *(int*)(actualAddress);
		if (value == 0) { // there is no more creature
			break;
		}
		auto creature = make_shared<Creature>(actualAddress);
		if (this->isEnemy(creature) && isVisible(creature)) {
			return creature;
		}
		i++;
	}
	return nullptr;
}

shared_ptr<Creature> BattleList::findFriend(int lowerHpPercent) {
	int i = 0;
	int value = 0;
	stringstream buffer;
	buffer.str();
	while (true) {
		int actualAddress = this->creatureStartAddress + (this->creatureOffset * i);
		value = *(int*)(actualAddress);
		if (value == 0) { // there is no more creature
			break;
		}
		auto creature = make_shared<Creature>(actualAddress);
		if (this->isFriend(creature) && isVisible(creature) && creature->healthPercent() < lowerHpPercent) {
			return creature;
		}
		i++;
	}
	return nullptr;
}

bool BattleList::isEnemy(shared_ptr<Creature> creature) {
	return enemies->find(creature->name()) != enemies->end();
}

bool BattleList::isFriend(shared_ptr<Creature> creature) {
	return friends->find(creature->name()) != friends->end();
}

shared_ptr<Creature> BattleList::findCreature(int creatureId) {
	int i = 0;
	int value = 0;
	stringstream buffer;
	buffer.str();
	while (true) {
		int actualAddress = this->creatureStartAddress + (this->creatureOffset * i);
		value = *(int*)(actualAddress);
		if (value == 0) { // there is no more creature
			break;
		}
		auto creature = make_shared<Creature>(actualAddress);
		if (creature->id() == creatureId && isVisible(creature)) {
			return creature;
		}
		i++;
	}
	return nullptr;
}

shared_ptr<Creature> BattleList::findMe() {
	int i = 0;
	int value = 0;
	stringstream buffer;
	buffer.str();
	while (true) {
		int actualAddress = this->creatureStartAddress + (this->creatureOffset * i);
		value = *(int*)(actualAddress);
		if (value == 0) { // there is no more creature
			break;
		}
		auto creature = make_shared<Creature>(actualAddress);
		if (creature->id() == v->player->id()) {
			return creature;
		}
		i++;
	}
	return nullptr;
}

bool BattleList::isVisible(shared_ptr<Creature> creature) {
	return
		creature->id() != v->player->id() &&
		creature->pos->z() == v->player->myPos->z()
		&& abs(creature->pos->x() - v->player->myPos->x()) <= 7
		&& abs(creature->pos->y() - v->player->myPos->y()) <= 5
		&& creature->healthPercent() > 0;
}

void BattleList::readCreaturesVisible() {
	int i = 0;
	int value = 0;
	stringstream buffer;
	buffer.str();
	while (true) {
		int actualAddress = this->creatureStartAddress + (this->creatureOffset * i);
		value = *(int*)(actualAddress);
		if (value == 0) { // there is no more creature
			break;
		}
		auto creature = make_shared<Creature>(actualAddress);
		if (isVisible(creature)) {
			//here
			buffer << "[" << creature->name();
			buffer << "|" << creature->isOnline();
			buffer << "|" << creature->speed() << "] ";

		}
		i++;
	}
	v->player->printWhiteMessage(buffer.str().c_str());
}