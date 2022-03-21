#include "pch.h"
#include "include.h"
#include "NaviPlayerReader.h"
#include "variables.h"

extern shared_ptr<Variables> v;

void NaviPlayerReader::process(string* commands) {
	string druidName;
	char action = commands[0][0];
	int itemId = stoi(commands[1]);
	int creatureId = stoi(commands[2]);
	cout << "action: " << action << ", ";
	cout << "itemId: " << itemId << ", ";
	cout << "creatureId: " << creatureId << ", ";
	if (action == NavigationAction::PARALYZE) {
		druidName = commands[3];
		cout << "druidName: " << druidName;
		if (strcmp(v->player->name(), druidName.c_str()) == 0) { //is not druid requested to paralyze
			this->doParalyze(itemId, creatureId);
		}
		else {
			v->player->runeCreatureId((ItemId)itemId, creatureId);
		}
		//v->player->runeCreatureId((ItemId)itemId, creatureId);
	}
	else if (action == NavigationAction::DEFAULT) {
		v->player->runeCreatureId((ItemId)itemId, creatureId);
	}
	cout << endl;
}

void NaviPlayerReader::doParalyze(int itemId, int creatureId) {
	if (v->player->canShootParalyze()) {
		if (!v->battleList->isParalyzed(creatureId)) {
			v->player->runeCreatureId(ItemId::PARALYSE, creatureId);
		}
	}
	else {
		v->player->runeCreatureId((ItemId)itemId, creatureId);
	}
}