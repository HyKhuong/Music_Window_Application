#include "Tab.h"
#include "global.h"
#include <commctrl.h>
#include "ListSongs_Type.h"
#include "PopUp.h"

HWND PlayList_Songs;

LRESULT CALLBACK PlayList_SongsProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RegisterPlayListSongs(HINSTANCE hInst)
{
	WNDCLASSW wc = { 0 };
	wc.lpfnWndProc = PlayList_SongsProc;
	wc.lpszClassName = L"PlayListSongsClass";
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClassW(&wc);
}

HWND Create_PlayListSongs(HWND hParent)
{
	PlayList_Songs = CreateWindowEx(
		WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME,
		L"PlayListSongsClass",
		L"",
		WS_POPUP | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 500,
		hParent,
		NULL,
		g_hInst,
		NULL
	);
	ShowWindow(PlayList_Songs, SW_SHOW);
	UpdateWindow(PlayList_Songs);

	return PlayList_Songs;
}

LRESULT CALLBACK PlayList_SongsProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_NOTIFY:
		{
			
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}