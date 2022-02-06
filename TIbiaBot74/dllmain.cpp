// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "Player.h"
#include "Containers.h"
#include "BattleList.h"

using namespace std;

shared_ptr<Player> player;
shared_ptr<Containers> containers;
shared_ptr<BattleList> battleList;

DWORD WINAPI HackThread(HMODULE hModule) {

    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    
    cout << "new version generated\n";

    int moduleBase = (int)GetModuleHandle(L"Tibia Old Client.exe");
    cout << "moduleBase: " << moduleBase << endl;

    player = make_shared<Player>(moduleBase);
    containers = make_shared<Containers>(moduleBase);
    battleList = make_shared<BattleList>(moduleBase);
    
    while (true) {
        containers->loadContainers();
        if (GetAsyncKeyState(VK_SCROLL) & 1) {
            break;
        }
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            player->uhSelf();
        }
        if (GetAsyncKeyState(VK_END) & 1) {
            player->mfSelf();
        }
        if (GetAsyncKeyState(VK_DELETE) & 1) {
            player->sdTarget();
        }
        if (GetAsyncKeyState(VK_HOME) & 1) {
            player->paralyzeTarget();
        }
        if (GetAsyncKeyState(VK_NEXT) & 1) {
            player->explosionTarget();
        }
        
        player->refreshLastTargetId();
        Sleep(10);
    }

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

