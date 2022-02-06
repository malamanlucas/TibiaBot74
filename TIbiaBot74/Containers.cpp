#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "Containers.h"
#include "Player.h"

extern shared_ptr<Player> player;
extern shared_ptr<set<int>> idsBackpack;
extern shared_ptr<set<int>> lootsAmountable;
extern shared_ptr<set<int>> ignoreLoot;

void Containers::init() {
	idsBackpack = make_shared<set<int>>();
	lootsAmountable = make_shared<set<int>>();
	ignoreLoot = make_shared<set<int>>();
	ifstream input;

	input.open("C:\\ferramentas\\idsBackpack.txt");
	int a;
	while (input >> a) {
		idsBackpack->insert(a);
	}
	input.close();

	input.open("C:\\ferramentas\\lootsAmountable.txt");
	while (input >> a) {
		lootsAmountable->insert(a);
	}

	input.open("C:\\ferramentas\\ignoreLoots.txt");
	while (input >> a) {
		ignoreLoot->insert(a);
	}

	input.close();
}

void Containers::readContainersOpened() {
	stringstream buffer;
	buffer.str();
	for (auto containerIndex = this->containers.begin(); containerIndex < this->containers.end(); containerIndex++) {
		auto container = containerIndex->get();
		if (container->isOpened()) {
			buffer << "[" << container->id();
			buffer << "," << container->name();
			buffer << "," << container->isInside();
			buffer << "," << container->isDeadMonster();
			buffer << "] ";
		}
	}
	player->printWhiteMessage(buffer.str().c_str());
}

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

void Containers::drawLoot() {
	int bpPos = 0;
	int slotPos = 0;
	for (auto containerIndex = this->containers.begin(); containerIndex < this->containers.end(); containerIndex++, bpPos++) {
		auto container = containerIndex->get();
		if (container->isOpened() && container->isDeadMonster()) {
			slotPos = 0;
			for (auto itemIndex = container->items.begin(); itemIndex < container->items.end(); itemIndex++, slotPos++) {
				auto item = itemIndex->get();
				if (item->isLootable()) {
					auto itemSearch = make_shared<ItemSearch>();
					itemSearch->amount = item->amount();
					itemSearch->bpPos = bpPos;
					itemSearch->slotPos = slotPos;
					itemSearch->id = item->id();
					player->moveItemTobackpack(itemSearch);
				}
				
			}
		}
	}
}