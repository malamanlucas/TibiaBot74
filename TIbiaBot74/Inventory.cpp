#include "pch.h"
#include "Inventory.h"
#include "include.h"
#include "Player.h"
#include "Containers.h"

extern shared_ptr<Player> player;
extern shared_ptr<Containers> containers;

void Inventory::init() {
	this->aols = make_shared<set<int>>();
	this->aols->insert(ItemId::AOL_VIP);
	this->aols->insert(ItemId::AOL);
}

int Inventory::neck() {
	return *(int*)(this->baseAddress + 0x8);
}

int Inventory::finger() {
	return *(int*)(this->baseAddress + 0x5C);
}

int Inventory::ammo() {
	return *(int*)(this->baseAddress + 0x68);
}

int Inventory::ammoAmount() {
	return *(int*)(this->baseAddress + 0x6C);
}

bool Inventory::isUsingRing(ItemId itemUsed) {
	return this->finger() == itemUsed;
}

bool Inventory::isUsingAmulet(ItemId item) {
	return this->neck() == item;
}

bool Inventory::isUsingAnyAmulet(shared_ptr<set<int>> items) {
	bool found = items->find(this->neck()) != items->end();
	return found;
}

void Inventory::equipRing(ItemId item, ItemId itemUsed) {
	if (!this->isUsingRing(itemUsed)) {
		auto itemSearch = containers->searchItem(item);
		if (itemSearch != nullptr) {
			cout << "moving item to ring" << endl;
			int xTo = 65535;
			int yTo = InventoryType::Finger;
			int zTo = 0;
			player->moveItem(itemSearch, xTo, yTo, zTo);
		}
	}
}

void Inventory::equipAmulet(ItemId item) {
	if (!this->isUsingAmulet(item)) {
		auto itemSearch = containers->searchItem(item);
		if (itemSearch != nullptr) {
			cout << "moving item to neck" << endl;
			int xTo = 65535;
			int yTo = InventoryType::Neck;
			int zTo = 0;
			player->moveItem(itemSearch, xTo, yTo, zTo);
		}
	}
}

void Inventory::equipAnyAmulet(shared_ptr<set<int>> items) {
	if (!this->isUsingAnyAmulet(items)) {
		auto itemSearch = containers->searchItem(items);
		if (itemSearch != nullptr) {
			cout << "moving item to neck" << endl;
			int xTo = 65535;
			int yTo = InventoryType::Neck;
			int zTo = 0;
			player->moveItem(itemSearch, xTo, yTo, zTo);
		}
	}
}

void Inventory::useEnergyRing() {
	this->equipRing(ItemId::ENERGY_RING, ItemId::U_ENERGY_RING);
}
void Inventory::useRingOfHealing() {
	this->equipRing(ItemId::RING_OF_HEALING, ItemId::U_RING_OF_HEALING);
}
void Inventory::useVipRing() {
	this->equipRing(ItemId::VIP_RING_MAGE, ItemId::VIP_RING_MAGE);
}
void Inventory::useSSA() {
	this->equipAmulet(ItemId::SSA);
}
void Inventory::useAnyAOL() {
	this->equipAnyAmulet(this->aols);
}

void Inventory::useEnergyRingIfLowHp() {
	if (player->healthPercent() <= 70) {
		this->useEnergyRing();
	} else {
		this->useVipRing();
	}
}

void Inventory::useTimeRing() {
	this->equipRing(ItemId::TIME_RING, ItemId::U_TIME_RING);
}

void Inventory::refillBurstArrows() {
	this->refillAmmo(ItemId::BURST_ARROW, 20);
}

void Inventory::refillPosionArrow() {
	this->refillAmmo(ItemId::POISION_ARROW, 20);
}

void Inventory::refillAmmo(ItemId itemId, int minQuantity) {
	if (this->ammo() == 0 || (this->ammo() == itemId && this->ammoAmount() < minQuantity)) {
		auto itemSearch = containers->searchItem(itemId);
		if (itemSearch != nullptr) {
			int xTo = 65535;
			int yTo = InventoryType::Ammo;
			int zTo = 0;
			player->moveItem(itemSearch, xTo, yTo, zTo);
		}
	}
}

const char* Inventory::toString() {
	stringstream buffer;
	buffer.str();
	buffer << "[neck:" << this->neck();
	buffer << ",finger:" << this->finger();
	buffer << ",ammo:" << this->ammo();
	buffer << ",ammoAmount:" << this->ammoAmount();
	buffer << "]";
	return buffer.str().c_str();
} 