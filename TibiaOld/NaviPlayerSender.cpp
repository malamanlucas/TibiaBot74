#include "pch.h"
#include "include.h"
#include "NaviPlayerSender.h"
#include "variables.h"

extern shared_ptr<Variables> v;

void NaviPlayerSender::comboTarget(ItemId runeId) {
	this->comboTarget(runeId, v->player->lastTargetId);
}

void NaviPlayerSender::comboTarget(ItemId runeId, int creatureId) {
	ostringstream buf;
	buf << "/" << (char) NavigationAction::DEFAULT << " " << runeId << " " << creatureId;
	v->client->sendMsg(buf.str().c_str());
}

void NaviPlayerSender::comboEnemy(ItemId runeId) {
	auto creature = v->battleList->findEnemy();
	if (creature != nullptr) {
		ostringstream buf;
		buf << "/" << (char) NavigationAction::DEFAULT << " " << runeId << " " << creature->id();
		v->client->sendMsg(buf.str().c_str());
	}
}

void NaviPlayerSender::comboParaTarget(ItemId runeId) {
	this->comboParaTarget(v->player->name(), runeId);
}

void NaviPlayerSender::comboParaTarget(const char* druidName, ItemId runeId) {
	ostringstream buf;
	buf << "/" << (char)NavigationAction::PARALYZE << " " << runeId << " " << v->player->lastTargetId << " " << druidName;
	v->client->sendMsg(buf.str().c_str());
}

void NaviPlayerSender::comboParaEnemy(const char* druidName, ItemId runeId) {
	auto creature = v->battleList->findEnemy();
	if (creature != nullptr) {
		ostringstream buf;
		buf << "/" << (char) NavigationAction::PARALYZE << " " << runeId << " " << creature->id() << " " <<  druidName;
		v->client->sendMsg(buf.str().c_str());
	}
}

void NaviPlayerSender::comboParaEnemy(ItemId runeId) {
	this->comboParaTarget(v->player->name(), runeId);
}