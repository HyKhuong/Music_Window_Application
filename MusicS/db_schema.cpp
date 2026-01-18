#include "Database.h"

static const char* createTableSongs =
"CREATE TABLE IF NOT EXISTS songs ("
"id INTEGER PRIMARY KEY AUTOINCREMENT,"
"title TEXT,"
"path TEXT,"
"duration INTEGER);";

static const char* createTablePlayList =
"CREATE TABLE IF NOT EXISTS playlist ("
"id INTEGER PRIMARY KEY AUTOINCREMENT,"
"title TEXT NOT NULL UNIQUE,"
"total INTEGER);";

static const char* createTable_PlayerListSongs =
"CREATE TABLE IF NOT EXISTS playlist_songs ("
"id INTEGER PRIMARY KEY AUTOINCREMENT,"
"playlist_id INTEGER NOT NULL,"
"song_id INTEGER NOT NULL,"
"FOREIGN KEY (playlist_id) REFERENCES playerlist(id),"
"FOREIGN KEY (song_id) REFERENCES songs(id));";

void DB_Schema()
{
	Create_TableForDB(createTableSongs);
	Create_TableForDB(createTablePlayList);
	Create_TableForDB(createTable_PlayerListSongs);
}