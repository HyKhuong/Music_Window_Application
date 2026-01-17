#include <cstddef>
#include <wtypes.h>
#include "bass.h"

// -- Track Song Duration --
HWND hTimeText = NULL;
HWND hTrack = NULL;

HWND g_hWnd = NULL;
HINSTANCE g_hInst = NULL;

int g_totalTime = 0;

int g_currentId = -1;
int isPaused = 0;
int g_isSeeking = 0;