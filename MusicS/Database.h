#pragma once

#include <windows.h>

void Database_Init();
void Database_LoadSongs(HWND listView);
void Database_AddSong(const char* path);

int GetSongById(int id, char* outPath, int maxLen);
