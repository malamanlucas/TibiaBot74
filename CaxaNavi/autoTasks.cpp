#include "pch.h"
#include "variables.h"
#include "include.h"

extern shared_ptr<Variables> v;

void AutoTasks::enableAutoTask(AutoTask* autoTask) {
	while (true) {
		if (autoTask->flag && v->player->isOnline()) {
			autoTask->func();
		}
		Sleep(400);
	}
}

void AutoTasks::joinThreads() {
	for (auto thread = threads.begin(); thread != threads.end(); thread++) {
		thread->join();
	}
}

thread buildThread() {
	thread t([]() {
		v->autoTasks->enableAutoTask(v->autoTasks->tasks[v->autoTasks->i]);
	});
	return t;
}

void AutoTasks::enableAllConditionally() {

	this->tasks.push_back(new AutoTask("autoMf", []() {
		if (v->player->manaPercent() < 90) {
			v->player->mfSelf();
		}
		}));


	this->tasks.push_back(new AutoTask("autoFollow", []() {
		v->player->followPlayer("Antocolle");
		}));

	this->tasks.push_back(new AutoTask("sdTarget", []() {
		v->player->sdTarget();
		}));

	this->tasks.push_back(new AutoTask("manaShield", []() {
		v->player->status->autoManaShield();
		}));

	this->tasks.push_back(new AutoTask("granHur", []() {
		v->player->status->autoHaste("utani gran hur", 100);
		}));

	this->tasks.push_back(new AutoTask("autoHur", []() {
		v->player->status->autoHaste("utani hur", 60);
		}));

	this->tasks.push_back(new AutoTask("antiParalyze", []() {
		v->player->status->cureParalyze("exura", 25);
		}));

	this->tasks.push_back(new AutoTask("comboSD", []() {
		if (v->player->isAttacking()) {
			v->player->naviSender->comboTarget(ItemId::SD);
		}
		else {
			v->player->naviSender->comboEnemy(ItemId::SD);
		}
		}));

	this->tasks.push_back(new AutoTask("autoHealFriend", []() {
		v->player->autoHealFriend();
		}));

	this->tasks.push_back(new AutoTask("autoHeal", []() {
		v->player->autoHeal();
		}));

	this->tasks.push_back(new AutoTask("energyRingLowHp", []() {
		v->player->inventory->useEnergyRingIfLowHp();
		}));

	this->tasks.push_back(new AutoTask("ringOfHealing", []() {
		v->player->inventory->useRingOfHealing();
		}));

	this->tasks.push_back(new AutoTask("autoSSA", []() {
		v->player->inventory->useSSA();
		}));

	for (int i = 0; i < tasks.size(); i++) {
		this->threads.push_back(thread([](int index) {
			v->autoTasks->enableAutoTask(v->autoTasks->tasks[index]);
		}, i));
		v->autoTasks->i++;
	}
	
}

