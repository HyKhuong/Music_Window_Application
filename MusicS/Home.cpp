#include "Tab.h"
#include <commctrl.h>
#include "Database.h"
#include "Button_Type.h"
#include "listSongs_Type.h"
#include "ListSongs.h"
#include "Tab_Type.h"
#include "PopUp.h"
#include "global.h"
#include "Player.h"

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

void HomePage_Init()
{
	// -- Create Home List Song --
	HomeSongs_List = Create_ListSongs(hPages[0], 25, 70, 460, 200, 4);

	Add_ColumnListView(HomeSongs_List, COL_PLAY, L"@", 30);
	Add_ColumnListView(HomeSongs_List, COL_ID, L"ID", 50);
	Add_ColumnListView(HomeSongs_List, COL_TITLE, L"TITLE", 300);
	Add_ColumnListView(HomeSongs_List, COL_DURATION, L"DURATION", 100);

	ListView_DeleteAllItems(HomeSongs_List);

	// -- Load All Songs From DB To List View --
	sql = "SELECT id, title, duration FROM songs";

	LoadList_Songs(HomeSongs_List, sql);
}

// -- Click Songs --
void ClickSongs(HWND ListView, LPARAM lParam, int id)
{
	LPNMHDR hdr = (LPNMHDR)lParam;

	if (hdr->idFrom == id)
	{
		switch (hdr->code)
		{
		case NM_CLICK:
		{
			LPNMITEMACTIVATE p = (LPNMITEMACTIVATE)lParam;

			int row = p->iItem;
			int col = p->iSubItem;

			if (col == 0 && row != -1)
			{
				LVITEM item = { 0 };
				item.mask = LVIF_PARAM;
				item.iItem = row;

				g_currentIndex = row;
				g_listView = ListView;

				ListView_GetItem(ListView, &item);
				int id = item.lParam;
				wchar_t path[521];

				if (GetSongById(id, path, sizeof(path)))
				{
					Player_Play(id, path);
				}
				else
				{
					MessageBoxA(NULL, "Song not found", "ERROR", MB_OK);
				}
			}
		}
		break;

		case NM_RCLICK:
		{
			LPNMITEMACTIVATE p = (LPNMITEMACTIVATE)lParam;

			LVHITTESTINFO hit = { 0 };
			hit.pt = p->ptAction;

			int row = ListView_SubItemHitTest(ListView, &hit);

			LVITEM item = { 0 };
			item.mask = LVIF_PARAM;
			item.iItem = row;

			ListView_GetItem(ListView, &item);
			g_SongId = item.lParam;

			if (row == -1) break;

			ListView_SetItemState(
				ListView,
				row,
				LVIS_SELECTED | LVIS_FOCUSED,
				LVIS_SELECTED | LVIS_FOCUSED
			);
			Create_MenuPopUp(p, ListView, 1, L"Open Songs Detail");
		}
		break;
		}
	}
}

LRESULT CALLBACK HomePageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_NOTIFY:
		// -- Play songs from click --
		ClickSongs(HomeSongs_List, lParam, 4);
		break;
	case WM_COMMAND:
		MenuPopUp_HandleCommand(wParam, hWnd);
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}




