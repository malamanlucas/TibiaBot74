// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "include.h"
#include "tipos.h"
#include "Player.h"
#include "Containers.h"
#include "BattleList.h"
#include "SocketClient.h"
#include "WindowsApi.h"
#include "GUI.h"
#include "variables.h";
#include "hotkeys.h"

const char* windowClasseName = "Old-UltraCl";
//const char* windowClasseName = "TibiaPVP cl";
shared_ptr<Variables> v;

DWORD WINAPI HackThread(HMODULE hModule) {

    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    cout << "new version generated\n";

    //int moduleBase = (int)GetModuleHandle("Tibia.exe");
    int moduleBase = (int)GetModuleHandle("Old-Ultra.exe");
    //int moduleBase = (int)GetModuleHandle("Tibia Old Client.exe");
    cout << "moduleBase: " << moduleBase << endl;

    v = make_shared<Variables>(moduleBase);
    thread guiThread([] {
        GUI::initImGui();
        });

    v->autoTasks->enableAllConditionally();

    auto threadClient = v->client->initThread();

    thread dash([] {
        while (true) {
            if (v->autoTasks->dash) {
                v->player->walker->dash();
            }
            Sleep(50);
        }
    });

    initHotkeys();
    
    dash.join();
    threadClient.join();
    guiThread.join();
    v->autoTasks->joinThreads();

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            HANDLE thread = nullptr;
            thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr);
            if (thread) {
                CloseHandle(thread);
            }
            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH: {
            break;
        }
    }
    return TRUE;
}


