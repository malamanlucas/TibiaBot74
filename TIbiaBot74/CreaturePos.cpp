#include "pch.h"
#include "CreaturePos.h"
#include "include.h"

int CreaturePos::x() {
	return *(int*)this->xAddress;
}

int CreaturePos::y() {
	return *(int*)this->yAddress;
}

int CreaturePos::z() {
	return *(int*)this->zAddress;
}