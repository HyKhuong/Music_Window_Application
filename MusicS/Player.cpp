#include "Player.h"
#include "bass.h"
#include <stdio.h>

#pragma comment(lib, "bass.lib")

static HSTREAM g_stream = 0;
static int isPaused = 0;  

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

    // Start playing
    if (!BASS_ChannelPlay(g_stream, FALSE)) {
        printf("Can't play file: %d\n", BASS_ErrorGetCode());
        return;
    }

    isPaused = 0; // reset pause flag
}

void Player_Pause()
{
    if (!g_stream) return;

    if (!isPaused) {
        BASS_ChannelPause(g_stream);
        isPaused = 1;
    }
    else {
        BASS_ChannelPlay(g_stream, FALSE); // resume
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
