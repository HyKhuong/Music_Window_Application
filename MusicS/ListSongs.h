#pragma once
#include <windows.h>

HWND Create_ListSongs(HWND hParent, int x, int y, int w, int h, int id);
void Add_ColumnListView(HWND listSongs, int index, LPWSTR text, int width);
