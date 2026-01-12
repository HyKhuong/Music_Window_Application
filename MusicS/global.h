#pragma once

#include <Windows.h>

#define WM_PLAY_NEXT_SONG (WM_APP + 1)
extern HWND g_hWnd;
extern HINSTANCE g_hInst;

// Track Timer
extern HWND hTimeText;
extern HWND hTrack;
extern int g_totalTime;
extern int g_isSeeking;
