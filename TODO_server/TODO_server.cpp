#include <iostream>
#include "DB_manager.h"

int main()
{
    std::cout << "Hello World!\n";
    DB_manager& db = DB_manager::get_instance();

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

    vector<string> names = db.getLists("hareltz912@gmail.com");
    
    for (string name : names)
    {
        cout << name << endl;
    }
}


