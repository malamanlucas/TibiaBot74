#include "pch.h"
#include "Inventory.h"
#include "include.h"
#include "Player.h"
#include "Containers.h"
#include "variables.h"

extern shared_ptr<Variables> v;

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
		auto itemSearch = v->containers->searchItem(item);
		if (itemSearch != nullptr) {
			cout << "moving item to ring" << endl;
			int xTo = 65535;
			int yTo = InventoryType::Finger;
			int zTo = 0;
			v->player->moveItem(itemSearch, xTo, yTo, zTo);
		}
	}
}

void Inventory::equipAmulet(ItemId item) {
	if (!this->isUsingAmulet(item)) {
		auto itemSearch = v->containers->searchItem(item);
		if (itemSearch != nullptr) {
			cout << "moving item to neck" << endl;
			int xTo = 65535;
			int yTo = InventoryType::Neck;
			int zTo = 0;
			v->player->moveItem(itemSearch, xTo, yTo, zTo);
		}
	}
}

void Inventory::equipAnyAmulet(shared_ptr<set<int>> items) {
	if (!this->isUsingAnyAmulet(items)) {
		auto itemSearch = v->containers->searchItem(items);
		if (itemSearch != nullptr) {
			cout << "moving item to neck" << endl;
			int xTo = 65535;
			int yTo = InventoryType::Neck;
			int zTo = 0;
			v->player->moveItem(itemSearch, xTo, yTo, zTo);
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
	if (v->player->healthPercent() <= 70) {
		this->useEnergyRing();
	}
	else {
		this->useVipRing();
	}
}

void Inventory::useTimeRing() {
	this->equipRing(ItemId::TIME_RING, ItemId::U_TIME_RING);
}

void Inventory::refillBurstArrows() {
	this->refillAmmo(ItemId::BURST_ARROW, 20);
}

void Inventory::refillPowerBolts() {
	this->refillAmmo(ItemId::POWER_BOLT, 20);
}

void Inventory::refillPosionArrow() {
	this->refillAmmo(ItemId::POISION_ARROW, 20);
}

void Inventory::refillAmmo(ItemId itemId, int minQuantity) {
	if (this->ammo() == 0 || (this->ammo() == itemId && this->ammoAmount() < minQuantity)) {
		auto itemSearch = v->containers->searchItem(itemId);
		if (itemSearch != nullptr) {
			int xTo = 65535;
			int yTo = InventoryType::Ammo;
			int zTo = 0;
			v->player->moveItem(itemSearch, xTo, yTo, zTo);
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

#define ANYTHING 2874

void Inventory::openMainContainer() {
	if (!v->containers->hasAnyOwnContainerOpened()) {
		if (this->isCarryingContainer()) {
			int fromBp = 3;
			this->openContainer(fromBp, 0);
		}
	}
}

void Inventory::openContainer(shared_ptr<ItemSearch> itemSearch) {	
	cout << "[opening itemSearch, bpPos:" << itemSearch->bpPos << ", slotPos:" << itemSearch->slotPos << endl;
	int fromBp = 64 + itemSearch->bpPos;
	this->openContainer(fromBp, itemSearch->slotPos);
}

bool Inventory::isCarryingContainer() {
	int containerValue =  *(int*)(this->baseAddress + 0x14);
	return containerValue != 0;
}

void Inventory::openContainer(int bpPos, int slotPos) {
	int id = ANYTHING;
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB2A0)(0x82);
	//pos = Position(0xFFFF, 0, 0); //means that is an item in inventory
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(65535); //xPos Item means

	//64 first bp opened, 65 second bp opened, and so on...
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(bpPos);

	//0 first pos, 1 second pos
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(slotPos);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB6E0)(id); //uh

	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(slotPos);

	//50 (amountItems)
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCB540)(bpPos - 64);
	reinterpret_cast<void(__cdecl*)(int)>(this->moduleBase + 0xCBE00)(1);
}