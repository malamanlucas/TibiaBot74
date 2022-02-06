#include "pch.h"
#include "ContainerItem.h"
#include "tipos.h"

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