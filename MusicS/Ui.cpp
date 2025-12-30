#include "Ui.h"
#include <commctrl.h>
#include "Player.h"
#include "Database.h"
#include <cstdio>


HWND btnPlay, btnPause, btnStop;
HWND listSongs, hTimeText, hTrack, filePicker;

HWND hWnd;


//Create the ui 
void UI_Init(HWND hWnd)
{
	//Play button
	btnPlay = CreateWindowA("BUTTON", "Play",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		20, 20, 80, 30,
		hWnd, (HMENU)1, NULL, NULL);

	//Pause button
	btnPause = CreateWindow(L"BUTTON", L"Pause",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		110, 20, 80, 30,
		hWnd, (HMENU)2, NULL, NULL);

	//Stop button
	btnStop = CreateWindow(L"BUTTON", L"Stop",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		200, 20, 80, 30,
		hWnd, (HMENU)3, NULL, NULL);

	//List of all songs
	listSongs = CreateWindow(WC_LISTVIEW, L"",
		WS_VISIBLE | WS_CHILD | LVS_REPORT,
		20, 70, 740, 360,
		hWnd, (HMENU)4, NULL, NULL);

	LVCOLUMN lvc = { 0 };
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	// Column 0 = ID
	lvc.pszText = (LPWSTR)L"ID";
	lvc.cx = 50;
	ListView_InsertColumn(listSongs, 0, &lvc);

	// Column 1 = Title
	lvc.pszText = (LPWSTR)L"Title";
	lvc.cx = 300;
	ListView_InsertColumn(listSongs, 1, &lvc);

	// Column 2 = Path
	lvc.pszText = (LPWSTR)L"Path";
	lvc.cx = 400;
	ListView_InsertColumn(listSongs, 2, &lvc);

	// Column 3 = Duration
	lvc.pszText = (LPWSTR)L"Duration";
	lvc.cx = 100;
	ListView_InsertColumn(listSongs, 3, &lvc);

	ListView_DeleteAllItems(listSongs);

	Database_LoadSongs(listSongs);

	hTimeText = CreateWindow(
		L"STATIC",
		L"00:00 / 00:00",
		WS_CHILD | WS_VISIBLE,
		20, 300, 120, 20,
		hWnd,
		(HMENU)5,
		NULL,
		NULL
	);

	hTrack = CreateWindowEx(
		0, TRACKBAR_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | TBS_HORZ,
		150, 300, 400, 30,
		hWnd, (HMENU)6,
		NULL,
		NULL
	);

	filePicker = CreateWindow(
		L"BUTTON",
		L"OPEN FILE",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 20, 80, 30,
		hWnd, (HMENU)4,
		NULL,
		NULL
	);
}

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

	InserSongIntoDB(title, pathC, duration);

	ListView_DeleteAllItems(listSongs);

	Database_LoadSongs(listSongs);
}

//Handle system
void UI_HandleCommand(WPARAM wParam) 
{
	switch (LOWORD(wParam))
	{
	case 1:
		//char path[256];
		//int id = 0;
			//Player_Play(id, path);
		break;

	case 2:
		Player_Pause();
		break;

	case 3:
		Player_Stop();
		break;

	case 4:
		PickSongToDB(hWnd);
		break;
	}
}