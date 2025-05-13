#include "DB_manager.h"

DB_manager::DB_manager(string DbAddr)
{
	
    if (createDbFiles(DbAddr)) { createTables(); }
	// creates the tables in the DB file

}

codes DB_manager::createNewUser(string email, string password)
{
	return codes();
}

codes DB_manager::deleteUser(string email, string password)
{
	return codes();
}

codes DB_manager::connectUser(string email, string password)
{
    return codes();
}

codes DB_manager::createNewTodoList(string listName)
{
	return codes();
}

codes DB_manager::addTask(int userId, string listName, string task)
{
	return codes();
}

codes DB_manager::markTask(int userId, string listName, string task)
{
	return codes();
}

/// <summary>
/// this function create / open the DB file
/// </summary>
/// <param name="DbAddr">the db address </param>
/// <returns></returns>
bool DB_manager::createDbFiles(string DbAddr)
{
	// open / create the DB file
	int exit = sqlite3_open(DbAddr.c_str(), &this->_DB);
	if (exit) {
		cerr << "Error open DB: " << sqlite3_errmsg(this->_DB) << endl;
		return false;
	}
	cout << "Opened database successfully!" << endl;
	return true;
}

/// <summary>
/// this function creats all the tables: users, lists, tasks.
/// </summary>
/// <returns>
/// true: success
/// false: Error
/// </returns>
bool DB_manager::createTables()
{
    char* errorMessage;

    // 1. Create `users` table
    const char* usersTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "email TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL"
        ");";

    if (sqlite3_exec(this->_DB, usersTable, NULL, 0, &errorMessage) != SQLITE_OK) {
        cerr << "Error creating users table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    else {
        cout << "Users table created successfully." << endl;
    }

    // 2. Create `lists` table
    const char* listsTable =
        "CREATE TABLE IF NOT EXISTS lists ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "user_id INTEGER NOT NULL, "
        "name TEXT NOT NULL, "
        "FOREIGN KEY(user_id) REFERENCES users(id)"
        ");";

    if (sqlite3_exec(this->_DB, listsTable, NULL, 0, &errorMessage) != SQLITE_OK) {
        cerr << "Error creating lists table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    else {
        cout << "Lists table created successfully." << endl;
    }

    // 3. Create `tasks` table
    const char* tasksTable =
        "CREATE TABLE IF NOT EXISTS tasks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "list_id INTEGER NOT NULL, "
        "task TEXT NOT NULL, "
        "done BOOLEAN NOT NULL DEFAULT 0, "
        "FOREIGN KEY(list_id) REFERENCES lists(id)"
        ");";

    if (sqlite3_exec(this->_DB, tasksTable, NULL, 0, &errorMessage) != SQLITE_OK) {
        cerr << "Error creating tasks table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return false;
    }
    else {
        cout << "Tasks table created successfully." << endl;
    }

    return true;
}

int DB_manager::getUserIdByEmail(string email)
{
    return 0;
}

int DB_manager::getTableIdByName(string email)
{
    return 0;
}
