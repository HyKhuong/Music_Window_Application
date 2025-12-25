#pragma once
#include <windows.h>
#include "bass.h"

extern int g_currentId;

void CALLBACK OnSongEnd(
    HSYNC handle,
    DWORD channel,
    DWORD data,
    void* user
);

void Player_Init();
void Player_Play(int id, const char* filePath);
void Player_Next_Song(int id);
void Player_Pause();
void Player_Stop();
int GetSongLength(const char* filePath);