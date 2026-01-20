#include <windows.h>
#include "Player.h"
#include "Database.h"
#include <CommCtrl.h>
#include "Ui.h"
#include <stdio.h>
#include "global.h"
#include "ListSongs_Type.h"
#include "PopUp.h"
#include "Tab.h"

#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
    g_hInst = hInst;
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = TEXT("MusicPlayerClass");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    //INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_TAB_CLASSES };
    //InitCommonControlsEx(&icex);

    RegisterClass(&wc);

    RegisterPopupClass(hInst);
    RegisterSongDetail(hInst);
    RegisterHomePageClass(hInst);
    RegisterPlayListPageClass(hInst);

    HWND hWnd = CreateWindow(
        wc.lpszClassName,
        TEXT("Hi-Res Music Player"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        560, 540,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        g_hWnd = hWnd;

        Database_Init();
        DB_Schema();

        UI_Init(hWnd);

        Player_Init();
        //SetTimer(hWnd, 1, 16, NULL);
    }
        break;
    case WM_NOTIFY:
    {
        // -- Tab Switch Control
        CallTab(lParam);
    }
    break;
    case WM_HSCROLL:
        GetScrollPosition(lParam, wParam);
        break;
    case WM_TIMER:
        UpdateTimer();

        //InvalidateRect(hWnd, &g_waveRect, FALSE);
        //InvalidateRect(hWnd, &g_barRect, FALSE);
        break;
    case WM_PLAY_NEXT_SONG:
        KillTimer(hWnd, 1);
        Player_Next_Song();
        break;
    case WM_COMMAND:
        UI_HandleCommand(wParam, g_hInst);
        break;
    /*case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        DrawWaveForm(hdc);
        DrawSpecTrum(hdc);
        EndPaint(hWnd, &ps);
    }*/
    case WM_ERASEBKGND:
        return 1; // prevent flicker
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
