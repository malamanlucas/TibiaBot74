#pragma once
#include "include.h"
#include "ItemSearch.h"
#include "Container.h"
#include "include.h"

class Containers {
public:
	Containers(int moduleBase) {
		this->moduleBase = moduleBase;
		this->containerStartAddress = *(int*)(moduleBase + 0xA4F0C);
		this->containerOffset = 0x1EC;;
		this->init();
	}
	int lastBpPos[9999];
	shared_ptr<set<int>> idsBackpack;
	shared_ptr<set<int>> lootsAmountable;
	shared_ptr<set<int>> ignoreLoot;
	int moduleBase;
	int containerStartAddress;
	int containerOffset;
	vector<shared_ptr<Container>> containers;
	
	void setLastBpPos(int itemId, int bpPos);
	bool isItemActionable(int itemId);
	shared_ptr<ItemSearch> getFirstBp(int bpPos);
	bool hasAnyOwnContainerOpened();
	void init();
	void loadContainers();
	void readContainersOpened();
	void drawLoot();
	shared_ptr<ItemSearch> searchItem(ItemId itemId);
	shared_ptr<ItemSearch> searchItemAndOpenBp(ItemId itemId);
	shared_ptr<ItemSearch> searchItem(shared_ptr<set<int>> itemsId);
	shared_ptr<ItemSearch> buildItemSearch(ContainerItem* item, int bpPos, int slotPos);
	void updateInfoList(int itemId, int bpPos);
	void openFirstBpOfLastUsedItem(int itemId);
};