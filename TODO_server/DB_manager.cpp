#include "DB_manager.h"

/// <summary>
/// c'tor
/// </summary>
/// <param name="DbAddr">the address/name for the DB file</param>
DB_manager::DB_manager(string DbAddr)
{
    if (createDbFiles(DbAddr)) { createTables(); }
	// creates the tables in the DB file

}

/// <summary>
/// this function adds a user to the db
/// it's also checks if the email is already exist in the DB
/// </summary>
/// <param name="email">the user's email</param>
/// <param name="password">the user's password</param>
/// <returns>one of the codes</returns>
codes DB_manager::createNewUser(string email, string password)
{
    char* errorMessage;
    string sqlMsg = "INSERT INTO users (email, password) VALUES ('" + email + "', '" + password + "');";
    int code = sqlite3_exec(this->_DB, sqlMsg.c_str(), NULL, 0, &errorMessage);

    if ( code != SQLITE_OK)
    {
        std::string errMsg = errorMessage ? errorMessage : "";

        if (code == SQLITE_CONSTRAINT && errMsg.find("users.email") != std::string::npos) {
            sqlite3_free(errorMessage);
            std::cerr << "SQLite error: " << errMsg << std::endl;
            return codes::EMAIL_EXISTS;
        }

        std::cerr << "SQLite error: " << errMsg << std::endl;
        sqlite3_free(errorMessage);
        return codes::ERROR;
    }

	return codes::SUCCESS;
}

/// <summary>
/// this function check's if the password is right
/// if it is, the function will delete the user from the DB
/// </summary>
/// <param name="email">the user's email</param>
/// <param name="password">the user's password</param>
/// <returns>SUCCESS/ERROR</returns>
codes DB_manager::deleteUser(string email, string password)
{
    if (checkUserPassword(email, password) == codes::ERROR) { return codes::PASSWORD_NOT_MATCH; } // Check if the passwords are the same.

    char* errorMessage;
    string sqlMsg = "DELETE FROM users WHERE email = '" + email + "';";

    if (sqlite3_exec(this->_DB, sqlMsg.c_str(), NULL, 0, &errorMessage) != SQLITE_OK)
    {
        std::cerr << "Error deleting user: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return codes::ERROR;
    }

    return codes::SUCCESS;
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
/// this function check the password of the user with a callback funtion inside sqlite3_exec
/// </summary>
/// <param name="email">the user's email</param>
/// <param name="password">the user's password</param>
/// <returns>SUCCESS/ERROR</returns>
codes DB_manager::checkUserPassword(string email, string password)
{
    char* errorMessage = nullptr;

    // SQL query to get the password for the provided email
    std::string sql = "SELECT password FROM users WHERE email = '" + email + "';";

    // Function to execute the SQL query and fetch the password
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        // If email exists, compare the password (argv[0] contains the password from DB)
        if (argc > 0 && argv[0]) {
            std::string storedPassword = argv[0];
            std::string providedPassword = *(static_cast<std::string*>(data));

            // Compare the passwords
            if (storedPassword == providedPassword) {
                return SQLITE_OK; // Passwords match
            }
            else {
                return SQLITE_ERROR; // Passwords do not match
            }
        }
        return SQLITE_ERROR; // Return success
        };

    // Execute the query and check the result using the callback
    if (sqlite3_exec(this->_DB, sql.c_str(), callback, &password, &errorMessage) != SQLITE_OK) {
        std::cerr << "The passwords does not match!" << std::endl;
        sqlite3_free(errorMessage);
        return codes::ERROR;
    }

    return codes::SUCCESS;
}

/// <summary>
/// this function create / open the DB file
/// and enable foreign keys
/// </summary>
/// <param name="DbAddr">the db address </param>
/// <returns>
/// true: success
/// false: Error
/// </returns>
bool DB_manager::createDbFiles(string DbAddr)
{
	// open / create the DB file
	int exit = sqlite3_open(DbAddr.c_str(), &this->_DB);
	if (exit) {
		cerr << "Error open DB: " << sqlite3_errmsg(this->_DB) << endl;
		return false;
	}
	cout << "Opened database successfully!" << endl;

    // Enable foreign keys
    exit = sqlite3_exec(this->_DB, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Failed to enable foreign keys: " << sqlite3_errmsg(this->_DB) << std::endl;
    }

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
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE" // "ON DELETE CASCADE" means that if i delete the user with the id that connected to this item, the item will be deleted forever. 
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
        "FOREIGN KEY(list_id) REFERENCES lists(id) ON DELETE CASCADE"
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
    return 0; // after checkUserPassword
}

int DB_manager::getTableIdByName(string email)
{
    return 0;
}
