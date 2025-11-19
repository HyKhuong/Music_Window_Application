#include "database.h"
#include "sqlite3.h"

sqlite3* db;

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
    // TODO: load songs into ListView
}

