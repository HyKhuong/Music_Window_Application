#include "Ui.h"

#include "Player.h"
#include "Database.h"
#include <cstdio>

#include "Button.h"
#include "Button_Type.h"
#include "DurationTrackBar.h"
#include "DurationTrackBar_Type.h"
#include "PopUp.h"
#include "ListSongs_Type.h"
#include "Tab_Type.h"

#include "ListSongs.h"
#include "Tab.h"

#include <commctrl.h>

UIContext g_ui = { 0 };
Button btn;

void UI_Init(HWND hWnd)
{	
	// -- Button --
	btn.btnPlay = Create_Button(hWnd, 1, L"Play", 20, 20, 80, 30);
	btn.btnPause = Create_Button(hWnd, 2, L"Pause", 110, 20, 80, 30);
	btn.btnStop = Create_Button(hWnd, 3, L"Stop", 200, 20, 80, 30);
	btn.filePicker = Create_Button(hWnd, 4, L"Add Song", 300, 20, 80, 30);

	// -- Track Duration Bar --
	Create_TrackBar(hWnd);
	
	// -- Add Custom List Songs --
	g_ui.hCustomListSongs = CreateWindow(
		L"Button",
		L"Create Your List Songs",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 20, 80, 30,
		hWnd, (HMENU)5,
		NULL,
		NULL
	);

	// -- Create Tab --
	Create_Tab(hWnd, 100);

	// -- Add Tab --
	Add_ChildTab(L"HOME", Home);
	Add_ChildTab(L"PLAYER LIST", PlayerList);

	// -- Create Tabs --
	Create_ChildTab(hWnd);

	// -- Create Home List Song --
	Create_ListSongs(hPages[0], 30, 70, 450, 200, 4);

	Add_ColumnListView(listSongs, COL_ID, (LPWSTR)L"ID", 50);
	Add_ColumnListView(listSongs, COL_TITLE, (LPWSTR)L"TITLE", 300);
	Add_ColumnListView(listSongs, COL_DURATION, (LPWSTR)L"DURATION", 100);

	Clear_ListSongs();
	// -- Load All Songs From DB To List View --
	Database_LoadSongs(listSongs);

	// -- Create Player List Song --
	Create_ListSongs(hPages[1], 30, 70, 450, 200, 4);

	Add_ColumnListView(listSongs, COL_ID, (LPWSTR)L"ID", 50);
	Add_ColumnListView(listSongs, COL_TITLE, (LPWSTR)L"TITLE", 300);

	//Database_LoadSongs(listSongs);

	g_ui.hTextBox = CreateWindowW(
		L"EDIT",
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		190, 20, 200, 25,
		hPages[1], NULL, GetModuleHandle(NULL), NULL
	);

	btn.AddText = Create_Button(hPages[1], 5, L"ADD", 400, 20, 80, 25);
}

// -- Show Tab Logic --
void ShowTabPage(int index) 
{
	for(int i = 0; i < 3; i++) 
	{
		ShowWindow(hPages[i], i == index ? SW_SHOW : SW_HIDE);
	}
}

void CallTab(LPARAM lParam)
{
	LPNMHDR hdr = (LPNMHDR)lParam;

	if(hdr->hwndFrom == hTab && hdr->code == TCN_SELCHANGE)
	{
		int sel = TabCtrl_GetCurSel(hTab);
		ShowTabPage(sel);
	}
}

// -- Get Add File to DB logic -- 
BOOL PickAFile(HWND hWnd, wchar_t* outPath, DWORD outSize)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
		
	wchar_t fileName[MAX_PATH] = L"";

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter =
		L"Audio Files (*.mp3;*.wav;*.flac)\0*.mp3;*.wav;*.flac\0"
		"All Files (*.*)\0*.*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrTitle = L"Select a song";

	if (GetOpenFileName(&ofn))
	{
		wcsncpy_s(outPath, outSize, fileName, _TRUNCATE);
		return TRUE;
	}
	return FALSE;
}

void PickSongToDB(HWND hWnd) 
{
	wchar_t path[MAX_PATH];

	if (!PickAFile(hWnd, path, MAX_PATH)) return;

	char pathC[MAX_PATH];

	WideCharToMultiByte(CP_UTF8, 0, path, -1, pathC, sizeof(pathC), NULL, NULL);

	char title[MAX_PATH];
	char* name = strrchr(pathC, '\\');
	name = name ? name + 1 : pathC;

	strncpy_s(title, sizeof(title), name, _TRUNCATE);
	char* dot = strrchr(title, '.');
	if (dot) *dot = '\0';

	int duration = GetSongLength(pathC);

	InsertSongIntoDB(title, pathC, duration);

	Database_LoadSongs(listSongs);
}

// -- Handle system --
void UI_HandleCommand(WPARAM wParam, HINSTANCE hInst) 
{
	switch (LOWORD(wParam))
	{
	case 1:
	{
		char path[256];
		if (GetSongById(1, path, sizeof(path)))
		{
			Player_Play(1, path);
		}
	}
		break;

	case 2:
		Player_Pause();
		break;

	case 3:
		Player_Stop();
		break;

	case 4:
		PickSongToDB(g_ui.hWnd);
		break;
	}
}