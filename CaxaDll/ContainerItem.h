#pragma once

class ContainerItem {
public:
	ContainerItem(int baseAddress) {
		this->baseAddress = baseAddress;
	}
	int baseAddress;
	int id();
	int amount();
	bool isFilled();
	bool isLootable();
	bool isIgnorable();
	bool isBpOrBag();
};