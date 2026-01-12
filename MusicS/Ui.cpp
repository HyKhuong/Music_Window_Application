#include "Ui.h"
#include <commctrl.h>
#include "Player.h"
#include "Database.h"
#include <cstdio>
#include "global.h"

// -- Comon Ui --
HWND btnPlay, btnPause, btnStop;
HWND listSongs, filePicker;

// -- Track Song Duration --
HWND hTimeText = NULL;
HWND hTrack = NULL;

// -- PopUp --
HWND hPopUp, hCustomListSongs, hTextBox, hAddText;

// -- Create Tab For Window --
HWND hTab;
HWND hPages[3];

int g_totalTime = 0;

HWND hWnd;


void UI_Init(HWND hWnd)
{
	// -- Play button --
	btnPlay = CreateWindowA("BUTTON", "Play",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		20, 20, 80, 30,
		hWnd, (HMENU)1, NULL, NULL);

	// -- Pause button --
	btnPause = CreateWindow(L"BUTTON", L"Pause",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		110, 20, 80, 30,
		hWnd, (HMENU)2, NULL, NULL);

	// -- Stop button --
	btnStop = CreateWindow(L"BUTTON", L"Stop",
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		200, 20, 80, 30,
		hWnd, (HMENU)3, NULL, NULL);

	// -- Add File --
	filePicker = CreateWindow(
		L"BUTTON",
		L"OPEN FILE",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 20, 80, 30,
		hWnd, (HMENU)4,
		NULL,
		NULL
	);

	// -- Add Custom List Songs --
	hCustomListSongs = CreateWindow(
		L"Button",
		L"Create Your List Songs",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		400, 20, 80, 30,
		hWnd, (HMENU)5,
		NULL,
		NULL
	);

	// -- List of all songs --
	listSongs = CreateWindow(WC_LISTVIEW, L"",
		WS_VISIBLE | WS_CHILD | LVS_REPORT,
		20, 70, 600, 300,
		hWnd, (HMENU)4, NULL, NULL);

	LVCOLUMN lvc = { 0 };
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	// Column 0 = ID
	lvc.pszText = (LPWSTR)L"ID";
	lvc.cx = 100;
	ListView_InsertColumn(listSongs, 0, &lvc);

	// Column 1 = Title
	lvc.pszText = (LPWSTR)L"Title";
	lvc.cx = 300;
	ListView_InsertColumn(listSongs, 1, &lvc);

	// Column 2 = Path
	//lvc.pszText = (LPWSTR)L"Path";
	//lvc.cx = 400;
	//ListView_InsertColumn(listSongs, 2, &lvc);

	// Column 3 = Duration
	lvc.pszText = (LPWSTR)L"Duration";
	lvc.cx = 200;
	ListView_InsertColumn(listSongs, 2, &lvc);

	ListView_DeleteAllItems(listSongs);

	// -- Load All Songs From DB To List View --
	Database_LoadSongs(listSongs);

	// -- Show Duration Bar --
	hTimeText = CreateWindow(
		L"STATIC",
		L"00:00 / 00:00",
		WS_CHILD | WS_VISIBLE,
		20, 395, 120, 30,
		hWnd,
		(HMENU)8,
		NULL,
		NULL
	);

	// -- Show Track Bar --
	hTrack = CreateWindowEx(
		0, TRACKBAR_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | TBS_HORZ,
		150, 395, 620, 30,
		hWnd, (HMENU)9,
		NULL,
		NULL
	);

	
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

	InserSongIntoDB(title, pathC, duration);

	ListView_DeleteAllItems(listSongs);

	Database_LoadSongs(listSongs);
}

// -- Create A New Window --
LRESULT CALLBACK PopupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
	switch(msg) 
	{
	case WM_CREATE:
	{
		hTextBox = CreateWindowW(
			L"EDIT",
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			20, 20, 200, 25,
			hWnd, NULL, GetModuleHandle(NULL), NULL
		);

		hAddText = CreateWindow(
			L"BUTTON",
			L"ADD",
			WS_CHILD | WS_VISIBLE,
			20, 60, 80, 30,
			hWnd, (HMENU)2, NULL, NULL
		);

		// -- Create Tab --
		hTab = CreateWindow(
			WC_TABCONTROL,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
			20, 300, 500, 300,
			hWnd, (HMENU)100, NULL, NULL
		);

		// -- Add Tab --
		TCITEM tie;
		tie.mask = TCIF_TEXT;

		tie.pszText = (LPWSTR)L"Home";
		TabCtrl_InsertItem(hTab, 0, &tie);

		tie.pszText = (LPWSTR)L"Player";
		TabCtrl_InsertItem(hTab, 1, &tie);

		tie.pszText = (LPWSTR)L"Setting";
		TabCtrl_InsertItem(hTab, 2, &tie);

		// -- Create Pages --
		hPages[0] = CreateWindow(
			L"STATIC",
			NULL,
			WS_CHILD | WS_VISIBLE,
			30, 400, 450, 240,
			hWnd, NULL, NULL, NULL
		);

		hPages[1] = CreateWindow(
			L"STATIC",
			L"PLAYER PAGE",
			WS_CHILD,
			30, 400, 450, 240,
			hWnd, NULL, NULL, NULL
		);

		hPages[2] = CreateWindow(
			L"STATIC",
			L"SETTINGS PAGE",
			WS_CHILD,
			30, 400, 450, 240,
			hWnd, NULL, NULL, NULL
		);
		
		listSongs = CreateWindow(WC_LISTVIEW, L"",
			WS_VISIBLE | WS_CHILD | LVS_REPORT,
			35, 70, 300, 200,
			hPages[0], (HMENU)4, NULL, NULL);

		LVCOLUMN lvc = { 0 };
		lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

		// Column 0 = ID
		lvc.pszText = (LPWSTR)L"ID";
		lvc.cx = 100;
		ListView_InsertColumn(listSongs, 0, &lvc);

		// Column 1 = Title
		lvc.pszText = (LPWSTR)L"Title";
		lvc.cx = 300;
		ListView_InsertColumn(listSongs, 1, &lvc);

		// Column 2 = Path
		//lvc.pszText = (LPWSTR)L"Path";
		//lvc.cx = 400;
		//ListView_InsertColumn(listSongs, 2, &lvc);

		// Column 3 = Duration
		lvc.pszText = (LPWSTR)L"Duration";
		lvc.cx = 200;
		ListView_InsertColumn(listSongs, 2, &lvc);

		ListView_DeleteAllItems(listSongs);

		// -- Load All Songs From DB To List View --
		Database_LoadSongs(listSongs);
	}
	break;
	case WM_NOTIFY:
		CallTab(lParam);
		break;
	case WM_COMMAND:
		if(LOWORD(wParam) == 2) 
		{
			wchar_t buf[256];
			GetWindowTextW(hTextBox, buf, 256);
			MessageBoxW(hWnd, buf, L"Add Your List Song Success", MB_OK);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		hPopUp = NULL;
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void RegisterPopupClass(HINSTANCE hInst)
{
	WNDCLASSW wc = { 0 };
	wc.lpfnWndProc = PopupProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"PopupClass";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_TAB_CLASSES };
	InitCommonControlsEx(&icex);

	RegisterClassW(&wc);
}

// -- Pop up -- 
void ShowPopUp(HWND parent, HINSTANCE hInst)
{
	if (hPopUp) return;

	hPopUp = CreateWindowExW(
		WS_EX_DLGMODALFRAME,
		L"PopupClass",
		L"My PopUp Form",
		WS_POPUP | WS_CAPTION | WS_SYSMENU,
		400, 300, 800, 850,
		parent, NULL, hInst, NULL
	);

	ShowWindow(hPopUp, SW_SHOW);
	UpdateWindow(hPopUp);
}

// -- Handle system --
void UI_HandleCommand(WPARAM wParam, HINSTANCE hInst) 
{
	switch (LOWORD(wParam))
	{
	case 1:
		char path[256];
		if(GetSongById(1, path, sizeof(path)))
		{
			Player_Play(1, path);
		}
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
	case 5:
		ShowPopUp(hWnd, hInst);
		break;
	}
}