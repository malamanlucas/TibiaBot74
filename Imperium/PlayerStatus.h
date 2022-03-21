#pragma once


class PlayerStatus {

public:
	PlayerStatus(int moduleBase) {
		this->statusAddress = (moduleBase + 0x1C67D8);
	}
	int statusAddress;
	int status();
	bool isManaShield();
	bool isHaste();
	bool isFighting();
	bool isParalyzed();
	void autoManaShield();
	void autoHaste(const char* spell, int manaSpell);
	void cureParalyze(const char* spell, int manaSpell);
	const char* toString();

};