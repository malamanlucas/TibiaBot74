#pragma once
#include "tipos.h"

class NaviPlayerSender {
public:
	void comboTarget(ItemId runeId);
	void comboTarget(ItemId runeId, int creatureId);
	void comboEnemy(ItemId runeId);

	void comboParaTarget(ItemId runeId);
	void comboParaTarget(const char* druidName, ItemId runeId);
	void comboParaEnemy(const char* druidName, ItemId runeId);
	void comboParaEnemy(ItemId runeId);
};