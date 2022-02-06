#include "pch.h"
#include "ContainerItem.h"
#include "tipos.h"
#include "include.h"

extern shared_ptr<set<int>> lootsAmountable;
extern shared_ptr<set<int>> ignoreLoot;

int ContainerItem::id() {
	return *(int*)this->baseAddress;
}

bool ContainerItem::isFilled() {
	return this->amount() >= 0;
}

int ContainerItem::amount() {
	int value = *(int*)(this->baseAddress + 0x4);
	if (this->id() == ItemId::MF) {
		return value == 7 ? 0 : -1; //tibia-olds works like this when has 7 is filled
	}
	return value;
}

bool ContainerItem::isLootable() {
	bool isLootable = lootsAmountable->find(this->id()) != lootsAmountable->end();
	return isLootable;
}

bool ContainerItem::isIgnorable() {
	bool isIgnorable = ignoreLoot->find(this->id()) != ignoreLoot->end();
	return isIgnorable;
}