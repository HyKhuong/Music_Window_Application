#include <wtypes.h>
#include "Ui.h"
#include <CommCtrl.h>
#include "ListSongs.h"
#include "Database.h"
#include "ListSongs_Type.h"

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
	if (g_ui.hPopUp) return;

	g_ui.hPopUp = CreateWindowExW(
		WS_EX_DLGMODALFRAME,
		L"PopupClass",
		L"My PopUp Form",
		WS_POPUP | WS_CAPTION | WS_SYSMENU,
		400, 200, 800, 600,
		parent, NULL, hInst, NULL
	);

	ShowWindow(g_ui.hPopUp, SW_SHOW);
	UpdateWindow(g_ui.hPopUp);
}

// -- Create A New Window --
LRESULT CALLBACK PopupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		g_ui.hTextBox = CreateWindowW(
			L"EDIT",
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			20, 20, 200, 25,
			hWnd, NULL, GetModuleHandle(NULL), NULL
		);

		g_ui.hAddText = CreateWindow(
			L"BUTTON",
			L"ADD",
			WS_CHILD | WS_VISIBLE,
			20, 60, 80, 30,
			hWnd, (HMENU)2, NULL, NULL
		);

		// -- Create Tab --
		g_ui.hTab = CreateWindow(
			WC_TABCONTROL,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
			20, 100, 600, 450,
			hWnd, (HMENU)100, NULL, NULL
		);

		// -- Add Tab --
		TCITEM tie;
		tie.mask = TCIF_TEXT;

		tie.pszText = (LPWSTR)L"Home";
		TabCtrl_InsertItem(g_ui.hTab, 0, &tie);

		tie.pszText = (LPWSTR)L"Player";
		TabCtrl_InsertItem(g_ui.hTab, 1, &tie);

		// -- Create Pages --
		g_ui.hPages[0] = CreateWindow(
			L"STATIC",
			NULL,
			WS_CHILD | WS_VISIBLE,
			35, 150, 550, 390,
			hWnd, NULL, NULL, NULL
		);

		g_ui.hPages[1] = CreateWindow(
			L"STATIC",
			L"PLAYER PAGE",
			WS_CHILD,
			35, 150, 550, 390,
			hWnd, NULL, NULL, NULL
		);

		Create_ListSongs(g_ui.hPages[1], 30, 70, 450, 200, 4);

		Add_ColumnListView(listSongs, COL_ID, (LPWSTR)L"ID", 50);
		Add_ColumnListView(listSongs, COL_TITLE, (LPWSTR)L"TITLE", 300);
		Add_ColumnListView(listSongs, COL_DURATION, (LPWSTR)L"DURATION", 100);

		Clear_ListSongs();

		// -- Load All Songs From DB To List View --
		Database_LoadSongs(listSongs);
	}
	break;
	case WM_NOTIFY:
		CallTab(lParam);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 2)
		{
			wchar_t buf[256];
			GetWindowTextW(g_ui.hTextBox, buf, 256);
			MessageBoxW(hWnd, buf, L"Add Your List Song Success", MB_OK);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		g_ui.hPopUp = NULL;
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
