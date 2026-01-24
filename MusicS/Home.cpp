#include "Tab.h"
#include <commctrl.h>
#include "Database.h"
#include "Button_Type.h"
#include "listSongs_Type.h"
#include "ListSongs.h"
#include "Tab_Type.h"
#include "Ui.h"
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

// -- Click Songs --
void ClickSongs(LPARAM lParam)
{
	LPNMHDR hdr = (LPNMHDR)lParam;

	if (hdr->idFrom == 4)
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

				ListView_GetItem(HomeSongs_List, &item);
				int id = item.lParam;
				char path[521];

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

			int row = ListView_SubItemHitTest(HomeSongs_List, &hit);

			LVITEM item = { 0 };
			item.mask = LVIF_PARAM;
			item.iItem = row;

			ListView_GetItem(HomeSongs_List, &item);
			g_SongId = item.lParam;

			if (row == -1) break;

			ListView_SetItemState(
				HomeSongs_List,
				row,
				LVIS_SELECTED | LVIS_FOCUSED,
				LVIS_SELECTED | LVIS_FOCUSED
			);
			Create_MenuPopUp(p, HomeSongs_List, 1, L"Open Songs Detail");
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
		ClickSongs(lParam);
		break;
	case WM_COMMAND:
		MenuPopUp_HandleCommand(wParam, hWnd);
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}




