#pragma once

#ifndef WINDOW
#define WINDOW

#include "include.h"



struct overlay_t
{
	WNDCLASSEX Class;
	char Name[256];
	int Width, Height;
	HWND Window;
	MSG Message;
	MARGINS Margin;
};

struct target_t
{
	char Name[256];
	HWND Window;
	RECT Size;
	DWORD Style;
	BOOL Checked;
};


class Overlay {
public:
	overlay_t details;
	target_t target;
	HINSTANCE hInstance;
	DWORD pid;
	DWORD Module;
	void CreateClass(WNDPROC winproc, const char* windowname);
	void GetTargetWindow();
	void CreateWindowOverlay();
};



#endif // !WINDOW
