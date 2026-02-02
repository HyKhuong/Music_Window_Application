#include "ListSongs.h"
#include "ListSongs_Type.h"
#include <commctrl.h>

HWND HomeSongs_List;
HWND PlayListSongs_List;
HWND PlayListSongsDetail_List;

HWND Create_ListSongs(HWND hParent, int x, int y, int w, int h, int id)
{
	return CreateWindow(
		WC_LISTVIEW, 
		L"",
		WS_CHILD | WS_VISIBLE | LVS_REPORT,
		x, y, w, h,
		hParent, 
		(HMENU)id, 
		NULL, 
		NULL
	);
}

void Add_ColumnListView(HWND listSongs,int index, const wchar_t* text, int width)
{
	LVCOLUMN lvc = { 0 };
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.iSubItem = index;
	lvc.pszText = (LPWSTR)text;
	lvc.cx = width;

	ListView_InsertColumn(listSongs, index, &lvc);
}

int GetId_ListView(HWND ListView, int row, int column)
{
	LVITEM lv = { 0 };
	lv.mask = LVIF_PARAM;
	lv.iItem = row;
	lv.iSubItem = column;

	ListView_GetItem(ListView, &lv);

	int id = lv.lParam;
	return id;
}
