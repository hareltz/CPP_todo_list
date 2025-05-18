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

    //std::cout << db.addList("hareltz912@gmail.com", "apps") << std::endl;
    //std::cout << db.addList("hareltz912@gmail.com", "apps2") << std::endl;
    //std::cout << db.addList("hareltz912@gmail.com", "apps2") << std::endl;


    //std::cout << db.addTask("hareltz912@gmail.com", "apps2", "task1") << std::endl;
    //std::cout << db.addTask("hareltz912@gmail.com", "apps2", "task2") << std::endl;
    //std::cout << db.addTask("hareltz912@gmail.com", "apps2", "task3") << std::endl;
    //std::cout << db.addTask("hareltz912@gmail.com", "apps2", "task4") << std::endl;

    std::cout << db.markTask("hareltz912@gmail.com", "apps2", "task4") << std::endl;
    std::cout << db.markTask("hareltz912@gmail.com", "apps2", "task2") << std::endl;
    std::cout << db.unmarkTask("hareltz912@gmail.com", "apps2", "task4") << std::endl;


    //vector<string> tasks = db.getTasks("hareltz912@gmail.com", "apps2");

    //for (string t : tasks)
    //{
    //    cout << t << endl;
    //}
}


