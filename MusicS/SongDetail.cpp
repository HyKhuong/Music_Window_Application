#include "windows.h"
#include "Tab.h"
#include <commctrl.h>
#include "global.h"
#include "Database.h"

HWND SongDetail;
HWND hComboBox;
LRESULT CALLBACK SongDetailProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RegisterSongDetail(HINSTANCE hInst)
{
	WNDCLASSW wc = { 0 };
	wc.lpfnWndProc = SongDetailProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"SongDetailClass";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_TAB_CLASSES };
	InitCommonControlsEx(&icex);

	RegisterClassW(&wc);
}

HWND Create_SongDetailPage(HWND hParent)
{
	 SongDetail = CreateWindowExW(
		WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME,
		L"SongDetailClass",
		L"Song Info",
		WS_POPUP | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
		hParent,
		NULL,
		g_hInst,
		NULL
	);

	ShowWindow(SongDetail, SW_SHOW);
	UpdateWindow(SongDetail);

	return SongDetail;
}

void ComboBox(HWND hParent) {
	hComboBox = CreateWindow(
		WC_COMBOBOXW,
		L"",
		CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
		20, 20, 200, 200,
		hParent,
		(HMENU)10,
		NULL,
		NULL
	);
}

void ComboboxSelect(HWND ComboBox)
{
	int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (index == CB_ERR)
		return;

	(int)SendMessage(hComboBox, CB_GETITEMDATA, index, 0);
}

LRESULT CALLBACK SongDetailProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
			ComboBox(hWnd);
			//SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
			LoadPlayList_Combobox(hComboBox);
			break;
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == 10 && HIWORD(wParam) == CBN_SELCHANGE)
			{
				ComboboxSelect(hComboBox);
			}
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}