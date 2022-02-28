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

            if (GetAsyncKeyState(VK_F1) & 1) {
                v->player->uhSelf();
            }
            if (GetAsyncKeyState(VK_F2) & 1) {
                v->player->runeTarget(ItemId::SD);
            }
            if (GetAsyncKeyState(VK_F3) & 1) {
                v->player->mfSelf();
            }

        }

        Sleep(50);
    }

}