#pragma once
#include "include.h"

//extern const char* windowClasseName;

class WindowsApi {
public:
	WindowsApi();
	HWND clientWindow;

	bool isFocused();
};