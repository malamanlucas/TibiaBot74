#include "pch.h"
#include "include.h"
#include "PlayerStatus.h"
#include "variables.h"

extern shared_ptr<Variables> v;

enum State {
	None = 0,
	Poison = 1,
	Burn = 2,
	Energy = 4,
	Drunk = 8,
	ManaShield = 16,
	Paralyze = 32,
	Haste = 64,
	Swords = 128,
	Drowning = 256,
	Freezing = 512,
	Dazzled = 1024,
	Cursed = 2048,
	PartyBuff = 4096,
	PzBlock = 8192,
	Pz = 16384,
	Bleeding = 32768,
	Hungry = 65536
};

int PlayerStatus::status() {
	return *(int*)this->statusAddress;
}

bool PlayerStatus::isManaShield() {
	int status = this->status();
	if (status >= State::Swords) {
		status -= State::Swords;
	}
	if (status >= State::Haste) {
		status -= State::Haste;
	}
	if (status >= State::Paralyze) {
		status -= State::Paralyze;
	}
	return status >= State::ManaShield;
}

bool PlayerStatus::isHaste() {
	int status = this->status();
	if (status >= State::Swords) {
		status -= State::Swords;
	}
	return status >= State::Haste;
}

bool PlayerStatus::isFighting() {
	int status = this->status();
	return status >= State::Swords;
}

bool PlayerStatus::isParalyzed() {
	int status = this->status();
	if (status >= State::Swords) {
		status -= State::Swords;
	}
	if (status >= State::Haste) {
		status -= State::Haste;
	}
	return status >= State::Paralyze;
}

void PlayerStatus::autoManaShield() {
	if (!this->isManaShield() && v->player->mana() > 50) {
		v->player->say("utamo vita");
	}
}

void PlayerStatus::autoHaste(const char* spell, int manaSpell) {
	if (!this->isHaste() && v->player->mana() > manaSpell) {
		v->player->say(spell);
	}
}

void PlayerStatus::cureParalyze(const char* spell, int manaSpell) {
	if (this->isParalyzed()) {
		v->player->mana() > manaSpell ? v->player->say(spell) : v->player->uhSelf();
	}
}

const char* PlayerStatus::toString() {
	stringstream buffer;
	buffer.str();
	buffer << "status: " << this->status();
	buffer << " manaShield: " << this->isManaShield();
	buffer << " haste: " << this->isHaste();
	buffer << " Fighting: " << this->isFighting();
	buffer << " isParalyzed: " << this->isParalyzed();
	return buffer.str().c_str();
}
