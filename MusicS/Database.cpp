#include "database.h"
#include "sqlite3.h"
#include "Player.h"
#include <stdio.h>
#include <CommCtrl.h>

sqlite3* db;

void Database_Init()
{
    int rc = sqlite3_open("D:\\MusicS\\MusicS\\songs.db", &db);

    if (rc != SQLITE_OK) {
        MessageBox(NULL, TEXT("Cannot open database"), TEXT("Error"), MB_OK);
    }

    const char* createTable =
        "CREATE TABLE IF NOT EXISTS songs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT,"
        "path TEXT,"
        "duration INTEGER)";
        

    char* errMsg = 0;
    rc = sqlite3_exec(db, createTable, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        MessageBoxA(NULL, errMsg, "SQL Error", MB_OK);
        sqlite3_free(errMsg);
    }
}

void Database_LoadSongs(HWND listView)
{
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM songs";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    int index = 0;

    while(sqlite3_step(stmt) == SQLITE_ROW) 
    {
        int id = sqlite3_column_int(stmt, 0);
        const char* title = (const char*)sqlite3_column_text(stmt, 1);
        //const char* path = (const char*)sqlite3_column_text(stmt, 2);
        int duration = sqlite3_column_int(stmt, 3);

        // ---- Convert ID to wchar ----
        char idBuffer[32];
        sprintf_s(idBuffer, "%d", id);

        wchar_t idW[32];
        MultiByteToWideChar(CP_UTF8, 0, idBuffer, -1, idW, 32);

        wchar_t titleW[256];
        MultiByteToWideChar(CP_UTF8, 0, title, -1, titleW, 256);

        //wchar_t pathW[256];
        //MultiByteToWideChar(CP_UTF8, 0, path, -1, pathW, 256);

        // ---- Format duration mm:ss ----
        int min = duration / 60;
        int sec = duration % 60;

        wchar_t durationW[256];
        swprintf_s(durationW, 32, L"%02d:%02d", min, sec);

        LVITEM lvi = { 0 };
        lvi.mask = LVIF_TEXT | LVIF_PARAM;
        lvi.iItem = index;
        lvi.pszText = idW;
        lvi.lParam = (LPARAM)id;

        ListView_InsertItem(listView, &lvi);

        ListView_SetItemText(listView, index, 1, titleW);
        //ListView_SetItemText(listView, index, 2, pathW);
        ListView_SetItemText(listView, index, 2, durationW);

        index++;
    }

    sqlite3_finalize(stmt);
}

int GetSongById(int id, char* outPath, int maxLen)
{
    sqlite3_stmt* stmt;
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

int SongCount()
{
    sqlite3_stmt* stmt;
    const char* sql = "SELECT COUNT(*) FROM songs";

    int count = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    if(sqlite3_step(stmt) == SQLITE_ROW) 
    {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    return count;
}

void InserSongDurationDB(int id, const char* path) 
{
    int duration = GetSongLength(path);

    sqlite3_stmt* stmt;
    const char* sql = "UPDATE songs SET duration = ? WHERE id = ?";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return;

    sqlite3_bind_int(stmt,1,duration);
    sqlite3_bind_int(stmt, 2, id);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);   
}

int CheckSongDurationStatus(int id) 
{
    sqlite3_stmt* stmt;
    const char* sql = "SELECT EXISTS(SELECT duration FROM songs WHERE id = ? AND duration > 0)";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) return 0;

    sqlite3_bind_int(stmt, 1, id);

    int exist = 0;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exist = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);

    return exist;
}

void InserSongIntoDB(const char* title,const char* path, int duration)
{
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO songs (title, path, duration) VALUES (?,?,?)";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Prepare failed: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, path, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, duration);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("Insert failed: %s\n", sqlite3_errmsg(db));
    }


    sqlite3_finalize(stmt);
}

