#include "pch.h"
#include "include.h"
#include "WindowsApi.h"

extern const char* windowClasseName;

bool WindowsApi::isFocused() {
	HWND hwnd = GetForegroundWindow();
	return this->clientWindow == hwnd;
}

WindowsApi::WindowsApi() {
	this->clientWindow = FindWindow(windowClasseName, nullptr);
}
