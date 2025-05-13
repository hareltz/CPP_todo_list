#include <iostream>
#include "DB_manager.h"

int main()
{
    std::cout << "Hello World!\n";
    DB_manager db("db_name.db");

    if (db.createNewUser("hareltz912@gmail.com", "harel912txw") == codes::SUCCESS)
    {
        std::cout << "user created!" << std::endl;
    }

    //std::cout << db.checkUserPassword("hareltz912@gmail.com", "harel912txw") << std::endl;
    //std::cout << db.checkUserPassword("hareltz912@gmail.com", "harel912=txw") << std::endl;

    //if (db.deleteUser("hareltz912@gmail.com", "harel912dddtxw") == codes::SUCCESS) { std::cout << "good" << std::endl; }
    //if (db.deleteUser("hareltz912@gmail.com", "harel912txw") == codes::SUCCESS) { std::cout << "good" << std::endl; }

    std::cout << db.createNewTodoList("hareltz912@gmail.com", "apps") << std::endl;
    std::cout << db.createNewTodoList("hareltz912@gmail.com", "apps2") << std::endl;
    std::cout << db.createNewTodoList("hareltz912@gmail.com", "apps2") << std::endl;
}


