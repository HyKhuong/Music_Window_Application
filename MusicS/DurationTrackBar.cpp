#include "DurationTrackBar.h"
#include "DurationTrackBar_Type.h"
#include <CommCtrl.h>

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
		150, 420, 620, 30,
		hParent, (HMENU)9,
		NULL,
		NULL
	);
}