#include "DB_manager.h"

/// <summary>
/// c'tor
/// </summary>
DB_manager::DB_manager()
{
    if (createDbFiles("db.db")) { createTables(); }
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
        string errMsg = errorMessage ? errorMessage : "";

        if (code == SQLITE_CONSTRAINT && errMsg.find("users.email") != string::npos) {
            sqlite3_free(errorMessage);
            cerr << "SQLite error: " << errMsg << endl;
            return codes::USER_EXISTS;
        }

        cerr << "SQLite error: " << errMsg << endl;
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
        cerr << "Error deleting user: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        return codes::ERROR;
    }

    return codes::SUCCESS;
}

/// <summary>
/// this function add a new todo list
/// </summary>
/// <param name="email">the email of the user who want to add a list</param>
/// <param name="listName">the list name</param>
/// <returns>one of the codes</returns>
codes DB_manager::createNewTodoList(string email, string listName)
{
    int userId = -1;
    char* errorMessage;
    string sqlMsg = "";

    userId = getUserIdByEmail(email);
    if (userId == -1)
    {
        return codes::USER_NOT_EXISTS;
    }

    sqlMsg = "INSERT INTO lists (user_id, name) VALUES ('" + to_string(userId) + "', '" + listName + "');";

    // Execute the query and pass the callback
    if (sqlite3_exec(this->_DB, sqlMsg.c_str(), NULL, 0, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error adding list: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return codes::ALREADY_LIST_EXISTS;
    }

    return codes::SUCCESS;

}

codes DB_manager::addTask(string email, string listName, string task)
{
    // get all the lists that related to this user
    // find if a listName exist in the lists that we got
    // add the new task and connect it to the list

    // create: ->>>>>
    // getLists
    // getListIdByName
    // get tasks
    // delete tasks
    // maybe more, my brain is dead :(
    return codes(); 
}

codes DB_manager::markTask(string email, string listName, string task)
{
    // same as add task
    // but we will not add but update the task!
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
    string sql = "SELECT password FROM users WHERE email = '" + email + "';";

    // Function to execute the SQL query and fetch the password
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        // If email exists, compare the password (argv[0] contains the password from DB)
        if (argc > 0 && argv[0]) {
            string storedPassword = argv[0];
            string providedPassword = *(static_cast<string*>(data));

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
        cerr << "The passwords does not match!" << endl;
        sqlite3_free(errorMessage);
        return codes::ERROR;
    }

    return codes::SUCCESS;
}

vector<string> DB_manager::getLists(string email)
{
    int userId = getUserIdByEmail(email);
    char* errorMessage = nullptr;
    vector<string> names;

    string sqlMsg = "SELECT name FROM lists WHERE user_id = '" + to_string(userId) + "';";

    // Callback function to handle the query result
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        if (argc > 0 && argv[0]) {
            std::vector<std::string>* names = static_cast<std::vector<std::string>*>(data); // Pointer to names (of the function "getLists")
            names->emplace_back(argv[0]);  // Add the name to the vector

            cout << argv[0] << endl;

        }
        return 0;
        };

    // Execute the query and pass the callback
    if (sqlite3_exec(this->_DB, sqlMsg.c_str(), callback, &names, &errorMessage) != SQLITE_OK) {
        cerr << "Error retrieving names: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    return names;
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
        cerr << "Failed to enable foreign keys: " << sqlite3_errmsg(this->_DB) << endl;
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
        "FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE, " // "ON DELETE CASCADE" means that if i delete the user with the id that connected to this item, the item will be deleted forever. 
        "UNIQUE(user_id, name)"  // Ensure that the combination of user_id and name is unique
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

/// <summary>
/// this function get's the user id
/// </summary>
/// <param name="email">the user email</param>
/// <returns>the user id</returns>
int DB_manager::getUserIdByEmail(string email)
{
    char* errorMessage = nullptr;
    int userId = -1;  // Default to -1 if the user is not found

    string sqlMsg = "SELECT id FROM users WHERE email = '" + email + "';";

    // Callback function to handle the query result
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        if (argc > 0 && argv[0]) {
            int* userId = static_cast<int*>(data); // Pointer to userId (of the function "getUserIdByEmail")
            *userId = stoi(argv[0]);  // Set the user ID from the query result
        }
        return 0;
        };

    // Execute the query and pass the callback
    if (sqlite3_exec(this->_DB, sqlMsg.c_str(), callback, &userId, &errorMessage) != SQLITE_OK) {
        cerr << "Error retrieving user ID: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    return userId;

}

/// <summary>
/// this function get's the list id
/// </summary>
/// <param name="email">the user's email</param>
/// <param name="listName">the list name</param>
/// <returns>the list id</returns>
int DB_manager::getListIdByName(string email, string listName)
{
    int userId = getUserIdByEmail(email);
    char* errorMessage = nullptr;
    int listId = -1;  // Default to -1 if the user is not found

    string sqlMsg = "SELECT id FROM lists WHERE user_id = '" + to_string(userId) + "' AND name = '" + listName + "';";

    // Callback function to handle the query result
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        if (argc > 0 && argv[0]) {
            int* listId = static_cast<int*>(data); // Pointer to listId (of the function "getListIdByName")
            *listId = stoi(argv[0]);  // Set the user ID from the query result
        }
        return 0;
        };

    // Execute the query and pass the callback
    if (sqlite3_exec(this->_DB, sqlMsg.c_str(), callback, &listId, &errorMessage) != SQLITE_OK) {
        cerr << "Error retrieving list ID: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    return listId;
}
