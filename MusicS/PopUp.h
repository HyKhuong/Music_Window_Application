#pragma once
#include <windows.h>
#include <commctrl.h>

// -- Create Pop Up --
void ShowPopUp(HWND parent, HINSTANCE hInst);
void RegisterPopupClass(HINSTANCE hInst);

void Create_MenuPopUp(LPNMITEMACTIVATE p, HWND ListView);
void MenuPopUp_HandleCommand(WPARAM wParam, HWND hWnd);