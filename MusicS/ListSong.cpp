#include "ListSongs.h"
#include "ListSongs_Type.h"

HWND listSongs;
//20, 70, 600, 300,
HWND Create_ListSongs(HWND hParent, int x, int y, int w, int h, int id)
{
	return listSongs = CreateWindow(
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

void Clear_ListSongs(void) {
	ListView_DeleteAllItems(listSongs);
}

void Add_ColumnListView(HWND listSongs,int index, LPWSTR text, int width)
{
	LVCOLUMN lvc = { 0 };
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lvc.iSubItem = index;
	lvc.pszText = (LPWSTR)text;
	lvc.cx = width;

	ListView_InsertColumn(listSongs, index, &lvc);
}

