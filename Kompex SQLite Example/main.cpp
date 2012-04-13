/*
    This file is part of Kompex SQLite Wrapper.
	Copyright (c) 2008-2012 Sven Broeske

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

#include "KompexSQLitePrerequisites.h"
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"
#include "KompexSQLiteStreamRedirection.h"
#include "KompexSQLiteBlob.h"

#include <iostream>
#include <exception>

void FunctionWithLocalVariable(Kompex::SQLiteStatement *stmt);

int main()
{
	// uncomment to redirect streams to a file
	//Kompex::CerrRedirection cerrRedirection("error.log");
	//Kompex::CoutRedirection coutRedirection("output.log");

	try
	{
		// create and open database
		Kompex::SQLiteDatabase *pDatabase = new Kompex::SQLiteDatabase("test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
		// create statement instance for sql queries/statements
		Kompex::SQLiteStatement *pStmt = new Kompex::SQLiteStatement(pDatabase);

		std::cout << "SQLite version: " << pDatabase->GetLibVersionNumber() << std::endl;
		
		// ---------------------------------------------------------------------------------------------------------
		// create table and insert some data
		pStmt->SqlStatement("CREATE TABLE user (userID INTEGER NOT NULL PRIMARY KEY, lastName VARCHAR(50) NOT NULL, firstName VARCHAR(50), age INTEGER, weight DOUBLE)");
		pStmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (1, 'Lehmann', 'Jamie', 20, 65.5)");
		pStmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (2, 'Burgdorf', 'Peter', 55, NULL)");
		pStmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (3, 'Lehmann', 'Fernando', 18, 70.2)");
		pStmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (4, 'Lehmann', 'Carlene ', 17, 50.8)");

		// ---------------------------------------------------------------------------------------------------------
		// insert some data with Bind..() methods
		pStmt->Sql("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES(?, ?, ?, ?, ?);");
		pStmt->BindInt(1, 5);
		pStmt->BindString(2, "Murahama");
		pStmt->BindString(3, "Yura");
		pStmt->BindInt(4, 28);
		pStmt->BindDouble(5, 60.2);
		// executes the INSERT statement and cleans-up automatically
		pStmt->ExecuteAndFree();

		// ---------------------------------------------------------------------------------------------------------
		// lets have a look on a query which is shown in console
		std::cout << std::endl;
		pStmt->GetTable("SELECT firstName, userID, age, weight FROM user WHERE lastName = 'Lehmann';", 13);
		std::cout << std::endl;

		// some example SQLite aggregate functions
		std::cout << "COUNT(*): " << pStmt->SqlAggregateFuncResult("SELECT COUNT(*) FROM user WHERE lastName = 'Lehmann';") << std::endl;
		std::cout << "COUNT(weight): " << pStmt->SqlAggregateFuncResult("SELECT COUNT(weight) FROM user;") << std::endl;
		std::cout << "MAX(age): " << pStmt->SqlAggregateFuncResult("SELECT MAX(age) FROM user;") << std::endl;
		std::cout << "MIN(age): " << pStmt->SqlAggregateFuncResult("SELECT MIN(age) FROM user;") << std::endl;
		std::cout << "AVG(age): " << pStmt->SqlAggregateFuncResult("SELECT AVG(age) FROM user;") << std::endl;
		std::cout << "SUM(age): " << pStmt->SqlAggregateFuncResult("SELECT SUM(age) FROM user;") << std::endl;
		std::cout << "TOTAL(age): " << pStmt->SqlAggregateFuncResult("SELECT TOTAL(age) FROM user;") << std::endl;

		// ---------------------------------------------------------------------------------------------------------

		// sql query - searching for all people with lastName "Lehmann"
		pStmt->Sql("SELECT firstName FROM user WHERE lastName = 'Lehmann';");

		// after a Sql() call we can get some special information
		std::cout << "\nGetColumnName: " << pStmt->GetColumnName(0) << std::endl;
		std::cout << "GetColumnCount: " << pStmt->GetColumnCount() << std::endl;
		std::cout << "GetColumnDatabaseName: " << pStmt->GetColumnDatabaseName(0) << std::endl;
		std::cout << "GetColumnTableName: " << pStmt->GetColumnTableName(0) << std::endl;
		std::cout << "GetColumnOriginName: " << pStmt->GetColumnOriginName(0) << "\n" << std::endl;

		// do not forget to clean-up
		pStmt->FreeQuery();
		
		// ---------------------------------------------------------------------------------------------------------
		// another sql query
		pStmt->Sql("SELECT * FROM user WHERE firstName = 'Jamie';");

		// after a Sql() call we can get some other special information
		std::cout << "GetColumnName(0): " << pStmt->GetColumnName(0) << std::endl;
		std::cout << "GetColumnName(1): " << pStmt->GetColumnName(1) << std::endl;
		std::cout << "GetColumnName(2): " << pStmt->GetColumnName(2) << std::endl;
		std::cout << "GetColumnName(3): " << pStmt->GetColumnName(3) << std::endl;
		std::cout << "GetColumnName(4): " << pStmt->GetColumnName(4) << std::endl;
		std::cout << "GetColumnCount: " << pStmt->GetColumnCount() << std::endl;	
		std::cout << "GetColumnDeclaredDatatype(0): " << pStmt->GetColumnDeclaredDatatype(0) << std::endl;
		std::cout << "GetColumnDeclaredDatatype(1): " << pStmt->GetColumnDeclaredDatatype(1) << std::endl;
		std::cout << "GetColumnDeclaredDatatype(2): " << pStmt->GetColumnDeclaredDatatype(2) << std::endl;
		std::cout << "GetColumnDeclaredDatatype(3): " << pStmt->GetColumnDeclaredDatatype(3) << std::endl;
		std::cout << "GetColumnDeclaredDatatype(4): " << pStmt->GetColumnDeclaredDatatype(4) << std::endl;

		// process all results
		while(pStmt->FetchRow())
		{
			std::cout << "\nGetDataCount: " << pStmt->GetDataCount() << std::endl;
			std::cout << "SQL query - GetColumnDouble(0): " << pStmt->GetColumnDouble(0) << std::endl;
			std::cout << "SQL query - GetColumnString(1): " << pStmt->GetColumnString(1) << std::endl;
			std::cout << "SQL query - GetColumnString(2): " << pStmt->GetColumnString(2) << std::endl;
			std::cout << "SQL query - GetColumnString(3): " << pStmt->GetColumnString(3) << std::endl;
			std::cout << "SQL query - GetColumnString(4): " << pStmt->GetColumnString(4) << std::endl;
			std::cout << "\nColumnTypes (look at the documentation for the meaning of the numbers):\n";
			std::cout << "GetColumnType(0): " << pStmt->GetColumnType(0) << std::endl;
			std::cout << "GetColumnType(1): " << pStmt->GetColumnType(1) << std::endl;
			std::cout << "GetColumnType(2): " << pStmt->GetColumnType(2) << std::endl;
			std::cout << "GetColumnType(3): " << pStmt->GetColumnType(3) << std::endl;
			std::cout << "GetColumnType(4): " << pStmt->GetColumnType(4) << std::endl;
		}

		// do not forget to clean-up
		pStmt->FreeQuery();

		// ---------------------------------------------------------------------------------------------------------
		// a little example how to get some queried data via column name
		std::cout << "\nGet some queried data via column name:\n";
		pStmt->Sql("SELECT * FROM user WHERE lastName = 'Lehmann';");

		// process all results
		while(pStmt->FetchRow())
		{
			std::cout << "firstName: " << pStmt->GetColumnString("firstName") << std::endl;
			std::cout << "age: " << pStmt->GetColumnInt("age") << std::endl;
		}

		// do not forget to clean-up
		pStmt->FreeQuery();

		// ---------------------------------------------------------------------------------------------------------
		// example for prepared statements - repetitive execution (SELECT)
		std::cout << "\nPrepared statement - repetitive execution (SELECT):\n";
		pStmt->Sql("SELECT * FROM user WHERE userID=@id");
			
		for(int i = 1; i <= 3; ++i)
		{
			// bind an integer to the prepared statement
			pStmt->BindInt(1, i);

			// and now fetch all results
			while(pStmt->FetchRow())
				std::cout << pStmt->GetColumnCString(0) << " " << pStmt->GetColumnCString(1) << std::endl;

			// reset the prepared statement
			pStmt->Reset();
		}
		// do not forget to clean-up
		pStmt->FreeQuery();

		// ---------------------------------------------------------------------------------------------------------
		// example for prepared statements - repetitive execution (INSERT/UPDATE/DELETE)
		std::cout << "\nPrepared statement - repetitive execution (INSERT/UPDATE/DELETE):\n";
		std::cout << "no output here - there are only some INSERTs ;)\n";

		// create a table structure
		pStmt->SqlStatement("CREATE TABLE flower (flowerID INTEGER NOT NULL PRIMARY KEY, name VARCHAR(50) NOT NULL, size DOUBLE)");
		// create the prepared statement
		pStmt->Sql("INSERT INTO flower (flowerID, name, size) VALUES (@flowerID, @name, @size)");
			
		// bind all three values
		pStmt->BindInt(1, 1);					// flowerID
		pStmt->BindString(2, "rose");			// name
		pStmt->BindDouble(3, 50.5);				// size
		// execute the statement and reset the bindings
		pStmt->Execute();
		pStmt->Reset();

		// here we go - the second round
		pStmt->BindInt(1, 2);					// flowerID
		pStmt->BindString(2, "primrose");		// name
		pStmt->BindDouble(3, 6.21);				// size
		// execute the statement and reset the bindings
		pStmt->Execute();
		pStmt->Reset();

		// here we go - the second round
		pStmt->BindInt(1, 3);					// flowerID
		pStmt->BindString(2, "rhododendron");	// name
		pStmt->BindDouble(3, 109.84);			// size
		// execute the statement
		pStmt->Execute();
		// we don't need the prepared statement anymore so we clean-up everything
		pStmt->FreeQuery();
		
		// ---------------------------------------------------------------------------------------------------------
		// two possibilities to update data in the database

		// the first way
		std::cout << "\nUPDATE possibility 1: prepared statement - single execution";
		pStmt->Sql("UPDATE user SET lastName=@lastName, age=@age WHERE userID=@userID");
			
		// bind an integer to the prepared statement
        pStmt->BindString(1, "Urushihara");		// bind lastName
		pStmt->BindInt(2, 56);					// bind age
		pStmt->BindInt(3, 2);					// bind userID

		// execute it and clean-up
        pStmt->ExecuteAndFree();

		// the second way
		std::cout << "\nUPDATE possibility 2: common statement\n";
		pStmt->SqlStatement("UPDATE user SET weight=51.5, age=18 WHERE firstName='Carlene'");

		// ---------------------------------------------------------------------------------------------------------
		// get some instant results
		std::cout << "\nSELECT lastName FROM user WHERE userID = 3;\n" << pStmt->GetSqlResultString("SELECT lastName FROM user WHERE userID = 3");
		std::cout << "\nSELECT age FROM user WHERE userID = 4;\n" <<  pStmt->GetSqlResultInt("SELECT age FROM user WHERE userID = 4");
		std::cout << "\nSELECT weight FROM user WHERE userID = 3;\n" <<  pStmt->GetSqlResultDouble("SELECT weight FROM user WHERE userID = 3");

		// don't forget to delete the pointer for all GetSqlResult%() methods which return a pointer
		const unsigned char *lastName = pStmt->GetSqlResultCString("SELECT lastName FROM user WHERE userID = 2");
		// do something with lastName
		delete[] lastName;

		// ---------------------------------------------------------------------------------------------------------
		// DELETE statement and get afterwards the number of affected rows
		pStmt->SqlStatement("DELETE FROM user WHERE lastName = 'Lehmann'");
		std::cout << "\n\nGetDatabaseChanges: " << pDatabase->GetDatabaseChanges() << std::endl;

		// let's see, how many changes we have done
		std::cout << "GetTotalDatabaseChanges: " << pDatabase->GetTotalDatabaseChanges() << std::endl;
		std::cout << std::endl;

		// ---------------------------------------------------------------------------------------------------------
		// get all metadata from one column
		pStmt->GetTableColumnMetadata("user", "userID");
		std::cout << std::endl;

		// ---------------------------------------------------------------------------------------------------------
		// now we want try a transaction
		// if an error occurs, a rollback is automatically performed
		// note: you must use Transaction()
		pStmt->BeginTransaction();
		pStmt->Transaction("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (10, 'Kanzaki', 'Makoto', 23, 76.9)");
		FunctionWithLocalVariable(pStmt);
		pStmt->Transaction("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (12, 'Kanzaki', 'Peter', 63, 101.1)");
		pStmt->CommitTransaction();

		// if you want react on errors by yourself, you can use a own try() and catch() block
		// note: you must use SqlStatement()
		try
		{
			pStmt->BeginTransaction();
			pStmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (10, 'Kanzaki', 'Makoto', 23, 76.9)");
			pStmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (11, 'Kanzaki', 'Yura', 20, 56.9)");
			pStmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (12, 'Kanzaki', 'Peter', 63, 101.1)");
			pStmt->SqlStatement("INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (10, 'Henschel', 'Robert', 10, 34.5)");
			pStmt->CommitTransaction();
		}
		catch(Kompex::SQLiteException &exception) 
		{
			std::cerr << "Exception Occured: " << exception.GetString();
			pStmt->RollbackTransaction();
			std::cerr << "Rollback was executed!" << std::endl;
			std::cerr << "This is our own catch() block!" << std::endl;
		}

		// ---------------------------------------------------------------------------------------------------------
		// Kompex::SQLiteBlob example
		std::cout << "\nKompex::SQLiteBlob example\n";

		// create a little table and fill it with some dummy data 
		// the content of the BLOBs is simple plaintext so that you can see what happens
		pStmt->SqlStatement("CREATE TABLE boarduser (userID INTEGER NOT NULL PRIMARY KEY, username VARCHAR(20), picture BLOB)");
		pStmt->SqlStatement("INSERT INTO boarduser (userID, username, picture) VALUES (1, 'apori', 'abcdefghijklmnopqrstuvwxyz')");
		pStmt->SqlStatement("INSERT INTO boarduser (userID, username, picture) VALUES (2, 'sarina', 'abcdefghijklmnopqrstuvwxyz')");

		// open the existing BLOB for user 'apori'
		Kompex::SQLiteBlob *pKompexBlob = new Kompex::SQLiteBlob(pDatabase, "main", "boarduser", "picture", 1);
		// get the size of the current BLOB
		std::cout << "GetBlobSize(): " << pKompexBlob->GetBlobSize() << std::endl;

		// read the whole BLOB value
		int blobSize = pKompexBlob->GetBlobSize();
		char *readBuffer = new char[blobSize + 1];
		readBuffer[blobSize] = '\0';
		pKompexBlob->ReadBlob(readBuffer, blobSize);
		std::cout << "ReadBlob() output: " << readBuffer << std::endl;
		
		// overwrite a part of the BLOB
		std::cout << "WriteBlob() - change the BLOB data\n";
		char newData[8] = "-HELLO-";
		pKompexBlob->WriteBlob(newData, sizeof(newData) - 1, 10);

		// and read the whole BLOB value again
		pKompexBlob->ReadBlob(readBuffer, blobSize);
		std::cout << "ReadBlob() output: " << readBuffer << std::endl;
		delete readBuffer;

		delete pKompexBlob;

		// ---------------------------------------------------------------------------------------------------------
		// create a sql statement with SQLite functions
		const char *param1 = "It's a happy day.";
		const char *param2 = "Muhahaha!";
		std::cout << "\n" << pStmt->Mprintf("INSERT INTO table VALUES('%q', '%q')", param1, param2) << std::endl;

		// ---------------------------------------------------------------------------------------------------------
		// clean-up
		delete pStmt;
		delete pDatabase;
	}
	catch(Kompex::SQLiteException &exception)
	{
		std::cerr << "\nException Occured" << std::endl;
		exception.Show();
	}

	/*
	complete example for the usage of file and memory databases
	(database layout is only fictitious)

	Kompex::SQLiteDatabase *pDatabase = new Kompex::SQLiteDatabase("scores.db", SQLITE_OPEN_READWRITE, 0);
	// move database to memory, so that we are work on the memory database hence
	pDatabase->MoveDatabaseToMemory();

	Kompex::SQLiteStatement *pStmt = new Kompex::SQLiteStatement(pDatabase);
	// insert some data sets into the memory database
	pStmt->SqlStatement("INSERT INTO score(id, lastScore, avgScore) VALUES(1, 429, 341)");
	pStmt->SqlStatement("INSERT INTO score(id, lastScore, avgScore) VALUES(2, 37, 44)");
	pStmt->SqlStatement("INSERT INTO score(id, lastScore, avgScore) VALUES(3, 310, 280)");

	// save the memory database to a file
	// if you don't do it, all database changes will be lost after closing the memory database
	pDatabase->SaveDatabaseFromMemoryToFile("newScores.db");

	delete pStmt;
	delete pDatabase;
	*/

	std::cin.get();
	return 0;
}

void FunctionWithLocalVariable(Kompex::SQLiteStatement *stmt)
{
	const wchar_t *localVariable = L"INSERT INTO user (userID, lastName, firstName, age, weight) VALUES (11, 'Kanzaki', 'Yura', 20, 56.9)";
	// use SecureTransaction() which creates a internal copy of localVariable
	stmt->SecureTransaction(localVariable);
	// localVariable will be deleted on this end of scope now!
}
