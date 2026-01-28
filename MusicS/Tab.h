#pragma once
#include <windows.h>
#include "Button_Type.h"

// -- Tab --
HWND Create_Tab(HWND hParent, int id);
void Create_ChildTab(HWND hParent);
void Add_ChildTab(const wchar_t* text, int id);
void ShowTabPage(int index);
void CallTab(LPARAM lParam);
void ClickSongs(HWND ListView, LPARAM LParam, int id);

// -- Home && PlayList --
void RegisterHomePageClass(HINSTANCE hInst);
void HomePage_Init();
void RegisterPlayListPageClass(HINSTANCE hInst);
void PlayList_Init(Button btn);

// -- Song Detail --
void RegisterSongDetail(HINSTANCE hInst);
HWND Create_SongDetailPage(HWND hParent);
void ComboBox(HWND hParent);
int  GetComBoBox_ID(HWND ComboBox);

// -- Play List Songs --
void RegisterPlayListSongs(HINSTANCE hInst);
HWND Create_PlayListSongs(HWND hParent);
void OpenPlayList_Songs(LPARAM lParam);
void PlayListSongsDetail_Init(HWND hParent);
int CountSongsInPlayList(int id);