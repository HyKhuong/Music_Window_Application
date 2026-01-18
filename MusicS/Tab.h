#pragma once
#include <windows.h>

HWND Create_Tab(HWND hParent, int id);
void Create_ChildTab(HWND hParent);
void Add_ChildTab(const wchar_t* text, int id);
void ShowTabPage(int index);
void CallTab(LPARAM lParam);

void RegisterHomePageClass(HINSTANCE hInst);
void RegisterPlayListPageClass(HINSTANCE hInst);