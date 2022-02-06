#pragma once
#include "include.h"
#include "ItemSearch.h"
#include "Container.h"

class Containers {
public:
	Containers(int moduleBase) {
		this->moduleBase = moduleBase;
		this->containerStartAddress = *(int*)(moduleBase + 0xA4F0C);
		this->containerOffset = 0x1EC;
	}
	int moduleBase;
	int containerStartAddress;
	int containerOffset;
	vector<shared_ptr<Container>> containers;
	
	void loadContainers();
	shared_ptr<ItemSearch> searchItem(ItemId itemId);
};