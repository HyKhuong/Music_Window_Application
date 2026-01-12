#pragma once
#include <windows.h>
#include "bass.h"

// -- Variable -- 
extern int g_currentId;

void CALLBACK OnSongEnd(
    HSYNC handle,
    DWORD channel,
    DWORD data,
    void* user
);

extern RECT g_waveRect;
extern RECT g_barRect;

// -- Function --
void Player_Init();
void Player_Play(int id, const char* filePath);
void Player_Next_Song();
void Player_Pause();
void Player_Stop();
int GetSongLength(const char* filePath);
void UpdateTimer();
void GetScrollPosition(LPARAM lParam, WPARAM WParam);

// -- Some player UI --
void DrawWaveForm(HDC hdc);
void DrawSpecTrum(HDC hdc);