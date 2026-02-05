#include "Tab.h"
#include <commctrl.h>
#include "Database.h"
#include "Button_Type.h"
#include "listSongs_Type.h"
#include "ListSongs.h"
#include "Tab_Type.h"
#include "global.h"
#include "PopUp.h"
#include "Button.h"
#include "Player.h"

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

void PlayList_Init(Button btn)
{
	// -- Create Player List Song --
	PlayListSongs_List = Create_ListSongs(hPages[1], 25, 70, 460, 200, 10);

	Add_ColumnListView(PlayListSongs_List, COL_PLAY, L"@", 50);
	Add_ColumnListView(PlayListSongs_List, COL_ID, L"ID", 50);
	Add_ColumnListView(PlayListSongs_List, COL_TITLE, L"TITLE", 250);
	Add_ColumnListView(PlayListSongs_List, 3, L"Total", 50);

	ListView_DeleteAllItems(PlayListSongs_List);

	Database_LoadPlayList(PlayListSongs_List);

	hTextBox = CreateWindowW(
		L"EDIT",
		L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		160, 20, 200, 25,
		hPages[1], NULL, GetModuleHandle(NULL), NULL
	);

	btn.AddText = Create_Button(hPages[1], 5, L"ADD", 370, 20, 80, 25);
}

void OpenPlayList_Songs(LPARAM lParam)
{
	LPNMHDR hdr = (LPNMHDR)lParam;
	if (hdr->idFrom == 10)
	{
		switch (hdr->code)
		{
			case NM_CLICK:
			{
				LPNMITEMACTIVATE p = (LPNMITEMACTIVATE)lParam;
				int row = p->iItem;

				g_currentIndex = row;
				g_listView = PlayListSongs_List;
				
				int id = GetId_ListView(PlayListSongs_List, row, NULL);
				PutSongsToQueue(id);

				if(!g_stream)
				{
					PlaySongInQueue();
				}
				else {
					if(!g_isPaused)
					{
						ListView_SetItemText(PlayListSongs_List, row, 0, (LPTSTR)L">");
						BASS_ChannelPause(g_stream);
						g_isPaused = 1;
					}
					else {
						ListView_SetItemText(PlayListSongs_List, row, 0, (LPTSTR)L"=");
						BASS_ChannelPlay(g_stream, FALSE);
						g_isPaused = 0;
					}
				}
				
			}
			break;

			case NM_RCLICK:
			{
				LPNMITEMACTIVATE p = (LPNMITEMACTIVATE)lParam;
				LVHITTESTINFO h = { 0 };

				h.pt = p->ptAction;
				int row = ListView_SubItemHitTest(PlayListSongs_List, &h);

				if (row == -1) return;

				LVITEM item = { 0 };
				item.mask = LVIF_PARAM;
				item.iItem = row;

				ListView_GetItem(PlayListSongs_List, &item);
				g_PlayListId = item.lParam;

				Create_MenuPopUp(p, PlayListSongs_List, 2, L"Open PlayList Detail");
			}
			break;
		}
	}
}

LRESULT CALLBACK PlayListPageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_NOTIFY:
	{
		OpenPlayList_Songs(lParam);
		break;
	}
		case WM_COMMAND:
		{
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

			MenuPopUp_HandleCommand(wParam, hWnd);
		}

		
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}




