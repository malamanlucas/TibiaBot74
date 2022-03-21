#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "Containers.h"
#include "Player.h"
#include "variables.h"

extern shared_ptr<Variables> v;

void Containers::setLastBpPos(int itemId, int bpPos) {
	if (itemId == ItemId::MF) {
		v->infoList->set("lastBpPosMf", bpPos);
	}
	else if (itemId == ItemId::SD) {
		v->infoList->set("lastBpPosSD", bpPos);
		
	}
	this->lastBpPos[itemId] = bpPos;
}

void Containers::openFirstBpOfLastUsedItem(int itemId) {
	if (this->isItemActionable(itemId)) {
		int bpPos = this->lastBpPos[itemId];
		auto firstBp = this->getFirstBp(bpPos);
		if (firstBp != nullptr) {
			v->player->inventory->openContainer(firstBp);
		}
		else {
			this->setLastBpPos(itemId, -2);
		}
	}
}

shared_ptr<ItemSearch> Containers::searchItemAndOpenBp(ItemId itemId) {
	auto itemSearch = this->searchItem(itemId);
	if (itemSearch != nullptr) {
		return itemSearch;
	}
	else {
		this->openFirstBpOfLastUsedItem(itemId);
		auto itemSearch = this->searchItem(itemId);
		if (itemSearch != nullptr) {
			return itemSearch;
		}
	}
	this->setLastBpPos(itemId, -2);
	return nullptr;
}

void Containers::init() {	
	for (int i = 0; i < 4000; i++) this->lastBpPos[i] = -1;
	this->lastBpPos[ItemId::MF] = -2;
	this->lastBpPos[ItemId::SD] = -2;

	this->idsBackpack = make_shared<set<int>>();
	this->lootsAmountable = make_shared<set<int>>();
	this->ignoreLoot = make_shared<set<int>>();
	ifstream input;

	input.open("C:\\ferramentas\\idsBackpack.txt");
	int a;
	while (input >> a) {
		this->idsBackpack->insert(a);
	}
	input.close();

	input.open("C:\\ferramentas\\lootsAmountable.txt");
	while (input >> a) {
		this->lootsAmountable->insert(a);
	}
	input.close();

	input.open("C:\\ferramentas\\ignoreLoots.txt");
	while (input >> a) {
		this->ignoreLoot->insert(a);
	}

	input.close();
}

bool Containers::hasAnyOwnContainerOpened() {
	this->loadContainers();
	for (auto containerIndex = this->containers.begin(); containerIndex < this->containers.end(); containerIndex++) {
		auto container = containerIndex->get();
		if (container->isOpened()) {
			return true;
		}
	}
	return false;
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
	v->player->printWhiteMessage(buffer.str().c_str());
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

bool Containers::isItemActionable(int itemId) {
	return (itemId == ItemId::MF || itemId == ItemId::SD) && this->lastBpPos[itemId] != -2;
}

shared_ptr<ItemSearch> Containers::getFirstBp(int bpPos) {
	int slotPos = 0;
	//for (auto containerIndex = this->containers.begin(); containerIndex < this->containers.end(); containerIndex++) {
	auto container = this->containers[bpPos];
	if (container->isOpened()) {
		slotPos = 0;
		for (auto item : container->items) {
			if (item->isBpOrBag()) {
				//v->player->inventory->openContainer(this->buildItemSearch(item.get(), bpPos, slotPos), ContainerLocation::INSIDE);
				return this->buildItemSearch(item.get(), bpPos, slotPos);
			}
			slotPos++;
		}
	}
	//}
	return nullptr;
}

shared_ptr<ItemSearch> Containers::searchItem(ItemId itemId) {
	this->loadContainers();
	int bpPos = 0;
	int slotPos = 0;
	for (auto containerIndex = this->containers.begin(); containerIndex < this->containers.end(); containerIndex++, bpPos++) {
		auto container = containerIndex->get();
		if (container->isOpened()) {
			slotPos = 0;
			for (auto item : container->items) {
				if (item->id() == itemId && item->isFilled()) {
					return this->buildItemSearch(item.get(), bpPos, slotPos);
				}
				slotPos++;
			}
		}
	}
	return nullptr;
}

void Containers::updateInfoList(int itemId, int bpPos) {
	if (itemId == ItemId::MF || itemId == ItemId::SD) {
		this->setLastBpPos(itemId, bpPos);
	}
}

shared_ptr<ItemSearch> Containers::buildItemSearch(ContainerItem* item, int bpPos, int slotPos) {
	auto itemSearch = make_shared<ItemSearch>();
	itemSearch->amount = item->amount();
	itemSearch->slotPos = slotPos;
	itemSearch->id = item->id();
	itemSearch->bpPos = bpPos;
	this->updateInfoList(item->id(), bpPos);
	return itemSearch;
}

shared_ptr<ItemSearch> Containers::searchItem(shared_ptr<set<int>> itemsId) {
	this->loadContainers();
	int bpPos = 0;
	int slotPos = 0;
	for (auto containerIndex = this->containers.begin(); containerIndex < this->containers.end(); containerIndex++, bpPos++) {
		auto container = containerIndex->get();
		if (container->isOpened()) {
			slotPos = 0;
			for (auto itemIndex = container->items.begin(); itemIndex < container->items.end(); itemIndex++, slotPos++) {
				auto item = itemIndex->get();
				bool found = itemsId->find(item->id()) != itemsId->end();
				if (found && item->isFilled()) {
					return this->buildItemSearch(item, bpPos, slotPos);
				}
			}
		}
	}
	return nullptr;
}

void Containers::drawLoot() {
	this->loadContainers();
	int bpPos = this->containers.size() - 1;
	int slotPos = 0;
	for (auto containerIndex = this->containers.rbegin(); containerIndex < this->containers.rend(); containerIndex++, bpPos--) {
		auto container = containerIndex->get();
		if (container->isOpened() && container->isDeadMonster()) {
			slotPos = container->items.size() - 1;
			for (auto itemIndex = container->items.rbegin(); itemIndex < container->items.rend(); itemIndex++, slotPos--) {
				if (!container->isOpened() || !container->isDeadMonster())	break;
				auto item = itemIndex->get();
				if (item->isLootable()) {
					auto itemSearch = make_shared<ItemSearch>();
					itemSearch->amount = item->amount();
					itemSearch->bpPos = bpPos;
					itemSearch->slotPos = slotPos;
					itemSearch->id = item->id();
					v->player->moveItemToBackpack(itemSearch);
				}
				else if (!item->isIgnorable()) {
					auto itemSearch = make_shared<ItemSearch>();
					itemSearch->amount = item->amount();
					itemSearch->bpPos = bpPos;
					itemSearch->slotPos = slotPos;
					itemSearch->id = item->id();
					v->player->moveItemToFeet(itemSearch);
				}
			}
		}
	}
}