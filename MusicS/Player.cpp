#include "Player.h"
#include "bass.h"
#include <stdio.h>
#include "global.h"
#include "Database.h"
#include <CommCtrl.h>

#pragma comment(lib, "bass.lib")

// -- Wave Form config --
#define WAVE_SAMPLES 512
#define WAVE_X 20
#define WAVE_Y 500
#define WAVE_WIDTH 500
#define WAVE_HEIGHT 40

RECT g_waveRect = {
    WAVE_X,
    WAVE_Y,
    WAVE_X + WAVE_WIDTH,
    WAVE_Y + WAVE_HEIGHT
};
// -- Spectrum config --
#define SPEC_BARS     64
#define SPEC_X        20
#define SPEC_Y        600
#define SPEC_WIDTH    500
#define SPEC_HEIGHT   70

RECT g_barRect = {
    SPEC_X,
    SPEC_Y,
    SPEC_X + SPEC_WIDTH,
    SPEC_Y + SPEC_HEIGHT
};

static HSTREAM g_stream = 0;
int g_currentId = -1;
static int isPaused = 0;  
int g_isSeeking = 0;
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

int GetSongLength(const char* filePath) 
{
    g_stream = BASS_StreamCreateFile(FALSE, filePath, 0, 0, BASS_STREAM_DECODE | BASS_SAMPLE_FLOAT);

    QWORD lengthBytes = BASS_ChannelGetLength(g_stream, BASS_POS_BYTE);
    double length = BASS_ChannelBytes2Seconds(g_stream, lengthBytes);

    BASS_StreamFree(g_stream);

    return (int)(length + 0.5);
}

void Player_Play(int id, const char* filePath)
{
    if (!filePath) return;

    g_currentId = id;

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

    // ---- SET DURATION + TRACKBAR ----
    double totalTime = BASS_ChannelBytes2Seconds(
        g_stream,
        BASS_ChannelGetLength(g_stream, BASS_POS_BYTE)
    );

    g_totalTime = (int)totalTime;

    SendMessage(hTrack, TBM_SETRANGE, TRUE, MAKELPARAM(0, g_totalTime));

    SendMessage(hTrack, TBM_SETPOS, TRUE, 0);

    wchar_t buf[32];
    swprintf_s(buf, 32, L"00:00 / %02d:%02d",
        g_totalTime / 60,
        g_totalTime % 60);

    SetWindowTextW(hTimeText, buf);

    // ---- START TIMER ----
    SetTimer(g_hWnd, 1, 500, NULL);
}

void UpdateTimer()
{
    if (!g_stream || g_isSeeking) return;

    double cur = BASS_ChannelBytes2Seconds(
        g_stream,
        BASS_ChannelGetPosition(g_stream, BASS_POS_BYTE)
    );

    int curSec = (int)cur;

    SendMessage(hTrack, TBM_SETPOS, TRUE, curSec);

    wchar_t buf[32];
    swprintf_s(buf, 32, L"%02d:%02d / %02d:%02d",
        curSec / 60, curSec % 60,
        g_totalTime / 60, g_totalTime % 60);

    SetWindowTextW(hTimeText, buf);
}

void GetScrollPosition(LPARAM lParam ,WPARAM wParam)
{
    if((HWND)lParam == hTrack && g_stream)
    {
        int code = LOWORD(wParam);

        if(code == TB_THUMBTRACK || code == TB_THUMBPOSITION)
        {
            g_isSeeking = 1;
            int pos = (int)SendMessage(hTrack, TBM_GETPOS, 0, 0);

            // Convert seconds Å® bytes
            QWORD bytePos = BASS_ChannelSeconds2Bytes(g_stream, (double)pos);

            // Seek
            BASS_ChannelSetPosition(g_stream, bytePos, BASS_POS_BYTE);

            // Update time text immediately
            wchar_t buf[32];
            swprintf_s(buf, 32, L"%02d:%02d / %02d:%02d",
                pos / 60, pos % 60,
                g_totalTime / 60, g_totalTime % 60);

            SetWindowTextW(hTimeText, buf);
        }else if(code == TB_ENDTRACK)
        {
            g_isSeeking = 0;
        }
    }
}

void DrawWaveForm(HDC hdc) 
{
    if (!g_stream) return;

    float wave[WAVE_SAMPLES];

    BASS_ChannelGetData(g_stream, wave, WAVE_SAMPLES | BASS_DATA_FLOAT);

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 200, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    int midY = WAVE_Y + WAVE_HEIGHT / 2;

    MoveToEx(hdc, WAVE_X, midY, NULL);

    for(int i = 0; i < WAVE_SAMPLES; i++)
    {
        int x = WAVE_X + (i * WAVE_WIDTH) / WAVE_SAMPLES;
        int y = midY - (int)(wave[i] * (WAVE_HEIGHT / 2));

        LineTo(hdc, x, y);
    }

    SelectObject(hdc, oldPen);
    DeleteObject(hPen); 
}

void DrawSpecTrum(HDC hdc)
{
    if (!g_stream) return;

    float FFT[1024];

    BASS_ChannelGetData(g_stream, FFT, BASS_DATA_FFT2048);

    int barWidth = SPEC_WIDTH / SPEC_BARS;

    HBRUSH hBrush = CreateSolidBrush(RGB(0, 200, 255));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    for(int i = 0; i < SPEC_BARS; i++)
    {
        // FFT index mapping (log-style)
        int fftIndex = i * 1024 / SPEC_BARS;

        float magnitude = FFT[fftIndex];

        // Boost visibility
        int barHeight = (int)(magnitude * SPEC_HEIGHT * 4);

        if (barHeight > SPEC_HEIGHT)
            barHeight = SPEC_HEIGHT;

        int x = SPEC_X + i * barWidth;
        int y = SPEC_Y + (SPEC_HEIGHT - barHeight);

        Rectangle(
            hdc,
            x,
            y,
            x + barWidth - 2,
            SPEC_Y + SPEC_HEIGHT
        );

        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);
    }
}

void Player_Next_Song()
{
    int id = g_currentId + 1;

    char path[256];
    
    if(GetSongById(id, path, sizeof(path))) 
    {
        Player_Play(id, path);
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
