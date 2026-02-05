#include <stdio.h>
#include <wchar.h>
#include "Player.h"
#include "global.h"
#include "DurationTrackBar_Type.h"
#include <commctrl.h>
#include "ListSongs.h"
#define MAX_QUEUE 100

wchar_t Queue[MAX_QUEUE][MAX_PATH];


static void CALLBACK SongEnd(HSYNC handle, DWORD channel, DWORD data, void* user)
{
    PostMessage(g_hWnd, WM_PLAY_NEXT_SONG_IN_QUEUE, 0, 0);
}

void play(wchar_t* path)
{
    if (g_stream)
    {
        BASS_StreamFree(g_stream);
        g_stream = 0;
    }

    g_stream = BASS_StreamCreateFile(FALSE, path, 0, 0, 0);

    BASS_ChannelPlay(g_stream, FALSE);

    BASS_ChannelSetSync(
        g_stream,
        BASS_SYNC_END,
        0,
        SongEnd,
        NULL
    );

    // ---- SET DURATION + TRACKBAR ----
    double totalTime = BASS_ChannelBytes2Seconds(
        g_stream,
        BASS_ChannelGetLength(g_stream, BASS_POS_BYTE)
    );

    g_totalTime = (int)totalTime;

    SendMessage(track.hTrack, TBM_SETRANGE, TRUE, MAKELPARAM(0, g_totalTime));

    SendMessage(track.hTrack, TBM_SETPOS, TRUE, 0);

    wchar_t buf[32];
    swprintf_s(buf, 32, L"00:00 / %02d:%02d",
        g_totalTime / 60,
        g_totalTime % 60);

    SetWindowTextW(track.hTimeText, buf);

    // ---- START TIMER ----
    SetTimer(g_hWnd, 1, 500, NULL);
}

void AddToQueue(const wchar_t* path)
{
    if (!path || !*path)
        return;

	if (queueCount >= MAX_QUEUE) return;

	wcscpy_s(Queue[queueCount], MAX_PATH, path);
	queueCount++;
}

void StopSongInQueue()
{
    if(g_stream)
    {
        ListView_SetItemText(g_listView, g_currentIndex, 0, (LPTSTR)L">");
        BASS_StreamFree(g_stream);
        g_stream = 0;
        SetWindowTextW(track.hTimeText, L"00:00 / 00:00");
    }
}

void PlaySongInQueue()
{
	if (currentSong <= queueCount)
	{
        ListView_SetItemText(g_listView, g_currentIndex, 0, (LPTSTR)L"=");
		play(Queue[currentSong]);
	}
    
}

void PlayNextSongInQueue()
{
	currentSong++;
	if (currentSong < queueCount)
	{
		play(Queue[currentSong]);
	}

    if(currentSong == queueCount)
    {
        StopSongInQueue();
    }
}

