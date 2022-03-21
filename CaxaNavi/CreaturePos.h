#pragma once

class CreaturePos {
public:
	CreaturePos(int zAddress) {
		this->zAddress = zAddress;
		this->xAddress = zAddress - 8;
		this->yAddress = zAddress - 4;
	}

	CreaturePos(int zAddress, int xAddress, int yAddress) {
		this->zAddress = zAddress;
		this->xAddress = xAddress;
		this->yAddress = yAddress;
	}
	int zAddress;
	int xAddress;
	int yAddress;
	int moduleBase;

	int x();
	int y();
	int z();
	const char* toString();
};
