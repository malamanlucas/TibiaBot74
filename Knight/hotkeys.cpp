#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "Player.h"
#include "Containers.h"
#include "BattleList.h"
#include "SocketClient.h"
#include "WindowsApi.h"
#include "variables.h"

extern shared_ptr<Variables> v;

void initHotkeys() {
    while (true) {
        if (v->windowsApi->isFocused()) {

            if (GetAsyncKeyState(VK_SCROLL) & 1) {
                v->autoTasks->running = false;
                v->client->close();
                break;
            }
            v->player->refreshLastTargetId();

            if (GetAsyncKeyState(VK_INSERT) & 1) {
                v->player->uhSelf();
            }
            if (GetAsyncKeyState(VK_END) & 1) {
                v->player->mfSelf();
            }
            if (GetAsyncKeyState(VK_DELETE) & 1) {
                if (v->player->isAttacking()) {
                    v->player->comboTarget(ItemId::SD, v->player->lastTargetId);
                }
                else {
                    v->player->comboEnemy(ItemId::SD);
                }
            }
            if (GetAsyncKeyState(VK_HOME) & 1) {
                /*if (v->player->isAttacking()) {
                    v->player->runeTarget(ItemId::EXPLOSION);
                }
                else {
                    v->player->runeEnemy(ItemId::EXPLOSION);
                }*/
                //v->player->comboTarget(ItemId::HMM, v->player->lastTargetId);
                v->player->paralyzeTarget();
            }
            if (GetAsyncKeyState(VK_NEXT) & 1) { // pageDown
                if (v->player->isAttacking()) {
                    v->player->runeTarget(ItemId::EXPLOSION);
                }
                else {
                    v->player->runeEnemy(ItemId::EXPLOSION);
                }
            }
            if (GetAsyncKeyState(VK_PRIOR) & 1) { // pageup
                v->player->comboTarget(ItemId::HMM, v->player->lastTargetId);
            }
            if (GetAsyncKeyState(VK_F11) & 1) {
                v->battleList->init();
            }

            if (GetAsyncKeyState(VK_F12) & 1) {
                //v->player->sendItemSelf(ItemId::FIRE_BOMB);
                v->autoTasks->showGui = !v->autoTasks->showGui;
            }

        }

        Sleep(50);
    }

}