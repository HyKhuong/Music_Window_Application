#include "Tab.h"
#include "global.h"
#include <commctrl.h>
#include "ListSongs_Type.h"
#include "ListSongs.h"
#include "PopUp.h"
#include "Database.h"

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
		CW_USEDEFAULT, CW_USEDEFAULT, 530, 500,
		hParent,
		NULL,
		g_hInst,
		NULL
	);
	ShowWindow(PlayList_Songs, SW_SHOW);
	UpdateWindow(PlayList_Songs);

	return PlayList_Songs;
}

void PlayListSongsDetail_Init(HWND hParent)
{
	PlayListSongsDetail_List = Create_ListSongs(hParent, 25, 70, 460, 200, 7);

	Add_ColumnListView(PlayListSongsDetail_List, COL_PLAY, L"@", 30);
	Add_ColumnListView(PlayListSongsDetail_List, COL_ID, L"ID", 50);
	Add_ColumnListView(PlayListSongsDetail_List, COL_TITLE, L"TITLE", 300);
	Add_ColumnListView(PlayListSongsDetail_List, COL_DURATION, L"DURATION", 100);

	sql = "SELECT s.id, s.title, s.duration "
		"FROM playlist_songs ps "
		"INNER JOIN songs s " 
		"ON ps.song_id = s.id " 
		"WHERE ps.playlist_id = ?";

	LoadPlayList_Songs(PlayListSongsDetail_List, sql);
}

LRESULT CALLBACK PlayList_SongsProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_CREATE:
		{
			PlayListSongsDetail_Init(hWnd);
			break;
		}
		case WM_NOTIFY:
		{
			ClickSongs(lParam, 7);
			break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}