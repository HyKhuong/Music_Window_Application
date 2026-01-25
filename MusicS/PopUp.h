#pragma once
#include <windows.h>
#include <commctrl.h>

// -- Create Pop Up --
void ShowPopUp(HWND parent, HINSTANCE hInst);
void RegisterPopupClass(HINSTANCE hInst);

void Create_MenuPopUp(LPNMITEMACTIVATE p, HWND ListView, int id, const wchar_t* text);
void MenuPopUp_HandleCommand(WPARAM wParam, HWND hWnd);