#include "Player.h"
#include "bass.h"
#include <stdio.h>
#include "global.h"
#include "Database.h"

#pragma comment(lib, "bass.lib")

static HSTREAM g_stream = 0;
int g_currentId = -1;
static int isPaused = 0;  
int totalSong = SongCount();

void CALLBACK OnSongEnd(HSYNC handle, DWORD channel, DWORD data, void* user)
{
    PostMessage(g_hWnd, WM_PLAY_NEXT_SONG, 0, 0);
}

void Player_Init()
{
    if (!BASS_Init(-1, 44100, 0, 0, NULL)) {
        printf("Can't initialize audio device. Error: %d\n", BASS_ErrorGetCode());
    }
}

void Player_Play(const char* filePath)
{
    if (!filePath) return;

    // Free previous stream if any
    if (g_stream) {
        BASS_StreamFree(g_stream);
        g_stream = 0;
    }

    // Load the file
    g_stream = BASS_StreamCreateFile(FALSE, filePath, 0, 0, 0);
    if (!g_stream) {
        printf("Can't load file: %d\n", BASS_ErrorGetCode());
        return;
    }

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

    isPaused = 0; 
}

void Player_Next_Song(int g_currentId)
{
    if(g_currentId < 0) 
    {
        return;
    }

    int id = g_currentId + 2;

    char path[256];
    
    if(GetSongById(g_currentId, path, sizeof(path))) 
    {
        Player_Play(path);
    }
}

void Player_Pause()
{
    if (!g_stream) return;

    if (!isPaused) {
        BASS_ChannelPause(g_stream);
        isPaused = 1;
    }
    else {
        BASS_ChannelPlay(g_stream, FALSE); 
        isPaused = 0;
    }
}

void Player_Stop()
{
    if (g_stream) {
        BASS_StreamFree(g_stream);
        g_stream = 0;
    }
}
