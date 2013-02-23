/*
    This file is part of Kompex SQLite Wrapper.
	Copyright (c) 2008-2013 Sven Broeske

    Kompex SQLite Wrapper is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Kompex SQLite Wrapper is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Kompex SQLite Wrapper. If not, see <http://www.gnu.org/licenses/>.
*/

#include <fstream>
#include <iostream>
#include <exception>
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteException.h"

namespace Kompex
{

SQLiteDatabase::SQLiteDatabase():
	mDatabaseHandle(0),
	mIsMemoryDatabaseActive(false),
	mDatabaseFilenameUtf8(""),
	mDatabaseFilenameUtf16(L"")
{
}

SQLiteDatabase::SQLiteDatabase(const char *filename, int flags, const char *zVfs):
	mDatabaseHandle(0),
	mIsMemoryDatabaseActive(false)
{
	Open(filename, flags, zVfs);
}

SQLiteDatabase::SQLiteDatabase(const wchar_t *filename):
	mDatabaseHandle(0),
	mIsMemoryDatabaseActive(false)
{
	Open(filename);
}

SQLiteDatabase::SQLiteDatabase(const std::string &filename, int flags, const char *zVfs):
	mDatabaseHandle(0),
	mIsMemoryDatabaseActive(false)
{
	Open(filename, flags, zVfs);
}

SQLiteDatabase::~SQLiteDatabase()
{
	Close();
}

void SQLiteDatabase::Open(const char *filename, int flags, const char *zVfs)
{
	// close old db, if one exist
	if(mDatabaseHandle)
		Close();

	if(sqlite3_open_v2(filename, &mDatabaseHandle, flags, zVfs) != SQLITE_OK)
		KOMPEX_EXCEPT(sqlite3_errmsg(mDatabaseHandle));

	mDatabaseFilenameUtf8 = std::string(filename);
	mDatabaseFilenameUtf16 = L"";
}

void SQLiteDatabase::Open(const std::string &filename, int flags, const char *zVfs)
{
	// close old db, if one exist
	if(mDatabaseHandle)
		Close();

	if(sqlite3_open_v2(filename.c_str(), &mDatabaseHandle, flags, zVfs) != SQLITE_OK)
		KOMPEX_EXCEPT(sqlite3_errmsg(mDatabaseHandle));

	mDatabaseFilenameUtf8 = std::string(filename);
	mDatabaseFilenameUtf16 = L"";
}

void SQLiteDatabase::Open(const wchar_t *filename)
{
	// close old db, if one exist
	if(mDatabaseHandle)
		Close();

	// standard usage: SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
	if(sqlite3_open16(filename, &mDatabaseHandle) != SQLITE_OK)
		KOMPEX_EXCEPT(sqlite3_errmsg(mDatabaseHandle));

	mDatabaseFilenameUtf8 = "";
	mDatabaseFilenameUtf16 = filename;
}

void SQLiteDatabase::Close()
{	
	// detach database if the database was moved into memory
	if(mIsMemoryDatabaseActive)
	{
		if(sqlite3_exec(mDatabaseHandle, "DETACH DATABASE origin", 0, 0, 0) != SQLITE_OK)
			KOMPEX_EXCEPT(sqlite3_errmsg(mDatabaseHandle));
	}

	// close the database
	if(mDatabaseHandle && sqlite3_close(mDatabaseHandle) != SQLITE_OK)
	{
		KOMPEX_EXCEPT(sqlite3_errmsg(mDatabaseHandle));
	}
	else
	{
		mDatabaseHandle = 0;
		mDatabaseFilenameUtf8 = "";
		mDatabaseFilenameUtf16 = L"";
		mIsMemoryDatabaseActive = false;
	}
}

void SQLiteDatabase::TraceOutput(void *ptr, const char *sql)
{
	std::cout << "trace: " << sql << std::endl;
}

void SQLiteDatabase::ProfileOutput(void* ptr, const char* sql, sqlite3_uint64 time)
{
	std::cout << "profile: " << sql << std::endl;
	std::cout << "profile time: " << time << std::endl;
}

void SQLiteDatabase::MoveDatabaseToMemory(UtfEncoding encoding)
{
	if(!mIsMemoryDatabaseActive)
	{
		if(mDatabaseFilenameUtf8 == "" && mDatabaseFilenameUtf16 == L"")
		{
			KOMPEX_EXCEPT("No opened database! Please open a database first.");
			return;
		}

		sqlite3 *memoryDatabase;
		if(mDatabaseFilenameUtf8 != "")
			sqlite3_open(":memory:", &memoryDatabase);   
		else
			sqlite3_open16(L":memory:", &memoryDatabase);
		
		// create the in-memory schema from the origin database
		sqlite3_exec(mDatabaseHandle, "BEGIN", 0, 0, 0);

		if(encoding == UTF8)
		{
			sqlite3_exec(mDatabaseHandle, "SELECT sql FROM sqlite_master WHERE sql NOT NULL AND tbl_name != 'sqlite_sequence'", &Kompex::SQLiteDatabase::ProcessDDLRow, memoryDatabase, 0);
		}
		else if(encoding == UTF16)
		{
			struct sqlite3_stmt *statement;
			if(sqlite3_prepare_v2(mDatabaseHandle, "SELECT sql FROM sqlite_master WHERE sql NOT NULL AND tbl_name != 'sqlite_sequence'", -1, &statement, 0) != SQLITE_OK)
				CleanUpFailedMemoryDatabase(memoryDatabase, mDatabaseHandle, false, true, statement, sqlite3_errmsg(mDatabaseHandle));

			bool resultsAvailable = true;
			while(resultsAvailable)
			{
				switch(sqlite3_step(statement))
				{
					case SQLITE_ROW:
					{
						resultsAvailable = true;
						struct sqlite3_stmt *transferStatement;
						if(sqlite3_prepare16_v2(memoryDatabase, sqlite3_column_text16(statement, 0), -1, &transferStatement, 0) != SQLITE_OK)
						{
							sqlite3_finalize(transferStatement);
							CleanUpFailedMemoryDatabase(memoryDatabase, mDatabaseHandle, false, true, statement, sqlite3_errmsg(memoryDatabase));
						}
						
						// SQLITE_DONE should always be returned
						if(sqlite3_step(transferStatement) != SQLITE_DONE)
						{
							sqlite3_finalize(transferStatement);
							CleanUpFailedMemoryDatabase(memoryDatabase, mDatabaseHandle, false, true, statement, sqlite3_errmsg(memoryDatabase));
						}

						sqlite3_finalize(transferStatement);
						break;
					}
					case SQLITE_DONE:
						resultsAvailable = false;
						break;
					case SQLITE_BUSY:
						CleanUpFailedMemoryDatabase(memoryDatabase, mDatabaseHandle, false, true, statement, "SQLITE_BUSY");
						resultsAvailable = false;
						break;
					case SQLITE_ERROR:
						CleanUpFailedMemoryDatabase(memoryDatabase, mDatabaseHandle, false, true, statement, sqlite3_errmsg(mDatabaseHandle));
						resultsAvailable = false;
						break;
					default:
						CleanUpFailedMemoryDatabase(memoryDatabase, mDatabaseHandle, false, true, statement, sqlite3_errmsg(mDatabaseHandle));
						resultsAvailable = false;
				}
			}

			sqlite3_finalize(statement);
		}

		sqlite3_exec(mDatabaseHandle, "COMMIT", 0, 0, 0);

		// attach the origin database to the in-memory
		if(mDatabaseFilenameUtf8 != "")
		{
			std::string sql = "ATTACH DATABASE '" + mDatabaseFilenameUtf8 + "' as origin";
			if(sqlite3_exec(memoryDatabase, sql.c_str(), 0, 0, 0) != SQLITE_OK)
			{
				sqlite3_close(memoryDatabase);
				KOMPEX_EXCEPT(sqlite3_errmsg(memoryDatabase));
			}
		}
		else
		{
			struct sqlite3_stmt *statement;
			std::wstring sql = L"ATTACH DATABASE '" + mDatabaseFilenameUtf16 + L"' as origin";
			if(sqlite3_prepare16_v2(memoryDatabase, sql.c_str(), -1, &statement, 0) != SQLITE_OK)
				CleanUpFailedMemoryDatabase(memoryDatabase, memoryDatabase, false, false, statement, sqlite3_errmsg(memoryDatabase));

			if(sqlite3_step(statement) != SQLITE_DONE)
				CleanUpFailedMemoryDatabase(memoryDatabase, memoryDatabase, false, false, statement, sqlite3_errmsg(memoryDatabase));

			sqlite3_finalize(statement);
		}

		// copy the data from the origin database to the in-memory
		sqlite3_exec(memoryDatabase, "BEGIN", 0, 0, 0);

		if(encoding == UTF8)
		{
			sqlite3_exec(memoryDatabase, "SELECT name FROM origin.sqlite_master WHERE type='table'", &Kompex::SQLiteDatabase::ProcessDMLRow, memoryDatabase, 0);
		}
		else if(encoding == UTF16)
		{
			struct sqlite3_stmt *statement;
			if(sqlite3_prepare_v2(memoryDatabase, "SELECT name FROM origin.sqlite_master WHERE type='table'", -1, &statement, 0) != SQLITE_OK)
				CleanUpFailedMemoryDatabase(memoryDatabase, memoryDatabase, true, false, statement, sqlite3_errmsg(memoryDatabase));

			bool resultsAvailable = true;
			while(resultsAvailable)
			{
				switch(sqlite3_step(statement))
				{
					case SQLITE_ROW:
					{
						resultsAvailable = true;
						
						std::wstring tableName = (wchar_t*)sqlite3_column_text16(statement, 0);
						std::wstring stmt = std::wstring(L"INSERT INTO main.") + tableName + std::wstring(L" SELECT * FROM origin.") + tableName;
						struct sqlite3_stmt *transferStatement;
						if(sqlite3_prepare16_v2(memoryDatabase, stmt.c_str(), -1, &transferStatement, 0) != SQLITE_OK)
						{
							sqlite3_finalize(transferStatement);
							CleanUpFailedMemoryDatabase(memoryDatabase, memoryDatabase, true, true, statement, sqlite3_errmsg(memoryDatabase));
						}

						// SQLITE_DONE should always be returned
						if(sqlite3_step(transferStatement) != SQLITE_DONE)
						{
							sqlite3_finalize(transferStatement);
							CleanUpFailedMemoryDatabase(memoryDatabase, memoryDatabase, true, true, statement, sqlite3_errmsg(memoryDatabase));
						}

						sqlite3_finalize(transferStatement);

						break;
					}
					case SQLITE_DONE:
						resultsAvailable = false;
						break;
					case SQLITE_BUSY:
						CleanUpFailedMemoryDatabase(memoryDatabase, memoryDatabase, true, true, statement, "SQLITE_BUSY");
						resultsAvailable = false;
						break;
					case SQLITE_ERROR:
						CleanUpFailedMemoryDatabase(memoryDatabase, memoryDatabase, true, true, statement, sqlite3_errmsg(memoryDatabase));
						resultsAvailable = false;
						break;
					default:
						CleanUpFailedMemoryDatabase(memoryDatabase, memoryDatabase, true, true, statement, sqlite3_errmsg(memoryDatabase));
						resultsAvailable = false;
				}
			}
			
			sqlite3_finalize(statement);
		}

		if(sqlite3_exec(memoryDatabase, "COMMIT", 0, 0, 0) == SQLITE_OK)
		{
			sqlite3_close(mDatabaseHandle);
			mDatabaseHandle = memoryDatabase;
			mIsMemoryDatabaseActive = true;
		}
		else
		{
			CleanUpFailedMemoryDatabase(memoryDatabase, memoryDatabase, true, true, 0, sqlite3_errmsg(memoryDatabase));
		}
	}
}

void SQLiteDatabase::CleanUpFailedMemoryDatabase(sqlite3 *memoryDatabase, sqlite3 *rollbackDatabase, bool isDetachNecessary, bool isRollbackNecessary, sqlite3_stmt *stmt, const std::string &errMsg)
{
	if(stmt != 0)
		sqlite3_finalize(stmt);

	if(isRollbackNecessary)
		sqlite3_exec(rollbackDatabase, "ROLLBACK", 0, 0, 0);

	if(isDetachNecessary)
		sqlite3_exec(memoryDatabase, "DETACH DATABASE origin", 0, 0, 0);

	sqlite3_close(memoryDatabase);
	KOMPEX_EXCEPT(errMsg);
}

int SQLiteDatabase::ProcessDDLRow(void *db, int columnsCount, char **values, char **columns)
{
	if(columnsCount != 1)
	{
		KOMPEX_EXCEPT("error occured during DDL: columnsCount != 1");
		return -1;
	}

	// execute a sql statement in values[0] in the database db.
	if(sqlite3_exec(static_cast<sqlite3*>(db), values[0], 0, 0, 0) != SQLITE_OK)
		KOMPEX_EXCEPT("error occured during DDL: sqlite3_exec (error message: " + std::string(sqlite3_errmsg(static_cast<sqlite3*>(db))) + ")");

	return 0;
}

int SQLiteDatabase::ProcessDMLRow(void *db, int columnsCount, char **values, char **columns)
{
	if(columnsCount != 1)
	{
		KOMPEX_EXCEPT("error occured during DML: columnsCount != 1");
		return -1;
	}
	
	char *stmt = sqlite3_mprintf("INSERT INTO main.%q SELECT * FROM origin.%q", values[0], values[0]);

	if(sqlite3_exec(static_cast<sqlite3*>(db), stmt, 0, 0, 0) != SQLITE_OK)
		KOMPEX_EXCEPT("error occured during DDL: sqlite3_exec (error message: " + std::string(sqlite3_errmsg(static_cast<sqlite3*>(db))) + ")");

	sqlite3_free(stmt);    

	return 0;
}

void SQLiteDatabase::SaveDatabaseFromMemoryToFile(const std::string &filename)
{
	if(mIsMemoryDatabaseActive)
	{
		sqlite3 *fileDatabase;
		if(filename == "")
		{
			if(mDatabaseFilenameUtf8 != "")
			{
				if(sqlite3_open_v2(mDatabaseFilenameUtf8.c_str(), &fileDatabase, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0) != SQLITE_OK)
					KOMPEX_EXCEPT(sqlite3_errmsg(fileDatabase));
			}
			else
			{
				if(sqlite3_open16(mDatabaseFilenameUtf16.c_str(), &fileDatabase) != SQLITE_OK)
					KOMPEX_EXCEPT(sqlite3_errmsg(fileDatabase));
			}
		}
		else
		{
			if(sqlite3_open_v2(filename.c_str(), &fileDatabase, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0) != SQLITE_OK)
				KOMPEX_EXCEPT(sqlite3_errmsg(fileDatabase));
		}
		
		TakeSnapshot(fileDatabase);
	}
}

void SQLiteDatabase::SaveDatabaseFromMemoryToFile(const wchar_t *filename)
{
	if(mIsMemoryDatabaseActive)
	{
		sqlite3 *fileDatabase;
		if(sqlite3_open16(filename, &fileDatabase) != SQLITE_OK)
			KOMPEX_EXCEPT(sqlite3_errmsg(fileDatabase));
		
		TakeSnapshot(fileDatabase);
	}
}

void SQLiteDatabase::TakeSnapshot(sqlite3 *destinationDatabase)
{
	sqlite3_backup *backup;
	backup = sqlite3_backup_init(destinationDatabase, "main", mDatabaseHandle, "main");
	if(backup)
	{
		// -1 to copy the entire source database to the destination
		if(sqlite3_backup_step(backup, -1) != SQLITE_DONE)
			KOMPEX_EXCEPT(sqlite3_errmsg(destinationDatabase));
		// clean up resources allocated by sqlite3_backup_init()
		if(sqlite3_backup_finish(backup) != SQLITE_OK)
			KOMPEX_EXCEPT(sqlite3_errmsg(destinationDatabase));
	}

	if(sqlite3_close(destinationDatabase) != SQLITE_OK)
		KOMPEX_EXCEPT(sqlite3_errmsg(destinationDatabase));
}

bool SQLiteDatabase::IsDatabaseReadOnly()
{
	int result = sqlite3_db_readonly(mDatabaseHandle, "main");
	if(result == -1)
		KOMPEX_EXCEPT("'main' is not the name of a database on connection mDatabaseHandle");

	return !!result;
}

void SQLiteDatabase::CreateModule(const std::string &moduleName, const sqlite3_module *module, void *clientData, void(*xDestroy)(void*))
{
	if(sqlite3_create_module_v2(mDatabaseHandle, moduleName.c_str(), module, clientData, xDestroy))
		KOMPEX_EXCEPT(sqlite3_errmsg(mDatabaseHandle));
}

}	// namespace Kompex
