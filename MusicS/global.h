#pragma once
#include <Windows.h>
#include "bass.h"

#define WM_PLAY_NEXT_SONG (WM_APP + 1)
extern HWND g_hWnd;
extern HINSTANCE g_hInst;

extern int g_totalTime;
extern int g_isSeeking;
extern int g_currentId;

extern int g_SongId;
extern int g_PlayListId;

extern const char* sql;

// -- Wave Form config --
#define WAVE_SAMPLES 512
#define WAVE_X 20
#define WAVE_Y 500
#define WAVE_WIDTH 500
#define WAVE_HEIGHT 40

// -- Spectrum config --
#define SPEC_BARS     64
#define SPEC_X        20
#define SPEC_Y        600
#define SPEC_WIDTH    500
#define SPEC_HEIGHT   70
