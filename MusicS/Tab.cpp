#include "Tab.h"
#include <commctrl.h>
#include "Tab_Type.h"

HWND hTab;
HWND hPages[2];

HWND Create_Tab(HWND hParent, int id)
{
	return hTab = CreateWindow(
		WC_TABCONTROL,
		NULL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		20, 60, 500, 350,
		hParent, (HMENU)id, NULL, NULL
	);
}

void Create_ChildTab(HWND hParent)
{
	hPages[0] = CreateWindow(
		L"HomePage",
		NULL,
		WS_CHILD | WS_VISIBLE,
		35, 100, 450, 290,
		hParent, NULL, NULL, NULL
	);

	hPages[1] = CreateWindow(
		L"PlayListPage",
		NULL,
		WS_CHILD,
		35, 100, 450, 290,
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

// -- Show Tab Logic --
void ShowTabPage(int index)
{
	for (int i = 0; i < 3; i++)
	{
		ShowWindow(hPages[i], i == index ? SW_SHOW : SW_HIDE);
	}
}

void CallTab(LPARAM lParam)
{
	LPNMHDR hdr = (LPNMHDR)lParam;

	if (hdr->hwndFrom == hTab && hdr->code == TCN_SELCHANGE)
	{
		int sel = TabCtrl_GetCurSel(hTab);
		ShowTabPage(sel);
	}
}
