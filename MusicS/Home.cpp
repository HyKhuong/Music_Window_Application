#include "Tab.h"
#include <commctrl.h>
#include "Database.h"
#include "Button_Type.h"
#include "listSongs_Type.h"
#include "ListSongs.h"
#include "Tab_Type.h"
#include "Ui.h"

LRESULT CALLBACK HomePageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RegisterHomePageClass(HINSTANCE hInst)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = HomePageProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"HomePage";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);
}

LRESULT CALLBACK HomePageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_NOTIFY:
		// -- Play songs from click --
		ClickSongs(lParam);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}




