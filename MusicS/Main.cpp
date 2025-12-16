#include <windows.h>
#include "Player.h"
#include "Database.h"
#include <CommCtrl.h>
#include "Ui.h"
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = TEXT("MusicPlayerClass");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hWnd = CreateWindow(
        wc.lpszClassName,
        TEXT("Hi-Res Music Player"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 500,
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
        Database_Init();
        UI_Init(hWnd);

        Player_Init();
        break;
    case WM_NOTIFY:
    {
        LPNMHDR hdr = (LPNMHDR)lParam;
        if (hdr->idFrom == 10 && hdr->code == LVN_ITEMACTIVATE)
        {
            LPNMITEMACTIVATE p = (LPNMITEMACTIVATE)lParam;

            int index = p->iItem;

            LVITEM item = { 0 };
            item.mask = LVIF_PARAM;
            item.iItem = index;

            ListView_GetItem(listSongs, &item);
            int id = item.lParam;
            char path[521];

            if (GetSongById(id, path, sizeof(path)))
            {
                Player_Play(path);
            }
            else
            {
                MessageBoxA(NULL, "Song not found", "ERROR", MB_OK);
            }

        }
    }
    case WM_COMMAND:
        UI_HandleCommand(wParam);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}
