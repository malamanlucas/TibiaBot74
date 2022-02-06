#include "pch.h"
#include "BattleList.h"
#include "Creature.h"
#include "include.h"
#include "Player.h"

extern shared_ptr<Player> player;

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

bool BattleList::isVisible(shared_ptr<Creature> creature) {
	return
		creature->id() != player->id() &&
		creature->pos->z() == player->myPos->z()
		&& abs(creature->pos->x() - player->myPos->x()) <= 7
		&& abs(creature->pos->y() - player->myPos->y()) <= 5
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
	player->printWhiteMessage(buffer.str().c_str());
}