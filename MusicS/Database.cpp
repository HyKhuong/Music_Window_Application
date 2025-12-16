#include "database.h"
#include "sqlite3.h"
#include <stdio.h>
#include <CommCtrl.h>

sqlite3* db;
sqlite3_stmt* stmt;

void Database_Init()
{
    int rc = sqlite3_open("D:\\MusicS\\MusicS\\songs.db", &db);

    if (rc != SQLITE_OK) {
        MessageBox(NULL, TEXT("Cannot open database"), TEXT("Error"), MB_OK);
        return;
    }

    const char* createTable =
        "CREATE TABLE IF NOT EXISTS songs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT,"
        "path TEXT);";


    char* errMsg = 0;
    rc = sqlite3_exec(db, createTable, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        MessageBoxA(NULL, errMsg, "SQL Error", MB_OK);
        sqlite3_free(errMsg);
    }
}

void Database_LoadSongs(HWND listView)
{
    const char* sql = "SELECT * FROM songs";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    int index = 0;

    while(sqlite3_step(stmt) == SQLITE_ROW) 
    {
        int id = sqlite3_column_int(stmt, 0);
        const char* title = (const char*)sqlite3_column_text(stmt, 1);
        const char* path = (const char*)sqlite3_column_text(stmt, 2);

        // ---- Convert ID to wchar ----
        char idBuffer[32];
        sprintf_s(idBuffer, "%d", id);

        wchar_t idW[32];
        MultiByteToWideChar(CP_UTF8, 0, idBuffer, -1, idW, 32);

        // ---- Convert title to wchar ----
        wchar_t titleW[256];
        MultiByteToWideChar(CP_UTF8, 0, title, -1, titleW, 256);

        // ---- Convert path to wchar ----
        wchar_t pathW[512];
        MultiByteToWideChar(CP_UTF8, 0, path, -1, pathW, 512);;

        LVITEM lvi = { 0 };
        lvi.mask = LVIF_TEXT | LVIF_PARAM;
        lvi.iItem = index;
        lvi.pszText = idW;
        lvi.lParam = (LPARAM)id;
        ListView_InsertItem(listView, &lvi);

        ListView_SetItemText(listView, index, 1, titleW);
        ListView_SetItemText(listView, index, 2, pathW);

        index++;
    }

    sqlite3_finalize(stmt);
}

int GetSongById(int id, char* outPath, int maxLen)
{
    const char* sql = "SELECT * FROM songs WHERE id = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return 0;
    }

    sqlite3_bind_int(stmt, 1, id);

    int success = 0;
    if(sqlite3_step(stmt) == SQLITE_ROW) 
    {
        const unsigned char* path = sqlite3_column_text(stmt, 2);
        strncpy_s(outPath,maxLen, (const char*)path, _TRUNCATE);
        printf("Run complete");
        success = 1;
    }

    sqlite3_finalize(stmt);

    return success;
}
