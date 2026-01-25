#include "database.h"
#include "sqlite3.h"
#include "Player.h"
#include <stdio.h>
#include <CommCtrl.h>

sqlite3* db;

void Database_Init()
{
    int rc = sqlite3_open("D:\\Work\\MusicS\\MusicS\\songs.db", &db);

    if (rc != SQLITE_OK) {
        MessageBox(NULL, TEXT("Cannot open database"), TEXT("Error"), MB_OK);
        sqlite3_close(db);
        db = NULL;
        return;
    }
} 

void Create_TableForDB(const char* sql)
{
    char* errMsg = 0;
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK)
    {
        MessageBoxA(NULL, errMsg, "Create Table Error", MB_OK);
        sqlite3_free(errMsg);
    }
}

int Insert_SongIntoPlayList(int SongId, int PlayListId)
{
    sqlite3_stmt* stmt;

    const char* sql = "INSERT INTO playlist_songs (playlist_id, song_id) VALUES (?, ?)";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        return SQLITE_ERROR;
    }

    sqlite3_bind_int(stmt, 1, PlayListId);
    sqlite3_bind_int(stmt, 2, SongId);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return rc;
}
