#pragma once
#include <windows.h>

extern HWND listSongs;
void UI_Init(HWND hWnd);
void UI_HandleCommand(WPARAM wParam);