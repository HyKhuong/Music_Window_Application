#pragma once
#include <windows.h>
#include "sqlite3.h"

extern sqlite3* db;

// -- Database --
void Database_Init();
int GetSongById(int id, char* outPath, int maxLen);
void Create_TableForDB(const char* sql);
void DB_Schema();
int Insert_SongIntoPlayList(int SongId, int PlayListId);

// -- Songs Table -- 
void Database_LoadSongs(HWND listView);
void InserSongDurationDB(int id, const char* path);
int CheckSongDurationStatus(int id);
int SongCount();
void InsertSongIntoDB(const char* title, const char* path, int duration);

// -- PlayList Table --
void InserIntoPlayerList(const char* title);
void Database_LoadPlayList(HWND listView);
void LoadPlayList_Combobox(HWND hComboBox);