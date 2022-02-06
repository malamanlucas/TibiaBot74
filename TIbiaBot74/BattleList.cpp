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
		if (creature->id() == creatureId 
			&& creature->pos->z() == player->myPos->z() 
			&& abs(creature->pos->x() == player->myPos->x()) <= 7
			&& abs(creature->pos->y() == player->myPos->y()) <= 5
			&& abs(creature->pos->y() == player->myPos->y()) <= 5
			&& player->healthPercent() > 0) {
			return creature;
		}
		i++;
	}
	return nullptr;
}