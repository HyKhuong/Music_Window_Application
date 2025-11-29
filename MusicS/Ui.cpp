#include "Ui.h"
#include <commctrl.h>
#include "Player.h"
#include "Database.h"

HWND btnPlay, btnPause, btnStop;
HWND listSongs;


//Create the ui 
void UI_Init(HWND hWnd)
{
	btnPlay = CreateWindow(TEXT("BUTTON"), TEXT("Play"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		20, 20, 80, 30,
		hWnd, (HMENU)1, NULL, NULL);

	btnPause = CreateWindow(TEXT("BUTTON"), TEXT("Pause"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		110, 20, 80, 30,
		hWnd, (HMENU)2, NULL, NULL);

	btnStop = CreateWindow(TEXT("BUTTON"), TEXT("Stop"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		200, 20, 80, 30,
		hWnd, (HMENU)3, NULL, NULL);

	listSongs = CreateWindow(WC_LISTVIEW, TEXT(""),
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

	Database_LoadSongs(listSongs);
}

//Handle system
void UI_HandleCommand(WPARAM wParam) 
{
	switch (LOWORD(wParam))
	{
	case 1:
		Player_Play();
		break;

	case 2:
		Player_Pause();
		break;

	case 3:
		Player_Stop();
		break;
	}
}