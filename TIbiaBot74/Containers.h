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
	int moduleBase;
	int containerStartAddress;
	int containerOffset;
	vector<shared_ptr<Container>> containers;
	
	void init();
	void loadContainers();
	void readContainersOpened();
	void drawLoot();
	shared_ptr<ItemSearch> searchItem(ItemId itemId);
	shared_ptr<ItemSearch> searchItem(shared_ptr<set<int>> itemsId);
};