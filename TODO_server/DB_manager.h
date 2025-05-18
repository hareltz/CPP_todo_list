#pragma once
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum codes
{
	SUCCESS,
	ERROR,
	LIST_NOT_EXISTS,
	LIST_ALREADY_EXISTS, // -> for user
	TASK_ALREADY_EXISTS, // -> for user
	DATABASE_ERROR, 
	USER_EXISTS,
	USER_NOT_EXISTS,
	PASSWORD_NOT_MATCH
};

/// <summary>
/// this class will manage the DB
/// this class is also Singleton class
/// </summary>
/// <param name="DbAddr">the DB</param>
class DB_manager
{

public:

	static DB_manager& get_instance()
	{
		// the first call of the function will create the only DB_manager instance in the program
		// the other calls will just return the same instance
		static DB_manager instance;
		return instance;
	};

	codes createNewUser(string email, string password);
	codes deleteUser(string email, string password);
	codes addList(string email, string listName);
	codes addTask(string email, string listName, string task);
	codes markTask(string email, string listName, string task);
	codes unmarkTask(string email, string listName, string task);
	codes checkUserPassword(string email, string password);
	vector<string> getLists(string email);
	vector<string> getTasks(string email, string listName);
	
	

private:

	sqlite3* _DB;

	DB_manager();

	// delete all the default c'tors
	DB_manager(const DB_manager&) = delete;
	DB_manager(DB_manager&&) = delete;
	DB_manager& operator=(const DB_manager&) = delete;
	DB_manager& operator=(DB_manager&&) = delete;


	bool createDbFiles(string DbAddr);
	bool createTables();
	int getUserIdByEmail(string email);
	int getListIdByName(string email, string listName);

};