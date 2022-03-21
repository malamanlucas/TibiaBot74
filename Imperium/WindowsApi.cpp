#include "pch.h"
#include "include.h"
#include "WindowsApi.h"

extern const char* windowClasseName;

bool WindowsApi::isFocused() {
	HWND hwnd = GetForegroundWindow();
	return this->clientWindow == hwnd;
}

void WindowsApi::showPopup(const char* msg) {
	MessageBox(NULL, msg, NULL, NULL);
}

WindowsApi::WindowsApi() {
	this->clientWindow = FindWindow(windowClasseName, nullptr);
}
