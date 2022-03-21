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

            if (GetAsyncKeyState(VK_F10) & 1) {
                v->player->uhSelf();
            }
            if (GetAsyncKeyState(VK_F11) & 1) {
                v->player->mfSelf();
            }

            //v->player->walker->dash();

            if (GetAsyncKeyState(VK_F9) && 1) {
                v->player->sdTarget();
            }
            if (GetAsyncKeyState(VK_MULTIPLY) & 1) {
                v->player->inventory->useSSA();
            }

            if (GetAsyncKeyState(VK_F11) & 1) {
                v->battleList->init();
            }

            if (GetAsyncKeyState(VK_F12) & 1) {
                //v->player->sendItemSelf(ItemId::FIRE_BOMB);
                v->autoTasks->showGui = !v->autoTasks->showGui;
            
            }

            Sleep(10);
        }

    }

}