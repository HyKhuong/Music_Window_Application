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
#include "global.h"

UIContext g_ui = { 0 };
Button btn;

void UI_FONT() 
{
	g_WinFont = CreateFontW(
		-14,                    
		0, 0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,        
		FF_DONTCARE,
		L"Segoe UI"        
	);
}

void SetUIFont(HWND hCtrl)
{
	SendMessageW(hCtrl, WM_SETFONT, (WPARAM)g_WinFont, TRUE);
}

void UI_Init(HWND hWnd)
{	
	// -- Button --
	btn.btnPlay = Create_Button(hWnd, 1, L"Play", 20, 20, 80, 30);
	SetUIFont(btn.btnPlay);

	btn.btnPause = Create_Button(hWnd, 2, L"Pause", 110, 20, 80, 30);
	SetUIFont(btn.btnPause);

	btn.btnStop = Create_Button(hWnd, 3, L"Stop", 200, 20, 80, 30);
	SetUIFont(btn.btnStop);

	btn.filePicker = Create_Button(hWnd, 4, L"Add Song", 300, 20, 80, 30);
	SetUIFont(btn.filePicker);


	// -- Track Duration Bar --
	Create_TrackBar(hWnd);
	
	// -- Add Custom List Songs --
	/*g_ui.hCustomListSongs = CreateWindow(
		L"Button",
		L"Create Your List Songs",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 20, 80, 30,
		hWnd, (HMENU)5,
		NULL,
		NULL
	);*/

	// -- Create Tab --
	Create_Tab(hWnd, 100);

	// -- Add Tab --
	Add_ChildTab(L"HOME", Home);
	Add_ChildTab(L"PLAYER LIST", PlayerList);

	// -- Create Tabs --
	Create_ChildTab(hWnd);

	// -- Create Home  List Song --
	HomePage_Init();

	// -- Create PlayList --
	PlayList_Init(btn);
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

	while (*name >= '0' && *name <= '9')
		name++;

	if (*name == '.')
		name++;

	strncpy_s(title, sizeof(title), name, _TRUNCATE);
	char* dot = strrchr(title, '.');
	if (dot) *dot = '\0';

	MultiByteToWideChar(CP_UTF8, 0, pathC, -1, path, sizeof(path));
	int duration = GetSongLength(path);

	InsertSongIntoDB(title, pathC, duration);

	ListView_DeleteAllItems(HomeSongs_List);
	LoadList_Songs(HomeSongs_List, sql);
}

// -- Handle system --
void UI_HandleCommand(WPARAM wParam, HINSTANCE hInst) 
{
	switch (LOWORD(wParam))
	{
	case 1:
	{
		wchar_t path[256];
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
	case 5:
		ShowPopUp(g_hWnd, hInst);
	}
}