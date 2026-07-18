#pragma once
#include <windows.h>
#include "sqlite3.h"

//extern const char* sql;

extern sqlite3* db;

// -- Database --
void Database_Init();
int GetSongById(int id, wchar_t* outPath, wchar_t* outTitle);
void Create_TableForDB(const char* sql);
void DB_Schema();
int Insert_SongIntoPlayList(int SongId, int PlayListId);

// -- Songs Table -- 
void InserSongDurationDB(int id, const wchar_t* path);
int CheckSongDurationStatus(int id);
int SongCount();
void InsertSongIntoDB(const char* title, const char* path, int duration);
void UpdateSongs(int SongId, const char* title, const char* path, int duration);
void DeleteSongs(int id);

// -- PlayList Table --
void InserIntoPlayerList(const char* title);
void Database_LoadPlayList(HWND listView);
void LoadPlayList_Combobox(HWND hComboBox);
void PutSongsToQueue(int id);

// -- Use for two --
void LoadSongs_Data(HWND listView, sqlite3_stmt* stmt);
void LoadList_Songs(HWND listView, const char* sql);
void LoadPlayList_Songs(HWND listView, const char* sql);