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
                //v->autoTasks->running = false;
                v->client->close();
                Sleep(500);
                //break;
            }
            v->player->refreshLastTargetId();

            if (GetAsyncKeyState(VK_INSERT) & 1) {
                v->player->uhSelf();
            }
            if (GetAsyncKeyState(VK_END) & 1) {
                v->player->mfSelf();
            }

            if (GetAsyncKeyState(VK_DELETE) && 1) {
                if (!v->client->isConnected()) {
                    if (v->player->isAttacking())
                        v->player->sdTarget();
                    else
                        v->player->runeEnemy(ItemId::SD);
                }
                else {
                    if (v->player->isAttacking())
                        v->player->naviSender->comboTarget(ItemId::SD);
                    else
                        v->player->naviSender->comboEnemy(ItemId::SD);
                }
            }
            if (GetAsyncKeyState(VK_HOME) & 1) {
                v->player->inventory->useSSA();
            }
            if (GetAsyncKeyState(VK_NEXT) & 1) { // pageDown
                if (v->player->inventory->isUsingRing(ItemId::U_TIME_RING)) {
                    v->player->inventory->useVipRing();
                }
                else {
                    v->player->inventory->useTimeRing();
                }
            }
            if (GetAsyncKeyState(VK_PRIOR) & 1) { // pageup
                if (!v->client->isConnected()) {
                    if (v->player->isAttacking())
                        v->player->paralyzeTarget();
                    else
                        v->player->runeEnemy(ItemId::SD);
                }
                else {
                    if (v->player->isAttacking())
                        v->player->naviSender->comboParaTarget(ItemId::PARALYSE);
                    else
                        v->player->naviSender->comboParaEnemy(ItemId::SD);
                }
            }
            if (GetAsyncKeyState(VK_F11) & 1) {
                v->battleList->init();
            }

            if (GetAsyncKeyState(VK_F12) & 1) {
                v->autoTasks->showGui = !v->autoTasks->showGui;
            }

            Sleep(10);
        }

    }

}