#include "pch.h"
#include "include.h"
#include "Overlay.h"
#include "DirectX.h"
#include "GUi.h"
#include "variables.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "detours.h"

extern shared_ptr<Variables> v;

extern shared_ptr<Overlay> overlay;
extern directx_t DirectX;

bool GetD3D9Device(void** pTable, size_t Size);
HHOOK MouseHook;

HHOOK hMouseHook;
/*Hooking*/
typedef HRESULT(_stdcall* f_EndScene)(IDirect3DDevice9* pDevice);
HRESULT _stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
void buildGui();
f_EndScene oEndScene;
WNDPROC oWndProc;

bool onlyOnce = false;

static bool MenuOpen = true;

// Data
IDirect3DDevice9* g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS g_d3dpp;
D3DPRESENT_PARAMETERS d3dpp = { 0 };
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void* d3d9Device[119];


void GUI::initImGui() {

    overlay = make_shared<Overlay>();
    overlay->GetTargetWindow();
    /*DirectxFunctions::DirectXInit(overlay->details.Window);
    GetD3D9Device(d3d9Device, sizeof(d3d9Device));*/

    if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
    {
        std::cout << "[+] Found DirectXDevice vTable at: ";
        std::cout << std::hex << d3d9Device[0] << "\n";

        std::cout << "[+] Trying to hook function..\n";

        // Using detours
        // The following line is NOT needed if we already have a ** void (aka *void [])
        // void ** pVTable = *reinterpret_cast<void***>(DirectXDevice);

        oEndScene = (f_EndScene)DetourFunction((PBYTE)d3d9Device[42], (PBYTE)Hooked_EndScene);
    }

}

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (v->autoTasks->showGui) // bool
    {
        ImGuiIO& io = ImGui::GetIO();
        MOUSEHOOKSTRUCT* pMouseStruct = (MOUSEHOOKSTRUCT*)lParam;
        if (pMouseStruct != NULL) {
            switch (wParam)
            {
            case WM_LBUTTONDOWN:
                io.MouseDown[0] = true;
                break;
            case WM_LBUTTONUP:
                io.MouseDown[0] = false;
                io.MouseReleased[0] = true;
                break;
            case WM_RBUTTONDOWN:
                io.MouseDown[1] = true;
                break;
            case WM_RBUTTONUP:
                io.MouseDown[1] = false;
                io.MouseReleased[1] = true;
                break;
            }
        }
    }
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (true && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}


bool GetD3D9Device(void** pTable, size_t Size)
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, ("ImGui Example"), NULL };
    RegisterClassEx(&wc);

    if (!pTable)
    {
        std::cout << "[-] pTable not set.\n";
        return false;
    }


    std::cout << "[*] Right before Direct3DCreate9.\n";
    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D)
    {
        UnregisterClass(("ImGui Example"), wc.hInstance);
        std::cout << "[-] Direct3DCreate9 failed.\n";
        return false;
    }
    std::cout << "[+] Direct3DCreate9 successfull.\n";

    d3dpp.hDeviceWindow = overlay->target.Window;
    d3dpp.Windowed = ((GetWindowLong(d3dpp.hDeviceWindow, GWL_STYLE) & WS_POPUP) != 0) ? FALSE : TRUE;;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE; // Present with vsync
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // Present without vsync, maximum unthrottled framerate

    std::cout << "[*] Got here..\n";

    IDirect3DDevice9* pDummyDevice = nullptr;
    HRESULT create_device_ret = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);
    std::cout << "[*] Return of CreateDevice:\n";
    std::cout << create_device_ret;
    std::cout << "\n";
    if (!pDummyDevice || FAILED(create_device_ret))
    {
        std::cout << "[-] CreateDevice failed\n";
        pD3D->Release();
        UnregisterClass(("ImGui Example"), wc.hInstance);
        return false;
    }
    std::cout << "[+] CreateDevice successfull.\n";

    memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), Size);

    pDummyDevice->Release();
    pD3D->Release();

    std::cout << "[+] Success!\n";
    return true;
}



void buildGui() {
    //ImGui::Text(v->player->toString());
    ImGui::Checkbox("dash", &v->autoTasks->dash);
    ImGui::Checkbox("IsDruid?", &v->player->isDruid);
    for (auto task : v->autoTasks->tasks)
    {
        ImGui::Checkbox(task->label, &task->flag);
    }
    //ImGui::InputText("text", test, IM_ARRAYSIZE(test));
}

HRESULT _stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
{
    g_pd3dDevice = pDevice;
    // Do directx things here..
    D3DRECT BarRect = { 100, 100, 200, 200 };

    if (!onlyOnce)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.MouseDrawCursor = 1;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

        // Setup Platform/Renderer bindings
        ImGui_ImplWin32_Init(d3dpp.hDeviceWindow);
        ImGui_ImplDX9_Init(g_pd3dDevice);
        MouseHook = SetWindowsHookEx(WH_MOUSE, mouseProc, 0, GetCurrentThreadId()); // Our MouseInput Hook

        // Setup Style
        ImGui::StyleColorsDark();

        // Main loop
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        ShowWindow(d3dpp.hDeviceWindow, SW_SHOWDEFAULT);
        UpdateWindow(d3dpp.hDeviceWindow);
        onlyOnce = true;
    }

    if (v->autoTasks->showGui) {
        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        buildGui();

        // Rendering
        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    }
    
    return oEndScene(g_pd3dDevice);
}