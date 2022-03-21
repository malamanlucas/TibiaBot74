#include "pch.h"
#include "ContainerItem.h"
#include "tipos.h"
#include "include.h"
#include "variables.h"

extern shared_ptr<Variables> v;

int ContainerItem::id() {
	return *(int*)this->baseAddress;
}

bool ContainerItem::isFilled() {
	return this->amount() >= 0;
}

bool ContainerItem::isBpOrBag() {
	return v->containers->idsBackpack->find(this->id()) != v->containers->idsBackpack->end();
}

int ContainerItem::amount() {
	int value = *(int*)(this->baseAddress + 0x4);
	if (this->id() == ItemId::MF) {
		return value == 7 ? 0 : -1; //tibia-olds works like this when has 7 is filled
	}
	return value;
}

bool ContainerItem::isLootable() {
	bool isLootable = v->containers->lootsAmountable->find(this->id()) != v->containers->lootsAmountable->end();
	return isLootable;
}

bool ContainerItem::isIgnorable() {
	bool isIgnorable = v->containers->ignoreLoot->find(this->id()) != v->containers->ignoreLoot->end();
	return ItemId::BAG == this->id() || isIgnorable;
}