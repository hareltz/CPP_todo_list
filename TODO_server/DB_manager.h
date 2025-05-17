#pragma once
#include <sqlite3.h>
#include <iostream>
#include <string>
using namespace std;

enum codes
{
	SUCCESS,
	ERROR,
	LIST_NOT_FOUND,
	ALREADY_LIST_EXISTS, // -> for user
	DATABASE_ERROR, 
	USER_EXISTS,
	USER_NOT_EXISTS,
	PASSWORD_NOT_MATCH
};

class DB_manager
{

public:

	DB_manager() = default;
	DB_manager(string DbAddr);

	codes createNewUser(string email, string password);
	codes deleteUser(string email, string password);
	codes createNewTodoList(string email, string listName);
	codes addTask(string email, string listName, string task);
	codes markTask(string email, string listName, string task);
	codes checkUserPassword(string email, string password);
	


private:

	sqlite3* _DB;
	bool createDbFiles(string DbAddr);
	bool createTables();
	int getUserIdByEmail(string email);
	int getListIdByName(string email);

};