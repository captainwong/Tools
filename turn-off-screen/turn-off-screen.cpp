// turn-off-screen.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "turn-off-screen.h"

// based on https://docs.microsoft.com/en-us/windows/desktop/menurc/wm-syscommand

/**
Sets the state of the display. This command supports devices that have power-saving features, such as a battery-powered personal computer.
The lParam parameter can have the following values:
-1 (the display is powering on)
1 (the display is going to low power)
2 (the display is being shut off)
*/

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	PostMessageW(HWND(INVALID_HANDLE_VALUE), WM_SYSCOMMAND, SC_MONITORPOWER, 2);

	return 0;
}


