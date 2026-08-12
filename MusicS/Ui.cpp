#include "Ui.h"

#include "Player.h"
#include "Database.h"
#include <cstdio>

#include "Button.h"
#include "Button_Type.h"
#include "DurationTrackBar.h"
#include "DurationTrackBar_Type.h"
#include "PopUp.h"
#include "ListSongs_Type.h"
#include "Tab_Type.h"

#include "ListSongs.h"
#include "Tab.h"

#include <commctrl.h>
#include "global.h"

#include <tchar.h>
#include <shlobj_core.h>
#include <wchar.h>
#include <string.h>

UIContext g_ui = { 0 };
Button btn;

void UI_FONT() 
{
	g_WinFont = CreateFontW(
		-14,                    
		0, 0, 0,
		FW_NORMAL,
		FALSE, FALSE, FALSE,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY,        
		FF_DONTCARE,
		L"Segoe UI"        
	);
}

void SetUIFont(HWND hCtrl)
{
	SendMessageW(hCtrl, WM_SETFONT, (WPARAM)g_WinFont, TRUE);
}

void UI_Init(HWND hWnd)
{	
	// -- Button --
	btn.btnPlay = Create_Button(hWnd, 1, L"Play", 20, 20, 80, 30);
	SetUIFont(btn.btnPlay);

	btn.btnPause = Create_Button(hWnd, 2, L"Pause", 110, 20, 80, 30);
	SetUIFont(btn.btnPause);

	btn.btnStop = Create_Button(hWnd, 3, L"Stop", 200, 20, 80, 30);
	SetUIFont(btn.btnStop);

	btn.filePicker = Create_Button(hWnd, 4, L"Add File", 350, 20, 80, 30);
	SetUIFont(btn.filePicker);

	btn.btnAddFolder = Create_Button(hWnd, 5, L"Add Folder", 440, 20, 80, 30);
	SetUIFont(btn.btnAddFolder);

	// -- Track Duration Bar --
	Create_TrackBar(hWnd);

	// -- Create Tab --
	Create_Tab(hWnd, 100);

	// -- Add Tab --
	Add_ChildTab(L"HOME", Home);
	Add_ChildTab(L"PLAYER LIST", PlayerList);

	// -- Create Tabs --
	Create_ChildTab(hWnd);

	// -- Create Home  List Song --
	HomePage_Init();

	// -- Create PlayList --
	PlayList_Init(btn);
}

// -- Get Add File to DB logic -- 
BOOL PickAFile(HWND hWnd, wchar_t* outPath, DWORD outSize)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
		
	wchar_t fileName[MAX_PATH] = L"";

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter =
		L"Audio Files (*.mp3;*.wav;*.flac)\0*.mp3;*.wav;*.flac\0"
		"All Files (*.*)\0*.*\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.lpstrTitle = L"Select a song";

	if (GetOpenFileName(&ofn))
	{
		wcsncpy_s(outPath, outSize, fileName, _TRUNCATE);
		return TRUE;
	}
	return FALSE;
}

char* removeString(char* songName)
{
	while (*songName >= '0' && *songName <= '9')
	{
		songName++;
		if (*songName == '-')
		{
			songName++;
		}
		else if (*songName == '.')
		{
			songName++;
		}
	}

	return songName;
}

char* GetPathFromFile(char* path, char* fileName)
{
	char* p;
	p = strstr(path, "*");

	//char* arr = (char*)malloc(sizeof(path) + sizeof(fileName));

	strcpy_s(p, strlen(path) + strlen(fileName) + 1, fileName);
	return path;
}

void FindData(wchar_t* filePath)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	DWORD  retval = 0;
	TCHAR  fullPath[256] = TEXT("");
	TCHAR* lppPart = NULL;

	hFind = FindFirstFile(filePath, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return;
	}
	
	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			_tprintf(TEXT("  %s   <DIR>\n"), FindFileData.cFileName);
		}
		else
		{
			char pathC[MAX_PATH];
			char title[MAX_PATH];

			WideCharToMultiByte(CP_UTF8, 0, filePath, -1, pathC, sizeof(pathC), NULL, NULL);

			WideCharToMultiByte(CP_UTF8, 0, FindFileData.cFileName, -1, title, sizeof(title), NULL, NULL);

			char* pathA = GetPathFromFile(pathC, title);

			int duration = GetSongLength(pathA);

			InsertSongIntoDB(title, pathA, duration);
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);

	ListView_DeleteAllItems(HomeSongs_List);
	LoadList_Songs(HomeSongs_List, g_sql);
}

void PickFolder()
{
	IFileDialog* pfd;
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
	{
		DWORD dwOptions;
		if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
		{
			pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
		}
		if (SUCCEEDED(pfd->Show(NULL)))
		{
			IShellItem* psi;
			if (SUCCEEDED(pfd->GetResult(&psi)))
			{
				PWSTR pszFolderPath = (PWSTR)"";
				if (SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFolderPath)))
				{
					PWSTR quot = (PWSTR)"\\";
					PWSTR all = (PWSTR)"*";

					wcscat_s(pszFolderPath, wcslen(pszFolderPath) + wcslen(quot) + 1, quot);
					wcscat_s(pszFolderPath, wcslen(pszFolderPath) + wcslen(all) + 1, all);

					FindData(pszFolderPath);
				}
				psi->Release();
			}
		}
		pfd->Release();
	}
}

void PickSongToDB(HWND hWnd) 
{
	wchar_t path[MAX_PATH];

	if (!PickAFile(hWnd, path, MAX_PATH)) return;

	char pathC[MAX_PATH];

	WideCharToMultiByte(CP_UTF8, 0, path, -1, pathC, sizeof(pathC), NULL, NULL);

	char title[MAX_PATH];
	char* name = strrchr(pathC, '\\');
	name = name ? name + 1 : pathC;

	char* songName = removeString(name);

	strncpy_s(title, sizeof(title), songName, _TRUNCATE);
	char* dot = strrchr(title, '.');
	if (dot) *dot = '\0';

	int duration = GetSongLength(pathC);

	InsertSongIntoDB(title, pathC, duration);

	ListView_DeleteAllItems(HomeSongs_List);
	LoadList_Songs(HomeSongs_List, g_sql);

	UpdateTimer();
}

// -- Handle system --
void UI_HandleCommand(WPARAM wParam, HINSTANCE hInst) 
{
	switch (LOWORD(wParam))
	{
	case 1:
	{
		wchar_t path[256];
		wchar_t tile[10];
		if (GetSongById(1, path, tile))
		{
			Player_Play(1, path);
		}
	}
		break;

	case 2:
		Player_Pause();
		break;

	case 3:
		Player_Stop();
		break;

	case 4:
		PickSongToDB(g_ui.hWnd);
		break;

	case 5:
		PickFolder();
	}
}