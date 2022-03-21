#pragma once

enum Direction {
	NORTH = 0,
	RIGHT = 1,
	SOUTH = 2,
	LEFT = 3
};

class Walker {
public:
	Walker(int moduleBase) {
		this->moduleBase = moduleBase;
	}
	int moduleBase;

	void moveRight();
	void moveLeft();
	void moveNorth();
	void moveSouth();
	Direction direction();
	void dash();
	const char* toString();
};