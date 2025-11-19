#include "player.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static ma_engine engine;

void Player_Init()
{
    ma_engine_init(NULL, &engine);
}

void Player_Play()
{
    ma_engine_play_sound(&engine, "song.FLAC", NULL);
}

void Player_Pause() {}
void Player_Stop() {}
