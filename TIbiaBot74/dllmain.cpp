// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "Player.h"
#include "Containers.h"
#include "BattleList.h"
#include "SocketClient.h"

using namespace std;

shared_ptr<Player> player;
shared_ptr<Containers> containers;
shared_ptr<BattleList> battleList;
shared_ptr<set<int>> idsBackpack;
shared_ptr<set<int>> lootsAmountable;
shared_ptr<set<int>> ignoreLoot;
shared_ptr<SocketClient> client;

DWORD WINAPI HackThread(HMODULE hModule) {

    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    
    cout << "new version generated\n";

    int moduleBase = (int)GetModuleHandle(L"Old-Ultra.exe");
    //int moduleBase = (int)GetModuleHandle(L"Tibia Old Client.exe");
    cout << "moduleBase: " << moduleBase << endl;

    player = make_shared<Player>(moduleBase);
    containers = make_shared<Containers>(moduleBase);
    battleList = make_shared<BattleList>(moduleBase);
    client = make_shared<SocketClient>();

    // lamda expression as callable
    //thread th3([]() {
    //    while (true) {
    //        if (GetAsyncKeyState(VK_SCROLL) & 1) {
    //            break;
    //        }
    //        player->autoHealFriend();
    //        /*containers->drawLoot();
    //        player->inventory->refillBurstArrows();*/
    //        Sleep(300);
    //    }
    //});

    // Wait for thread t3 to finish
    auto threadClient= client->initThread();
    
    while (true) {
        if (GetAsyncKeyState(VK_SCROLL) & 1) {
            break;
        }
        //player->autoHealFriend();
        //player->inventory->refillPosionArrow();
        /*Sleep(300);*/
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            //reinterpret_cast<void(__cdecl*)(int, int, int, int)>(moduleBase + 0xE70A0)(0,0,0,0);
            //reinterpret_cast<void(__cdecl*)(int, int, int, int)>(moduleBase + 0xE70F0)(0, 0, 0, 0);

            //reinterpret_cast<void(__cdecl*)(int)>(moduleBase + 0xE7140)(0);

            ////1698764
            ///*reinterpret_cast<void(__cdecl*)(int)>(moduleBase + 0xA6150)(1698764);*/
            //
            //reinterpret_cast<void(__cdecl*)(int, int, int, int, int, int)>(moduleBase + 0xE70F0)(3160, 0, 0, 0, 0xFFFF, 40);
            //reinterpret_cast<void(__cdecl*)(int, int, int, int ,int ,int)>(moduleBase + 0xE70F0)(3160, 0, 0, 0, 0xFFFF, 40);
            //reinterpret_cast<void(__cdecl*)(int, int)>(moduleBase + 0xD1C40)(3160, 7);
            client->sendMsg(player->name());
;            if (player->healthPercent() < 90) {
                player->uhSelf();
            }
        }
        if (GetAsyncKeyState(VK_END) & 1) {
            player->mfSelf();
        }
        if (GetAsyncKeyState(VK_DELETE) & 1) {            
            player->runeTarget(ItemId::SD);
        }
        if (GetAsyncKeyState(VK_HOME) & 1) {
            //player->autoHealFriend();
            player->paralyzeTarget();
            //player->runeTarget(ItemId::UH);
        }
        if (GetAsyncKeyState(VK_NEXT) & 1) { // pageDown
            if (player->inventory->isUsingRing(ItemId::U_ENERGY_RING)) {
                player->inventory->useTimeRing();
            }
            else {
                player->inventory->useEnergyRing();
            }
        }
        if (GetAsyncKeyState(VK_PRIOR) & 1) { // pageup
            player->antiPush();
            //player->runeEnemy(ItemId::HMM);
        }
        if (GetAsyncKeyState(VK_F11) & 1) {
            battleList->init();
        }

        if (GetAsyncKeyState(VK_F12) & 1) {
            player->sendItemSelf(ItemId::FIRE_BOMB);
        }

        player->refreshLastTargetId();
        
        //player->printWhiteMessage(player->toString());
        //battleList->readCreaturesVisible();
        Sleep(10);
    }

    threadClient.join();
    /*th3.join();*/

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

