#pragma once
#include "include.h"
#include "ContainerItem.h"

class Container {
public: 
	Container(int addressBase) {
		this->addressBase = addressBase;
		this->loadItems();
	}
	vector<shared_ptr<ContainerItem>> items;
	void loadItems();
	int addressBase;
	bool isOpened();
	bool isCarrying();
	int id();
	char* name();
	int maxItems();
	int amountItems();
	const char* toString();
};