#pragma once
#include "include.h"
#include "tipos.h"

class Inventory {
public:
	Inventory(int moduleBase) {
		this->baseAddress = *(int*)(moduleBase + 0x0A4478);
		this->init();
	}
	shared_ptr<set<int>> aols;

	int baseAddress;
	void init();
	int neck();
	int finger();
	int ammo();
	int ammoAmount();
	const char* toString();
	void equipRing(ItemId item, ItemId itemUsed);
	void equipAmulet(ItemId item);
	void equipAnyAmulet(shared_ptr<set<int>> items);
	void useEnergyRing();
	void useRingOfHealing();
	void useTimeRing();
	void useVipRing();
	void useSSA();
	void useAnyAOL();
	bool isUsingRing(ItemId itemUsed);
	bool isUsingAmulet(ItemId item);
	bool isUsingAnyAmulet(shared_ptr<set<int>> items);
	void useEnergyRingIfLowHp();
	void refillBurstArrows();
	void refillPosionArrow();
	void refillAmmo(ItemId itemId, int minQuantity);

};