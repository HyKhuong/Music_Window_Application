#pragma once
#include <windows.h>
#include "bass.h"

// -- Variable -- 

void CALLBACK OnSongEnd(
    HSYNC handle,
    DWORD channel,
    DWORD data,
    void* user
);

// -- Function --
void Player_Init();
void Player_Play(int id, const wchar_t* filePath);
void Player_Next_Song();
void Player_Pause();
void Player_Stop();
int GetSongLength(char* filePath);
void UpdateTimer();
void GetScrollPosition(LPARAM lParam, WPARAM WParam);


// -- Some player UI --
void DrawWaveForm(HDC hdc);
void DrawSpecTrum(HDC hdc);

// -- Queue Player -- 
void AddToQueue(const wchar_t* path);
void PlaySongInQueue();
void PlayNextSongInQueue();
void play(const wchar_t* path);
void StopSongInQueue();