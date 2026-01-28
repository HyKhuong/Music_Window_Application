#include "Database.h"
#include "stdio.h"
#include "sqlite3.h"
#include "commctrl.h"

int CountSongsInPlayList(int id)
{
    int count = 0;
    sqlite3_stmt* stmt;
    const char* sql = "SELECT COUNT(song_id) FROM playlist_songs WHERE playlist_id = ?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) 
    {
        return 0;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    };

    sqlite3_finalize(stmt);

    return count;
}

void Database_LoadPlayList(HWND listView)
{
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM playlist";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    int index = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const char* title = (const char*)sqlite3_column_text(stmt, 1);

        int total = CountSongsInPlayList(id);
        // ---- Convert ID to wchar ----
        char idBuffer[32];
        sprintf_s(idBuffer, "%d", id);

        char totalBuffer[32];
        sprintf_s(totalBuffer, "%d", total);

        wchar_t idW[32];
        MultiByteToWideChar(CP_UTF8, 0, idBuffer, -1, idW, 32);

        wchar_t titleW[256];
        MultiByteToWideChar(CP_UTF8, 0, title, -1, titleW, 256);

        wchar_t totalW[32];
        MultiByteToWideChar(CP_UTF8, 0, totalBuffer, -1, totalW, 32);


        LVITEM lvi = { 0 };
        lvi.mask = LVIF_TEXT | LVIF_PARAM;
        lvi.iItem = index;
        lvi.pszText = idW;
        lvi.lParam = (LPARAM)id;

        ListView_InsertItem(listView, &lvi);

        ListView_SetItemText(listView, index, 1, titleW);
        ListView_SetItemText(listView, index, 2, totalW);
        index++;
    }

    sqlite3_finalize(stmt);
}

void InserIntoPlayerList(const char* title)
{
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO playlist (title) VALUES (?)";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, title, -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
}

void LoadPlayList_Combobox(HWND hComboBox)
{
    sqlite3_stmt* stmt;

    const char* sql = "SELECT * FROM playlist;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const char* title = (const char*)sqlite3_column_text(stmt, 1);

        wchar_t Wtitle[256];
        MultiByteToWideChar(CP_UTF8, 0, title, -1, Wtitle, 256);

        int index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)Wtitle);

        SendMessage(hComboBox, CB_SETITEMDATA, index, id);

        SendMessageW(hComboBox, CB_SETCURSEL, 0, 0);
    }

    sqlite3_finalize(stmt);
}


