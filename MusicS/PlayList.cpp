#include "Tab.h"
#include <commctrl.h>
#include "Database.h"
#include "Button_Type.h"
#include "listSongs_Type.h"
#include "ListSongs.h"
#include "Tab_Type.h"

LRESULT CALLBACK PlayListPageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RegisterPlayListPageClass(HINSTANCE hInst)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = PlayListPageProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"PlayListPage";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&wc);
}

LRESULT CALLBACK PlayListPageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 5:
		{
			wchar_t Wtext[256];
			GetWindowTextW(hTextBox, Wtext, 256);

			char text[256];
			WideCharToMultiByte(
				CP_UTF8, 0,
				Wtext, -1,
				text, sizeof(text),
				NULL, NULL
			);

			if (text[0] == '\0') {
				MessageBoxW(hPages[1], L"You need to enter your playlist name", L"ERROR", MB_OK);
				break;
			}

			ListView_DeleteAllItems(PlayListSongs_List);
			InserIntoPlayerList(text);
			SetWindowText(hTextBox, L"");
			Database_LoadPlayList(PlayListSongs_List);
		}
		break;
		}
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}




