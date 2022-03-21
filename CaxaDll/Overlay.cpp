#include "pch.h"
#include "Overlay.h"
#include "include.h"
#include "variables.h"

extern shared_ptr<Variables> v;
extern const char* windowClasseName;

void Overlay::CreateClass(WNDPROC winproc, const char* windowname)
{
	sprintf_s(this->details.Name, "%s", windowname);
	this->details.Class.cbClsExtra = 0;
	this->details.Class.cbSize = sizeof(WNDCLASSEX);
	this->details.Class.cbWndExtra = 0;
	this->details.Class.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	this->details.Class.hCursor = LoadCursor(0, IDC_ARROW);
	this->details.Class.hIcon = LoadIcon(0, IDI_APPLICATION);
	this->details.Class.hIconSm = LoadIcon(0, IDI_APPLICATION);
	this->details.Class.hInstance = hInstance;
	this->details.Class.lpfnWndProc = winproc;
	this->details.Class.lpszClassName = this->details.Name;
	this->details.Class.lpszMenuName = this->details.Name;
	this->details.Class.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&this->details.Class)) {
		cout << "!RegisterClassEx(&this->details.Class)" << endl;
		exit(1);
	}
		
}

void Overlay::GetTargetWindow()
{
	
	//this->target.Window = FindWindow(0, windowClasseName);
	if (!this->target.Window) {
		while (!v->windowsApi->isFocused()) {
			Sleep(200);
		}
		//this->target.Window = v->windowsApi->FindWindowByCurrentProcessId();//
		this->target.Window = GetForegroundWindow();
	}

	if (this->target.Window)
	{
		GetWindowThreadProcessId(this->target.Window, &pid);
		//pHandle = ProcessFunctions::GetHandle();
		this->target.Checked = true;
		GetWindowRect(this->target.Window, &this->target.Size);
	
		this->details.Width = this->target.Size.right - this->target.Size.left;
		this->details.Height = this->target.Size.bottom - this->target.Size.top;
		this->target.Style = GetWindowLong(this->target.Window, GWL_STYLE);
		

		if (this->target.Style & WS_BORDER)
		{
			this->target.Size.top += (this->target.Size.bottom * 0.6);
			this->details.Height -= 23;
		}
		//this->target.Size.left += (this->target.Size.right - this->target.Size.left) * 0.4;
		/*this->details.Width /= 6;
		this->target.Size.left -= (this->target.Size.left * 0.4);*/
		//this->target.Size.left -= 15;
		/*this->details.Width = 350;
		this->target.Size.left = 48;*/

		/*cout << "WindowRect: left: " << this->target.Size.left;
		cout << ", top: " << this->target.Size.top;
		cout << ", width: " << this->details.Width;
		cout << ", Height: " << this->details.Height << endl;*/

		MoveWindow(this->details.Window, this->target.Size.left, this->target.Size.top, this->details.Width, this->details.Height, true);
		this->details.Margin = { 0, 0, this->details.Width, this->details.Height };

	}
	else {
		cout << "Não encontrei a janela" << endl;
	}
}

void Overlay::CreateWindowOverlay()
{
	this->details.Window = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT, this->details.Name, this->details.Name, WS_POPUP, 1, 1, this->details.Width, this->details.Height, 0, 0, hInstance, 0);
	SetLayeredWindowAttributes(this->details.Window, RGB(0, 0, 0), 255, LWA_COLORKEY | LWA_ALPHA);
	ShowWindow(this->details.Window, SW_SHOW);
	DwmExtendFrameIntoClientArea(this->details.Window, &this->details.Margin);
}

