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

shared_ptr<Overlay> overlay;
directx_t DirectX;

LRESULT CALLBACK GUI::WinProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
        //Render();
        //oEndScene = (f_EndScene)DetourFunction((PBYTE)d3d9Device[42], (PBYTE)Hooked_EndScene);
		DirectxFunctions::RenderDirectX();
		break;

	case WM_DESTROY:
		PostQuitMessage(1);
		break;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void GUI::initGui() {
	while (!v->windowsApi->isFocused()) {
		Sleep(200);
	}
	overlay = make_shared<Overlay>();
	overlay->GetTargetWindow();
	v->infoList->set("pid HWND", overlay->pid);
	overlay->CreateClass(WinProcedure, "overlay");
	overlay->CreateWindowOverlay();
	DirectxFunctions::DirectXInit(overlay->details.Window);

	while(true)
	{
		if (!v->autoTasks->running) {
			break;
		}
		if (PeekMessage(&overlay->details.Message, overlay->details.Window, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&overlay->details.Message);
			TranslateMessage(&overlay->details.Message);
		}
		Sleep(1);
		overlay->GetTargetWindow();
	}
}
