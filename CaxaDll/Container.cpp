#include "pch.h"
#include "Container.h"
#include "ContainerItem.h"
#include "tipos.h"
#include "variables.h"

extern shared_ptr<Variables> v;

bool Container::isOpened() {
	return *(int*)(this->addressBase - 0x4);
}

const char* Container::toString() {
	stringstream buffer;
	buffer.str();
	buffer << "[isOpened: " << this->isOpened();
	buffer << ", isDeadMonster: " << this->isDeadMonster();
	buffer << " id: " << this->id() << " |";
	for (auto item = this->items.begin(); item < this->items.end(); item++) {
		buffer << "{" << item->get()->id() << "," << item->get()->amount() << "},";
	}
	buffer << " | maxItems: " << this->maxItems() << "] - ";
	return buffer.str().c_str();
}

void Container::loadItems() {
	this->items.clear();
	int offsetItem = 0xC;
	int itemStartAddress = this->addressBase + 0x38;
	for (int i = 0; i < this->amountItems(); i++) {
		int address = itemStartAddress + (offsetItem * i);
		this->items.push_back(make_shared<ContainerItem>(address));
	}
}

bool Container::isDeadMonster() {
	bool isNotBackpack = v->containers->idsBackpack->find(this->id()) == v->containers->idsBackpack->end();
	return this->id() == ItemId::BAG || isNotBackpack;
}

bool Container::isInside() {
	int value = *(int*)(this->addressBase + 0x30);
	return value == 1;
}

int Container::maxItems() {
	return *(int*)(this->addressBase + 0x2C);
}

int Container::amountItems() {
	return *(int*)(this->addressBase + 0x34);
}

int Container::id() {
	return *(int*)(this->addressBase);
}

char* Container::name() {
	return (char*)(this->addressBase + 0xC);
}