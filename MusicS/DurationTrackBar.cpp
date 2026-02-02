#include "DurationTrackBar.h"
#include "DurationTrackBar_Type.h"
#include <CommCtrl.h>
#include "ListSongs_Type.h"
#include "stdio.h"

TrackBar track;

void Create_TrackBar(HWND hParent)
{
	// -- Show Duration Bar --
	track.hTimeText = CreateWindow(
		L"STATIC",
		L"00:00 / 00:00",
		WS_CHILD | WS_VISIBLE,
		20, 420, 120, 30,
		hParent,
		(HMENU)8,
		NULL,
		NULL
	);

	// -- Show Track Bar --
	track.hTrack = CreateWindowEx(
		0, TRACKBAR_CLASS, NULL,
		WS_CHILD | WS_VISIBLE | TBS_HORZ,
		150, 420, 370, 30,
		hParent, (HMENU)9,
		NULL,
		NULL
	);

	track.tSongPlay = CreateWindow(
		L"STATIC",
		L"NOW PLAYING:",
		WS_CHILD | WS_VISIBLE,
		20, 460, 500, 30,
		hParent, 
		(HMENU)10,
		NULL,
		NULL
	);
}

void Display_CurrentSong(HWND ListView, int index)
{
	wchar_t songName[256];
	ListView_GetItemText(ListView, index, COL_TITLE, songName, 256);

	wchar_t title[256];
	swprintf_s(title, 256, L"Now Playing: %s", songName);

	SetWindowText(track.tSongPlay, title);
}