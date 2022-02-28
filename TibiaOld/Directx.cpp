#include "pch.h"
#include "DirectX.h"
#include "Overlay.h"
#include "variables.h"
#include <atlconv.h>

extern shared_ptr<Overlay> overlay;
extern shared_ptr<Variables> v;


#define MENUITEMS 3
bool bMenuItems[MENUITEMS];
int iSelectedItem = 0;
wchar_t wMenuItems[MENUITEMS][255] =
{
	L"Option 1",
	L"Option 2",
	L"EXIT"
};


bool bInitialized = false;

bool bKeys[255];
bool bKeyPrev[255];

bool bMenuEnabled = true;

void DirectxFunctions::RenderDirectX()
{
	DirectX.Device->BeginScene();
	if (GetForegroundWindow() == overlay->target.Window)
	{

		RECT pos;
		pos.right = 1001;
		pos.bottom = 1001;

		pos.left = 20;
		pos.top = 20;

		//bg
		Drawing::FilledRect(8, 20, 195, 180, D3DCOLOR_ARGB(5, 255, 0, 0));
		//outer rect
		Drawing::BorderedRect(8, 19, 195, 180, 1, 1, 1, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
		Drawing::FilledRect(8, 19, 195, 19, D3DCOLOR_ARGB(255, 255, 255, 255));
		DirectX.Font->DrawTextW(NULL, L"Malaman Bot", -1, &pos, 0, D3DCOLOR_ARGB(255, 5, 5, 5));
		pos.top += 20;

		wchar_t swf[255];
		wchar_t swf2[255];


		D3DCOLOR color;
		color = D3DCOLOR_ARGB(237, 243, 243, 24);
		pos.left -= 10;


		for (auto info = v->infoList->infos.begin(); info != v->infoList->infos.end(); info++) {
			ostringstream buf;
			buf << info->first << ": " << info->second;
			std::wstring stemp = std::wstring(buf.str().begin(), buf.str().end()).c_str();

			DirectX.Font->DrawTextW(NULL, stemp.c_str(), -1, &pos, 0, color);
			pos.top += 16;
			
		}

	}

	DirectX.Device->EndScene();
	DirectX.Device->PresentEx(0, 0, 0, 0, 0);
	DirectX.Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
}


void DirectxFunctions::DirectXInit(HWND hwnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX.Object))) {
		cout << " FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX.Object)) " << endl;
		exit(1);
	}


	ZeroMemory(&DirectX.Param, sizeof(DirectX.Param));
	DirectX.Param.Windowed = true;
	DirectX.Param.BackBufferFormat = D3DFMT_A8R8G8B8;
	DirectX.Param.BackBufferHeight = overlay->details.Height;
	DirectX.Param.BackBufferWidth = overlay->details.Width;
	DirectX.Param.EnableAutoDepthStencil = true;
	DirectX.Param.AutoDepthStencilFormat = D3DFMT_D16;
	DirectX.Param.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	DirectX.Param.SwapEffect = D3DSWAPEFFECT_DISCARD;

	if (FAILED(DirectX.Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectX.Param, 0, &DirectX.Device))) {
		cout << " FAILED(DirectX.Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectX.Param, 0, &DirectX.Device)) " << endl;
		exit(1);
	}

	D3DXCreateFont(DirectX.Device, 14, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &DirectX.Font);
	D3DXCreateFont(DirectX.Device, 14, 0, 0, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial", &DirectX.espFont);

	if (!DirectX.Line)
		D3DXCreateLine(DirectX.Device, &DirectX.Line);
}