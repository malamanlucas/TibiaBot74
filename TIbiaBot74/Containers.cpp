#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "Containers.h"
#include "Player.h"

extern shared_ptr<Player> player;

void Containers::loadContainers() {
	int i = 0;
	int value = 0;
	/*stringstream buffer;
	buffer.str();*/
	this->containers.clear();
	while (true) {
		int actualAddress = this->containerStartAddress + (this->containerOffset * i);
		value = *(int*)(actualAddress);
		if (value == 0) { // is no more any container found
			break;
		}
		auto container = make_shared<Container>(actualAddress);
		this->containers.push_back(container);
		//buffer << container->toString();
		i++;
	}
	// cout << buffer.str().c_str() << endl;
}

shared_ptr<ItemSearch> Containers::searchItem(ItemId itemId) {
	auto itemSearch = make_shared<ItemSearch>();
	int bpPos = 0;
	int slotPos = 0;
	for (auto containerIndex = this->containers.begin(); containerIndex < this->containers.end(); containerIndex++, bpPos++) {
		auto container = containerIndex->get();
		if (container->isOpened()) {
			slotPos = 0;
			for (auto itemIndex = container->items.begin(); itemIndex < container->items.end(); itemIndex++, slotPos++) {
				auto item = itemIndex->get();
				if (item->id() == itemId && item->isFilled()) {
					itemSearch->amount = item->amount();
					itemSearch->slotPos = slotPos;
					itemSearch->id = itemId;
					itemSearch->bpPos = bpPos;
					/*if (itemSearch->bpPos == 0) {
						if (container->isCarrying()) {
							itemSearch->bpPos = bpPos + 1;
						} else {
							itemSearch->bpPos = bpPos;
						}
					}*/
					return itemSearch;
				}
			}
		}
	}
	return nullptr;
}