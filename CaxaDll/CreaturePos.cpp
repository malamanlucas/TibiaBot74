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

const char* CreaturePos::toString() {
	ostringstream buf;
	buf << "[x: " << this->x();
	buf << ",y: " << this->y();
	buf << ",z: " << this->z() << "]";
	return buf.str().c_str();
}