#ifndef EDITACCOUNTDATABASE_H
#define EDITACCOUNTDATABASE_H
#include <string>

class editAccountDatabase {

public:
    static int testConnection();
    static int sendAccountDetails( std::string username, std::string password);
    static int checkAccountDetailsorCreateAccount(std::string username, std::string password, int choice);
};

#endif //EDITACCOUNTDATABASE_H
