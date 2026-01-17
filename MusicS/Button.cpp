#include "Button.h"

HWND Create_Button(HWND hParent, int id, const wchar_t* text, int x, int y, int w, int h) 
{
	return CreateWindow(
		L"BUTTON",
		text,
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, y, w, h,
		hParent,
		(HMENU)id,
		NULL,
		NULL
	);
}