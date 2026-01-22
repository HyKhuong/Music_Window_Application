#pragma once
#include <windows.h>

void Database_Init();
void Database_LoadSongs(HWND listView);
void Database_AddSong(const char* path);

int GetSongById(int id, char* outPath, int maxLen);
int SongCount();
void InserSongDurationDB(int id, const char* path);
int CheckSongDurationStatus(int id);
void InsertSongIntoDB(const char* title,const char* path, int duration);

void Create_TableForDB(const char* sql);

void DB_Schema();

void InserIntoPlayerList(const char* title);
void Database_LoadPlayList(HWND listView);

void LoadPlayList_Combobox(HWND hComboBox);