#include "Tab.h"
#include <commctrl.h>

HWND hTab;
HWND hPages[2];

LRESULT CALLBACK PageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RegisterPageClass(HINSTANCE hInst)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = PageProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"PageWindow";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);
}

HWND Create_Tab(HWND hParent, int id) 
{
	return hTab = CreateWindow(
		WC_TABCONTROL,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		20, 60, 600, 350,
		hParent, (HMENU)id, NULL, NULL
	);
}

void Create_ChildTab(HWND hParent) 
{
	hPages[0] = CreateWindow(
		L"STATIC",
		L"HOME",
		WS_CHILD | WS_VISIBLE,
		35, 100, 550, 290,
		hParent, NULL, NULL, NULL
	);

	hPages[1] = CreateWindow(
		L"PageWindow",
		L"PLAYER LIST",
		WS_CHILD,
		35, 100, 550, 290,
		hParent, NULL, NULL, NULL
	);
}

void Add_ChildTab(const wchar_t* text, int id)
{
	TCITEM tie;
	tie.mask = TCIF_TEXT;

	tie.pszText = (LPWSTR)text;
	TabCtrl_InsertItem(hTab, id, &tie);
}

LRESULT CALLBACK PageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case 5:
			{

			}
				break;
			}
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
