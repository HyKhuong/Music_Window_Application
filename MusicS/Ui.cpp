#include "Ui.h"
#include <commctrl.h>
#include "Player.h"
#include "Database.h"

HWND btnPlay, btnPause, btnStop;
HWND listSongs, hTimeText, hTrack;


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
		hWnd, (HMENU)10, NULL, NULL);

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

	ListView_DeleteAllItems(listSongs);

	Database_LoadSongs(listSongs);

	hTimeText = CreateWindow(
		L"STATIC",
		L"00:00 / 00:00",
		WS_CHILD | WS_VISIBLE,
		20, 300, 120, 20,
		hWnd,
		(HMENU)10,
		NULL,
		NULL
	);

	hTrack = CreateWindowEx(
		0, TRACKBAR_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | TBS_HORZ,
		150, 300, 400, 30,
		hWnd, (HMENU)101,
		NULL,
		NULL
	);
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
	}
}