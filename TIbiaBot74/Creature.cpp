#include "pch.h"
#include "Creature.h"
#include "include.h"

int Creature::healthPercent() {
	return *(int*)(this->baseAddres + 0x84);
}

int Creature::id() {
	return *(int*)(this->baseAddres);
}

int Creature::speed() {
	return *(int*)(this->baseAddres + 0x88);
}

char* Creature::name() {
	return (char*)(this->baseAddres + 0x4);
}

int Creature::isOnline() {
	return *(int*)(this->baseAddres + 0x8C);
}

const char* Creature::toString() {
	stringstream buffer;
	buffer.str();
	buffer << "name: " << this->name();
	buffer << " x: " << this->pos->x();
	buffer << " y: " << this->pos->y();
	buffer << " z: " << this->pos->z();
	buffer << " " << this->healthPercent() << "%  - ";
	return buffer.str().c_str();
}
