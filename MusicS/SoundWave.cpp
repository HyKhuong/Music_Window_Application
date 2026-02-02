#include "global.h"

void DrawWaveForm(HDC hdc)
{
    if (!g_stream) return;

    float wave[WAVE_SAMPLES];

    BASS_ChannelGetData(g_stream, wave, WAVE_SAMPLES | BASS_DATA_FLOAT);

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 200, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    int midY = WAVE_Y + WAVE_HEIGHT / 2;

    MoveToEx(hdc, WAVE_X, midY, NULL);

    for (int i = 0; i < WAVE_SAMPLES; i++)
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

    for (int i = 0; i < SPEC_BARS; i++)
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