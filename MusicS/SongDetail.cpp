#include "windows.h"
#include "Tab.h"
#include <commctrl.h>
#include "global.h"
#include "Database.h"
#include "Button.h"

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
	 SongDetail = CreateWindowEx(
		WS_EX_DLGMODALFRAME,
		L"SongDetailClass",
		L"Song Info",
		WS_POPUP | WS_CAPTION | WS_SYSMENU,
		600, 40, 400, 300,
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
		(HMENU)1,
		NULL,
		NULL
	);
}

int GetComBoBox_ID(HWND ComboBox)
{
	int index = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (index == CB_ERR)
		return -1;

	return (int)SendMessage(hComboBox, CB_GETITEMDATA, index, 0);
}

LRESULT CALLBACK SongDetailProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			ComboBox(hWnd);
			LoadPlayList_Combobox(hComboBox);
			if (SendMessage(hComboBox, CB_GETCOUNT, 0, 0) > 0)
			{
				g_PlayListId = GetComBoBox_ID(hComboBox);
			}
			
			Create_Button(hWnd, 2, L"Add", 230, 20, 60, 30);
		}
		
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case 1:
				{
					if(HIWORD(wParam) ==  CBN_SELCHANGE)
					{
						g_PlayListId = GetComBoBox_ID(hComboBox);
						wchar_t buf[64];
						wsprintfW(buf, L"Playlist ID = %d", g_PlayListId);

						MessageBoxW(hWnd, buf, L"Debug", MB_OK);
					}
				}	
				break;
				case 2:
				{
					if (g_SongId == -1 || g_PlayListId == -1) 
					{
						MessageBox(hWnd, L"Some thing went wrong", L"Bug", MB_OK);
						break;
					}
					
					int rc = Insert_SongIntoPlayList(g_SongId, g_PlayListId);
					if (rc == SQLITE_DONE)
					{
						MessageBoxW(hWnd,
							L"Song added to playlist successfully!",
							L"Success",
							MB_OK | MB_ICONINFORMATION);
					}
					else if (rc == SQLITE_CONSTRAINT)
					{
						MessageBoxW(hWnd,
							L"This song already exists in the playlist.",
							L"Duplicate song",
							MB_OK | MB_ICONWARNING);
					}
					else
					{
						MessageBoxW(hWnd,
							L"Failed to add song to playlist.",
							L"Error",
							MB_OK | MB_ICONERROR);
					}
				}
				break;
			}
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}