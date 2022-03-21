#include "pch.h"
#include "include.h"
#include "Walker.h"

Direction Walker::direction() {
	int danceAddress = *(int*) (this->moduleBase + 0x0051B2C);
	return (Direction) *(int*) (danceAddress + 0x18);
}

const char* Walker::toString() {
	stringstream buffer;
	buffer.str();
	buffer << " direction:" << this->direction();
	return buffer.str().c_str();
}

void Walker::dash() {
	if ((GetAsyncKeyState('w') || GetAsyncKeyState('W'))) {
		this->moveNorth();
	}
	if ((GetAsyncKeyState('s') || GetAsyncKeyState('S'))) {
		this->moveSouth();
	}
	if ((GetAsyncKeyState('d') || GetAsyncKeyState('D'))) {
		this->moveRight();
	}
	if ((GetAsyncKeyState('a') || GetAsyncKeyState('A'))) {
		this->moveLeft();
	}
	/*if (GetAsyncKeyState(VK_DELETE) && 1) {
		Direction direction = this->direction();
		switch (direction) {
		case Direction::NORTH: {
			this->moveNorth();
			break;
		}
		case Direction::LEFT: {
			this->moveLeft();
			break;
		}
		case Direction::RIGHT: {
			this->moveRight();
			break;
		}
		case Direction::SOUTH: {
			this->moveSouth();
			break;
		}
		}
	}*/
}

void Walker::moveRight() {
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0x66);

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}

void Walker::moveLeft() {
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0x68);

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}

void Walker::moveNorth() {
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0x65);

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}

void Walker::moveSouth() {
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0x67);

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}