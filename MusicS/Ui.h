#pragma once
#include <windows.h>

extern HWND listSongs;
void UI_Init(HWND hWnd);
void UI_HandleCommand(WPARAM wParam, HINSTANCE hInst);
BOOL PickAFile(HWND hWnd, wchar_t* outPath, DWORD outSize);
void PickSongToDB(HWND hWnd);

// -- Create Pop Up --
void ShowPopUp(HWND parent, HINSTANCE hInst);
void RegisterPopupClass(HINSTANCE hInst);

void ShowTabPage(int index);
void CallTab(LPARAM lParam);