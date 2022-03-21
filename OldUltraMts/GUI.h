#pragma once


namespace GUI {
	LRESULT CALLBACK WinProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	void initGui();
	void initImGui();
	void destroy();
}