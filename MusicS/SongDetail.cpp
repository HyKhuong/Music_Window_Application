#include "windows.h"
#include "Tab.h"
#include <commctrl.h>
#include "global.h"

HWND SongDetail;
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

LRESULT CALLBACK SongDetailProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_NOTIFY:

			break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
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