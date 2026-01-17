#pragma once
#include <windows.h>

HWND Create_Tab(HWND hParent, int id);
void Create_ChildTab(HWND hParent);
void Add_ChildTab(const wchar_t* text, int id);

void RegisterPageClass(HINSTANCE hInst);