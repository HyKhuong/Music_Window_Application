#include "player.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <io.h>
#include <stdio.h>

static ma_engine engine;

void Player_Init()
{
    ma_engine_init(NULL, &engine);
}

void Player_Play()
{
    if (_access("D:\\Music\\Call of the Night Season 2  FLAC 48kHz24bit\\01. Mirage.flac", 0) != 0) {
        MessageBox(NULL, L"File not found", L"Error", MB_OK);
        return;
    }

    ma_engine_play_sound(&engine, "D:\\Music\\Call of the Night Season 2  FLAC 48kHz24bit\\01. Mirage.flac", NULL);
}

void Player_Pause() {}
void Player_Stop() {}
