#include <wtypes.h>
#include "Ui.h"
#include <commctrl.h>
#include "ListSongs.h"
#include "Database.h"
#include "ListSongs_Type.h"
#include "Tab_Type.h"
#include "Tab.h"

LRESULT CALLBACK PopupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// -- Create A New Window --
LRESULT CALLBACK PopupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		/*hTextBox = CreateWindowW(
			L"EDIT",
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			20, 20, 200, 25,
			hWnd, NULL, GetModuleHandle(NULL), NULL
		);*/

		/*g_ui.hAddText = CreateWindow(
			L"BUTTON",
			L"ADD",
			WS_CHILD | WS_VISIBLE,
			20, 60, 80, 30,
			hWnd, (HMENU)2, NULL, NULL
		);*/

	}
	break;
	case WM_NOTIFY:
		CallTab(lParam);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 2)
		{
			/*wchar_t buf[256];
			GetWindowTextW(g_ui.hTextBox, buf, 256);
			MessageBoxW(hWnd, buf, L"Add Your List Song Success", MB_OK);*/
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		g_ui.hPopUp = NULL;
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void RegisterPopupClass(HINSTANCE hInst)
{
	WNDCLASSW wc = { 0 };
	wc.lpfnWndProc = PopupProc;
	wc.hInstance = hInst;
	wc.lpszClassName = L"PopupClass";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_TAB_CLASSES };
	InitCommonControlsEx(&icex);

	RegisterClassW(&wc);
}

// -- Pop up -- 
void ShowPopUp(HWND parent, HINSTANCE hInst)
{
	if (g_ui.hPopUp) return;

	g_ui.hPopUp = CreateWindowExW(
		WS_EX_DLGMODALFRAME,
		L"PopupClass",
		L"My PopUp Form",
		WS_POPUP | WS_CAPTION | WS_SYSMENU,
		400, 200, 800, 600,
		parent, NULL, hInst, NULL
	);

	ShowWindow(g_ui.hPopUp, SW_SHOW);
	UpdateWindow(g_ui.hPopUp);
}

// -- Pop Menu --
void Create_MenuPopUp(LPNMITEMACTIVATE p, HWND ListView)
{
	HMENU menu = CreatePopupMenu();
	AppendMenuW(menu, MF_STRING, 1, L"Open Song Detail");

	POINT pt = p->ptAction;
	ClientToScreen(ListView, &pt);

	TrackPopupMenu(
		menu,
		TPM_RIGHTBUTTON,
		pt.x, pt.y,
		0,
		GetParent(HomeSongs_List),
		NULL
	);

	DestroyMenu(menu);
}

void MenuPopUp_HandleCommand(WPARAM wParam, HWND hWnd)
{
	switch (LOWORD(wParam)) 
	{
		case 1:
			Create_SongDetailPage(hWnd);
			break;
	}
}

