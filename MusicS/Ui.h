#pragma once
#include <windows.h>

typedef struct UIContext {
	// Main window
	HWND hWnd;

	// Popup
	HWND hPopUp;
	HWND hCustomListSongs;
	HWND hTextBox;
	HWND hAddText;

	// Tabs
	HWND hTab;
	HWND hPages[3];
};

extern UIContext g_ui;


void UI_Init(HWND hWnd);
void UI_HandleCommand(WPARAM wParam, HINSTANCE hInst);
BOOL PickAFile(HWND hWnd, wchar_t* outPath, DWORD outSize);
void PickSongToDB(HWND hWnd);

void ShowTabPage(int index);
void CallTab(LPARAM lParam);