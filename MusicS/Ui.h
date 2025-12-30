#pragma once
#include <windows.h>

extern HWND listSongs;
void UI_Init(HWND hWnd);
void UI_HandleCommand(WPARAM wParam);
BOOL PickAFile(HWND hWnd, wchar_t* outPath, DWORD outSize);
void PickSongToDB(HWND hWnd);