#include "global.h"
#include "bass.h"

// -- Track Song Duration --
HWND hTimeText = NULL;
HWND hTrack = NULL;

HWND g_hWnd = NULL;
HINSTANCE g_hInst = NULL;

HSTREAM g_stream = 0;

int g_totalTime = 0;

int g_currentIndex = -1;
int g_isPaused = 0;
int g_isSeeking = 0;

int g_SongId = -1;
int g_PlayListId = -1;

HWND g_listView = NULL;
const char* g_sql = "SELECT id, title, duration FROM songs";

HFONT g_WinFont = NULL;

int currentSong = 0;
int queueCount = 0;

int g_currentPage = -1;