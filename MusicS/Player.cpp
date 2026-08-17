#include "Player.h"
#include "bass.h"
#include "customUi.h"
#include <stdio.h>
#include "global.h"
#include "Database.h"
#include <CommCtrl.h>
#include "Ui.h"
#include "ListSongs.h"
#include "ListSongs_Type.h"
#include "DurationTrackBar.h"
#include "DurationTrackBar_Type.h"

#pragma comment(lib, "bass.lib")
int totalSong = SongCount();
int randomSet = 0;

void CALLBACK OnSongEnd(HSYNC handle, DWORD channel, DWORD data, void* user)
{
    if(!randomSet)
    {
        PostMessage(g_hWnd, WM_PLAY_NEXT_SONG, 0, 0);
    }
    else 
    {
        PostMessage(g_hWnd, WM_PLAY_RANDOM, 0, 0);
    }
}

void Player_Init()
{
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        printf("Can't initialize audio device. Error: %d\n", BASS_ErrorGetCode());
    }
}

int GetSongLength(char* filePath) 
{
    int stream = BASS_StreamCreateFile(FALSE, filePath, 0, 0, BASS_STREAM_DECODE | BASS_SAMPLE_FLOAT);

    QWORD lengthBytes = BASS_ChannelGetLength(stream, BASS_POS_BYTE);
    double length = BASS_ChannelBytes2Seconds(stream, lengthBytes);

    BASS_StreamFree(stream);

    return (int)(length + 0.5);
}

void SongDuration_Display()
{
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
    SetTimer(g_hWnd, 1, 1000, NULL);
}

void Player_Play(int id, const wchar_t* filePath)
{
    if (!filePath) return;

    if(!CheckSongDurationStatus(id)) 
    {
        InserSongDurationDB(id, filePath);
    }

    // Free previous stream if any
    if (g_stream) {
        BASS_StreamFree(g_stream);
        g_stream = 0;
    }

    // Load the file
    g_stream = BASS_StreamCreateFile(FALSE, filePath, 0, 0, 0);

    BASS_ChannelSetSync(
        g_stream,
        BASS_SYNC_END,
        0,
        OnSongEnd,
        NULL
    );

    // Start playing
    if (!BASS_ChannelPlay(g_stream, FALSE)) {
        printf("Can't play file: %d\n", BASS_ErrorGetCode());
        return;
    }

    g_isPaused = 0; 

    SongDuration_Display();
}

void setRandom()
{
    if(!randomSet)
    {
        randomSet = 1;
    }
    else
    {
        randomSet = 0;
    }
}

void Play_Random()
{
    int r = (rand() % 100) + 50;

    if(randomSet)
    {
        wchar_t* filePath = (wchar_t*)malloc(256 * sizeof(wchar_t));
        wchar_t* title = (wchar_t*)malloc(256 * sizeof(wchar_t));

        if (GetSongById(r, filePath, title))
        {
            Player_Play(r, filePath);
        }

        free(filePath);
        free(title);
    }
}

void UpdateTimer()
{
    if (!g_stream || g_isSeeking) return;

    int curSec = BASS_ChannelBytes2Seconds(
        g_stream,
        BASS_ChannelGetPosition(g_stream, BASS_POS_BYTE)
    );

    SendMessage(track.hTrack, TBM_SETPOS, TRUE, curSec);

    wchar_t buf[32];
    swprintf_s(buf, 32, L"%02d:%02d / %02d:%02d",
         curSec / 60, curSec % 60,
         g_totalTime / 60, g_totalTime % 60);

    SetWindowTextW(track.hTimeText, buf);
}

void GetScrollPosition(LPARAM lParam ,WPARAM wParam)
{
    if((HWND)lParam == track.hTrack && g_stream)
    {
        int code = LOWORD(wParam);

        if(code == TB_THUMBTRACK || code == TB_THUMBPOSITION)
        {
            g_isSeeking = 1;
            int pos = (int)SendMessage(track.hTrack, TBM_GETPOS, 0, 0);

            // Convert seconds ¨ bytes
            QWORD bytePos = BASS_ChannelSeconds2Bytes(g_stream, (double)pos);

            // Seek
            BASS_ChannelSetPosition(g_stream, bytePos, BASS_POS_BYTE);

            // Update time text immediately
            wchar_t buf[32];
            swprintf_s(buf, 32, L"%02d:%02d / %02d:%02d",
                pos / 60, pos % 60,
                g_totalTime / 60, g_totalTime % 60);

            SetWindowTextW(track.hTimeText, buf);
        }else if(code == TB_ENDTRACK)
        {
            g_isSeeking = 0;
        }
    }
}

void Player_Next_Song()
{
    ListView_SetItemText(g_listView, g_currentIndex, 0, (LPWSTR)">");

    int row = ++g_currentIndex;

    if(row < SongCount())
    {
        int id = GetId_ListView(g_listView, row, NULL);

        Display_CurrentSong(g_listView, row);

        ListView_SetItemText(g_listView, row, 0, (LPWSTR)"=");

        wchar_t* path = (wchar_t*)malloc(565 * sizeof(wchar_t));
        wchar_t* title = (wchar_t*)malloc(256 * sizeof(wchar_t));

        if (GetSongById(id, path, title))
        {
            Player_Play(id, path);
        }

        free(path);
        free(title);
    }
    else 
    {
        MessageBox(g_hWnd, L"There is no more songs", L"Warning", MB_OK);
        Player_Stop();
    }
}

void Player_Pause()
{
    if (!g_stream) return;

    if (!g_isPaused) {
        BASS_ChannelPause(g_stream);
        g_isPaused = 1;
    }
    else {
        BASS_ChannelPlay(g_stream, FALSE); 
        g_isPaused = 0;
    }
}

void Player_Stop()
{
    if (g_stream) {
        BASS_StreamFree(g_stream);
        g_stream = 0;
        SetWindowTextW(track.hTimeText, L"00:00 / 00:00");
    }
}

