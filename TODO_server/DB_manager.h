#pragma once
#include <sqlite3.h>
#include <iostream>
using namespace std;

enum codes
{
	SUCCESS = 0,
	ERROR = 1,
	LIST_NOT_FOUND = 2,
	ALREADY_LIST_NAMED = 3,
	DATABASE_ERROR = 4
};

class DB_manager
{

public:

	DB_manager() = default;
	DB_manager(string DbAddr);

	codes createNewUser(string email, string password);
	codes deleteUser(string email, string password);
	codes createNewTodoList(string listName);
	codes addTask(string listName, string task);
	codes deleteTask(string listName, string task);
	codes markTask(string listName, string task);


private:

	sqlite3* _DB;
	bool createDbFiles(string DbAddr);
	bool createTables();
	
};